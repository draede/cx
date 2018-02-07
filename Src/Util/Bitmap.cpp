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
 
#include "CX/Util/Bitmap.hpp"
#include "CX/Mem.hpp"


namespace CX
{

namespace Util
{

const Byte Bitmap::POWS[8] = { 1, 2, 4, 8, 16, 32, 64, 128 };

Bitmap::Bitmap(Size cBitsCount/* = 8*/)
{
	m_pBytes      = NULL;
	m_cBitsCount  = 0;
	m_cBytesCount = 0;
	SetBitsCount(cBitsCount);
}

Bitmap::Bitmap(const Bitmap &bitset)
{
	m_pBytes      = NULL;
	m_cBitsCount  = 0;
	m_cBytesCount = 0;
	Copy(bitset);
}

Bitmap::~Bitmap()
{
	if (NULL != m_pBytes)
	{
		Mem::Free(m_pBytes);
	}
}

Bitmap &Bitmap::operator=(const Bitmap &bitset)
{
	Copy(bitset);

	return *this;
}

Status Bitmap::Copy(const Bitmap &bitset)
{
	Status status;

	if ((status = SetBitsCount(bitset.m_cBitsCount)).IsNOK())
	{
		return status;
	}
	memcpy(m_pBytes, bitset.m_pBytes, m_cBytesCount);

	return Status();
}

Status Bitmap::SetBitsCount(Size cBitsCount)
{
	Size cBytesCount;
	Byte *pBytes;

	cBytesCount = cBitsCount / 8;
	if (0 < cBitsCount % 8)
	{
		cBytesCount++;
	}
	if (NULL == (pBytes = (Byte *)Mem::Alloc(cBitsCount)))
	{
		return Status_MemAllocFailed;
	}
	if (NULL != m_pBytes)
	{
		Size cCount = cBytesCount;

		if (cCount > m_cBytesCount)
		{
			cCount = m_cBytesCount;
		}
		if (0 < cCount)
		{
			memcpy(pBytes, m_pBytes, cCount);
		}
	}
	else
	{
		memset(pBytes, 0x00, cBytesCount);
	}
	if (NULL != m_pBytes)
	{
		Mem::Free(m_pBytes);
	}
	m_pBytes      = pBytes;
	m_cBytesCount = cBytesCount;
	m_cBitsCount  = cBitsCount;

	return Status();
}

Size Bitmap::GetBitsCount() const
{
	return m_cBitsCount;
}

Size Bitmap::GetBytesCount() const
{
	return m_cBytesCount;
}

Status Bitmap::SetBit(Size cIndex, bool bValue)
{
	if (cIndex >= m_cBitsCount)
	{
		return Status_OutOfBounds;
	}

	Size cByteIndex = cIndex / 8;
	Size cBitIndex  = cIndex % 8;

	if (bValue)
	{
		m_pBytes[cByteIndex] |= POWS[cBitIndex];
	}
	else
	{
		m_pBytes[cByteIndex] &= ~POWS[cBitIndex];
	}

	return Status();
}

bool Bitmap::GetBit(Size cIndex, Status *pStatus/* = NULL*/) const
{
	if (cIndex >= m_cBitsCount)
	{
		if (NULL != pStatus)
		{
			*pStatus = Status_OutOfBounds;
		}

		return false;
	}

	if (NULL != pStatus)
	{
		*pStatus = Status_OK;
	}

	Size cByteIndex = cIndex / 8;
	Size cBitIndex  = cIndex % 8;

	return (0 != (m_pBytes[cByteIndex] & POWS[cBitIndex]));
}

Status Bitmap::SetBitsRange(bool bValue, Size cStart, Size cCount/* = (Size) - 1*/)
{
	Size cEnd;

	if (cStart >= m_cBitsCount)
	{
		return Status_OutOfBounds;
	}
	if ((Size) - 1 == cCount)
	{
		cEnd = m_cBitsCount - 1;
	}
	else
	{
		if (0 == cCount)
		{
			return Status();
		}
		cEnd = cStart + cCount - 1;
		if (cEnd >= m_cBitsCount)
		{
			cEnd = m_cBitsCount - 1;
		}
	}	

	Size cStartByte     = cStart / 8;
	Size cEndByte       = cEnd / 8;
	Size cStartLeftover = cStart % 8;
	Size cEndLeftover   = cEnd % 8;
	Size cBitIndex      = cStartByte * 8 + cStartLeftover;

	if (0 < cStartLeftover)
	{
		for (Size i = cStartLeftover; i < 8; i++)
		{
			if (cBitIndex > cEnd)
			{
				break;
			}
			if (bValue)
			{
				m_pBytes[cStartByte] |= POWS[i];
			}
			else
			{
				m_pBytes[cStartByte] &= ~POWS[i];
			}
			cBitIndex++;
		}
		cStartByte++;
	}
	if (cStartByte < cEndByte)
	{
		Size cCountEx = cEndByte - cStartByte;

		if (bValue)
		{
			memset(m_pBytes + cStartByte, 0xFF, cCountEx);
		}
		else
		{
			memset(m_pBytes + cStartByte, 0x00, cCountEx);
		}
		cBitIndex += cCountEx * 8;
	}
	if (cBitIndex <= cEnd)
	{
		for (Size i = 0; i <= cEndLeftover; i++)
		{
			if (bValue)
			{
				m_pBytes[cEndByte] |= POWS[i];
			}
			else
			{
				m_pBytes[cEndByte] &= ~POWS[i];
			}
		}
	}

	return Status();
}

void Bitmap::SetAllBits(bool bValue)
{
	if (0 < m_cBytesCount)
	{
		if (bValue)
		{
			memset(m_pBytes, 0xFF, m_cBytesCount);
		}
		else
		{
			memset(m_pBytes, 0x00, m_cBytesCount);
		}
	}
}

Byte *Bitmap::GetBytes()
{
	return m_pBytes;
}

const Byte *Bitmap::GetBytes() const
{
	return m_pBytes;
}

}//namespace Util

}//namespace CX
