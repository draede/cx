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
#include "CX/Log/StreamOutput.hpp"
#include "CX/Mem.hpp"
#include "CX/Status.hpp"


namespace CX
{

namespace Log
{

StreamOutput::StreamOutput(IO::IOutputStream *pOutputStream, Size cMSFlushDelay/* = 3*/, bool bUseLock/* = true*/)
{
	m_pOutputStream = pOutputStream;
	m_cMSFlushDelay = cMSFlushDelay;
	m_bUseLock      = bUseLock;
}

StreamOutput::~StreamOutput()
{
	if (NULL != m_pOutputStream)
	{
		delete(m_pOutputStream);
	}
}

Status StreamOutput::Write(Level nLevel, const Char *szTag, const Char *pBuffer, Size cLen)
{
	CX_UNUSED(nLevel);
	CX_UNUSED(szTag);
	
	if (NULL == m_pOutputStream)
	{
		return Status(Status_NotInitialized, "No valid stream set");
	}

	Status status;
	Size   cbAckSize;

	if (m_bUseLock)
	{
		m_lock.Enter();
	}

	status = m_pOutputStream->Write(pBuffer, cLen, &cbAckSize);

	if (0 < m_cMSFlushDelay)
	{
		if ((double)m_cMSFlushDelay <= m_timer.GetElapsedTime() * 1000.0)
		{
			m_pOutputStream->Flush();
			m_timer.ResetTimer();
		}
	}

	if (m_bUseLock)
	{
		m_lock.Leave();
	}

	return status;
}

Status StreamOutput::Flush()
{
	if (m_bUseLock)
	{
		m_lock.Enter();
	}
	if (NULL != m_pOutputStream)
	{
		m_pOutputStream->Flush();
		m_timer.ResetTimer();
	}
	if (m_bUseLock)
	{
		m_lock.Leave();
	}

	return Status();
}

}//namespace Log

}//namespace CX

