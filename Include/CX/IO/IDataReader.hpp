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
		EntryType_Invalid, // == ERROR
		EntryType_EOG,     // == End of group
		EntryType_Null,
		EntryType_Bool,
		EntryType_Int,
		EntryType_Real,
		EntryType_String,
		EntryType_WString,
		EntryType_BLOB,
		EntryType_Object,
		EntryType_Array,
	};

	virtual ~IDataReader() { }

	virtual EntryType GetRootEntryType() = 0;

	virtual Status BeginRootObject() = 0;

	virtual Status EndRootObject() = 0;

	virtual Status BeginRootArray() = 0;

	virtual Status EndRootArray() = 0;

	virtual EntryType GetEntryType() = 0;

	//object member - will return Status_OutOfBounds at the end of the object
	virtual Status ReadNull(String *psName) = 0;

	//array item - will return Status_OutOfBounds at the end of the array
	virtual Status ReadNull() = 0;

	//object member - will return Status_OutOfBounds at the end of the object
	virtual Status ReadBool(String *psName, Bool *pbValue) = 0;

	//array item - will return Status_OutOfBounds at the end of the array
	virtual Status ReadBool(Bool *pbValue) = 0;

	//object member - will return Status_OutOfBounds at the end of the object
	virtual Status ReadInt(String *psName, Int64 *pnValue) = 0;

	//array item - will return Status_OutOfBounds at the end of the array
	virtual Status ReadInt(Int64 *pnValue) = 0;

	//object member - will return Status_OutOfBounds at the end of the object
	virtual Status ReadReal(String *psName, Double *plfValue) = 0;

	//array item - will return Status_OutOfBounds at the end of the array
	virtual Status ReadReal(Double *plfValue) = 0;

	//object member - will return Status_OutOfBounds at the end of the object
	virtual Status ReadString(String *psName, String *psValue) = 0;

	//array item - will return Status_OutOfBounds at the end of the array
	virtual Status ReadString(String *psValue) = 0;

	//object member - will return Status_OutOfBounds at the end of the object
	virtual Status ReadWString(String *psName, WString *pwsValue) = 0;

	//array item - will return Status_OutOfBounds at the end of the array
	virtual Status ReadWString(WString *pwsValue) = 0;

	//object member - will return Status_OutOfBounds at the end of the object; free using CX::Free
	virtual Status ReadBLOB(String *psName, void **ppData, Size *pcbSize) = 0;

	//array item - will return Status_OutOfBounds at the end of the array free using CX::Free
	virtual Status ReadBLOB(void **ppData, Size *pcbSize) = 0;

	//object member
	virtual Status BeginObject(String *psName) = 0;

	//array item
	virtual Status BeginObject() = 0;

	//object member
	virtual Status BeginArray(String *psName) = 0;

	//array item
	virtual Status BeginArray() = 0;

	virtual Status EndObject() = 0;

	virtual Status EndArray() = 0;

};

}//namespace IO

}//namespace CX

