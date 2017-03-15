/*
 * https://github.com/draede/cx
 * 
 * Copyright (C) 2014 - 2017 draede, draede [at] outlook [dot] com
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

#include "CX/Util/CircularBuffer.hpp"
#include "CX/Mem.hpp"


namespace CX
{

namespace Util
{

CircularBuffer::CircularBuffer()
{
	m_pBuffer       = NULL;
	m_cbSize        = 0;
	m_cbReadOffset  = 0;
	m_cbReadSize    = 0;
	m_cbWriteOffset = 0;
	m_cbWriteSize   = 0;
}

CircularBuffer::~CircularBuffer()
{
	Uninit();
}

Status CircularBuffer::Init(Size cbSize)
{
	Uninit();

	if (NULL == (m_pBuffer = (Byte *)Mem::Alloc(cbSize)))
	{
		return Status_MemAllocFailed;
	}
	m_cbSize        = cbSize;
	m_cbReadOffset  = 0;
	m_cbReadSize    = 0;
	m_cbWriteOffset = 0;
	m_cbWriteSize   = cbSize;

	return Status();
}

Status CircularBuffer::Uninit()
{
	if (NULL != m_pBuffer)
	{
		Mem::Free(m_pBuffer);
		m_pBuffer       = NULL;
		m_cbSize        = 0;
		m_cbReadOffset  = 0;
		m_cbReadSize    = 0;
		m_cbWriteOffset = 0;
		m_cbWriteSize   = 0;
	}

	return Status();
}

Bool CircularBuffer::IsOK() const
{
	return (NULL != m_pBuffer);
}

Size CircularBuffer::GetSize() const
{
	return m_cbSize;
}

void *CircularBuffer::GetBuffer()
{
	return m_pBuffer;
}

const void *CircularBuffer::GetBuffer() const
{
	return m_pBuffer;
}

Size CircularBuffer::GetReadSize() const
{
	return m_cbReadSize;
}

Size CircularBuffer::GetReadOffset() const
{
	return m_cbReadOffset;
}

Byte *CircularBuffer::GetReadBuffer()
{
	if (NULL == m_pBuffer)
	{
		return NULL;
	}
	if (0 == m_cbReadSize)
	{
		return NULL;
	}

	return m_pBuffer + m_cbReadOffset;
}

const Byte *CircularBuffer::GetReadBuffer() const
{
	if (NULL == m_pBuffer)
	{
		return NULL;
	}
	if (0 == m_cbReadSize)
	{
		return NULL;
	}

	return m_pBuffer + m_cbReadOffset;
}

Size CircularBuffer::GetWriteSize() const
{
	return m_cbWriteSize;
}

Size CircularBuffer::GetWriteOffset() const
{
	return m_cbWriteOffset;
}

Byte *CircularBuffer::GetWriteBuffer()
{
	if (NULL == m_pBuffer)
	{
		return NULL;
	}
	if (0 == m_cbWriteSize)
	{
		return NULL;
	}

	return m_pBuffer + m_cbWriteOffset;
}

const Byte *CircularBuffer::GetWriteBuffer() const
{
	if (NULL == m_pBuffer)
	{
		return NULL;
	}
	if (0 == m_cbWriteSize)
	{
		return NULL;
	}

	return m_pBuffer + m_cbWriteOffset;
}

Status CircularBuffer::Read(void *pData, Size cbReqSize, Size *pcbAckSize)
{
	if (NULL == m_pBuffer)
	{
		return Status_NotInitialized;
	}
	if (0 == m_cbReadSize)
	{
		*pcbAckSize = 0;

		return Status_OutOfBounds;
	}
	if (cbReqSize > m_cbReadSize)
	{
		*pcbAckSize = m_cbReadSize;
	}
	else
	{
		*pcbAckSize = cbReqSize;
	}

	Byte *pInput  = m_pBuffer + m_cbReadOffset;
	Byte *pOutput = (Byte *)pData;

	for (Size i = 0; i < *pcbAckSize; i++)
	{
		*pOutput = *pInput;
		pOutput++;
		pInput++;
		m_cbReadOffset++;
		m_cbReadSize--;
		m_cbWriteSize++;
		if (m_cbReadOffset >= m_cbSize)
		{
			m_cbReadOffset -= m_cbSize;
		}
	}

	return Status();
}

Status CircularBuffer::Read(Size cbReqSize, Size *pcbAckSize)
{
	if (NULL == m_pBuffer)
	{
		return Status_NotInitialized;
	}
	if (0 == m_cbReadSize)
	{
		*pcbAckSize = 0;

		return Status_OutOfBounds;
	}
	if (cbReqSize > m_cbReadSize)
	{
		*pcbAckSize = m_cbReadSize;
	}
	else
	{
		*pcbAckSize = cbReqSize;
	}

	for (Size i = 0; i < *pcbAckSize; i++)
	{
		m_cbReadOffset++;
		m_cbReadSize--;
		m_cbWriteSize++;
		if (m_cbReadOffset >= m_cbSize)
		{
			m_cbReadOffset -= m_cbSize;
		}
	}

	return Status();
}

Status CircularBuffer::Write(const void *pData, Size cbReqSize, Size *pcbAckSize)
{
	if (NULL == m_pBuffer)
	{
		return Status_NotInitialized;
	}
	if (0 == m_cbWriteSize)
	{
		*pcbAckSize = 0;

		return Status_OutOfBounds;
	}
	if (cbReqSize > m_cbWriteSize)
	{
		*pcbAckSize = m_cbWriteSize;
	}
	else
	{
		*pcbAckSize = cbReqSize;
	}

	const Byte *pInput  = (const Byte *)pData;
	Byte       *pOutput = m_pBuffer + m_cbWriteOffset;

	for (Size i = 0; i < *pcbAckSize; i++)
	{
		*pOutput = *pInput;
		pOutput++;
		pInput++;
		m_cbWriteOffset++;
		m_cbWriteSize--;
		m_cbReadSize++;
		if (m_cbWriteOffset >= m_cbSize)
		{
			m_cbWriteOffset -= m_cbSize;
		}
	}

	return Status();
}

Status CircularBuffer::Write(Size cbReqSize, Size *pcbAckSize)
{
	if (NULL == m_pBuffer)
	{
		return Status_NotInitialized;
	}
	if (0 == m_cbWriteSize)
	{
		*pcbAckSize = 0;

		return Status_OutOfBounds;
	}
	if (cbReqSize > m_cbWriteSize)
	{
		*pcbAckSize = m_cbWriteSize;
	}
	else
	{
		*pcbAckSize = cbReqSize;
	}

	for (Size i = 0; i < *pcbAckSize; i++)
	{
		m_cbWriteOffset++;
		m_cbWriteSize--;
		m_cbReadSize++;
		if (m_cbWriteOffset >= m_cbSize)
		{
			m_cbWriteOffset -= m_cbSize;
		}
	}

	return Status();
}

}//namespace Util

}//namespace CX

