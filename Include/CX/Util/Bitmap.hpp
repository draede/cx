/* 
 * CX - C++ framework for general purpose development
 *
 * https://github.com/draede/cx
 * 
 * Copyright (C) 2014 - 2017 draede, draede [at] outlook [dot] com
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

#pragma once


#include "CX/Types.hpp"
#include "CX/Status.hpp"
#include "CX/Print.hpp"


namespace CX
{

namespace Util
{

class Bitmap
{
public:

	Bitmap(Size cBitsCount = 8);

	Bitmap(const Bitmap &bitset);

	~Bitmap();

	Bitmap &operator=(const Bitmap &bitset);

	Status Copy(const Bitmap &bitset);

	Status SetBitsCount(Size cBitsCount);

	Size GetBitsCount() const;

	Size GetBytesCount() const;

	Status SetBit(Size cIndex, bool bValue);

	bool GetBit(Size cIndex, Status *pStatus = NULL) const;

	Status SetBitsRange(bool bValue, Size cStart, Size cCount = (Size)-1);

	void SetAllBits(bool bValue);

	Byte *GetBytes();

	const Byte *GetBytes() const;

	template <typename OUTPUT>
	void Dump(OUTPUT output)
	{
		for (Size i = 0; i < m_cBitsCount; i++)
		{
			Print(output, "{1}", 0 == (m_pBytes[i / 8] & POWS[i % 8]) ? '0' : '1');
		}
		Print(output, "\n");
	}

private:

	static const Byte POWS[8];

	Byte *m_pBytes;
	Size m_cBitsCount;
	Size m_cBytesCount;

};

}//namespace Util

}//namespace CX

