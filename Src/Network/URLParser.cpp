/*
* CX - C++ framework for general purpose development
*
* https://github.com/draede/cx
*
* Copyright (C) 2014-2016 draede, draede [at] outlook [dot] com
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

#include "CX/Network/URLParser.hpp"
#include "CX/C/ctype.h"


namespace CX
{

namespace Network
{

URLParser::URLParser()
{
}

URLParser::~URLParser()
{
}

//<protocol>://<host>[:<port>]/[<path>][?<query>]
Status URLParser::Parse(const Char *szURL, String &sProtocol, String &sHost, int &nPort, String &sPath, String &sQuery)
{
	sProtocol.clear();
	sHost.clear();
	nPort = -1;
	sPath.clear();
	sQuery.clear();

	const Char *pszStart;
	const Char *pszPos;

	pszStart = pszPos = szURL;
	while (0 != *pszPos && ':' != *pszPos)
	{
		pszPos++;
	}
	if (pszPos == pszStart || 0 != cx_strncmp(pszPos, "://", 3))
	{
		return Status_InvalidArg;
	}
	sProtocol.assign(pszStart, pszPos - pszStart);
	pszPos += 3;
	pszStart = pszPos;
	while (0 != *pszPos && ':' != *pszPos && '/' != *pszPos)
	{
		pszPos++;
	}
	if (pszPos == pszStart)
	{
		return Status_InvalidArg;
	}
	sHost.assign(pszStart, pszPos - pszStart);
	if (':' == *pszPos)
	{
		pszPos++;
		nPort = 0;
		while (cx_isdigit((unsigned char)*pszPos))
		{
			nPort *= 10;
			nPort += *pszPos - '0';
			if (65535 < nPort)
			{
				return Status_InvalidArg;
			}
			pszPos++;
		}
		if (0 == nPort)
		{
			return Status_InvalidArg;
		}
		pszStart = pszPos;
		if (0 != *pszPos && '/' != *pszPos)
		{
			return Status_InvalidArg;
		}
	}
	if ('/' == *pszPos)
	{
		pszPos++;
		pszStart = pszPos;
		while (0 != *pszPos && '?' != *pszPos)
		{
			pszPos++;
		}
		sPath.assign(pszStart, pszPos - pszStart);
		if ('?' == *pszPos)
		{
			sQuery = pszPos + 1;
		}
	}

	return Status();
}

Status URLParser::Encode(const Char *szSrcURL, String &sDestURL)
{
	static const Char hexdigits[] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F' };

	const Char *pszPos;

	sDestURL.clear();
	pszPos = szSrcURL;
	while (0 != *pszPos)
	{
		if (32 > (Byte )*pszPos || 127 < (Byte )*pszPos || 
		    '!' == *pszPos || '*' == *pszPos || '\'' == *pszPos || '(' == *pszPos || ')' == *pszPos || ';' == *pszPos || 
		    ':' == *pszPos || '@' == *pszPos || '&' == *pszPos || '=' == *pszPos || '+' == *pszPos || '$' == *pszPos || 
		    '/' == *pszPos || '?' == *pszPos || '%' == *pszPos || '#' == *pszPos || '[' == *pszPos || ']' == *pszPos)
		{
			sDestURL += "%";
#pragma warning(suppress: 6385)
			sDestURL += hexdigits[((Byte )*pszPos) / 16];
			sDestURL += hexdigits[((Byte )*pszPos) % 16];
		}
		else
		{
			sDestURL += *pszPos;
		}
		pszPos++;
	}

	return Status();
}

Status URLParser::Decode(const Char *szSrcURL, String &sDestURL)
{
	const Char *pszPos;

	sDestURL.clear();
	pszPos = szSrcURL;
	while (0 != *pszPos)
	{
		if ('%' == *pszPos)
		{
			pszPos++;
			if (cx_isdigit((unsigned char)*pszPos) && cx_isxdigit((unsigned char)*(pszPos + 1)))
			{
				Byte x = 0;

				x = 0;
				if ('0' <= *pszPos && '9' >= *pszPos)
				{
					x += 16 * (*pszPos - '0');
				}
				else
				if ('A' <= *pszPos && 'F' >= *pszPos)
				{
					x += 16 * (*pszPos - 'A' + 10);
				}
				else
				{
					x += 16 * (*pszPos - 'a' + 10);
				}
				pszPos++;
				if ('0' <= *pszPos && '9' >= *pszPos)
				{
					x += *pszPos - '0';
				}
				else
				if ('A' <= *pszPos && 'F' >= *pszPos)
				{
					x += *pszPos - 'A' + 10;
				}
				else
				{
					x += *pszPos - 'a' + 10;
				}
				pszPos++;
				sDestURL += (Char)x;
			}
			else
			{
				sDestURL += '%';
			}
		}
		else
		{
			sDestURL += *pszPos;
			pszPos++;
		}
	}

	return Status();
}

}//namespace Network

}//namespace CX
