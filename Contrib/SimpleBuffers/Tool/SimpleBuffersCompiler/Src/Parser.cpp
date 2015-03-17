
#include "Parser.hpp"
#include "CX/IO/FileInputStream.hpp"
#include "CX/C/ctype.h"


namespace CX
{

namespace SB
{

const Char *Parser::RESERVED_KEYWORDS[] = 
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

const CX_Size Parser::RESERVED_KEYWORDS_COUNT = sizeof(Parser::RESERVED_KEYWORDS) / sizeof(Parser::RESERVED_KEYWORDS[0]);

Parser::Parser()
{
	m_nState = State_None;
}

Status Parser::BeginParse()
{
	m_nState       = State_BeforeDeclaration;
	m_cLine        = 1;
	m_cColumn      = 1;
	m_cImportDepth = 1;
	m_sPath.clear();

	return Status();
}

Status Parser::BeginParseInternal(Size cImportDepth)
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

	return Status();
}

Status Parser::EndParse()
{
	if (State_BeforeDeclaration != m_nState)
	{
		return Status(Status_InvalidCall, "Unexpected end");
	}

	for (StructsMap::iterator iterStructs = m_mapStructs.begin(); iterStructs != m_mapStructs.end(); iterStructs++)
	{
		for (FieldsVector::iterator iterFields = iterStructs->second.m_vectorFields.begin(); iterFields != iterStructs->second.m_vectorFields.end(); 
		     iterFields++)
		{
			if (Field::Type_Struct == iterFields->m_nType)
			{
				if (0 == cx_stricmp(iterFields->m_sStructName.c_str(), iterStructs->first.c_str()))
				{
					return Status(Status_ParseFailed, "Field '{1}'.'{2}' cannot be of type {3}", iterStructs->first, iterFields->m_sName,
					              iterFields->m_sStructName);
				}

				StructsMap::iterator iter = m_mapStructs.find(iterFields->m_sStructName);

				if (m_mapStructs.end() == iter)
				{
					return Status(Status_ParseFailed, "Unknown struct '{1}' used as '{2}'.'{3}' type", iterFields->m_sStructName, iterStructs->first, 
					              iterFields->m_sName);
				}

				for (FieldsVector::iterator iterFields2 = iter->second.m_vectorFields.begin(); iterFields2 != iter->second.m_vectorFields.end(); 
				     iterFields2++)
				{
					if (Field::Type_Struct == iterFields2->m_nType)
					{
						if (0 == cx_stricmp(iterFields2->m_sStructName.c_str(), iterStructs->first.c_str()))
						{
							return Status(Status_ParseFailed, "Circular reference detector for structs '{1}' and '{2}'", iterFields2->m_sStructName, 
							              iterStructs->first);
						}
					}
				}
			}
		}
	}

	m_nState = State_None;

	return Status();
}

Status Parser::Parse(const Char *szFilePath)
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

Status Parser::Parse(IO::IInputStream *pInputStream)
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

Status Parser::Parse(const void *pBuffer, Size cbSize)
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
					if (0 == cx_strcmp(m_sToken.c_str(), "struct"))
					{
						m_nState = State_BeforeStructName;
						m_sToken.clear();
					}
					else
					if (0 == cx_strcmp(m_sToken.c_str(), "import"))
					{
						m_nState = State_BeforeImport;
						m_sToken.clear();
					}
					else
					{
						return Status(Status_ParseFailed, "Expected 'struct' or 'import' at line {1}, column {2}", m_cLine, m_cColumn - m_sToken.size());
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
			case State_BeforeStructName:
			{
				if ((status = WalkWhiteSpaces()).IsNOK())
				{
					return status;
				}
				if (m_pPos < m_pEnd)
				{
					m_nState = State_InStructName;
					m_sToken.clear();
				}
			}
			break;
			case State_InStructName:
			{
				if ((status = WalkToken()).IsNOK())
				{
					return status;
				}
				if (m_pPos < m_pEnd)
				{
					if (m_sToken.empty())
					{
						return Status(Status_ParseFailed, "Expected struct name at line {1}, column {2}", m_cLine, m_cColumn);
					}
					m_nState = State_AfterStructName;
				}
			}
			break;
			case State_BeforeStructStart:
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
						m_nState = State_AfterStruct;
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
					m_nState = State_BeforeArrayBegin;
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
			case State_AfterStructName:
			{
				if (IsReservedKeyword(m_sToken.c_str()))
				{
					return Status(Status_ParseFailed, "Invalid struct name. '{1}' is a C/C++ reserved keyword", m_sToken);
				}
				if (m_mapStructs.end() != m_mapStructs.find(m_sToken))
				{
					return Status(Status_ParseFailed, "Duplicate struct name. '{1}' has already been declared", m_sToken);
				}
				m_struct.m_sName = m_sToken;
				m_struct.m_vectorFields.clear();
				m_nState = State_BeforeStructStart;
				m_sToken.clear();
			}
			break;
			case State_AfterName:
			{
				if (IsReservedKeyword(m_sToken.c_str()))
				{
					return Status(Status_ParseFailed, "Invalid field name. '{1}' is a C/C++ reserved keyword", m_sToken);
				}

				for (FieldsVector::iterator iter = m_struct.m_vectorFields.begin(); iter != m_struct.m_vectorFields.end(); ++iter)
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
				m_field.m_nType     = Field::GetTypeFromString(m_sToken.c_str());
				m_field.m_bIsVector = m_bIsArray;
				if (Field::Type_Struct == m_field.m_nType)
				{
					m_field.m_sStructName = m_sToken;
				}
				m_struct.m_vectorFields.push_back(m_field);
				m_sToken.clear();
				m_nState = State_BeforeName;
			}
			break;
			case State_AfterStruct:
			{
				m_mapStructs[m_struct.m_sName] = m_struct;
				m_nState = State_BeforeDeclaration;
			}
			break;
			case State_AfterImport:
			{
				String sPath;
				Parser parser;
				
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
				
				for (StructsMap::const_iterator iter = parser.GetStructs().begin(); iter != parser.GetStructs().end(); iter++)
				{
					if (m_mapStructs.end() == m_mapStructs.find(iter->first))
					{
						m_mapStructs[iter->first] = iter->second;
					}
				}

				m_nState = State_BeforeDeclaration;
				m_sToken.clear();
			}
			break;
		}
	}

	return Status();
}

Status Parser::WalkWhiteSpaces()
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

Status Parser::WalkToken()
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

bool Parser::IsReservedKeyword(const Char *szStr)
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

void Parser::GetFilePath(const Char *szRefPath, const Char *szOrigPath, String *psPath)
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

}//namespace SB

}//namespace CX
