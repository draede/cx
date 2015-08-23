
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
		}
	}

	return Status();
}

}//namespace IO

}//namespace CX


