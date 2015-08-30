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
	m_nDir      = nDir;
	m_nDictSize = nDictSize;
	m_nLevel    = nLevel;
	m_nMemLevel = nMemLevel;
	m_nStrategy = nStrategy;
	m_pStream   = NULL;
	Reset();
}

LZHAMFilter::~LZHAMFilter()
{
	End();
}

void LZHAMFilter::End()
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
	if ((Size)TYPE_UINT32_MAX < pBuffers->cbInSize || (Size)TYPE_UINT32_MAX < pBuffers->cbOutSize)
	{
		return Status_InvalidArg;
	}
	if (Dir_Compress == m_nDir)
	{
		int nStatus;

		((lzham_z_stream *)m_pStream)->next_in   = (const unsigned char *)pBuffers->pInBuffer;
		((lzham_z_stream *)m_pStream)->avail_in  = (unsigned int)pBuffers->cbInSize;
		((lzham_z_stream *)m_pStream)->next_out  = (unsigned char *)pBuffers->pOutBuffer;
		((lzham_z_stream *)m_pStream)->avail_out = (unsigned int)pBuffers->cbOutSize;
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

		((lzham_z_stream *)m_pStream)->next_in   = (const unsigned char *)pBuffers->pInBuffer;
		((lzham_z_stream *)m_pStream)->avail_in  = (unsigned int)pBuffers->cbInSize;
		((lzham_z_stream *)m_pStream)->next_out  = (unsigned char *)pBuffers->pOutBuffer;
		((lzham_z_stream *)m_pStream)->avail_out = (unsigned int)pBuffers->cbOutSize;
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

Status LZHAMFilter::Reset()
{
	End();

	if (Dir_Compress == m_nDir)
	{
		int nStatus;

		if (NULL != (m_pStream = new lzham_z_stream()))
		{
			if (LZHAM_Z_OK != (nStatus = lzham_z_deflateInit2((lzham_z_stream *)m_pStream, m_nLevel, LZHAM_Z_LZHAM, m_nDictSize, 
			                                                  m_nMemLevel, m_nStrategy)))
			{
				delete (lzham_z_stream *)m_pStream;
				m_pStream = NULL;

				return Status(Status_OperationFailed, "lzham_z_deflateInit2 failed with error {1}", nStatus);
			}
		}
	}
	else
	if (Dir_Uncompress == m_nDir)
	{
		int nStatus;

		if (NULL != (m_pStream = new lzham_z_stream()))
		{
			if (LZHAM_Z_OK != (nStatus = lzham_z_inflateInit2((lzham_z_stream *)m_pStream, m_nDictSize)))
			{
				delete (lzham_z_stream *)m_pStream;
				m_pStream = NULL;

				return Status(Status_OperationFailed, "lzham_z_inflateInit2 failed with error {1}", nStatus);
			}
		}
	}
	else
	{
		m_pStream = NULL;

		return Status_InvalidArg;
	}

	return Status();
}

}//namespace Archive

}//namespace CX
