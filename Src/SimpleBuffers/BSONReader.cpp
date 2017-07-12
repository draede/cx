/* 
 * CX - C++ framework for general purpose development
 *
 * https://github.com/draede/cx
 * 
 * Copyright (C) 2014 - 2017 draede - draede [at] outlook [dot] com
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

#include "CX/SimpleBuffers/BSONReader.hpp"


namespace CX
{

namespace SimpleBuffers
{

BSONReader::BSONReader()
{
	m_pBSON = NULL;
	m_pData = NULL;
}

BSONReader::~BSONReader()
{
	if (NULL != m_pBSON)
	{
		bson_destroy(m_pBSON);
		m_pBSON = NULL;
	}
	if (NULL != m_pData)
	{
		bson_free(m_pData);
		m_pData = NULL;
	}
}

Status BSONReader::Begin(IO::IInputStream *pInputStream)
{
	if (NULL != m_pBSON)
	{
		bson_destroy(m_pBSON);
		m_pBSON = NULL;
	}
	if (NULL != m_pData)
	{
		bson_free(m_pData);
		m_pData = NULL;
	}

	UInt64  cbSize;
	size_t  cbDataSize;
	size_t  cbAckSize;
	Status  status;

	if (!(status = pInputStream->GetSize(&cbSize)))
	{
		return status;
	}
	if (1073741824 < cbSize)
	{
		return Status_InvalidArg;
	}
	cbDataSize = (Size)cbSize;
	if (NULL == (m_pData = (uint8_t *)bson_realloc(NULL, cbDataSize)))
	{
		return Status_MemAllocFailed;
	}
	if (!(status = pInputStream->Read(m_pData, cbDataSize, &cbAckSize)))
	{
		bson_free(m_pData);
		m_pData = NULL;

		return status;
	}
	if (cbAckSize != cbDataSize)
	{
		bson_free(m_pData);
		m_pData = NULL;

		return status;
	}
	if (NULL == (m_pBSON = bson_new_from_buffer(&m_pData, &cbDataSize, &bson_realloc_ctx, NULL)))
	{
		bson_free(m_pData);
		m_pData = NULL;

		return Status_OperationFailed;
	}

	return Status();
}

Status BSONReader::End()
{
	if (NULL == m_pBSON)
	{
		return Status_InvalidCall;
	}
	if (!m_stackStates.empty())
	{
		return Status_InvalidCall;
	}
	if (NULL != m_pBSON)
	{
		bson_destroy(m_pBSON);
		m_pBSON = NULL;
	}
	if (NULL != m_pData)
	{
		bson_free(m_pData);
		m_pData = NULL;
	}

	return Status();
}

Status BSONReader::ReadBool(Bool &v, const Char *szName/* = NULL*/)
{
	bson_type_t nType;
	Status      status;

	if (!(status = PrepareRead(szName, &nType)))
	{
		return status;
	}
	if (BSON_TYPE_BOOL != nType)
	{
		return Status_InvalidCall;
	}
	v = bson_iter_bool_unsafe(&m_stackStates.top().iter);

	return Status();
}

Status BSONReader::ReadInt8(Int8 &v, const Char *szName/* = NULL*/)
{
	bson_type_t nType;
	Status      status;

	if (!(status = PrepareRead(szName, &nType)))
	{
		return status;
	}
	if (BSON_TYPE_INT32 != nType)
	{
		return Status_InvalidCall;
	}
	v = (Int8)bson_iter_int32_unsafe(&m_stackStates.top().iter);

	return Status();
}

Status BSONReader::ReadUInt8(UInt8 &v, const Char *szName/* = NULL*/)
{
	bson_type_t nType;
	Status      status;

	if (!(status = PrepareRead(szName, &nType)))
	{
		return status;
	}
	if (BSON_TYPE_INT32 != nType)
	{
		return Status_InvalidCall;
	}
	v = (UInt8)bson_iter_int32_unsafe(&m_stackStates.top().iter);

	return Status();
}

