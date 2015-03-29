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

#include "CX/SB/TextDataReader.hpp"
#include "document.h"
#include "stringbuffer.h"
#include "writer.h"
#include "CX/Stack.hpp"
#include "../Src/Data/JSON/RapidJSONStreams.hpp"
#include "CX/Print.hpp"


namespace CX
{

namespace SB
{

class Helper
{
public:

	Status BeginMember(const Char *szName)
	{
		if (m_stack.empty())
		{
			return Status(Status_InvalidCall, "Out of order");
		}
		if (State_Object != m_stack.top().nState)
		{
			return Status(Status_InvalidCall, "Out of order");;
		}
		if (!m_stack.top().pVal->IsObject())
		{
			return Status(Status_InvalidCall, "Out of order");;
		}
		if (m_stack.top().pVal->MemberEnd() == m_stack.top().iterMembers)
		{
			return Status(Status_InvalidCall, "Out of order");
		}
		if (0 != cx_strcmp(m_stack.top().iterMembers->name.GetString(), szName))
		{
			return Status(Status_InvalidCall, "Out of order");
		}

		return Status();
	}

	Status EndMember()
	{
		if (m_stack.empty())
		{
			return Status(Status_InvalidCall, "Out of order");
		}
		if (State_Object != m_stack.top().nState)
		{
			return Status(Status_InvalidCall, "Out of order");;
		}
		if (!m_stack.top().pVal->IsObject())
		{
			return Status(Status_InvalidCall, "Out of order");;
		}
		if (m_stack.top().pVal->MemberEnd() == m_stack.top().iterMembers)
		{
			return Status(Status_InvalidCall, "Out of order");
		}
		m_stack.top().iterMembers++;

		return Status();
	}

	Status BeginItem()
	{
		if (m_stack.empty())
		{
			return Status(Status_InvalidCall, "Out of order");
		}
		if (State_Array != m_stack.top().nState)
		{
			return Status(Status_InvalidCall, "Out of order");;
		}
		if (!m_stack.top().pVal->IsArray())
		{
			return Status(Status_InvalidCall, "Out of order");;
		}
		if (m_stack.top().pVal->End() == m_stack.top().iterItems)
		{
			return Status(Status_InvalidCall, "Out of order");
		}

		return Status();
	}

	Status EndItem()
	{
		if (m_stack.empty())
		{
			return Status(Status_InvalidCall, "Out of order");
		}
		if (State_Array != m_stack.top().nState)
		{
			return Status(Status_InvalidCall, "Out of order");;
		}
		if (!m_stack.top().pVal->IsArray())
		{
			return Status(Status_InvalidCall, "Out of order");;
		}
		if (m_stack.top().pVal->End() == m_stack.top().iterItems)
		{
			return Status(Status_InvalidCall, "Out of order");
		}
		m_stack.top().iterItems++;

		return Status();
	}

	Status BeginObject(Size *pcCount)
	{
		if (m_stack.empty())
		{
			if (!m_root.IsObject())
			{
				return Status(Status_InvalidCall, "Out of order");
			}
			*pcCount = m_root.MemberCount();

			StateData sd;

			sd.nState      = State_Object;
			sd.pVal        = &m_root;
			sd.iterMembers = sd.pVal->MemberBegin();

			m_stack.push(sd);
		}
		else
		{
			if (State_Object == m_stack.top().nState)
			{
				if (!m_stack.top().pVal->IsObject())
				{
					return Status(Status_InvalidCall, "Out of order");
				}
				if (m_stack.top().pVal->MemberEnd() == m_stack.top().iterMembers)
				{
					return Status(Status_InvalidCall, "Out of order");
				}
				if (!m_stack.top().iterMembers->value.IsObject())
				{
					return Status(Status_InvalidCall, "Out of order");
				}

				*pcCount = m_stack.top().iterMembers->value.MemberCount();

				StateData sd;

				sd.nState      = State_Object;
				sd.pVal        = &m_stack.top().iterMembers->value;
				sd.iterMembers = sd.pVal->MemberBegin();
				m_stack.push(sd);
			}
			else
			if (State_Array == m_stack.top().nState)
			{
				if (!m_stack.top().pVal->IsArray())
				{
					return Status(Status_InvalidCall, "Out of order");
				}
				if (m_stack.top().pVal->End() == m_stack.top().iterItems)
				{
					return Status(Status_InvalidCall, "Out of order");
				}
				if (!m_stack.top().iterItems->IsObject())
				{
					return Status(Status_InvalidCall, "Out of order");
				}

				*pcCount = m_stack.top().iterItems->MemberCount();

				StateData sd;

				sd.nState      = State_Object;
				sd.pVal        = &*m_stack.top().iterItems;
				sd.iterMembers = sd.pVal->MemberBegin();
				m_stack.push(sd);
			}
			else
			{
				return Status(Status_InvalidCall, "Out of order");
			}
		}

		return Status();
	}

