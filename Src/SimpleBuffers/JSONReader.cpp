/* 
 * CX - C++ framework for general purpose development
 *
 * https://github.com/draede/cx
 * 
 * Copyright (C) 2014 - 2022 draede
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
 
#include "CX/precomp.hpp"
#include "CX/SimpleBuffers/JSONReader.hpp"
#include "CX/Data/JSON/RapidJSONStreams.hpp"
#include "CX/Stack.hpp"
#include "stringbuffer.h"
#include "writer.h"


namespace CX
{

namespace SimpleBuffers
{

JSONReader::JSONReader()
{
	m_pDoc = NULL;
}

JSONReader::~JSONReader()
{
	if (NULL != m_pDoc)
	{
		delete m_pDoc;
		m_pDoc = NULL;
	}
}

Status JSONReader::Begin(IO::IInputStream *pInputStream)
{
	if (NULL == pInputStream)
	{
		return Status(Status_InvalidArg, "Invalid input stream");
	}
	if (NULL != m_pDoc)
	{
		delete m_pDoc;
		m_pDoc = NULL;
	}
	while (!m_stackStates.empty())
	{
		m_stackStates.pop();
	}

	Status status;

	m_pDoc = NULL;
	status.Clear();
	for (;;)
	{
		if (NULL == (m_pDoc = new (std::nothrow) rapidjson::Document()))
		{
			status = Status(Status_MemAllocFailed, "Failed to allocate document");

			break;
		}
		Data::JSON::RapidJSONInputStream str(pInputStream);

		m_pDoc->ParseStream<0>(str);
		if (m_pDoc->HasParseError())
		{
			status = Status(Status_ParseFailed, "Failed to parse JSON");

			break;
		}
		if (!m_pDoc->IsObject())
		{
			status = Status(Status_InvalidArg, "Root node must be an object");

			break;
		}

		break;
	}
	if (status.IsNOK())
	{
		if (NULL != m_pDoc)
		{
			delete m_pDoc;
			m_pDoc = NULL;
		}

		return status;
	}
	m_stackStates.push(State_Root);
	
	return Status();
}

Status JSONReader::End()
{
	if (m_stackStates.empty())
	{
		return Status(Status_InvalidCall);
	}
	if (State_Root != m_stackStates.top().nState)
	{
		return Status(Status_InvalidCall);
	}
	m_stackStates.pop();
	if (!m_stackStates.empty())
	{
		return Status(Status_InvalidCall);
	}
	if (NULL != m_pDoc)
	{
		delete m_pDoc;
		m_pDoc = NULL;
	}

	return Status();
}

Status JSONReader::ReadBool(Bool &v, const Char *szName/* = NULL*/)
{
	return Read(v, szName);
}

Status JSONReader::ReadInt8(Int8 &v, const Char *szName/* = NULL*/)
{
	return Read(v, szName);
}

Status JSONReader::ReadUInt8(UInt8 &v, const Char *szName/* = NULL*/)
{
	return Read(v, szName);
}

Status JSONReader::ReadInt16(Int16 &v, const Char *szName/* = NULL*/)
{
	return Read(v, szName);
}

Status JSONReader::ReadUInt16(UInt16 &v, const Char *szName/* = NULL*/)
{
	return Read(v, szName);
}

Status JSONReader::ReadInt32(Int32 &v, const Char *szName/* = NULL*/)
{
	return Read(v, szName);
}

Status JSONReader::ReadUInt32(UInt32 &v, const Char *szName/* = NULL*/)
{
	return Read(v, szName);
}

Status JSONReader::ReadInt64(Int64 &v, const Char *szName/* = NULL*/)
{
	return Read(v, szName);
}

Status JSONReader::ReadUInt64(UInt64 &v, const Char *szName/* = NULL*/)
{
	return Read(v, szName);
}

Status JSONReader::ReadFloat(Float &v, const Char *szName/* = NULL*/)
{
	return Read(v, szName);
}

Status JSONReader::ReadDouble(Double &v, const Char *szName/* = NULL*/)
{
	return Read(v, szName);
}

Status JSONReader::ReadString(String &v, const Char *szName/* = NULL*/)
{
	return Read(v, szName);
}

Status JSONReader::ReadBLOB(BLOB &v, const Char *szName/* = NULL*/)
{
	return Read(v, szName);
}

