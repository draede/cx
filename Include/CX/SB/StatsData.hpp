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
#include "CX/IObject.hpp"


namespace CX
{

namespace SB
{

class CX_API StatsData : public IObject
{
public:

	Size m_cMembers;
	Size m_cItems;
	Size m_cObjects;
	Size m_cArrays;
	Size m_cBools;
	Size m_cInt8s;
	Size m_cUInt8s;
	Size m_cInt16s;
	Size m_cUInt16s;
	Size m_cInt32s;
	Size m_cUInt32s;
	Size m_cInt64s;
	Size m_cUInt64s;
	Size m_cFloats;
	Size m_cDoubles;
	Size m_cStrings;
	Size m_cWStrings;
	Size m_cAllValues;
	Size m_cbDataSize;
	Size m_cbTotalSize;

	StatsData()
	{
		Reset();
	}

	void Reset()
	{
		m_cMembers    = 0;
		m_cItems      = 0;
		m_cObjects    = 0;
		m_cArrays     = 0;
		m_cBools      = 0;
		m_cInt8s      = 0;
		m_cUInt8s     = 0;
		m_cInt16s     = 0;
		m_cUInt16s    = 0;
		m_cInt32s     = 0;
		m_cUInt32s    = 0;
		m_cInt64s     = 0;
		m_cUInt64s    = 0;
		m_cFloats     = 0;
		m_cDoubles    = 0;
		m_cStrings    = 0;
		m_cWStrings   = 0;
		m_cAllValues  = 0;
		m_cbDataSize  = 0;
		m_cbTotalSize = 0;
	}

	void Add(const StatsData &data)
	{
		m_cMembers += data.m_cMembers;
		m_cItems += data.m_cItems;
		m_cObjects += data.m_cObjects;
		m_cArrays += data.m_cArrays;
		m_cBools += data.m_cBools;
		m_cInt8s += data.m_cInt8s;
		m_cUInt8s += data.m_cUInt8s;
		m_cInt16s += data.m_cInt16s;
		m_cUInt16s += data.m_cUInt16s;
		m_cInt32s += data.m_cInt32s;
		m_cUInt32s += data.m_cUInt32s;
		m_cInt64s += data.m_cInt64s;
		m_cUInt64s += data.m_cUInt64s;
		m_cFloats += data.m_cFloats;
		m_cDoubles += data.m_cDoubles;
		m_cStrings += data.m_cStrings;
		m_cWStrings += data.m_cWStrings;
		m_cAllValues += data.m_cAllValues;
		m_cbDataSize += data.m_cbDataSize;
		m_cbTotalSize += data.m_cbTotalSize;
	}

};

}//namespace SB

}//namespace CX

