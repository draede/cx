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

class CX_API IDataReader
{
public:

	virtual ~IDataReader() { }

	virtual Status Begin() = 0;

	virtual Status End() = 0;

	virtual Status BeginObjectObject(const Char *szName) = 0;

	virtual Status EndObjectObject() = 0;

	virtual Status BeginObjectArray(const Char *szName) = 0;

	virtual Status EndObjectArray() = 0;

	virtual Status ReadObjectBool(const Char *szName, Bool *pbValue) = 0;

	virtual Status ReadObjectInt(const Char *szName, Int64 *pnValue) = 0;

	virtual Status ReadObjectReal(const Char *szName, Double *plfValue) = 0;

	virtual Status ReadObjectString(const Char *szName, String *psValue) = 0;

	virtual Status ReadObjectWString(const Char *szName, WString *pwsValue) = 0;

	//will return Status_NoMoreItems at the end of the array
	virtual Status BeginArrayObject() = 0;

	virtual Status EndArrayObject() = 0;

	//will return Status_NoMoreItems at the end of the array
	virtual Status BeginArrayArray() = 0;

	virtual Status EndArrayArray() = 0;

	//will return Status_NoMoreItems at the end of the array
	virtual Status ReadArrayBool(Bool *pbValue) = 0;

	//will return Status_NoMoreItems at the end of the array
	virtual Status ReadArrayInt(Int64 *pnValue) = 0;

	//will return Status_NoMoreItems at the end of the array
	virtual Status ReadArrayReal(Double *plfValue) = 0;

	//will return Status_NoMoreItems at the end of the array
	virtual Status ReadArrayString(String *psValue) = 0;

	//will return Status_NoMoreItems at the end of the array
	virtual Status ReadArrayWString(WString *pwsValue) = 0;

};

}//namespace IO

}//namespace CX

