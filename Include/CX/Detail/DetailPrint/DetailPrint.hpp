/* 
 * CX - C++ framework for general purpose development
 *
 * https://github.com/draede/cx
 * 
 * Copyright (C) 2014 - 2017 draede - draede [at] outlook [dot] com
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
#include "CX/APIDefs.hpp"
#include "CX/StatusCodes.hpp"
#include "CX/String.hpp"
#include "CX/EmptyType.hpp"
#include "CX/Slice.hpp"
#include "CX/Limits.hpp"
#include <stdio.h>
#include "CX/C/ctype.h"


namespace CX
{

namespace IO
{

class IOutputStream;

}//namespace IO

}//namespace CX


namespace CX
{

namespace Detail
{

enum ExtraFlags
{
	ExtraFlag_None,
	ExtraFlag_HexLower,
	ExtraFlag_HexUpper,
};

namespace DetailPrint
{

inline StatusCode HexPrintHelper(const void *p, Size cbSize, Char *szOutput, Size cLen, Size *pcFinalLen, 
                                        bool bReverse = true, bool bUpperCase = true)
{
	static const Char hexdigits1[] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F' };
	static const Char hexdigits2[] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f' };
	Char              *hexdigits;
	Char              *pPos;
	bool              bZero;

	if (2 * cbSize + 1 > cLen)
	{
		return Status_TooSmall;
	}
	bZero = true;
	pPos  = szOutput;
	if (bUpperCase)
	{
		hexdigits = (Char *)hexdigits1;
	}
	else
	{
		hexdigits = (Char *)hexdigits2;
	}
	*pcFinalLen = 0;
	for (Size i = 0; i < cbSize; i++)
	{
		if (!bReverse)
		{
			*pPos = hexdigits[(*((Byte *)p + i)) / 16];
			pPos++;
			(*pcFinalLen)++;
			*pPos = hexdigits[(*((Byte *)p + i)) % 16];
			pPos++;
			(*pcFinalLen)++;
		}
		else
		{
			Byte hi = (*((Byte *)p + cbSize - i - 1)) / 16;
			Byte lo = (*((Byte *)p + cbSize - i - 1)) % 16;

			if (bZero)
			{
				if (0 < hi)
				{
					*pPos = hexdigits[hi];
					pPos++;
					(*pcFinalLen)++;
					bZero = false;
				}
			}
			else
			{
				*pPos = hexdigits[hi];
				pPos++;
				(*pcFinalLen)++;
			}
			if (bZero)
			{
				if (0 < lo)
				{
					*pPos = hexdigits[lo];
					pPos++;
					(*pcFinalLen)++;
					bZero = false;
				}
			}
			else
			{
				*pPos = hexdigits[lo];
				pPos++;
				(*pcFinalLen)++;
			}
		}
	}
	if (0 < cbSize && 0 == *pcFinalLen)
	{
		*pPos = '0';
		pPos++;
		*pPos = '0';
		pPos++;
		(*pcFinalLen)++;
	}
	*pPos = 0;

	return Status_OK;
}

CX_API Bool DoubleToString(Double lfValue, Char *szOutput, Size cLen, Size cPrecision);

CX_API Bool UTF8toWChar(const Char *szUTF8, WString *pwsWChar, Size cUTF8Len = TYPE_SIZE_MAX);

CX_API Bool WChartoUTF8(const WChar *wszWChar, String *psUTF8, Size cWCharLen = TYPE_SIZE_MAX);

CX_API Bool UTF8toWCharx(const Char *szUTF8, std::wstring *pwsWChar, Size cUTF8Len = TYPE_SIZE_MAX);

CX_API Bool WChartoUTF8x(const WChar *wszWChar, std::string *psUTF8, Size cWCharLen = TYPE_SIZE_MAX);

CX_API StatusCode WriteStream(IO::IOutputStream *pOutputStream, const Char *pBuffer, Size cLen);

}//namespace DetailPrint

}//namespace Detail

#ifndef CX_USE_CUSTOM_PRINTOUTPUT

template <typename T> 
inline StatusCode PrintOutput(T o, const Char *pBuffer, Size cLen);

template <> 
inline StatusCode PrintOutput<FILE *>(FILE *pFile, const Char *pBuffer, Size cLen)
{
	if (cLen * sizeof(Char) != ::fwrite(pBuffer, 1, cLen * sizeof(Char), pFile))
	{
		return Status_WriteFailed;
	}

	return Status_OK;
}

template <>
inline StatusCode PrintOutput<IO::IOutputStream *>(IO::IOutputStream *pOutputStream,
                                                          const Char *pBuffer, Size cLen)
{
	return Detail::DetailPrint::WriteStream(pOutputStream, pBuffer, cLen);
}

template <>
inline StatusCode PrintOutput<String *>(String *psStr, const Char *pBuffer, Size cLen)
{
	psStr->append(pBuffer, cLen);

	return Status_OK;
}

template <>
inline StatusCode PrintOutput<WString *>(WString *pwsStr, const Char *pBuffer, Size cLen)
{
	if (!Detail::DetailPrint::UTF8toWChar(pBuffer, pwsStr, cLen))
	{
		return Status_ConversionFailed;
	}

	return Status_OK;
}

/*

template <>
inline StatusCode PrintOutput<std::string *>(std::string *psStr, const Char *pBuffer, Size cLen)
{
	psStr->append(pBuffer, cLen);

	return Status_OK;
}

template <>
inline StatusCode PrintOutput<std::wstring *>(std::wstring *pwsStr, const Char *pBuffer, Size cLen)
{
	if (!Detail::DetailPrint::UTF8toWCharx(pBuffer, pwsStr, cLen))
	{
		return Status_ConversionFailed;
	}

	return Status_OK;
}

*/

template <>
inline StatusCode PrintOutput<Slice *>(Slice *pSlice, const Char *pBuffer, Size cLen)
{
	if (pSlice->cbSize + 1 < cLen)
	{
		return Status_OutOfBounds;
	}
	memcpy(pSlice->pBuffer, pBuffer, cLen);
	*((Byte *)pSlice->pBuffer + cLen) = 0;

	return Status_OK;
}

