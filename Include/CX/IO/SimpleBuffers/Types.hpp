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
#include "CX/String.hpp"
#include "CX/Vector.hpp"
#include "CX/IO/IDataReader.hpp"
#include "CX/IO/IDataWriter.hpp"
#include "CX/Status.hpp"


namespace CX
{

namespace IO
{

namespace SimpleBuffers
{

typedef Vector<Bool>::Type      BoolArray;
typedef Vector<Int8>::Type      Int8Array;
typedef Vector<UInt8>::Type     UInt8Array;
typedef Vector<Int16>::Type     Int16Array;
typedef Vector<UInt16>::Type    UInt16Array;
typedef Vector<Int32>::Type     Int32Array;
typedef Vector<UInt32>::Type    UInt32Array;
typedef Vector<Int64>::Type     Int64Array;
typedef Vector<UInt64>::Type    UInt64Array;
typedef Vector<Float>::Type     FloatArray;
typedef Vector<Double>::Type    DoubleArray;
typedef Vector<String>::Type    StringArray;
typedef Vector<WString>::Type   WStringArray;


class IObject
{
public:

	virtual ~IObject() { }

	virtual void Reset() = 0;

	virtual bool Compare(const IObject *pObj) = 0;

	virtual Status Read(IDataReader *pReader) = 0;

	virtual Status Write(IDataWriter *pWriter) const = 0;

};


}//namespace SimpleBuffers

}//namespace IO

}//namespace CX