Status JSONReader::ReadCustom(ICustom *pCustom, const Char *szName/* = NULL*/)
{
	rapidjson::Value *pValue;
	Status           status;

	if (m_stackStates.empty())
	{
		return Status(Status_InvalidCall);
	}
	if (State_Root == m_stackStates.top().nState)
	{
		if (0 < m_stackStates.top().cElems)
		{
			return Status(Status_InvalidCall, "Only one root object allowed");
		}
		if (!m_pDoc->IsObject())
		{
			return Status(Status_InvalidCall, "Root element must be an object");
		}

		pValue = m_pDoc;
		m_stackStates.pop();
	}
	else
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
		m_stackStates.top().cElems++;
	}
	else
	{
		return Status(Status_InvalidCall, "Data must be placed inside objects or arrays");
	}

	if (!pValue->IsObject())
	{
		return Status(Status_InvalidCall, "Custom data must be an object");
	}

	struct Node
	{
		explicit Node(rapidjson::Value *pVal)
		{
			pValue = pVal;
			if (pValue->IsObject())
			{
				iterMembers = pValue->MemberBegin();
			}
			else
			{
				iterItems = pValue->Begin();
			}
		}

		rapidjson::Value                   *pValue;
		rapidjson::Value::MemberIterator   iterMembers;
		rapidjson::Value::ValueIterator    iterItems;
	};

	Stack<Node>::Type   stackNodes;
	Node                *pNode;
	const Char          *szMemberName;
	
	for (;;)
	{
		if (!stackNodes.empty())
		{
			pNode = &stackNodes.top();
			if (pNode->pValue->IsObject())
			{
				if (pNode->iterMembers == pNode->pValue->MemberEnd())
				{
					if ((status = pCustom->OnEndObject()).IsNOK())
					{
						return status;
					}
					stackNodes.pop();
					if (stackNodes.empty())
					{
						break;
					}
					
					continue;
				}
				szMemberName = pNode->iterMembers->name.GetString();
				pValue       = &pNode->iterMembers->value;
				pNode->iterMembers++;
			}
			else
			{
				if (pNode->iterItems == pNode->pValue->End())
				{
					if ((status = pCustom->OnEndArray()).IsNOK())
					{
						return status;
					}
					stackNodes.pop();
					if (stackNodes.empty())
					{
						break;
					}

					continue;
				}
				szMemberName = NULL;
				pValue       = &*pNode->iterItems;
				pNode->iterItems++;
			}
		}
		else
		{
			pNode        = NULL;
			szMemberName = NULL;
		}

		if (pValue->IsBool())
		{
			if ((status = pCustom->OnBool(pValue->GetBool(), szMemberName)).IsNOK())
			{
				return status;
			}
		}
		else
		if (pValue->IsInt64())
		{
			if ((status = pCustom->OnInt(pValue->GetInt64(), szMemberName)).IsNOK())
			{
				return status;
			}
		}
		else
		if (pValue->IsDouble())
		{
			if ((status = pCustom->OnDouble(pValue->GetDouble(), szMemberName)).IsNOK())
			{
				return status;
			}
		}
		else
		if (pValue->IsString())
		{
			if ((status = pCustom->OnString(pValue->GetString(), szMemberName)).IsNOK())
			{
				return status;
			}
		}
		else
		if (pValue->IsObject())
		{
			if ((status = pCustom->OnBeginObject(szMemberName)).IsNOK())
			{
				return status;
			}
			stackNodes.push(Node(pValue));
		}
		else
		if (pValue->IsArray())
		{
			if ((status = pCustom->OnBeginArray(szMemberName)).IsNOK())
			{
				return status;
			}
			stackNodes.push(Node(pValue));
		}

		if (stackNodes.empty())
		{
			break;
		}
	}

	return Status();
}

Status JSONReader::ReadCustom(ICustom::Type nType, void *pData, ICustom::Allocator *pAllocator, 
                              const Char *szName/* = NULL*/)
{
	CX_UNUSED(nType);
	CX_UNUSED(pData);
	CX_UNUSED(pAllocator);
	CX_UNUSED(szName);

	return Status_NotSupported;
}