	Status EndObject()
	{
		if (m_stack.empty())
		{
			return Status(Status_InvalidCall, "Out of order");
		}
		if (State_Object != m_stack.top().nState)
		{
			return Status(Status_InvalidCall, "Out of order");
		}
		if (!m_stack.top().pVal->IsObject())
		{
			return Status(Status_InvalidCall, "Out of order");
		}
		m_stack.pop();

		return Status();
	}

	Status BeginArray(Size *pcCount)
	{
		if (m_stack.empty())
		{
			if (!m_root.IsArray())
			{
				return Status(Status_InvalidCall, "Out of order");
			}
			*pcCount = m_root.Capacity();

			StateData sd;

			sd.nState    = State_Array;
			sd.pVal      = &m_root;
			sd.iterItems = sd.pVal->Begin();
			m_stack.push(sd);
		}
		else
		{
			if (State_Object == m_stack.top().nState)
			{
				if (!m_stack.top().pVal->IsObject())
				{
					return Status(Status_InvalidCall, "Out of order");
				}
				if (m_stack.top().pVal->MemberEnd() == m_stack.top().iterMembers)
				{
					return Status(Status_InvalidCall, "Out of order");
				}
				if (!m_stack.top().iterMembers->value.IsArray())
				{
					return Status(Status_InvalidCall, "Out of order");
				}

				*pcCount = m_stack.top().iterMembers->value.Capacity();

				StateData sd;

				sd.nState      = State_Array;
				sd.pVal        = &m_stack.top().iterMembers->value;
				sd.iterItems   = sd.pVal->Begin();
				m_stack.push(sd);
			}
			else
			if (State_Array == m_stack.top().nState)
			{
				if (!m_stack.top().pVal->IsArray())
				{
					return Status(Status_InvalidCall, "Out of order");
				}
				if (m_stack.top().pVal->End() == m_stack.top().iterItems)
				{
					return Status(Status_InvalidCall, "Out of order");
				}
				if (!m_stack.top().iterItems->IsArray())
				{
					return Status(Status_InvalidCall, "Out of order");
				}

				*pcCount = m_stack.top().iterItems->Capacity();

				StateData sd;

				sd.nState      = State_Array;
				sd.pVal        = &*m_stack.top().iterItems;
				sd.iterItems   = sd.pVal->Begin();
				m_stack.push(sd);
			}
			else
			{
				return Status(Status_InvalidCall, "Out of order");
			}
		}

		return Status();
	}

	Status EndArray()
	{
		if (m_stack.empty())
		{
			return Status(Status_InvalidCall, "Out of order");
		}
		if (State_Array != m_stack.top().nState)
		{
			return Status(Status_InvalidCall, "Out of order");
		}
		if (!m_stack.top().pVal->IsArray())
		{
			return Status(Status_InvalidCall, "Out of order");
		}
		m_stack.pop();

		return Status();
	}


