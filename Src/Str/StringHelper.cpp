/* 
 * CX - C++ framework for general purpose development
 *
 * https://github.com/draede/cx
 * 
 * Copyright (C) 2014 - 2018 draede, draede [at] outlook [dot] com
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

bool StringHelper::Compare(const Char *pStr1, Size cStr1Len, const Char *pStr2, Size cStr2Len, bool bCaseSensitive, 
                           Size cMaxLen/* = 0*/)
{
	const Char *pPosStr1;
	const Char *pPosStr2;
	Size       cIndex;
	bool       bHas1Len = (0 < cStr1Len);
	bool       bHas2Len = (0 < cStr2Len);

	if (0 == cMaxLen && bHas1Len && bHas2Len && cStr1Len != cStr2Len)
	{
		return false;
	}
	cIndex   = 0;
	pPosStr1 = pStr1;
	pPosStr2 = pStr2;
	while (((bHas1Len && (Size)(pPosStr1 - pStr1) < cStr1Len) || (!bHas1Len && 0 != *pPosStr1)) && 
	       ((bHas2Len && (Size)(pPosStr2 - pStr2) < cStr2Len) || (!bHas2Len && 0 != *pPosStr2)))
	{
		if (0 < cMaxLen && cIndex == cMaxLen)
		{
			break;
		}
		cIndex++;
		if (bCaseSensitive)
		{
			if (*pPosStr1 != *pPosStr2)
			{
				return false;
			}
		}
		else
		{
			if (cx_tolower((unsigned char)*pPosStr1) != cx_tolower((unsigned char)*pPosStr2))
			{
				return false;
			}
		}
		pPosStr1++;
		pPosStr2++;
	}
	if (0 < cMaxLen)
	{
		return (cIndex == cMaxLen);
	}
	if (!((bHas1Len && (Size)(pPosStr1 - pStr1) == cStr1Len) || (!bHas1Len && 0 == *pPosStr1)) ||
	    !((bHas2Len && (Size)(pPosStr2 - pStr2) == cStr1Len) || (!bHas2Len && 0 == *pPosStr2)))
	{
		return false;
	}

	return true;
}

Status StringHelper::FindSubStr(const Char *pText, Size cTextLen, const Char *pWhat, Size cWhatLen, bool bCaseSensitive, 
                                const Char **pPos)
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
				*pPos = pPosText;

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
                                     const Char **pBegin, const Char **pEnd)
{
	Status     status;

	if (!(status = FindSubStr(pText, cTextLen, pPrefix, cPrefixLen, bCaseSensitive, pBegin)))
	{
		return Status_NotFound;
	}
	*pBegin += cPrefixLen;
	if (0 < cTextLen)
	{
		cTextLen -= *pBegin - pText;
	}
	if (!(status = FindSubStr(*pBegin, cTextLen, pPostfix, cPostfixLen, bCaseSensitive, pEnd)))
	{
		return Status_NotFound;
	}

	return Status();
}

Status StringHelper::Split(const Char *pText, Size cTextLen, const Char *pSeparator, Size cSeparatorLen, 
                           bool bCaseSensitive, StringsVector &vectorStrings)
{
	const Char *pPosText;
	const Char *pBegin;
	bool       bHasLen = (0 < cTextLen);
	Status     status;

	pPosText = pText;
	while ((bHasLen && (Size)(pPosText - pText) < cTextLen) || (!bHasLen && 0 != *pPosText))
	{
		if ((status = FindSubStr(pPosText, bHasLen ? (cTextLen - (pPosText - pText)) : 0, pSeparator, cSeparatorLen, 
		                         bCaseSensitive, &pBegin)))
		{
			if (pPosText < pBegin)
			{
				String sToken(pPosText, pBegin - pPosText);

				vectorStrings.push_back(sToken);
			}
			pPosText = pBegin + cSeparatorLen;
		}
		else
		{
			String sToken;

			while ((bHasLen && (Size)(pPosText - pText) < cTextLen) || (!bHasLen && 0 != *pPosText))
			{
				sToken += *pPosText;
				pPosText++;
			}
			if (!sToken.empty())
			{
				vectorStrings.push_back(sToken);
			}

			break;
		}
	}

	return Status();
}

Status StringHelper::Strip(const Char *pText, Size cTextLen, const Char *pStrip, Size cStripLen, 
                           bool bCaseSensitive, const Char **pBegin, const Char **pEnd, StripType nStripType/* = Strip_Both*/)
{
	const Char *pPosText;
	bool       bHasLen = (0 < cTextLen);
	Size       cIndex;

	pPosText = pText;
	if ((nStripType & Strip_Begin))
	{
		while ((bHasLen && (Size)(pPosText - pText) < cTextLen) || (!bHasLen && 0 != *pPosText))
		{
			for (cIndex = 0; cIndex < cStripLen; cIndex++)
			{
				if ((bCaseSensitive && *pPosText == pStrip[cIndex]) || 
				    (!bCaseSensitive && cx_tolower((UChar)*pPosText) == cx_tolower((UChar)pStrip[cIndex])))
				{
					break;
				}
			}
			if (cIndex < cStripLen)
			{
				pPosText++;
			}
			else
			{
				break;
			}
		}
		*pBegin = pPosText;
	}
	else
	{
		*pBegin = pPosText;
	}
	while ((bHasLen && (Size)(pPosText - pText) < cTextLen) || (!bHasLen && 0 != *pPosText))
	{
		pPosText++;
	}
	if ((nStripType & Strip_End))
	{
		pPosText--;
		while (pPosText > *pBegin)
		{
			for (cIndex = 0; cIndex < cStripLen; cIndex++)
			{
				if ((bCaseSensitive && *pPosText == pStrip[cIndex]) || 
				    (!bCaseSensitive && cx_tolower((UChar)*pPosText) == cx_tolower((UChar)pStrip[cIndex])))
				{
					break;
				}
			}
			if (cIndex < cStripLen)
			{
				pPosText--;
			}
			else
			{
				break;
			}
		}
		*pEnd = pPosText + 1;
	}
	else
	{
		*pEnd = pPosText;
	}

	return Status();
}

Status StringHelper::Replace(const Char *pText, Size cTextLen, const Char *pWhat, Size cWhatLen, const Char *pWithWhat, 
                             Size cWithWhatLen, bool bCaseSensitive, String &sString, bool bAll/* = true*/)
{
	const Char *pBegin;
	const Char *pPosText;
	bool       bHasLen = (0 < cTextLen);
	Status     status;

	sString.clear();
	pPosText = pText;
	while ((bHasLen && (Size)(pPosText - pText) < cTextLen) || (!bHasLen && 0 != *pPosText))
	{
		if ((status = FindSubStr(pPosText, bHasLen ? (cTextLen - (pPosText - pText)) : 0, pWhat, cWhatLen,
		                         bCaseSensitive, &pBegin)))
		{
			if (pBegin > pPosText)
			{
				sString.append(pPosText, pBegin - pPosText);
			}
			sString.append(pWithWhat, cWithWhatLen);
			pPosText = pBegin + cWhatLen;
			if (!bAll)
			{
				while ((bHasLen && (Size)(pPosText - pText) < cTextLen) || (!bHasLen && 0 != *pPosText))
				{
					sString += *pPosText;
					pPosText++;
				}

				break;
			}
		}
		else
		{
			while ((bHasLen && (Size)(pPosText - pText) < cTextLen) || (!bHasLen && 0 != *pPosText))
			{
				sString += *pPosText;
				pPosText++;
			}

			break;
		}
	}

	return Status();
}

}//namespace Str

}//namespace CX

