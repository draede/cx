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

#include "CX/Util/RndGen.hpp"
#include "../../Contrib/TinyMT/Include/tinymt32.h"
#include "../../Contrib/TinyMT/Include/tinymt64.h"
#include "CX/Util/Timer.hpp"
#include "CX/Limits.hpp"


namespace CX
{

namespace Util
{

RndGen::RndGen()
{
	m_pState32 = New<tinymt32_t>();
	m_pState64 = New<tinymt64_t>();

	Timer timer;

	Seed32((UInt32)(((UInt64)this) / 23 + timer.GetTimeStamp() / 17));
	Seed64((UInt64)(((UInt64)this) / 13 + timer.GetTimeStamp() / 29));
}

RndGen::~RndGen()
{
	Delete((tinymt32_t *)m_pState32);
	Delete((tinymt64_t *)m_pState64);
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

	return szCharset[GetUInt32() % cChars];
}

Int8 RndGen::GetInt8()
{
	return (Int8)(GetUInt8());
}

UInt8 RndGen::GetUInt8()
{
	return (GetUInt32() % CX::TYPE_UINT8_MAX);
}

Int16 RndGen::GetInt16()
{
	return (Int16)(GetUInt16());
}

UInt16 RndGen::GetUInt16()
{
	return (GetUInt32() % TYPE_UINT16_MAX);
}

Int32 RndGen::GetInt32()
{
	return (Int32)(GetUInt32());
}

UInt32 RndGen::GetUInt32()
{
	return tinymt32_generate_uint32((tinymt32_t *)m_pState32);
}

Int64 RndGen::GetInt64()
{
	return (Int64)(GetUInt64());
}

UInt64 RndGen::GetUInt64()
{
	return tinymt64_generate_uint64((tinymt64_t *)m_pState64);
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

}//namespace Util

}//namespace CX
