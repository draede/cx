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
#include "CX/IO/IInputStream.hpp"
#include "CX/Util/MemPool.hpp"


namespace CX
{

namespace CBuffers
{

namespace Tools
{

class InputStreamStream : public IStream
{
public:

	InputStreamStream(IO::IInputStream *pInputStream)
	{
		m_pInputStream = pInputStream;
		m_cbSize       = 0;
		m_cbOffset     = 0;
		m_bEOF         = False;
		m_mem.SetSize(BUFFER_SIZE);
	}

	virtual Byte Peek()
	{
		if (BUFFER_SIZE != m_mem.GetSize())
		{
			return 0;
		}
		if (m_bEOF)
		{
			return 0;
		}
		ReadMore();
		if (m_bEOF)
		{
			return 0;
		}

		return *((Byte *)m_mem.GetMem() + m_cbOffset);
	}

	virtual Byte Get()
	{
		if (BUFFER_SIZE != m_mem.GetSize())
		{
			return 0;
		}
		if (m_bEOF)
		{
			return 0;
		}
		ReadMore();
		if (m_bEOF)
		{
			return 0;
		}

		return *((Byte *)m_mem.GetMem() + m_cbOffset++);
	}

	virtual Bool Next()
	{
		if (BUFFER_SIZE != m_mem.GetSize())
		{
			return False;
		}
		if (m_bEOF)
		{
			return False;
		}
		ReadMore();
		if (m_bEOF)
		{
			return False;
		}
		m_cbOffset++;

		return True;
	}

	virtual Bool HasMore()
	{
		if (BUFFER_SIZE != m_mem.GetSize())
		{
			return False;
		}
		if (m_bEOF)
		{
			return False;
		}
		ReadMore();
		if (m_bEOF)
		{
			return False;
		}

		return True;
	}

private:

	static const Size   BUFFER_SIZE = 16384;

	IO::IInputStream   *m_pInputStream;
	Util::DynMemPool   m_mem;
	Size               m_cbSize;
	Size               m_cbOffset;
	Bool               m_bEOF;

	void ReadMore()
	{
		if (m_cbOffset < m_cbSize)
		{
			return;
		}

		Size   cbAckSize;
		Status status;

		if (!(status = m_pInputStream->Read(m_mem.GetMem(), m_mem.GetSize(), &cbAckSize)))
		{
			m_bEOF = True;

			return;
		}
		if (0 == cbAckSize)
		{
			m_bEOF = True;

			return;
		}

		m_cbOffset = 0;
		m_cbSize   = cbAckSize;

		return;
	}

};

}//namespace Tools

}//namespace CBuffers

}//namespace CX
