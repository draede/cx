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
#include "CX/Data/BINJSON/Spec.hpp"
#include "CX/Str/UTF8.hpp"


namespace CX
{

namespace Data
{

namespace BINJSON
{

DataWriter::DataWriter(IO::IOutputStream *pOutputStream)
{
	m_pOutputStream = pOutputStream;
	m_stackStates.push(State_None);
}

DataWriter::~DataWriter()
{
}

Status DataWriter::Write(const void *pData, Size cbSize)
{
	Size   cbAckSize;
	Status status;

	if ((status = m_pOutputStream->Write(pData, cbSize, &cbAckSize)).IsNOK())
	{
		return status;
	}
	if (cbAckSize != cbSize)
	{
		return Status(Status_WriteFailed, "Failed to write all bytes to stream");
	}

	return Status();
}

Status DataWriter::WriteEx(const void *pData, Size cbSize)
{
	Status status;

	if ((status = Write(pData, cbSize)).IsNOK())
	{
		return status;
	}
	if ((status = m_hash.Update(pData, cbSize)).IsNOK())
	{
		return status;
	}

	return status;
}

Status DataWriter::WriteHeader()
{
	Status status;

	if ((status = Write(&Spec::MAGIC, sizeof(Spec::MAGIC))).IsNOK())
	{
		return status;
	}
	if ((status = Write(&Spec::ENCODING_NONE, sizeof(Spec::ENCODING_NONE))).IsNOK())
	{
		return status;
	}

	return Status();
}

Status DataWriter::WriteFooter(UInt32 nHash)
{
	Status status;

	if ((status = Write(&nHash, sizeof(nHash))).IsNOK())
	{
		return status;
	}

	return Status();
}

Status DataWriter::BeginRootObject()
{
	Status status;

	if (NULL == m_pOutputStream)
	{
		return Status(Status_NotInitialized, "No valid output stream");
	}
	if (m_stackStates.empty() || State_None != m_stackStates.top())
	{
		return Status(Status_InvalidCall, "Out of order call (must be called at the beginning)");
	}
	if ((status = WriteHeader()).IsNOK())
	{
		return status;
	}
	if ((status = WriteEx(&Spec::TYPE_BEGINROOTOBJECT, sizeof(Spec::TYPE_BEGINROOTOBJECT))).IsNOK())
	{
		return status;
	}
	m_stackStates.push(State_RootObject);

	return Status();
}

Status DataWriter::EndRootObject()
{
	Status status;

	if (NULL == m_pOutputStream)
	{
		return Status(Status_NotInitialized, "No valid output stream");
	}
	if (m_stackStates.empty() || State_RootObject != m_stackStates.top())
	{
		return Status(Status_InvalidCall, "Out of order call (must be in the root object)");
	}
	if ((status = WriteEx(&Spec::TYPE_ENDROOTOBJECT, sizeof(Spec::TYPE_ENDROOTOBJECT))).IsNOK())
	{
		return status;
	}

	UInt32 nHash;

	if ((status = m_hash.Done(&nHash)).IsNOK())
	{
		return status;
	}

	if ((status = WriteFooter(nHash)).IsNOK())
	{
		return status;
	}

	m_stackStates.pop();

	return Status();
}

Status DataWriter::BeginRootArray()
{
	Status status;

	if (NULL == m_pOutputStream)
	{
		return Status(Status_NotInitialized, "No valid output stream");
	}
	if (m_stackStates.empty() || State_None != m_stackStates.top())
	{
		return Status(Status_InvalidCall, "Out of order call (must be called at the beginning)");
	}
	if ((status = WriteHeader()).IsNOK())
	{
		return status;
	}
	if ((status = WriteEx(&Spec::TYPE_BEGINROOTARRAY, sizeof(Spec::TYPE_BEGINROOTARRAY))).IsNOK())
	{
		return status;
	}
	m_stackStates.push(State_RootArray);

	return Status();
}

Status DataWriter::EndRootArray()
{
	Status status;

	if (NULL == m_pOutputStream)
	{
		return Status(Status_NotInitialized, "No valid output stream");
	}
	if (m_stackStates.empty() || State_RootArray != m_stackStates.top())
	{
		return Status(Status_InvalidCall, "Out of order call (must be in the root array)");
	}
	if ((status = WriteEx(&Spec::TYPE_ENDROOTARRAY, sizeof(Spec::TYPE_ENDROOTARRAY))).IsNOK())
	{
		return status;
	}

	UInt32 nHash;

	if ((status = m_hash.Done(&nHash)).IsNOK())
	{
		return status;
	}

	if ((status = WriteFooter(nHash)).IsNOK())
	{
		return status;
	}

	m_stackStates.pop();

	return Status();
}

//object member
Status DataWriter::WriteNull(const Char *szName)
{
	UInt32 cNameLen;
	Size   cTmpNameLen;
	String sName;
	Status status;

	if (NULL == m_pOutputStream)
	{
		return Status(Status_NotInitialized, "No valid output stream");
	}
	if (m_stackStates.empty() || 
	    (State_Object != m_stackStates.top() && State_RootObject != m_stackStates.top()))
	{
		return Status(Status_InvalidCall, "Out of order call (must be called from an object)");
	}
	if ((status = WriteEx(&Spec::TYPE_NULL, sizeof(Spec::TYPE_NULL))).IsNOK())
	{
		return status;
	}
	cTmpNameLen = cx_strlen(szName);
	if ((Size)UINT32_MAX < cTmpNameLen)
	{
		return Status(Status_OutOfBounds, "Name too long");
	}
	cNameLen = (UInt32)cTmpNameLen;
	if ((status = WriteEx(&cNameLen, sizeof(cNameLen))).IsNOK())
	{
		return status;
	}
	if ((status = WriteEx(szName, cNameLen * sizeof(Char))).IsNOK())
	{
		return status;
	}

	return Status();
}

//array item
Status DataWriter::WriteNull()
{
	Status status;

	if (NULL == m_pOutputStream)
	{
		return Status(Status_NotInitialized, "No valid output stream");
	}
	if (m_stackStates.empty() ||
		(State_Array != m_stackStates.top() && State_RootArray != m_stackStates.top()))
	{
		return Status(Status_InvalidCall, "Out of order call (must be called from an array)");
	}
	if ((status = WriteEx(&Spec::TYPE_NULL, sizeof(Spec::TYPE_NULL))).IsNOK())
	{
		return status;
	}

	return Status();
}

//object member
Status DataWriter::WriteBool(const Char *szName, Bool bValue)
{
	UInt32 cNameLen;
	Size   cTmpNameLen;
	String sName;
	Status status;

	if (NULL == m_pOutputStream)
	{
		return Status(Status_NotInitialized, "No valid output stream");
	}
	if (m_stackStates.empty() || 
	    (State_Object != m_stackStates.top() && State_RootObject != m_stackStates.top()))
	{
		return Status(Status_InvalidCall, "Out of order call (must be called from an object)");
	}
	if (bValue)
	{
		if ((status = WriteEx(&Spec::TYPE_TRUE, sizeof(Spec::TYPE_FALSE))).IsNOK())
		{
			return status;
		}
	}
	else
	{
		if ((status = WriteEx(&Spec::TYPE_FALSE, sizeof(Spec::TYPE_FALSE))).IsNOK())
		{
			return status;
		}
	}
	if ((Size)UINT32_MAX < cx_strlen(szName))
	{
		return Status(Status_OutOfBounds, "Name too long");
	}
	cTmpNameLen = cx_strlen(szName);
	if ((Size)UINT32_MAX < cTmpNameLen)
	{
		return Status(Status_OutOfBounds, "Name too long");
	}
	cNameLen = (UInt32)cTmpNameLen;
	if ((status = WriteEx(&cNameLen, sizeof(cNameLen))).IsNOK())
	{
		return status;
	}
	if ((status = WriteEx(szName, cNameLen * sizeof(Char))).IsNOK())
	{
		return status;
	}

	return Status();
}

//array item
Status DataWriter::WriteBool(Bool bValue)
{
	Status status;

	if (NULL == m_pOutputStream)
	{
		return Status(Status_NotInitialized, "No valid output stream");
	}
	if (m_stackStates.empty() ||
		(State_Array != m_stackStates.top() && State_RootArray != m_stackStates.top()))
	{
		return Status(Status_InvalidCall, "Out of order call (must be called from an array)");
	}
	if (bValue)
	{
		if ((status = WriteEx(&Spec::TYPE_TRUE, sizeof(Spec::TYPE_FALSE))).IsNOK())
		{
			return status;
		}
	}
	else
	{
		if ((status = WriteEx(&Spec::TYPE_FALSE, sizeof(Spec::TYPE_FALSE))).IsNOK())
		{
			return status;
		}
	}

	return Status();
}

//object member
Status DataWriter::WriteInt(const Char *szName, Int64 nValue)
{
	UInt32 cNameLen;
	Size   cTmpNameLen;
	String sName;
	Status status;

	if (NULL == m_pOutputStream)
	{
		return Status(Status_NotInitialized, "No valid output stream");
	}
	if (m_stackStates.empty() || 
	    (State_Object != m_stackStates.top() && State_RootObject != m_stackStates.top()))
	{
		return Status(Status_InvalidCall, "Out of order call (must be called from an object)");
	}
	if ((status = WriteEx(&Spec::TYPE_INT, sizeof(Spec::TYPE_INT))).IsNOK())
	{
		return status;
	}
	cTmpNameLen = cx_strlen(szName);
	if ((Size)UINT32_MAX < cTmpNameLen)
	{
		return Status(Status_OutOfBounds, "Name too long");
	}
	cNameLen = (UInt32)cTmpNameLen;
	if ((status = WriteEx(&cNameLen, sizeof(cNameLen))).IsNOK())
	{
		return status;
	}
	if ((status = WriteEx(szName, cNameLen * sizeof(Char))).IsNOK())
	{
		return status;
	}
	if ((status = WriteEx(&nValue, sizeof(nValue))).IsNOK())
	{
		return status;
	}

	return Status();
}

//array item
Status DataWriter::WriteInt(Int64 nValue)
{
	Status status;

	if (NULL == m_pOutputStream)
	{
		return Status(Status_NotInitialized, "No valid output stream");
	}
	if (m_stackStates.empty() ||
		(State_Array != m_stackStates.top() && State_RootArray != m_stackStates.top()))
	{
		return Status(Status_InvalidCall, "Out of order call (must be called from an array)");
	}
	if ((status = WriteEx(&Spec::TYPE_INT, sizeof(Spec::TYPE_INT))).IsNOK())
	{
		return status;
	}
	if ((status = WriteEx(&nValue, sizeof(nValue))).IsNOK())
	{
		return status;
	}

	return Status();
}

//object member
Status DataWriter::WriteReal(const Char *szName, Double lfValue)
{
	UInt32 cNameLen;
	Size   cTmpNameLen;
	String sName;
	Status status;

	if (NULL == m_pOutputStream)
	{
		return Status(Status_NotInitialized, "No valid output stream");
	}
	if (m_stackStates.empty() || 
	    (State_Object != m_stackStates.top() && State_RootObject != m_stackStates.top()))
	{
		return Status(Status_InvalidCall, "Out of order call (must be called from an object)");
	}
	if ((status = WriteEx(&Spec::TYPE_REAL, sizeof(Spec::TYPE_REAL))).IsNOK())
	{
		return status;
	}
	cTmpNameLen = cx_strlen(szName);
	if ((Size)UINT32_MAX < cTmpNameLen)
	{
		return Status(Status_OutOfBounds, "Name too long");
	}
	cNameLen = (UInt32)cTmpNameLen;
	if ((status = WriteEx(&cNameLen, sizeof(cNameLen))).IsNOK())
	{
		return status;
	}
	if ((status = WriteEx(szName, cNameLen * sizeof(Char))).IsNOK())
	{
		return status;
	}
	if ((status = WriteEx(&lfValue, sizeof(lfValue))).IsNOK())
	{
		return status;
	}

	return Status();
}

//array item
Status DataWriter::WriteReal(Double lfValue)
{
	Status status;

	if (NULL == m_pOutputStream)
	{
		return Status(Status_NotInitialized, "No valid output stream");
	}
	if (m_stackStates.empty() ||
		(State_Array != m_stackStates.top() && State_RootArray != m_stackStates.top()))
	{
		return Status(Status_InvalidCall, "Out of order call (must be called from an array)");
	}
	if ((status = WriteEx(&Spec::TYPE_REAL, sizeof(Spec::TYPE_REAL))).IsNOK())
	{
		return status;
	}
	if ((status = WriteEx(&lfValue, sizeof(lfValue))).IsNOK())
	{
		return status;
	}

	return Status();
}

//object member
Status DataWriter::WriteString(const Char *szName, const Char *szValue)
{
	UInt32 cNameLen;
	Size   cTmpNameLen;
	UInt32 cValueLen;
	Size   cTmpValueLen;
	Status status;

	if (NULL == m_pOutputStream)
	{
		return Status(Status_NotInitialized, "No valid output stream");
	}
	if (m_stackStates.empty() || 
	    (State_Object != m_stackStates.top() && State_RootObject != m_stackStates.top()))
	{
		return Status(Status_InvalidCall, "Out of order call (must be called from an object)");
	}
	if ((status = WriteEx(&Spec::TYPE_STRING, sizeof(Spec::TYPE_STRING))).IsNOK())
	{
		return status;
	}
	cTmpNameLen = cx_strlen(szName);
	if ((Size)UINT32_MAX < cTmpNameLen)
	{
		return Status(Status_OutOfBounds, "Name too long");
	}
	cNameLen = (UInt32)cTmpNameLen;
	if ((status = WriteEx(&cNameLen, sizeof(cNameLen))).IsNOK())
	{
		return status;
	}
	if ((status = WriteEx(szName, cNameLen * sizeof(Char))).IsNOK())
	{
		return status;
	}
	cTmpValueLen = cx_strlen(szValue);
	if ((Size)UINT32_MAX < cTmpValueLen)
	{
		return Status(Status_OutOfBounds, "Value too big");
	}
	cValueLen = (UInt32)cTmpValueLen;
	if ((status = WriteEx(&cValueLen, sizeof(cValueLen))).IsNOK())
	{
		return status;
	}
	if ((status = WriteEx(szValue, cValueLen * sizeof(Char))).IsNOK())
	{
		return status;
	}

	return Status();
}

//array item
Status DataWriter::WriteString(const Char *szValue)
{
	UInt32 cValueLen;
	Size   cTmpValueLen;
	Status status;

	if (NULL == m_pOutputStream)
	{
		return Status(Status_NotInitialized, "No valid output stream");
	}
	if (m_stackStates.empty() ||
		(State_Array != m_stackStates.top() && State_RootArray != m_stackStates.top()))
	{
		return Status(Status_InvalidCall, "Out of order call (must be called from an array)");
	}
	if ((status = WriteEx(&Spec::TYPE_STRING, sizeof(Spec::TYPE_STRING))).IsNOK())
	{
		return status;
	}
	cTmpValueLen = cx_strlen(szValue);
	if ((Size)UINT32_MAX < cTmpValueLen)
	{
		return Status(Status_OutOfBounds, "Value too big");
	}
	cValueLen = (UInt32)cTmpValueLen;
	if ((status = WriteEx(&cValueLen, sizeof(cValueLen))).IsNOK())
	{
		return status;
	}
	if ((status = WriteEx(szValue, cValueLen * sizeof(Char))).IsNOK())
	{
		return status;
	}

	return Status();
}

//object member
Status DataWriter::WriteWString(const Char *szName, const WChar *wszValue)
{
	UInt32 cNameLen;
	Size   cTmpNameLen;
	UInt32 cValueLen;
	String sValue;
	Status status;

	if (NULL == m_pOutputStream)
	{
		return Status(Status_NotInitialized, "No valid output stream");
	}
	if (m_stackStates.empty() ||
		(State_Object != m_stackStates.top() && State_RootObject != m_stackStates.top()))
	{
		return Status(Status_InvalidCall, "Out of order call (must be called from an object)");
	}
	if ((status = WriteEx(&Spec::TYPE_STRING, sizeof(Spec::TYPE_STRING))).IsNOK())
	{
		return status;
	}
	cTmpNameLen = cx_strlen(szName);
	if ((Size)UINT32_MAX < cTmpNameLen)
	{
		return Status(Status_OutOfBounds, "Name too long");
	}
	cNameLen = (UInt32)cTmpNameLen;
	if ((status = WriteEx(&cNameLen, sizeof(cNameLen))).IsNOK())
	{
		return status;
	}
	if ((status = WriteEx(szName, cNameLen * sizeof(Char))).IsNOK())
	{
		return status;
	}
	if ((status = Str::UTF8::FromWChar(wszValue, &sValue)).IsNOK())
	{
		return status;
	}
	if ((Size)UINT32_MAX < sValue.size())
	{
		return Status(Status_OutOfBounds, "Value too big");
	}
	cValueLen = (UInt32)sValue.size();
	if ((status = WriteEx(&cValueLen, sizeof(cValueLen))).IsNOK())
	{
		return status;
	}
	if ((status = WriteEx(sValue.c_str(), cValueLen * sizeof(Char))).IsNOK())
	{
		return status;
	}

	return Status();
}

//array item
Status DataWriter::WriteWString(const WChar *wszValue)
{
	UInt32 cValueLen;
	String sValue;
	Status status;

	if (NULL == m_pOutputStream)
	{
		return Status(Status_NotInitialized, "No valid output stream");
	}
	if (m_stackStates.empty() ||
		(State_Array != m_stackStates.top() && State_RootArray != m_stackStates.top()))
	{
		return Status(Status_InvalidCall, "Out of order call (must be called from an array)");
	}
	if ((status = WriteEx(&Spec::TYPE_STRING, sizeof(Spec::TYPE_STRING))).IsNOK())
	{
		return status;
	}
	if ((status = Str::UTF8::FromWChar(wszValue, &sValue)).IsNOK())
	{
		return status;
	}
	if ((Size)UINT32_MAX < sValue.size())
	{
		return Status(Status_OutOfBounds, "Value too big");
	}
	cValueLen = (UInt32)sValue.size();
	if ((status = WriteEx(&cValueLen, sizeof(cValueLen))).IsNOK())
	{
		return status;
	}
	if ((status = WriteEx(sValue.c_str(), cValueLen * sizeof(Char))).IsNOK())
	{
		return status;
	}

	return Status();
}

//object member
Status DataWriter::WriteBLOB(const Char *szName, const void *pData, Size cbSize)
{
	UInt32 cNameLen;
	Size   cTmpNameLen;
	UInt32 cbValueSize;
	Status status;

	if (NULL == m_pOutputStream)
	{
		return Status(Status_NotInitialized, "No valid output stream");
	}
	if (m_stackStates.empty() ||
		(State_Object != m_stackStates.top() && State_RootObject != m_stackStates.top()))
	{
		return Status(Status_InvalidCall, "Out of order call (must be called from an object)");
	}
	if ((status = WriteEx(&Spec::TYPE_BLOB, sizeof(Spec::TYPE_BLOB))).IsNOK())
	{
		return status;
	}
	cTmpNameLen = cx_strlen(szName);
	if ((Size)UINT32_MAX < cTmpNameLen)
	{
		return Status(Status_OutOfBounds, "Name too long");
	}
	cNameLen = (UInt32)cTmpNameLen;
	if ((status = WriteEx(&cNameLen, sizeof(cNameLen))).IsNOK())
	{
		return status;
	}
	if ((status = WriteEx(szName, cNameLen * sizeof(Char))).IsNOK())
	{
		return status;
	}
	if ((Size)UINT32_MAX < cbSize)
	{
		return Status(Status_OutOfBounds, "Value too big");
	}
	cbValueSize = (UInt32)cbSize;
	if ((status = WriteEx(&cbValueSize, sizeof(cbValueSize))).IsNOK())
	{
		return status;
	}
	if ((status = WriteEx(pData, cbSize)).IsNOK())
	{
		return status;
	}

	return Status();
}

//array item
Status DataWriter::WriteBLOB(const void *pData, Size cbSize)
{
	String sValue;
	UInt32 cbValueSize;
	Status status;

	if (NULL == m_pOutputStream)
	{
		return Status(Status_NotInitialized, "No valid output stream");
	}
	if (m_stackStates.empty() ||
		(State_Array != m_stackStates.top() && State_RootArray != m_stackStates.top()))
	{
		return Status(Status_InvalidCall, "Out of order call (must be called from an array)");
	}
	if ((status = WriteEx(&Spec::TYPE_BLOB, sizeof(Spec::TYPE_BLOB))).IsNOK())
	{
		return status;
	}
	if ((Size)UINT32_MAX < cbSize)
	{
		return Status(Status_OutOfBounds, "Value too big");
	}
	cbValueSize = (UInt32)cbSize;
	if ((status = WriteEx(&cbValueSize, sizeof(cbValueSize))).IsNOK())
	{
		return status;
	}
	if ((status = WriteEx(pData, cbSize)).IsNOK())
	{
		return status;
	}

	return Status();
}

//object member
Status DataWriter::BeginObject(const Char *szName)
{
	UInt32 cNameLen;
	Size   cTmpNameLen;
	String sName;
	Status status;

	if (NULL == m_pOutputStream)
	{
		return Status(Status_NotInitialized, "No valid output stream");
	}
	if (m_stackStates.empty() || 
	    (State_Object != m_stackStates.top() && State_RootObject != m_stackStates.top()))
	{
		return Status(Status_InvalidCall, "Out of order call (must be called from an object)");
	}
	if ((status = WriteEx(&Spec::TYPE_BEGINOBJECT, sizeof(Spec::TYPE_BEGINOBJECT))).IsNOK())
	{
		return status;
	}
	cTmpNameLen = cx_strlen(szName);
	if ((Size)UINT32_MAX < cTmpNameLen)
	{
		return Status(Status_OutOfBounds, "Name too long");
	}
	cNameLen = (UInt32)cTmpNameLen;
	if ((status = WriteEx(&cNameLen, sizeof(cNameLen))).IsNOK())
	{
		return status;
	}
	if ((status = WriteEx(szName, cNameLen * sizeof(Char))).IsNOK())
	{
		return status;
	}
	m_stackStates.push(State_Object);

	return Status();
}

//array item
Status DataWriter::BeginObject()
{
	Status status;

	if (NULL == m_pOutputStream)
	{
		return Status(Status_NotInitialized, "No valid output stream");
	}
	if (m_stackStates.empty() ||
		(State_Array != m_stackStates.top() && State_RootArray != m_stackStates.top()))
	{
		return Status(Status_InvalidCall, "Out of order call (must be called from an array)");
	}
	if ((status = WriteEx(&Spec::TYPE_BEGINOBJECT, sizeof(Spec::TYPE_BEGINOBJECT))).IsNOK())
	{
		return status;
	}
	m_stackStates.push(State_Object);

	return Status();
}

//object member
Status DataWriter::BeginArray(const Char *szName)
{
	UInt32 cNameLen;
	Size   cTmpNameLen;
	String sName;
	Status status;

	if (NULL == m_pOutputStream)
	{
		return Status(Status_NotInitialized, "No valid output stream");
	}
	if (m_stackStates.empty() ||
		(State_Object != m_stackStates.top() && State_RootObject != m_stackStates.top()))
	{
		return Status(Status_InvalidCall, "Out of order call (must be called from an object)");
	}
	if ((status = WriteEx(&Spec::TYPE_BEGINARRAY, sizeof(Spec::TYPE_BEGINARRAY))).IsNOK())
	{
		return status;
	}
	cTmpNameLen = cx_strlen(szName);
	if ((Size)UINT32_MAX < cTmpNameLen)
	{
		return Status(Status_OutOfBounds, "Name too long");
	}
	cNameLen = (UInt32)cTmpNameLen;
	if ((status = WriteEx(&cNameLen, sizeof(cNameLen))).IsNOK())
	{
		return status;
	}
	if ((status = WriteEx(szName, cNameLen * sizeof(Char))).IsNOK())
	{
		return status;
	}
	m_stackStates.push(State_Array);

	return Status();
}

//array item
Status DataWriter::BeginArray()
{
	Status status;

	if (NULL == m_pOutputStream)
	{
		return Status(Status_NotInitialized, "No valid output stream");
	}
	if (m_stackStates.empty() ||
		(State_Array != m_stackStates.top() && State_RootArray != m_stackStates.top()))
	{
		return Status(Status_InvalidCall, "Out of order call (must be called from an array)");
	}
	if ((status = WriteEx(&Spec::TYPE_BEGINARRAY, sizeof(Spec::TYPE_BEGINARRAY))).IsNOK())
	{
		return status;
	}
	m_stackStates.push(State_Array);

	return Status();
}

Status DataWriter::EndObject()
{
	Status status;

	if (NULL == m_pOutputStream)
	{
		return Status(Status_NotInitialized, "No valid output stream");
	}
	if (m_stackStates.empty() || State_Object != m_stackStates.top())
	{
		return Status(Status_InvalidCall, "Out of order call (must be called from an object)");
	}
	if ((status = WriteEx(&Spec::TYPE_ENDOBJECT, sizeof(Spec::TYPE_ENDOBJECT))).IsNOK())
	{
		return status;
	}
	m_stackStates.pop();

	return Status();
}

Status DataWriter::EndArray()
{
	Status status;

	if (NULL == m_pOutputStream)
	{
		return Status(Status_NotInitialized, "No valid output stream");
	}
	if (m_stackStates.empty() || State_Array != m_stackStates.top())
	{
		return Status(Status_InvalidCall, "Out of order call (must be called from an object)");
	}
	if ((status = WriteEx(&Spec::TYPE_ENDARRAY, sizeof(Spec::TYPE_ENDARRAY))).IsNOK())
	{
		return status;
	}
	m_stackStates.pop();

	return Status();
}

}//namespace BINJSON

}//namespace Data

}//namespace CX