template <>
inline StatusCode PrintOutput<Slice &>(Slice &slice, const Char *pBuffer, Size cLen)
{
	if (slice.cbSize + 1 < cLen)
	{
		return Status_OutOfBounds;
	}
	memcpy(slice.pBuffer, pBuffer, cLen);
	*((Byte *)slice.pBuffer + cLen) = 0;

	return Status_OK;
}

#endif

#ifndef CX_USE_CUSTOM_TOSTRING

namespace Detail
{

namespace DetailPrint
{

static inline Bool StrCopy(CX_Char *szDst, CX_Size cDstLen, const CX_Char *szSrc, CX_Size *pcSrcLen)
{
	CX_Char        *pszDst;
	const CX_Char  *pszSrc;

	*pcSrcLen = 0;
	pszSrc = szSrc;
	pszDst = szDst;
	while (*pcSrcLen + 1 < cDstLen && '\0' != *pszSrc)
	{
		*pszDst = *pszSrc;
		pszSrc++;
		pszDst++;
		(*pcSrcLen)++;
	}
	*pszDst = 0;

	return ('\0' == *pszSrc);
}

//from Andrei Alexandrescu
static inline Size GetUInt64DigitsCount(UInt64 nValue)
{
	Size cDigitsCount = 1;

	for (;;)
	{
		if (nValue < 10)
		{
			return cDigitsCount;
		}
		if (nValue < 100)
		{
			return cDigitsCount + 1;
		}
		if (nValue < 1000)
		{
			return cDigitsCount + 2;
		}
		if (nValue < 10000)
		{
			return cDigitsCount + 3;
		}
		nValue /= 10000U;
		cDigitsCount += 4;
	}
}

//from Andrei Alexandrescu
static inline Size UInt64ToString(UInt64 nValue, Char *szDst, Size cPreCalcLen = 0)
{
	static const char digits[201] =
		"0001020304050607080910111213141516171819"
		"2021222324252627282930313233343536373839"
		"4041424344454647484950515253545556575859"
		"6061626364656667686970717273747576777879"
		"8081828384858687888990919293949596979899";

	Size const cLen = (0 == cPreCalcLen ? GetUInt64DigitsCount(nValue) : cPreCalcLen);

	Size cNext = cLen - 1;

	while (nValue >= 100)
	{
		const UInt64 i = (nValue % 100) * 2;

		nValue /= 100;
		szDst[cNext] = digits[i + 1];
		szDst[cNext - 1] = digits[i];
		cNext -= 2;
	}
	if (nValue < 10)
	{
		szDst[cNext] = (Char)('0' + nValue);
	}
	else
	{
		UInt64 i = UInt64(nValue) * 2;

		szDst[cNext] = digits[i + 1];
		szDst[cNext - 1] = digits[i];
	}

	return cLen;
}

}//namespace DetailPrint

}//namespace Detail

template <typename T>
inline StatusCode ToString(T p, unsigned int nExtraFlags, Char *szOutput, Size cLen, Size *pcFinalLen, Size cPrecision);

template <>
inline StatusCode ToString<EmptyType>(EmptyType p, unsigned int nExtraFlags, Char *szOutput, Size cLen, Size *pcFinalLen, 
                                             Size cPrecision)
{
	CX_UNUSED(p);
	CX_UNUSED(nExtraFlags);
	CX_UNUSED(szOutput);
	CX_UNUSED(cLen);
	CX_UNUSED(pcFinalLen);
	CX_UNUSED(cPrecision);

	return Status_NotSupported;
}

template <>
inline StatusCode ToString<Char>(Char p, unsigned int nExtraFlags, Char *szOutput, Size cLen, Size *pcFinalLen, 
                                        Size cPrecision)
{
	CX_UNUSED(nExtraFlags);
	CX_UNUSED(cPrecision);

	if (cLen < 2)
	{
		return Status_TooSmall;
	}
	*szOutput	= p;
	szOutput[1]	= 0;
	*pcFinalLen	= 1;

	return Status_OK;
}

template <>
inline StatusCode ToString<Bool>(Bool p, unsigned int nExtraFlags, Char *szOutput, Size cLen, Size *pcFinalLen, 
                                        Size cPrecision)
{
	CX_UNUSED(nExtraFlags);
	CX_UNUSED(cPrecision);

	if (p)
	{
		if (cLen < 5)
		{
			return Status_TooSmall;
		}
		memcpy(szOutput, "true", 4);
		szOutput[4] = 0;
		*pcFinalLen = 4;

		return Status_OK;
	}
	else
	{
		if (cLen < 6)
		{
			return Status_TooSmall;
		}
		memcpy(szOutput, "false", 5);
		szOutput[5]	= 0;
		*pcFinalLen = 5;

		return Status_OK;
	}
}

template <>
inline StatusCode ToString<Int8>(Int8 p, unsigned int nExtraFlags, Char *szOutput, Size cLen, Size *pcFinalLen, 
                                        Size cPrecision)
{
	CX_UNUSED(cPrecision);

	if (Detail::ExtraFlag_HexLower == nExtraFlags || Detail::ExtraFlag_HexUpper == nExtraFlags)
	{
		return Detail::DetailPrint::HexPrintHelper(&p, sizeof(p), szOutput, cLen, pcFinalLen, true, 
		                                           (Detail::ExtraFlag_HexUpper == nExtraFlags));
	}
	else
	{
		if (0 <= p)
		{
			*pcFinalLen = Detail::DetailPrint::GetUInt64DigitsCount((UInt64)p);
			if (*pcFinalLen + 1 > cLen)
			{
				return Status_TooSmall;
			}
			Detail::DetailPrint::UInt64ToString((UInt64)p, szOutput, *pcFinalLen);
			szOutput[*pcFinalLen] = 0;
		}
		else
		{
			*pcFinalLen = Detail::DetailPrint::GetUInt64DigitsCount((UInt64)(-p));
			if (*pcFinalLen + 2 > cLen)
			{
				return Status_TooSmall;
			}
			*szOutput = '-';
			Detail::DetailPrint::UInt64ToString((UInt64)(-p), szOutput + 1, *pcFinalLen);
			(*pcFinalLen)++;
			szOutput[*pcFinalLen] = 0;
		}

		return Status_OK;
	}
}

