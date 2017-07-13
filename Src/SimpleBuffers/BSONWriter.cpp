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
 
#include "CX/SimpleBuffers/BSONWriter.hpp"
#include "CX/SimpleBuffers/BSONReader.hpp"


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

BSONWriter::BSONWriter()
{
	m_pBSON = NULL;
}

BSONWriter::~BSONWriter()
{
	if (NULL != m_pBSON)
	{
		bson_destroy(m_pBSON);
		free(m_pBSON);
		m_pBSON = NULL;
	}
}

Status BSONWriter::Begin(IO::IOutputStream *pOutputStream)
{
	if (NULL != m_pBSON)
	{
		bson_destroy(m_pBSON);
		free(m_pBSON);
		m_pBSON = NULL;
	}
	m_pBSON = (bson_t *)malloc(sizeof(bson_t));
	m_pOutputStream = pOutputStream;
	bson_init(m_pBSON);

	return Status();
}

Status BSONWriter::End()
{
	Size   cbAckSize;
	Status status;

	if (NULL == m_pBSON)
	{
		return Status_InvalidCall;
	}
	if (!m_stackStates.empty())
	{
		return Status_InvalidCall;
	}
	if (!(status = m_pOutputStream->Write(bson_get_data(m_pBSON), (Size)m_pBSON->len, &cbAckSize)))
	{
		return status;
	}
	if ((Size)m_pBSON->len != cbAckSize)
	{
		return Status_WriteFailed;
	}
	bson_destroy(m_pBSON);
	free(m_pBSON);
	m_pBSON = NULL;

	return Status();
}

Status BSONWriter::WriteBool(Bool v, const Char *szName/* = NULL*/)
{
	const char *pszName;
	Status     status;

	if (!(status = PrepareWrite(szName, &pszName)))
	{
		return status;
	}
	if (!bson_append_bool(m_stackStates.top().pBSON, pszName, -1, v))
	{
		return Status_OperationFailed;
	}
	m_stackStates.top().cCount++;

	return Status();
}

Status BSONWriter::WriteInt8(Int8 v, const Char *szName/* = NULL*/)
{
	const char *pszName;
	Status     status;

	if (!(status = PrepareWrite(szName, &pszName)))
	{
		return status;
	}
	if (!bson_append_int32(m_stackStates.top().pBSON, pszName, -1, (int32_t)v))
	{
		return Status_OperationFailed;
	}
	m_stackStates.top().cCount++;

	return Status();
}

Status BSONWriter::WriteUInt8(UInt8 v, const Char *szName/* = NULL*/)
{
	const char *pszName;
	Status     status;

	if (!(status = PrepareWrite(szName, &pszName)))
	{
		return status;
	}
	if (!bson_append_int32(m_stackStates.top().pBSON, pszName, -1, (int32_t)v))
	{
		return Status_OperationFailed;
	}
	m_stackStates.top().cCount++;

	return Status();
}

Status BSONWriter::WriteInt16(Int16 v, const Char *szName/* = NULL*/)
{
	const char *pszName;
	Status     status;

	if (!(status = PrepareWrite(szName, &pszName)))
	{
		return status;
	}
	if (!bson_append_int32(m_stackStates.top().pBSON, pszName, -1, (int32_t)v))
	{
		return Status_OperationFailed;
	}
	m_stackStates.top().cCount++;

	return Status();
}

Status BSONWriter::WriteUInt16(UInt16 v, const Char *szName/* = NULL*/)
{
	const char *pszName;
	Status     status;

	if (!(status = PrepareWrite(szName, &pszName)))
	{
		return status;
	}
	if (!bson_append_int32(m_stackStates.top().pBSON, pszName, -1, (int32_t)v))
	{
		return Status_OperationFailed;
	}
	m_stackStates.top().cCount++;

	return Status();
}

Status BSONWriter::WriteInt32(Int32 v, const Char *szName/* = NULL*/)
{
	const char *pszName;
	Status     status;

	if (!(status = PrepareWrite(szName, &pszName)))
	{
		return status;
	}
	if (!bson_append_int32(m_stackStates.top().pBSON, pszName, -1, (int32_t)v))
	{
		return Status_OperationFailed;
	}
	m_stackStates.top().cCount++;

	return Status();
}

