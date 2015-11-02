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
 
#pragma once


#include "CX/SimpleBuffers/MemberType.hpp"
#include "CX/String.hpp"
#include "CX/BLOB.hpp"
#include "CX/Vector.hpp"
#include "CX/Set.hpp"
#include "CX/Map.hpp"


namespace CX
{

namespace SimpleBuffers
{

template <MemberType M, typename K, typename V = void>
struct DataIniter;

template <>
struct DataIniter<MemberType_Scalar, Bool>
{
	static void Init(Bool &v)
	{
		v = False;
	}
};

template <>
struct DataIniter<MemberType_Scalar, Int8>
{
	static void Init(Int8 &v)
	{
		v = 0;
	}
};

template <>
struct DataIniter<MemberType_Scalar, UInt8>
{
	static void Init(UInt8 &v)
	{
		v = 0;
	}
};

template <>
struct DataIniter<MemberType_Scalar, Int16>
{
	static void Init(Int16 &v)
	{
		v = 0;
	}
};

template <>
struct DataIniter<MemberType_Scalar, UInt16>
{
	static void Init(UInt16 &v)
	{
		v = 0;
	}
};

template <>
struct DataIniter<MemberType_Scalar, Int32>
{
	static void Init(Int32 &v)
	{
		v = 0;
	}
};

template <>
struct DataIniter<MemberType_Scalar, UInt32>
{
	static void Init(UInt32 &v)
	{
		v = 0;
	}
};

template <>
struct DataIniter<MemberType_Scalar, Int64>
{
	static void Init(Int64 &v)
	{
		v = 0;
	}
};

template <>
struct DataIniter<MemberType_Scalar, UInt64>
{
	static void Init(UInt64 &v)
	{
		v = 0;
	}
};

template <>
struct DataIniter<MemberType_Scalar, Float>
{
	static void Init(Float &v)
	{
		v = 0.0f;
	}
};

template <>
struct DataIniter<MemberType_Scalar, Double>
{
	static void Init(Double &v)
	{
		v = 0.0;
	}
};

template <>
struct DataIniter<MemberType_Scalar, String>
{
	static void Init(String &v)
	{
		v = "";
	}
};

template <>
struct DataIniter<MemberType_Scalar, BLOB>
{
	static void Init(BLOB &v)
	{
		v.clear();
	}
};

template <typename K>
struct DataIniter<MemberType_Vector, K>
{
	static void Init(typename Vector<K>::Type &v)
	{
		v.clear();
	}
};

template <typename K>
struct DataIniter<MemberType_Set, K>
{
	static void Init(typename Set<K>::Type &v)
	{
		v.clear();
	}
};

template <typename K, typename V>
struct DataIniter<MemberType_Map, K, V>
{
	static void Init(typename Map<K, V>::Type &v)
	{
		v.clear();
	}
};

}//namespace SimpleBuffers 

}//namespace CX
