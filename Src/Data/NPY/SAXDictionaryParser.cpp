/* 
 * CX - C++ framework for general purpose development
 *
 * https://github.com/draede/cx
 * 
 * Copyright (C) 2014 - 2022 draede - draede [at] outlook [dot] com
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

#include "CX/precomp.hpp"
#include "CX/Data/NPY/SAXDictionaryParser.hpp"
#include "CX/IO/FileInputStream.hpp"


namespace CX
{

namespace Data
{

namespace NPY
{

SAXDictionaryParser::SAXDictionaryParser()
{
}

SAXDictionaryParser::~SAXDictionaryParser()
{
}

Status SAXDictionaryParser::ParseBuffer(const void *pBuffer, Size cbSize, Size *pcbActualSize/* = NULL*/)
{
	Context   ctx;
	Status    status;

	ctx.pPos    = (const UChar *)pBuffer;
	ctx.cLen    = cbSize;
	ctx.cLine   = 1;
	ctx.cColumn = 1;

	for (auto iter = m_vectorObservers.begin(); iter != m_vectorObservers.end(); ++iter)
	{
		if (!(status = (*iter)->OnBeginParse()))
		{
			return status;
		}
	}

	if (!(status = ParseWhitespaces(ctx)))
	{
		return status;
	}
	if ('{' != *ctx.pPos)
	{
		return Status(Status_ParseFailed, "Expected '{' end at {1}:{2}", ctx.cLine, ctx.cColumn);
	}
	ctx.Move();

	if (!(status = ParseObject(ctx, 1, MAX_DEPTH)))
	{
		return status;
	}

	for (auto iter = m_vectorObservers.begin(); iter != m_vectorObservers.end(); ++iter)
	{
		if (!(status = (*iter)->OnEndParse()))
		{
			return status;
		}
	}

	if (NULL != pcbActualSize)
	{
		*pcbActualSize = ctx.pPos - (const UChar *)pBuffer;
	}

	return Status();
}

Status SAXDictionaryParser::ParseObject(Context &ctx, Size cDepth, Size cMaxDepth)
{
	Bool         bSingleQuote;
	const Char   *pKey;
	Size         cKeyLen;
	Size         cEntries;
	Status       status;

	if (cDepth > cMaxDepth)
	{
		return Status(Status_ParseFailed, "Nesting level too deep");
	}

	cEntries = 0;
	for (auto iter = m_vectorObservers.begin(); iter != m_vectorObservers.end(); ++iter)
	{
		if (!(status = (*iter)->OnBeginObject()))
		{
			return status;
		}
	}

	for (;;)
	{
		//key
		if (!(status = ParseWhitespaces(ctx)))
		{
			return status;
		}
		if (0 == ctx.cLen)
		{
			return Status(Status_ParseFailed, "Unexpected end at {1}:{2}", ctx.cLine, ctx.cColumn);
		}
		if ('\'' == *ctx.pPos)
		{
			ctx.Move();
			bSingleQuote = True;
		}
		else
		if ('"' == *ctx.pPos)
		{
			ctx.Move();
			bSingleQuote = False;
		}
		else
		if ('}' == *ctx.pPos)
		{
			//it's ok even when cEntries > 0 (meaning that before this was a comma)

			ctx.Move();

			break;
		}
		else
		{
			return Status(Status_ParseFailed, "Expected '\\'', '\"' or '}' at {1}:{2}", ctx.cLine, ctx.cColumn);
		}
		if (!(status = ParseString(ctx, bSingleQuote, &pKey, &cKeyLen)))
		{
			return status;
		}
		for (auto iter = m_vectorObservers.begin(); iter != m_vectorObservers.end(); ++iter)
		{
			if (!(status = (*iter)->OnKey(pKey, cKeyLen)))
			{
				return status;
			}
		}
		if (!(status = ParseWhitespaces(ctx)))
		{
			return status;
		}
		if (0 == ctx.cLen)
		{
			return Status(Status_ParseFailed, "Unexpected end at {1}:{2}", ctx.cLine, ctx.cColumn);
		}
		if (':' != *ctx.pPos)
		{
			return Status(Status_ParseFailed, "Expected ':' at {1}:{2}", ctx.cLine, ctx.cColumn);
		}
		ctx.Move();

		//value
		if (!(status = ParseAny(ctx, cDepth + 1, cMaxDepth)))
		{
			return status;
		}

		cEntries++;

		//} or ,
		if (!(status = ParseWhitespaces(ctx)))
		{
			return status;
		}
		if (0 == ctx.cLen)
		{
			return Status(Status_ParseFailed, "Unexpected end at {1}:{2}", ctx.cLine, ctx.cColumn);
		}
		if (',' == *ctx.pPos)
		{
			ctx.Move();
		}
		else
		if ('}' == *ctx.pPos)
		{
			ctx.Move();

			break;
		}
		else
		{
			return Status(Status_ParseFailed, "Expected ',' or '}' at {1}:{2}", ctx.cLine, ctx.cColumn);
		}
	}

	for (auto iter = m_vectorObservers.begin(); iter != m_vectorObservers.end(); ++iter)
	{
		if (!(status = (*iter)->OnEndObject()))
		{
			return status;
		}
	}

	return Status();
}

