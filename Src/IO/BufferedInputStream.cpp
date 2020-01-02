/* 
 * CX - C++ framework for general purpose development
 *
 * https://github.com/draede/cx
 * 
 * Copyright (C) 2014 - 2020 draede - draede [at] outlook [dot] com
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
#include "CX/IO/BufferedInputStream.hpp"
#include "CX/Mem.hpp"


namespace CX
{

namespace IO
{

BufferedInputStream::BufferedInputStream(IO::IInputStream *pInputStream, bool bTakeStreamOwnership/* = false*/, 
                                         Size cbBufferSize/* = BUFFER_SIZE*/)
{
	m_pInputStream         = pInputStream;
	m_bTakeStreamOwnership = bTakeStreamOwnership;
	m_pBuffer              = (Byte *)Mem::Alloc(cbBufferSize);
	m_cbOffset             = 0;
	m_cbSize               = 0;
	m_cbTotalSize          = cbBufferSize;
	m_cbPos                = 0;
	m_bReady               = false;
}

BufferedInputStream::~BufferedInputStream()
{
	if (NULL != m_pBuffer)
	{
		Mem::Free(m_pBuffer);
	}
	if (m_bTakeStreamOwnership && NULL != m_pInputStream)
	{
		delete m_pInputStream;
	}
}

Status BufferedInputStream::Read(void *pBuffer, Size cbReqSize, Size *pcbAckSize)
{
	Byte   *pOutput = (Byte *)pBuffer;
	Size   cbSize;
	Status status;

	if (m_bReady)
	{
		return Status_EOF;
	}
	if (NULL == m_pBuffer || NULL == m_pInputStream)
	{
		return Status_NotInitialized;
	}
	*pcbAckSize = 0;
	while (0 < cbReqSize)
	{
		if (m_cbSize >= cbReqSize)
		{
			cbSize = cbReqSize;
		}
		else
		{
			cbSize = m_cbSize;
		}
		if (0 < cbSize)
		{
			memcpy(pOutput, m_pBuffer + m_cbOffset, cbSize);
			pOutput += cbSize;
			m_cbOffset += cbSize;
			m_cbSize -= cbSize;
			cbReqSize -= cbSize;
			*pcbAckSize += cbSize;
			m_cbPos += cbSize;
		}
		if (0 == m_cbSize && 0 < cbReqSize)
		{
			m_cbOffset = 0;
			if ((status = m_pInputStream->Read(m_pBuffer, m_cbTotalSize, &m_cbSize)).IsNOK())
			{
				if (Status_EOF == status.GetCode())
				{
					m_cbSize = 0;
					m_bReady = true;

					break;
				}
				else
				{
					m_cbSize = 0;

					return status;
				}
			}
		}
	}

	return Status();
}

Status BufferedInputStream::SetPos(UInt64 cbPos)
{
	CX_UNUSED(cbPos);

	if (NULL == m_pBuffer || NULL == m_pInputStream)
	{
		return Status_NotInitialized;
	}

	return Status_NotSupported;
}

Status BufferedInputStream::GetPos(UInt64 *pcbPos) const
{
	if (NULL == m_pBuffer || NULL == m_pInputStream)
	{
		return Status_NotInitialized;
	}

	*pcbPos = m_cbPos;

	return Status();
}

Status BufferedInputStream::GetSize(UInt64 *pcbSize) const
{
	if (NULL == m_pBuffer || NULL == m_pInputStream)
	{
		return Status_NotInitialized;
	}
	
	return m_pInputStream->GetSize(pcbSize);
}

Bool BufferedInputStream::IsEOF() const
{
	if (NULL == m_pBuffer || NULL == m_pInputStream)
	{
		return Status_NotInitialized;
	}

	return m_bReady;
}

Bool BufferedInputStream::IsOK() const
{
	return (NULL != m_pBuffer && NULL != m_pInputStream);
}

const Char *BufferedInputStream::GetPath() const
{
	if (NULL == m_pBuffer || NULL == m_pInputStream)
	{
		return "";
	}

	return m_pInputStream->GetPath();
}

}//namespace IO

}//namespace CX
