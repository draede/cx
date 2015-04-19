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

#include "CX/IO/MemInputStream.hpp"
#include "CX/Status.hpp"


namespace CX
{

namespace IO
{

MemInputStream::MemInputStream(const void *pMem, Size cbSize)
{
	m_nType     = Type_Mem;
	m_pMem      = pMem;
	m_cbMemSize = cbSize;
	m_cbOffset  = 0;
}

MemInputStream::MemInputStream(const String *pStr)
{
	m_nType     = Type_String;
	m_pStr      = pStr;
	m_cbOffset  = 0;
}

MemInputStream::MemInputStream(const Util::IMemPool *pMemPool)
{
	m_nType     = Type_MemPool;
	m_pMemPool  = pMemPool;
	m_cbOffset  = 0;
}

MemInputStream::~MemInputStream()
{
}

Size MemInputStream::GetSizeImpl() const
{
	switch(m_nType)
	{
		case Type_Mem:       return m_cbMemSize;
		case Type_String:    return m_pStr->size();
		case Type_MemPool:   return m_pMemPool->GetSize();
	}

	return 0;
}

const void *MemInputStream::GetMemImpl(Size cbOffset) const
{
	switch (m_nType)
	{
		case Type_Mem:       return (const Byte *)m_pMem + cbOffset;
		case Type_String:    return m_pStr->c_str() + cbOffset;
		case Type_MemPool:   return (const Byte *)m_pMemPool->GetMem() + cbOffset;
	}

	return 0;
}

Status MemInputStream::Read(void *pBuffer, Size cbReqSize, Size *pcbAckSize)
{
	if (cbReqSize > GetSizeImpl() - m_cbOffset)
	{
		*pcbAckSize = GetSizeImpl() - m_cbOffset;
	}
	else
	{
		*pcbAckSize = cbReqSize;
	}
	if (0 < *pcbAckSize)
	{
		memcpy(pBuffer, GetMemImpl(m_cbOffset), *pcbAckSize);
		m_cbOffset += *pcbAckSize;
	}
	else
	{
		return Status(Status_EOF, "End of stream reached");
	}

	return Status();
}

Status MemInputStream::SetPos(UInt64 cbPos)
{
	if (cbPos >= (UInt64)GetSizeImpl())
	{
		return Status(Status_OutOfBounds, "Invalid offset");
	}

	return Status();
}

Status MemInputStream::GetPos(UInt64 *pcbPos) const
{
	*pcbPos = m_cbOffset;

	return Status();
}

Status MemInputStream::GetSize(UInt64 *pcbSize) const
{
	*pcbSize = GetSizeImpl();

	return Status();
}

Bool MemInputStream::IsEOF() const
{
	return (m_cbOffset >= GetSizeImpl());
}

Bool MemInputStream::IsOK() const
{
	return True;
}

const Char *MemInputStream::GetPath() const
{
	return "";
}

}//namespace IO

}//namespace CX

