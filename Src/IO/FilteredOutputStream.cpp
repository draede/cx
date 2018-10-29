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
#include "CX/IO/FilteredOutputStream.hpp"
#include "CX/Limits.hpp"
#include "CX/Mem.hpp"


namespace CX
{

namespace IO
{

FilteredOutputStream::FilteredOutputStream(IOutputFilter *pFilter, IO::IOutputStream *pOutputStream, 
                                           bool bTakeStreamOwnership/* = false*/)
{
	m_cbBufferSize = (BUFFER_SIZE / pFilter->GetBlockSize()) * pFilter->GetBlockSize();
	if (0 == m_cbBufferSize)
	{
		m_cbBufferSize = pFilter->GetBlockSize();
	}
	m_pFilter              = pFilter;
	m_pOutputStream        = pOutputStream;
	m_bTakeStreamOwnership = bTakeStreamOwnership;
	m_pBuffer              = (Byte *)Mem::Alloc(m_cbBufferSize);
	m_cbBufferOffset       = 0;
	m_cbReceivedSize       = 0;
	m_bReady               = false;
}

FilteredOutputStream::~FilteredOutputStream()
{
	if (!m_bReady)
	{
		if (NULL != m_pFilter)
		{
			Flush();
		}
	}
	if (NULL != m_pBuffer)
	{
		Mem::Free(m_pBuffer);
	}
	if (NULL != m_pFilter)
	{
		delete m_pFilter;
	}
	if (m_bTakeStreamOwnership && NULL != m_pOutputStream)
	{
		delete m_pOutputStream;
	}
}

Status FilteredOutputStream::Filter(const void *pInput, Size cbInputSize)
{
	UInt32 nTmp;
	Size   cbSize;
	void   *pOutput;
	Size   cbOutputSize;
	Status status;

	if ((Size)TYPE_INT32_MAX < cbInputSize)
	{
		return Status_TooBig;
	}
	if (!(status = m_pFilter->Filter(pInput, cbInputSize, &pOutput, &cbOutputSize)))
	{
		return status;
	}
	if ((Size)TYPE_INT32_MAX < cbOutputSize)
	{
		return Status_TooBig;
	}
	nTmp = (UInt32)cbInputSize;
	if (!(status = m_pOutputStream->Write(&nTmp, sizeof(nTmp), &cbSize)))
	{
		return status;
	}
	if (sizeof(nTmp) != cbSize)
	{
		return Status_WriteFailed;
	}
	nTmp = (UInt32)cbOutputSize;
	if (!(status = m_pOutputStream->Write(&nTmp, sizeof(nTmp), &cbSize)))
	{
		return status;
	}
	if (sizeof(nTmp) != cbSize)
	{
		return Status_WriteFailed;
	}
	if (!(status = m_pOutputStream->Write(pOutput, cbOutputSize, &cbSize)))
	{
		return status;
	}
	if (cbOutputSize != cbSize)
	{
		return Status_WriteFailed;
	}
	m_cbReceivedSize += cbInputSize;

	return Status();
}

Status FilteredOutputStream::Write(const void *pBuffer, Size cbReqSize, Size *pcbAckSize)
{
	if (NULL == m_pOutputStream || NULL == m_pBuffer)
	{
		return Status_NotInitialized;
	}

	if (m_bReady)
	{
		return Status_InvalidCall;
	}

	const Byte *pInput;
	Size       cbSize;
	Status     status;

	pInput      = (const Byte *)pBuffer;
	*pcbAckSize = 0;
	while (0 < cbReqSize)
	{
		cbSize = m_cbBufferSize - m_cbBufferOffset;
		if (cbSize > cbReqSize)
		{
			cbSize = cbReqSize;
		}
		if (0 < cbSize)
		{
			memcpy(m_pBuffer + m_cbBufferOffset, pInput, cbSize);
			m_cbBufferOffset += cbSize;
			pInput += cbSize;
			cbReqSize -= cbSize;
			*pcbAckSize += cbSize;
		}
		if (m_cbBufferOffset == m_cbBufferSize)
		{
			if (!(status = Filter(m_pBuffer, m_cbBufferSize)))
			{
				return status;
			}
			m_cbBufferOffset = 0;
		}
	}

	return Status();
}

Status FilteredOutputStream::GetSize(UInt64 *pcbSize) const
{
	if (NULL == m_pOutputStream || NULL == m_pBuffer)
	{
		return Status_NotInitialized;
	}
	*pcbSize = m_cbReceivedSize;

	return Status();
}

Bool FilteredOutputStream::IsOK() const
{
	return (NULL != m_pOutputStream && NULL != m_pBuffer);
}

const Char *FilteredOutputStream::GetPath() const
{
	if (NULL == m_pOutputStream || NULL == m_pBuffer)
	{
		return "";
	}

	return m_pOutputStream->GetPath();
}

Status FilteredOutputStream::Flush()
{
	if (NULL == m_pOutputStream || NULL == m_pBuffer)
	{
		return Status_NotInitialized;
	}
	if (m_bReady)
	{
		return Status_InvalidCall;
	}
	m_bReady = true;

	if (0 < m_cbBufferOffset)
	{
		Status status;

		if (!(status = Filter(m_pBuffer, m_cbBufferOffset)))
		{
			return status;
		}
		m_cbBufferOffset = 0;
	}
	
	return Status();
}

}//namespace IO

}//namespace CX
