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


namespace CX
{

namespace Util
{

RndGen::RndGen()
{
	m_pState32 = New<tinymt32_t>();
	m_pState64 = New<tinymt64_t>();
	Seed32((UInt32)this);
	Seed64((UInt64)this);
}

RndGen::~RndGen()
{
	Delete((tinymt32_t *)m_pState32);
	Delete((tinymt64_t *)m_pState64);
}

RndGen &RndGen::GetInstance()
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

UInt32 RndGen::GetRandUInt32()
{
	return tinymt32_generate_uint32((tinymt32_t *)m_pState32);
}

Float RndGen::GetRandFloat()
{
	return tinymt32_generate_float((tinymt32_t *)m_pState32);
}

UInt64 RndGen::GetRandUInt64()
{
	return tinymt64_generate_uint64((tinymt64_t *)m_pState64);
}

Double RndGen::GetRandDouble()
{
	return tinymt64_generate_double((tinymt64_t *)m_pState64);
}

}//namespace Util

}//namespace CX
