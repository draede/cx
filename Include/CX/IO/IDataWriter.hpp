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
#include "CX/Status.hpp"
#include "CX/APIDefs.hpp"


namespace CX
{

namespace IO
{

class CX_API IDataWriter
{
public:

	virtual ~IDataWriter() { }

	virtual Status BeginRootObject() = 0;

	virtual Status EndRootObject() = 0;

	virtual Status BeginRootArray() = 0;

	virtual Status EndRootArray() = 0;

	//object member
	virtual Status WriteNull(const Char *szName) = 0;

	//array item
	virtual Status WriteNull() = 0;

	//object member
	virtual Status WriteBool(const Char *szName, Bool bValue) = 0;

	//array item
	virtual Status WriteBool(Bool bValue) = 0;

	//object member
	virtual Status WriteInt(const Char *szName, Int64 nValue) = 0;

	//array item
	virtual Status WriteInt(Int64 nValue) = 0;

	//object member
	virtual Status WriteReal(const Char *szName, Double lfValue) = 0;

	//array item
	virtual Status WriteReal(Double lfValue) = 0;

	//object member
	virtual Status WriteString(const Char *szName, const Char *szValue) = 0;

	//array item
	virtual Status WriteString(const Char *szValue) = 0;

	//object member
	virtual Status BeginObject(const Char *szName) = 0;

	//array item
	virtual Status BeginObject() = 0;

	//object member
	virtual Status BeginArray(const Char *szName) = 0;

	//array item
	virtual Status BeginArray() = 0;

	virtual Status EndObject() = 0;

	virtual Status EndArray() = 0;

};

}//namespace IO

}//namespace CX

