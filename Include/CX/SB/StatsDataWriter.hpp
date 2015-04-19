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


#include "CX/SB/IDataWriter.hpp"
#include "CX/SB/StatsData.hpp"
#include "CX/APIDefs.hpp"


namespace CX
{

namespace SB
{

class CX_API StatsDataWriter : public IDataWriter
{
public:

	StatsData m_data;

	StatsDataWriter(IDataWriter *pDataWriter);

	~StatsDataWriter();

	virtual Status BeginMember(const Char *szName);

	virtual Status EndMember();

	virtual Status BeginItem();

	virtual Status EndItem();

	virtual Status BeginObject(Size cCount);

	virtual Status EndObject();

	virtual Status BeginArray(Size cCount);

	virtual Status EndArray();

	virtual Status WriteBool(Bool bValue);

	virtual Status WriteInt8(Int8 nValue);

	virtual Status WriteUInt8(UInt8 uValue);

	virtual Status WriteInt16(Int16 nValue);

	virtual Status WriteUInt16(UInt16 uValue);

	virtual Status WriteInt32(Int32 nValue);

	virtual Status WriteUInt32(UInt32 uValue);

	virtual Status WriteInt64(Int64 nValue);

	virtual Status WriteUInt64(UInt64 uValue);

	virtual Status WriteFloat(Float fValue);

	virtual Status WriteDouble(Double lfValue);

	virtual Status WriteString(const Char *szValue);

	virtual Status WriteWString(const WChar *wszValue);

private:

	IDataWriter *m_pDataWriter;

};

}//namespace SB

}//namespace CX

