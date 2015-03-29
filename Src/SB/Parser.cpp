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

#include "CX/SB/Parser.hpp"
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
	for (Size i = 0; i < RESERVED_KEYWORDS_COUNT; i++)
	{
		m_setKeywords.insert(RESERVED_KEYWORDS[i]);
	}
}

Status Parser::ReadMore()
{
	Size   cbAckSize;
	Status status;

	if (m_cbOffset < m_cbSize)
	{
		return Status();
	}
	if ((status = m_pInputStream->Read(m_buffer, sizeof(m_buffer), &cbAckSize)).IsNOK())
	{
		return status;
	}
	m_cbOffset = 0;
	m_cbSize   = cbAckSize;

	return Status();
}

bool Parser::IsEOF()
{
	ReadMore();

	return (m_cbOffset >= m_cbSize);
}

Byte Parser::Get()
{
	ReadMore();

	if (m_cbOffset >= m_cbSize)
	{
		return 0;
	}

	return m_buffer[m_cbOffset];
}

void Parser::Next()
{
	if ('\n' == m_buffer[m_cbOffset])
	{
		m_cLine++;
		m_cColumn = 1;
	}
	else
	{
		m_cColumn++;
	}
	m_cbOffset++;
}

Status Parser::Parse(const Char *szFilePath, Data &data)
{
	CX::IO::FileInputStream fis(szFilePath);

	if (!fis.IsOK())
	{
		return Status(Status_OpenFailed, "Failed to open '{1}'", szFilePath);
	}
	m_sPath = szFilePath;

	return Parse(&fis, data);
}

Status Parser::Parse(IO::IInputStream *pInputStream, Data &data)
{
	Status status;

	m_pInputStream = pInputStream;
	if ((status = Parse(data)).IsNOK())
	{
		m_sPath.clear();

		return status;
	}

	m_sPath.clear();

	return Status();
}

Status Parser::Parse(Data &data)
{
	String sStr;
	Status status;

	m_cbSize       = 0;
	m_cbOffset     = 0;
	data.setExternals.clear();
	data.setImports.clear();
	m_cLine        = 1;
	m_cColumn      = 1;
	while (!IsEOF())
	{
		if ((status = ParseWhiteSpace()).IsNOK())
		{
			return status;
		}
		if ((status = ParseIdentifier(sStr)).IsNOK())
		{
			return status;
		}
		if (sStr.empty())
		{
			if (IsEOF())
			{
				break;
			}
			else
			{
				return Status(Status_ParseFailed, "Expected 'import', 'external' or 'object' declaration at line {1}, column {2}",
				              m_cLine, m_cColumn);
			}
		}
		if (0 == cx_strcmp(sStr.c_str(), "import"))
		{
			if ((status = ParseImport(sStr)).IsNOK())
			{
				return status;
			}
			if (data.setImports.end() == data.setImports.find(sStr))
			{
				data.setImports.insert(sStr);
			}
		}
		else
		if (0 == cx_strcmp(sStr.c_str(), "external"))
		{
			if ((status = ParseExternal(sStr)).IsNOK())
			{
				return status;
			}
			if (data.setExternals.end() == data.setExternals.find(sStr))
			{
				data.setExternals.insert(sStr);
			}
		}
		else
		if (0 == cx_strcmp(sStr.c_str(), "object"))
		{
			Object obj;

			if ((status = ParseObject(obj)).IsNOK())
			{
				return status;
			}
			if (obj.m_sName.empty())
			{
				return Status(Status_ParseFailed, "Expected object name at line {1}, column {2}", m_cLine, m_cColumn);
			}
			if (m_setKeywords.end() != m_setKeywords.find(obj.m_sName))
			{
				return Status(Status_ParseFailed, "Invalid object name {1}", obj.m_sName);
			}
			if (data.mapObjects.end() != data.mapObjects.find(obj.m_sName))
			{
				return Status(Status_ParseFailed, "Duplicate object '{1}'", obj.m_sName);
			}

			data.mapObjects[obj.m_sName] = obj;
		}
		else
		{
			return Status(Status_ParseFailed, "Expected 'import', 'external' or 'object' declaration at line {1}, column {2}", 
			              m_cLine, m_cColumn - sStr.size());
		}
	}

	return Status();
}

