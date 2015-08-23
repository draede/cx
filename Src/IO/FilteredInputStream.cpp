
#include "CX/IO/FilteredInputStream.hpp"
#include "CX/Limits.hpp"


namespace CX
{

namespace IO
{

FilteredInputStream::FilteredInputStream(IFilter *pFilter, IInputStream *pInputStream)
{
	m_pFilter              = pFilter;
	m_pInputStream         = pInputStream;
	m_pInBuffer            = Mem::Alloc(INPUT_BUFFER_SIZE);
	m_cbInBufferTotalSize  = INPUT_BUFFER_SIZE;
	m_cbBufferSize         = 0;
	m_buffers.pInBuffer    = m_pInBuffer;
	m_buffers.cbInSize     = m_cbBufferSize;
	m_buffers.pOutBuffer   = NULL;
	m_buffers.cbOutSize    = 0;
	m_buffers.bHasMoreData = true;
	m_buffers.nState       = IFilter::State_Continue;
}

FilteredInputStream::~FilteredInputStream()
{
	if (NULL != m_pInBuffer)
	{
		Mem::Free(m_pInBuffer);
		m_pInBuffer = NULL;
	}
}

Status FilteredInputStream::Read(void *pBuffer, Size cbReqSize, Size *pcbAckSize)
{
	if (NULL == m_pFilter || NULL == m_pInputStream || NULL == m_pInBuffer)
	{
		return Status_NotInitialized;
	}
	if ((Size)TYPE_UINT32_MAX < cbReqSize)
	{
		return Status_TooBig;
	}
	if (IFilter::State_Finish == m_buffers.nState)
	{
		return Status_EOF;
	}

	Size   cbAckSize;
	Status status;

	m_buffers.pOutBuffer = pBuffer;
	m_buffers.cbOutSize  = (UInt32)cbReqSize;
	while (0 < m_buffers.cbOutSize)
	{
		if (m_buffers.bHasMoreData && 0 == m_buffers.cbInSize)
		{
			if ((status = m_pInputStream->Read(m_pInBuffer, m_cbInBufferTotalSize, &cbAckSize)))
			{
				m_cbBufferSize      = (UInt32)cbAckSize;
				m_buffers.pInBuffer = m_pInBuffer;
				m_buffers.cbInSize  = m_cbBufferSize;
			}
			else
			{
				if (Status_EOF == (StatusCode)status)
				{
					m_buffers.bHasMoreData = false;
				}
				else
				{
					return status;
				}
			}
		}
		if (!(status = m_pFilter->Filter(&m_buffers)))
		{
			return status;
		}
		if (IFilter::State_Finish == m_buffers.nState)
		{
			break;
		}
	}
	*pcbAckSize = cbReqSize - m_buffers.cbOutSize;
	if (IFilter::State_Finish == m_buffers.nState)
	{
		return Status_EOF;
	}

	return Status();
}

Status FilteredInputStream::SetPos(UInt64 cbPos)
{
	CX_UNUSED(cbPos);

	if (NULL == m_pFilter || NULL == m_pInputStream || NULL == m_pInBuffer)
	{
		return Status_NotInitialized;
	}

	return Status_NotSupported;
}

Status FilteredInputStream::GetPos(UInt64 *pcbPos) const
{
	if (NULL == m_pFilter || NULL == m_pInputStream || NULL == m_pInBuffer)
	{
		return Status_NotInitialized;
	}

	return m_pInputStream->GetPos(pcbPos);
}

Status FilteredInputStream::GetSize(UInt64 *pcbSize) const
{
	if (NULL == m_pFilter || NULL == m_pInputStream || NULL == m_pInBuffer)
	{
		return Status_NotInitialized;
	}

	return m_pInputStream->GetSize(pcbSize);
}

Bool FilteredInputStream::IsEOF() const
{
	if (NULL == m_pFilter || NULL == m_pInputStream || NULL == m_pInBuffer)
	{
		return True;
	}

	return (IFilter::State_Finish == m_buffers.nState);
}

Bool FilteredInputStream::IsOK() const
{
	if (NULL == m_pFilter || NULL == m_pInputStream || NULL == m_pInBuffer)
	{
		return False;
	}

	return m_pInputStream->IsOK();
}

const Char *FilteredInputStream::GetPath() const
{
	if (NULL == m_pFilter || NULL == m_pInputStream || NULL == m_pInBuffer)
	{
		return "";
	}

	return m_pInputStream->GetPath();
}

}//namespace IO

}//namespace CX