	Status GetCurrentValue(const rapidjson::Value **ppVal)
	{
		if (m_stack.empty())
		{
			return Status(Status_InvalidCall, "Out of order");
		}
		if (State_Object == m_stack.top().nState)
		{
			if (!m_stack.top().pVal->IsObject())
			{
				return Status(Status_InvalidCall, "Out of order");
			}
			if (m_stack.top().pVal->MemberEnd() == m_stack.top().iterMembers)
			{
				return Status(Status_InvalidCall, "Out of order");
			}
			*ppVal = &m_stack.top().iterMembers->value;
		}
		else
		if (State_Array == m_stack.top().nState)
		{
			if (!m_stack.top().pVal->IsArray())
			{
				return Status(Status_InvalidCall, "Out of order");
			}
			if (m_stack.top().pVal->End() == m_stack.top().iterItems)
			{
				return Status(Status_InvalidCall, "Out of order");
			}
			*ppVal = &*m_stack.top().iterItems;
		}
		else
		{
			return Status(Status_InvalidCall, "Out of order");
		}

		return Status();
	}

	Status ReadBool(Bool *pbValue)
	{
		const rapidjson::Value *pVal;
		Status                 status;

		if ((status = GetCurrentValue(&pVal)).IsNOK())
		{
			return status;
		}
		if (!pVal->IsBool())
		{
			return Status(Status_InvalidCall, "Out of order");
		}
		*pbValue = pVal->GetBool();

		return Status();
	}

	Status ReadInt8(Int8 *pnValue)
	{
		const rapidjson::Value *pVal;
		Status                 status;

		if ((status = GetCurrentValue(&pVal)).IsNOK())
		{
			return status;
		}
		if (m_stack.empty())
		{
			return Status(Status_InvalidCall, "Out of order");
		}
		if (!pVal->IsInt64())
		{
			return Status(Status_InvalidCall, "Out of order");
		}
		*pnValue = (Int8)pVal->GetInt64();

		return Status();
	}

	Status ReadUInt8(UInt8 *puValue)
	{
		const rapidjson::Value *pVal;
		Status                 status;

		if ((status = GetCurrentValue(&pVal)).IsNOK())
		{
			return status;
		}
		if (m_stack.empty())
		{
			return Status(Status_InvalidCall, "Out of order");
		}
		if (!pVal->IsUint64())
		{
			return Status(Status_InvalidCall, "Out of order");
		}
		*puValue = (UInt8)pVal->GetUint64();

		return Status();
	}

	Status ReadInt16(Int16 *pnValue)
	{
		const rapidjson::Value *pVal;
		Status                 status;

		if ((status = GetCurrentValue(&pVal)).IsNOK())
		{
			return status;
		}
		if (m_stack.empty())
		{
			return Status(Status_InvalidCall, "Out of order");
		}
		if (!pVal->IsInt64())
		{
			return Status(Status_InvalidCall, "Out of order");
		}
		*pnValue = (Int16)pVal->GetInt64();

		return Status();
	}

	Status ReadUInt16(UInt16 *puValue)
	{
		const rapidjson::Value *pVal;
		Status                 status;

		if ((status = GetCurrentValue(&pVal)).IsNOK())
		{
			return status;
		}
		if (m_stack.empty())
		{
			return Status(Status_InvalidCall, "Out of order");
		}
		if (!pVal->IsUint64())
		{
			return Status(Status_InvalidCall, "Out of order");
		}
		*puValue = (UInt16)pVal->GetUint64();

		return Status();
	}

	Status ReadInt32(Int32 *pnValue)
	{
		const rapidjson::Value *pVal;
		Status                 status;

		if ((status = GetCurrentValue(&pVal)).IsNOK())
		{
			return status;
		}
		if (m_stack.empty())
		{
			return Status(Status_InvalidCall, "Out of order");
		}
		if (!pVal->IsInt64())
		{
			return Status(Status_InvalidCall, "Out of order");
		}
		*pnValue = (Int32)pVal->GetInt64();

		return Status();
	}