Status SAXDictionaryParser::ParseArray(Context &ctx, Size cDepth, Size cMaxDepth)
{
	Size         cEntries;
	Status       status;

	if (cDepth > cMaxDepth)
	{
		return Status(Status_ParseFailed, "Nesting level too deep");
	}

	cEntries = 0;
	for (auto iter = m_vectorObservers.begin(); iter != m_vectorObservers.end(); ++iter)
	{
		if (!(status = (*iter)->OnBeginArray()))
		{
			return status;
		}
	}

	for (;;)
	{
		//value
		if (!(status = ParseWhitespaces(ctx)))
		{
			return status;
		}
		if (0 == ctx.cLen)
		{
			return Status(Status_ParseFailed, "Unexpected end at {1}:{2}", ctx.cLine, ctx.cColumn);
		}
		if (']' == *ctx.pPos)
		{
			//it's ok even when cEntries > 0 (meaning that before this was a comma)

			ctx.Move();

			break;
		}

		if (!(status = ParseAny(ctx, cDepth + 1, cMaxDepth)))
		{
			return status;
		}

		cEntries++;

		//] or ,
		if (!(status = ParseWhitespaces(ctx)))
		{
			return status;
		}
		if (0 == ctx.cLen)
		{
			return Status(Status_ParseFailed, "Unexpected end at {1}:{2}", ctx.cLine, ctx.cColumn);
		}
		if (',' == *ctx.pPos)
		{
			ctx.Move();
		}
		else
		if (']' == *ctx.pPos)
		{
			ctx.Move();

			break;
		}
		else
		{
			return Status(Status_ParseFailed, "Expected ',' or ']' at {1}:{2}", ctx.cLine, ctx.cColumn);
		}
	}

	for (auto iter = m_vectorObservers.begin(); iter != m_vectorObservers.end(); ++iter)
	{
		if (!(status = (*iter)->OnEndArray()))
		{
			return status;
		}
	}

	return Status();
}

Status SAXDictionaryParser::ParseTuple(Context &ctx, Size cDepth, Size cMaxDepth)
{
	Size         cEntries;
	Status       status;

	if (cDepth > cMaxDepth)
	{
		return Status(Status_ParseFailed, "Nesting level too deep");
	}

	cEntries = 0;
	for (auto iter = m_vectorObservers.begin(); iter != m_vectorObservers.end(); ++iter)
	{
		if (!(status = (*iter)->OnBeginTuple()))
		{
			return status;
		}
	}

	for (;;)
	{
		//value
		if (!(status = ParseWhitespaces(ctx)))
		{
			return status;
		}
		if (0 == ctx.cLen)
		{
			return Status(Status_ParseFailed, "Unexpected end at {1}:{2}", ctx.cLine, ctx.cColumn);
		}
		if (')' == *ctx.pPos)
		{
			if (0 < cEntries)
			{
				//(1, 2, ... , ) -> before this was a comma

				for (auto iter = m_vectorObservers.begin(); iter != m_vectorObservers.end(); ++iter)
				{
					if (!(status = (*iter)->OnTupleDefault()))
					{
						return status;
					}
				}
			}

			ctx.Move();

			break;
		}

		if (!(status = ParseAny(ctx, cDepth + 1, cMaxDepth)))
		{
			return status;
		}

		cEntries++;

		//) or ,
		if (!(status = ParseWhitespaces(ctx)))
		{
			return status;
		}
		if (0 == ctx.cLen)
		{
			return Status(Status_ParseFailed, "Unexpected end at {1}:{2}", ctx.cLine, ctx.cColumn);
		}
		if (',' == *ctx.pPos)
		{
			ctx.Move();
		}
		else
		if (')' == *ctx.pPos)
		{
			ctx.Move();

			break;
		}
		else
		{
			return Status(Status_ParseFailed, "Expected ',' or ')' at {1}:{2}", ctx.cLine, ctx.cColumn);
		}
	}

	for (auto iter = m_vectorObservers.begin(); iter != m_vectorObservers.end(); ++iter)
	{
		if (!(status = (*iter)->OnEndTuple()))
		{
			return status;
		}
	}

	return Status();
}