Status BSONReader::ReadInt16(Int16 &v, const Char *szName/* = NULL*/)
{
	bson_type_t nType;
	Status      status;

	if (!(status = PrepareRead(szName, &nType)))
	{
		return status;
	}
	if (BSON_TYPE_INT32 != nType)
	{
		return Status_InvalidCall;
	}
	v = (Int16)bson_iter_int32_unsafe(&m_stackStates.top().iter);

	return Status();
}

Status BSONReader::ReadUInt16(UInt16 &v, const Char *szName/* = NULL*/)
{
	bson_type_t nType;
	Status      status;

	if (!(status = PrepareRead(szName, &nType)))
	{
		return status;
	}
	if (BSON_TYPE_INT32 != nType)
	{
		return Status_InvalidCall;
	}
	v = (UInt16)bson_iter_int32_unsafe(&m_stackStates.top().iter);

	return Status();
}

Status BSONReader::ReadInt32(Int32 &v, const Char *szName/* = NULL*/)
{
	bson_type_t nType;
	Status      status;

	if (!(status = PrepareRead(szName, &nType)))
	{
		return status;
	}
	if (BSON_TYPE_INT32 != nType)
	{
		return Status_InvalidCall;
	}
	v = (Int32)bson_iter_int32_unsafe(&m_stackStates.top().iter);

	return Status();
}

Status BSONReader::ReadUInt32(UInt32 &v, const Char *szName/* = NULL*/)
{
	bson_type_t nType;
	Status      status;

	if (!(status = PrepareRead(szName, &nType)))
	{
		return status;
	}
	if (BSON_TYPE_INT64 != nType)
	{
		return Status_InvalidCall;
	}
	v = (UInt32)bson_iter_int64_unsafe(&m_stackStates.top().iter);

	return Status();
}

Status BSONReader::ReadInt64(Int64 &v, const Char *szName/* = NULL*/)
{
	bson_type_t nType;
	Status      status;

	if (!(status = PrepareRead(szName, &nType)))
	{
		return status;
	}
	if (BSON_TYPE_INT64 != nType)
	{
		return Status_InvalidCall;
	}
	v = (Int64)bson_iter_int64_unsafe(&m_stackStates.top().iter);

	return Status();
}

Status BSONReader::ReadUInt64(UInt64 &v, const Char *szName/* = NULL*/)
{
	bson_type_t nType;
	Status      status;

	if (!(status = PrepareRead(szName, &nType)))
	{
		return status;
	}
	if (BSON_TYPE_INT64 != nType)
	{
		return Status_InvalidCall;
	}
	v = (UInt64)bson_iter_int64_unsafe(&m_stackStates.top().iter);

	return Status();
}

Status BSONReader::ReadFloat(Float &v, const Char *szName/* = NULL*/)
{
	bson_type_t nType;
	Status      status;

	if (!(status = PrepareRead(szName, &nType)))
	{
		return status;
	}
	if (BSON_TYPE_DOUBLE != nType)
	{
		return Status_InvalidCall;
	}
	v = (float)bson_iter_double_unsafe(&m_stackStates.top().iter);

	return Status();
}

Status BSONReader::ReadDouble(Double &v, const Char *szName/* = NULL*/)
{
	bson_type_t nType;
	Status      status;

	if (!(status = PrepareRead(szName, &nType)))
	{
		return status;
	}
	if (BSON_TYPE_DOUBLE != nType)
	{
		return Status_InvalidCall;
	}
	v = (double)bson_iter_double_unsafe(&m_stackStates.top().iter);

	return Status();
}

Status BSONReader::ReadString(String &v, const Char *szName/* = NULL*/)
{
	bson_type_t nType;
	Status      status;

	if (!(status = PrepareRead(szName, &nType)))
	{
		return status;
	}
	if (BSON_TYPE_UTF8 != nType)
	{
		return Status_InvalidCall;
	}
	size_t     cLen;
	const char *szStr = bson_iter_utf8_unsafe(&m_stackStates.top().iter, &cLen);
	v.assign(szStr, cLen);

	return Status();
}