Status JSONReader::BeginObject(const Char *szName/* = NULL*/)
{
	if (m_stackStates.empty())
	{
		return Status(Status_InvalidCall);
	}
	if (State_Root == m_stackStates.top().nState)
	{
		if (0 < m_stackStates.top().cElems)
		{
			return Status(Status_InvalidCall, "Only one root object allowed");
		}
		if (!m_pDoc->IsObject())
		{
			return Status(Status_InvalidCall, "Root element must be an object");
		}

		StateData sd(State_Object);

		sd.pValue = m_pDoc;

		m_stackStates.push(sd);

		return Status();
	}
	else
	if (State_Object == m_stackStates.top().nState)
	{
		if (NULL == szName)
		{
			return Status(Status_InvalidCall, "Member must have a name");
		}

		rapidjson::Value *pValue = m_stackStates.top().pValue;

		if (NULL == pValue)
		{
			return Status(Status_InvalidCall, "Internal error: null parent object");
		}
		if (!pValue->HasMember(szName))
		{
			return Status(Status_NotFound, "No such element : {1}", szName);
		}
		if (!(*pValue)[szName].IsObject())
		{
			return Status(Status_InvalidCall, "Element {1} is not an object", szName);
		}

		StateData sd(State_Object);

		sd.pValue = &(*pValue)[szName];

		m_stackStates.push(sd);

		return Status();
	}
	else
	if (State_Array == m_stackStates.top().nState)
	{
		if (NULL != szName)
		{
			return Status(Status_InvalidCall, "Item must not have a name");
		}
		rapidjson::Value *pValue = m_stackStates.top().pValue;

		if (NULL == pValue)
		{
			return Status(Status_InvalidCall, "Internal error: null parent array");
		}
		if (pValue->Capacity() <= m_stackStates.top().cElems)
		{
			return Status(Status_NoMoreItems, "No such element : {1}", m_stackStates.top().cElems);
		}
		if (!(*pValue)[(rapidjson::SizeType)m_stackStates.top().cElems].IsObject())
		{
			return Status(Status_InvalidCall, "Element {1} is not an object", m_stackStates.top().cElems);
		}

		StateData sd(State_Object);

		sd.pValue = &(*pValue)[(rapidjson::SizeType)m_stackStates.top().cElems];

		m_stackStates.push(sd);

		return Status();
	}
	else
	{
		return Status(Status_InvalidCall, "Objects must be placed as root or inside objects or arrays");
	}
}

Status JSONReader::EndObject()
{
	if (m_stackStates.empty())
	{
		return Status(Status_InvalidCall, "Begin was not called");
	}

	Status status;

	if (State_Object != m_stackStates.top().nState)
	{
		return Status(Status_InvalidCall, "Must be in an object");
	}
	m_stackStates.pop();
	if (m_stackStates.empty())
	{
		return Status(Status_InvalidCall, "Begin was not called");
	}
	m_stackStates.top().cElems++;

	return Status();
}

Status JSONReader::BeginArray(const Char *szName/* = NULL*/)
{
	if (m_stackStates.empty())
	{
		return Status(Status_InvalidCall);
	}
	if (State_Object == m_stackStates.top().nState)
	{
		if (NULL == szName)
		{
			return Status(Status_InvalidCall, "Member must have a name");
		}

		rapidjson::Value *pValue = m_stackStates.top().pValue;

		if (NULL == pValue)
		{
			return Status(Status_InvalidCall, "Internal error: null parent object");
		}
		if (!pValue->HasMember(szName))
		{
			return Status(Status_NotFound, "No such element : {1}", szName);
		}
		if (!(*pValue)[szName].IsArray())
		{
			return Status(Status_InvalidCall, "Element {1} is not an array", szName);
		}

		StateData sd(State_Array);

		sd.pValue = &(*pValue)[szName];

		m_stackStates.push(sd);

		return Status();
	}
	else
	if (State_Array == m_stackStates.top().nState)
	{
		if (NULL != szName)
		{
			return Status(Status_InvalidCall, "Item must not have a name");
		}
		rapidjson::Value *pValue = m_stackStates.top().pValue;

		if (NULL == pValue)
		{
			return Status(Status_InvalidCall, "Internal error: null parent array");
		}
		if (pValue->Capacity() <= m_stackStates.top().cElems)
		{
			return Status(Status_NoMoreItems, "No such element : {1}", m_stackStates.top().cElems);
		}
		if (!(*pValue)[(rapidjson::SizeType)m_stackStates.top().cElems].IsArray())
		{
			return Status(Status_InvalidCall, "Element {1} is not an array", m_stackStates.top().cElems);
		}

		StateData sd(State_Array);

		sd.pValue = &(*pValue)[(rapidjson::SizeType)m_stackStates.top().cElems];

		m_stackStates.push(sd);

		return Status();
	}
	else
	{
		return Status(Status_InvalidCall, "Arrays must be placed inside objects or arrays");
	}
}

Status JSONReader::EndArray()
{
	if (m_stackStates.empty())
	{
		return Status(Status_InvalidCall, "Begin was not called");
	}

	Status status;

	if (State_Array != m_stackStates.top().nState)
	{
		return Status(Status_InvalidCall, "Must be in an array");
	}
	m_stackStates.pop();
	if (m_stackStates.empty())
	{
		return Status(Status_InvalidCall, "Begin was not called");
	}
	m_stackStates.top().cElems++;

	return Status();
}

}//namespace SimpleBuffers

}//namespace CX
