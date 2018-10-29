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
#include "CX/Archive/LZHAMInputFilter.hpp"
#include "lzham.h"


namespace CX
{

namespace Archive
{

LZHAMInputFilter::LZHAMInputFilter()
{
}

LZHAMInputFilter::~LZHAMInputFilter()
{
}

Status LZHAMInputFilter::ResizeBuffer(Size cbSize)
{
	if (cbSize > m_buffer.GetSize())
	{
		return m_buffer.SetSize(cbSize);
	}

	return Status();
}

Status LZHAMInputFilter::Filter(const void *pInput, Size cbInputSize, Size cbOrigInputSize, void **ppOutput, Size *pcbOutputSize)
{
	lzham_z_ulong   cbSize;
	int             nRet;
	Status          status;

	if (0 == cbInputSize)
	{
		*ppOutput      = NULL;
		*pcbOutputSize = 0;

		return Status();
	}
	*pcbOutputSize = cbOrigInputSize;
	if (!(status = ResizeBuffer(*pcbOutputSize)))
	{
		return status;
	}
	if (LZHAM_Z_OK != (nRet = lzham_z_uncompress((unsigned char *)m_buffer.GetMem(), &cbSize, (const unsigned char *)pInput, 
	                                             (lzham_z_ulong)cbInputSize)))
	{
		return Status_OperationFailed;
	}
	if (*pcbOutputSize != (Size)cbSize)
	{
		return Status_InvalidArg;
	}
	*ppOutput = m_buffer.GetMem();

	return Status();
}

}//namespace Archive

}//namespace CX
