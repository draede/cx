
#include "CX/Archive/LZHAMFilter.hpp"
#include "lzham.h"


namespace CX
{

namespace Archive
{

LZHAMFilter::LZHAMFilter(Dir nDir/* = Dir_Compress*/, int nDictSize/* = DEF_DICT_SIZE*/, 
                         Level nLevel/* = Level_Default*/, MemLevel nMemLevel/* = MemLevel_Default*/, 
                         Strategy nStrategy/* = Strategy_Default*/)
{
	m_nDir = nDir;
	if (Dir_Compress == nDir)
	{
		int nStatus;

		if (NULL != (m_pStream = new lzham_z_stream()))
		{
			if (LZHAM_Z_OK != (nStatus = lzham_z_deflateInit2((lzham_z_stream *)m_pStream, nLevel, LZHAM_Z_LZHAM, nDictSize, 
			                                                  nMemLevel, nStrategy)))
			{
				delete (lzham_z_stream *)m_pStream;
				m_pStream = NULL;
			}
		}
	}
	else
	if (Dir_Uncompress == nDir)
	{
		int nStatus;

		if (NULL != (m_pStream = new lzham_z_stream()))
		{
			if (LZHAM_Z_OK != (nStatus = lzham_z_inflateInit2((lzham_z_stream *)m_pStream, nDictSize)))
			{
				delete (lzham_z_stream *)m_pStream;
				m_pStream = NULL;
			}
		}
	}
	else
	{
		m_pStream = NULL;
	}
}

LZHAMFilter::~LZHAMFilter()
{
	if (NULL != m_pStream)
	{
		if (Dir_Compress == m_nDir)
		{
			lzham_z_deflateEnd((lzham_z_stream *)m_pStream);
		}
		else
		if (Dir_Uncompress == m_nDir)
		{
			lzham_z_inflateEnd((lzham_z_stream *)m_pStream);
		}
		delete (lzham_z_stream *)m_pStream;
		m_pStream = NULL;
	}
}

Status LZHAMFilter::Filter(Buffers *pBuffers)
{
	if (NULL == m_pStream)
	{
		return Status_NotInitialized;
	}
	if (Dir_Compress == m_nDir)
	{
		int nStatus;

		((lzham_z_stream *)m_pStream)->next_in   = (const unsigned char *)pBuffers->pInBuffer;
		((lzham_z_stream *)m_pStream)->avail_in  = pBuffers->cbInSize;
		((lzham_z_stream *)m_pStream)->next_out  = (unsigned char *)pBuffers->pOutBuffer;
		((lzham_z_stream *)m_pStream)->avail_out = pBuffers->cbOutSize;
		if (LZHAM_Z_OK != (nStatus = lzham_z_deflate((lzham_z_stream *)m_pStream, 
		                                             pBuffers->bHasMoreData ? LZHAM_Z_NO_FLUSH : LZHAM_Z_FINISH)))
		{
			if (LZHAM_Z_STREAM_END == nStatus)
			{
				pBuffers->nState = State_Finish;
			}
			else
			{
				pBuffers->nState = State_Error;

				return Status(Status_OperationFailed, "lzham_z_deflateInit2 failed with error {1}\n", nStatus);
			}
		}
		else
		{
			pBuffers->nState = State_Continue;
		}
		pBuffers->pInBuffer  = ((lzham_z_stream *)m_pStream)->next_in;
		pBuffers->cbInSize   = ((lzham_z_stream *)m_pStream)->avail_in;
		pBuffers->pOutBuffer = ((lzham_z_stream *)m_pStream)->next_out;
		pBuffers->cbOutSize  = ((lzham_z_stream *)m_pStream)->avail_out;

		return Status();
	}
	else
	if (Dir_Uncompress == m_nDir)
	{
		int nStatus;

		((lzham_z_stream *)m_pStream)->next_in = (const unsigned char *)pBuffers->pInBuffer;
		((lzham_z_stream *)m_pStream)->avail_in = pBuffers->cbInSize;
		((lzham_z_stream *)m_pStream)->next_out = (unsigned char *)pBuffers->pOutBuffer;
		((lzham_z_stream *)m_pStream)->avail_out = pBuffers->cbOutSize;
		if (LZHAM_Z_OK != (nStatus = lzham_z_inflate((lzham_z_stream *)m_pStream, LZHAM_Z_SYNC_FLUSH)))
		{
			if (LZHAM_Z_STREAM_END == nStatus)
			{
				pBuffers->nState = State_Finish;
			}
			else
			{
				pBuffers->nState = State_Error;

				return Status(Status_OperationFailed, "lzham_z_inflate failed with error {1}\n", nStatus);
			}
		}
		else
		{
			pBuffers->nState = State_Continue;
		}
		pBuffers->pInBuffer  = ((lzham_z_stream *)m_pStream)->next_in;
		pBuffers->cbInSize   = ((lzham_z_stream *)m_pStream)->avail_in;
		pBuffers->pOutBuffer = ((lzham_z_stream *)m_pStream)->next_out;
		pBuffers->cbOutSize  = ((lzham_z_stream *)m_pStream)->avail_out;

		return Status();
	}
	else
	{
		return Status_NotInitialized;
	}
}

}//namespace Archive

}//namespace CX
