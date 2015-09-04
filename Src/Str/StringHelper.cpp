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

#include "CX/Str/StringHelper.hpp"


namespace CX
{

namespace Str
{

StringHelper::StringHelper()
{
}

StringHelper::~StringHelper()
{
}

Status StringHelper::FindSubStr(const Char *pText, Size cTextLen, const Char *pWhat, Size cWhatLen, bool bCaseSensitive, 
                                const Char **pszPos)
{
	const Char *pPosText;
	const Char *pPosWhat;
	const Char *pPosText2;
	bool       bHasLen = (0 < cTextLen);

	pPosText = pText;
	while ((bHasLen && (Size)(pPosText - pText) < cTextLen) || (!bHasLen && 0 != *pPosText))
	{
		if ((bCaseSensitive && *pPosText == *pWhat) || (!bCaseSensitive && 
		    cx_tolower((unsigned char)*pPosText) == cx_tolower((unsigned char)*pWhat)))
		{
			pPosText2 = pPosText + 1;
			pPosWhat  = pWhat + 1;
			while (((bHasLen && (Size)(pPosText - pText) < cTextLen) || (!bHasLen && 0 != *pPosText)) && 
			       ((Size)(pPosWhat - pWhat) < cWhatLen) && ((bCaseSensitive && *pPosText2 == *pPosWhat) || (!bCaseSensitive && 
			           cx_tolower((unsigned char)*pPosText2) == cx_tolower((unsigned char)*pPosWhat))))
			{
				pPosText2++;
				pPosWhat++;
			}
			if ((Size)(pPosWhat - pWhat) == cWhatLen)
			{
				*pszPos = pPosText;

				return Status();
			}
			else
			{
				pPosText++;
			}
		}
		else
		{
			pPosText++;
		}
	}

	return Status_NotFound;
}

Status StringHelper::FindWithMarkers(const Char *pText, Size cTextLen, const Char *pPrefix, Size cPrefixLen, 
                                     const Char *pPostfix, Size cPostfixLen, bool bCaseSensitive, 
                                     const Char **pszBegin, const Char **pszEnd)
{
	Status     status;

	if (!(status = FindSubStr(pText, cTextLen, pPrefix, cPrefixLen, bCaseSensitive, pszBegin)))
	{
		return Status_NotFound;
	}
	*pszBegin += cPrefixLen;
	if (0 < cTextLen)
	{
		cTextLen -= *pszBegin - pText;
	}
	if (!(status = FindSubStr(*pszBegin, cTextLen, pPostfix, cPostfixLen, bCaseSensitive, pszEnd)))
	{
		return Status_NotFound;
	}

	return Status();
}

}//namespace Str

}//namespace CX

