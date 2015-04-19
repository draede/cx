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

#include "CX/Hash/CRC32Hash.hpp"
#include "CX/Status.hpp"
#include "../../Contrib/CRC32Slice8/Include/crc32.h"


namespace CX
{

namespace Hash
{

const Char CRC32Hash::NAME[] = "CRC32";

CRC32Hash::CRC32Hash()
{
	Init();
}

CRC32Hash::~CRC32Hash()
{
}

const Char *CRC32Hash::GetName()
{
	return NAME;
}

Size CRC32Hash::GetSize()
{
	return SIZE;
}

Status CRC32Hash::Init(const void *pHash/* = NULL*/)
{
	if (NULL != pHash)
	{
		m_nCRC32 = *((UInt32 *)pHash);
	}
	else
	{
		m_nCRC32 = 0;
	}

	return Status();
}

Status CRC32Hash::Update(const void *pBuffer, Size cbSize)
{
	m_nCRC32 = crc32_8bytes_internal(pBuffer, cbSize, m_nCRC32);

	return Status();
}

Status CRC32Hash::Done(void *pHash)
{
	*((UInt32 *)pHash) = m_nCRC32;

	return Status();
}

}//namespace Hash

}//namespace CX

