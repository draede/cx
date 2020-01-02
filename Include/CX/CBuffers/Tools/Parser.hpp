/* 
 * CX - C++ framework for general purpose development
 *
 * https://github.com/draede/cx
 * 
 * Copyright (C) 2014 - 2020 draede - draede [at] outlook [dot] com
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

#pragma once


#include "CX/Types.hpp"
#include "CX/Status.hpp"
#include "CX/String.hpp"
#include "CX/Set.hpp"
#include "CX/C/ctype.h"
#include "CX/IO/FileInputStream.hpp"
#include "CX/CBuffers/Tools/IStream.hpp"
#include "CX/CBuffers/Tools/InputStreamStream.hpp"
#include "CX/CBuffers/Tools/ParserData.hpp"


namespace CX
{

namespace CBuffers
{

namespace Tools
{

class Parser
{
public:

	static inline Status Parse(const Char *szPath, StructsMap &mapStructs)
	{
		IO::FileInputStream is(szPath);

		if (!is.IsOK())
		{
			return Status(Status_OpenFailed, "Failed to open file '{1}'", szPath);
		}

		InputStreamStream stream(&is);

		Context   ctx;

		ctx.pStream  = &stream;
		ctx.cLine    = 1;
		ctx.cColumn  = 1;

		const Char *pszPos;

		if (NULL == (pszPos = cx_strrchr(szPath, '\\')))
		{
			pszPos = cx_strrchr(szPath, '/');
		}
		if (NULL == pszPos)
		{
			ctx.sRelPath = "";
		}
		else
		{
			ctx.sRelPath.assign(szPath, pszPos - szPath);
		}

		StringsSet setParsedFiles;

		return Parse(ctx, mapStructs, setParsedFiles);
	}

	static inline Status Parse(IStream *pStream, StructsMap &mapStructs)
	{
		Context   ctx;

		ctx.pStream  = pStream;
		ctx.cLine    = 1;
		ctx.cColumn  = 1;
		ctx.sRelPath = "";

		StringsSet setParsedFiles;

		return Parse(ctx, mapStructs, setParsedFiles);
	}

private:

	struct Context
	{
		IStream   *pStream;
		Size      cLine;
		Size      cColumn;
		String    sRelPath;
	};

	typedef Set<String>::Type   StringsSet;

	Parser()
	{
	}

	~Parser()
	{
	}

	static inline Status Parse(const Char *szPath, StructsMap &mapStructs, StringsSet &setParsedFiles)
	{
		IO::FileInputStream is(szPath);

		if (!is.IsOK())
		{
			return Status(Status_OpenFailed, "Failed to open file '{1}'", szPath);
		}

		InputStreamStream stream(&is);

		Context   ctx;

		ctx.pStream = &stream;
		ctx.cLine = 1;
		ctx.cColumn = 1;

		const Char *pszPos;

		if (NULL == (pszPos = cx_strrchr(szPath, '\\')))
		{
			pszPos = cx_strrchr(szPath, '/');
		}
		if (NULL == pszPos)
		{
			ctx.sRelPath = "";
		}
		else
		{
			ctx.sRelPath.assign(szPath, pszPos - szPath);
		}

		return Parse(ctx, mapStructs, setParsedFiles);
	}

	static inline Status Parse(Context &ctx, StructsMap &mapStructs, StringsSet &setParsedFiles)
	{
		String   sToken;
		Status   status;

		for (;;)
		{
			if (!(status = ParseWhiteSpace(ctx)))
			{
				return status;
			}
			if (!(status = ParseIdentifier(ctx, sToken)))
			{
				if (Status_EOF == status.GetCode())
				{
					break;
				}

				return status;
			}
#pragma warning(push)
#pragma warning(disable: 4996)
			if (0 == cx_strcmp(sToken.c_str(), "include"))
#pragma warning(pop)
			{
				if (!(status = ParseInclude(ctx, mapStructs, setParsedFiles)))
				{
					return status;
				}
			}
			else
#pragma warning(push)
#pragma warning(disable: 4996)
			if (0 == cx_strcmp(sToken.c_str(), "struct"))
#pragma warning(pop)
			{
				if (!(status = ParseStruct(ctx, mapStructs)))
				{
					return status;
				}
			}
			else
			{
				return Status(Status_ParseFailed, "Unexpected identifier '{1}' at line {2}, column {2}", sToken, 
				              ctx.cLine, ctx.cColumn);
			}
		}

		return Status();
	}

	static inline Status ParseInclude(Context &ctx, StructsMap &mapStructs, StringsSet &setParsedFiles)
	{
		String sPath;
		Status status;

		if (!(status = ParseWhiteSpace(ctx)))
		{
			return status;
		}
		if (!(status = ParseString(ctx, sPath)))
		{
			return status;
		}
		if (sPath.empty())
		{
			return Status(Status_ParseFailed, "Invalid include at line {1}, column {2}", ctx.cLine, ctx.cColumn);
		}
		if (!(status = ParseWhiteSpace(ctx)))
		{
			return status;
		}
		if (!ctx.pStream->HasMore())
		{
			return Status(Status_ParseFailed, "Unexpected EOF");
		}
		if ((Byte)';' != ctx.pStream->Peek())
		{
			return Status(Status_ParseFailed, "Expected ';' at line {1}, column {2}", ctx.cLine, ctx.cColumn);
		}
		ctx.pStream->Next();
		
		String sFullPath;

		if ('\\' != *sPath.begin() && '/' != *sPath.begin() && NULL == cx_strchr(sPath.c_str(), ':'))
		{
			if (!ctx.sRelPath.empty())
			{
				sFullPath = ctx.sRelPath;
				sFullPath += "\\";
			}
			sFullPath += sPath;
		}
		else
		{
			sFullPath = sPath;
		}
		if (setParsedFiles.end() != setParsedFiles.find(sFullPath))
		{
			return Status();
		}

		return Parse(sFullPath.c_str(), mapStructs, setParsedFiles);
	}

	static inline Status ParseStruct(Context &ctx, StructsMap &mapStructs)
	{
		Struct       s;
		StringsSet   setMembers;
		Size         cLine   = ctx.cLine;
		Size         cColumn = ctx.cColumn;
		Bool         bReady  = False;
		Status       status;

		s.cbSize = 0;
		for (;;)
		{
			if (!(status = ParseWhiteSpace(ctx)))
			{
				return status;
			}
			if (!(status = ParseType(ctx, s.sName)))
			{
				return status;
			}
			if (!(status = ParseWhiteSpace(ctx)))
			{
				return status;
			}
			if (!ctx.pStream->HasMore())
			{
				return Status(Status_ParseFailed, "Unexpected EOF");
			}
			if ((Byte)'{' != ctx.pStream->Peek())
			{
				return Status(Status_ParseFailed, "Expected '{{' at line {1}, column {2}", ctx.cLine, ctx.cColumn);
			}
			ctx.pStream->Next();
			if (!(status = ParseWhiteSpace(ctx)))
			{
				return status;
			}

			//parse members
			for (;;)
			{
				Member m;

				if (!(status = ParseMember(ctx, m)))
				{
					return status;
				}
				if ((Byte)'}' == ctx.pStream->Peek())
				{
					ctx.pStream->Next();
					if (!(status = ParseWhiteSpace(ctx)))
					{
						return status;
					}
					if (!ctx.pStream->HasMore())
					{
						return Status(Status_ParseFailed, "Unexpected EOF");
					}
					if ((Byte)';' != ctx.pStream->Peek())
					{
						return Status(Status_ParseFailed, "Expected ';' at line {1}, column {2}", ctx.cLine, ctx.cColumn);
					}
					ctx.pStream->Next();
					bReady = True;

					break;
				}
				else
				{
					if (setMembers.end() != setMembers.find(m.sName))
					{
						return Status(Status_ParseFailed, "Duplicate member found at line {1}, column {2}", 
						              ctx.cLine, ctx.cColumn);
					}
					setMembers.insert(m.sName);
					s.vectorMembers.push_back(m);
				}
			}

			if (s.vectorMembers.empty())
			{
				return Status(Status_ParseFailed, "Empty struct found at line {1}, column {2}", cLine, cColumn);
			}

			auto iter = mapStructs.find(s.sName);

			if (mapStructs.end() == iter)
			{
				mapStructs[s.sName] = s;
			}
			else
			{
				if (!s.Compare(iter->second))
				{
					return Status(Status_ParseFailed,
					              "Second definition found for struct '{1}' found at line {2}, column {3}",
					              s.sName, cLine, cColumn);
				}
			}

			if (bReady)
			{
				break;
			}
			if (!(status = ParseWhiteSpace(ctx)))
			{
				return status;
			}
			if (!ctx.pStream->HasMore())
			{
				break;
			}
		}

		return Status();
	}

	static inline Status ParseMember(Context &ctx, Member &m)
	{
		UInt64 nNumber;
		Status status;

		m.cbOffset = 0;
		m.cbSize   = 0;
		if (!(status = ParseWhiteSpace(ctx)))
		{
			return status;
		}
		if (!ctx.pStream->HasMore())
		{
			return Status(Status_ParseFailed, "Unexpected EOF");
		}
		if ((Byte)'}' == ctx.pStream->Peek())
		{
			return Status();
		}
		if (!(status = ParseType(ctx, m.sTypeName)))
		{
			return status;
		}
		if (!(status = GetTypeFromString(m.sTypeName, m)))
		{
			return status;
		}
		if (!(status = ParseWhiteSpace(ctx)))
		{
			return status;
		}
		if (!ctx.pStream->HasMore())
		{
			return Status(Status_ParseFailed, "Unexpected EOF");
		}
		if ((Byte)'[' == ctx.pStream->Peek())
		{
			ctx.pStream->Next();

			if (!(status = ParseWhiteSpace(ctx)))
			{
				return status;
			}
			if (!ctx.pStream->HasMore())
			{
				return Status(Status_ParseFailed, "Unexpected EOF");
			}
			if ((Byte)']' == ctx.pStream->Peek())
			{
				m.cItemsCount = 0;
				m.nAggregateType = Aggregate_Array;
				ctx.pStream->Next();
			}
			else
			{
				if (!(status = ParseNumber(ctx, nNumber)))
				{
					return status;
				}
				if (0 == nNumber || Struct::MAX_STATIC_ARRAY_COUNT < nNumber)
				{
					return Status(Status_ParseFailed, "Invalid array size <{1}> at line {2}, column {3}", nNumber,
					              ctx.cLine, ctx.cColumn);
				}
				if (!(status = ParseWhiteSpace(ctx)))
				{
					return status;
				}
				if (!ctx.pStream->HasMore())
				{
					return Status(Status_ParseFailed, "Unexpected EOF");
				}
				if ((Byte)']' == ctx.pStream->Peek())
				{
					m.cItemsCount = (Size)nNumber;
					m.nAggregateType = Aggregate_StaticArray;
					ctx.pStream->Next();
				}
				else
				{
					return Status(Status_ParseFailed, "Expected '{' at line {1}, column {2}", ctx.cLine, ctx.cColumn);
				}
			}
			if (!(status = ParseWhiteSpace(ctx)))
			{
				return status;
			}
		}
		else
		{
			m.cItemsCount    = 0;
			m.nAggregateType = Aggregate_Scalar;
		}
		if (!(status = ParseIdentifier(ctx, m.sName)))
		{
			return status;
		}
		if (!(status = ParseWhiteSpace(ctx)))
		{
			return status;
		}
		if (!ctx.pStream->HasMore())
		{
			return Status(Status_ParseFailed, "Unexpected EOF");
		}
		if ((Byte)';' != ctx.pStream->Peek())
		{
			return Status(Status_ParseFailed, "Expected ';' at line {1}, column {2}", ctx.cLine, ctx.cColumn);
		}
		
		if (0 == cx_strcmp(m.sTypeName.c_str(), "string"))
		{
			if (Aggregate_Scalar != m.nAggregateType)
			{
				return Status(Status_ParseFailed, "string type already is an array (member '{1}')", m.sName);
			}
			m.nType           = Type_Char;
			m.sActualTypeName = "CX_Char";
			m.nAggregateType  = Aggregate_Array;
		}
		else
		if (0 == cx_strcmp(m.sTypeName.c_str(), "wstring"))
		{
			if (Aggregate_Scalar != m.nAggregateType)
			{
				return Status(Status_ParseFailed, "wstring type already is an array (member '{1}')", m.sName);
			}
			m.nType           = Type_WChar;
			m.sActualTypeName = "CX_WChar";
			m.nAggregateType  = Aggregate_Array;
		}
		else
		if (0 == cx_strcmp(m.sTypeName.c_str(), "blob"))
		{
			if (Aggregate_Scalar != m.nAggregateType)
			{
				return Status(Status_ParseFailed, "blob type already is an array (member '{1}')", m.sName);
			}
			m.nType           = Type_UInt8;
			m.sActualTypeName = "CX_UInt8";
			m.nAggregateType  = Aggregate_Array;
		}

		ctx.pStream->Next();

		return Status();
	}

	static inline Status ParseWhiteSpace(Context &ctx)
	{
		Byte ch;

		for (;;)
		{
			if (!ctx.pStream->HasMore())
			{
				break;
			}
			ch = ctx.pStream->Peek();
			if (cx_isspace(ch))
			{
				if ((Byte)'\n' == ch)
				{
					ctx.cLine++;
					ctx.cColumn = 1;
				}
				else
				{
					ctx.cColumn++;
				}
				ctx.pStream->Next();
			}
			else
			if ('/' == ch)
			{
				ctx.cColumn++;
				ctx.pStream->Next();
				ch = ctx.pStream->Peek();
				if ('/' == ch)
				{
					ctx.pStream->Next();
					for (;;)
					{
						ch = ctx.pStream->Peek();
						if ('\n' == ch)
						{
							ctx.pStream->Next();
							ctx.cLine++;
							ctx.cColumn = 1;

							break;
						}
						else
						{
							ctx.pStream->Next();
						}
					}
				}
				else
				if ('*' == ch)
				{
					ctx.pStream->Next();
					for (;;)
					{
						ch = ctx.pStream->Peek();
						if ('\n' == ch)
						{
							ctx.pStream->Next();
							ctx.cLine++;
							ctx.cColumn = 1;
						}
						else
						if ('*' == ch)
						{
							ctx.pStream->Next();
							ch = ctx.pStream->Peek();
							if ('/' == ch)
							{
								ctx.pStream->Next();

								break;
							}
							else
							{
								ctx.pStream->Next();
							}
						}
						else
						{
							ctx.pStream->Next();
						}
					}
				}
				else
				{
					return Status(Status_ParseFailed, "Unexpected '/' at line {1}, column {2}", ctx.cLine, ctx.cColumn);
				}
			}
			else
			{
				break;
			}
		}

		return Status();
	}

	static inline Status ParseIdentifier(Context &ctx, String &sToken)
	{
		Byte ch;

		sToken.clear();
		for (;;)
		{
			if (!ctx.pStream->HasMore())
			{
				break;
			}
			ch = ctx.pStream->Peek();
			if ('_' == ch || cx_isalpha(ch) || (!sToken.empty() && (cx_isdigit(ch))))
			{
				sToken += (Char)ch;
				ctx.pStream->Next();
			}
			else
			{
				break;
			}
		}
		if (sToken.empty())
		{
			if (ctx.pStream->HasMore())
			{
				return Status(Status_ParseFailed, "Expected identifier at line {1}, column {2}", ctx.cLine, ctx.cColumn);
			}
			else
			{
				return Status(Status_EOF, "Unexpected EOF");
			}
		}
		else
		{
			if (!ctx.pStream->HasMore())
			{
				return Status(Status_EOF, "Unexpected EOF");
			}
		}

		return Status();
	}

	static inline Status ParseType(Context &ctx, String &sToken)
	{
		Byte ch;

		sToken.clear();
		for (;;)
		{
			if (!ctx.pStream->HasMore())
			{
				break;
			}
			ch = ctx.pStream->Peek();
			if ('_' == ch || cx_isalpha(ch) || (!sToken.empty() && (cx_isdigit(ch) || '.' == ch)))
			{
				sToken += (Char)ch;
				ctx.pStream->Next();
			}
			else
			{
				break;
			}
		}
		if (sToken.empty())
		{
			if (ctx.pStream->HasMore())
			{
				return Status(Status_ParseFailed, "Expected identifier at line {1}, column {2}", ctx.cLine, ctx.cColumn);
			}
			else
			{
				return Status(Status_EOF, "Unexpected EOF");
			}
		}
		else
		{
			if (!ctx.pStream->HasMore())
			{
				return Status(Status_EOF, "Unexpected EOF");
			}
		}

		return Status();
	}

	static inline Status ParseString(Context &ctx, String &sToken)
	{
		if (!ctx.pStream->HasMore())
		{
			return Status(Status_ParseFailed, "Expected string at line {1}, column {2}", ctx.cLine, ctx.cColumn);
		}
		if ((Byte)'"' != ctx.pStream->Peek())
		{
			return Status(Status_ParseFailed, "Expected string at line {1}, column {2}", ctx.cLine, ctx.cColumn);
		}
		ctx.pStream->Next();

		Byte ch;

		sToken.clear();
		for (;;)
		{
			if (!ctx.pStream->HasMore())
			{
				return Status(Status_ParseFailed, "Expected string at line {1}, column {2}", ctx.cLine, ctx.cColumn);
			}
			ch = ctx.pStream->Peek();
			if ('"' == ch)
			{
				ctx.pStream->Next();

				break;
			}
			else
			if ('\\' == ch)
			{
				ctx.pStream->Next();
				if (!ctx.pStream->HasMore())
				{
					return Status(Status_ParseFailed, "Expected string at line {1}, column {2}", ctx.cLine, ctx.cColumn);
				}
				ch = ctx.pStream->Peek();
				if ('"' == ch)
				{
					sToken += '"';
					ctx.pStream->Next();
				}
				else
				if ('\\' == ch)
				{
					sToken += '\\';
					ctx.pStream->Next();
				}
				else
				{
					sToken += '\\';
					sToken += (Char)ch;
					ctx.pStream->Next();
				}
			}
			else
			{
				sToken += (Char)ch;
				ctx.pStream->Next();
			}
		}

		return Status();
	}

	static inline Status ParseNumber(Context &ctx, UInt64 &nNumber)
	{
		Byte ch;
		Size cDigits = 0;

		nNumber = 0;
		for (;;)
		{
			if (!ctx.pStream->HasMore())
			{
				break;
			}
			ch = ctx.pStream->Peek();
			if (cx_isdigit(ch))
			{
				nNumber *= 10;
				nNumber += ch - (Byte)'0';
				cDigits++;
				ctx.pStream->Next();
			}
			else
			{
				break;
			}
		}
		if (0 == cDigits)
		{
			return Status(Status_ParseFailed, "Expected number at line {1}, column {2}", ctx.cLine, ctx.cColumn);
		}

		return Status();
	}

	static inline Status GetTypeFromString(const String &sTypeName, Member &m)
	{
#pragma warning(push)
#pragma warning(disable: 4996)
		if (0 == cx_strcmp(sTypeName.c_str(), "bool"))
		{
			m.nType           = Type_Bool;
			m.sActualTypeName = "CX_Bool";
		}
		else
		if (0 == cx_strcmp(sTypeName.c_str(), "int8"))
		{
			m.nType           = Type_Int8;
			m.sActualTypeName = "CX_Int8";
		}
		else
		if (0 == cx_strcmp(sTypeName.c_str(), "uint8"))
		{
			m.nType           = Type_UInt8;
			m.sActualTypeName = "CX_UInt8";
		}
		else
		if (0 == cx_strcmp(sTypeName.c_str(), "int16"))
		{
			m.nType           = Type_Int16;
			m.sActualTypeName = "CX_Int16";
		}
		else
		if (0 == cx_strcmp(sTypeName.c_str(), "uint16"))
		{
			m.nType           = Type_UInt16;
			m.sActualTypeName = "CX_UInt16";
		}
		else
		if (0 == cx_strcmp(sTypeName.c_str(), "int32"))
		{
			m.nType           = Type_Int32;
			m.sActualTypeName = "CX_Int32";
		}
		else
		if (0 == cx_strcmp(sTypeName.c_str(), "uint32"))
		{
			m.nType           = Type_UInt32;
			m.sActualTypeName = "CX_UInt32";
		}
		else
		if (0 == cx_strcmp(sTypeName.c_str(), "int64"))
		{
			m.nType           = Type_Int64;
			m.sActualTypeName = "CX_Int64";
		}
		else
		if (0 == cx_strcmp(sTypeName.c_str(), "uint64"))
		{
			m.nType           = Type_UInt64;
			m.sActualTypeName = "CX_UInt64";
		}
		else
		if (0 == cx_strcmp(sTypeName.c_str(), "float"))
		{
			m.nType           = Type_Float;
			m.sActualTypeName = "CX_Float";
		}
		else
		if (0 == cx_strcmp(sTypeName.c_str(), "double"))
		{
			m.nType           = Type_Double;
			m.sActualTypeName = "CX_Double";
		}
		else
		if (0 == cx_strcmp(sTypeName.c_str(), "char"))
		{
			m.nType           = Type_Char;
			m.sActualTypeName = "CX_Char";
		}
		else
		if (0 == cx_strcmp(sTypeName.c_str(), "wchar"))
		{
			m.nType           = Type_WChar;
			m.sActualTypeName = "CX_WChar";
		}
		else
		{
			m.nType           = Type_Struct;
			m.sActualTypeName = sTypeName;
		}
#pragma warning(pop)

		return Status();
	}

};

}//namespace Tools

}//namespace CBuffers

}//namespace CX
