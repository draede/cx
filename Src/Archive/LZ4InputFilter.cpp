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
 
#include "CX/Archive/LZ4InputFilter.hpp"
#include "LZ4.h"


namespace CX
{

namespace Archive
{

LZ4InputFilter::LZ4InputFilter()
{
}

LZ4InputFilter::~LZ4InputFilter()
{
}

Status LZ4InputFilter::ResizeBuffer(Size cbSize)
{
	if (cbSize > m_buffer.GetSize())
	{
		return m_buffer.SetSize(cbSize);
	}

	return Status();
}

Status LZ4InputFilter::Filter(const void *pInput, Size cbInputSize, Size cbOrigInputSize, void **ppOutput, Size *pcbOutputSize)
{
	int    cbResSize;
	Status status;

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
	if (0 == (cbResSize = LZ4_decompress_safe((const char *)pInput, (char *)m_buffer.GetMem(), (int)cbInputSize, *pcbOutputSize)))
	{
		return Status_OperationFailed;
	}
	*ppOutput = m_buffer.GetMem();

	return Status();
}

}//namespace Archive

}//namespace CX
