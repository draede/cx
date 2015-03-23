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


namespace CX
{

namespace IO
{

namespace SimpleBuffers
{

class CX_API BinaryData
{
public:

	static const UInt32 MAGIC                = 0x42535843;
	static const UInt8  ARRAY_ITEM           = 0x00;
	static const UInt8  ARRAY_END            = 0xFF;
	static const UInt8  OP_BEGIN             = 0x01;
	static const UInt8  OP_END               = 0x02;
	static const UInt8  OP_BEGINOBJECTOBJECT = 0x03;
	static const UInt8  OP_ENDOBJECTOBJECT   = 0x04;
	static const UInt8  OP_BEGINOBJECTARRAY  = 0x05;
	static const UInt8  OP_ENDOBJECTARRAY    = 0x06;
	static const UInt8  OP_OBJECTBOOL        = 0x07;
	static const UInt8  OP_OBJECTINT         = 0x08;
	static const UInt8  OP_OBJECTREAL        = 0x09;
	static const UInt8  OP_OBJECTSTRING      = 0x0A;
	static const UInt8  OP_OBJECTWSTRING     = 0x0B;
	static const UInt8  OP_BEGINARRAYOBJECT  = 0x0C;
	static const UInt8  OP_ENDARRAYOBJECT    = 0x0D;
	static const UInt8  OP_BEGINARRAYARRAY   = 0x0E;
	static const UInt8  OP_ENDARRAYARRAY     = 0x0F;
	static const UInt8  OP_ARRAYBOOL         = 0x10;
	static const UInt8  OP_ARRAYINT          = 0x11;
	static const UInt8  OP_ARRAYREAL         = 0x12;
	static const UInt8  OP_ARRAYSTRING       = 0x13;
	static const UInt8  OP_ARRAYWSTRING      = 0x14;

private:

	BinaryData()
	{
	}

	~BinaryData()
	{
	}

};

}//namespace SimpleBuffers

}//namespace IO

}//namespace CX

