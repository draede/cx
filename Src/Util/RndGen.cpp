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
	m_pState32 = new tinymt32_t();
	m_pState64 = new tinymt64_t();

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

Bool RndGen::GetBool()
{
	return (0 == (GetUInt32() % 2));
}

Char RndGen::GetChar(const Char *szCharset/* = NULL*/)
{
	static const Char charset[] = 
	"!\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~";

	if (NULL == szCharset)
	{
		szCharset = charset;
	}

	Size cChars = cx_strlen(szCharset);

	return szCharset[GetSize() % cChars];
}

WChar RndGen::GetWChar(const WChar *wszCharset/* = NULL*/)
{
	static const WChar charset[] =
		L"!\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~";

	if (NULL == wszCharset)
	{
		wszCharset = charset;
	}

	Size cChars = cxw_strlen(wszCharset);

	return wszCharset[GetSize() % cChars];
}

Char RndGen::GetCharRange(Char nMin, Char nMax)
{
	if (nMin >= nMax)
	{
		return nMin;
	}
	else
	{
		return (nMin + (GetChar() % (nMax - nMin + 1)));
	}
}

Int8 RndGen::GetInt8()
{
	return (Int8)(GetUInt8());
}

Int8 RndGen::GetInt8Range(Int8 nMin, Int8 nMax)
{
	if (nMin >= nMax)
	{
		return nMin;
	}
	else
	{
		return (nMin + (GetInt8() % (nMax - nMin + 1)));
	}
}

UInt8 RndGen::GetUInt8()
{
	return (GetUInt32() % CX::TYPE_UINT8_MAX);
}

UInt8 RndGen::GetUInt8Range(UInt8 nMin, UInt8 nMax)
{
	if (nMin >= nMax)
	{
		return nMin;
	}
	else
	{
		return (nMin + (GetUInt8() % (nMax - nMin + 1)));
	}
}

Int16 RndGen::GetInt16()
{
	return (Int16)(GetUInt16());
}

Int16 RndGen::GetInt16Range(Int16 nMin, Int16 nMax)
{
	if (nMin >= nMax)
	{
		return nMin;
	}
	else
	{
		return (nMin + (GetInt16() % (nMax - nMin + 1)));
	}
}

UInt16 RndGen::GetUInt16()
{
	return (GetUInt32() % TYPE_UINT16_MAX);
}

UInt16 RndGen::GetUInt16Range(UInt16 nMin, UInt16 nMax)
{
	if (nMin >= nMax)
	{
		return nMin;
	}
	else
	{
		return (nMin + (GetUInt16() % (nMax - nMin + 1)));
	}
}

Int32 RndGen::GetInt32()
{
	return (Int32)(GetUInt32());
}

Int32 RndGen::GetInt32Range(Int32 nMin, Int32 nMax)
{
	if (nMin >= nMax)
	{
		return nMin;
	}
	else
	{
		return (nMin + (GetInt32() % (nMax - nMin + 1)));
	}
}

UInt32 RndGen::GetUInt32()
{
	return tinymt32_generate_uint32((tinymt32_t *)m_pState32);
}

UInt32 RndGen::GetUInt32Range(UInt32 nMin, UInt32 nMax)
{
	if (nMin >= nMax)
	{
		return nMin;
	}
	else
	{
		return (nMin + (GetUInt32() % (nMax - nMin + 1)));
	}
}

Int64 RndGen::GetInt64()
{
	return (Int64)(GetUInt64());
}

Int64 RndGen::GetInt64Range(Int64 nMin, Int64 nMax)
{
	if (nMin >= nMax)
	{
		return nMin;
	}
	else
	{
		return (nMin + (GetInt64() % (nMax - nMin + 1)));
	}
}

UInt64 RndGen::GetUInt64()
{
	return tinymt64_generate_uint64((tinymt64_t *)m_pState64);
}

UInt64 RndGen::GetUInt64Range(UInt64 nMin, UInt64 nMax)
{
	if (nMin >= nMax)
	{
		return nMin;
	}
	else
	{
		return (nMin + (GetUInt64() % (nMax - nMin + 1)));
	}
}

Float RndGen::GetFloat()
{
	return tinymt32_generate_float((tinymt32_t *)m_pState32);
}

Double RndGen::GetDouble()
{
	return tinymt64_generate_double((tinymt64_t *)m_pState64);
}

Size RndGen::GetSize()
{
	return (Size)GetUInt64();
}

void RndGen::GetString(String *psStr, Size cMinLen, Size cMaxLen, const Char *szCharset/* = NULL*/)
{
	Size cLen = cMinLen + (GetSize() % (cMaxLen - cMinLen + 1));

	psStr->clear();
	for (Size i = 0; i < cLen; i++)
	{
		*psStr += GetChar(szCharset);
	}
}

void RndGen::GetWString(WString *pwsStr, Size cMinLen, Size cMaxLen, const WChar *wszCharset/* = NULL*/)
{
	Size cLen = cMinLen + (GetSize() % (cMaxLen - cMinLen + 1));

	pwsStr->clear();
	for (Size i = 0; i < cLen; i++)
	{
		*pwsStr += GetWChar(wszCharset);
	}
}

const Char *RndGen::GetString(Size cMinLen, Size cMaxLen, const Char *szCharset/* = NULL*/)
{
	static String sTmp;

	GetString(&sTmp, cMinLen, cMaxLen, szCharset);

	return sTmp.c_str();
}

const WChar *RndGen::GetWString(Size cMinLen, Size cMaxLen, const WChar *wszCharset/* = NULL*/)
{
	static WString wsTmp;

	GetWString(&wsTmp, cMinLen, cMaxLen, wszCharset);

	return wsTmp.c_str();
}


}//namespace Util

}//namespace CX
