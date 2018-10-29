/* 
 * CX - C++ framework for general purpose development
 *
 * https://github.com/draede/cx
 * 
 * Copyright (C) 2014 - 2018 draede - draede [at] outlook [dot] com
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


#include "CX/CBuffers/Tools/IStream.hpp"


namespace CX
{

namespace CBuffers
{

namespace Tools
{

class MemoryStream : public IStream
{
public:

	MemoryStream(const void *pData, Size cbSize)
	{
		m_pData = (const Byte *)pData;
		m_cbSize = cbSize;
		m_cbOffset = 0;
	}

	virtual Byte Peek()
	{
		if (m_cbOffset < m_cbSize)
		{
			return *(m_pData + m_cbOffset);
		}
		else
		{
			return 0;
		}
	}

	virtual Byte Get()
	{
		if (m_cbOffset < m_cbSize)
		{
			return *(m_pData + m_cbOffset++);
		}
		else
		{
			return 0;
		}
	}

	virtual Bool Next()
	{
		if (m_cbOffset < m_cbSize)
		{
			m_cbOffset++;

			return True;
		}
		else
		{
			return False;
		}
	}

	virtual Bool HasMore()
	{
		return (m_cbOffset < m_cbSize);
	}

private:

	const Byte   *m_pData;
	Size         m_cbSize;
	Size         m_cbOffset;

};

}//namespace Tools

}//namespace CBuffers

}//namespace CX
