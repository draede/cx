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

#include "CX/SB/StatsDataWriter.hpp"


namespace CX
{

namespace SB
{

StatsDataWriter::StatsDataWriter(IDataWriter *pDataWriter)
{
	m_pDataWriter = pDataWriter;
	m_data.Reset();
}

StatsDataWriter::~StatsDataWriter()
{
}

Status StatsDataWriter::BeginMember(const Char *szName)
{
	m_data.m_cMembers++;
	m_data.m_cbTotalSize += cx_strlen(szName);

	return m_pDataWriter->BeginMember(szName);
}

Status StatsDataWriter::EndMember()
{
	return m_pDataWriter->EndMember();
}

Status StatsDataWriter::BeginItem()
{
	m_data.m_cItems++;

	return m_pDataWriter->BeginItem();
}

Status StatsDataWriter::EndItem()
{
	return m_pDataWriter->EndItem();
}

Status StatsDataWriter::BeginObject(Size cCount)
{
	m_data.m_cObjects++;
	m_data.m_cbTotalSize += sizeof(cCount);

	return m_pDataWriter->BeginObject(cCount);
}

Status StatsDataWriter::EndObject()
{
	return m_pDataWriter->EndObject();
}

Status StatsDataWriter::BeginArray(Size cCount)
{
	m_data.m_cArrays++;
	m_data.m_cbTotalSize += sizeof(cCount);

	return m_pDataWriter->BeginArray(cCount);
}

Status StatsDataWriter::EndArray()
{
	return m_pDataWriter->EndArray();
}

Status StatsDataWriter::WriteBool(Bool bValue)
{
	m_data.m_cBools++;
	m_data.m_cAllValues++;
	m_data.m_cbDataSize += sizeof(bValue);
	m_data.m_cbTotalSize += sizeof(bValue);

	return m_pDataWriter->WriteBool(bValue);
}

Status StatsDataWriter::WriteInt8(Int8 nValue)
{
	m_data.m_cInt8s++;
	m_data.m_cAllValues++;
	m_data.m_cbDataSize += sizeof(nValue);
	m_data.m_cbTotalSize += sizeof(nValue);

	return m_pDataWriter->WriteInt8(nValue);
}

Status StatsDataWriter::WriteUInt8(UInt8 uValue)
{
	m_data.m_cUInt8s++;
	m_data.m_cAllValues++;
	m_data.m_cbDataSize += sizeof(uValue);
	m_data.m_cbTotalSize += sizeof(uValue);

	return m_pDataWriter->WriteUInt8(uValue);
}

Status StatsDataWriter::WriteInt16(Int16 nValue)
{
	m_data.m_cInt16s++;
	m_data.m_cAllValues++;
	m_data.m_cbDataSize += sizeof(nValue);
	m_data.m_cbTotalSize += sizeof(nValue);

	return m_pDataWriter->WriteInt16(nValue);
}

Status StatsDataWriter::WriteUInt16(UInt16 uValue)
{
	m_data.m_cUInt16s++;
	m_data.m_cAllValues++;
	m_data.m_cbDataSize += sizeof(uValue);
	m_data.m_cbTotalSize += sizeof(uValue);

	return m_pDataWriter->WriteUInt16(uValue);
}

Status StatsDataWriter::WriteInt32(Int32 nValue)
{
	m_data.m_cInt32s++;
	m_data.m_cAllValues++;
	m_data.m_cbDataSize += sizeof(nValue);
	m_data.m_cbTotalSize += sizeof(nValue);

	return m_pDataWriter->WriteInt32(nValue);
}

Status StatsDataWriter::WriteUInt32(UInt32 uValue)
{
	m_data.m_cUInt32s++;
	m_data.m_cAllValues++;
	m_data.m_cbDataSize += sizeof(uValue);
	m_data.m_cbTotalSize += sizeof(uValue);

	return m_pDataWriter->WriteUInt32(uValue);
}

Status StatsDataWriter::WriteInt64(Int64 nValue)
{
	m_data.m_cInt64s++;
	m_data.m_cAllValues++;
	m_data.m_cbDataSize += sizeof(nValue);
	m_data.m_cbTotalSize += sizeof(nValue);

	return m_pDataWriter->WriteInt64(nValue);
}

Status StatsDataWriter::WriteUInt64(UInt64 uValue)
{
	m_data.m_cUInt64s++;
	m_data.m_cAllValues++;
	m_data.m_cbDataSize += sizeof(uValue);
	m_data.m_cbTotalSize += sizeof(uValue);

	return m_pDataWriter->WriteUInt64(uValue);
}

Status StatsDataWriter::WriteFloat(Float fValue)
{
	m_data.m_cFloats++;
	m_data.m_cAllValues++;
	m_data.m_cbDataSize += sizeof(fValue);
	m_data.m_cbTotalSize += sizeof(fValue);

	return m_pDataWriter->WriteFloat(fValue);
}

Status StatsDataWriter::WriteDouble(Double lfValue)
{
	m_data.m_cDoubles++;
	m_data.m_cAllValues++;
	m_data.m_cbDataSize += sizeof(lfValue);
	m_data.m_cbTotalSize += sizeof(lfValue);

	return m_pDataWriter->WriteDouble(lfValue);
}

Status StatsDataWriter::WriteString(const Char *szValue)
{
	Size cLen = cx_strlen(szValue);

	m_data.m_cStrings++;
	m_data.m_cAllValues++;
	m_data.m_cbDataSize += sizeof(Char) * cLen + sizeof(UInt32);
	m_data.m_cbTotalSize += sizeof(Char) * cLen + sizeof(UInt32);

	return m_pDataWriter->WriteString(szValue);
}

Status StatsDataWriter::WriteWString(const WChar *wszValue)
{
	Size cLen = cxw_strlen(wszValue);

	m_data.m_cStrings++;
	m_data.m_cAllValues++;
	m_data.m_cbDataSize += sizeof(WChar) * cLen + sizeof(UInt32);
	m_data.m_cbTotalSize += sizeof(WChar) * cLen + sizeof(UInt32);

	return m_pDataWriter->WriteWString(wszValue);
}

}//namespace SB

}//namespace CX

