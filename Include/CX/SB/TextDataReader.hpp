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

#pragma once


#include "CX/SB/IDataReader.hpp"
#include "CX/SB/Readers.hpp"
#include "CX/IO/IInputStream.hpp"
#include "CX/APIDefs.hpp"


namespace CX
{

namespace SB
{

class CX_API TextDataReader : public IDataReader
{
public:

	TextDataReader(IO::IInputStream *pInputStream);

	~TextDataReader();

	virtual Status BeginMember(const Char *szName);

	virtual Status EndMember();

	virtual Status BeginItem();

	virtual Status EndItem();

	virtual Status BeginObject(Size *pcCount);

	virtual Status EndObject();

	virtual Status BeginArray(Size *pcCount);

	virtual Status EndArray();

	virtual Status ReadBool(Bool *pbValue);

	virtual Status ReadInt8(Int8 *pnValue);

	virtual Status ReadUInt8(UInt8 *puValue);

	virtual Status ReadInt16(Int16 *pnValue);

	virtual Status ReadUInt16(UInt16 *puValue);

	virtual Status ReadInt32(Int32 *pnValue);

	virtual Status ReadUInt32(UInt32 *puValue);

	virtual Status ReadInt64(Int64 *pnValue);

	virtual Status ReadUInt64(UInt64 *puValue);

	virtual Status ReadFloat(Float *pfValue);

	virtual Status ReadDouble(Double *plfValue);

	virtual Status ReadString(String *psValue);

	virtual Status ReadWString(WString *pwsValue);

private:


	void   *m_pHelper;

};

}//namespace SB

}//namespace CX