	Status ReadUInt32(UInt32 *puValue)
	{
		const rapidjson::Value *pVal;
		Status                 status;

		if ((status = GetCurrentValue(&pVal)).IsNOK())
		{
			return status;
		}
		if (m_stack.empty())
		{
			return Status(Status_InvalidCall, "Out of order");
		}
		if (!pVal->IsUint64())
		{
			return Status(Status_InvalidCall, "Out of order");
		}
		*puValue = (UInt32)pVal->GetUint64();

		return Status();
	}

	Status ReadInt64(Int64 *pnValue)
	{
		const rapidjson::Value *pVal;
		Status                 status;

		if ((status = GetCurrentValue(&pVal)).IsNOK())
		{
			return status;
		}
		if (m_stack.empty())
		{
			return Status(Status_InvalidCall, "Out of order");
		}
		if (!pVal->IsInt64())
		{
			return Status(Status_InvalidCall, "Out of order");
		}
		*pnValue = (Int64)pVal->GetInt64();

		return Status();
	}

	Status ReadUInt64(UInt64 *puValue)
	{
		const rapidjson::Value *pVal;
		Status                 status;

		if ((status = GetCurrentValue(&pVal)).IsNOK())
		{
			return status;
		}
		if (m_stack.empty())
		{
			return Status(Status_InvalidCall, "Out of order");
		}
		if (!pVal->IsUint64())
		{
			return Status(Status_InvalidCall, "Out of order");
		}
		*puValue = (UInt64)pVal->GetUint64();

		return Status();
	}

	Status ReadFloat(Float *pfValue)
	{
		const rapidjson::Value *pVal;
		Status                 status;

		if ((status = GetCurrentValue(&pVal)).IsNOK())
		{
			return status;
		}
		if (m_stack.empty())
		{
			return Status(Status_InvalidCall, "Out of order");
		}
		if (!pVal->IsDouble())
		{
			return Status(Status_InvalidCall, "Out of order");
		}
		*pfValue = (Float)pVal->GetDouble();

		return Status();
	}

	Status ReadDouble(Double *plfValue)
	{
		const rapidjson::Value *pVal;
		Status                 status;

		if ((status = GetCurrentValue(&pVal)).IsNOK())
		{
			return status;
		}
		if (m_stack.empty())
		{
			return Status(Status_InvalidCall, "Out of order");
		}
		if (!pVal->IsDouble())
		{
			return Status(Status_InvalidCall, "Out of order");
		}
		*plfValue = (Double)pVal->GetDouble();

		return Status();
	}

	Status ReadString(String *psValue)
	{
		const rapidjson::Value *pVal;
		Status                 status;

		if ((status = GetCurrentValue(&pVal)).IsNOK())
		{
			return status;
		}
		if (m_stack.empty())
		{
			return Status(Status_InvalidCall, "Out of order");
		}
		if (!pVal->IsString())
		{
			return Status(Status_InvalidCall, "Out of order");
		}
		*psValue = pVal->GetString();

		return Status();
	}

	Status ReadWString(WString *pwsValue)
	{
		const rapidjson::Value *pVal;
		Status                 status;

		if ((status = GetCurrentValue(&pVal)).IsNOK())
		{
			return status;
		}
		if (m_stack.empty())
		{
			return Status(Status_InvalidCall, "Out of order");
		}
		if (!pVal->IsString())
		{
			return Status(Status_InvalidCall, "Out of order");
		}

		return Print(pwsValue, "{1}", pVal->GetString());
	}

	enum State
	{
		State_Object,
		State_Array,
	};

	typedef struct _StateData
	{
		_StateData()
		{
		}

		State                                  nState;
		const rapidjson::Value                 *pVal;
		rapidjson::Value::ConstMemberIterator  iterMembers;
		rapidjson::Value::ConstValueIterator   iterItems;
	}StateData;

