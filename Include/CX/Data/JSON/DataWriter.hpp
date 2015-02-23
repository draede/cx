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


#include "CX/IO/IDataWriter.hpp"
#include "CX/IO/IOutputStream.hpp"


namespace CX
{

namespace Data
{

namespace JSON
{

class CX_API DataWriter : public IO::IDataWriter
{
public:

	DataWriter(IO::IOutputStream *pOutputStream);

	virtual ~DataWriter();

	virtual Status BeginRootObject();

	virtual Status EndRootObject();

	virtual Status BeginRootArray();

	virtual Status EndRootArray();

	virtual Status ObjWriteNull(const Char *szName);

	virtual Status ObjWriteBool(const Char *szName, Bool bValue);

	virtual Status ObjWriteInt(const Char *szName, Int64 nValue);

	virtual Status ObjWriteReal(const Char *szName, Double lfValue);

	virtual Status ObjWriteString(const Char *szName, const Char *szValue);

	virtual Status ObjBeginObject(const Char *szName);

	virtual Status ObjEndObject();

	virtual Status ObjBeginArray(const Char *szName);

	virtual Status ObjEndArray();

	virtual Status ArrWriteNull();

	virtual Status ArrWriteBool(Bool bValue);

	virtual Status ArrWriteInt(Int64 nValue);

	virtual Status ArrWriteReal(Double lfValue);

	virtual Status ArrWriteBString(const Char *szValue);

	virtual Status ArrBeginObject();

	virtual Status ArrEndObject();

	virtual Status ArrBeginArray();

	virtual Status ArrEndArray();

private:

	enum State
	{
		State_Begin,
		State_RootObject,
		State_RootArray,
		State_Object,
		State_Array,
		State_End,
	};

	IO::IOutputStream   *m_pOutputStream;
	State               m_nState;

};

}//namespace JSON

}//namespace Data

}//namespace CX

