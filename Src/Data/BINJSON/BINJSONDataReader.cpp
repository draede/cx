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

#include "CX/Data/BINJSON/DataReader.hpp"
#include "CX/Str/UTF8.hpp"


namespace CX
{

namespace Data
{

namespace BINJSON
{

DataReader::DataReader(IO::IInputStream *pInputStream, 
                       Alloc_Func pfnAlloc/* = NULL*/, 
                       Realloc_Func pfnRealloc/* = NULL*/, 
                       Free_Func pfnFree/* = NULL*/)
{
	m_pInputStream  = pInputStream;
	m_pfnAlloc      = NULL != pfnAlloc ? pfnAlloc : &DataReader::CustomAlloc;
	m_pfnRealloc    = NULL != pfnRealloc ? pfnRealloc : &DataReader::CustomRealloc;
	m_pfnFree       = NULL != pfnFree ? pfnFree : &DataReader::CustomFree;

	CX_BINJSON_HelperAPI api;

	api.Alloc        = m_pfnAlloc;
	api.Realloc      = m_pfnRealloc;
	api.Free         = m_pfnFree;

	CX_BINJSON_Reader_Init(&m_reader, this, &api, &DataReader::CustomRead);
}

DataReader::~DataReader()
{
}

void DataReader::FreeBLOBMem(void *pData)
{
	CX_BINJSON_Reader_FreeBLOB(&m_reader, pData);
}

void *DataReader::CustomAlloc(void *pUserContext, Size cbSize)
{
	pUserContext;

	return Alloc(cbSize);
}

void *DataReader::CustomRealloc(void *pUserContext, void *pPtr, Size cbSize)
{
	pUserContext;

	return Realloc(pPtr, cbSize);
}

void DataReader::CustomFree(void *pUserContext, void *pPtr)
{
	pUserContext;

	Free(pPtr);
}

StatusCode DataReader::CustomRead(void *pUserContext, void *pData, CX_Size cbSize)
{
	DataReader *pThis = (DataReader *)pUserContext;
	Size       cbAckSize;
	Status     status;

	if ((status = pThis->m_pInputStream->Read(pData, cbSize, &cbAckSize)).IsNOK())
	{
		return status.GetCode();
	}
	if (cbSize != cbAckSize)
	{
		return Status_ReadFailed;
	}

	return Status_OK;
}

DataReader::EntryType DataReader::GetRootEntryType()
{
	if (NULL == m_pInputStream)
	{
		return EntryType_Invalid;
	}

	return (DataReader::EntryType)CX_BINJSON_Reader_GetRootEntryType(&m_reader);
}

Status DataReader::BeginRootObject()
{
	if (NULL == m_pInputStream)
	{
		return Status(Status_NotInitialized, "Not initialized");
	}

	return CX_BINJSON_Reader_BeginRootObject(&m_reader);
}

Status DataReader::EndRootObject()
{
	if (NULL == m_pInputStream)
	{
		return Status(Status_NotInitialized, "Not initialized");
	}

	return CX_BINJSON_Reader_EndRootObject(&m_reader);
}

Status DataReader::BeginRootArray()
{
	if (NULL == m_pInputStream)
	{
		return Status(Status_NotInitialized, "Not initialized");
	}

	return CX_BINJSON_Reader_BeginRootArray(&m_reader);
}

Status DataReader::EndRootArray()
{
	if (NULL == m_pInputStream)
	{
		return Status(Status_NotInitialized, "Not initialized");
	}

	return CX_BINJSON_Reader_EndRootArray(&m_reader);
}

DataReader::EntryType DataReader::GetEntryType()
{
	if (NULL == m_pInputStream)
	{
		return EntryType_Invalid;
	}

	return (DataReader::EntryType)CX_BINJSON_Reader_GetEntryType(&m_reader);
}

//object member - will return Status_OutOfBounds at the end of the object
Status DataReader::ReadNull(String *psName)
{
	if (NULL == m_pInputStream)
	{
		return EntryType_Invalid;
	}

	CX_BINJSON_Reader_String name;
	StatusCode               nStatus;

	if (CXNOK(nStatus = CX_BINJSON_Reader_ObjReadNull(&m_reader, &name)))
	{
		return Status(nStatus);
	}
	*psName = name.pString;
	CX_BINJSON_Reader_FreeString(&m_reader, &name);

	return Status();
}

//array item - will return Status_OutOfBounds at the end of the array
Status DataReader::ReadNull()
{
	if (NULL == m_pInputStream)
	{
		return EntryType_Invalid;
	}

	StatusCode               nStatus;

	if (CXNOK(nStatus = CX_BINJSON_Reader_ArrReadNull(&m_reader)))
	{
		return Status(nStatus);
	}

	return Status();
}

//object member - will return Status_OutOfBounds at the end of the object
Status DataReader::ReadBool(String *psName, Bool *pbValue)
{
	if (NULL == m_pInputStream)
	{
		return EntryType_Invalid;
	}

	CX_BINJSON_Reader_String name;
	StatusCode               nStatus;

	if (CXNOK(nStatus = CX_BINJSON_Reader_ObjReadBool(&m_reader, &name, pbValue)))
	{
		return Status(nStatus);
	}
	*psName = name.pString;
	CX_BINJSON_Reader_FreeString(&m_reader, &name);

	return Status();
}

//array item - will return Status_OutOfBounds at the end of the array
Status DataReader::ReadBool(Bool *pbValue)
{
	if (NULL == m_pInputStream)
	{
		return EntryType_Invalid;
	}

	StatusCode               nStatus;

	if (CXNOK(nStatus = CX_BINJSON_Reader_ArrReadBool(&m_reader, pbValue)))
	{
		return Status(nStatus);
	}

	return Status();
}

//object member - will return Status_OutOfBounds at the end of the object
Status DataReader::ReadInt(String *psName, Int64 *pnValue)
{
	if (NULL == m_pInputStream)
	{
		return EntryType_Invalid;
	}

	CX_BINJSON_Reader_String name;
	StatusCode               nStatus;

	if (CXNOK(nStatus = CX_BINJSON_Reader_ObjReadInt(&m_reader, &name, pnValue)))
	{
		return Status(nStatus);
	}
	*psName = name.pString;
	CX_BINJSON_Reader_FreeString(&m_reader, &name);

	return Status();
}

//array item - will return Status_OutOfBounds at the end of the array
Status DataReader::ReadInt(Int64 *pnValue)
{
	if (NULL == m_pInputStream)
	{
		return EntryType_Invalid;
	}

	StatusCode               nStatus;

	if (CXNOK(nStatus = CX_BINJSON_Reader_ArrReadInt(&m_reader, pnValue)))
	{
		return Status(nStatus);
	}

	return Status();
}

//object member - will return Status_OutOfBounds at the end of the object
Status DataReader::ReadReal(String *psName, Double *plfValue)
{
	if (NULL == m_pInputStream)
	{
		return EntryType_Invalid;
	}

	CX_BINJSON_Reader_String name;
	StatusCode               nStatus;

	if (CXNOK(nStatus = CX_BINJSON_Reader_ObjReadReal(&m_reader, &name, plfValue)))
	{
		return Status(nStatus);
	}
	*psName = name.pString;
	CX_BINJSON_Reader_FreeString(&m_reader, &name);

	return Status();
}

//array item - will return Status_OutOfBounds at the end of the array
Status DataReader::ReadReal(Double *plfValue)
{
	if (NULL == m_pInputStream)
	{
		return EntryType_Invalid;
	}

	StatusCode               nStatus;

	if (CXNOK(nStatus = CX_BINJSON_Reader_ArrReadReal(&m_reader, plfValue)))
	{
		return Status(nStatus);
	}

	return Status();
}

//object member - will return Status_OutOfBounds at the end of the object
Status DataReader::ReadString(String *psName, String *psValue)
{
	if (NULL == m_pInputStream)
	{
		return EntryType_Invalid;
	}

	CX_BINJSON_Reader_String name;
	CX_BINJSON_Reader_String value;
	StatusCode               nStatus;

	if (CXNOK(nStatus = CX_BINJSON_Reader_ObjReadString(&m_reader, &name, &value)))
	{
		return Status(nStatus);
	}
	*psName  = name.pString;
	*psValue = value.pString;
	CX_BINJSON_Reader_FreeString(&m_reader, &name);
	CX_BINJSON_Reader_FreeString(&m_reader, &value);

	return Status();
}

//array item - will return Status_OutOfBounds at the end of the array
Status DataReader::ReadString(String *psValue)
{
	if (NULL == m_pInputStream)
	{
		return EntryType_Invalid;
	}

	CX_BINJSON_Reader_String value;
	StatusCode               nStatus;

	if (CXNOK(nStatus = CX_BINJSON_Reader_ArrReadString(&m_reader, &value)))
	{
		return Status(nStatus);
	}
	*psValue = value.pString;
	CX_BINJSON_Reader_FreeString(&m_reader, &value);

	return Status();
}

//object member - will return Status_OutOfBounds at the end of the object
Status DataReader::ReadWString(String *psName, WString *pwsValue)
{
	if (NULL == m_pInputStream)
	{
		return EntryType_Invalid;
	}

	CX_BINJSON_Reader_String  name;
	CX_BINJSON_Reader_WString value;
	StatusCode                nStatus;

	if (CXNOK(nStatus = CX_BINJSON_Reader_ObjReadWString(&m_reader, &name, &value)))
	{
		return Status(nStatus);
	}
	*psName   = name.pString;
	*pwsValue = value.pWString;
	CX_BINJSON_Reader_FreeString(&m_reader, &name);
	CX_BINJSON_Reader_FreeWString(&m_reader, &value);

	return Status();
}

//array item - will return Status_OutOfBounds at the end of the array
Status DataReader::ReadWString(WString *pwsValue)
{
	if (NULL == m_pInputStream)
	{
		return EntryType_Invalid;
	}

	CX_BINJSON_Reader_WString value;
	StatusCode                nStatus;

	if (CXNOK(nStatus = CX_BINJSON_Reader_ArrReadWString(&m_reader, &value)))
	{
		return Status(nStatus);
	}
	*pwsValue = value.pWString;
	CX_BINJSON_Reader_FreeWString(&m_reader, &value);

	return Status();
}

//object member - will return Status_OutOfBounds at the end of the object; free using CX::Free
Status DataReader::ReadBLOB(String *psName, void **ppData, Size *pcbSize)
{
	if (NULL == m_pInputStream)
	{
		return EntryType_Invalid;
	}

	CX_BINJSON_Reader_String name;
	StatusCode               nStatus;

	if (CXNOK(nStatus = CX_BINJSON_Reader_ObjReadBLOB(&m_reader, &name, ppData, pcbSize)))
	{
		return Status(nStatus);
	}
	*psName = name.pString;
	CX_BINJSON_Reader_FreeString(&m_reader, &name);

	return Status();
}

//array item - will return Status_OutOfBounds at the end of the array free using CX::Free
Status DataReader::ReadBLOB(void **ppData, Size *pcbSize)
{
	if (NULL == m_pInputStream)
	{
		return EntryType_Invalid;
	}

	StatusCode               nStatus;

	if (CXNOK(nStatus = CX_BINJSON_Reader_ArrReadBLOB(&m_reader, ppData, pcbSize)))
	{
		return Status(nStatus);
	}

	return Status();
}

//object member
Status DataReader::BeginObject(String *psName)
{
	if (NULL == m_pInputStream)
	{
		return EntryType_Invalid;
	}

	CX_BINJSON_Reader_String name;
	StatusCode               nStatus;

	if (CXNOK(nStatus = CX_BINJSON_Reader_ObjBeginObject(&m_reader, &name)))
	{
		return Status(nStatus);
	}
	*psName = name.pString;
	CX_BINJSON_Reader_FreeString(&m_reader, &name);

	return Status();
}

//array item
Status DataReader::BeginObject()
{
	if (NULL == m_pInputStream)
	{
		return EntryType_Invalid;
	}

	StatusCode               nStatus;

	if (CXNOK(nStatus = CX_BINJSON_Reader_ArrBeginObject(&m_reader)))
	{
		return Status(nStatus);
	}

	return Status();
}

//object member
Status DataReader::BeginArray(String *psName)
{
	if (NULL == m_pInputStream)
	{
		return EntryType_Invalid;
	}

	CX_BINJSON_Reader_String name;
	StatusCode               nStatus;

	if (CXNOK(nStatus = CX_BINJSON_Reader_ObjBeginArray(&m_reader, &name)))
	{
		return Status(nStatus);
	}
	*psName = name.pString;
	CX_BINJSON_Reader_FreeString(&m_reader, &name);

	return Status();
}

//array item
Status DataReader::BeginArray()
{
	if (NULL == m_pInputStream)
	{
		return EntryType_Invalid;
	}

	StatusCode               nStatus;

	if (CXNOK(nStatus = CX_BINJSON_Reader_ArrBeginArray(&m_reader)))
	{
		return Status(nStatus);
	}

	return Status();
}

Status DataReader::EndObject()
{
	if (NULL == m_pInputStream)
	{
		return EntryType_Invalid;
	}

	return Status(CX_BINJSON_Reader_EndObject(&m_reader));
}

Status DataReader::EndArray()
{
	if (NULL == m_pInputStream)
	{
		return EntryType_Invalid;
	}

	return Status(CX_BINJSON_Reader_EndArray(&m_reader));
}

}//namespace BINJSON

}//namespace Data

}//namespace CX

