/* 
 * CX - C++ framework for general purpose development
 *
 * https://github.com/draede/cx
 * 
 * Copyright (C) 2014 - 2021 draede - draede [at] outlook [dot] com
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


#include "CX/IO/IInputStream.hpp"
#include "unzip.h"


namespace CX
{

namespace IO
{

namespace VFS
{

class ZIPInputStream : public IInputStream
{
public:

	//path?<pwd>
	ZIPInputStream(const Char *szZIPPath, const Char *szFileInZIPPath);

	~ZIPInputStream();

	virtual Status Read(void *pBuffer, Size cbReqSize, Size *pcbAckSize);

	virtual Status SetPos(UInt64 cbPos);

	virtual Status GetPos(UInt64 *pcbPos) const;

	virtual Status GetSize(UInt64 *pcbSize) const;

	virtual Bool IsEOF() const;

	virtual Bool IsOK() const;

	virtual const Char *GetPath() const;

private:

	void   *m_pZIP;
#pragma warning(push)
#pragma warning(disable: 4251)
	String   m_sPath;
#pragma warning(pop)
	UInt64   m_cbSize;
	UInt64   m_cbOffset;

	void Close();

};

}//namespace VFS

}//namespace IO

}//namespace CX