Status SAXDictionaryParser::ParseAny(Context &ctx, Size cDepth, Size cMaxDepth)
{
	Status   status;

	if (!(status = ParseWhitespaces(ctx)))
	{
		return status;
	}
	if (0 == ctx.cLen)
	{
		return Status(Status_ParseFailed, "Unexpected end at {1}:{2}", ctx.cLine, ctx.cColumn);
	}

	if ('{' == *ctx.pPos)
	{
		ctx.Move();

		return ParseObject(ctx, cDepth, cMaxDepth);
	}
	else
	if ('[' == *ctx.pPos)
	{
		ctx.Move();

		return ParseArray(ctx, cDepth, cMaxDepth);
	}
	else
	if ('(' == *ctx.pPos)
	{
		ctx.Move();

		return ParseTuple(ctx, cDepth, cMaxDepth);
	}
	else
	if ('\'' == *ctx.pPos || '"' == *ctx.pPos)
	{
		ctx.Move();

		const Char   *pStr;
		Size         cStrLen;

		if (!(status = ParseString(ctx, '\'' == *(ctx.pPos - 1), &pStr, &cStrLen)))
		{
			return status;
		}

		for (auto iter = m_vectorObservers.begin(); iter != m_vectorObservers.end(); ++iter)
		{
			if (!(status = (*iter)->OnString(pStr, cStrLen)))
			{
				return status;
			}
		}
	}
	else
	if (5 <= ctx.cLen && 0 == cx_strnicmp((const Char *)ctx.pPos, "false", 5))
	{
		ctx.Move(5);
		for (auto iter = m_vectorObservers.begin(); iter != m_vectorObservers.end(); ++iter)
		{
			if (!(status = (*iter)->OnBool(False)))
			{
				return status;
			}
		}
	}
	else
	if (4 <= ctx.cLen && 0 == cx_strnicmp((const Char *)ctx.pPos, "true", 4))
	{
		ctx.Move(4);
		for (auto iter = m_vectorObservers.begin(); iter != m_vectorObservers.end(); ++iter)
		{
			if (!(status = (*iter)->OnBool(True)))
			{
				return status;
			}
		}
	}
	else
	{
		Bool     bNegative = False;
		Bool     bPoint    = False;
		UInt64   uValue    = 0;
		Double   lfValue   = 0.0;

		if ('-' == *ctx.pPos || '+' == *ctx.pPos)
		{
			bNegative = '-' == *ctx.pPos;
			ctx.Move();
			if (0 == ctx.cLen)
			{
				return Status(Status_ParseFailed, "Unexpected end at {1}:{2}", ctx.cLine, ctx.cColumn);
			}
		}

		if ('.' == *ctx.pPos)
		{
			bNegative = '-' == *ctx.pPos;
			ctx.Move();
			if (0 == ctx.cLen)
			{
				return Status(Status_ParseFailed, "Unexpected end at {1}:{2}", ctx.cLine, ctx.cColumn);
			}
		}
		else
		if (!cx_isdigit(*ctx.pPos))
		{
			return Status(Status_ParseFailed, "Expected a digit at {1}:{2}", ctx.cLine, ctx.cColumn);
		}

		if (!bPoint)
		{
			while (cx_isdigit(*ctx.pPos))
			{
				uValue *= 10;
				uValue += *ctx.pPos - '0';
				ctx.Move();
			}
			if (0 < ctx.cLen)
			{
				if ('.' == *ctx.pPos)
				{
					ctx.Move();
				}
			}
		}
		if (bPoint)
		{
			Double lfPow = 1.0;

			bPoint  = True;
			lfValue = (Double)uValue;
			while (cx_isdigit(*ctx.pPos))
			{
				lfPow *= 10.0;
				lfValue += (*ctx.pPos - '0') / lfPow;
				ctx.Move();
			}
			if (0 < ctx.cLen)
			{
				if ('.' == *ctx.pPos)
				{
					return Status(Status_ParseFailed, "Unexpected '.' at {1}:{2}", ctx.cLine, ctx.cColumn);
				}
			}
			if (bNegative)
			{
				lfValue = -lfValue;
			}
			for (auto iter = m_vectorObservers.begin(); iter != m_vectorObservers.end(); ++iter)
			{
				if (!(status = (*iter)->OnReal(lfValue)))
				{
					return status;
				}
			}
		}
		else
		{
			if (bNegative)
			{
				Int64  nValue = (Int64)uValue;

				nValue = -nValue;
				for (auto iter = m_vectorObservers.begin(); iter != m_vectorObservers.end(); ++iter)
				{
					if (!(status = (*iter)->OnInt(nValue)))
					{
						return status;
					}
				}
			}
			else
			{
				for (auto iter = m_vectorObservers.begin(); iter != m_vectorObservers.end(); ++iter)
				{
					if (!(status = (*iter)->OnUInt(uValue)))
					{
						return status;
					}
				}
			}
		}
	}

	return Status();
}

