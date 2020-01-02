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
#include "CX/IO/FilteredInputStream.hpp"
#include "aes.h"


namespace CX
{

namespace IO
{

FilteredInputStream::FilteredInputStream(IInputFilter *pFilter, IO::IInputStream *pInputStream, 
                                         bool bTakeStreamOwnership/* = false*/)
{
	m_pFilter              = pFilter;
	m_pInputStream         = pInputStream;
	m_bTakeStreamOwnership = bTakeStreamOwnership;
	m_cbReceivedSize       = 0;
	m_bReady               = false;
	m_pOutput              = NULL;
	m_cbOutputSize         = 0;
}

FilteredInputStream::~FilteredInputStream()
{
	if (NULL != m_pFilter)
	{
		delete m_pFilter;
	}
	if (m_bTakeStreamOwnership && NULL != m_pInputStream)
	{
		delete m_pInputStream;
	}
}

Status FilteredInputStream::Read(void *pBuffer, Size cbReqSize, Size *pcbAckSize)
{
	if (m_bReady)
	{
		return Status_EOF;
	}
	if (NULL == m_pFilter || NULL == m_pInputStream)
	{
		return Status();
	}

	UInt32 cbSrcSize;
	UInt32 cbFltSize;
	Size   cbSize;
	Byte   *pOut;
	Status status;

	*pcbAckSize = 0;
	pOut        = (Byte *)pBuffer;
	while (0 < cbReqSize)
	{
		if (0 < m_cbOutputSize)
		{
			if (m_cbOutputSize >= cbReqSize)
			{
				cbSize = cbReqSize;
			}
			else
			{
				cbSize = m_cbOutputSize;
			}
			memcpy(pOut, m_pOutput, cbSize);
			pOut += cbSize;
			(*pcbAckSize) += cbSize;
			cbReqSize -= cbSize;
			m_pOutput += cbSize;
			m_cbOutputSize -= cbSize;
			m_cbReceivedSize += cbSize;
		}
		if (0 == cbReqSize)
		{
			break;
		}

		//read next 
		if (!(status = m_pInputStream->Read(&cbSrcSize, sizeof(cbSrcSize), &cbSize)))
		{
			if (Status_EOF == (StatusCode)status)
			{
				m_bReady = true;
			}

			return status;
		}
		if (sizeof(cbSrcSize) != cbSize)
		{
			return Status_ReadFailed;
		}
		if (!(status = m_pInputStream->Read(&cbFltSize, sizeof(cbFltSize), &cbSize)))
		{
			if (Status_EOF == (StatusCode)status)
			{
				return Status_ReadFailed;
			}

			return status;
		}
		if (sizeof(cbFltSize) != cbSize)
		{
			return Status_ReadFailed;
		}

		if (m_buffer.GetSize() < (Size)cbFltSize)
		{
			if (!(status = m_buffer.SetSize((Size)cbFltSize)))
			{
				return status;
			}
		}
		if (!(status = m_pInputStream->Read((Byte *)m_buffer.GetMem(), (Size)cbFltSize, &cbSize)))
		{
			if (Status_EOF != (StatusCode)status)
			{
				return status;
			}
		}
		if ((Size)cbFltSize != cbSize)
		{
			return Status_ReadFailed;
		}

		if (!(status = m_pFilter->Filter(m_buffer.GetMem(), (Size)cbFltSize, (Size)cbSrcSize, (void **)&m_pOutput, &m_cbOutputSize)))
		{
			return status;
		}
	}

	return Status();
}

Status FilteredInputStream::SetPos(UInt64 cbPos)
{
	CX_UNUSED(cbPos);

	if (NULL == m_pFilter || NULL == m_pInputStream)
	{
		return Status();
	}

	return Status_NotSupported;
}

Status FilteredInputStream::GetPos(UInt64 *pcbPos) const
{
	if (NULL == m_pFilter || NULL == m_pInputStream)
	{
		return Status();
	}
	*pcbPos = m_cbReceivedSize;

	return Status();
}

Status FilteredInputStream::GetSize(UInt64 *pcbSize) const
{
	if (NULL == m_pFilter || NULL == m_pInputStream)
	{
		return Status();
	}
	*pcbSize = m_cbReceivedSize;
	
	return Status();
}

Bool FilteredInputStream::IsEOF() const
{
	if (NULL == m_pFilter || NULL == m_pInputStream)
	{
		return Status();
	}
	if (0 < m_cbOutputSize)
	{
		return false;
	}

	return m_pInputStream->IsEOF();
}

Bool FilteredInputStream::IsOK() const
{
	return (NULL != m_pFilter && NULL != m_pInputStream);
}

const Char *FilteredInputStream::GetPath() const
{
	if (NULL == m_pFilter || NULL == m_pInputStream)
	{
		return "";
	}

	return m_pInputStream->GetPath();
}

}//namespace IO

}//namespace CX