template <>
inline StatusCode ToString<UInt8>(UInt8 p, unsigned int nExtraFlags, Char *szOutput, Size cLen, Size *pcFinalLen, 
                                         Size cPrecision)
{
	CX_UNUSED(cPrecision);

	if (Detail::ExtraFlag_HexLower == nExtraFlags || Detail::ExtraFlag_HexUpper == nExtraFlags)
	{
		return Detail::DetailPrint::HexPrintHelper(&p, sizeof(p), szOutput, cLen, pcFinalLen, true,
		                                           (Detail::ExtraFlag_HexUpper == nExtraFlags));
	}
	else
	{
		*pcFinalLen = Detail::DetailPrint::GetUInt64DigitsCount((UInt64)p);
		if (*pcFinalLen + 1 > cLen)
		{
			return Status_TooSmall;
		}
		Detail::DetailPrint::UInt64ToString((UInt64)p, szOutput, *pcFinalLen);
		szOutput[*pcFinalLen] = 0;

		return Status_OK;
	}
}

template <>
inline StatusCode ToString<Int16>(Int16 p, unsigned int nExtraFlags, Char *szOutput, Size cLen, Size *pcFinalLen, 
                                         Size cPrecision)
{
	CX_UNUSED(cPrecision);

	if (Detail::ExtraFlag_HexLower == nExtraFlags || Detail::ExtraFlag_HexUpper == nExtraFlags)
	{
		return Detail::DetailPrint::HexPrintHelper(&p, sizeof(p), szOutput, cLen, pcFinalLen, true,
		                                           (Detail::ExtraFlag_HexUpper == nExtraFlags));
	}
	else
	{
		if (0 <= p)
		{
			*pcFinalLen = Detail::DetailPrint::GetUInt64DigitsCount((UInt64)p);
			if (*pcFinalLen + 1 > cLen)
			{
				return Status_TooSmall;
			}
			Detail::DetailPrint::UInt64ToString((UInt64)p, szOutput, *pcFinalLen);
			szOutput[*pcFinalLen] = 0;
		}
		else
		{
			*pcFinalLen = Detail::DetailPrint::GetUInt64DigitsCount((UInt64)(-p));
			if (*pcFinalLen + 2 > cLen)
			{
				return Status_TooSmall;
			}
			*szOutput = '-';
			Detail::DetailPrint::UInt64ToString((UInt64)(-p), szOutput + 1, *pcFinalLen);
			(*pcFinalLen)++;
			szOutput[*pcFinalLen] = 0;
		}

		return Status_OK;
	}
}

template <>
inline StatusCode ToString<UInt16>(UInt16 p, unsigned int nExtraFlags, Char *szOutput, Size cLen, Size *pcFinalLen, 
                                          Size cPrecision)
{
	CX_UNUSED(cPrecision);

	if (Detail::ExtraFlag_HexLower == nExtraFlags || Detail::ExtraFlag_HexUpper == nExtraFlags)
	{
		return Detail::DetailPrint::HexPrintHelper(&p, sizeof(p), szOutput, cLen, pcFinalLen, true,
			(Detail::ExtraFlag_HexUpper == nExtraFlags));
	}
	else
	{
		*pcFinalLen = Detail::DetailPrint::GetUInt64DigitsCount((UInt64)p);
		if (*pcFinalLen + 1 > cLen)
		{
			return Status_TooSmall;
		}
		Detail::DetailPrint::UInt64ToString((UInt64)p, szOutput, *pcFinalLen);
		szOutput[*pcFinalLen] = 0;

		return Status_OK;
	}
}

template <>
inline StatusCode ToString<Int32>(Int32 p, unsigned int nExtraFlags, Char *szOutput, Size cLen, Size *pcFinalLen, 
                                         Size cPrecision)
{
	CX_UNUSED(cPrecision);

	if (Detail::ExtraFlag_HexLower == nExtraFlags || Detail::ExtraFlag_HexUpper == nExtraFlags)
	{
		return Detail::DetailPrint::HexPrintHelper(&p, sizeof(p), szOutput, cLen, pcFinalLen, true,
		                                           (Detail::ExtraFlag_HexUpper == nExtraFlags));
	}
	else
	{
		if (0 <= p)
		{
			*pcFinalLen = Detail::DetailPrint::GetUInt64DigitsCount((UInt64)p);
			if (*pcFinalLen + 1 > cLen)
			{
				return Status_TooSmall;
			}
			Detail::DetailPrint::UInt64ToString((UInt64)p, szOutput, *pcFinalLen);
			szOutput[*pcFinalLen] = 0;
		}
		else
		{
			*pcFinalLen = Detail::DetailPrint::GetUInt64DigitsCount((UInt64)(-p));
			if (*pcFinalLen + 2 > cLen)
			{
				return Status_TooSmall;
			}
			*szOutput = '-';
			Detail::DetailPrint::UInt64ToString((UInt64)(-p), szOutput + 1, *pcFinalLen);
			(*pcFinalLen)++;
			szOutput[*pcFinalLen] = 0;
		}

		return Status_OK;
	}
}

template <>
inline StatusCode ToString<UInt32>(UInt32 p, unsigned int nExtraFlags, Char *szOutput, Size cLen, Size *pcFinalLen, 
                                          Size cPrecision)
{
	CX_UNUSED(cPrecision);

	if (Detail::ExtraFlag_HexLower == nExtraFlags || Detail::ExtraFlag_HexUpper == nExtraFlags)
	{
		return Detail::DetailPrint::HexPrintHelper(&p, sizeof(p), szOutput, cLen, pcFinalLen, true,
		                                           (Detail::ExtraFlag_HexUpper == nExtraFlags));
	}
	else
	{
		*pcFinalLen = Detail::DetailPrint::GetUInt64DigitsCount((UInt64)p);
		if (*pcFinalLen + 1 > cLen)
		{
			return Status_TooSmall;
		}
		Detail::DetailPrint::UInt64ToString((UInt64)p, szOutput, *pcFinalLen);
		szOutput[*pcFinalLen] = 0;

		return Status_OK;
	}
}