Status Parser::ParseObject(Object &obj)
{
	Status status;

	obj.m_sName.clear();
	obj.m_vectorMembers.clear();
	if ((status = ParseWhiteSpace()).IsNOK())
	{
		return status;
	}
	if ((status = ParseScalarType(obj.m_sName)).IsNOK())
	{
		return status;
	}
	if ((status = ParseWhiteSpace()).IsNOK())
	{
		return status;
	}
	if (IsEOF())
	{
		return Status(Status_ParseFailed, "Unexpected EOF");
	}
	if ('{' != Get())
	{
		return Status(Status_ParseFailed, "Expected '{' at line {1}, column {2}", m_cLine, m_cColumn);
	}
	Next();
	while (!IsEOF())
	{
		if ((status = ParseWhiteSpace()).IsNOK())
		{
			return status;
		}
		if (IsEOF())
		{
			return Status(Status_ParseFailed, "Unexpected EOF");
		}
		if ('}' == Get())
		{
			Next();
			break;
		}

		Member member;

		if ((status = ParseMember(member)).IsNOK())
		{
			return status;
		}
		if (member.m_sName.empty())
		{
			return Status(Status_ParseFailed, "Expected member name at line {1}, column {2}", m_cLine, m_cColumn);
		}
		if (m_setKeywords.end() != m_setKeywords.find(member.m_sName))
		{
			return Status(Status_ParseFailed, "Invalid member name {1}", member.m_sName);
		}
		for (MembersVector::iterator iter = obj.m_vectorMembers.begin(); iter != obj.m_vectorMembers.end(); ++iter)
		{
			if (0 == cx_strcmp(iter->m_sName.c_str(), member.m_sName.c_str()))
			{
				return Status(Status_ParseFailed, "Duplicate member name {1}", member.m_sName);
			}
		}
		obj.m_vectorMembers.push_back(member);
	}

	return Status();
}

Status Parser::ParseMember(Member &member)
{
	String sStr1;
	String sStr2;
	String sStr3;
	Status status;

	member.m_sName.clear();
	member.m_sKeyType.clear();
	member.m_sValType.clear();
	member.m_nType = Member::Type_Scalar;

	if ((status = ParseWhiteSpace()).IsNOK())
	{
		return status;
	}
	if (IsEOF())
	{
		return Status(Status_ParseFailed, "Unexpected EOF");
	}
	if ((status = ParseIdentifier(member.m_sName)).IsNOK())
	{
		return status;
	}
	if ((status = ParseWhiteSpace()).IsNOK())
	{
		return status;
	}
	if (IsEOF())
	{
		return Status(Status_ParseFailed, "Unexpected EOF");
	}
	if (':' != Get())
	{
		return Status(Status_ParseFailed, "Expected ':' at line {1}, column {2}", m_cLine, m_cColumn);
	}
	Next();
	if ((status = ParseWhiteSpace()).IsNOK())
	{
		return status;
	}
	if (IsEOF())
	{
		return Status(Status_ParseFailed, "Unexpected EOF");
	}
	if ((status = ParseScalarType(sStr1)).IsNOK())
	{
		return status;
	}
	if (0 == cx_strcmp(sStr1.c_str(), "vector") || 0 == cx_strcmp(sStr1.c_str(), "set") || 
	    0 == cx_strcmp(sStr1.c_str(), "map") || 0 == cx_strcmp(sStr1.c_str(), "hashset") ||
	    0 == cx_strcmp(sStr1.c_str(), "hashmap"))
	{
		if ((status = ParseWhiteSpace()).IsNOK())
		{
			return status;
		}
		if (IsEOF())
		{
			return Status(Status_ParseFailed, "Unexpected EOF");
		}
		if ('<' != Get())
		{
			return Status(Status_ParseFailed, "Expected '<' at line {1}, column {2}", m_cLine, m_cColumn);
		}
		Next();
		if ((status = ParseWhiteSpace()).IsNOK())
		{
			return status;
		}
		if (IsEOF())
		{
			return Status(Status_ParseFailed, "Unexpected EOF");
		}
		if ((status = ParseScalarType(sStr2)).IsNOK())
		{
			return status;
		}
		if (0 == cx_strcmp(sStr1.c_str(), "vector"))
		{
			member.m_sValType = sStr2;
			member.m_nType    = Member::Type_Vector;
		}
		else
		if (0 == cx_strcmp(sStr1.c_str(), "set"))
		{
			member.m_sKeyType = sStr2;
			member.m_nType    = Member::Type_Set;
		}
		else
		if (0 == cx_strcmp(sStr1.c_str(), "hashset"))
		{
			member.m_sKeyType = sStr2;
			member.m_nType    = Member::Type_HashSet;
		}
		else
		{
			if ((status = ParseWhiteSpace()).IsNOK())
			{
				return status;
			}
			if (IsEOF())
			{
				return Status(Status_ParseFailed, "Unexpected EOF");
			}
			if (',' != Get())
			{
				return Status(Status_ParseFailed, "Expected ',' at line {1}, column {2}", m_cLine, m_cColumn);
			}
			Next();
			if ((status = ParseWhiteSpace()).IsNOK())
			{
				return status;
			}
			if (IsEOF())
			{
				return Status(Status_ParseFailed, "Unexpected EOF");
			}
			if ((status = ParseScalarType(sStr3)).IsNOK())
			{
				return status;
			}
			if (0 == cx_strcmp(sStr1.c_str(), "map"))
			{
				member.m_sKeyType = sStr2;
				member.m_sValType = sStr3;
				member.m_nType    = Member::Type_Map;
			}
			else
			if (0 == cx_strcmp(sStr1.c_str(), "hashmap"))
			{
				member.m_sKeyType = sStr2;
				member.m_sValType = sStr3;
				member.m_nType    = Member::Type_HashMap;
			}
		}
		if ((status = ParseWhiteSpace()).IsNOK())
		{
			return status;
		}
		if (IsEOF())
		{
			return Status(Status_ParseFailed, "Unexpected EOF");
		}
		if ('>' != Get())
		{
			return Status(Status_ParseFailed, "Expected '>' at line {1}, column {2}", m_cLine, m_cColumn);
		}
		Next();
	}
	else
	{
		member.m_sValType = sStr1;
		member.m_nType    = Member::Type_Scalar;
	}
	if ((status = ParseWhiteSpace()).IsNOK())
	{
		return status;
	}
	if (IsEOF())
	{
		return Status(Status_ParseFailed, "Unexpected EOF");
	}
	if (';' != Get())
	{
		return Status(Status_ParseFailed, "Expected ';' at line {1}, column {2}", m_cLine, m_cColumn);
	}
	Next();

	return Status();
}

