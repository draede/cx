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
#include "CX/IO/BufferedOutputStream.hpp"
#include "CX/Mem.hpp"


namespace CX
{

namespace IO
{

BufferedOutputStream::BufferedOutputStream(IO::IOutputStream *pOutputStream, bool bTakeStreamOwnership/* = false*/, 
                                           Size cbBufferSize/* = BUFFER_SIZE*/)
{
	m_pOutputStream        = pOutputStream;
	m_bTakeStreamOwnership = bTakeStreamOwnership;
	m_pBuffer              = (Byte *)Mem::Alloc(cbBufferSize);
	m_cbSize               = 0;
	m_cbTotalSize          = cbBufferSize;
	m_cbPos                = 0;
}

BufferedOutputStream::~BufferedOutputStream()
{
	Flush();
	if (NULL != m_pBuffer)
	{
		Mem::Free(m_pBuffer);
	}
	if (m_bTakeStreamOwnership && NULL != m_pOutputStream)
	{
		delete m_pOutputStream;
	}
}

Status BufferedOutputStream::Write(const void *pBuffer, Size cbReqSize, Size *pcbAckSize)
{
	if (NULL == m_pBuffer || NULL == m_pOutputStream)
	{
		return Status_NotInitialized;
	}

	const Byte *pInput = (const Byte *)pBuffer;
	Size       cbSize;
	Status     status;

	*pcbAckSize = 0;
	while (0 < cbReqSize)
	{
		cbSize = m_cbTotalSize - m_cbSize;
		if (cbReqSize < cbSize)
		{
			cbSize = cbReqSize;
		}
		if (0 < cbSize)
		{
			memcpy(m_pBuffer + m_cbSize, pInput, cbSize);
			m_cbSize += cbSize;
			cbReqSize -= cbSize;
			pInput += cbSize;
			*pcbAckSize += cbSize;
		}
		else
		{
			while (0 < m_cbSize)
			{
				if ((status = m_pOutputStream->Write(m_pBuffer, m_cbSize, &cbSize)).IsNOK())
				{
					return status;
				}
				m_cbSize -= cbSize;
			}
		}
	}

	return Status();
}

Status BufferedOutputStream::GetSize(UInt64 *pcbSize) const
{
	if (NULL == m_pBuffer || NULL == m_pOutputStream)
	{
		return Status_NotInitialized;
	}

	*pcbSize = m_cbPos;

	return Status();
}

Bool BufferedOutputStream::IsOK() const
{
	return (NULL == m_pBuffer || NULL == m_pOutputStream);
}

const Char *BufferedOutputStream::GetPath() const
{
	if (NULL == m_pBuffer || NULL == m_pOutputStream)
	{
		return "";
	}

	return m_pOutputStream->GetPath();
}

Status BufferedOutputStream::Flush()
{
	if (NULL == m_pBuffer || NULL == m_pOutputStream)
	{
		return Status_NotInitialized;
	}
	if (0 == m_cbSize)
	{
		return Status();
	}

	Byte   *pBuffer = m_pBuffer;
	Size   cbAckSize;
	Status status;

	while (0 < m_cbSize)
	{
		if ((status = m_pOutputStream->Write(pBuffer, m_cbSize, &cbAckSize)).IsNOK())
		{
			return status;
		}
		pBuffer += cbAckSize;
		m_cbSize -= cbAckSize;
	}

	return Status();
}

}//namespace IO

}//namespace CX