Status BSONReader::ReadBLOB(BLOB &v, const Char *szName/* = NULL*/)
{
	bson_type_t nType;
	Status      status;

	if (!(status = PrepareRead(szName, &nType)))
	{
		return status;
	}
	if (BSON_TYPE_BINARY != nType)
	{
		return Status_InvalidCall;
	}
	bson_subtype_t nSubtype;
	uint32_t       cbSize;
	const uint8_t  *pData;
	bson_iter_binary(&m_stackStates.top().iter, &nSubtype, &cbSize, &pData);
	v.assign(pData, pData + cbSize);

	return Status();
}

Status BSONReader::ReadCustom(ICustom *pCustom, const Char *szName/* = NULL*/)
{
	bson_iter_t *pIter;
	bson_type_t nType;
	bool        bObject;

	if (m_stackStates.empty())
	{
		return Status_InvalidCall;
	}
	if (State_Object == m_stackStates.top().nState)
	{
		if (NULL == szName)
		{
			return Status_InvalidCall;
		}
		if (!bson_iter_find(&m_stackStates.top().iter, szName))
		{
			return Status_InvalidCall;
		}
		pIter   = &m_stackStates.top().iter;
		bObject = true;
	}
	else
	if (State_Array == m_stackStates.top().nState)
	{
		if (NULL != szName)
		{
			return Status_InvalidCall;
		}
		if (!bson_iter_next(&m_stackStates.top().iter))
		{
			return Status_NoMoreItems;
		}
		pIter   = &m_stackStates.top().iter;
		bObject = false;
	}
	else
	{
		return Status_InvalidCall;
	}

	nType = bson_iter_type(pIter);

	if (BSON_TYPE_DOCUMENT != nType)
	{
		return Status(Status_InvalidCall, "Custom data must be an object");
	}

	struct Node
	{
		bson_iter_t iter;
		bool        bObject;
	};

	typedef Stack<Node>::Type   NodesStack;

	NodesStack stackNodes;
	const Char *szKey;
	Status     status;

	for (;;)
	{
		if (!stackNodes.empty())
		{
			pIter   = &stackNodes.top().iter;
			bObject = stackNodes.top().bObject;
			if (!bson_iter_next(pIter))
			{
				if (bObject)
				{
					if ((status = pCustom->OnEndObject()).IsNOK())
					{
						return status;
					}
				}
				else
				{
					if ((status = pCustom->OnEndArray()).IsNOK())
					{
						return status;
					}
				}
				stackNodes.pop();
				if (stackNodes.empty())
				{
					break;
				}

				continue;
			}
		}

		nType = bson_iter_type(pIter);
		szKey = NULL;
		if (bObject)
		{
			szKey = bson_iter_key(pIter);
		}

		if (BSON_TYPE_BOOL == nType)
		{
			if ((status = pCustom->OnBool(bson_iter_bool_unsafe(pIter), szKey)).IsNOK())
			{
				return status;
			}
		}
		else
		if (BSON_TYPE_INT32 == nType)
		{
			if ((status = pCustom->OnInt(bson_iter_int32_unsafe(pIter), szKey)).IsNOK())
			{
				return status;
			}
		}
		else
		if (BSON_TYPE_INT64 == nType)
		{
			if ((status = pCustom->OnInt(bson_iter_int64_unsafe(pIter), szKey)).IsNOK())
			{
				return status;
			}
		}
		else
		if (BSON_TYPE_DOUBLE == nType)
		{
			if ((status = pCustom->OnDouble(bson_iter_double(pIter), szKey)).IsNOK())
			{
				return status;
			}
		}
		else
		if (BSON_TYPE_UTF8 == nType)
		{
			size_t     cLen;
			const char *szStr = bson_iter_utf8_unsafe(pIter, &cLen);

			if ((status = pCustom->OnString(szStr, szKey)).IsNOK())
			{
				return status;
			}
		}
		else
		if (BSON_TYPE_DOCUMENT == nType)
		{
			Node node;

			if (!bson_iter_recurse(pIter, &node.iter))
			{
				return Status_OperationFailed;
			}
			node.bObject = true;
			stackNodes.push(node);
			if (!(status = pCustom->OnBeginObject(szKey)))
			{
				return status;
			}
		}
		else
		if (BSON_TYPE_ARRAY == nType)
		{
			Node node;

			if (!bson_iter_recurse(pIter, &node.iter))
			{
				return Status_OperationFailed;
			}
			node.bObject = false;
			stackNodes.push(node);
			if (!(status = pCustom->OnBeginArray(szKey)))
			{
				return status;
			}
		}

		if (stackNodes.empty())
		{
			break;
		}
	}

	return Status();
}

