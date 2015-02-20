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

#pragma once


#include "CX/Types.hpp"
#include "CX/APIDefs.hpp"
#include "CX/StatusCodes.hpp"
#include "CX/String.hpp"
#include "CX/EmptyType.hpp"
#include "CX/Slice.hpp"
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

namespace DetailPrint
{

CX_API StatusCode WriteStream(IO::IOutputStream *pOutputStream, const Char *pBuffer, Size cLen);

}//namespace DetailPrint

}//namespace Detail

#ifndef CX_USE_CUSTOM_PRINTOUTPUT

template <typename T> 
static inline StatusCode PrintOutput(T o, const Char *pBuffer, Size cLen);

template <> 
static inline StatusCode PrintOutput<FILE *>(FILE *pFile, const Char *pBuffer, Size cLen)
{
	if (cLen * sizeof(Char) != fwrite(pBuffer, 1, cLen * sizeof(Char), pFile))
	{
		return Status_WriteFailed;
	}

	return Status_OK;
}

template <>
static inline StatusCode PrintOutput<IO::IOutputStream *>(IO::IOutputStream *pOutputStream,
                                                          const Char *pBuffer, Size cLen)
{
	return Detail::DetailPrint::WriteStream(pOutputStream, pBuffer, cLen);
}

template <>
static inline StatusCode PrintOutput<String *>(String *psStr, const Char *pBuffer, Size cLen)
{
	psStr->append(pBuffer, cLen);

	return Status_OK;
}

template <>
static inline StatusCode PrintOutput<Slice *>(Slice *pSlice, const Char *pBuffer, Size cLen)
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
static inline StatusCode PrintOutput<Slice &>(Slice &slice, const Char *pBuffer, Size cLen)
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

static inline Bool StrCopy(CX_Char *szDst, CX_Size cDstLen, const CX_Char *szSrc,
                           CX_Size *pcSrcLen)
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

CX_API Bool DoubleToString(Double lfValue, Char *szOutput, Size cLen, Size cPrecision);

}//namespace DetailPrint

}//namespace Detail

template <typename T>
static inline StatusCode ToString(T p, Char *szOutput, Size cLen, Size *pcFinalLen, 
                                  Size cPrecision);

template <>
static inline StatusCode ToString<EmptyType>(EmptyType p, Char *szOutput, Size cLen, 
                                             Size *pcFinalLen, Size cPrecision)
{
	p;
	szOutput;
	cLen;
	pcFinalLen;
	cPrecision;

	return Status_NotSupported;
}

