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
 
#include "CX/IO/FilteredOutputStream.hpp"
#include "CX/Limits.hpp"


namespace CX
{

namespace IO
{

FilteredOutputStream::FilteredOutputStream(IFilter *pFilter, IOutputStream *pOutputStream)
{
	m_pFilter              = pFilter;
	m_pOutputStream        = pOutputStream;
	m_pOutBuffer           = Mem::Alloc(OUTPUT_BUFFER_SIZE);
	m_cbOutBufferSize      = OUTPUT_BUFFER_SIZE;
	m_buffers.pInBuffer    = NULL;
	m_buffers.cbInSize     = 0;
	m_buffers.pOutBuffer   = m_pOutBuffer;
	m_buffers.cbOutSize    = m_cbOutBufferSize;
	m_buffers.bHasMoreData = true;
	m_buffers.nState       = IFilter::State_Continue;
}

FilteredOutputStream::~FilteredOutputStream()
{
	Flush();
	if (NULL != m_pOutBuffer)
	{
		Mem::Free(m_pOutBuffer);
		m_pOutBuffer = NULL;
	}
}

Status FilteredOutputStream::Write(const void *pBuffer, Size cbReqSize, Size *pcbAckSize)
{
	if (NULL == m_pFilter || NULL == m_pOutputStream || NULL == m_pOutBuffer)
	{
		return Status_NotInitialized;
	}
	if ((Size)TYPE_UINT32_MAX < cbReqSize)
	{
		return Status_TooBig;
	}

	Size   cbAckSize;
	Status status;

	m_buffers.pInBuffer    = pBuffer;
	m_buffers.cbInSize     = (UInt32)cbReqSize;
	m_buffers.bHasMoreData = true;
	while (0 < m_buffers.cbInSize && IFilter::State_Continue == m_buffers.nState)
	{
		if (!(status = m_pFilter->Filter(&m_buffers)))
		{
			return status;
		}
		if (0 == m_buffers.cbOutSize || (IFilter::State_Finish == m_buffers.nState && m_buffers.cbOutSize < m_cbOutBufferSize))
		{
			if (!(status = m_pOutputStream->Write(m_pOutBuffer, m_cbOutBufferSize - m_buffers.cbOutSize, &cbAckSize)))
			{
				return status;
			}
			if (cbAckSize != m_cbOutBufferSize - m_buffers.cbOutSize)
			{
				return Status_WriteFailed;
			}
			
			m_buffers.pOutBuffer = m_pOutBuffer;
			m_buffers.cbOutSize  = m_cbOutBufferSize;
		}
	}
	(*pcbAckSize) = cbReqSize;

	return Status();
}

Status FilteredOutputStream::GetSize(UInt64 *pcbSize) const
{
	if (NULL == m_pFilter || NULL == m_pOutputStream || NULL == m_pOutBuffer)
	{
		return Status_NotInitialized;
	}

	return m_pOutputStream->GetSize(pcbSize);
}

Bool FilteredOutputStream::IsOK() const
{
	if (NULL == m_pFilter || NULL == m_pOutputStream || NULL == m_pOutBuffer)
	{
		return False;
	}

	return m_pOutputStream->IsOK();
}

const Char *FilteredOutputStream::GetPath() const
{
	if (NULL == m_pFilter || NULL == m_pOutputStream || NULL == m_pOutBuffer)
	{
		return "";
	}

	return m_pOutputStream->GetPath();
}

Status FilteredOutputStream::Flush()
{
	if (NULL == m_pFilter || NULL == m_pOutputStream || NULL == m_pOutBuffer)
	{
		return Status_NotInitialized;
	}
	if (IFilter::State_Continue != m_buffers.nState)
	{
		return Status_InvalidArg;
	}

	Size   cbAckSize;
	Status status;

	m_buffers.bHasMoreData = false;
	while (IFilter::State_Continue == m_buffers.nState)
	{
		if (!(status = m_pFilter->Filter(&m_buffers)))
		{
			return status;
		}
		if (0 == m_buffers.cbOutSize || (IFilter::State_Finish == m_buffers.nState && m_buffers.cbOutSize < m_cbOutBufferSize))
		{
			if (!(status = m_pOutputStream->Write(m_pOutBuffer, m_cbOutBufferSize - m_buffers.cbOutSize, &cbAckSize)))
			{
				return status;
			}
			if (cbAckSize != m_cbOutBufferSize - m_buffers.cbOutSize)
			{
				return Status_WriteFailed;
			}
			m_buffers.pOutBuffer = m_pOutBuffer;
			m_buffers.cbOutSize  = m_cbOutBufferSize;
			m_pOutputStream->Flush();
		}
	}

	return Status();
}

}//namespace IO

}//namespace CX


