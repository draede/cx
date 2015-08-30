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


#include "CX/Types.hpp"
#include "CX/Status.hpp"
#include "CX/String.hpp"
#include "CX/IO/IOutputStream.hpp"
#include "CX/BLOB.hpp"
#include "CX/APIDefs.hpp"


namespace CX
{

namespace SimpleBuffers
{

class CX_API IWriter
{
public:

	virtual ~IWriter() { }

	virtual Status Begin(IO::IOutputStream *pOutputStream) = 0;

	virtual Status End() = 0;

	virtual Status WriteBool(Bool v, const Char *szName = NULL) = 0;

	virtual Status WriteInt8(Int8 v, const Char *szName = NULL) = 0;

	virtual Status WriteUInt8(UInt8 v, const Char *szName = NULL) = 0;

	virtual Status WriteInt16(Int16 v, const Char *szName = NULL) = 0;

	virtual Status WriteUInt16(UInt16 v, const Char *szName = NULL) = 0;

	virtual Status WriteInt32(Int32 v, const Char *szName = NULL) = 0;

	virtual Status WriteUInt32(UInt32 v, const Char *szName = NULL) = 0;

	virtual Status WriteInt64(Int64 v, const Char *szName = NULL) = 0;

	virtual Status WriteUInt64(UInt64 v, const Char *szName = NULL) = 0;

	virtual Status WriteFloat(Float v, const Char *szName = NULL) = 0;

	virtual Status WriteDouble(Double v, const Char *szName = NULL) = 0;

	virtual Status WriteString(const String &v, const Char *szName = NULL) = 0;

	virtual Status WriteBLOB(const BLOB &v, const Char *szName = NULL) = 0;

	virtual Status BeginObject(const Char *szName = NULL) = 0;

	virtual Status EndObject() = 0;

	virtual Status BeginArray(const Char *szName = NULL) = 0;

	virtual Status EndArray() = 0;

};

}//namespace SimpleBuffers

}//namespace CX