template <>
static inline StatusCode ToString<Char>(Char p, Char *szOutput, Size cLen, Size *pcFinalLen, 
                                        Size cPrecision)
{
	cPrecision;

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
static inline StatusCode ToString<Bool>(Bool p, Char *szOutput, Size cLen, Size *pcFinalLen, 
                                        Size cPrecision)
{
	cPrecision;

	if (p)
	{
		if (cLen < 5)
		{
			return Status_TooSmall;
		}
		memcpy(szOutput, "True", 4);
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
		memcpy(szOutput, "False", 5);
		szOutput[5]	= 0;
		*pcFinalLen = 5;

		return Status_OK;
	}
}

template <>
static inline StatusCode ToString<Int8>(Int8 p, Char *szOutput, Size cLen, Size *pcFinalLen, 
                                        Size cPrecision)
{
	cPrecision;

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

template <>
static inline StatusCode ToString<UInt8>(UInt8 p, Char *szOutput, Size cLen, Size *pcFinalLen, 
                                         Size cPrecision)
{
	cPrecision;

	*pcFinalLen = Detail::DetailPrint::GetUInt64DigitsCount((UInt64)p);
	if (*pcFinalLen + 1 > cLen)
	{
		return Status_TooSmall;
	}
	Detail::DetailPrint::UInt64ToString((UInt64)p, szOutput, *pcFinalLen);
	szOutput[*pcFinalLen] = 0;

	return Status_OK;
}

template <>
static inline StatusCode ToString<Int16>(Int16 p, Char *szOutput, Size cLen, Size *pcFinalLen, 
                                         Size cPrecision)
{
	cPrecision;

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

template <>
static inline StatusCode ToString<UInt16>(UInt16 p, Char *szOutput, Size cLen, Size *pcFinalLen, 
                                          Size cPrecision)
{
	cPrecision;

	*pcFinalLen = Detail::DetailPrint::GetUInt64DigitsCount((UInt64)p);
	if (*pcFinalLen + 1 > cLen)
	{
		return Status_TooSmall;
	}
	Detail::DetailPrint::UInt64ToString((UInt64)p, szOutput, *pcFinalLen);
	szOutput[*pcFinalLen] = 0;

	return Status_OK;
}

template <>
static inline StatusCode ToString<Int32>(Int32 p, Char *szOutput, Size cLen, Size *pcFinalLen, 
                                         Size cPrecision)
{
	cPrecision;

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

template <>
static inline StatusCode ToString<UInt32>(UInt32 p, Char *szOutput, Size cLen, Size *pcFinalLen, 
                                          Size cPrecision)
{
	cPrecision;

	*pcFinalLen = Detail::DetailPrint::GetUInt64DigitsCount((UInt64)p);
	if (*pcFinalLen + 1 > cLen)
	{
		return Status_TooSmall;
	}
	Detail::DetailPrint::UInt64ToString((UInt64)p, szOutput, *pcFinalLen);
	szOutput[*pcFinalLen] = 0;

	return Status_OK;
}

template <>
static inline StatusCode ToString<Int64>(Int64 p, Char *szOutput, Size cLen, Size *pcFinalLen, 
                                         Size cPrecision)
{
	cPrecision;

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

template <>
static inline StatusCode ToString<UInt64>(UInt64 p, Char *szOutput, Size cLen, Size *pcFinalLen, 
                                          Size cPrecision)
{
	cPrecision;

	*pcFinalLen = Detail::DetailPrint::GetUInt64DigitsCount((UInt64)p);
	if (*pcFinalLen + 1 > cLen)
	{
		return Status_TooSmall;
	}
	Detail::DetailPrint::UInt64ToString((UInt64)p, szOutput, *pcFinalLen);
	szOutput[*pcFinalLen] = 0;

	return Status_OK;
}

template <>
static inline StatusCode ToString<Float>(Float p, Char *szOutput, Size cLen, Size *pcFinalLen, 
                                         Size cPrecision)
{
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
static inline StatusCode ToString<Double>(Double p, Char *szOutput, Size cLen, Size *pcFinalLen, 
                                          Size cPrecision)
{
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
static inline StatusCode ToString<long>(long p, Char *szOutput, Size cLen, Size *pcFinalLen, 
                                        Size cPrecision)
{
	cPrecision;

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

template <>
static inline StatusCode ToString<unsigned long>(unsigned long p, Char *szOutput, Size cLen, 
                                                 Size *pcFinalLen, Size cPrecision)
{
	cPrecision;

	*pcFinalLen = Detail::DetailPrint::GetUInt64DigitsCount((UInt64)p);
	if (*pcFinalLen + 1 > cLen)
	{
		return Status_TooSmall;
	}
	Detail::DetailPrint::UInt64ToString((UInt64)p, szOutput, *pcFinalLen);
	szOutput[*pcFinalLen] = 0;

	return Status_OK;
}

template <>
static inline StatusCode ToString<const Char *>(const Char *p, Char *szOutput, Size cLen, 
                                                Size *pcFinalLen, Size cPrecision)
{
	cPrecision;

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
static inline StatusCode ToString<Char *>(Char *p, Char *szOutput, Size cLen, Size *pcFinalLen, 
                                          Size cPrecision)
{
	return ToString<const Char *>(p, szOutput, cLen, pcFinalLen, cPrecision);
}

template <>
static inline StatusCode ToString<const String &>(const String &p, Char *szOutput, Size cLen, 
                                                  Size *pcFinalLen, Size cPrecision)
{
	cPrecision;

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
static inline StatusCode ToString<String &>(String &p, Char *szOutput, Size cLen, Size *pcFinalLen, 
                                            Size cPrecision)
{
	return ToString<const String &>(p, szOutput, cLen, pcFinalLen, cPrecision);
}

template <>
static inline StatusCode ToString<String>(String p, Char *szOutput, Size cLen, Size *pcFinalLen, 
                                          Size cPrecision)
{
	return ToString<const String &>(p, szOutput, cLen, pcFinalLen, cPrecision);
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

	Char     cFillChar;
	UInt32   cWidth;
	UInt32   cPrecision;
	Align    nAlign;

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
	}
}Flags;

//arg format = {$(argpos)[:[<|>]['$(fillchr)']]$(width)][.$(precision)]]}
//align = < (left), > (right - default), | (center)
//fillchr = default ' '
//width = default 0 (= arg width)
//precision = default 6
template <Size ARGC, typename O, 
          typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, 
          typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, 
          typename T13, typename T14, typename T15,	typename T16>
static inline StatusCode Print(O o, const Char *szFormat, 
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
	Size        cLen;
	Size        cPreLen;
	Size        cPostLen;
	Char        buf[1024];
	StatusCode  nStatus;

	pszPos   = szFormat;
	pszStart = szFormat;

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
			}
			else
			{
				if (!cx_isdigit(*pszPos))
				{
					return Status_InvalidArg;
				}
			}
			cIndex = 0;
			while (cx_isdigit(*pszPos))
			{
				cIndex *= 10;
				cIndex += *pszPos - '0';
				pszPos++;
			}
			if (0 == cIndex || cIndex > ARGC)
			{
				return Status_InvalidArg;
			}
			cIndex--;

			flags.Init();

			if (':' == *pszPos)
			{
				pszPos++;

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
				if (cx_isdigit(*pszPos))
				{
					cWidth = 0;
					while (cx_isdigit(*pszPos))
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
					while (cx_isdigit(*pszPos))
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
				
				cLen    = 0;
				nStatus = Status_NotImplemented;
				if (0 == cIndex) { nStatus = ToString(p1, buf, sizeof(buf), &cLen, flags.cPrecision); }
				else if (1 == cIndex) { nStatus = ToString(p2, buf, sizeof(buf), &cLen, flags.cPrecision); }
				else if (2 == cIndex) { nStatus = ToString(p3, buf, sizeof(buf), &cLen, flags.cPrecision); }
				else if (3 == cIndex) { nStatus = ToString(p4, buf, sizeof(buf), &cLen, flags.cPrecision); }
				else if (4 == cIndex) { nStatus = ToString(p5, buf, sizeof(buf), &cLen, flags.cPrecision); }
				else if (5 == cIndex) { nStatus = ToString(p6, buf, sizeof(buf), &cLen, flags.cPrecision); }
				else if (6 == cIndex) { nStatus = ToString(p7, buf, sizeof(buf), &cLen, flags.cPrecision); }
				else if (7 == cIndex) { nStatus = ToString(p8, buf, sizeof(buf), &cLen, flags.cPrecision); }
				else if (8 == cIndex) { nStatus = ToString(p9, buf, sizeof(buf), &cLen, flags.cPrecision); }
				else if (9 == cIndex) { nStatus = ToString(p10, buf, sizeof(buf), &cLen, flags.cPrecision); }
				else if (10 == cIndex) { nStatus = ToString(p11, buf, sizeof(buf), &cLen, flags.cPrecision); }
				else if (11 == cIndex) { nStatus = ToString(p12, buf, sizeof(buf), &cLen, flags.cPrecision); }
				else if (12 == cIndex) { nStatus = ToString(p13, buf, sizeof(buf), &cLen, flags.cPrecision); }
				else if (13 == cIndex) { nStatus = ToString(p14, buf, sizeof(buf), &cLen, flags.cPrecision); }
				else if (14 == cIndex) { nStatus = ToString(p15, buf, sizeof(buf), &cLen, flags.cPrecision); }
				else if (15 == cIndex) { nStatus = ToString(p16, buf, sizeof(buf), &cLen, flags.cPrecision); }
				
				if (CXNOK(nStatus))
				{
					return Status_InvalidArg;
				}

				if (Align_Left == flags.nAlign)
				{
					cPreLen  = 0;
					cPostLen = (cLen < flags.cWidth ? flags.cWidth - cLen : 0);
				}
				else
				if (Align_Center == flags.nAlign)
				{
					if (cLen < flags.cWidth)
					{
						cPreLen = cPostLen = (flags.cWidth - cLen) / 2;
						cPreLen += (flags.cWidth - cLen) % 2;
					}
					else
					{
						cPreLen  = 0;
						cPostLen = 0;
					}
				}
				else
				{
					cPreLen  = (cLen < flags.cWidth ? flags.cWidth - cLen : 0);
					cPostLen = 0;
				}
				for (Size i = 0; i < cPreLen; i++)
				{
					if (CXNOK(nStatus = buffer.Write(o, &flags.cFillChar, 1)))
					{
						return nStatus;
					}
				}
				if (CXNOK(nStatus = buffer.Write(o, buf, cLen)))
				{
					return nStatus;
				}
				for (Size i = 0; i < cPostLen; i++)
				{
					if (CXNOK(nStatus = buffer.Write(o, &flags.cFillChar, 1)))
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

