/* 
 * CX - C++ framework for general purpose developement
 *
 * https://github.com/draede/cx
 * 
 * Copyright (C) 2014 draede - draede [at] outlook [dot] com
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

#include "CX/IO/MemOutputStream.h"
#include "CX/Status.h"


namespace CX
{

namespace IO
{

MemOutputStream::MemOutputStream(void *pMem, Size cbSize)
{
	Status::Clear();
	m_nType     = Type_Mem;
	m_pMem      = pMem;
	m_cbMemSize = cbSize;
}

MemOutputStream::MemOutputStream(String *pStr)
{
	Status::Clear();
	m_nType     = Type_String;
	m_pStr      = pStr;
}

MemOutputStream::MemOutputStream(Util::IMemPool *pMemPool)
{
	Status::Clear();
	m_nType     = Type_MemPool;
	m_pMemPool  = pMemPool;
}

MemOutputStream::~MemOutputStream()
{
	Status::Clear();
}

Size MemOutputStream::GetSizeImpl() const
{
	Status::Clear();

	switch(m_nType)
	{
		case Type_Mem:       return m_cbMemSize;
		case Type_String:    return m_pStr->size();
		case Type_MemPool:   return m_pMemPool->GetSize();
	}

	return 0;
}

StatusCode MemOutputStream::WriteImpl(const void *pMem, Size cbReqSize, Size *pcbAckSize)
{
	Status::Clear();

	switch(m_nType)
	{
		case Type_Mem:
		{
			if (m_cbOffset + cbReqSize > m_cbMemSize)
			{
				*pcbAckSize = m_cbMemSize - m_cbOffset;
			}
			else
			{
				*pcbAckSize = cbReqSize;
			}
			if (0 < *pcbAckSize)
			{
				memcpy((Byte *)m_pMem + m_cbOffset, pMem, *pcbAckSize);
				m_cbOffset += *pcbAckSize;

				return Status_OK;
			}
			else
			{
				return Status::Set(Status_WriteFailed, "Max capacity reached");
			}
		}
		break;
		case Type_String:
		{
			m_pStr->append((const Char *)pMem, cbReqSize);
			*pcbAckSize = cbReqSize;

			return Status_OK;
		}
		break;
		case Type_MemPool:
		{
			if (CXOK(m_pMemPool->Add(pMem, cbReqSize)))
			{
				*pcbAckSize = cbReqSize;

				return Status_OK;
			}
			else
			{
				return Status::GetCode();
			}
		}
		break;
	}

	return Status::Set(Status_NotSupported, "Not supported");
}

StatusCode MemOutputStream::Write(const void *pBuffer, Size cbReqSize, Size *pcbAckSize)
{
	Status::Clear();

	return WriteImpl(pBuffer, cbReqSize, pcbAckSize);
}

StatusCode MemOutputStream::GetSize(UInt64 *pcbSize) const
{
	Status::Clear();

	*pcbSize = (UInt64)GetSizeImpl();

	return Status_OK;
}

bool MemOutputStream::IsOK() const
{
	Status::Clear();

	return true;
}

const Char *MemOutputStream::GetPath() const
{
	Status::Clear();

	return "";
}

}//namespace IO

}//namespace CX