	typedef Stack<StateData>::Type   StatesStack;

	StatesStack           m_stack;
	rapidjson::Document   m_doc;
	rapidjson::Value      m_root;

};

#pragma warning(suppress: 6262)
TextDataReader::TextDataReader(IO::IInputStream *pInputStream)
{
	Data::JSON::RapidJSONInputStream str(pInputStream);
	Helper                           *pHelper = New<Helper>();

	pHelper->m_doc.ParseStream<0>(str);

	/*
	rapidjson::StringBuffer strbuf;
	rapidjson::Writer<rapidjson::StringBuffer> writer(strbuf);
	pHelper->m_doc.Accept(writer);
	FILE *pFile = fopen("c:\\out.json", "wb");
	fprintf(pFile, "%s", strbuf.GetString());
	fclose(pFile);
	*/

	if (pHelper->m_doc.HasParseError())
	{
		Print(stdout, "PARSE ERROR = {1}\n", (int)pHelper->m_doc.GetParseError());
		Print(stdout, "PARSE OFFSET = {1}\n", pHelper->m_doc.GetErrorOffset());

		Delete(pHelper);
		m_pHelper = NULL;
	}
	else
	{
		pHelper->m_root.Swap(pHelper->m_doc);
		m_pHelper = pHelper;
	}
}

TextDataReader::~TextDataReader()
{
	Helper *pHelper = (Helper *)m_pHelper;

	if (NULL != pHelper)
	{
		Delete(pHelper);
		pHelper = NULL;
	}
}

Status TextDataReader::BeginMember(const Char *szName)
{
	Helper *pHelper = (Helper *)m_pHelper;

	if (NULL == pHelper)
	{
		return Status(Status_NotInitialized, "Failed to load/parse JSON");
	}

	return pHelper->BeginMember(szName);
}

Status TextDataReader::EndMember()
{
	Helper *pHelper = (Helper *)m_pHelper;

	if (NULL == pHelper)
	{
		return Status(Status_NotInitialized, "Failed to load/parse JSON");
	}

	return pHelper->EndMember();
}

Status TextDataReader::BeginItem()
{
	Helper *pHelper = (Helper *)m_pHelper;

	if (NULL == pHelper)
	{
		return Status(Status_NotInitialized, "Failed to load/parse JSON");
	}

	return pHelper->BeginItem();
}

Status TextDataReader::EndItem()
{
	Helper *pHelper = (Helper *)m_pHelper;

	if (NULL == pHelper)
	{
		return Status(Status_NotInitialized, "Failed to load/parse JSON");
	}

	return pHelper->EndItem();
}

Status TextDataReader::BeginObject(Size *pcCount)
{
	Helper *pHelper = (Helper *)m_pHelper;

	if (NULL == pHelper)
	{
		return Status(Status_NotInitialized, "Failed to load/parse JSON");
	}

	return pHelper->BeginObject(pcCount);
}

Status TextDataReader::EndObject()
{
	Helper *pHelper = (Helper *)m_pHelper;

	if (NULL == pHelper)
	{
		return Status(Status_NotInitialized, "Failed to load/parse JSON");
	}

	return pHelper->EndObject();
}

Status TextDataReader::BeginArray(Size *pcCount)
{
	Helper *pHelper = (Helper *)m_pHelper;

	if (NULL == pHelper)
	{
		return Status(Status_NotInitialized, "Failed to load/parse JSON");
	}

	return pHelper->BeginArray(pcCount);
}

Status TextDataReader::EndArray()
{
	Helper *pHelper = (Helper *)m_pHelper;

	if (NULL == pHelper)
	{
		return Status(Status_NotInitialized, "Failed to load/parse JSON");
	}

	return pHelper->EndArray();
}

Status TextDataReader::ReadBool(Bool *pbValue)
{
	Helper *pHelper = (Helper *)m_pHelper;

	if (NULL == pHelper)
	{
		return Status(Status_NotInitialized, "Failed to load/parse JSON");
	}

	return pHelper->ReadBool(pbValue);
}

Status TextDataReader::ReadInt8(Int8 *pnValue)
{
	Helper *pHelper = (Helper *)m_pHelper;

	if (NULL == pHelper)
	{
		return Status(Status_NotInitialized, "Failed to load/parse JSON");
	}

	return pHelper->ReadInt8(pnValue);
}

Status TextDataReader::ReadUInt8(UInt8 *puValue)
{
	Helper *pHelper = (Helper *)m_pHelper;

	if (NULL == pHelper)
	{
		return Status(Status_NotInitialized, "Failed to load/parse JSON");
	}

	return pHelper->ReadUInt8(puValue);
}

Status TextDataReader::ReadInt16(Int16 *pnValue)
{
	Helper *pHelper = (Helper *)m_pHelper;

	if (NULL == pHelper)
	{
		return Status(Status_NotInitialized, "Failed to load/parse JSON");
	}

	return pHelper->ReadInt16(pnValue);
}

Status TextDataReader::ReadUInt16(UInt16 *puValue)
{
	Helper *pHelper = (Helper *)m_pHelper;

	if (NULL == pHelper)
	{
		return Status(Status_NotInitialized, "Failed to load/parse JSON");
	}

	return pHelper->ReadUInt16(puValue);
}

Status TextDataReader::ReadInt32(Int32 *pnValue)
{
	Helper *pHelper = (Helper *)m_pHelper;

	if (NULL == pHelper)
	{
		return Status(Status_NotInitialized, "Failed to load/parse JSON");
	}

	return pHelper->ReadInt32(pnValue);
}

Status TextDataReader::ReadUInt32(UInt32 *puValue)
{
	Helper *pHelper = (Helper *)m_pHelper;

	if (NULL == pHelper)
	{
		return Status(Status_NotInitialized, "Failed to load/parse JSON");
	}

	return pHelper->ReadUInt32(puValue);
}

Status TextDataReader::ReadInt64(Int64 *pnValue)
{
	Helper *pHelper = (Helper *)m_pHelper;

	if (NULL == pHelper)
	{
		return Status(Status_NotInitialized, "Failed to load/parse JSON");
	}

	return pHelper->ReadInt64(pnValue);
}

Status TextDataReader::ReadUInt64(UInt64 *puValue)
{
	Helper *pHelper = (Helper *)m_pHelper;

	if (NULL == pHelper)
	{
		return Status(Status_NotInitialized, "Failed to load/parse JSON");
	}

	return pHelper->ReadUInt64(puValue);
}

Status TextDataReader::ReadFloat(Float *pfValue)
{
	Helper *pHelper = (Helper *)m_pHelper;

	if (NULL == pHelper)
	{
		return Status(Status_NotInitialized, "Failed to load/parse JSON");
	}

	return pHelper->ReadFloat(pfValue);
}

Status TextDataReader::ReadDouble(Double *plfValue)
{
	Helper *pHelper = (Helper *)m_pHelper;

	if (NULL == pHelper)
	{
		return Status(Status_NotInitialized, "Failed to load/parse JSON");
	}

	return pHelper->ReadDouble(plfValue);
}

Status TextDataReader::ReadString(String *psValue)
{
	Helper *pHelper = (Helper *)m_pHelper;

	if (NULL == pHelper)
	{
		return Status(Status_NotInitialized, "Failed to load/parse JSON");
	}

	return pHelper->ReadString(psValue);
}

Status TextDataReader::ReadWString(WString *pwsValue)
{
	Helper *pHelper = (Helper *)m_pHelper;

	if (NULL == pHelper)
	{
		return Status(Status_NotInitialized, "Failed to load/parse JSON");
	}

	return pHelper->ReadWString(pwsValue);
}

}//namespace SB

}//namespace CX

