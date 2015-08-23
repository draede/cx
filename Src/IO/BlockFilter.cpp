
#include "CX/IO/BlockFilter.hpp"
#include "CX/Sys/ByteOrder.hpp"


namespace CX
{

namespace IO
{

BlockFilter::BlockFilter(Dir nDir, IBlockFilterHandler *pHandler)
{
	m_nDir                 = nDir;

	m_pHandler             = pHandler;

	if (Dir_Encode == nDir)
	{
		m_cbBlockSize       = pHandler->GetBlockSize();
	}
	else
	{
		m_cbBlockSize       = pHandler->GetBlockSize() * 2;
	}

	m_pInBuffer            = (Byte *)Mem::Alloc(m_cbBlockSize * 2);
	m_cbInBufferSize       = 0;

	m_pOutBuffer           = (Byte *)Mem::Alloc(m_cbBlockSize * 2);
	m_cbOutBufferSize      = 0;
	m_cbOutBufferOffset    = 0;

	m_cbReceivedBytes      = 0;
	m_cbSentBytes          = 0;

	m_nMode                = Mode_InOut;
}

BlockFilter::~BlockFilter()
{
	if (NULL != m_pInBuffer)
	{
		Mem::Free(m_pInBuffer);
	}
	if (NULL != m_pOutBuffer)
	{
		Mem::Free(m_pOutBuffer);
	}
}

Status BlockFilter::Filter(Buffers *pBuffers)
{
	if (sizeof(UInt32) > m_cbBlockSize || NULL == m_pHandler || NULL == m_pInBuffer || NULL == m_pOutBuffer)
	{
		return Status(Status_NotInitialized);
	}
	if (Mode_Error == m_nMode)
	{
		pBuffers->nState = State_Error;

		return Status(Status_InvalidCall);
	}
	if (Mode_Finished == m_nMode)
	{
		pBuffers->nState = State_Finish;

		return Status();
	}

	Size   cbSize;
	Size   cbSize2;
	Status status;

	for (;;)
	{
		if (Mode_InOut == m_nMode)
		{
			if (0 < pBuffers->cbInSize)
			{
				cbSize  = (pBuffers->cbInSize / m_cbBlockSize) * m_cbBlockSize;
				if (cbSize == pBuffers->cbInSize)
				{
					cbSize -= m_cbBlockSize;
				}
				cbSize2 = (pBuffers->cbOutSize / m_cbBlockSize) * m_cbBlockSize;
				if (cbSize > cbSize2)
				{
					cbSize = cbSize2;
				}
				if (0 < cbSize)
				{
					if (!(status = m_pHandler->OnBlocks(cbSize, pBuffers->pInBuffer, pBuffers->pOutBuffer)))
					{
						pBuffers->nState = State_Error;
						m_nMode          = Mode_Error;

						return status;
					}
					m_cbReceivedBytes += cbSize;
					m_cbSentBytes += cbSize;
					pBuffers->cbInSize -= (UInt32)cbSize;
					pBuffers->cbInTotalSize = (UInt32)m_cbReceivedBytes;
					pBuffers->pInBuffer = (const Byte *)pBuffers->pInBuffer + cbSize;
					pBuffers->cbOutSize -= (UInt32)cbSize;
					pBuffers->cbOutTotalSize +=(UInt32) m_cbSentBytes;
					pBuffers->pOutBuffer = (Byte *)pBuffers->pOutBuffer + cbSize;
				}
			}
			if (0 < pBuffers->cbInSize)
			{
				m_nMode            = Mode_In;
				m_cbInBufferSize   = 0;
			}
			else
			{
				if (!pBuffers->bHasMoreData)
				{
					m_nMode            = Mode_In;
					m_cbInBufferSize   = 0;
				}
				else
				{
					pBuffers->nState = State_Continue;
					
					return Status();
				}
			}
		}
		if (Mode_In == m_nMode)
		{
			if (m_cbInBufferSize < m_cbBlockSize)
			{
				cbSize = m_cbBlockSize - m_cbInBufferSize;
				if (cbSize > pBuffers->cbInSize)
				{
					cbSize = pBuffers->cbInSize;
				}
				if (0 < cbSize)
				{
					memcpy(m_pInBuffer + m_cbInBufferSize, pBuffers->pInBuffer, cbSize);
					m_cbInBufferSize += cbSize;
					m_cbReceivedBytes += cbSize;
					pBuffers->cbInSize -= (UInt32)cbSize;
					pBuffers->cbInTotalSize = (UInt32)m_cbReceivedBytes;
					pBuffers->pInBuffer = (const Byte *)pBuffers->pInBuffer + cbSize;
				}
			}
			if (0 == pBuffers->cbInSize)
			{
				if (pBuffers->bHasMoreData)
				{
					pBuffers->nState = State_Continue;

					return Status();
				}
				else
				{
					if (0 < m_cbInBufferSize)
					{
						if (Dir_Encode == m_nDir)
						{
							UInt32 cbLastBlockSize = (UInt32)(m_cbReceivedBytes % m_cbBlockSize);

							if (0 == cbLastBlockSize)
							{
								cbLastBlockSize = (UInt32)m_cbBlockSize;
							}
							for (Size i = cbLastBlockSize; i < m_cbBlockSize; i++)
							{
								*(m_pInBuffer + i) = 0;
							}
							cbLastBlockSize = Sys::ByteOrder::H2LE(cbLastBlockSize);
							memset(m_pInBuffer + m_cbBlockSize, 0, m_cbBlockSize);
							memcpy(m_pInBuffer + m_cbBlockSize, &cbLastBlockSize, sizeof(cbLastBlockSize));
							if (!(status = m_pHandler->OnBlocks(m_cbBlockSize * 2, m_pInBuffer, m_pOutBuffer)))
							{
								pBuffers->nState = State_Error;
								m_nMode          = Mode_Error;

								return status;
							}
							m_nMode             = Mode_Out;
							m_cbOutBufferOffset = 0;
							m_cbOutBufferSize   = m_cbBlockSize * 2;
						}
						else
						{
							if (m_cbBlockSize != m_cbInBufferSize)
							{
								pBuffers->nState = State_Error;
								m_nMode          = Mode_Error;

								return status;
							}

							if (!(status = m_pHandler->OnBlocks(m_cbBlockSize, m_pInBuffer, m_pOutBuffer)))
							{
								pBuffers->nState = State_Error;
								m_nMode          = Mode_Error;

								return status;
							}

							UInt32 cbLastBlockSize;

							memcpy(&cbLastBlockSize, m_pOutBuffer + m_cbBlockSize / 2, sizeof(UInt32));
							cbLastBlockSize = Sys::ByteOrder::LE2H(cbLastBlockSize);

							m_nMode             = Mode_Out;
							m_cbOutBufferOffset = 0;
							m_cbOutBufferSize   = (Size)cbLastBlockSize;
						}
					}
					else //empty stream
					{
						m_nMode          = Mode_Finished;
						pBuffers->nState = State_Finish;

						return Status();
					}
				}
			}
			else
			{
				if (m_cbInBufferSize == m_cbBlockSize)
				{
					if (!(status = m_pHandler->OnBlocks(m_cbBlockSize, m_pInBuffer, m_pOutBuffer)))
					{
						pBuffers->nState = State_Error;
						m_nMode          = Mode_Error;

						return status;
					}
					m_nMode             = Mode_Out;
					m_cbOutBufferOffset = 0;
					m_cbOutBufferSize   = m_cbBlockSize;
				}
			}
		}
		if (Mode_Out == m_nMode)
		{
			if (m_cbOutBufferOffset < m_cbOutBufferSize)
			{
				cbSize = m_cbOutBufferSize - m_cbOutBufferOffset;
				if (cbSize > pBuffers->cbOutSize)
				{
					cbSize = pBuffers->cbOutSize;
				}
				if (0 < cbSize)
				{
					memcpy(pBuffers->pOutBuffer, m_pOutBuffer, cbSize);
					m_cbOutBufferOffset += cbSize;
					m_cbSentBytes += cbSize;
					pBuffers->cbOutSize -= (UInt32)cbSize;
					pBuffers->cbOutTotalSize = (UInt32)m_cbSentBytes;
					pBuffers->pOutBuffer = (Byte *)pBuffers->pOutBuffer + cbSize;
				}
			}
			if (m_cbOutBufferOffset == m_cbOutBufferSize)
			{
				if (pBuffers->bHasMoreData)
				{
					m_nMode = Mode_InOut;
				}
				else
				{
					m_nMode          = Mode_Finished;
					pBuffers->nState = State_Finish;

					return Status();
				}
			}
			else
			{
				if (0 == pBuffers->cbOutSize)
				{
					pBuffers->nState = State_Continue;

					return Status();
				}
			}
		}
	}
}

}//namespace IO

}//namespace CX