template <>
inline StatusCode ToString<Int64>(Int64 p, unsigned int nExtraFlags, Char *szOutput, Size cLen, Size *pcFinalLen, 
                                         Size cPrecision)
{
	CX_UNUSED(cPrecision);

	if (Detail::ExtraFlag_HexLower == nExtraFlags || Detail::ExtraFlag_HexUpper == nExtraFlags)
	{
		return Detail::DetailPrint::HexPrintHelper(&p, sizeof(p), szOutput, cLen, pcFinalLen, true, 
		                                           (Detail::ExtraFlag_HexUpper == nExtraFlags));
	}
	else
	{
		if (0 <= p)
		{
			*pcFinalLen = Detail::DetailPrint::GetUInt64DigitsCount((UInt64)p);
			if (*pcFinalLen + 1 > cLen)
			{
				return Status_TooSmall;
			}
			Detail::DetailPrint::UInt64ToString((UInt64)p, szOutput, *pcFinalLen);
			szOutput[*pcFinalLen] = 0;
		}
		else
		{
			*pcFinalLen = Detail::DetailPrint::GetUInt64DigitsCount((UInt64)(-p));
			if (*pcFinalLen + 2 > cLen)
			{
				return Status_TooSmall;
			}
			*szOutput = '-';
			Detail::DetailPrint::UInt64ToString((UInt64)(-p), szOutput + 1, *pcFinalLen);
			(*pcFinalLen)++;
			szOutput[*pcFinalLen] = 0;
		}

		return Status_OK;
	}
}

template <>
inline StatusCode ToString<UInt64>(UInt64 p, unsigned int nExtraFlags, Char *szOutput, Size cLen, Size *pcFinalLen, 
                                          Size cPrecision)
{
	CX_UNUSED(cPrecision);

	if (Detail::ExtraFlag_HexLower == nExtraFlags || Detail::ExtraFlag_HexUpper == nExtraFlags)
	{
		return Detail::DetailPrint::HexPrintHelper(&p, sizeof(p), szOutput, cLen, pcFinalLen, true,
		                                           (Detail::ExtraFlag_HexUpper == nExtraFlags));
	}
	else
	{
		*pcFinalLen = Detail::DetailPrint::GetUInt64DigitsCount((UInt64)p);
		if (*pcFinalLen + 1 > cLen)
		{
			return Status_TooSmall;
		}
		Detail::DetailPrint::UInt64ToString((UInt64)p, szOutput, *pcFinalLen);
		szOutput[*pcFinalLen] = 0;

		return Status_OK;
	}
}

template <>
inline StatusCode ToString<Float>(Float p, unsigned int nExtraFlags, Char *szOutput, Size cLen, Size *pcFinalLen, 
                                         Size cPrecision)
{
	CX_UNUSED(nExtraFlags);

	if (!Detail::DetailPrint::DoubleToString(p, szOutput, cLen, cPrecision))
	{
		return Status_ConversionFailed;
	}
	else
	{
		*pcFinalLen = 0;

		const Char *pszPos = szOutput;

		while ('\0' != *pszPos)
		{
			pszPos++;
			(*pcFinalLen)++;
		}

		return Status_OK;
	}
}

template <>
inline StatusCode ToString<Double>(Double p, unsigned int nExtraFlags, Char *szOutput, Size cLen, Size *pcFinalLen, 
                                          Size cPrecision)
{
	CX_UNUSED(nExtraFlags);

	if (!Detail::DetailPrint::DoubleToString(p, szOutput, cLen, cPrecision))
	{
		return Status_ConversionFailed;
	}
	else
	{
		*pcFinalLen = 0;

		const Char *pszPos = szOutput;

		while ('\0' != *pszPos)
		{
			pszPos++;
			(*pcFinalLen)++;
		}

		return Status_OK;
	}
}

template <>
inline StatusCode ToString<long>(long p, unsigned int nExtraFlags, Char *szOutput, Size cLen, Size *pcFinalLen, 
                                        Size cPrecision)
{
	CX_UNUSED(cPrecision);

	if (Detail::ExtraFlag_HexLower == nExtraFlags || Detail::ExtraFlag_HexUpper == nExtraFlags)
	{
		return Detail::DetailPrint::HexPrintHelper(&p, sizeof(p), szOutput, cLen, pcFinalLen, true, 
		                                           (Detail::ExtraFlag_HexUpper == nExtraFlags));
	}
	else
	{
		if (0 <= p)
		{
			*pcFinalLen = Detail::DetailPrint::GetUInt64DigitsCount((UInt64)p);
			if (*pcFinalLen + 1 > cLen)
			{
				return Status_TooSmall;
			}
			Detail::DetailPrint::UInt64ToString((UInt64)p, szOutput, *pcFinalLen);
			szOutput[*pcFinalLen] = 0;
		}
		else
		{
			*pcFinalLen = Detail::DetailPrint::GetUInt64DigitsCount((UInt64)(-p));
			if (*pcFinalLen + 2 > cLen)
			{
				return Status_TooSmall;
			}
			*szOutput = '-';
			Detail::DetailPrint::UInt64ToString((UInt64)(-p), szOutput + 1, *pcFinalLen);
			(*pcFinalLen)++;
			szOutput[*pcFinalLen] = 0;
		}

		return Status_OK;
	}
}

template <>
inline StatusCode ToString<unsigned long>(unsigned long p, unsigned int nExtraFlags, Char *szOutput, Size cLen, 
                                                 Size *pcFinalLen, Size cPrecision)
{
	CX_UNUSED(cPrecision);

	if (Detail::ExtraFlag_HexLower == nExtraFlags || Detail::ExtraFlag_HexUpper == nExtraFlags)
	{
		return Detail::DetailPrint::HexPrintHelper(&p, sizeof(p), szOutput, cLen, pcFinalLen, true, 
		                                           (Detail::ExtraFlag_HexUpper == nExtraFlags));
	}
	else
	{
		*pcFinalLen = Detail::DetailPrint::GetUInt64DigitsCount((UInt64)p);
		if (*pcFinalLen + 1 > cLen)
		{
			return Status_TooSmall;
		}
		Detail::DetailPrint::UInt64ToString((UInt64)p, szOutput, *pcFinalLen);
		szOutput[*pcFinalLen] = 0;

		return Status_OK;
	}
}

