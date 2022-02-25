/*
 * https://github.com/draede/cx
 * 
 * Copyright (C) 2014 - 2022 draede - draede [at] outlook [dot] com
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


#include "CX/Types.hpp"
#include "CX/Status.hpp"


namespace CX
{

namespace Util
{

class CircularBuffer
{
public:

	CircularBuffer();

	~CircularBuffer();

	Status Init(Size cbSize);

	Status Uninit();

	Bool IsOK() const;

	Size GetSize() const;

	void *GetBuffer();

	const void *GetBuffer() const;

	Size GetReadSize() const;

	Size GetReadOffset() const;

	Byte *GetReadBuffer();

	const Byte *GetReadBuffer() const;

	Size GetWriteSize() const;

	Size GetWriteOffset() const;

	Byte *GetWriteBuffer();

	const Byte *GetWriteBuffer() const;

	Status Read(void *pData, Size cbReqSize, Size *pcbAckSize);

	Status Read(Size cbReqSize, Size *pcbAckSize);

	Status Write(const void *pData, Size cbReqSize, Size *pcbAckSize);

	Status Write(Size cbReqSize, Size *pcbAckSize);

private:

	Byte *m_pBuffer;
	Size m_cbSize;
	Size m_cbReadOffset;
	Size m_cbReadSize;
	Size m_cbWriteOffset;
	Size m_cbWriteSize;

};

}//namespace Util

}//namespace CX

