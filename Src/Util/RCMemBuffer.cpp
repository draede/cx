/* 
 * CX - C++ framework for general purpose development
 *
 * https://github.com/draede/cx
 * 
 * Copyright (C) 2014 - 2016 draede - draede [at] outlook [dot] com
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

#include "CX/Util/RCMemBuffer.hpp"
#include "CX/Util/RCMemBufferMgr.hpp"


namespace CX
{

namespace Util
{

RCMemBuffer::RCMemBuffer()
{
	m_cbSize    = 0;
	m_pMem      = NULL;
	m_bStatic   = true;
}

RCMemBuffer::RCMemBuffer(const RCMemBuffer &mb)
{
	m_pMem      = mb.GetMem();
	m_cbSize    = mb.GetSize();
	m_bStatic   = false;
	RCMemBufferMgr::Get().Retain(m_pMem);
}

RCMemBuffer::RCMemBuffer(Size cbSize)
{
	m_pMem      = RCMemBufferMgr::Get().Alloc(cbSize);
	m_cbSize    = cbSize;
	m_bStatic   = false;
}

RCMemBuffer::RCMemBuffer(const void *pMem, Size cbSize, bool bStatic/* = false*/)
{
	m_bStatic = bStatic;
	if (m_bStatic)
	{
		m_pMem   = (void *)pMem;
		m_cbSize = cbSize;
	}
	else
	{
		m_pMem      = RCMemBufferMgr::Get().Alloc(cbSize);
		m_cbSize    = cbSize;
		memcpy(m_pMem, pMem, cbSize);
	}
}

RCMemBuffer::RCMemBuffer(const Char *szStr, bool bStatic/* = false*/)
{
	const void   *pMem  = szStr;
	Size         cbSize = cx_strlen(szStr) * sizeof(Char);

	m_bStatic = bStatic;
	if (m_bStatic)
	{
		m_pMem = (void *)pMem;
		m_cbSize = cbSize;
	}
	else
	{
		m_pMem = RCMemBufferMgr::Get().Alloc(cbSize);
		m_cbSize = cbSize;
		memcpy(m_pMem, pMem, cbSize);
	}
}

RCMemBuffer::RCMemBuffer(const WChar *wszStr, bool bStatic/* = false*/) 
{
	const void   *pMem  = wszStr;
	Size         cbSize = cxw_strlen(wszStr) * sizeof(WChar);

	m_bStatic = bStatic;
	if (m_bStatic)
	{
		m_pMem = (void *)pMem;
		m_cbSize = cbSize;
	}
	else
	{
		m_pMem = RCMemBufferMgr::Get().Alloc(cbSize);
		m_cbSize = cbSize;
		memcpy(m_pMem, pMem, cbSize);
	}
}

RCMemBuffer::RCMemBuffer(const String &sStr, bool bStatic/* = false*/) 
{
	const void   *pMem  = sStr.c_str();
	Size         cbSize = sStr.size() * sizeof(Char);

	m_bStatic = bStatic;
	if (m_bStatic)
	{
		m_pMem = (void *)pMem;
		m_cbSize = cbSize;
	}
	else
	{
		m_pMem = RCMemBufferMgr::Get().Alloc(cbSize);
		m_cbSize = cbSize;
		memcpy(m_pMem, pMem, cbSize);
	}
}

RCMemBuffer::RCMemBuffer(const WString &wsStr, bool bStatic/* = false*/)
{
	const void   *pMem  = wsStr.c_str();
	Size         cbSize = wsStr.size() * sizeof(WChar);

	m_bStatic = bStatic;
	if (m_bStatic)
	{
		m_pMem = (void *)pMem;
		m_cbSize = cbSize;
	}
	else
	{
		m_pMem = RCMemBufferMgr::Get().Alloc(cbSize);
		m_cbSize = cbSize;
		memcpy(m_pMem, pMem, cbSize);
	}
}

RCMemBuffer::~RCMemBuffer()
{
	if (!m_bStatic)
	{
		RCMemBufferMgr::Get().Release(m_pMem);
	}
}

RCMemBuffer &RCMemBuffer::operator=(const RCMemBuffer &mb)
{
	if (!m_bStatic)
	{
		RCMemBufferMgr::Get().Release(m_pMem);
	}
	m_pMem      = mb.GetMem();
	m_cbSize    = mb.GetSize();
	m_bStatic   = false;
	RCMemBufferMgr::Get().Retain(m_pMem);

	return *this;
}

void *RCMemBuffer::GetMem() const
{
	return m_pMem;
}

Size RCMemBuffer::GetSize() const
{
	return m_cbSize;
}

}//namespace Util

}//namespace CX
