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


#include "CX/IO/IDataReader.hpp"
#include "CX/IO/IInputStream.hpp"
#include "CX/String.hpp"
#include "../../../../Contrib/BINJSON/Include/BINJSONReader.h"


namespace CX
{

namespace Data
{

namespace BINJSON
{

class CX_API DataReader : public IO::IDataReader
{
public:

	typedef void * (* Alloc_Func)(void *pUserContext, Size cbSize);

	typedef void * (* Realloc_Func)(void *pUserContext, void *pPtr, Size cbSize);

	typedef void (* Free_Func)(void *pUserContext, void *pPtr);

	DataReader(IO::IInputStream *pInputStream, 
	           Alloc_Func pfnAlloc = NULL, 
	           Realloc_Func pfnRealloc = NULL, 
	           Free_Func pfnFree = NULL);

	virtual ~DataReader();

	void FreeBLOBMem(void *pData);

	virtual EntryType GetRootEntryType();

	virtual Status BeginRootObject();

	virtual Status EndRootObject();

	virtual Status BeginRootArray();

	virtual Status EndRootArray();

	virtual EntryType GetEntryType();

	//object member - will return Status_OutOfBounds at the end of the object
	virtual Status ReadNull(String *psName);

	//array item - will return Status_OutOfBounds at the end of the array
	virtual Status ReadNull();

	//object member - will return Status_OutOfBounds at the end of the object
	virtual Status ReadBool(String *psName, Bool *pbValue);

	//array item - will return Status_OutOfBounds at the end of the array
	virtual Status ReadBool(Bool *pbValue);

	//object member - will return Status_OutOfBounds at the end of the object
	virtual Status ReadInt(String *psName, Int64 *pnValue);

	//array item - will return Status_OutOfBounds at the end of the array
	virtual Status ReadInt(Int64 *pnValue);

	//object member - will return Status_OutOfBounds at the end of the object
	virtual Status ReadReal(String *psName, Double *plfValue);

	//array item - will return Status_OutOfBounds at the end of the array
	virtual Status ReadReal(Double *plfValue);

	//object member - will return Status_OutOfBounds at the end of the object
	virtual Status ReadString(String *psName, String *psValue);

	//array item - will return Status_OutOfBounds at the end of the array
	virtual Status ReadString(String *psValue);

	//object member - will return Status_OutOfBounds at the end of the object
	virtual Status ReadWString(String *psName, WString *pwsValue);

	//array item - will return Status_OutOfBounds at the end of the array
	virtual Status ReadWString(WString *pwsValue);

	//object member - will return Status_OutOfBounds at the end of the object; free using CX::Free
	virtual Status ReadBLOB(String *psName, void **ppData, Size *pcbSize);

	//array item - will return Status_OutOfBounds at the end of the array free using CX::Free
	virtual Status ReadBLOB(void **ppData, Size *pcbSize);

	//object member
	virtual Status BeginObject(String *psName);

	//array item
	virtual Status BeginObject();

	//object member
	virtual Status BeginArray(String *psName);

	//array item
	virtual Status BeginArray();

	virtual Status EndObject();

	virtual Status EndArray();

private:

	IO::IInputStream   *m_pInputStream;
	Alloc_Func          m_pfnAlloc;
	Realloc_Func        m_pfnRealloc;
	Free_Func           m_pfnFree;
	CX_BINJSON_Reader   m_reader;

	static void * CustomAlloc(void *pUserContext, Size cbSize);

	static void * CustomRealloc(void *pUserContext, void *pPtr, Size cbSize);

	static void CustomFree(void *pUserContext, void *pPtr);

	static StatusCode CustomRead(void *pUserContext, void *pData, CX_Size cbSize);

	static StatusCode CustomUTF8ToUTF16(void *pUserContext, const Char *szSrc,
	                                    WChar *wszDest, Size *pcDestLen);

	static StatusCode CustomUTF16ToUTF8(void *pUserContext, const WChar *wszSrc,
	                                    Char *szDest, Size *pcDestLen);

};

}//namespace BINJSON

}//namespace Data

}//namespace CX

