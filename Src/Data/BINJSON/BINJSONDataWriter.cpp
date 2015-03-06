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

#include "CX/Data/BINJSON/DataWriter.hpp"
#include "CX/Str/UTF8.hpp"


namespace CX
{

namespace Data
{

namespace BINJSON
{

DataWriter::DataWriter(IO::IOutputStream *pOutputStream, 
                       Alloc_Func pfnAlloc/* = NULL*/, 
                       Realloc_Func pfnRealloc/* = NULL*/, 
                       Free_Func pfnFree/* = NULL*/)
{
	m_pOutputStream = pOutputStream;
	m_pfnAlloc      = NULL != pfnAlloc ? pfnAlloc : &DataWriter::CustomAlloc;
	m_pfnRealloc    = NULL != pfnRealloc ? pfnRealloc : &DataWriter::CustomRealloc;
	m_pfnFree       = NULL != pfnFree ? pfnFree : &DataWriter::CustomFree;

	CX_BINJSON_HelperAPI api;

	api.Alloc        = m_pfnAlloc;
	api.Realloc      = m_pfnRealloc;
	api.Free         = m_pfnFree;
	api.UTF8ToWUTF16 = &CustomUTF8ToUTF16;
	api.UTF16ToWUTF8 = &CustomUTF16ToUTF8;

	CX_BINJSON_Writer_Init(&m_writer, this, &api, &DataWriter::CustomWrite);
}

DataWriter::~DataWriter()
{
}

void *DataWriter::CustomAlloc(void *pUserContext, Size cbSize)
{
	pUserContext;

	return Alloc(cbSize);
}

void *DataWriter::CustomRealloc(void *pUserContext, void *pPtr, Size cbSize)
{
	pUserContext;

	return Realloc(pPtr, cbSize);
}

void DataWriter::CustomFree(void *pUserContext, void *pPtr)
{
	pUserContext;

	Free(pPtr);
}

StatusCode DataWriter::CustomWrite(void *pUserContext, const void *pData, CX_Size cbSize)
{
	DataWriter *pThis = (DataWriter *)pUserContext;
	Size       cbAckSize;
	Status     status;

	if ((status = pThis->m_pOutputStream->Write(pData, cbSize, &cbAckSize)).IsNOK())
	{
		return status.GetCode();
	}
	if (cbSize != cbAckSize)
	{
		return Status_WriteFailed;
	}

	return Status_OK;
}

StatusCode DataWriter::CustomUTF8ToUTF16(void *pUserContext, const Char *szSrc,
                                         WChar *wszDest, Size *pcDestLen)
{
	pUserContext;

	return Str::UTF8::ToUTF16(szSrc, SIZET_MAX, wszDest, pcDestLen).GetCode();
}

StatusCode DataWriter::CustomUTF16ToUTF8(void *pUserContext, const WChar *wszSrc,
                                        Char *szDest, Size *pcDestLen)
{
	pUserContext;

	return Str::UTF8::FromUTF16(wszSrc, SIZET_MAX, szDest, pcDestLen).GetCode();
}

Status DataWriter::BeginRootObject()
{
	return Status(CX_BINJSON_Writer_BeginRootObject(&m_writer));
}

Status DataWriter::EndRootObject()
{
	return Status(CX_BINJSON_Writer_EndRootObject(&m_writer));
}

Status DataWriter::BeginRootArray()
{
	return Status(CX_BINJSON_Writer_BeginRootArray(&m_writer));
}

Status DataWriter::EndRootArray()
{
	return Status(CX_BINJSON_Writer_EndRootArray(&m_writer));
}

//object member
Status DataWriter::WriteNull(const Char *szName)
{
	return Status(CX_BINJSON_Writer_ObjWriteNull(&m_writer, szName));
}

//array item
Status DataWriter::WriteNull()
{
	return Status(CX_BINJSON_Writer_ArrWriteNull(&m_writer));
}

//object member
Status DataWriter::WriteBool(const Char *szName, Bool bValue)
{
	return Status(CX_BINJSON_Writer_ObjWriteBool(&m_writer, szName, bValue));
}

//array item
Status DataWriter::WriteBool(Bool bValue)
{
	return Status(CX_BINJSON_Writer_ArrWriteBool(&m_writer, bValue));
}

//object member
Status DataWriter::WriteInt(const Char *szName, Int64 nValue)
{
	return Status(CX_BINJSON_Writer_ObjWriteInt(&m_writer, szName, nValue));
}

//array item
Status DataWriter::WriteInt(Int64 nValue)
{
	return Status(CX_BINJSON_Writer_ArrWriteInt(&m_writer, nValue));
}

//object member
Status DataWriter::WriteReal(const Char *szName, Double lfValue)
{
	return Status(CX_BINJSON_Writer_ObjWriteReal(&m_writer, szName, lfValue));
}

//array item
Status DataWriter::WriteReal(Double lfValue)
{
	return Status(CX_BINJSON_Writer_ArrWriteReal(&m_writer, lfValue));
}

//object member
Status DataWriter::WriteString(const Char *szName, const Char *szValue)
{
	return Status(CX_BINJSON_Writer_ObjWriteString(&m_writer, szName, szValue));
}

//array item
Status DataWriter::WriteString(const Char *szValue)
{
	return Status(CX_BINJSON_Writer_ArrWriteString(&m_writer, szValue));
}

//object member
Status DataWriter::WriteWString(const Char *szName, const WChar *wszValue)
{
	return Status(CX_BINJSON_Writer_ObjWriteWString(&m_writer, szName, wszValue));
}

//array item
Status DataWriter::WriteWString(const WChar *wszValue)
{
	return Status(CX_BINJSON_Writer_ArrWriteWString(&m_writer, wszValue));
}

//object member
Status DataWriter::WriteBLOB(const Char *szName, const void *pData, Size cbSize)
{
	return Status(CX_BINJSON_Writer_ObjWriteBLOB(&m_writer, szName, pData, cbSize));
}

//array item
Status DataWriter::WriteBLOB(const void *pData, Size cbSize)
{
	return Status(CX_BINJSON_Writer_ArrWriteBLOB(&m_writer, pData, cbSize));
}

//object member
Status DataWriter::BeginObject(const Char *szName)
{
	return Status(CX_BINJSON_Writer_ObjBeginObject(&m_writer, szName));
}

//array item
Status DataWriter::BeginObject()
{
	return Status(CX_BINJSON_Writer_ArrBeginObject(&m_writer));
}

//object member
Status DataWriter::BeginArray(const Char *szName)
{
	return Status(CX_BINJSON_Writer_ObjBeginArray(&m_writer, szName));
}

//array item
Status DataWriter::BeginArray()
{
	return Status(CX_BINJSON_Writer_ArrBeginArray(&m_writer));
}

Status DataWriter::EndObject()
{
	return Status(CX_BINJSON_Writer_EndObject(&m_writer));
}

Status DataWriter::EndArray()
{
	return Status(CX_BINJSON_Writer_EndArray(&m_writer));
}

}//namespace BINJSON

}//namespace Data

}//namespace CX

