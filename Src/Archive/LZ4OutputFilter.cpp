/* 
 * CX - C++ framework for general purpose development
 *
 * https://github.com/draede/cx
 * 
 * Copyright (C) 2014 - 2022 draede - draede [at] outlook [dot] com
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
#include "CX/Archive/LZ4OutputFilter.hpp"
#include "CX/Hash/BLAKE2Hash.hpp"
#include "CX/Util/RndGen.hpp"
#include "CX/Limits.hpp"
#include "lz4.h"
#include "lz4hc.h"


namespace CX
{

namespace Archive
{

LZ4OutputFilter::LZ4OutputFilter(int nCompressionLevel/* = DEFAULT_COMPRESSION_LEVEL*/)
{
	if (DEFAULT_COMPRESSION_LEVEL > nCompressionLevel)
	{
		nCompressionLevel = DEFAULT_COMPRESSION_LEVEL;
	}
	if (MAX_COMPRESSION_LEVEL < nCompressionLevel)
	{
		nCompressionLevel = MAX_COMPRESSION_LEVEL;
	}
	m_nCompressionLevel = nCompressionLevel;
}

LZ4OutputFilter::~LZ4OutputFilter()
{
}

Status LZ4OutputFilter::ResizeBuffer(Size cbSize)
{
	if (cbSize > m_buffer.GetSize())
	{
		return m_buffer.SetSize(cbSize);
	}

	return Status();
}

Size LZ4OutputFilter::GetBlockSize()
{
	return BLOCK_SIZE;
}

Status LZ4OutputFilter::Filter(const void *pInput, Size cbInputSize, void **ppOutput, Size *pcbOutputSize)
{
	int    cbSize;
	int    cbResSize;
	Status status;

	if (0 == cbInputSize)
	{
		*ppOutput      = NULL;
		*pcbOutputSize = 0;

		return Status();
	}
	if ((Size)LZ4_MAX_INPUT_SIZE < cbInputSize)
	{
		return Status_TooBig;
	}
	if (0 == (cbSize = LZ4_compressBound((int)cbInputSize)))
	{
		return Status_TooBig;
	}
	if (!(status = ResizeBuffer((Size)cbSize)))
	{
		return status;
	}
	if (DEFAULT_COMPRESSION_LEVEL == m_nCompressionLevel)
	{
		cbResSize = LZ4_compress_default((const char *)pInput, (char *)m_buffer.GetMem(), (int)cbInputSize, (int)cbSize);
	}
	else
	{
		cbResSize = LZ4_compress_HC((const char *)pInput, (char *)m_buffer.GetMem(), (int)cbInputSize, (int)cbSize, m_nCompressionLevel);
	}
	if (0 == cbResSize)
	{
		return Status_OperationFailed;
	}
	*pcbOutputSize = (Size)cbResSize;
	*ppOutput      = m_buffer.GetMem();

	return Status();
}

}//namespace Archive

}//namespace CX
