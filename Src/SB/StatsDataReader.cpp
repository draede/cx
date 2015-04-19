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

#include "CX/SB/StatsDataReader.hpp"


namespace CX
{

namespace SB
{

StatsDataReader::StatsDataReader(IDataReader *pDataReader)
{
	m_pDataReader= pDataReader;
	m_data.Reset();
}

StatsDataReader::~StatsDataReader()
{
}

Status StatsDataReader::BeginMember(const Char *szName)
{
	m_data.m_cMembers++;
	m_data.m_cbTotalSize += cx_strlen(szName);

	return m_pDataReader->BeginMember(szName);
}

Status StatsDataReader::EndMember()
{
	return m_pDataReader->EndMember();
}

Status StatsDataReader::BeginItem()
{
	m_data.m_cItems++;

	return m_pDataReader->BeginItem();
}

Status StatsDataReader::EndItem()
{
	return m_pDataReader->EndItem();
}

Status StatsDataReader::BeginObject(Size *pcCount)
{
	m_data.m_cObjects++;
	m_data.m_cbTotalSize += sizeof(*pcCount);

	return m_pDataReader->BeginObject(pcCount);
}

Status StatsDataReader::EndObject()
{
	return m_pDataReader->EndObject();
}

Status StatsDataReader::BeginArray(Size *pcCount)
{
	m_data.m_cArrays++;
	m_data.m_cbTotalSize += sizeof(*pcCount);

	return m_pDataReader->BeginArray(pcCount);
}

Status StatsDataReader::EndArray()
{
	return m_pDataReader->EndArray();
}

Status StatsDataReader::ReadBool(Bool *pbValue)
{
	m_data.m_cBools++;
	m_data.m_cAllValues++;
	m_data.m_cbDataSize += sizeof(*pbValue);
	m_data.m_cbTotalSize += sizeof(*pbValue);

	return m_pDataReader->ReadBool(pbValue);
}

Status StatsDataReader::ReadInt8(Int8 *pnValue)
{
	m_data.m_cInt8s++;
	m_data.m_cAllValues++;
	m_data.m_cbDataSize += sizeof(*pnValue);
	m_data.m_cbTotalSize += sizeof(*pnValue);

	return m_pDataReader->ReadInt8(pnValue);
}

Status StatsDataReader::ReadUInt8(UInt8 *puValue)
{
	m_data.m_cUInt8s++;
	m_data.m_cAllValues++;
	m_data.m_cbDataSize += sizeof(*puValue);
	m_data.m_cbTotalSize += sizeof(*puValue);

	return m_pDataReader->ReadUInt8(puValue);
}

Status StatsDataReader::ReadInt16(Int16 *pnValue)
{
	m_data.m_cInt16s++;
	m_data.m_cAllValues++;
	m_data.m_cbDataSize += sizeof(*pnValue);
	m_data.m_cbTotalSize += sizeof(*pnValue);

	return m_pDataReader->ReadInt16(pnValue);
}

Status StatsDataReader::ReadUInt16(UInt16 *puValue)
{
	m_data.m_cUInt16s++;
	m_data.m_cAllValues++;
	m_data.m_cbDataSize += sizeof(*puValue);
	m_data.m_cbTotalSize += sizeof(*puValue);

	return m_pDataReader->ReadUInt16(puValue);
}

Status StatsDataReader::ReadInt32(Int32 *pnValue)
{
	m_data.m_cInt32s++;
	m_data.m_cAllValues++;
	m_data.m_cbDataSize += sizeof(*pnValue);
	m_data.m_cbTotalSize += sizeof(*pnValue);

	return m_pDataReader->ReadInt32(pnValue);
}

Status StatsDataReader::ReadUInt32(UInt32 *puValue)
{
	m_data.m_cUInt32s++;
	m_data.m_cAllValues++;
	m_data.m_cbDataSize += sizeof(*puValue);
	m_data.m_cbTotalSize += sizeof(*puValue);

	return m_pDataReader->ReadUInt32(puValue);
}

Status StatsDataReader::ReadInt64(Int64 *pnValue)
{
	m_data.m_cInt64s++;
	m_data.m_cAllValues++;
	m_data.m_cbDataSize += sizeof(*pnValue);
	m_data.m_cbTotalSize += sizeof(*pnValue);

	return m_pDataReader->ReadInt64(pnValue);
}

Status StatsDataReader::ReadUInt64(UInt64 *puValue)
{
	m_data.m_cUInt64s++;
	m_data.m_cAllValues++;
	m_data.m_cbDataSize += sizeof(*puValue);
	m_data.m_cbTotalSize += sizeof(*puValue);

	return m_pDataReader->ReadUInt64(puValue);
}

Status StatsDataReader::ReadFloat(Float *pfValue)
{
	m_data.m_cFloats++;
	m_data.m_cAllValues++;
	m_data.m_cbDataSize += sizeof(*pfValue);
	m_data.m_cbTotalSize += sizeof(*pfValue);

	return m_pDataReader->ReadFloat(pfValue);
}

Status StatsDataReader::ReadDouble(Double *plfValue)
{
	m_data.m_cDoubles++;
	m_data.m_cAllValues++;
	m_data.m_cbDataSize += sizeof(*plfValue);
	m_data.m_cbTotalSize += sizeof(*plfValue);

	return m_pDataReader->ReadDouble(plfValue);
}

Status StatsDataReader::ReadString(String *psValue)
{
	Status status;

	if ((status = m_pDataReader->ReadString(psValue)).IsOK())
	{
		m_data.m_cStrings++;
		m_data.m_cAllValues++;
		m_data.m_cbDataSize += sizeof(Char) * psValue->size() + sizeof(UInt32);
		m_data.m_cbTotalSize += sizeof(Char) * psValue->size() + sizeof(UInt32);
	}

	return status;
}

Status StatsDataReader::ReadWString(WString *pwsValue)
{
	Status status;

	if ((status = m_pDataReader->ReadWString(pwsValue)).IsOK())
	{
		m_data.m_cStrings++;
		m_data.m_cAllValues++;
		m_data.m_cbDataSize += sizeof(WChar) * pwsValue->size() + sizeof(UInt32);
		m_data.m_cbTotalSize += sizeof(WChar) * pwsValue->size() + sizeof(UInt32);
	}

	return status;
}

}//namespace SB

}//namespace CX