Status Parser::ParseImport(String &sPath)
{
	Status status;

	sPath.clear();
	if ((status = ParseWhiteSpace()).IsNOK())
	{
		return status;
	}
	if (IsEOF())
	{
		return Status(Status_ParseFailed, "Unexpected EOF");
	}
	if ('"' != Get())
	{
		return Status(Status_ParseFailed, "Expected import path at line {1}, column {2}", m_cLine, m_cColumn);
	}
	Next();
	while (!IsEOF())
	{
		if ('"' == Get())
		{
			Next();
			break;
		}
		else
		if ('\n' == Get() || '\r' == Get() || '\t' == Get() || '\b' == Get() || '\f' == Get())
		{
			return Status(Status_ParseFailed, "Expected import path at line {1}, column {2}", m_cLine, m_cColumn);
		}
		else
		{
			sPath += (Char)Get();
			Next();
		}
	}
	if (sPath.empty())
	{
		return Status(Status_ParseFailed, "Expected import path at line {1}, column {2}", m_cLine, m_cColumn);
	}

	return Status();
}

Status Parser::ParseExternal(String &sType)
{
	Status status;

	sType.clear();
	if ((status = ParseWhiteSpace()).IsNOK())
	{
		return status;
	}
	if ((status = ParseScalarType(sType)).IsNOK())
	{
		return status;
	}
	if (sType.empty())
	{
		return Status(Status_ParseFailed, "Expected external type at line {1}, column {2}", m_cLine, m_cColumn);
	}
	if ((status = ParseWhiteSpace()).IsNOK())
	{
		return status;
	}
	if (IsEOF())
	{
		return Status(Status_ParseFailed, "Unexpected EOF");
	}
	if (';' != Get())
	{
		return Status(Status_ParseFailed, "Expected ';' at line {1}, column {2}", m_cLine, m_cColumn);
	}
	Next();

	return Status();
}

Status Parser::ParseScalarType(String &sType)
{
	Status status;

	sType.clear();
	if ((status = ParseWhiteSpace()).IsNOK())
	{
		return status;
	}
	if (IsEOF())
	{
		return Status();
	}
	while (!IsEOF())
	{
		String sStr;

		if ((status = ParseIdentifier(sStr)).IsNOK())
		{
			return status;
		}
		if (sStr.empty())
		{
			break;
		}
		sType += sStr;
		if ((status = ParseWhiteSpace()).IsNOK())
		{
			return status;
		}
		if (IsEOF())
		{
			break;
		}
		if ('.' == Get())
		{
			sType += ".";
			Next();
		}
		else
		{
			break;
		}
	}

	return Status();
}

Status Parser::ParseIdentifier(String &sIdentifier)
{
	sIdentifier.clear();
	while (!IsEOF())
	{
		if ('_' == Get() || cx_isalpha(Get()) || (!sIdentifier.empty() && cx_isdigit(Get())))
		{
			sIdentifier += (Char)Get();
		}
		else
		{
			break;
		}
		Next();
	}
	//check if empty identifier outside

	return Status();
}

Status Parser::ParseWhiteSpace()
{
	WSType nType = WSType_Normal;

	while (!IsEOF())
	{
		if (WSType_SLComment == nType)
		{
			if ('\n' == Get())
			{
				Next();
				nType = WSType_Normal;
			}
			else
			{
				Next();
				if (IsEOF())
				{
					return Status(Status_ParseFailed, "Unexpected EOF");
				}
				if ('/' == Get())
				{
					Next();
					nType = WSType_Normal;
				}
			}
		}
		else
		if (WSType_MLComment == nType)
		{
			if ('*' == Get())
			{
				Next();
			}
		}
		else
		{
			if ('/' == Get())
			{
				Next();
				if (IsEOF())
				{
					return Status(Status_ParseFailed, "Unexpected EOF");
				}
				if ('/' == Get())
				{
					Next();
					nType = WSType_SLComment;
				}
				else
				if ('*' == Get())
				{
					Next();
					nType = WSType_MLComment;
				}
				else
				{
					return Status(Status_ParseFailed, "Expected '/' at line {1}, column {2}", m_cLine, m_cColumn - 2);
				}
			}
			else
			if (cx_isspace(Get()))
			{
				Next();
			}
			else
			{
				break;
			}
		}
	}

	return Status();
}

}//namespace SB

}//namespace CX


