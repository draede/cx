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
#include "CX/String.hpp"
#include "../../../../Contrib/BINJSON/Include/BINJSONWriter.h"


namespace CX
{

namespace Data
{

namespace BINJSON
{

class CX_API DataWriter : public IO::IDataWriter
{
public:

	typedef void * (* Alloc_Func)(void *pUserContext, Size cbSize);

	typedef void * (* Realloc_Func)(void *pUserContext, void *pPtr, Size cbSize);

	typedef void (* Free_Func)(void *pUserContext, void *pPtr);

	DataWriter(IO::IOutputStream *pOutputStream, 
	           Alloc_Func pfnAlloc = NULL, 
	           Realloc_Func pfnRealloc = NULL, 
	           Free_Func pfnFree = NULL);

	virtual ~DataWriter();

	virtual Status BeginRootObject();

	virtual Status EndRootObject();

	virtual Status BeginRootArray();

	virtual Status EndRootArray();

	//object member
	virtual Status WriteNull(const Char *szName);

	//array item
	virtual Status WriteNull();

	//object member
	virtual Status WriteBool(const Char *szName, Bool bValue);

	//array item
	virtual Status WriteBool(Bool bValue);

	//object member
	virtual Status WriteInt(const Char *szName, Int64 nValue);

	//array item
	virtual Status WriteInt(Int64 nValue);

	//object member
	virtual Status WriteReal(const Char *szName, Double lfValue);

	//array item
	virtual Status WriteReal(Double lfValue);

	//object member
	virtual Status WriteString(const Char *szName, const Char *szValue);

	//array item
	virtual Status WriteString(const Char *szValue);

	//object member
	virtual Status WriteWString(const Char *szName, const WChar *wszValue);

	//array item
	virtual Status WriteWString(const WChar *wszValue);

	//object member
	virtual Status WriteBLOB(const Char *szName, const void *pData, Size cbSize);

	//array item
	virtual Status WriteBLOB(const void *pData, Size cbSize);

	//object member
	virtual Status BeginObject(const Char *szName);

	//array item
	virtual Status BeginObject();

	//object member
	virtual Status BeginArray(const Char *szName);

	//array item
	virtual Status BeginArray();

	virtual Status EndObject();

	virtual Status EndArray();

private:

	IO::IOutputStream   *m_pOutputStream;
	Alloc_Func          m_pfnAlloc;
	Realloc_Func        m_pfnRealloc;
	Free_Func           m_pfnFree;
	CX_BINJSON_Writer   m_writer;

	static void * CustomAlloc(void *pUserContext, Size cbSize);

	static void * CustomRealloc(void *pUserContext, void *pPtr, Size cbSize);

	static void CustomFree(void *pUserContext, void *pPtr);

	static StatusCode CustomWrite(void *pUserContext, const void *pData, CX_Size cbSize);

	static StatusCode CustomUTF8ToUTF16(void *pUserContext, const Char *szSrc,
	                                    WChar *wszDest, Size *pcDestLen);

	static StatusCode CustomUTF16ToUTF8(void *pUserContext, const WChar *wszSrc,
	                                    Char *szDest, Size *pcDestLen);

};

}//namespace BINJSON

}//namespace Data

}//namespace CX