template <>
inline StatusCode ToString<const Char *>(const Char *p, unsigned int nExtraFlags, Char *szOutput, Size cLen, 
                                                Size *pcFinalLen, Size cPrecision)
{
	CX_UNUSED(nExtraFlags);
	CX_UNUSED(cPrecision);

	if (!Detail::DetailPrint::StrCopy(szOutput, cLen, p, pcFinalLen))
	{
		return Status_TooSmall;
	}
	else
	{
		return Status_OK;
	}
}

template <>
inline StatusCode ToString<Char *>(Char *p, unsigned int nExtraFlags, Char *szOutput, Size cLen, Size *pcFinalLen, 
                                          Size cPrecision)
{
	return ToString<const Char *>(p, nExtraFlags, szOutput, cLen, pcFinalLen, cPrecision);
}

template <>
inline StatusCode ToString<const String &>(const String &p, unsigned int nExtraFlags, Char *szOutput, Size cLen, 
                                                  Size *pcFinalLen, Size cPrecision)
{
	CX_UNUSED(nExtraFlags);
	CX_UNUSED(cPrecision);

	if (!Detail::DetailPrint::StrCopy(szOutput, cLen, p.c_str(), pcFinalLen))
	{
		return Status_TooSmall;
	}
	else
	{
		return Status_OK;
	}
}

template <>
inline StatusCode ToString<String &>(String &p, unsigned int nExtraFlags, Char *szOutput, Size cLen, Size *pcFinalLen, 
                                            Size cPrecision)
{
	return ToString<const String &>(p, nExtraFlags, szOutput, cLen, pcFinalLen, cPrecision);
}

template <>
inline StatusCode ToString<String>(String p, unsigned int nExtraFlags, Char *szOutput, Size cLen, Size *pcFinalLen, 
                                          Size cPrecision)
{
	return ToString<const String &>(p, nExtraFlags, szOutput, cLen, pcFinalLen, cPrecision);
}

template <>
inline StatusCode ToString<const WChar *>(const WChar *p, unsigned int nExtraFlags, Char *szOutput, Size cLen, 
                                                 Size *pcFinalLen, Size cPrecision)
{
	String sTmp;

	CX_UNUSED(cPrecision);

	if (!Detail::DetailPrint::WChartoUTF8(p, &sTmp))
	{
		return Status_ConversionFailed;
	}

	return ToString<const String &>(sTmp, nExtraFlags, szOutput, cLen, pcFinalLen, cPrecision);
}

template <>
inline StatusCode ToString<WChar *>(WChar *p, unsigned int nExtraFlags, Char *szOutput, Size cLen, Size *pcFinalLen, 
                                           Size cPrecision)
{
	String sTmp;

	CX_UNUSED(cPrecision);

	if (!Detail::DetailPrint::WChartoUTF8(p, &sTmp))
	{
		return Status_ConversionFailed;
	}

	return ToString<const String &>(sTmp, nExtraFlags, szOutput, cLen, pcFinalLen, cPrecision);

}

template <>
inline StatusCode ToString<const WString &>(const WString &p, unsigned int nExtraFlags, Char *szOutput, Size cLen, 
                                                   Size *pcFinalLen, Size cPrecision)
{
	String sTmp;

	CX_UNUSED(cPrecision);

	if (!Detail::DetailPrint::WChartoUTF8(p.c_str(), &sTmp))
	{
		return Status_ConversionFailed;
	}

	return ToString<const String &>(sTmp, nExtraFlags, szOutput, cLen, pcFinalLen, cPrecision);
}

template <>
inline StatusCode ToString<WString &>(WString &p, unsigned int nExtraFlags, Char *szOutput, Size cLen, Size *pcFinalLen, 
                                             Size cPrecision)
{
	String sTmp;

	CX_UNUSED(cPrecision);

	if (!Detail::DetailPrint::WChartoUTF8(p.c_str(), &sTmp))
	{
		return Status_ConversionFailed;
	}

	return ToString<const String &>(sTmp, nExtraFlags, szOutput, cLen, pcFinalLen, cPrecision);

}

template <>
inline StatusCode ToString<WString>(WString p, unsigned int nExtraFlags, Char *szOutput, Size cLen, Size *pcFinalLen, 
                                           Size cPrecision)
{
	String sTmp;

	CX_UNUSED(cPrecision);

	if (!Detail::DetailPrint::WChartoUTF8(p.c_str(), &sTmp))
	{
		return Status_ConversionFailed;
	}

	return ToString<const String &>(sTmp, nExtraFlags, szOutput, cLen, pcFinalLen, cPrecision);
}