Status BSONWriter::WriteUInt32(UInt32 v, const Char *szName/* = NULL*/)
{
	const char *pszName;
	Status     status;

	if (!(status = PrepareWrite(szName, &pszName)))
	{
		return status;
	}
	if (!bson_append_int64(m_stackStates.top().pBSON, pszName, -1, (int64_t)v))
	{
		return Status_OperationFailed;
	}
	m_stackStates.top().cCount++;

	return Status();
}

Status BSONWriter::WriteInt64(Int64 v, const Char *szName/* = NULL*/)
{
	const char *pszName;
	Status     status;

	if (!(status = PrepareWrite(szName, &pszName)))
	{
		return status;
	}
	if (!bson_append_int64(m_stackStates.top().pBSON, pszName, -1, (int64_t)v))
	{
		return Status_OperationFailed;
	}
	m_stackStates.top().cCount++;

	return Status();
}

Status BSONWriter::WriteUInt64(UInt64 v, const Char *szName/* = NULL*/)
{
	const char *pszName;
	Status     status;

	if (!(status = PrepareWrite(szName, &pszName)))
	{
		return status;
	}
	if (!bson_append_int64(m_stackStates.top().pBSON, pszName, -1, (int64_t)v))
	{
		return Status_OperationFailed;
	}
	m_stackStates.top().cCount++;

	return Status();
}

Status BSONWriter::WriteFloat(Float v, const Char *szName/* = NULL*/)
{
	const char *pszName;
	Status     status;

	if (!(status = PrepareWrite(szName, &pszName)))
	{
		return status;
	}
	if (!bson_append_double(m_stackStates.top().pBSON, pszName, -1, (double)v))
	{
		return Status_OperationFailed;
	}
	m_stackStates.top().cCount++;

	return Status();
}

Status BSONWriter::WriteDouble(Double v, const Char *szName/* = NULL*/)
{
	const char *pszName;
	Status     status;

	if (!(status = PrepareWrite(szName, &pszName)))
	{
		return status;
	}
	if (!bson_append_double(m_stackStates.top().pBSON, pszName, -1, (double)v))
	{
		return Status_OperationFailed;
	}
	m_stackStates.top().cCount++;

	return Status();
}

Status BSONWriter::WriteString(const String &v, const Char *szName/* = NULL*/)
{
	const char *pszName;
	Status     status;

	if (!(status = PrepareWrite(szName, &pszName)))
	{
		return status;
	}
	if (!bson_append_utf8(m_stackStates.top().pBSON, pszName, -1, v.c_str(), (int)v.size()))
	{
		return Status_OperationFailed;
	}
	m_stackStates.top().cCount++;

	return Status();
}

Status BSONWriter::WriteBLOB(const void *pData, Size cbSize, const Char *szName/* = NULL*/)
{
	const char *pszName;
	Status     status;

	if (!(status = PrepareWrite(szName, &pszName)))
	{
		return status;
	}
	if (!bson_append_binary(m_stackStates.top().pBSON, pszName, -1, BSON_SUBTYPE_BINARY, (const uint8_t *)pData, 
		                     (uint32_t)cbSize))
	{
		return Status_OperationFailed;
	}
	m_stackStates.top().cCount++;

	return Status();
}

Status BSONWriter::WriteCustom(ICustom::Type nType, const void *pData, const Char *szName/* = NULL*/)
{
	if (ICustom::Type_BSON != nType)
	{
		return Status_NotSupported;
	}

	BSONReader::CustomData *pCustomData = (BSONReader::CustomData *)pData;

	const char  *pszName = "";
	static char szTmpName[40];

	if (NULL == m_pBSON)
	{
		return Status_InvalidCall;
	}
	if (m_stackStates.empty())
	{
		return Status_InvalidCall;
	}
	else
	if (State_Object == m_stackStates.top().nState)
	{
		if (NULL == szName)
		{
			return Status_InvalidArg;
		}
		pszName = szName;
	}
	else
	if (State_Array == m_stackStates.top().nState)
	{
		if (NULL != szName)
		{
			return Status_InvalidArg;
		}
		UInt64ToString(m_stackStates.top().cCount, szTmpName);
		pszName = szTmpName;
	}
	else
	{
		return Status_InvalidCall;
	}

	bson_t bson;

	if (!bson_init_static(&bson, (const uint8_t *)pCustomData->pData, pCustomData->cbSize))
	{
		return Status_OperationFailed;
	}
	if (!bson_append_document(m_stackStates.top().pBSON, pszName, -1, &bson))
	{
		return Status_OperationFailed;
	}

	return Status();
}