Status BSONReader::ReadCustom(ICustom::Type nCustomType, void *pData, ICustom::Allocator *pAllocator, 
                              const Char *szName/* = NULL*/)
{
	if (ICustom::Type_BSON != nCustomType)
	{
		return Status_NotSupported;
	}

	BSONReader::CustomData *pCustomData = (BSONReader::CustomData *)pData;

	bson_iter_t   *pIter;
	bson_type_t   nType;
	uint32_t      cbBSONSize;
	const uint8_t *pBSONData;
	bool          bObject;

	if (m_stackStates.empty())
	{
		return Status_InvalidCall;
	}
	if (State_Object == m_stackStates.top().nState)
	{
		if (NULL == szName)
		{
			return Status_InvalidCall;
		}
		if (!bson_iter_find(&m_stackStates.top().iter, szName))
		{
			return Status_InvalidCall;
		}
		pIter   = &m_stackStates.top().iter;
		bObject = true;
	}
	else
	if (State_Array == m_stackStates.top().nState)
	{
		if (NULL != szName)
		{
			return Status_InvalidCall;
		}
		if (!bson_iter_next(&m_stackStates.top().iter))
		{
			return Status_NoMoreItems;
		}
		pIter   = &m_stackStates.top().iter;
		bObject = false;
	}
	else
	{
		return Status_InvalidCall;
	}

	nType = bson_iter_type(pIter);

	if (BSON_TYPE_DOCUMENT != nType)
	{
		return Status(Status_InvalidCall, "Custom data must be an object");
	}

	nType = bson_iter_type(pIter);
	bson_iter_document(pIter, &cbBSONSize, &pBSONData);
	if (NULL == pData || 0 == cbBSONSize)
	{
		return Status_InvalidCall;
	}
	if (NULL == (pCustomData->pData = pAllocator->Alloc(NULL, (Size)cbBSONSize, pAllocator->pCTX)))
	{
		return Status_MemAllocFailed;
	}
	pCustomData->cbSize = (Size)cbBSONSize;
	memcpy(pCustomData->pData, pBSONData, (Size)cbBSONSize);

	return Status();
}

Status BSONReader::BeginObject(const Char *szName/* = NULL*/)
{
	bson_type_t nType;

	if (NULL == m_pBSON)
	{
		return Status_InvalidCall;
	}
	if (m_stackStates.empty())
	{
		if (NULL != szName)
		{
			return Status_InvalidCall;
		}

		StateData state(State_Object);

		bson_iter_init(&state.iter, m_pBSON);
		m_stackStates.push(state);

		return Status();
	}
	else
	if (State_Object == m_stackStates.top().nState)
	{
		if (NULL == szName)
		{
			return Status_InvalidCall;
		}
		if (!bson_iter_find(&m_stackStates.top().iter, szName))
		{
			return Status_InvalidCall;
		}
		nType = bson_iter_type(&m_stackStates.top().iter);
		if (BSON_TYPE_DOCUMENT != nType)
		{
			return Status_InvalidCall;
		}

		StateData state(State_Object);

		if (!bson_iter_recurse(&m_stackStates.top().iter, &state.iter))
		{
			return Status_InvalidCall;
		}
		m_stackStates.push(state);

		return Status();
	}
	else
	if (State_Array == m_stackStates.top().nState)
	{
		if (NULL != szName)
		{
			return Status_InvalidCall;
		}
		if (!bson_iter_next(&m_stackStates.top().iter))
		{
			return Status_NoMoreItems;
		}
		nType = bson_iter_type(&m_stackStates.top().iter);
		if (BSON_TYPE_DOCUMENT != nType)
		{
			return Status_InvalidCall;
		}

		StateData state(State_Object);

		if (!bson_iter_recurse(&m_stackStates.top().iter, &state.iter))
		{
			return Status_InvalidCall;
		}
		m_stackStates.push(state);

		return Status();
	}
	else
	{
		return Status_InvalidCall;
	}
}