/*

template <>
inline StatusCode ToString<const std::string &>(const std::string &p, unsigned int nExtraFlags, Char *szOutput, 
                                                       Size cLen, Size *pcFinalLen, Size cPrecision)
{
	CX_UNUSED(cPrecision);
	CX_UNUSED(nExtraFlags);
              
	if (!Detail::DetailPrint::StrCopy(szOutput, cLen, p.c_str(), pcFinalLen))
	{
		return Status_TooSmall;
	}
	else
	{
		return Status_OK;
	}
}

template <>
inline StatusCode ToString<std::string &>(std::string &p, unsigned int nExtraFlags, Char *szOutput, Size cLen, 
                                                 Size *pcFinalLen, Size cPrecision)
{
	return ToString<const std::string &>(p, nExtraFlags, szOutput, cLen, pcFinalLen, cPrecision);
}

template <>
inline StatusCode ToString<std::string>(std::string p, unsigned int nExtraFlags, Char *szOutput, Size cLen, 
                                               Size *pcFinalLen, Size cPrecision)
{
	return ToString<const std::string &>(p, nExtraFlags, szOutput, cLen, pcFinalLen, cPrecision);
}

template <>
inline StatusCode ToString<const std::wstring &>(const std::wstring &p, unsigned int nExtraFlags, Char *szOutput, 
                                                        Size cLen, Size *pcFinalLen, Size cPrecision)
{
	String sTmp;

	CX_UNUSED(cPrecision);

	if (!Detail::DetailPrint::WChartoUTF8(p.c_str(), &sTmp))
	{
		return Status_ConversionFailed;
	}

	return ToString<const String &>(sTmp, nExtraFlags, szOutput, cLen, pcFinalLen, cPrecision);
}

template <>
inline StatusCode ToString<std::wstring &>(std::wstring &p, unsigned int nExtraFlags, Char *szOutput, Size cLen, 
                                                  Size *pcFinalLen, Size cPrecision)
{
	String sTmp;

	CX_UNUSED(cPrecision);

	if (!Detail::DetailPrint::WChartoUTF8(p.c_str(), &sTmp))
	{
		return Status_ConversionFailed;
	}

	return ToString<const String &>(sTmp, nExtraFlags, szOutput, cLen, pcFinalLen, cPrecision);

}

template <>
inline StatusCode ToString<std::wstring>(std::wstring p, unsigned int nExtraFlags, Char *szOutput, Size cLen, 
                                                Size *pcFinalLen, Size cPrecision)
{
	String sTmp;

	CX_UNUSED(cPrecision);

	if (!Detail::DetailPrint::WChartoUTF8(p.c_str(), &sTmp))
	{
		return Status_ConversionFailed;
	}

	return ToString<const String &>(sTmp, nExtraFlags, szOutput, cLen, pcFinalLen, cPrecision);
}

*/

template <>
inline StatusCode ToString<void *>(void *p, unsigned int nExtraFlags, Char *szOutput, Size cLen, 
                                          Size *pcFinalLen, Size cPrecision)
{
	CX_UNUSED(cPrecision);

	if (Detail::ExtraFlag_HexLower != nExtraFlags && Detail::ExtraFlag_HexUpper != nExtraFlags)
	{
		nExtraFlags = Detail::ExtraFlag_HexUpper;
	}

	Size cAddr = (Size)p;

	return Detail::DetailPrint::HexPrintHelper(&cAddr, sizeof(cAddr), szOutput, cLen, pcFinalLen, true, 
	                                           (Detail::ExtraFlag_HexUpper == nExtraFlags));
}

#endif

namespace Detail
{

namespace DetailPrint
{

class Buffer
{
public:

	static const Size BUFFER_LEN = 4096;

	Buffer()
	{
		Init();
	}

	inline void Init()
	{
		m_cLen = 0;
	}

	template <typename O>
	inline StatusCode Write(O o, const Char *pBuffer, Size cLen)
	{
		Size	cPos;
		Size	cSize;

		cPos = 0;
		while (0 < cLen)
		{
			if (m_cLen + cLen > BUFFER_LEN)
			{
				cSize = BUFFER_LEN - m_cLen;
			}
			else
			{
				cSize = cLen;
			}
			memcpy(m_buffer + m_cLen, pBuffer + cPos, cSize);
			m_cLen += cSize;
			cLen -= cSize;
			cPos += cSize;
			if (BUFFER_LEN == m_cLen)
			{
				StatusCode nStatus;

				if (CXNOK(nStatus = Flush(o)))
				{
					return nStatus;
				}
			}
		}

		return Status_OK;
	}

	template <typename O>
	inline StatusCode Flush(O o)
	{
		if (0 < m_cLen)
		{
			StatusCode nStatus;

			m_buffer[m_cLen] = 0;
			if (CXNOK(nStatus = PrintOutput(o, m_buffer, m_cLen)))
			{
				return nStatus;
			}
			Init();
		}

		return Status_OK;
	}

private:

	Char m_buffer[BUFFER_LEN];
	Size m_cLen;

};

enum Align
{
	Align_Left,
	Align_Right,
	Align_Center,
};

typedef struct _Flags
{
	static const Char    DEFAULT_FILLCHAR   = ' ';
	static const UInt32  DEFAULT_WIDTH      = 0;
	static const UInt32  DEFAULT_PRECISION  = 6;
	static const Align   DEFAULT_ALIGN      = Align_Right;

	Char         cFillChar;
	UInt32       cWidth;
	UInt32       cPrecision;
	Align        nAlign;
	unsigned int nExtraFlags;

	_Flags()
	{
		Init();
	}

	inline void Init()
	{
		cFillChar   = DEFAULT_FILLCHAR;
		cWidth      = DEFAULT_WIDTH;
		cPrecision  = DEFAULT_PRECISION;
		nAlign      = DEFAULT_ALIGN;
		nExtraFlags = 0;
	}
}Flags;


template <typename O, typename T>
class ArgPrinter
{
public:

	static inline StatusCode PrintArg(O o, T p, Flags *pFlags, Char *pBuf, Size cBufLen, Buffer *pBuffer)
	{
		Size       cLen;
		Size       cPreLen;
		Size       cPostLen;
		StatusCode nStatus;

		cLen    = 0;
		nStatus = ToString(p, pFlags->nExtraFlags, pBuf, cBufLen, &cLen, pFlags->cPrecision);

		if (CXNOK(nStatus))
		{
			return Status_InvalidArg;
		}

		if (Align_Left == pFlags->nAlign)
		{
			cPreLen = 0;
			cPostLen = (cLen < (Size)pFlags->cWidth ? (Size)pFlags->cWidth - cLen : 0);
		}
		else
		if (Align_Center == pFlags->nAlign)
		{
			if (cLen < (Size)pFlags->cWidth)
			{
				cPreLen = cPostLen = ((Size)pFlags->cWidth - cLen) / 2;
				cPreLen += ((Size)pFlags->cWidth - cLen) % 2;
			}
			else
			{
				cPreLen = 0;
				cPostLen = 0;
			}
		}
		else
		{
			cPreLen  = (cLen < (Size)pFlags->cWidth ? (Size)pFlags->cWidth - cLen : 0);
			cPostLen = 0;
		}
		for (Size i = 0; i < cPreLen; i++)
		{
			if (CXNOK(nStatus = pBuffer->Write(o, &pFlags->cFillChar, 1)))
			{
				return nStatus;
			}
		}
		if (CXNOK(nStatus = pBuffer->Write(o, pBuf, cLen)))
		{
			return nStatus;
		}
		for (Size i = 0; i < cPostLen; i++)
		{
			if (CXNOK(nStatus = pBuffer->Write(o, &pFlags->cFillChar, 1)))
			{
				return nStatus;
			}
		}

		return Status_OK;
	}

};

template <typename O>
class ArgPrinter<O, Char *>
{
public:

