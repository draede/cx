/* 
 * CX - C++ framework for general purpose developement
 *
 * https://github.com/draede/cx
 * 
 * Copyright (C) 2014-2015 draede - draede [at] outlook [dot] com
 *
 * Released under the MIT License.
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */ 

#include "CX/IO/SimpleBuffers/ProtoParser.hpp"
#include "CX/IO/FileInputStream.hpp"
#include "CX/C/ctype.h"


namespace CX
{

namespace IO
{

namespace SimpleBuffers
{

const Char *ProtoParser::RESERVED_KEYWORDS[] = 
{
	"alignas", "alignof", "and", "and_eq", "asm", "auto", "bitand", "bitor", "bool", "break", 
	"case", "catch", "char", "char16_t", "char32_t", "class", "compl", "const", "constexpr", 
	"const_cast", "continue", "decltype", "default", "delete", "do", "double", "dynamic_cast", 
	"else", "enum", "explicit", "export", "extern", "false", "float", "for", "friend", "goto", "if", 
	"inline", "int", "long", "mutable", "namespace", "new", "noexcept", "not", "not_eq", "nullptr", 
	"operator", "or", "or_eq", "private", "protected", "public", "register", "reinterpret_cast", 
	"return", "short", "signed", "sizeof", "static", "static_assert", "static_cast", "struct", 
	"switch", "template", "this", "thread_local", "throw", "true", "try", "typedef", "typeid", 
	"typename", "union", "unsigned", "using", "virtual", "void", "volatile", "wchar_t", "while"
	"xor", "xor_eq", 
};

const CX_Size ProtoParser::RESERVED_KEYWORDS_COUNT = sizeof(ProtoParser::RESERVED_KEYWORDS) / sizeof(ProtoParser::RESERVED_KEYWORDS[0]);

ProtoParser::ProtoParser()
{
	m_nState = State_None;
}

Status ProtoParser::BeginParse()
{
	m_nState       = State_BeforeDeclaration;
	m_cLine        = 1;
	m_cColumn      = 1;
	m_cImportDepth = 1;
	m_sPath.clear();
	m_sNamespace.clear();
	m_object.m_sName.clear();
	m_object.m_vectorFields.clear();
	
	return Status();
}

Status ProtoParser::BeginParseInternal(Size cImportDepth)
{
	if (MAX_IMPORTS_DEPTH <= cImportDepth)
	{
		return Status(Status_InvalidCall, "Too many imports");
	}

	m_nState       = State_BeforeDeclaration;
	m_cLine        = 1;
	m_cColumn      = 1;
	m_cImportDepth = cImportDepth + 1;
	m_sPath.clear();
	m_sNamespace.clear();
	m_object.m_sName.clear();
	m_object.m_vectorFields.clear();

	return Status();
}

Status ProtoParser::EndParse()
{
	if (State_BeforeDeclaration != m_nState)
	{
		return Status(Status_InvalidCall, "Unexpected end");
	}

	for (ObjectsMap::iterator iterObjects = m_mapObjects.begin(); iterObjects != m_mapObjects.end(); iterObjects++)
	{
		for (FieldsVector::iterator iterFields = iterObjects->second.m_vectorFields.begin(); 
		     iterFields != iterObjects->second.m_vectorFields.end(); iterFields++)
		{
			if (Field::Type_Object == iterFields->m_nType)
			{
				if (0 == cx_stricmp(iterFields->m_sObjectName.c_str(), iterObjects->first.c_str()))
				{
					return Status(Status_ParseFailed, "Field '{1}'.'{2}' cannot be of type {3}", iterObjects->first, 
					              iterFields->m_sName, iterFields->m_sObjectName);
				}
				
				ObjectsMap::iterator iter = m_mapObjects.find(iterFields->m_sObjectName);

				if (m_mapObjects.end() == iter)
				{
					return Status(Status_ParseFailed, "Unknown object '{1}' used as '{2}'.'{3}' type", iterFields->m_sObjectName, 
					              iterObjects->first, iterFields->m_sName);
				}

				for (FieldsVector::iterator iterFields2 = iter->second.m_vectorFields.begin(); 
				     iterFields2 != iter->second.m_vectorFields.end(); iterFields2++)
				{
					if (Field::Type_Object == iterFields2->m_nType)
					{
						if (0 == cx_stricmp(iterFields2->m_sObjectName.c_str(), iterObjects->first.c_str()))
						{
							return Status(Status_ParseFailed, "Circular reference detected for objects '{1}' and '{2}'", 
							              iterFields2->m_sObjectName, iterObjects->first);
						}
					}
				}
			}
		}
	}

	m_nState = State_None;

	return Status();
}

Status ProtoParser::Parse(const Char *szFilePath)
{
	CX::IO::FileInputStream fis(szFilePath);

	if (State_None == m_nState)
	{
		return Status(Status_InvalidCall, "BeginParse was not called");
	}
	if (!fis.IsOK())
	{
		return Status(Status_OpenFailed, "Failed to open '{1}'", szFilePath);
	}
	m_sPath = szFilePath;

	return Parse(&fis);
}

Status ProtoParser::Parse(IO::IInputStream *pInputStream)
{
	Byte   buffer[8192];
	Size   cbSize;
	Status status;

	if (State_None == m_nState)
	{
		return Status(Status_InvalidCall, "BeginParse was not called");
	}
	while (!pInputStream->IsEOF())
	{
		if ((status = pInputStream->Read(buffer, sizeof(buffer), &cbSize)).IsNOK())
		{
			if (Status_EOF != status.GetCode())
			{
				return status;
			}
			else
			{
				break;
			}
		}
		else
		{
			if ((status = Parse(buffer, cbSize)).IsNOK())
			{
				return status;
			}
		}
	}

	return Status();
}

Status ProtoParser::Parse(const void *pBuffer, Size cbSize)
{
	Status status;

	if (State_None == m_nState)
	{
		return Status(Status_InvalidCall, "BeginParse was not called");
	}

	m_pPos = (const Char *)pBuffer;
	m_pEnd = m_pPos + cbSize;

	while (m_pPos < m_pEnd)
	{
		switch (m_nState)
		{
			case State_BeforeDeclaration:
			{
				if ((status = WalkWhiteSpaces()).IsNOK())
				{
					return status;
				}
				if (m_pPos < m_pEnd)
				{
					m_nState = State_InDeclaration;
					m_sToken.clear();
				}
			}
			break;
			case State_InDeclaration:
			{
				if ((status = WalkToken()).IsNOK())
				{
					return status;
				}
				if (m_pPos < m_pEnd)
				{
					if (0 == cx_strcmp(m_sToken.c_str(), "object"))
					{
						m_nState = State_BeforeObjectName;
						m_sToken.clear();
					}
					else
					if (0 == cx_strcmp(m_sToken.c_str(), "import"))
					{
						m_nState = State_BeforeImport;
						m_sToken.clear();
					}
					else
					if (0 == cx_strcmp(m_sToken.c_str(), "namespace"))
					{
						if (!m_sNamespace.empty())
						{
							return Status(Status_ParseFailed, "Namespace has already been declared");
						}
						m_nState = State_BeforeNamespace;
						m_sToken.clear();
					}
					else
					{
						return Status(Status_ParseFailed, "Expected 'object' or 'import' at line {1}, column {2}", 
						              m_cLine, m_cColumn - m_sToken.size());
					}
				}
			}
			break;
			case State_BeforeImport:
			{
				if ((status = WalkWhiteSpaces()).IsNOK())
				{
					return status;
				}
				if (m_pPos < m_pEnd)
				{
					if ('"' != *m_pPos)
					{
						return Status(Status_ParseFailed, "Expected '\"' at line {1}, column {2}", m_cLine, m_cColumn);
					}
					m_pPos++;
					m_nState = State_InImport;
					m_sToken.clear();
				}
			}
			break;
			case State_InImport:
			{
				while (m_pPos < m_pEnd && '"' != *m_pPos)
				{
					m_sToken += *m_pPos;
					m_pPos++;
				}
				if (m_pPos < m_pEnd)
				{
					m_pPos++;
					m_nState = State_AfterImport;
				}
			}
			break;
			case State_BeforeNamespace:
			{
				if ((status = WalkWhiteSpaces()).IsNOK())
				{
					return status;
				}
				if (m_pPos < m_pEnd)
				{
					m_nState = State_InNamespace;
					m_sToken.clear();
				}
			}
			break;
			case State_InNamespace:
			{
				while (m_pPos < m_pEnd && ('.' == *m_pPos || '_' == *m_pPos || cx_isalnum((Byte)*m_pPos)))
				{
					m_sToken += *m_pPos;
					m_pPos++;
				}
				if (m_pPos < m_pEnd)
				{
					m_pPos++;
					m_nState = State_AfterNamespace;
				}
			}
			break;
			case State_BeforeObjectName:
			{
				if ((status = WalkWhiteSpaces()).IsNOK())
				{
					return status;
				}
				if (m_pPos < m_pEnd)
				{
					m_nState = State_InObjectName;
					m_sToken.clear();
				}
			}
			break;
			case State_InObjectName:
			{
				if ((status = WalkToken()).IsNOK())
				{
					return status;
				}
				if (m_pPos < m_pEnd)
				{
					if (m_sToken.empty())
					{
						return Status(Status_ParseFailed, "Expected object name at line {1}, column {2}", m_cLine, m_cColumn);
					}
					m_nState = State_AfterObjectName;
				}
			}
			break;
			case State_BeforeObjectStart:
			{
				if ((status = WalkWhiteSpaces()).IsNOK())
				{
					return status;
				}
				if (m_pPos < m_pEnd)
				{
					if ('{' != *m_pPos)
					{
						return Status(Status_ParseFailed, "Expected '{' at line {1}, column {2}", m_cLine, m_cColumn);
					}
					m_pPos++;
					m_nState = State_BeforeName;
					m_sToken.clear();
				}
			}
			break;
			case State_BeforeName:
			{
				if ((status = WalkWhiteSpaces()).IsNOK())
				{
					return status;
				}
				if (m_pPos < m_pEnd)
				{
					if ('}' == *m_pPos)
					{
						m_pPos++;
						m_nState = State_AfterObject;
						m_sToken.clear();
					}
					else
					{
						m_nState = State_InName;
						m_sToken.clear();
					}
				}
			}
			break;
			case State_InName:
			{
				if ((status = WalkToken()).IsNOK())
				{
					return status;
				}
				if (m_pPos < m_pEnd)
				{
					if (m_sToken.empty())
					{
						return Status(Status_ParseFailed, "Expected field name at line {1}, column {2}", m_cLine, m_cColumn);
					}
					m_nState = State_AfterName;
				}
			}
			break;
			case State_BeforeColon:
			{
				if ((status = WalkWhiteSpaces()).IsNOK())
				{
					return status;
				}
				if (m_pPos < m_pEnd)
				{
					if (':' != *m_pPos)
					{
						return Status(Status_ParseFailed, "Expected ':' at line {1}, column {2}", m_cLine, m_cColumn);
					}
					m_pPos++;
					m_nState = State_BeforeType;
					m_sToken.clear();
				}
			}
			break;
			case State_BeforeType:
			{
				if ((status = WalkWhiteSpaces()).IsNOK())
				{
					return status;
				}
				if (m_pPos < m_pEnd)
				{
					m_nState   = State_InType;
					m_bIsArray = false;
					m_sToken.clear();
				}
			}
			break;
			case State_InType:
			{
				if ((status = WalkToken()).IsNOK())
				{
					return status;
				}
				if (m_pPos < m_pEnd)
				{
					if (m_sToken.empty())
					{
						return Status(Status_ParseFailed, "Expected field type at line {1}, column {2}", m_cLine, m_cColumn);
					}
					if ('.' == *m_pPos)
					{
						m_sToken += ".";
						m_pPos++;
					}
					else
					{
						m_nState = State_BeforeArrayBegin;
					}
				}
			}
			break;
			case State_BeforeArrayBegin:
			{
				if ((status = WalkWhiteSpaces()).IsNOK())
				{
					return status;
				}
				if (m_pPos < m_pEnd)
				{
					if ('[' == *m_pPos)
					{
						m_pPos++;
						m_nState = State_BeforeArrayEnd;
					}
					else
					{
						m_nState = State_BeforeSemicolon;
					}
				}
			}
			break;
			case State_BeforeArrayEnd:
			{
				if ((status = WalkWhiteSpaces()).IsNOK())
				{
					return status;
				}
				if (m_pPos < m_pEnd)
				{
					if (']' == *m_pPos)
					{
						m_pPos++;
						m_nState   = State_BeforeSemicolon;
						m_bIsArray = true;
					}
					else
					{
						return Status(Status_ParseFailed, "Expected ']' at line {1}, column {2}", m_cLine, m_cColumn);
					}
				}
			}
			break;
			case State_BeforeSemicolon:
			{
				if ((status = WalkWhiteSpaces()).IsNOK())
				{
					return status;
				}
				if (m_pPos < m_pEnd)
				{
					if (';' == *m_pPos)
					{
						m_pPos++;
						m_nState   = State_AfterType;
					}
					else
					{
						return Status(Status_ParseFailed, "Expected ';' at line {1}, column {2}", m_cLine, m_cColumn);
					}
				}
			}
			break;
			case State_BeforeComment:
			{
				if ((status = WalkWhiteSpaces()).IsNOK())
				{
					return status;
				}
			}
			break;
			case State_InSLComment:
			{
				if ((status = WalkWhiteSpaces()).IsNOK())
				{
					return status;
				}
			}
			break;
			case State_InMLComment:
			{
				if ((status = WalkWhiteSpaces()).IsNOK())
				{
					return status;
				}
			}
			break;
			case State_BeforeMLCommentEnd:
			{
				if ((status = WalkWhiteSpaces()).IsNOK())
				{
					return status;
				}
			}
			break;
			case State_AfterObjectName:
			{
				if (IsReservedKeyword(m_sToken.c_str()))
				{
					return Status(Status_ParseFailed, "Invalid object name. '{1}' is a C/C++ reserved keyword", m_sToken);
				}

				String sName;

				sName += m_sNamespace;
				if (!sName.empty())
				{
					sName += ".";
				}
				sName += m_sToken;

				if (m_mapObjects.end() != m_mapObjects.find(sName))
				{
					return Status(Status_ParseFailed, "Duplicate object name. '{1}' has already been declared", sName);
				}
				m_object.m_sName = sName;
				m_object.m_vectorFields.clear();
				m_nState = State_BeforeObjectStart;
				m_sToken.clear();
			}
			break;
			case State_AfterName:
			{
				if (IsReservedKeyword(m_sToken.c_str()))
				{
					return Status(Status_ParseFailed, "Invalid field name. '{1}' is a C/C++ reserved keyword", m_sToken);
				}

				for (FieldsVector::iterator iter = m_object.m_vectorFields.begin(); iter != m_object.m_vectorFields.end(); ++iter)
				{
					if (0 == cx_strcmp(iter->m_sName.c_str(),  m_sToken.c_str()))
					{
						return Status(Status_ParseFailed, "Duplicate field name. '{1}' has already been declared", m_sToken);
					}
				}

				m_field.m_sName = m_sToken;
				m_nState = State_BeforeColon;
				m_sToken.clear();
			}
			break;
			case State_AfterType:
			{
				m_field.m_nType     = Field::GetTypeFromProtoString(m_sToken.c_str());
				m_field.m_bIsVector = m_bIsArray;
				if (Field::Type_Object == m_field.m_nType)
				{
					m_field.m_sObjectName = m_sToken;
				}
				m_object.m_vectorFields.push_back(m_field);
				m_sToken.clear();
				m_nState = State_BeforeName;
			}
			break;
			case State_AfterObject:
			{
				m_mapObjects[m_object.m_sName] = m_object;
				m_nState = State_BeforeDeclaration;
			}
			break;
			case State_AfterImport:
			{
				String sPath;
				ProtoParser parser;
				
				GetFilePath(m_sPath.c_str(), m_sToken.c_str(), &sPath);
				if ((status = parser.BeginParseInternal(m_cImportDepth)).IsNOK())
				{
					return status;
				}
				if ((status = parser.Parse(sPath.c_str())).IsNOK())
				{
					return status;
				}
				if ((status = parser.EndParse()).IsNOK())
				{
					return status;
				}
				
				for (ObjectsMap::const_iterator iter = parser.GetObjects().begin(); iter != parser.GetObjects().end(); iter++)
				{
					if (m_mapObjects.end() == m_mapObjects.find(iter->first))
					{
						m_mapObjects[iter->first] = iter->second;
					}
				}

				m_nState = State_BeforeDeclaration;
				m_sToken.clear();
			}
			break;
			case State_AfterNamespace:
			{
				String     sToken;
				const Char *pPos;
				
				pPos = m_sToken.c_str();
				while (0 != *pPos)
				{
					if ('.' == *pPos)
					{
						if (sToken.empty())
						{
							return Status(Status_ParseFailed, "Invalid namespace");
						}
						if (!m_sNamespace.empty())
						{
							m_sNamespace += ".";
						}
						if (IsReservedKeyword(sToken.c_str()))
						{
							return Status(Status_ParseFailed, "Invalid namespace part. '{1}' is a C/C++ reserved keyword", sToken);
						}
						m_sNamespace += sToken;
						sToken.clear();
					}
					else
					{
						sToken += *pPos;
					}
					pPos++;
				}
				if (!sToken.empty())
				{
					if (!m_sNamespace.empty())
					{
						m_sNamespace += ".";
					}
					if (IsReservedKeyword(sToken.c_str()))
					{
						return Status(Status_ParseFailed, "Invalid namespace part. '{1}' is a C/C++ reserved keyword", sToken);
					}
					m_sNamespace += sToken;
				}
				m_nState = State_BeforeDeclaration;
				m_sToken.clear();
			}
			break;
		}
	}

	return Status();
}

Status ProtoParser::WalkWhiteSpaces()
{
	while (m_pPos < m_pEnd)
	{
		if (State_BeforeComment == m_nState)
		{
			if ('/' == *m_pPos)
			{
				m_pPos++;
				m_cColumn++;
				m_nState      = State_InSLComment;
			}
			else
			if ('*' == *m_pPos)
			{
				m_pPos++;
				m_cColumn++;
				m_nState      = State_InMLComment;
			}
			else
			{
				return Status(Status_ParseFailed, "Expected comment at line {1}, column {2}", m_cLine, m_cColumn - 1);
			}
		}
		else
		if (State_InSLComment == m_nState)
		{
			if ('\n' == *m_pPos)
			{
				m_pPos++;
				m_cLine++;
				m_cColumn = 1;
				m_nState = m_nSavedState;
			}
			else
			{
				m_pPos++;
				m_cColumn++;
			}
		}
		else
		if (State_InMLComment == m_nState)
		{
			if ('*' == *m_pPos)
			{
				m_pPos++;
				m_cColumn++;
				m_nState = State_BeforeMLCommentEnd;
			}
			else
			{
				m_pPos++;
				m_cColumn++;
			}
		}
		else
		if (State_BeforeMLCommentEnd == m_nState)
		{
			if ('/' == *m_pPos)
			{
				m_pPos++;
				m_cColumn++;
				m_nState = m_nSavedState;
			}
			else
			{
				m_pPos++;
			}
		}
		else
		if (cx_isspace((Byte)*m_pPos))
		{
			if ('\n' == *m_pPos)
			{
				m_pPos++;
				m_cLine++;
				m_cColumn = 1;
			}
			else
			{
				m_pPos++;
				m_cColumn++;
			}
		}
		else
		if ('/' == *m_pPos)
		{
			m_pPos++;
			m_cColumn++;
			m_nSavedState = m_nState;
			m_nState      = State_BeforeComment;
		}
		else
		{
			break;
		}
	}

	return Status();
}

Status ProtoParser::WalkToken()
{
	while (m_pPos < m_pEnd && 
	       ((m_sToken.empty() && ('_' == *m_pPos || cx_isalpha((Byte)*m_pPos))) ||
	       (!m_sToken.empty() && ('_' == *m_pPos || cx_isalnum((Byte)*m_pPos)))))
	{
		m_sToken += *m_pPos;
		m_pPos++;
	}

	return Status();
}

bool ProtoParser::IsReservedKeyword(const Char *szStr)
{
	for (Size i = 0; i < RESERVED_KEYWORDS_COUNT; i++)
	{
		if (0 == cx_strcmp(szStr, RESERVED_KEYWORDS[i]))
		{
			return true;
		}
	}

	return false;
}

void ProtoParser::GetFilePath(const Char *szRefPath, const Char *szOrigPath, String *psPath)
{
	Vector<String>::Type   vectorPath;
	const Char             *pPos;
	const Char             *pStartPos;

	pPos = pStartPos = szRefPath;
	while (0 != *pPos)
	{
		if ('/' == *pPos || '\\' == *pPos)
		{
			String s(pStartPos, pPos - pStartPos);

			if (0 == cx_strcmp(s.c_str(), "."))
			{
				//nothing
			}
			else
			if (0 == cx_strcmp(s.c_str(), ".."))
			{
				if (0 == vectorPath.size())
				{
					*psPath = szOrigPath;

					return;
				}
				else
				{
					vectorPath.erase(vectorPath.begin() + vectorPath.size() - 1);
				}
			}
			else
			{
				vectorPath.push_back(s);
			}
			pPos++;
			pStartPos = pPos;
		}
		else
		{
			pPos++;
		}
	}
	if (pPos > pStartPos)
	{
		String s(pStartPos, pPos - pStartPos);

		if (0 == cx_strcmp(s.c_str(), "."))
		{
			*psPath = szOrigPath;

			return;
		}
		else
		if (0 == cx_strcmp(s.c_str(), ".."))
		{
			*psPath = szOrigPath;

			return;
		}
		else
		{
			vectorPath.push_back(s);
		}
	}
	if (1 >= vectorPath.size())
	{
		*psPath = szOrigPath;

		return;
	}
	vectorPath.erase(vectorPath.begin() + vectorPath.size() - 1);

	pPos = pStartPos = szOrigPath;
	while (0 != *pPos)
	{
		if ('/' == *pPos || '\\' == *pPos)
		{
			String s(pStartPos, pPos - pStartPos);

			if (0 == cx_strcmp(s.c_str(), "."))
			{
				//nothing
			}
			else
			if (0 == cx_strcmp(s.c_str(), ".."))
			{
				if (0 == vectorPath.size())
				{
					*psPath = szOrigPath;

					return;
				}
				else
				{
					vectorPath.erase(vectorPath.begin() + vectorPath.size() - 1);
				}
			}
			else
			{
				vectorPath.push_back(s);
			}
			pPos++;
			pStartPos = pPos;
		}
		else
		{
			pPos++;
		}
	}
	if (pPos > pStartPos)
	{
		String s(pStartPos, pPos - pStartPos);

		if (0 == cx_strcmp(s.c_str(), "."))
		{
			*psPath = szOrigPath;

			return;
		}
		else
		if (0 == cx_strcmp(s.c_str(), ".."))
		{
			*psPath = szOrigPath;

			return;
		}
		else
		{
			vectorPath.push_back(s);
		}
	}

	*psPath = "";

	for (Vector<String>::Type::iterator iter = vectorPath.begin(); iter != vectorPath.end(); ++iter)
	{
		if (vectorPath.begin() != iter)
		{
			*psPath += "\\";
		}
		*psPath += *iter;
	}
}

}//namespace SimpleBuffers

}//namespace IO

}//namespace CX