Status BSONReader::EndObject()
{
	if (NULL == m_pBSON)
	{
		return Status_InvalidCall;
	}
	if (m_stackStates.empty())
	{
		return Status_InvalidCall;
	}
	if (State_Object != m_stackStates.top().nState)
	{
		return Status_InvalidCall;
	}
	m_stackStates.pop();

	return Status();
}

Status BSONReader::BeginArray(const Char *szName/* = NULL*/)
{
	bson_type_t nType;

	if (NULL == m_pBSON)
	{
		return Status_InvalidCall;
	}
	if (m_stackStates.empty())
	{
		if (NULL != szName)
		{
			return Status_InvalidCall;
		}

		StateData state(State_Array);

		bson_iter_init(&state.iter, m_pBSON);
		m_stackStates.push(state);

		return Status();
	}
	else
	if (State_Object == m_stackStates.top().nState)
	{
		if (NULL == szName)
		{
			return Status_InvalidCall;
		}
		if (!bson_iter_find(&m_stackStates.top().iter, szName))
		{
			return Status_InvalidCall;
		}
		nType = bson_iter_type(&m_stackStates.top().iter);
		if (BSON_TYPE_ARRAY != nType)
		{
			return Status_InvalidCall;
		}

		StateData state(State_Array);

		if (!bson_iter_recurse(&m_stackStates.top().iter, &state.iter))
		{
			return Status_InvalidCall;
		}
		m_stackStates.push(state);

		return Status();
	}
	else
	if (State_Array == m_stackStates.top().nState)
	{
		if (NULL != szName)
		{
			return Status_InvalidCall;
		}
		if (!bson_iter_next(&m_stackStates.top().iter))
		{
			return Status_NoMoreItems;
		}
		nType = bson_iter_type(&m_stackStates.top().iter);
		if (BSON_TYPE_ARRAY != nType)
		{
			return Status_InvalidCall;
		}

		StateData state(State_Array);

		if (!bson_iter_recurse(&m_stackStates.top().iter, &state.iter))
		{
			return Status_InvalidCall;
		}
		m_stackStates.push(state);

		return Status();
	}
	else
	{
		return Status_InvalidCall;
	}
}

Status BSONReader::EndArray()
{
	if (NULL == m_pBSON)
	{
		return Status_InvalidCall;
	}
	if (m_stackStates.empty())
	{
		return Status_InvalidCall;
	}
	if (State_Array != m_stackStates.top().nState)
	{
		return Status_InvalidCall;
	}
	m_stackStates.pop();

	return Status();
}

Status BSONReader::PrepareRead(const Char *szName, bson_type_t *pnType)
{
	if (NULL == m_pBSON)
	{
		return Status_InvalidCall;
	}
	if (m_stackStates.empty())
	{
		return Status_InvalidCall;
	}
	if (State_Object == m_stackStates.top().nState)
	{
		if (NULL == szName)
		{
			return Status_InvalidCall;
		}
		if (!bson_iter_find(&m_stackStates.top().iter, szName))
		{
			return Status_InvalidCall;
		}
		*pnType = bson_iter_type(&m_stackStates.top().iter);

		return Status();
	}
	else
	if (State_Array == m_stackStates.top().nState)
	{
		if (NULL != szName)
		{
			return Status_InvalidCall;
		}
		if (!bson_iter_next(&m_stackStates.top().iter))
		{
			return Status_NoMoreItems;
		}
		*pnType = bson_iter_type(&m_stackStates.top().iter);

		return Status();
	}
	else
	{
		return Status_InvalidCall;
	}
}

}//namespace SimpleBuffers

}//namespace CX

