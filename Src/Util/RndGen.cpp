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

#include "CX/Util/RndGen.hpp"
#include "../../Contrib/TinyMT/Include/tinymt32.h"
#include "../../Contrib/TinyMT/Include/tinymt64.h"
#include "CX/Util/Time.hpp"
#include "CX/Limits.hpp"


namespace CX
{

namespace Util
{

RndGen::RndGen()
{
	m_pState32 = new (std::nothrow) tinymt32_t();
	m_pState64 = new (std::nothrow) tinymt64_t();

	Time t;

	Seed32((UInt32)(((UInt64)this) / 23 + t.GetTimeStampMS() / 17));
	Seed64((UInt64)(((UInt64)this) / 13 + t.GetTimeStampMS() / 29));
}

RndGen::~RndGen()
{
	delete (tinymt32_t *)m_pState32;
	delete (tinymt64_t *)m_pState64;
}

RndGen &RndGen::Get()
{
	static RndGen rndgen;

	return rndgen;
}

void RndGen::Seed32(UInt32 nSeed)
{
	tinymt32_init((tinymt32_t *)m_pState32, nSeed);
}

void RndGen::Seed64(UInt64 nSeed)
{
	tinymt64_init((tinymt64_t *)m_pState64, nSeed);
}

UInt32 RndGen::GetRange32(UInt32 cValues)
{
	return GetUInt32() / (TYPE_UINT32_MAX / cValues);
}

UInt64 RndGen::GetRange64(UInt64 cValues)
{
	return GetUInt64() / (TYPE_UINT64_MAX / cValues);
}

UInt32 RndGen::GetUInt32()
{
	return tinymt32_generate_uint32((tinymt32_t *)m_pState32);
}

UInt64 RndGen::GetUInt64()
{
	return tinymt64_generate_uint64((tinymt64_t *)m_pState64);
}

UInt8 RndGen::GetUInt8()
{
	return (UInt8)GetRange32(TYPE_UINT8_MAX);
}

UInt16 RndGen::GetUInt16()
{
	return (UInt16)GetRange32(TYPE_UINT16_MAX);
}

Size RndGen::GetSize()
{
#ifdef CX_64BIT_ARCH
	return (Size)GetUInt64();
#else
	return (Size)GetUInt32();
#endif
}

UInt8 RndGen::GetUInt8Range(UInt8 nMin, UInt8 nMax)
{
	if (nMax <= nMin)
	{
		return nMin;
	}

	return nMin + (UInt8)GetRange32((nMax - nMin) + 1);
}

UInt16 RndGen::GetUInt16Range(UInt16 nMin, UInt16 nMax)
{
	if (nMax <= nMin)
	{
		return nMin;
	}

	return nMin + (UInt16)GetRange32((nMax - nMin) + 1);
}

UInt32 RndGen::GetUInt32Range(UInt32 nMin, UInt32 nMax)
{
	if (nMax <= nMin)
	{
		return nMin;
	}

	return nMin + GetRange32((nMax - nMin) + 1);
}

UInt64 RndGen::GetUInt64Range(UInt64 nMin, UInt64 nMax)
{
	if (nMax <= nMin)
	{
		return nMin;
	}

	return nMin + GetRange64((nMax - nMin) + 1);
}

Size RndGen::GetSizeRange(Size nMin, Size nMax)
{
#ifdef CX_64BIT_ARCH
	return (Size)GetUInt64Range(nMin, nMax);
#else
	return (Size)GetUInt32Range(nMin, nMax);
#endif
}

Bool RndGen::GetBool()
{
	return (1 == GetRange32(2));
}

Float RndGen::GetFloat()
{
	return tinymt32_generate_float((tinymt32_t *)m_pState32);
}

Double RndGen::GetDouble()
{
	return tinymt64_generate_double((tinymt64_t *)m_pState64);
}

Char RndGen::GetChar(const Char *szCharset/* = NULL*/)
{
	static const Char charset[] =
	"!\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~";

	if (NULL == szCharset)
	{
		szCharset = charset;
	}

	UInt32 cChars = (UInt32)cx_strlen(szCharset);

	return szCharset[GetRange32(cChars)];
}

WChar RndGen::GetWChar(const WChar *wszCharset/* = NULL*/)
{
	static const WChar charset[] =
	L"!\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~";

	if (NULL == wszCharset)
	{
		wszCharset = charset;
	}

	UInt32 cChars = (UInt32)cxw_strlen(wszCharset);

	return wszCharset[GetRange32(cChars)];
}

Byte RndGen::GetByte(Byte *pByteSet/* = NULL*/, Size cByteSetSize/* = 0*/)
{
	if (NULL == pByteSet || 0 == cByteSetSize)
	{
		return GetUInt8();
	}
	else
	{
		return pByteSet[GetRange32((UInt32)cByteSetSize)];
	}
}

void RndGen::GetString(Char *pStr, Size cSize, const Char *szCharset/* = NULL*/)
{
	for (Size i = 0; i < cSize; i++)
	{
		pStr[i] = GetChar(szCharset);
	}
}

void RndGen::GetWString(WChar *pwStr, Size cSize, const WChar *wszCharset/* = NULL*/)
{
	for (Size i = 0; i < cSize; i++)
	{
		pwStr[i] = GetWChar(wszCharset);
	}
}

void RndGen::GetBytes(Byte *pBuffer, Size cSize, Byte *pByteSet/* = NULL*/, Size cByteSetSize/* = 0*/)
{
	for (Size i = 0; i < cSize; i++)
	{
		pBuffer[i] = GetByte(pByteSet, cByteSetSize);
	}
}

void RndGen::GetString(String *psStr, Size cSize, const Char *szCharset/* = NULL*/)
{
	for (Size i = 0; i < cSize; i++)
	{
		*psStr += GetChar(szCharset);
	}
}

void RndGen::GetWString(WString *pwsStr, Size cSize, const WChar *wszCharset/* = NULL*/)
{
	for (Size i = 0; i < cSize; i++)
	{
		*pwsStr += GetWChar(wszCharset);
	}
}

void RndGen::GetBytes(BLOB *pBuffer, Size cSize, Byte *pByteSet/* = NULL*/, Size cByteSetSize/* = 0*/)
{
	for (Size i = 0; i < cSize; i++)
	{
		pBuffer->push_back(GetByte(pByteSet, cByteSetSize));
	}
}

}//namespace Util

}//namespace CX
