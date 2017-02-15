
#include "CX/Log/OptimizedStreamOutput.hpp"


namespace CX
{

namespace Log
{

OptimizedStreamOutput::OptimizedStreamOutput(const Char *szPath, bool bAppend/* = false*/, Size cFlushDelay/* = 3000*/, 
                                             Size cbMaxMem/* = 1048576*/)
{
	m_bWide = false;
	Init(CreateFileA(szPath, FILE_APPEND_DATA, FILE_SHARE_READ, 0, bAppend ? OPEN_ALWAYS : CREATE_ALWAYS, 
	                 FILE_ATTRIBUTE_NORMAL, NULL), cFlushDelay, cbMaxMem);
}

OptimizedStreamOutput::OptimizedStreamOutput(const WChar *wszPath, bool bAppend/* = false*/, 
                                             Size cFlushDelay/* = 3000*/, Size cbMaxMem/* = 1048576*/)
{
	m_bWide = true;
	Init(CreateFileW(wszPath, FILE_APPEND_DATA, FILE_SHARE_READ, 0, bAppend ? OPEN_ALWAYS : CREATE_ALWAYS, 
	                 FILE_ATTRIBUTE_NORMAL, NULL), cFlushDelay, cbMaxMem);
}

Status OptimizedStreamOutput::Init(void *hFile, Size cFlushDelay, Size cbMaxMem)
{
	DWORD  dwID;
	Status status;

	m_hStopEvent     = NULL;
	m_hThread        = NULL;
	m_pVectorStrings = NULL;
	m_cFlushDelay    = cFlushDelay;
	m_cbMaxMem       = cbMaxMem;
	m_cbCrMem        = 0;
	m_hFile          = hFile;

	for (;;)
	{
		if (INVALID_HANDLE_VALUE == m_hFile)
		{
			m_hFile = NULL;
			status  = Status(Status_CreateFailed, "Failed to create log file");

			break;
		}
		if (NULL == (m_pVectorStrings = new (std::nothrow) StringsVector()))
		{
			status  = Status(Status_MemAllocFailed, "Failed to allocate strings");
			
			break;
		}
		if (NULL == (m_hStopEvent = CreateEventA(NULL, FALSE, FALSE, NULL)))
		{
			status = Status(Status_OperationFailed, "CreateEventA failed with error {1}", GetLastError());

			break;
		}
		if (NULL == (m_hThread = CreateThread(NULL, 0, &OptimizedStreamOutput::ThreadProc, this, 0, &dwID)))
		{
			status = Status(Status_OperationFailed, "CreateThread failed with error {1}", GetLastError());

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
	if (NULL != m_hThread)
	{
		SetEvent(m_hStopEvent);
		WaitForSingleObject(m_hThread, INFINITE);
		CloseHandle(m_hThread);
		m_hThread = NULL;
	}
	if (NULL != m_hStopEvent)
	{
		CloseHandle(m_hStopEvent);
		m_hStopEvent = NULL;
	}
	if (NULL != m_hFile)
	{
		CloseHandle(m_hFile);
		m_hFile = NULL;
	}
	if (NULL != m_pVectorStrings)
	{
		delete m_pVectorStrings;
		m_pVectorStrings = NULL;
	}

	return Status();
}

Status OptimizedStreamOutput::Write(Level nLevel, const Char *szTag, const Char *pBuffer, Size cLen)
{
	CX_UNUSED(nLevel);
	CX_UNUSED(szTag);

	String sLog(pBuffer, cLen);

	{
		Sys::Locker locker(&m_lockStrings);

		if (m_cbCrMem + cLen > m_cbMaxMem)
		{
			return Status_TooBig;
		}
		if (NULL != m_pVectorStrings)
		{
			m_pVectorStrings->push_back(sLog);
			m_cbCrMem += cLen;
		}
	}

	return Status();
}

unsigned long __stdcall  OptimizedStreamOutput::ThreadProc(void *pArg)
{
	OptimizedStreamOutput *pThis = (OptimizedStreamOutput *)pArg;
	DWORD                 dwRet;
	DWORD                 dwAckSize;
	Status                status;

	for (;;)
	{
		dwRet = WaitForSingleObject(pThis->m_hStopEvent, (DWORD)pThis->m_cFlushDelay);

		StringsVector *pVectorStrings = NULL;

		{
			Sys::Locker locker(&pThis->m_lockStrings);

			pVectorStrings          = pThis->m_pVectorStrings;
			pThis->m_pVectorStrings = new (std::nothrow) StringsVector();
			pThis->m_cbCrMem        = 0;

			if (pThis->m_bWide)
			{
				WString wsPath;

				if (pThis->NeedsReopenWithNewPath(pThis->m_hFile, &wsPath))
				{
					if ((status = pThis->Reopen(wsPath.c_str())).IsNOK())
					{
						//
					}
				}
			}
			else
			{
				String sPath;

				if (pThis->NeedsReopenWithNewPath(pThis->m_hFile, &sPath))
				{
					if ((status = pThis->Reopen(sPath.c_str())).IsNOK())
					{
						//
					}
				}
			}
		}

		if (NULL != pVectorStrings)
		{
			if (NULL != pThis->m_hFile)
			{
				for (StringsVector::iterator iter = pVectorStrings->begin(); iter != pVectorStrings->end(); iter++)
				{
					WriteFile(pThis->m_hFile, iter->c_str(), (DWORD)iter->size(), &dwAckSize, NULL);
				}

				FlushFileBuffers(pThis->m_hFile);
			}
			delete pVectorStrings;
		}

		if (WAIT_OBJECT_0 == dwRet)
		{
			if (NULL != pThis->m_hFile)
			{
				FlushFileBuffers(pThis->m_hFile);
			}

			break;
		}
	}

	return 0;
}

Status OptimizedStreamOutput::Reopen(const Char *szPath)
{
	if (NULL != m_hFile)
	{
		FlushFileBuffers(m_hFile);
		CloseHandle(m_hFile);
		m_hFile = NULL;
	}

	if (INVALID_HANDLE_VALUE == (m_hFile = CreateFileA(szPath, FILE_APPEND_DATA, FILE_SHARE_READ, 0, CREATE_ALWAYS, 
	                                                   FILE_ATTRIBUTE_NORMAL, NULL)))
	{
		m_hFile = NULL;

		return Status_CreateFailed;
	}

	return Status();
}

Status OptimizedStreamOutput::Reopen(const WChar *wszPath)
{
	if (NULL != m_hFile)
	{
		FlushFileBuffers(m_hFile);
		CloseHandle(m_hFile);
		m_hFile = NULL;
	}

	if (INVALID_HANDLE_VALUE == (m_hFile = CreateFileW(wszPath, FILE_APPEND_DATA, FILE_SHARE_READ, 0, CREATE_ALWAYS,
	                                                   FILE_ATTRIBUTE_NORMAL, NULL)))
	{
		m_hFile = NULL;

		return Status_CreateFailed;
	}

	return Status();
}

}//namespace Log

}//namespace CX
