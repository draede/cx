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
 
#include "CX/Log/OptimizedStreamOutput.hpp"
#include "CX/Str/UTF8.hpp"
#include "CX/C/stdio.h"


namespace CX
{

namespace Log
{

OptimizedStreamOutput::OptimizedStreamOutput(const Char *szPath, UInt64 cbMaxFileSize/* = 10485760*/, 
                                             UInt32 cFlushDelay/* = 3000*/, Size cbMaxMem/* = 1048576*/)
{
	m_bWide = false;
	m_sPath = szPath;
	m_pFOS  = new (std::nothrow) IO::FileOutputStream(szPath, true);
	Init(cbMaxFileSize, cFlushDelay, cbMaxMem);
}

OptimizedStreamOutput::OptimizedStreamOutput(const WChar *wszPath, UInt64 cbMaxFileSize/* = 10485760*/, 
                                             UInt32 cFlushDelay/* = 3000*/, Size cbMaxMem/* = 1048576*/)
{
	m_bWide  = true;
	m_wsPath = wszPath;
	m_pFOS   = new (std::nothrow) IO::FileOutputStream(wszPath, true);
	Init(cbMaxFileSize, cFlushDelay, cbMaxMem);
}

Status OptimizedStreamOutput::Init(UInt64 cbMaxFileSize, UInt32 cFlushDelay, Size cbMaxMem)
{
	Status status;

	m_pMemPool       = NULL;
	m_cFlushDelay    = cFlushDelay;
	m_cbMaxMem       = cbMaxMem;
	m_cbCrMem        = 0;
	m_cbMaxFileSize  = cbMaxFileSize / 2;
	m_cbCrFileSize   = 0;


	for (;;)
	{
		if (NULL == m_pFOS)
		{
			status  = Status(Status_CreateFailed, "Failed to create log file");
			break;
		}

		m_pFOS->GetSize(&m_cbCrFileSize);

		if (!m_pFOS->IsOK())
		{
			delete m_pFOS;
			m_pFOS = 0;
			status = Status(Status_CreateFailed, "Failed to create log file");

			break;
		}
		if (NULL == (m_pMemPool = new (std::nothrow) Util::DynMemPool()))
		{
			status = Status(Status_MemAllocFailed, "Failed to allocate memory");

			break;
		}
		if ((status = m_eventStop.Create(false, false)).IsNOK())
		{
			break;
		}
		if ((status = m_threadWrite.Run(std::bind(&OptimizedStreamOutput::WriteThread, this))).IsNOK())
		{
			break;
		}

		break;
	}
	if (status.IsNOK())
	{
		Uninit();
	}

	return status;
}

OptimizedStreamOutput::~OptimizedStreamOutput()
{
	Uninit();
}

Status OptimizedStreamOutput::Uninit()
{
	if (m_threadWrite.IsRunning())
	{
		m_eventStop.Set();
		m_threadWrite.Wait();
	}
	if (m_eventStop.IsOK())
	{
		m_eventStop.Destroy();
	}
	if (NULL != m_pFOS)
	{
		delete m_pFOS;
		m_pFOS = NULL;
	}
	if (NULL != m_pMemPool)
	{
		delete m_pMemPool;
		m_pMemPool = NULL;
	}

	return Status();
}

Status OptimizedStreamOutput::Write(Level nLevel, const Char *szTag, const Char *pBuffer, Size cLen)
{
	CX_UNUSED(nLevel);
	CX_UNUSED(szTag);

	String sLog(pBuffer, cLen);

	{
		Sys::Locker locker(&m_lockData);

		if (m_cbCrMem + cLen > m_cbMaxMem)
		{
			return Status_TooBig;
		}
		if (NULL != m_pMemPool)
		{
			m_pMemPool->Add(sLog.c_str(), sLog.size());
			m_cbCrMem += cLen;
		}
	}

	return Status();
}

void OptimizedStreamOutput::WriteThread()
{
	Sys::Event::WaitResult nWaitRes;
	Size                   cbAckSize;
	Status                 status;

	for (;;)
	{
		nWaitRes = m_eventStop.Wait(m_cFlushDelay);

		Util::DynMemPool *pMemPool = m_pMemPool;

		{
			Sys::Locker locker(&m_lockData);

			pMemPool   = m_pMemPool;
			m_pMemPool = new (std::nothrow) Util::DynMemPool();
			m_cbCrMem  = 0;
		}

		if (NULL != pMemPool)
		{
			if (NULL != m_pFOS)
			{
				if (0 < m_cbMaxFileSize && m_cbCrFileSize + pMemPool->GetSize() > m_cbMaxFileSize)
				{
					delete m_pFOS;
					if (m_bWide)
					{
						WString wsNewPath;

						wsNewPath = m_wsPath;
						wsNewPath += L".old";
#ifdef CX_OS_WINDOWS
						MoveFileExW(m_wsPath.c_str(), wsNewPath.c_str(), 
							         MOVEFILE_REPLACE_EXISTING | MOVEFILE_WRITE_THROUGH);
#else
						String sPath;
						String sNewPath;

						Str::UTF8::FromWChar(m_wsPath.c_str(), &sPath);
						Str::UTF8::FromWChar(wsNewPath.c_str(), &sNewPath);

						rename(sPath.c_str(), sNewPath.c_str());
#endif
						m_pFOS         = new (std::nothrow) IO::FileOutputStream(m_wsPath.c_str(), false);
						m_cbCrFileSize = 0;
					}
					else
					{
						String sNewPath;

						sNewPath = m_sPath;
						sNewPath += ".old";
#ifdef CX_OS_WINDOWS
						MoveFileExA(m_sPath.c_str(), sNewPath.c_str(),
							         MOVEFILE_REPLACE_EXISTING | MOVEFILE_WRITE_THROUGH);
#else
						rename(m_sPath.c_str(), sNewPath.c_str());
#endif
						m_pFOS         = new (std::nothrow) IO::FileOutputStream(m_sPath.c_str(), false);
						m_cbCrFileSize = 0;
					}
				}
				if (NULL != m_pFOS)
				{
					m_pFOS->Write(pMemPool->GetMem(), pMemPool->GetSize(), &cbAckSize);
					m_cbCrFileSize += cbAckSize;
					m_pFOS->Flush();
				}
			}
			delete pMemPool;
		}

		if (Sys::Event::Wait_OK == nWaitRes)
		{
			break;
		}
	}
}

}//namespace Log

}//namespace CX