Status SAXDictionaryParser::ParseWhitespaces(Context &ctx)
{
	while (cx_isspace(*ctx.pPos))
	{
		if ('\n' == *ctx.pPos)
		{
			ctx.cLine++;
			ctx.cColumn = 1;
			ctx.pPos++;
			ctx.cLen--;
		}
		else
		if ('\r' == *ctx.pPos)
		{
			ctx.pPos++;
			ctx.cLen--;
		}
		else
		{
			ctx.Move();
		}
	}

	return Status();
}

Status SAXDictionaryParser::ParseString(Context &ctx, Bool bSingleQuote, const Char **ppKey, Size *pcLen)
{
	*ppKey = (const Char *)ctx.pPos;
	*pcLen = 0;

	for (;;)
	{
		if ((bSingleQuote && '\'' == *ctx.pPos) || (!bSingleQuote && '"' == *ctx.pPos))
		{
			ctx.Move();

			break;
		}
		else
		if ('\\' == *ctx.pPos)
		{
			ctx.Move();
			(*pcLen)++;
			if (0 == ctx.pPos)
			{
				return Status(Status_ParseFailed, "Unexpected end at {1}:{2}", ctx.cLine, ctx.cColumn);
			}
			ctx.Move();
			(*pcLen)++;
			if (0 == ctx.pPos)
			{
				return Status(Status_ParseFailed, "Unexpected end at {1}:{2}", ctx.cLine, ctx.cColumn);
			}
		}
		else
		{
			ctx.Move();
			(*pcLen)++;
			if (0 == ctx.pPos)
			{
				return Status(Status_ParseFailed, "Unexpected end at {1}:{2}", ctx.cLine, ctx.cColumn);
			}
		}
	}

	return Status();
}

Status SAXDictionaryParser::AddObserver(ISAXDictionaryParserObserver *pObserver)
{
	m_vectorObservers.push_back(pObserver);

	return Status();
}

Status SAXDictionaryParser::RemoveObservers()
{
	m_vectorObservers.clear();

	return Status();
}

