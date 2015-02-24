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

#include "CX/Data/JSON/DataReader.hpp"


namespace CX
{

namespace Data
{

namespace JSON
{

DataReader::DataReader(IO::IInputStream *pInputStream)
{
	Status status;

	if ((status = m_json.Read(pInputStream)).IsNOK())
	{
		m_bOK = false;
	}
	else
	{
		m_bOK = true;
	}
	m_pVar   = NULL;
}

DataReader::~DataReader()
{
}

DataReader::EntryType DataReader::GetRootEntryType()
{
	if (!m_bOK)
	{
		return EntryType_Invalid;
	}
	if (m_json.IsObject())
	{
		return EntryType_Object;
	}
	if (m_json.IsArray())
	{
		return EntryType_Array;
	}

	return EntryType_Invalid;
}

Status DataReader::BeginRootObject()
{
	if (!m_bOK)
	{
		return Status(Status_NotInitialized, "Not initialized");
	}
	if (NULL != m_pVar)
	{
		return Status(Status_Busy, "Root function already called");
	}
	if (!m_json.IsObject())
	{
		return Status(Status_NotInitialized, "Not an object");
	}
	m_pVar       = &m_json;
	m_iterObject = m_pVar->GetObjectConstIterator();

	return Status();
}

Status DataReader::EndRootObject()
{
	if (!m_bOK)
	{
		return Status(Status_NotInitialized, "Not initialized");
	}
	if (NULL == m_pVar)
	{
		return Status(Status_InvalidCall, "Root function not called");
	}
	if (m_pVar != &m_json)
	{
		return Status(Status_InvalidCall, "Out of order call");
	}
	if (!m_json.IsObject())
	{
		return Status(Status_NotInitialized, "Root not an object");
	}
	m_pVar = NULL;

	return Status();
}

Status DataReader::BeginRootArray()
{
	if (!m_bOK)
	{
		return Status(Status_NotInitialized, "Not initialized");
	}
	if (NULL != m_pVar)
	{
		return Status(Status_Busy, "Root function already called");
	}
	if (!m_json.IsArray())
	{
		return Status(Status_NotInitialized, "Not an array");
	}
	m_pVar      = &m_json;
	m_iterArray = m_pVar->GetArrayConstIterator();

	return Status();
}

Status DataReader::EndRootArray()
{
	if (!m_bOK)
	{
		return Status(Status_NotInitialized, "Not initialized");
	}
	if (NULL == m_pVar)
	{
		return Status(Status_InvalidCall, "Root function not called");
	}
	if (m_pVar != &m_json)
	{
		return Status(Status_InvalidCall, "Out of order call");
	}
	if (!m_json.IsArray())
	{
		return Status(Status_NotInitialized, "Root not an array");
	}
	m_pVar = NULL;

	return Status();
}

DataReader::EntryType DataReader::GetEntryType()
{
	if (!m_bOK)
	{
		return EntryType_Invalid;
	}
	if (NULL == m_pVar)
	{
		return EntryType_Invalid;
	}
	if (m_pVar->IsObject())
	{
		if (!m_iterObject.IsValid())
		{
			return EntryType_Invalid;
		}

		return VarTypeToEntryType(m_iterObject.Get().GetType());
	}
	if (m_pVar->IsArray())
	{
		if (!m_iterArray.IsValid())
		{
			return EntryType_Invalid;
		}

		return VarTypeToEntryType(m_iterArray.Get().GetType());
	}

	return EntryType_Invalid;
}

//object member - will return Status_OutOfBounds at the end of the object
Status DataReader::ReadNull(String *psName)
{
	if (!m_bOK)
	{
		return Status(Status_NotInitialized, "Not initialized");
	}
	if (NULL == m_pVar)
	{
		return Status(Status_InvalidCall, "Root function not called");
	}
	if (!m_pVar->IsObject())
	{
		return Status(Status_InvalidCall, "Not an object member");
	}
	if (!m_iterObject.IsValid())
	{
		return Status(Status_OutOfBounds, "No more data");
	}
	if (Var::Type_Null != m_iterObject.Get().GetType())
	{
		return Status(Status_InvalidCall, "Not a null");
	}
	*psName = m_iterObject.Get().GetName();
	m_iterObject.Next();

	return Status();
}

//array item - will return Status_OutOfBounds at the end of the array
Status DataReader::ReadNull()
{
	if (!m_bOK)
	{
		return Status(Status_NotInitialized, "Not initialized");
	}
	if (NULL == m_pVar)
	{
		return Status(Status_InvalidCall, "Root function not called");
	}
	if (!m_pVar->IsArray())
	{
		return Status(Status_InvalidCall, "Not an array item");
	}
	if (!m_iterArray.IsValid())
	{
		return Status(Status_OutOfBounds, "No more data");
	}
	if (Var::Type_Null != m_iterArray.Get().GetType())
	{
		return Status(Status_InvalidCall, "Not a null");
	}
	m_iterArray.Next();
	
	return Status();
}

//object member - will return Status_OutOfBounds at the end of the object
Status DataReader::ReadBool(String *psName, Bool *pbValue)
{
	if (!m_bOK)
	{
		return Status(Status_NotInitialized, "Not initialized");
	}
	if (NULL == m_pVar)
	{
		return Status(Status_InvalidCall, "Root function not called");
	}
	if (!m_pVar->IsObject())
	{
		return Status(Status_InvalidCall, "Not an object member");
	}
	if (!m_iterObject.IsValid())
	{
		return Status(Status_OutOfBounds, "No more data");
	}
	if (Var::Type_Bool != m_iterObject.Get().GetType())
	{
		return Status(Status_InvalidCall, "Not a bool");
	}
	*psName  = m_iterObject.Get().GetName();
	*pbValue = m_iterObject.Get().GetBool();
	m_iterObject.Next();

	return Status();
}

//array item - will return Status_OutOfBounds at the end of the array
Status DataReader::ReadBool(Bool *pbValue)
{
	if (!m_bOK)
	{
		return Status(Status_NotInitialized, "Not initialized");
	}
	if (NULL == m_pVar)
	{
		return Status(Status_InvalidCall, "Root function not called");
	}
	if (!m_pVar->IsArray())
	{
		return Status(Status_InvalidCall, "Not an array item");
	}
	if (!m_iterArray.IsValid())
	{
		return Status(Status_OutOfBounds, "No more data");
	}
	if (Var::Type_Bool != m_iterArray.Get().GetType())
	{
		return Status(Status_InvalidCall, "Not a bool");
	}
	*pbValue = m_iterArray.Get().GetBool();
	m_iterArray.Next();

	return Status();
}

//object member - will return Status_OutOfBounds at the end of the object
Status DataReader::ReadInt(String *psName, Int64 *pnValue)
{
	if (!m_bOK)
	{
		return Status(Status_NotInitialized, "Not initialized");
	}
	if (NULL == m_pVar)
	{
		return Status(Status_InvalidCall, "Root function not called");
	}
	if (!m_pVar->IsObject())
	{
		return Status(Status_InvalidCall, "Not an object member");
	}
	if (!m_iterObject.IsValid())
	{
		return Status(Status_OutOfBounds, "No more data");
	}
	if (Var::Type_Int != m_iterObject.Get().GetType())
	{
		return Status(Status_InvalidCall, "Not an int");
	}
	*psName  = m_iterObject.Get().GetName();
	*pnValue = m_iterObject.Get().GetInt();
	m_iterObject.Next();

	return Status();
}

//array item - will return Status_OutOfBounds at the end of the array
Status DataReader::ReadInt(Int64 *pnValue)
{
	if (!m_bOK)
	{
		return Status(Status_NotInitialized, "Not initialized");
	}
	if (NULL == m_pVar)
	{
		return Status(Status_InvalidCall, "Root function not called");
	}
	if (!m_pVar->IsArray())
	{
		return Status(Status_InvalidCall, "Not an array item");
	}
	if (!m_iterArray.IsValid())
	{
		return Status(Status_OutOfBounds, "No more data");
	}
	if (Var::Type_Int != m_iterArray.Get().GetType())
	{
		return Status(Status_InvalidCall, "Not an int");
	}
	*pnValue = m_iterArray.Get().GetInt();
	m_iterArray.Next();

	return Status();
}

//object member - will return Status_OutOfBounds at the end of the object
Status DataReader::ReadReal(String *psName, Double *plfValue)
{
	if (!m_bOK)
	{
		return Status(Status_NotInitialized, "Not initialized");
	}
	if (NULL == m_pVar)
	{
		return Status(Status_InvalidCall, "Root function not called");
	}
	if (!m_pVar->IsObject())
	{
		return Status(Status_InvalidCall, "Not an object member");
	}
	if (!m_iterObject.IsValid())
	{
		return Status(Status_OutOfBounds, "No more data");
	}
	if (Var::Type_Real != m_iterObject.Get().GetType())
	{
		return Status(Status_InvalidCall, "Not a real");
	}
	*psName   = m_iterObject.Get().GetName();
	*plfValue = m_iterObject.Get().GetReal();
	m_iterObject.Next();

	return Status();
}

//array item - will return Status_OutOfBounds at the end of the array
Status DataReader::ReadReal(Double *plfValue)
{
	if (!m_bOK)
	{
		return Status(Status_NotInitialized, "Not initialized");
	}
	if (NULL == m_pVar)
	{
		return Status(Status_InvalidCall, "Root function not called");
	}
	if (!m_pVar->IsArray())
	{
		return Status(Status_InvalidCall, "Not an array item");
	}
	if (!m_iterArray.IsValid())
	{
		return Status(Status_OutOfBounds, "No more data");
	}
	if (Var::Type_Real != m_iterArray.Get().GetType())
	{
		return Status(Status_InvalidCall, "Not a real");
	}
	*plfValue = m_iterArray.Get().GetReal();
	m_iterArray.Next();

	return Status();
}

//object member - will return Status_OutOfBounds at the end of the object
Status DataReader::ReadString(String *psName, String *psValue)
{
	if (!m_bOK)
	{
		return Status(Status_NotInitialized, "Not initialized");
	}
	if (NULL == m_pVar)
	{
		return Status(Status_InvalidCall, "Root function not called");
	}
	if (!m_pVar->IsObject())
	{
		return Status(Status_InvalidCall, "Not an object member");
	}
	if (!m_iterObject.IsValid())
	{
		return Status(Status_OutOfBounds, "No more data");
	}
	if (Var::Type_String != m_iterObject.Get().GetType())
	{
		return Status(Status_InvalidCall, "Not a string");
	}
	*psName  = m_iterObject.Get().GetName();
	*psValue = m_iterObject.Get().GetString();
	m_iterObject.Next();

	return Status();
}

//array item - will return Status_OutOfBounds at the end of the array
Status DataReader::ReadString(String *psValue)
{
	if (!m_bOK)
	{
		return Status(Status_NotInitialized, "Not initialized");
	}
	if (NULL == m_pVar)
	{
		return Status(Status_InvalidCall, "Root function not called");
	}
	if (!m_pVar->IsArray())
	{
		return Status(Status_InvalidCall, "Not an array item");
	}
	if (!m_iterArray.IsValid())
	{
		return Status(Status_OutOfBounds, "No more data");
	}
	if (Var::Type_String != m_iterArray.Get().GetType())
	{
		return Status(Status_InvalidCall, "Not a string");
	}
	*psValue = m_iterArray.Get().GetString();
	m_iterArray.Next();

	return Status();
}

//object member
Status DataReader::BeginObject(String *psName)
{
	if (!m_bOK)
	{
		return Status(Status_NotInitialized, "Not initialized");
	}
	if (NULL == m_pVar)
	{
		return Status(Status_InvalidCall, "Root function not called");
	}
	if (!m_pVar->IsObject())
	{
		return Status(Status_InvalidCall, "Not an object member");
	}
	if (!m_iterObject.IsValid())
	{
		return Status(Status_OutOfBounds, "No more data");
	}
	if (Var::Type_Object != m_iterObject.Get().GetType())
	{
		return Status(Status_InvalidCall, "Not an object");
	}
	*psName = m_iterObject.Get().GetName();

	Iterator iter;

	iter.bIsObject  = true;
	iter.iterObject = m_iterObject;

	m_stackIterators.push(iter);

	m_pVar       = &m_iterObject.Get();
	m_iterObject = m_pVar->GetObjectConstIterator();

	m_stackIterators.top().iterObject.Next();

	return Status();
}

//array item
Status DataReader::BeginObject()
{
	if (!m_bOK)
	{
		return Status(Status_NotInitialized, "Not initialized");
	}
	if (NULL == m_pVar)
	{
		return Status(Status_InvalidCall, "Root function not called");
	}
	if (!m_pVar->IsArray())
	{
		return Status(Status_InvalidCall, "Not an array item");
	}
	if (!m_iterArray.IsValid())
	{
		return Status(Status_OutOfBounds, "No more data");
	}
	if (Var::Type_Object != m_iterArray.Get().GetType())
	{
		return Status(Status_InvalidCall, "Not an object");
	}

	Iterator iter;

	iter.bIsObject = false;
	iter.iterArray = m_iterArray;

	m_stackIterators.push(iter);

	m_pVar       = &m_iterArray.Get();
	m_iterObject = m_pVar->GetObjectConstIterator();

	m_stackIterators.top().iterArray.Next();

	return Status();
}

//object member
Status DataReader::BeginArray(String *psName)
{
	if (!m_bOK)
	{
		return Status(Status_NotInitialized, "Not initialized");
	}
	if (NULL == m_pVar)
	{
		return Status(Status_InvalidCall, "Root function not called");
	}
	if (!m_pVar->IsObject())
	{
		return Status(Status_InvalidCall, "Not an object member");
	}
	if (!m_iterObject.IsValid())
	{
		return Status(Status_OutOfBounds, "No more data");
	}
	if (Var::Type_Array != m_iterObject.Get().GetType())
	{
		return Status(Status_InvalidCall, "Not an array");
	}
	*psName = m_iterObject.Get().GetName();

	Iterator iter;

	iter.bIsObject  = true;
	iter.iterObject = m_iterObject;

	m_stackIterators.push(iter);

	m_pVar       = &m_iterObject.Get();
	m_iterArray  = m_pVar->GetArrayConstIterator();

	m_stackIterators.top().iterObject.Next();

	return Status();
}

//array item
Status DataReader::BeginArray()
{
	if (!m_bOK)
	{
		return Status(Status_NotInitialized, "Not initialized");
	}
	if (NULL == m_pVar)
	{
		return Status(Status_InvalidCall, "Root function not called");
	}
	if (!m_pVar->IsArray())
	{
		return Status(Status_InvalidCall, "Not an array item");
	}
	if (!m_iterArray.IsValid())
	{
		return Status(Status_OutOfBounds, "No more data");
	}
	if (Var::Type_Array != m_iterArray.Get().GetType())
	{
		return Status(Status_InvalidCall, "Not an array");
	}

	Iterator iter;

	iter.bIsObject = false;
	iter.iterArray = m_iterArray;

	m_stackIterators.push(iter);

	m_pVar      = &m_iterArray.Get();
	m_iterArray = m_pVar->GetArrayConstIterator();

	m_stackIterators.top().iterArray.Next();

	return Status();
}

//array item
Size DataReader::GetItemsCount()
{
	if (!m_bOK)
	{
		return 0;
	}
	if (NULL == m_pVar)
	{
		return 0;
	}
	if (m_pVar->IsObject())
	{
		return m_pVar->GetObjectMembersCount();
	}
	if (m_pVar->IsArray())
	{
		return m_pVar->GetArrayItemsCount();
	}

	return 0;
}

Status DataReader::EndObject()
{
	if (!m_bOK)
	{
		return Status(Status_NotInitialized, "Not initialized");
	}
	if (NULL == m_pVar)
	{
		return Status(Status_InvalidCall, "Root function not called");
	}
	if (!m_pVar->IsObject())
	{
		return Status(Status_InvalidCall, "Not an object");
	}
	if (m_stackIterators.empty())
	{
		return Status(Status_InvalidCall, "Out of order");
	}
	if (m_stackIterators.top().bIsObject)
	{
		m_iterObject = m_stackIterators.top().iterObject;
	}
	else
	{
		m_iterArray = m_stackIterators.top().iterArray;
	}
	m_pVar       = m_pVar->GetParent();
	m_stackIterators.pop();

	return Status();
}

Status DataReader::EndArray()
{
	if (!m_bOK)
	{
		return Status(Status_NotInitialized, "Not initialized");
	}
	if (NULL == m_pVar)
	{
		return Status(Status_InvalidCall, "Root function not called");
	}
	if (!m_pVar->IsArray())
	{
		return Status(Status_InvalidCall, "Not an array");
	}
	if (m_stackIterators.empty())
	{
		return Status(Status_InvalidCall, "Out of order");
	}
	if (m_stackIterators.top().bIsObject)
	{
		m_iterObject = m_stackIterators.top().iterObject;
	}
	else
	{
		m_iterArray = m_stackIterators.top().iterArray;
	}
	m_pVar      = m_pVar->GetParent();
	m_iterArray = m_stackIterators.top().iterArray;
	m_stackIterators.pop();

	return Status();
}

}//namespace JSON

}//namespace Data

}//namespace CX

