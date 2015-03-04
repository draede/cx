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
#include "CX/Data/BINJSON/Spec.hpp"
#include "CX/Str/UTF8.hpp"


namespace CX
{

namespace Data
{

namespace BINJSON
{

DataReader::DataReader(IO::IInputStream *pInputStream)
{
	m_pInputStream = pInputStream;
	if (ReadHeader().IsNOK())
	{
		m_pInputStream = NULL;

		return;
	}
	if (ReadEntryType().IsNOK())
	{
		m_pInputStream = NULL;

		return;
	}
	if (Spec::TYPE_BEGINROOTOBJECT != m_nCrEntryType && Spec::TYPE_BEGINROOTARRAY != m_nCrEntryType)
	{
		m_pInputStream = NULL;
	}
	else
	{
		m_nRootEntryType = m_nCrEntryType;
	}
}

DataReader::~DataReader()
{
}

Status DataReader::ReadHeader()
{
	UInt64 nMagic;
	UInt32 nEncoding;
	Status status;

	if ((status = Read(&nMagic, sizeof(nMagic))).IsNOK())
	{
		m_pInputStream = NULL;

		return status;
	}
	if (Spec::MAGIC != nMagic)
	{
		m_pInputStream = NULL;

		return Status(Status_OperationFailed, "Invalid magic value");
	}
	if ((status = Read(&nEncoding, sizeof(nEncoding))).IsNOK())
	{
		m_pInputStream = NULL;

		return status;
	}
	if (Spec::ENCODING_NONE != nEncoding)
	{
		m_pInputStream = NULL;

		return Status(Status_OperationFailed, "Invalid encoding value");
	}

	return Status();
}

Status DataReader::ReadFooter(UInt32 *pnHash)
{
	Status status;

	if ((status = Read(pnHash, sizeof(*pnHash))).IsNOK())
	{
		m_pInputStream = NULL;

		return status;
	}

	return Status();
}

Status DataReader::ReadEntryType()
{
	Status status;

	if ((status = ReadEx(&m_nCrEntryType, sizeof(m_nCrEntryType))).IsNOK())
	{
		m_pInputStream = NULL;

		return status;
	}

	return Status();
}

Status DataReader::ReadStringData(String *psName)
{
	UInt32 cNameLen;
	Char   tmp[16384];
	Char   *pTmp;
	Status status;

	if ((status = ReadEx(&cNameLen, sizeof(cNameLen))).IsNOK())
	{
		return status;
	}
	if (cNameLen <= sizeof(tmp) / sizeof(tmp[0]))
	{
		pTmp = tmp;
	}
	else
	{
		if (NULL == (pTmp = (Char *)Alloc((cNameLen) * sizeof(Char))))
		{
			return Status(Status_MemAllocFailed, "Failed to allocate temp buffer");
		}
	}
	if (0 < cNameLen)
	{
		if ((status = ReadEx(pTmp, cNameLen * sizeof(Char))).IsNOK())
		{
			return status;
		}
		psName->assign(pTmp, cNameLen);
	}
	else
	{
		psName->clear();
	}
	if (pTmp != tmp)
	{
		Free(pTmp);
	}

	return Status();
}

Status DataReader::ReadObjectEntry(String *psName, Byte nType, void *pData, Size cbSize, 
                                   void **ppData, Size *pcbSize)
{
	Status status;

	if (NULL == m_pInputStream)
	{
		return Status(Status_NotInitialized, "Not initialized");
	}
	if (m_stackEntries.empty())
	{
		return Status(Status_InvalidCall, "Out of order");
	}
	if (m_stackEntries.top() != Spec::TYPE_BEGINROOTOBJECT && 
	    m_stackEntries.top() != Spec::TYPE_BEGINOBJECT)
	{
		return Status(Status_InvalidCall, "Out of order");
	}
	if (Spec::TYPE_ENDROOTOBJECT == m_nCrEntryType || 
	    Spec::TYPE_ENDROOTARRAY == m_nCrEntryType || 
	    Spec::TYPE_ENDOBJECT == m_nCrEntryType || 
	    Spec::TYPE_ENDARRAY == m_nCrEntryType)
	{
		return Status(Status_OutOfBounds, "Out of bounds");
	}
	if (nType != m_nCrEntryType)
	{
		if (!((Spec::TYPE_FALSE == m_nCrEntryType && Spec::TYPE_TRUE == nType) ||
		     (Spec::TYPE_TRUE == m_nCrEntryType && Spec::TYPE_FALSE == nType)))
		{
			return Status(Status_InvalidCall, "Invalid type");
		}
	}
	if ((status = ReadStringData(psName)).IsNOK())
	{
		return status;
	}
	if (Spec::TYPE_NULL != nType)
	{
		if (Spec::TYPE_FALSE == m_nCrEntryType)
		{
			*((Bool *)pData) = False;
		}
		else
		if (Spec::TYPE_TRUE== m_nCrEntryType)
		{
			*((Bool *)pData) = True;
		}
		else
		if (Spec::TYPE_STRING == m_nCrEntryType)
		{
			if ((status = ReadStringData((String *)pData)).IsNOK())
			{
				return status;
			}
		}
		else
		if (Spec::TYPE_BLOB == m_nCrEntryType)
		{
			UInt32 cbTmpSize;

			if ((status = ReadEx(&cbTmpSize, sizeof(cbTmpSize))).IsNOK())
			{
				return status;
			}
			if (NULL == (*ppData = Alloc(cbTmpSize)))
			{
				return Status(Status_MemAllocFailed, "Failed to allocate {1} bytes", cbTmpSize);
			}
			if ((status = ReadEx(*ppData, cbTmpSize)).IsNOK())
			{
				return status;
			}
			*pcbSize = cbTmpSize;
		}
		else
		{
			if ((status = ReadEx(pData, cbSize)).IsNOK())
			{
				return status;
			}
		}
	}

	return ReadEntryType();
}

Status DataReader::ReadArrayEntry(Byte nType, void *pData, Size cbSize, 
                                  void **ppData, Size *pcbSize)
{
	Status status;

	if (NULL == m_pInputStream)
	{
		return Status(Status_NotInitialized, "Not initialized");
	}
	if (m_stackEntries.empty())
	{
		return Status(Status_InvalidCall, "Out of order");
	}
	if (m_stackEntries.top() != Spec::TYPE_BEGINROOTARRAY && 
	    m_stackEntries.top() != Spec::TYPE_BEGINARRAY)
	{
		return Status(Status_InvalidCall, "Out of order");
	}
	if (Spec::TYPE_ENDROOTOBJECT == m_nCrEntryType || 
	    Spec::TYPE_ENDROOTARRAY == m_nCrEntryType || 
	    Spec::TYPE_ENDOBJECT == m_nCrEntryType || 
	    Spec::TYPE_ENDARRAY == m_nCrEntryType)
	{
		return Status(Status_OutOfBounds, "Out of bounds");
	}
	if (nType != m_nCrEntryType)
	{
		if (!((Spec::TYPE_FALSE == m_nCrEntryType && Spec::TYPE_TRUE == nType) ||
		     (Spec::TYPE_TRUE == m_nCrEntryType && Spec::TYPE_FALSE == nType)))
		{
			return Status(Status_InvalidCall, "Invalid type");
		}
	}
	if (Spec::TYPE_NULL != nType)
	{
		if (Spec::TYPE_FALSE == m_nCrEntryType)
		{
			*((Bool *)pData) = False;
		}
		else
		if (Spec::TYPE_TRUE== m_nCrEntryType)
		{
			*((Bool *)pData) = True;
		}
		else
		if (Spec::TYPE_STRING == nType)
		{
			if ((status = ReadStringData((String *)pData)).IsNOK())
			{
				return status;
			}
		}
		else
		if (Spec::TYPE_BLOB == nType)
		{
			UInt32 cbTmpSize;

			if ((status = ReadEx(&cbTmpSize, sizeof(cbTmpSize))).IsNOK())
			{
				return status;
			}
			if (NULL == (*ppData = Alloc(cbTmpSize)))
			{
				return Status(Status_MemAllocFailed, "Failed to allocate {1} bytes", cbTmpSize);
			}
			if ((status = ReadEx(*ppData, cbTmpSize)).IsNOK())
			{
				return status;
			}
			*pcbSize = cbTmpSize;
		}
		else
		{
			if ((status = ReadEx(pData, cbSize)).IsNOK())
			{
				return status;
			}
		}
	}

	return ReadEntryType();
}

Status DataReader::Read(void *pData, Size cbSize)
{
	Size   cbAckSize;
	Status status;

	if ((status = m_pInputStream->Read(pData, cbSize, &cbAckSize)).IsNOK())
	{
		return status;
	}
	if (cbSize != cbAckSize)
	{
		return Status(Status_ReadFailed, "Failed to read all bytes from stream");
	}

	return Status();
}

Status DataReader::ReadEx(void *pData, Size cbSize)
{
	Status status;

	if ((status = Read(pData, cbSize)).IsNOK())
	{
		return status;
	}
	if ((status = m_hash.Update(pData, cbSize)).IsNOK())
	{
		return status;
	}

	return Status();
}

DataReader::EntryType DataReader::GetRootEntryType()
{
	if (NULL == m_pInputStream)
	{
		return EntryType_Invalid;
	}
	if (Spec::TYPE_BEGINROOTOBJECT == m_nRootEntryType)
	{
		return EntryType_Object;
	}
	else
	if (Spec::TYPE_BEGINROOTARRAY == m_nRootEntryType)
	{
		return EntryType_Array;
	}
	else
	{
		return EntryType_Invalid;
	}
}

Status DataReader::BeginRootObject()
{
	if (NULL == m_pInputStream)
	{
		return Status(Status_NotInitialized, "Not initialized");
	}
	if (!m_stackEntries.empty())
	{
		return Status(Status_InvalidCall, "Out of order");
	}
	if (Spec::TYPE_BEGINROOTOBJECT != m_nRootEntryType)
	{
		return Status(Status_NotInitialized, "Not an object");
	}
	m_stackEntries.push(Spec::TYPE_BEGINROOTOBJECT);

	return ReadEntryType();
}

Status DataReader::EndRootObject()
{
	if (NULL == m_pInputStream)
	{
		return Status(Status_NotInitialized, "Not initialized");
	}
	if (m_stackEntries.empty())
	{
		return Status(Status_InvalidCall, "Out of order");
	}
	if (m_stackEntries.top() != Spec::TYPE_BEGINROOTOBJECT)
	{
		return Status(Status_InvalidCall, "Out of order");
	}
	m_stackEntries.pop();
	if (!m_stackEntries.empty())
	{
		return Status(Status_InvalidCall, "Out of order");
	}

	UInt32 nHashInFile;
	UInt32 nHashComputed;
	Status status;

	if ((status = ReadFooter(&nHashInFile)).IsNOK())
	{
		m_pInputStream = NULL;

		return status;
	}
	if ((status = m_hash.Done(&nHashComputed)).IsNOK())
	{
		m_pInputStream = NULL;

		return status;
	}
	if (nHashComputed != nHashInFile)
	{
		return Status(Status_OperationFailed, "Invalid checksum");
	}

	m_pInputStream = NULL;

	return Status();
}

Status DataReader::BeginRootArray()
{
	if (NULL == m_pInputStream)
	{
		return Status(Status_NotInitialized, "Not initialized");
	}
	if (!m_stackEntries.empty())
	{
		return Status(Status_InvalidCall, "Out of order");
	}
	if (Spec::TYPE_BEGINROOTARRAY != m_nRootEntryType)
	{
		return Status(Status_NotInitialized, "Not an array");
	}
	m_stackEntries.push(Spec::TYPE_BEGINROOTARRAY);

	return ReadEntryType();
}

Status DataReader::EndRootArray()
{
	if (NULL == m_pInputStream)
	{
		return Status(Status_NotInitialized, "Not initialized");
	}
	if (m_stackEntries.empty())
	{
		return Status(Status_InvalidCall, "Out of order");
	}
	if (m_stackEntries.top() != Spec::TYPE_BEGINROOTARRAY)
	{
		return Status(Status_InvalidCall, "Out of order");
	}
	m_stackEntries.pop();
	if (!m_stackEntries.empty())
	{
		return Status(Status_InvalidCall, "Out of order");
	}

	UInt32 nHashInFile;
	UInt32 nHashComputed;
	Status status;

	if ((status = ReadFooter(&nHashInFile)).IsNOK())
	{
		m_pInputStream = NULL;

		return status;
	}
	if ((status = m_hash.Done(&nHashComputed)).IsNOK())
	{
		m_pInputStream = NULL;

		return status;
	}
	if (nHashComputed != nHashInFile)
	{
		return Status(Status_OperationFailed, "Invalid checksum");
	}

	m_pInputStream = NULL;

	return Status();
}

DataReader::EntryType DataReader::GetEntryType()
{
	if (NULL == m_pInputStream)
	{
		return EntryType_Invalid;
	}
	if (Spec::TYPE_BEGINROOTOBJECT == m_nCrEntryType)
	{
		return EntryType_Object;
	}
	else
	if (Spec::TYPE_BEGINROOTARRAY == m_nCrEntryType)
	{
		return EntryType_Array;
	}
	else
	if (Spec::TYPE_BEGINOBJECT == m_nCrEntryType)
	{
		return EntryType_Object;
	}
	else
	if (Spec::TYPE_BEGINARRAY == m_nCrEntryType)
	{
		return EntryType_Array;
	}
	else
	if (Spec::TYPE_NULL == m_nCrEntryType)
	{
		return EntryType_Null;
	}
	else
	if (Spec::TYPE_FALSE == m_nCrEntryType)
	{
		return EntryType_Bool;
	}
	else
	if (Spec::TYPE_TRUE == m_nCrEntryType)
	{
		return EntryType_Bool;
	}
	else
	if (Spec::TYPE_INT == m_nCrEntryType)
	{
		return EntryType_Int;
	}
	else
	if (Spec::TYPE_REAL == m_nCrEntryType)
	{
		return EntryType_Real;
	}
	else
	if (Spec::TYPE_STRING == m_nCrEntryType)
	{
		return EntryType_String;
	}
	else
	if (Spec::TYPE_BLOB == m_nCrEntryType)
	{
		return EntryType_BLOB;
	}
	else
	if (Spec::TYPE_ENDROOTOBJECT == m_nCrEntryType)
	{
		return EntryType_EOG;
	}
	else
	if (Spec::TYPE_ENDROOTARRAY == m_nCrEntryType)
	{
		return EntryType_EOG;
	}
	else
	if (Spec::TYPE_ENDOBJECT == m_nCrEntryType)
	{
		return EntryType_EOG;
	}
	else
	if (Spec::TYPE_ENDARRAY == m_nCrEntryType)
	{
		return EntryType_EOG;
	}
	else
	{
		return EntryType_Invalid;
	}
}

//object member - will return Status_OutOfBounds at the end of the object
Status DataReader::ReadNull(String *psName)
{
	return ReadObjectEntry(psName, Spec::TYPE_NULL, NULL, 0, NULL, NULL);
}

//array item - will return Status_OutOfBounds at the end of the array
Status DataReader::ReadNull()
{
	return ReadArrayEntry(Spec::TYPE_NULL, NULL, 0, NULL, NULL);
}

//object member - will return Status_OutOfBounds at the end of the object
Status DataReader::ReadBool(String *psName, Bool *pbValue)
{
	return ReadObjectEntry(psName, Spec::TYPE_FALSE, pbValue, sizeof(*pbValue), NULL, NULL);
}

//array item - will return Status_OutOfBounds at the end of the array
Status DataReader::ReadBool(Bool *pbValue)
{
	return ReadArrayEntry(Spec::TYPE_FALSE, pbValue, sizeof(*pbValue), NULL, NULL);
}

//object member - will return Status_OutOfBounds at the end of the object
Status DataReader::ReadInt(String *psName, Int64 *pnValue)
{
	return ReadObjectEntry(psName, Spec::TYPE_INT, pnValue, sizeof(*pnValue), NULL, NULL);
}

//array item - will return Status_OutOfBounds at the end of the array
Status DataReader::ReadInt(Int64 *pnValue)
{
	return ReadArrayEntry(Spec::TYPE_INT, pnValue, sizeof(*pnValue), NULL, NULL);
}

//object member - will return Status_OutOfBounds at the end of the object
Status DataReader::ReadReal(String *psName, Double *plfValue)
{
	return ReadObjectEntry(psName, Spec::TYPE_REAL, plfValue, sizeof(*plfValue), NULL, NULL);
}

//array item - will return Status_OutOfBounds at the end of the array
Status DataReader::ReadReal(Double *plfValue)
{
	return ReadArrayEntry(Spec::TYPE_REAL, plfValue, sizeof(*plfValue), NULL, NULL);
}

//object member - will return Status_OutOfBounds at the end of the object
Status DataReader::ReadString(String *psName, String *psValue)
{
	return ReadObjectEntry(psName, Spec::TYPE_STRING, psValue, 0, NULL, NULL);
}

//array item - will return Status_OutOfBounds at the end of the array
Status DataReader::ReadString(String *psValue)
{
	return ReadArrayEntry(Spec::TYPE_STRING, psValue, 0, NULL, NULL);
}

//object member - will return Status_OutOfBounds at the end of the object
Status DataReader::ReadWString(String *psName, WString *pwsValue)
{
	String sValue;
	Status status;

	if ((status = ReadString(psName, &sValue)).IsNOK())
	{
		return status;
	}
	if ((status = Str::UTF8::ToWChar(sValue.c_str(), pwsValue)).IsNOK())
	{
		return status;
	}

	return Status();
}

//array item - will return Status_OutOfBounds at the end of the array
Status DataReader::ReadWString(WString *pwsValue)
{
	String sValue;
	Status status;

	if ((status = ReadString(&sValue)).IsNOK())
	{
		return status;
	}
	if ((status = Str::UTF8::ToWChar(sValue.c_str(), pwsValue)).IsNOK())
	{
		return status;
	}

	return Status();
}

//object member - will return Status_OutOfBounds at the end of the object; free using CX::Free
Status DataReader::ReadBLOB(String *psName, void **ppData, Size *pcbSize)
{
	return ReadObjectEntry(psName, Spec::TYPE_BLOB, NULL, NULL, ppData, pcbSize);
}

//array item - will return Status_OutOfBounds at the end of the array free using CX::Free
Status DataReader::ReadBLOB(void **ppData, Size *pcbSize)
{
	return ReadArrayEntry(Spec::TYPE_BLOB, NULL, NULL, ppData, pcbSize);
}

//object member
Status DataReader::BeginObject(String *psName)
{
	Status status;

	if (NULL == m_pInputStream)
	{
		return Status(Status_NotInitialized, "Not initialized");
	}
	if (m_stackEntries.empty())
	{
		return Status(Status_InvalidCall, "Out of order");
	}
	if (m_stackEntries.top() != Spec::TYPE_BEGINROOTOBJECT && 
	    m_stackEntries.top() != Spec::TYPE_BEGINOBJECT)
	{
		return Status(Status_InvalidCall, "Out of order");
	}
	if (Spec::TYPE_BEGINOBJECT != m_nCrEntryType)
	{
		return Status(Status_InvalidCall, "Not an object");
	}
	if ((status = ReadStringData(psName)).IsNOK())
	{
		return status;
	}
	m_stackEntries.push(Spec::TYPE_BEGINOBJECT);

	return ReadEntryType();
}

//array item
Status DataReader::BeginObject()
{
	Status status;

	if (NULL == m_pInputStream)
	{
		return Status(Status_NotInitialized, "Not initialized");
	}
	if (m_stackEntries.empty())
	{
		return Status(Status_InvalidCall, "Out of order");
	}
	if (m_stackEntries.top() != Spec::TYPE_BEGINROOTARRAY &&
	    m_stackEntries.top() != Spec::TYPE_BEGINARRAY)
	{
		return Status(Status_InvalidCall, "Out of order");
	}
	if (Spec::TYPE_BEGINOBJECT != m_nCrEntryType)
	{
		return Status(Status_InvalidCall, "Not an object");
	}
	m_stackEntries.push(Spec::TYPE_BEGINOBJECT);

	return ReadEntryType();
}

//object member
Status DataReader::BeginArray(String *psName)
{
	Status status;

	if (NULL == m_pInputStream)
	{
		return Status(Status_NotInitialized, "Not initialized");
	}
	if (m_stackEntries.empty())
	{
		return Status(Status_InvalidCall, "Out of order");
	}
	if (m_stackEntries.top() != Spec::TYPE_BEGINROOTOBJECT &&
	    m_stackEntries.top() != Spec::TYPE_BEGINOBJECT)
	{
		return Status(Status_InvalidCall, "Out of order");
	}
	if (Spec::TYPE_BEGINARRAY != m_nCrEntryType)
	{
		return Status(Status_InvalidCall, "Not an array");
	}
	if ((status = ReadStringData(psName)).IsNOK())
	{
		return status;
	}
	m_stackEntries.push(Spec::TYPE_BEGINARRAY);

	return ReadEntryType();
}

//array item
Status DataReader::BeginArray()
{
	Status status;

	if (NULL == m_pInputStream)
	{
		return Status(Status_NotInitialized, "Not initialized");
	}
	if (m_stackEntries.empty())
	{
		return Status(Status_InvalidCall, "Out of order");
	}
	if (m_stackEntries.top() != Spec::TYPE_BEGINROOTARRAY && 
	    m_stackEntries.top() != Spec::TYPE_BEGINARRAY)
	{
		return Status(Status_InvalidCall, "Out of order");
	}
	if (Spec::TYPE_BEGINARRAY != m_nCrEntryType)
	{
		return Status(Status_InvalidCall, "Not an array");
	}
	m_stackEntries.push(Spec::TYPE_BEGINARRAY);

	return ReadEntryType();
}

Status DataReader::EndObject()
{
	Status status;

	if (NULL == m_pInputStream)
	{
		return Status(Status_NotInitialized, "Not initialized");
	}
	if (m_stackEntries.empty())
	{
		return Status(Status_InvalidCall, "Out of order");
	}
	if (m_stackEntries.top() != Spec::TYPE_BEGINOBJECT)
	{
		return Status(Status_InvalidCall, "Out of order");
	}
	m_stackEntries.pop();

	return ReadEntryType();
}

Status DataReader::EndArray()
{
	Status status;

	if (NULL == m_pInputStream)
	{
		return Status(Status_NotInitialized, "Not initialized");
	}
	if (m_stackEntries.empty())
	{
		return Status(Status_InvalidCall, "Out of order");
	}
	if (m_stackEntries.top() != Spec::TYPE_BEGINARRAY)
	{
		return Status(Status_InvalidCall, "Out of order");
	}
	m_stackEntries.pop();

	return ReadEntryType();
}

}//namespace BINJSON

}//namespace Data

}//namespace CX