Status SAXDictionaryParser::EscapeString(const Char *szStr, String *psStr)
{
	static Char hexdigits[] = 
	{ 
		'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F' 
	};
	const Byte *pPos;
	Char       escape[6];
	Status     status;

	pPos = (const Byte *)szStr;
	while (0 != *pPos)
	{
		if (0x20 > *pPos) //control char
		{
			escape[0] = '\\';
			escape[1] = 'u';
			escape[2] = '0';
			escape[3] = '0';
			escape[4] = hexdigits[*pPos / 16];
			escape[5] = hexdigits[*pPos % 16];
			psStr->append(escape, 6);
			pPos++;
		}
		else
		if (0x80 > *pPos) //ASCII
		{
			if ('"' == *pPos || '\'' == *pPos || '\\' == *pPos || '/' == *pPos || '\b' == *pPos || '\f' == *pPos ||
				'\n' == *pPos || '\r' == *pPos || '\t' == *pPos)
			{
				escape[0] = '\\';
				escape[1] = *pPos;
				psStr->append(escape, 2);
				pPos++;
			}
			else
			{
				*psStr += (Char)*pPos;
				pPos++;
			}
		}
		else
		if (0xC2 > *pPos)
		{
			return Status(Status_InvalidArg, "Invalid UTF8 string");
		}
		else
		{
			unsigned cp = 0;

			if (0xE0 > *pPos) //2 bytes
			{
				if (0 == *(pPos + 1))
				{
					return Status(Status_InvalidArg, "Invalid UTF8 string");
				}
				if ((pPos[1] & 0xC0) != 0x80)
				{
					return Status(Status_InvalidArg, "Invalid UTF8 string");
				}

				cp = (pPos[0] << 6) + pPos[1] - 0x3080;

				pPos += 2;
			}
			else
			if (0xF0 > *pPos) //3 bytes
			{
				if (0 == *(pPos + 1) || 0 == *(pPos + 2))
				{
					return Status(Status_InvalidArg, "Invalid UTF8 string");
				}
				if ((pPos[1] & 0xC0) != 0x80)
				{
					return Status(Status_InvalidArg, "Invalid UTF8 string");
				}
				if (pPos[0] == 0xE0 && pPos[1] < 0xA0)
				{
					return Status(Status_InvalidArg, "Invalid UTF8 string");
				}
				if ((pPos[2] & 0xC0) != 0x80)
				{
					return Status(Status_InvalidArg, "Invalid UTF8 string");
				}

				cp = (pPos[0] << 12) + (pPos[1] << 6) + pPos[2] - 0xE2080;

				pPos += 3;
			}
			else
			if (0xF5 > *pPos) //4 bytes
			{
				if (0 == *(pPos + 1) || 0 == *(pPos + 2) || 0 == *(pPos + 3))
				{
					return Status(Status_InvalidArg, "Invalid UTF8 string");
				}
				if ((pPos[1] & 0xC0) != 0x80)
				{
					return Status(Status_InvalidArg, "Invalid UTF8 string");
				}
				if (pPos[0] == 0xF0 && pPos[1] < 0x90)
				{
					return Status(Status_InvalidArg, "Invalid UTF8 string");
				}
				if (pPos[0] == 0xF4 && pPos[1] >= 0x90)
				{
					return Status(Status_InvalidArg, "Invalid UTF8 string");
				}
				if ((pPos[2] & 0xC0) != 0x80)
				{
					return Status(Status_InvalidArg, "Invalid UTF8 string");
				}
				if ((pPos[3] & 0xC0) != 0x80)
				{
					return Status(Status_InvalidArg, "Invalid UTF8 string");
				}

				cp = (pPos[0] << 18) + (pPos[1] << 12) + (pPos[2] << 6) + pPos[3] - 0x3C82080;

				pPos += 4;
			}
			else
			{
				return Status(Status_InvalidArg, "Invalid UTF8 string");
			}

			if (cp <= 0xD7FF || (cp >= 0xE000 && cp <= 0xFFFF)) 
			{
				escape[0] = '\\';
				escape[1] = 'u';
				escape[2] = hexdigits[((Byte *)&cp)[1] / 16];
				escape[3] = hexdigits[((Byte *)&cp)[1] % 16];
				escape[4] = hexdigits[((Byte *)&cp)[0] / 16];
				escape[5] = hexdigits[((Byte *)&cp)[0] % 16];
				psStr->append(escape, 6);
			}
			else 
			if (cp >= 0x010000 && cp <= 0x10FFFF)
			{
				// Surrogate pair
				unsigned s     = cp - 0x010000;
				unsigned lead  = (s >> 10) + 0xD800;
				unsigned trail = (s & 0x3FF) + 0xDC00;

				escape[0] = '\\';
				escape[1] = 'u';
				escape[2] = hexdigits[((Byte *)&lead)[1] / 16];
				escape[3] = hexdigits[((Byte *)&lead)[1] % 16];
				escape[4] = hexdigits[((Byte *)&lead)[0] / 16];
				escape[5] = hexdigits[((Byte *)&lead)[0] % 16];
				psStr->append(escape, 6);
				escape[0] = '\\';
				escape[1] = 'u';
				escape[2] = hexdigits[((Byte *)&trail)[1] / 16];
				escape[3] = hexdigits[((Byte *)&trail)[1] % 16];
				escape[4] = hexdigits[((Byte *)&trail)[0] / 16];
				escape[5] = hexdigits[((Byte *)&trail)[0] % 16];
				psStr->append(escape, 6);
			}
			else
			{
				return Status(Status_InvalidArg, "Invalid UTF8 string");
			}
		}
	}

	return Status();
}

}//namespace NPY

}//namespace Data

}//namespace CX
