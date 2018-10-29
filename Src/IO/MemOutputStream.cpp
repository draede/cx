/* 
 * CX - C++ framework for general purpose development
 *
 * https://github.com/draede/cx
 * 
 * Copyright (C) 2014 - 2018 draede, draede [at] outlook [dot] com
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

#include "CX/precomp.hpp"
#include "CX/IO/MemOutputStream.hpp"
#include "CX/Status.hpp"
#include "CX/C/stdio.h"


namespace CX
{

namespace IO
{

MemOutputStream::MemOutputStream(void *pMem, Size cbSize)
{
	m_nType     = Type_Mem;
	m_pMem      = pMem;
	m_cbMemSize = cbSize;
	m_cbOffset  = 0;
}

MemOutputStream::MemOutputStream(String *pStr)
{
	m_nType     = Type_String;
	m_pStr      = pStr;
	m_cbOffset  = 0;
}

MemOutputStream::MemOutputStream(Util::IMemPool *pMemPool)
{
	m_nType     = Type_MemPool;
	m_pMemPool  = pMemPool;
	m_cbOffset  = 0;
}

MemOutputStream::~MemOutputStream()
{
}

Size MemOutputStream::GetSizeImpl() const
{
	switch(m_nType)
	{
		case Type_Mem:       return m_cbMemSize;
		case Type_String:    return m_pStr->size();
		case Type_MemPool:   return m_cbOffset;
	}

	return 0;
}

Status MemOutputStream::WriteImpl(const void *pMem, Size cbReqSize, Size *pcbAckSize)
{
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
				return Status(Status_WriteFailed, "Max capacity reached");
			}
		}
		break;
		case Type_String:
		{
			m_pStr->append((const Char *)pMem, cbReqSize);
			*pcbAckSize = cbReqSize;
			m_cbOffset += cbReqSize;

			return Status_OK;
		}
		break;
		case Type_MemPool:
		{
			Status status;

			if (m_cbOffset + cbReqSize > m_pMemPool->GetSize())
			{
				if (!(status = m_pMemPool->SetSize(m_cbOffset + cbReqSize)))
				{
					return status;
				}
			}
			memcpy((Byte *)m_pMemPool->GetMem() + m_cbOffset, pMem, cbReqSize);
			*pcbAckSize = cbReqSize;
			m_cbOffset += cbReqSize;

			return status;
		}
		break;
	}

	return Status(Status_NotSupported, "Not supported");
}

Status MemOutputStream::Write(const void *pBuffer, Size cbReqSize, Size *pcbAckSize)
{
	return WriteImpl(pBuffer, cbReqSize, pcbAckSize);
}

Status MemOutputStream::GetSize(UInt64 *pcbSize) const
{
	*pcbSize = (UInt64)GetSizeImpl();

	return Status_OK;
}

Bool MemOutputStream::IsOK() const
{
	return True;
}

const Char *MemOutputStream::GetPath() const
{
	return "";
}

}//namespace IO

}//namespace CX

