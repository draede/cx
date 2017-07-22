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

#include "CX/SimpleBuffers/BSONValue.hpp"
#include "CX/SimpleBuffers/BSONReader.hpp"
#include "CX/Stack.hpp"
#include "bson.h"


namespace CX
{

namespace SimpleBuffers
{

//from Andrei Alexandrescu
static inline Size GetUInt64DigitsCount(UInt64 nValue)
{
	Size cDigitsCount = 1;

	for (;;)
	{
		if (nValue < 10)
		{
			return cDigitsCount;
		}
		if (nValue < 100)
		{
			return cDigitsCount + 1;
		}
		if (nValue < 1000)
		{
			return cDigitsCount + 2;
		}
		if (nValue < 10000)
		{
			return cDigitsCount + 3;
		}
		nValue /= 10000U;
		cDigitsCount += 4;
	}
}

//from Andrei Alexandrescu
static inline void UInt64ToString(UInt64 nValue, Char *szDst)
{
	static const char digits[201] =
		"0001020304050607080910111213141516171819"
		"2021222324252627282930313233343536373839"
		"4041424344454647484950515253545556575859"
		"6061626364656667686970717273747576777879"
		"8081828384858687888990919293949596979899";

	Size const cLen = GetUInt64DigitsCount(nValue);

	Size cNext = cLen - 1;

	while (nValue >= 100)
	{
		const UInt64 i = (nValue % 100) * 2;

		nValue /= 100;
		szDst[cNext] = digits[i + 1];
		szDst[cNext - 1] = digits[i];
		cNext -= 2;
	}
	if (nValue < 10)
	{
		szDst[cNext] = (Char)('0' + nValue);
	}
	else
	{
		UInt64 i = UInt64(nValue) * 2;

		szDst[cNext] = digits[i + 1];
		szDst[cNext - 1] = digits[i];
	}
	szDst[cLen] = 0;
}

BSONValue::BSONValue()
{
	m_pAllData  = NULL;
	m_cbAllSize = 0;
	m_pData     = NULL;
	m_cbSize    = 0;
}

BSONValue::BSONValue(const BSONValue &v)
{
	m_pAllData  = NULL;
	m_cbAllSize = 0;
	m_pData     = NULL;
	m_cbSize    = 0;
	Copy(v);
}

BSONValue::~BSONValue()
{
	Init();
}

const void *BSONValue::GetBuffer() const
{
	return m_pData;
}

void *BSONValue::GetBuffer()
{
	return m_pData;
}

Size BSONValue::GetBufferSize() const 
{
	return m_cbSize;
}

const void *BSONValue::GetAllBuffer() const
{
	return m_pAllData;
}

void *BSONValue::GetAllBuffer()
{
	return m_pAllData;
}

Size BSONValue::GetAllBufferSize() const
{
	return m_cbAllSize;
}

int BSONValue::Compare(const BSONValue &v) const
{
	if (m_cbSize < v.m_cbSize)
	{
		return -1;
	}
	else
	if (m_cbSize > v.m_cbSize)
	{
		return 1;
	}
	else
	{
		if (0 == m_cbSize)
		{
			return 0;
		}
		else
		{
			return memcmp(m_pData, v.m_pData, m_cbSize);
		}
	}
}

Status BSONValue::Copy(const BSONValue &v)
{
	if (0 == v.m_cbSize)
	{
		Init();

		return Status();
	}

	void *pData;

	if (NULL == (pData = m_allocator.Alloc(NULL, v.m_cbSize, m_allocator.pCTX)))
	{
		return Status_MemAllocFailed;
	}
	Init();
	memcpy(pData, v.m_pData, v.m_cbSize);
	m_pData  = m_pAllData  = pData;
	m_cbSize = m_cbAllSize = v.m_cbSize;

	return Status();
}

BSONValue &BSONValue::operator=(const BSONValue &v)
{
	Copy(v);

	return *this;
}

bool BSONValue::operator<(const BSONValue &v) const
{
	return (0 > Compare(v));
}

bool BSONValue::operator>(const BSONValue &v) const
{
	return (0 < Compare(v));
}

ICustom::Allocator *BSONValue::GetAllocator()
{
	return &m_allocator;
}

void BSONValue::Init()
{
	if (NULL != m_pAllData)
	{
		m_allocator.Free(m_pAllData, NULL);
		m_pAllData  = NULL;
		m_cbAllSize = 0;
		m_pData     = NULL;
		m_cbSize    = 0;
	}
}

Status BSONValue::Write(IWriter *pWriter, const CX::Char *szName/* = NULL*/) const
{
	Status status;

	if (NULL == m_pData || 0 == m_cbSize)
	{
		if (!(status = pWriter->BeginObject(szName)))
		{
			return status;
		}
		if (!(status = pWriter->EndObject()))
		{
			return status;
		}

		return Status();
	}

	bson_t bson;

	if (!bson_init_static(&bson, (const uint8_t *)m_pData, m_cbSize))
	{
		return Status_OperationFailed;
	}

	bson_iter_t iter;

	if (!bson_iter_init(&iter, &bson))
	{
		return Status_OperationFailed;
	}

	if (!bson_iter_next(&iter))
	{
		return Status_OperationFailed;
	}

	struct Node
	{
		bson_iter_t iter;
		bool        bObject;
	};

	typedef Stack<Node>::Type   NodesStack;

	bson_iter_t *pIter;
	NodesStack  stackNodes;
	const Char  *szKey;
	bool        bObject;
	bson_type_t nType;

	szKey   = szName;
	pIter   = &iter;
	bObject = false;
	for (;;)
	{
		if (!stackNodes.empty())
		{
			szKey   = NULL;
			pIter   = &stackNodes.top().iter;
			bObject = stackNodes.top().bObject;
			if (!bson_iter_next(pIter))
			{
				if (bObject)
				{
					if ((status = pWriter->EndObject()).IsNOK())
					{
						return status;
					}
				}
				else
				{
					if ((status = pWriter->EndArray()).IsNOK())
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
			else
			{
				if (bObject)
				{
					szKey = bson_iter_key(pIter);
				}
			}
		}

		nType = bson_iter_type(pIter);

		if (BSON_TYPE_BOOL == nType)
		{
			if ((status = pWriter->WriteBool(bson_iter_bool_unsafe(pIter), szKey)).IsNOK())
			{
				return status;
			}
		}
		else
		if (BSON_TYPE_INT32 == nType)
		{
			if ((status = pWriter->WriteInt32(bson_iter_int32_unsafe(pIter), szKey)).IsNOK())
			{
				return status;
			}
		}
		else
		if (BSON_TYPE_INT64 == nType)
		{
			if ((status = pWriter->WriteInt64(bson_iter_int64_unsafe(pIter), szKey)).IsNOK())
			{
				return status;
			}
		}
		else
		if (BSON_TYPE_DOUBLE == nType)
		{
			if ((status = pWriter->WriteDouble(bson_iter_double(pIter), szKey)).IsNOK())
			{
				return status;
			}
		}
		else
		if (BSON_TYPE_UTF8 == nType)
		{
			size_t     cLen;
			const char *szStr = bson_iter_utf8_unsafe(pIter, &cLen);

			if ((status = pWriter->WriteString(szStr, szKey)).IsNOK())
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
			if (!(status = pWriter->BeginObject(szKey)))
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
			if (!(status = pWriter->BeginArray(szKey)))
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

Status BSONValue::Read(IReader *pReader, const CX::Char *szName/* = NULL*/)
{
	BSONReader::CustomData data;
	Status                 status;

	if ((status = pReader->ReadCustom(ICustom::Type_BSON, &data, &m_allocator, szName)))
	{
		Init();
		m_pData  = m_pAllData  = data.pData;
		m_cbSize = m_cbAllSize = data.cbSize;

		return Status();
	}

	struct Helper : public ICustom
	{
		struct Node
		{
			bson_t *pBSON;
			bool   bObject;
			Size   cIndex;
		};

		typedef Stack<Node>::Type   NodesStack;

		bson_t       *pRoot;
		NodesStack   stackNodes;
		Char         szKey[64];

		virtual Status OnBeginObject(const Char *szName = NULL)
		{
			if (stackNodes.empty())
			{
				Node node;

				node.cIndex  = 0;
				node.bObject = true;
				node.pBSON   = pRoot;
				stackNodes.push(node);
			}
			else
			if (stackNodes.top().bObject)
			{
				if (1 < stackNodes.size() && NULL == szName)
				{
					return Status_InvalidCall;
				}

				Node node;

				node.cIndex  = 0;
				node.bObject = true;
				node.pBSON   = (bson_t *)malloc(sizeof(bson_t));
				if (!bson_append_document_begin(stackNodes.top().pBSON, szName, -1, node.pBSON))
				{
					free(node.pBSON);

					return Status_OperationFailed;
				}
				stackNodes.top().cIndex++;
				stackNodes.push(node);
			}
			else
			{
				if (NULL != szName)
				{
					return Status_InvalidCall;
				}
				UInt64ToString(stackNodes.top().cIndex, szKey);

				Node node;

				node.cIndex  = 0;
				node.bObject = true;
				node.pBSON   = (bson_t *)malloc(sizeof(bson_t));
				if (!bson_append_document_begin(stackNodes.top().pBSON, szKey, -1, node.pBSON))
				{
					free(node.pBSON);

					return Status_OperationFailed;
				}
				stackNodes.top().cIndex++;
				stackNodes.push(node);
			}

			return Status();
		}

		virtual Status OnEndObject()
		{
			if (stackNodes.empty())
			{
				return Status_InvalidCall;
			}
			if (!stackNodes.top().bObject)
			{
				return Status_InvalidCall;
			}

			bson_t *pBSON = stackNodes.top().pBSON;

			stackNodes.pop();

			if (pBSON != pRoot)
			{
				if (!bson_append_document_end(stackNodes.top().pBSON, pBSON))
				{
					free(pBSON);

					return Status_OperationFailed;
				}
				free(pBSON);
			}

			return Status();
		}

		virtual Status OnBeginArray(const Char *szName = NULL)
		{
			if (stackNodes.empty())
			{
				return Status_InvalidCall;
			}
			if (stackNodes.top().bObject)
			{
				if (1 < stackNodes.size() && NULL == szName)
				{
					return Status_InvalidCall;
				}

				Node node;

				node.cIndex  = 0;
				node.bObject = false;
				node.pBSON   = (bson_t *)malloc(sizeof(bson_t));
				if (!bson_append_array_begin(stackNodes.top().pBSON, szName, -1, node.pBSON))
				{
					free(node.pBSON);

					return Status_OperationFailed;
				}
				stackNodes.top().cIndex++;
				stackNodes.push(node);
			}
			else
			{
				if (NULL != szName)
				{
					return Status_InvalidCall;
				}
				UInt64ToString(stackNodes.top().cIndex, szKey);

				Node node;

				node.cIndex  = 0;
				node.bObject = false;
				node.pBSON   = (bson_t *)malloc(sizeof(bson_t));
				if (!bson_append_array_begin(stackNodes.top().pBSON, szKey, -1, node.pBSON))
				{
					free(node.pBSON);

					return Status_OperationFailed;
				}
				stackNodes.top().cIndex++;
				stackNodes.push(node);
			}

			return Status();
		}

		virtual Status OnEndArray()
		{
			if (stackNodes.empty())
			{
				return Status_InvalidCall;
			}
			if (stackNodes.top().bObject)
			{
				return Status_InvalidCall;
			}

			bson_t *pBSON = stackNodes.top().pBSON;

			stackNodes.pop();

			if (stackNodes.empty())
			{
				free(pBSON);

				return Status_InvalidCall;
			}
			if (!stackNodes.top().bObject)
			{
				free(pBSON);

				return Status_InvalidCall;
			}
			if (!bson_append_array_end(stackNodes.top().pBSON, pBSON))
			{
				free(pBSON);

				return Status_OperationFailed;
			}
			free(pBSON);

			return Status();
		}

		virtual Status OnBool(Bool v, const Char *szName = NULL)
		{
			if (stackNodes.empty())
			{
				return Status_InvalidCall;
			}
			if (stackNodes.top().bObject)
			{
				if (NULL == szName)
				{
					return Status_InvalidCall;
				}
				if (!bson_append_bool(stackNodes.top().pBSON, szName, -1, v))
				{
					return Status_OperationFailed;
				}
			}
			else
			{
				if (NULL != szName)
				{
					return Status_InvalidCall;
				}
				UInt64ToString(stackNodes.top().cIndex, szKey);
				if (!bson_append_bool(stackNodes.top().pBSON, szKey, -1, v))
				{
					return Status_OperationFailed;
				}
			}
			stackNodes.top().cIndex++;

			return Status();
		}

		virtual Status OnInt(Int64 v, const Char *szName = NULL)
		{
			if (stackNodes.empty())
			{
				return Status_InvalidCall;
			}
			if (stackNodes.top().bObject)
			{
				if (NULL == szName)
				{
					return Status_InvalidCall;
				}
				if (!bson_append_int64(stackNodes.top().pBSON, szName, -1, v))
				{
					return Status_OperationFailed;
				}
			}
			else
			{
				if (NULL != szName)
				{
					return Status_InvalidCall;
				}
				UInt64ToString(stackNodes.top().cIndex, szKey);
				if (!bson_append_int64(stackNodes.top().pBSON, szKey, -1, v))
				{
					return Status_OperationFailed;
				}
			}
			stackNodes.top().cIndex++;

			return Status();
		}

		virtual Status OnDouble(Double v, const Char *szName = NULL)
		{
			if (stackNodes.empty())
			{
				return Status_InvalidCall;
			}
			if (stackNodes.top().bObject)
			{
				if (NULL == szName)
				{
					return Status_InvalidCall;
				}
				if (!bson_append_double(stackNodes.top().pBSON, szName, -1, v))
				{
					return Status_OperationFailed;
				}
			}
			else
			{
				if (NULL != szName)
				{
					return Status_InvalidCall;
				}
				UInt64ToString(stackNodes.top().cIndex, szKey);
				if (!bson_append_double(stackNodes.top().pBSON, szKey, -1, v))
				{
					return Status_OperationFailed;
				}
			}
			stackNodes.top().cIndex++;

			return Status();
		}

		virtual Status OnString(const Char *v, const Char *szName = NULL)
		{
			if (stackNodes.empty())
			{
				return Status_InvalidCall;
			}
			if (stackNodes.top().bObject)
			{
				if (NULL == szName)
				{
					return Status_InvalidCall;
				}
				if (!bson_append_utf8(stackNodes.top().pBSON, szName, -1, v, -1))
				{
					return Status_OperationFailed;
				}
			}
			else
			{
				if (NULL != szName)
				{
					return Status_InvalidCall;
				}
				UInt64ToString(stackNodes.top().cIndex, szKey);
				if (!bson_append_utf8(stackNodes.top().pBSON, szKey, -1, v, -1))
				{
					return Status_OperationFailed;
				}
			}
			stackNodes.top().cIndex++;

			return Status();
		}

	};

	Helper helper;

	data.pData  = NULL;
	data.cbSize = 0;
	if (NULL == (helper.pRoot = bson_new_from_buffer((uint8_t **)&data.pData, &data.cbSize, m_allocator.Alloc, 
	                                                 m_allocator.pCTX)))
	{
		return Status_OperationFailed;
	}
	if (!(status = pReader->ReadCustom(&helper, szName)))
	{
		bson_destroy(helper.pRoot);

		return status;
	}
	Init();

	m_pAllData  = data.pData;
	m_cbAllSize = data.cbSize;
	m_pData     = (void *)bson_get_data(helper.pRoot);
	m_cbSize    = (Size)helper.pRoot->len;
	bson_destroy(helper.pRoot);

	return Status();
}

}//namespace SimpleBuffers

}//namespace CX
