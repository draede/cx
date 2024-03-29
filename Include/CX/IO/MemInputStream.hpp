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

#pragma once


#include "CX/IO/IInputStream.hpp"
#include "CX/String.hpp"
#include "CX/Util/MemPool.hpp"
#include "CX/APIDefs.hpp"


namespace CX
{

namespace IO
{

class CX_API MemInputStream : public IInputStream
{
public:

	MemInputStream(const void *pMem, Size cbSize);

	MemInputStream(const String *pStr);

	MemInputStream(const Util::IMemPool *pMemPool, Size cbSize);

	~MemInputStream();

	virtual Status Read(void *pBuffer, Size cbReqSize, Size *pcbAckSize);

	virtual Status SetPos(UInt64 cbPos);

	virtual Status GetPos(UInt64 *pcbPos) const;

	virtual Status GetSize(UInt64 *pcbSize) const;

	virtual Bool IsEOF() const;

	virtual Bool IsOK() const;

	virtual const Char *GetPath() const;

private:

	enum Type
	{
		Type_Mem,
		Type_String,
		Type_MemPool,
	};

	Type                  m_nType;
	const void            *m_pMem;
	Size                  m_cbSize;
	const String          *m_pStr;
	const Util::IMemPool  *m_pMemPool;
	Size                  m_cbOffset;

	Size GetSizeImpl() const;

	const void *GetMemImpl(Size cbOffset) const;

};

}//namespace IO

}//namespace CX