	static inline StatusCode PrintArg(O o, Char *p, Flags *pFlags, Char *pBuf, Size cBufLen, Buffer *pBuffer)
	{
		return ArgPrinter<O, const Char *>::PrintArg(o, p, pFlags, pBuf, cBufLen, pBuffer);
	}

};

template <typename O>
class ArgPrinter<O, const Char *>
{
public:

	static inline StatusCode PrintArg(O o, const Char *p, Flags *pFlags, Char *pBuf, Size cBufLen, Buffer *pBuffer)
	{
		const Char *pPos;
		Size       cLen;
		Size       cPreLen;
		Size       cPostLen;
		StatusCode nStatus;

		CX_UNUSED(pBuf);
		CX_UNUSED(cBufLen);

		cLen    = 0;
		nStatus = Status_NotImplemented;
		pPos    = p;
		while (0 != *pPos)
		{
			cLen++;
			pPos++;
		}

		if (Align_Left == pFlags->nAlign)
		{
			cPreLen = 0;
			cPostLen = (cLen < (Size)pFlags->cWidth ? (Size)pFlags->cWidth - cLen : 0);
		}
		else
		if (Align_Center == pFlags->nAlign)
		{
			if (cLen < (Size)pFlags->cWidth)
			{
				cPreLen = cPostLen = ((Size)pFlags->cWidth - cLen) / 2;
				cPreLen += ((Size)pFlags->cWidth - cLen) % 2;
			}
			else
			{
				cPreLen = 0;
				cPostLen = 0;
			}
		}
		else
		{
			cPreLen = (cLen < (Size)pFlags->cWidth ? (Size)pFlags->cWidth - cLen : 0);
			cPostLen = 0;
		}
		for (Size i = 0; i < cPreLen; i++)
		{
			if (CXNOK(nStatus = pBuffer->Write(o, &pFlags->cFillChar, 1)))
			{
				return nStatus;
			}
		}
		if (CXNOK(nStatus = pBuffer->Write(o, p, cLen)))
		{
			return nStatus;
		}
		for (Size i = 0; i < cPostLen; i++)
		{
			if (CXNOK(nStatus = pBuffer->Write(o, &pFlags->cFillChar, 1)))
			{
				return nStatus;
			}
		}

		return Status_OK;
	}

};

template <typename O>
class ArgPrinter<O, String>
{
public:

	static inline StatusCode PrintArg(O o, String p, Flags *pFlags, Char *pBuf, Size cBufLen, Buffer *pBuffer)
	{
		return ArgPrinter<O, const Char *>::PrintArg(o, p.c_str(), pFlags, pBuf, cBufLen, pBuffer);
	}

};

template <typename O>
class ArgPrinter<O, String &>
{
public:

	static inline StatusCode PrintArg(O o, String &p, Flags *pFlags, Char *pBuf, Size cBufLen, Buffer *pBuffer)
	{
		return ArgPrinter<O, const Char *>::PrintArg(o, p.c_str(), pFlags, pBuf, cBufLen, pBuffer);
	}

};

template <typename O>
class ArgPrinter<O, const String>
{
public:

	static inline StatusCode PrintArg(O o, const String p, Flags *pFlags, Char *pBuf, Size cBufLen, Buffer *pBuffer)
	{
		return ArgPrinter::PrintArg(o, p.c_str(), pFlags, pBuf, cBufLen, pBuffer);
	}

};

template <typename O>
class ArgPrinter<O, const String &>
{
public:

