/* 
 * CX - C++ framework for general purpose development
 *
 * https://github.com/draede/cx
 * 
 * Copyright (C) 2014 - 2019 draede - draede [at] outlook [dot] com
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


namespace CX
{

namespace Data
{

namespace NPY
{

enum Type
{
	Type_Invalid,
	Type_Int8,         // 'i1'
	Type_UInt8,        // 'u1'
	Type_Int16,        // 'i2'
	Type_UInt16,       // 'u2'
	Type_Int32,        // 'i4'
	Type_UInt32,       // 'u4'
	Type_Int64,        // 'i8'
	Type_UInt64,       // 'u8'
	Type_Float,        // 'f4'
	Type_Double,       // 'f8'
};

struct TypeSize
{
	static Size Get(Type nType)
	{
		switch (nType)
		{
			case Type_Int8:   return 1;
			case Type_UInt8:  return 1;
			case Type_Int16:  return 2;
			case Type_UInt16: return 2;
			case Type_Int32:  return 4;
			case Type_UInt32: return 4;
			case Type_Int64:  return 8;
			case Type_UInt64: return 8;
			case Type_Float:  return 4;
			case Type_Double: return 8;
		}

		return 0;
	}
};

}//namespace NPY

}//namespace Data

}//namespace CX
