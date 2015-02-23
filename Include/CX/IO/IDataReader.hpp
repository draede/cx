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

	enum EntryType
	{
		EntryType_Null,
		EntryType_Bool,
		EntryType_Int,
		EntryType_Real,
		EntryType_String,
		EntryType_Object,
		EntryType_Array,
	};

	virtual ~IDataReader() { }

	virtual EntryType GetRootEntryType() = 0;

	virtual Status BeginRootObject() = 0;

	virtual Status EndRootObject() = 0;

	virtual Status BeginRootArray() = 0;

	virtual Status EndRootArray() = 0;

	//will return no more items at the end of the object
	virtual EntryType ObjGetEntryType(String *psName) = 0;

	//will return no more items at the end of the object
	virtual Status ObjReadNull(String *psName) = 0;

	//will return no more items at the end of the object
	virtual Status ObjReadBool(String *psName, Bool *pbValue) = 0;

	//will return no more items at the end of the object
	virtual Status ObjReadInt(String *psName, Int64 *pnValue) = 0;

	//will return no more items at the end of the object
	virtual Status ObjReadReal(String *psName, Double *plfValue) = 0;

	//will return no more items at the end of the object
	virtual Status ObjWriteString(String *psName, String *psValue) = 0;

	virtual Status ObjBeginObject(String *psName) = 0;

	virtual Status ObjEndObject() = 0;

	virtual Status ObjBeginArray(String *psName) = 0;

	virtual Status ObjEndArray() = 0;

	//will return no more items at the end of the array
	virtual EntryType ArrGetEntryType() = 0;

	virtual Size ArrGetItemsCount() = 0;

	//will return no more items at the end of the array
	virtual Status ArrReadNull() = 0;

	//will return no more items at the end of the array
	virtual Status ArrReadBool(Bool *pbValue) = 0;

	//will return no more items at the end of the array
	virtual Status ArrReadInt(Int64 *pnValue) = 0;

	//will return no more items at the end of the array
	virtual Status ArrReadReal(Double *plfValue) = 0;

	//will return no more items at the end of the array
	virtual Status ArrReadString(String *psValue) = 0;

	virtual Status ArrBeginObject() = 0;

	virtual Status ArrEndObject() = 0;

	virtual Status ArrBeginArray() = 0;

	virtual Status ArrEndArray() = 0;

};

}//namespace IO

}//namespace CX