	static inline StatusCode PrintArg(O o, const String &p, Flags *pFlags, Char *pBuf, Size cBufLen, Buffer *pBuffer)
	{
		return ArgPrinter::PrintArg(o, p.c_str(), pFlags, pBuf, cBufLen, pBuffer);
	}

};

template <typename O, typename T>
inline StatusCode PrintArgHelper(O o, T p, Flags *pFlags, Char *pBuf, Size cBufLen, Buffer *pBuffer)
{
	return ArgPrinter<O, T>::PrintArg(o, p, pFlags, pBuf, cBufLen, pBuffer);
}

//arg format = {$(argpos)[:[extraflags:][<|>]['$(fillchr)']]$(width)][.$(precision)]]}
//extraflags: 'x' = hex print lowercase - ignored on non integral types or non pointers 
//extraflags: 'X' = hex print uppercase - ignored on non integral types or non pointers 
//align = < (left), > (right - default), | (center)
//fillchr = default ' '
//width = default 0 (= arg width)
//precision = default 6
template <Size ARGC, typename O, 
          typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, 
          typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, 
          typename T13, typename T14, typename T15,	typename T16>
inline StatusCode Print(O o, const Char *szFormat, 
                               T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, T8 p8, T9 p9, 
                               T10 p10, T11 p11, T12 p12, T13 p13, T14 p14, T15 p15, T16 p16)
{
	const Char  *pszStart;
	const Char  *pszPos;
	Flags       flags;
	Buffer      buffer;
	Size        cIndex;
	UInt32      cWidth;
	UInt32      cPrec;
	Char        buf[8192];
	StatusCode  nStatus;

	pszPos    = szFormat;
	pszStart  = szFormat;

	for (;;)
	{
		while ('{' != *pszPos && '\0' != *pszPos)
		{
			pszPos++;
		}
		if (pszPos > pszStart)
		{
			if (CXNOK(nStatus = buffer.Write(o, pszStart, pszPos - pszStart)))
			{
				return nStatus;
			}
		}
		if ('{' == *pszPos)
		{
			pszStart = pszPos;
			pszPos++;
			if ('{' == *pszPos)
			{
				pszPos++;
				pszStart = pszPos;
				if (CXNOK(nStatus = buffer.Write(o, "{", 1)))
				{
					return nStatus;
				}
				continue;
			}
			else
			{
				if (!cx_isdigit((unsigned char)*pszPos))
				{
					return Status_InvalidArg;
				}
			}

			const Char *pszBeginIndex = pszPos;

			cIndex = 0;
			while (cx_isdigit((unsigned char)*pszPos))
			{
				cIndex *= 10;
				cIndex += (Size)(*pszPos - '0');
				pszPos++;
			}

			const Char *pszEndIndex = pszPos;

			flags.Init();

			if (':' == *pszPos)
			{
				pszPos++;

				if ('x' == *pszPos)
				{
					pszPos++;
					if (':' == *pszPos)
					{
						pszPos++;
					}
					else
					if ('}' != *pszPos)
					{
						return Status_InvalidArg;
					}
					flags.nExtraFlags = ExtraFlag_HexLower;
				}
				else
				if ('X' == *pszPos)
				{
					pszPos++;
					if (':' == *pszPos)
					{
						pszPos++;
					}
					else
					if ('}' != *pszPos)
					{
						return Status_InvalidArg;
					}
					flags.nExtraFlags = ExtraFlag_HexUpper;
				}

				// align
				if ('<' == *pszPos)
				{
					pszPos++;
					flags.nAlign = Align_Left;
				}
				else
				if ('>' == *pszPos)
				{
					pszPos++;
					flags.nAlign = Align_Right;
				}
				else
				if ('|' == *pszPos)
				{
					pszPos++;
					flags.nAlign = Align_Center;
				}

				//fill char
				if ('\'' == *pszPos)
				{
					pszPos++;
					if ('\0' == *pszPos)
					{
						return Status_InvalidArg;
					}
					flags.cFillChar = *pszPos;
					pszPos++;
					if ('\'' != *pszPos)
					{
						return Status_InvalidArg;
					}
					pszPos++;
				}
				else
				{
					flags.cFillChar = Flags::DEFAULT_FILLCHAR;
				}

				//width
				if (cx_isdigit((unsigned char)*pszPos))
				{
					cWidth = 0;
					while (cx_isdigit((unsigned char)*pszPos))
					{
						cWidth *= 10;
						cWidth += *pszPos - '0';
						pszPos++;
					}
					flags.cWidth = cWidth;
				}

				//precision
				if ('.' == *pszPos)
				{
					pszPos++;
					cPrec = 0;
					while (cx_isdigit((unsigned char)*pszPos))
					{
						cPrec *= 10;
						cPrec += *pszPos - '0';
						pszPos++;
					}
					flags.cPrecision = cPrec;
				}
			}

			if ('}' == *pszPos)
			{
				pszPos++;
				
				if (0 == cIndex || cIndex > ARGC)
				{
					if (CXNOK(nStatus = buffer.Write(o, "{", 1)))
					{
						return nStatus;
					}
					if (pszEndIndex > pszBeginIndex)
					{
						if (CXNOK(nStatus = buffer.Write(o, pszBeginIndex, pszEndIndex - pszBeginIndex)))
						{
							return nStatus;
						}
					}
					if (CXNOK(nStatus = buffer.Write(o, "}", 1)))
					{
						return nStatus;
					}
				}
				else
				{
					cIndex--;

					nStatus = Status_NotImplemented;
					if (0 == cIndex) { nStatus = PrintArgHelper(o, p1, &flags, buf, sizeof(buf), &buffer); }
					else if (1 == cIndex) { nStatus = PrintArgHelper(o, p2, &flags, buf, sizeof(buf), &buffer); }
					else if (2 == cIndex) { nStatus = PrintArgHelper(o, p3, &flags, buf, sizeof(buf), &buffer); }
					else if (3 == cIndex) { nStatus = PrintArgHelper(o, p4, &flags, buf, sizeof(buf), &buffer); }
					else if (4 == cIndex) { nStatus = PrintArgHelper(o, p5, &flags, buf, sizeof(buf), &buffer); }
					else if (5 == cIndex) { nStatus = PrintArgHelper(o, p6, &flags, buf, sizeof(buf), &buffer); }
					else if (6 == cIndex) { nStatus = PrintArgHelper(o, p7, &flags, buf, sizeof(buf), &buffer); }
					else if (7 == cIndex) { nStatus = PrintArgHelper(o, p8, &flags, buf, sizeof(buf), &buffer); }
					else if (8 == cIndex) { nStatus = PrintArgHelper(o, p9, &flags, buf, sizeof(buf), &buffer); }
					else if (9 == cIndex) { nStatus = PrintArgHelper(o, p10, &flags, buf, sizeof(buf), &buffer); }
					else if (10 == cIndex) { nStatus = PrintArgHelper(o, p11, &flags, buf, sizeof(buf), &buffer); }
					else if (11 == cIndex) { nStatus = PrintArgHelper(o, p12, &flags, buf, sizeof(buf), &buffer); }
					else if (12 == cIndex) { nStatus = PrintArgHelper(o, p13, &flags, buf, sizeof(buf), &buffer); }
					else if (13 == cIndex) { nStatus = PrintArgHelper(o, p14, &flags, buf, sizeof(buf), &buffer); }
					else if (14 == cIndex) { nStatus = PrintArgHelper(o, p15, &flags, buf, sizeof(buf), &buffer); }
					else if (15 == cIndex) { nStatus = PrintArgHelper(o, p16, &flags, buf, sizeof(buf), &buffer); }

					if (CXNOK(nStatus))
					{
						return nStatus;
					}
				}
				pszStart = pszPos;
			}
			else
			{
				return Status_InvalidArg;
			}
		}
		else
		if ('\0' == *pszPos)
		{
			break;
		}
	}
	buffer.Flush(o);

	return Status_OK;
}

}//namespace Detail

}//namespace DetailPrint

}//namespace CX

