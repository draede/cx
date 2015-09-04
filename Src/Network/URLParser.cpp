/*
* CX - C++ framework for general purpose development
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

}//namespace Network

}//namespace CX