Status BSONWriter::BeginObject(const Char *szName/* = NULL*/)
{
	const char  *pszName = "";
	static char szTmpName[40];
	StateData   state(State_Object);
	Status      status;

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
		state.pBSON = m_pBSON;
	}
	else
	if (State_Object == m_stackStates.top().nState)
	{
		if (NULL == szName)
		{
			return Status_InvalidArg;
		}
		pszName = szName;
		state.pBSON = (bson_t *)malloc(sizeof(bson_t));
	}
	else
	if (State_Array == m_stackStates.top().nState)
	{
		if (NULL != szName)
		{
			return Status_InvalidArg;
		}
		UInt64ToString(m_stackStates.top().cCount, szTmpName);
		pszName = szTmpName;
		state.pBSON = (bson_t *)malloc(sizeof(bson_t));
	}
	else
	{
		return Status_InvalidCall;
	}
	if (state.pBSON != m_pBSON)
	{
		if (!bson_append_document_begin(m_stackStates.top().pBSON, pszName, -1, state.pBSON))
		{
			free(state.pBSON);

			return Status_OperationFailed;
		}
		m_stackStates.top().cCount++;
	}
	m_stackStates.push(state);

	return Status();
}

Status BSONWriter::EndObject()
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

	bson_t *pChild = m_stackStates.top().pBSON;
	m_stackStates.pop();

	if (!m_stackStates.empty())
	{
		bson_append_document_end(m_stackStates.top().pBSON, pChild);
		free(pChild);
	}

	return Status();
}

Status BSONWriter::BeginArray(const Char *szName/* = NULL*/)
{
	const char  *pszName;
	static char szTmpName[40];
	StateData   state(State_Array);
	Status      status;

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
			return Status_InvalidArg;
		}
		pszName = szName;
	}
	else
	if (State_Array == m_stackStates.top().nState)
	{
		if (NULL != szName)
		{
			return Status_InvalidArg;
		}
		//sprintf(szTmpName, "%zu", m_stackStates.top().cCount);
		UInt64ToString(m_stackStates.top().cCount, szTmpName);
		pszName = szTmpName;
	}
	else
	{
		return Status_InvalidCall;
	}
	state.pBSON = (bson_t *)malloc(sizeof(bson_t));
	if (!bson_append_array_begin(m_stackStates.top().pBSON, pszName, -1, state.pBSON))
	{
		free(state.pBSON);

		return Status_OperationFailed;
	}
	m_stackStates.top().cCount++;
	m_stackStates.push(state);

	return Status();
}

Status BSONWriter::EndArray()
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

	bson_t *pChild = m_stackStates.top().pBSON;
	m_stackStates.pop();

	if (m_stackStates.empty())
	{
		return Status_InvalidCall;
	}

	bson_append_array_end(m_stackStates.top().pBSON, pChild);
	free(pChild);

	return Status();
}

Status BSONWriter::PrepareWrite(const Char *szName, const Char **pszName)
{
	static char szTmpName[40];
		
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
			return Status_InvalidArg;
		}
		*pszName = szName;
	}
	else
	if (State_Array == m_stackStates.top().nState)
	{
		if (NULL != szName)
		{
			return Status_InvalidArg;
		}
		//sprintf(szTmpName, "%zu", m_stackStates.top().cCount);
		UInt64ToString(m_stackStates.top().cCount, szTmpName);
		*pszName = szTmpName;
	}
	else
	{
		return Status_InvalidCall;
	}

	return Status();
}

}//namespace SimpleBuffers

}//namespace CX
