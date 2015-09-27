/* 
 * CX - C++ framework for general purpose development
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


#include "CX/SimpleBuffers/IReader.hpp"
#include "CX/Stack.hpp"
#include "CX/Print.hpp"
#include "CX/Str/Z85BinStr.hpp"
#include "document.h"
#include "CX/APIDefs.hpp"


namespace CX
{

namespace SimpleBuffers
{

class CX_API JSONReader : public IReader, public IObject
{
public:

	JSONReader();

	~JSONReader();

	virtual Status Begin(IO::IInputStream *pInputStream);

	virtual Status End();

	virtual Status ReadBool(Bool &v, const Char *szName = NULL);

	virtual Status ReadInt8(Int8 &v, const Char *szName = NULL);

	virtual Status ReadUInt8(UInt8 &v, const Char *szName = NULL);

	virtual Status ReadInt16(Int16 &v, const Char *szName = NULL);

	virtual Status ReadUInt16(UInt16 &v, const Char *szName = NULL);

	virtual Status ReadInt32(Int32 &v, const Char *szName = NULL);

	virtual Status ReadUInt32(UInt32 &v, const Char *szName = NULL);

	virtual Status ReadInt64(Int64 &v, const Char *szName = NULL);

	virtual Status ReadUInt64(UInt64 &v, const Char *szName = NULL);

	virtual Status ReadFloat(Float &v, const Char *szName = NULL);

	virtual Status ReadDouble(Double &v, const Char *szName = NULL);

	virtual Status ReadString(String &v, const Char *szName = NULL);

	virtual Status ReadBLOB(BLOB &v, const Char *szName = NULL);

	virtual Status BeginObject(const Char *szName = NULL);

	virtual Status EndObject();

	virtual Status BeginArray(const Char *szName = NULL);

	virtual Status EndArray();

private:

	enum State
	{
		State_Root,
		State_Object,
		State_Array,
	};

	struct StateData
	{
		StateData()
		{
		}

		StateData(State nState)
		{
			this->nState = nState;
			this->cElems = 0;
			this->pValue = NULL;
		}

		State            nState;
		Size             cElems;
		rapidjson::Value *pValue;

	};

	typedef Stack<StateData>::Type  StatesStack;

#pragma warning(push)
#pragma warning(disable: 4251)
	StatesStack           m_stackStates;
#pragma warning(pop)
	rapidjson::Document   *m_pDoc;

	template <typename T> bool IsType(rapidjson::Value *pValue);
	
	template <typename T> Status GetVal(rapidjson::Value *pValue, T &val);

	template <typename T>
	Status Read(T &v, const Char *szName = NULL)
	{
		if (m_stackStates.empty())
		{
			return Status(Status_InvalidCall, "Begin was not called");
		}

		rapidjson::Value *pValue;
		Status           status;

		if (State_Object == m_stackStates.top().nState)
		{
			if (NULL == szName)
			{
				return Status(Status_InvalidCall, "Member must have a name");
			}
			if (NULL == m_stackStates.top().pValue)
			{
				return Status(Status_InvalidCall, "Internal error: parent element is null");
			}
			if (!m_stackStates.top().pValue->HasMember(szName))
			{
				return Status(Status_NotFound, "Member [{1}] not found", szName);
			}
			pValue = &(*m_stackStates.top().pValue)[szName];
		}
		else
		if (State_Array == m_stackStates.top().nState)
		{
			if (NULL != szName)
			{
				return Status(Status_InvalidCall, "Item must not have a name");
			}
			if (NULL == m_stackStates.top().pValue)
			{
				return Status(Status_InvalidCall, "Internal error: parent element is null");
			}
			if (m_stackStates.top().pValue->Capacity() <= m_stackStates.top().cElems)
			{
				return Status(Status_NoMoreItems, "Member [{1}] not found", m_stackStates.top().cElems);
			}
			pValue = &(*m_stackStates.top().pValue)[(rapidjson::SizeType)m_stackStates.top().cElems];
		}
		else
		{
			return Status(Status_InvalidCall, "Data must be placed inside objects or arrays");
		}
		if (!IsType<T>(pValue))
		{
			return Status(Status_InvalidCall, "Invalid element type");
		}
		if ((status = GetVal<T>(pValue, v)).IsNOK())
		{
			return status;
		}
		m_stackStates.top().cElems++;

		return Status();
	}

};

template <> inline bool JSONReader::IsType<Bool>(rapidjson::Value *pValue) { return pValue->IsBool(); }
template <> inline bool JSONReader::IsType<Int8>(rapidjson::Value *pValue) { return pValue->IsInt(); }
template <> inline bool JSONReader::IsType<UInt8>(rapidjson::Value *pValue) { return pValue->IsUint(); }
template <> inline bool JSONReader::IsType<Int16>(rapidjson::Value *pValue) { return pValue->IsInt(); }
template <> inline bool JSONReader::IsType<UInt16>(rapidjson::Value *pValue) { return pValue->IsUint(); }
template <> inline bool JSONReader::IsType<Int32>(rapidjson::Value *pValue) { return pValue->IsInt(); }
template <> inline bool JSONReader::IsType<UInt32>(rapidjson::Value *pValue) { return pValue->IsUint(); }
template <> inline bool JSONReader::IsType<Int64>(rapidjson::Value *pValue) { return pValue->IsInt(); }
template <> inline bool JSONReader::IsType<UInt64>(rapidjson::Value *pValue) { return pValue->IsUint(); }
template <> inline bool JSONReader::IsType<Float>(rapidjson::Value *pValue) { return pValue->IsDouble(); }
template <> inline bool JSONReader::IsType<Double>(rapidjson::Value *pValue) { return pValue->IsDouble(); }
template <> inline bool JSONReader::IsType<String>(rapidjson::Value *pValue) { return pValue->IsString(); }
template <> inline bool JSONReader::IsType<BLOB>(rapidjson::Value *pValue) { return pValue->IsString(); }


template <> inline Status JSONReader::GetVal<Bool>(rapidjson::Value *pValue, Bool &val) 
{ 
	val = pValue->GetBool(); 
	
	return Status(); 
}

template <> inline Status JSONReader::GetVal<Int8>(rapidjson::Value *pValue, Int8 &val) 
{ 
	val = (Int8)pValue->GetInt(); 
	
	return Status(); 
}

template <> inline Status JSONReader::GetVal<UInt8>(rapidjson::Value *pValue, UInt8 &val) 
{ 
	val = (UInt8)pValue->GetUint(); 
	
	return Status(); 
}

template <> inline Status JSONReader::GetVal<Int16>(rapidjson::Value *pValue, Int16 &val) 
{ 
	val = (Int16)pValue->GetInt(); 
	
	return Status(); 
}

template <> inline Status JSONReader::GetVal<UInt16>(rapidjson::Value *pValue, UInt16 &val) 
{ 
	val = (UInt16)pValue->GetUint(); 
	
	return Status(); 
}

template <> inline Status JSONReader::GetVal<Int32>(rapidjson::Value *pValue, Int32 &val) 
{ 
	val = (Int32)pValue->GetInt(); 
	
	return Status(); 
}

template <> inline Status JSONReader::GetVal<UInt32>(rapidjson::Value *pValue, UInt32 &val) 
{ 
	val = (UInt32)pValue->GetUint(); 
	
	return Status(); 
}

template <> inline Status JSONReader::GetVal<Int64>(rapidjson::Value *pValue, Int64 &val) 
{ 
	val = (Int64)pValue->GetInt(); 
	
	return Status(); 
}

template <> inline Status JSONReader::GetVal<UInt64>(rapidjson::Value *pValue, UInt64 &val) 
{ 
	val = (UInt64)pValue->GetUint(); 
	
	return Status(); 
}

template <> inline Status JSONReader::GetVal<Float>(rapidjson::Value *pValue, Float &val) 
{ 
	val = (Float)pValue->GetDouble(); 
	
	return Status(); 
}

template <> inline Status JSONReader::GetVal<Double>(rapidjson::Value *pValue, Double &val)
{
	val = (Double)pValue->GetDouble(); 
	
	return Status();
}

template <> inline Status JSONReader::GetVal<String>(rapidjson::Value *pValue, String &val) 
{ 
	val = pValue->GetString(); 
	return Status(); 
}

template <> inline Status JSONReader::GetVal<BLOB>(rapidjson::Value *pValue, BLOB &val)
{
	Str::Z85BinStr z85;
	Status         status;

	try
	{
		val.resize(z85.GetBinSizeFromStrLen(pValue->GetString(), pValue->GetStringLength()));
	}
	catch (...)
	{
		return Status(Status_MemAllocFailed, "Failed to allocate blob");
	}
	if ((status = z85.FromString(pValue->GetString(), pValue->GetStringLength(), &val[0], val.size())).IsNOK())
	{
		return Status(Status_ParseFailed, "Failed to parse blob");
	}

	return Status();
}

}//namespace SimpleBuffers

}//namespace CX

