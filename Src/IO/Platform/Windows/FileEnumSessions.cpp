/* 
 * CX - C++ framework for general purpose development
 *
 * https://github.com/draede/cx
 * 
 * Copyright (C) 2014 - 2022 draede
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
#include "CX/Platform.hpp"


#if defined(CX_OS_WINDOWS)


#include "CX/IO/Platform/Windows/FileEnumSessions.hpp"
#include "CX/Sys/Platform/Windows/UUID.hpp"


namespace CX
{

namespace IO
{

FileEnumSessions::FileEnumSessions()
{
	m_pHandler    = NULL;
	m_bRunning    = False;
	m_hEventReady = CreateEvent(NULL, FALSE, FALSE, NULL);
}

FileEnumSessions::~FileEnumSessions()
{
	Stop();
	CloseHandle(m_hEventReady);
}

Status FileEnumSessions::Start(IHandler *pHandler)
{
	Sys::FastWLocker   locker(&m_frwlSessions);

	if (m_bRunning)
	{
		return Status_Busy;
	}
	m_pHandler = pHandler;
	m_bRunning = True;

	return Status();
}

Status FileEnumSessions::Stop()
{
	ThreadsVector   vectorThreads;

	{
		Sys::FastWLocker   locker(&m_frwlSessions);

		if (!m_bRunning)
		{
			return Status_NotInitialized;
		}
		for (auto iter = m_mapSessions.begin(); iter != m_mapSessions.end(); ++iter)
		{
			vectorThreads.push_back(iter->second->m_hThread);
		}
		m_bRunning = False;
	}
	if (!vectorThreads.empty())
	{
		WaitForMultipleObjects((DWORD)vectorThreads.size(), &vectorThreads[0], TRUE, INFINITE);
	}
	m_pHandler = NULL;
	m_bRunning = False;

	return Status();
}

Status FileEnumSessions::Wait()
{
	if (!m_bRunning)
	{
		return Status_NotInitialized;
	}
	WaitForSingleObject(m_hEventReady, INFINITE);

	return Status();
}

Bool FileEnumSessions::IsRunning() const
{
	Sys::FastRLocker   locker(&m_frwlSessions);

	return m_bRunning;
}

Status FileEnumSessions::StartFileEnumSession(String *psID, void *pCustomData, const WChar *wszPath, 
                                      const FileEnumerator2::Config &config/* = FileEnumerator2::Config::GetDefault()*/)
{
	FileEnumerator2::PathsVector   vectorPaths;

	vectorPaths.push_back(wszPath);

	return StartFileEnumSession(psID, pCustomData, vectorPaths, config);
}

Status FileEnumSessions::StartFileEnumSession(String *psID, void *pCustomData, 
                                      const FileEnumerator2::PathsVector &vectorPaths, 
                                      const FileEnumerator2::Config &config/* = FileEnumerator2::Config::GetDefault()*/)
{
	Sys::FastWLocker   locker(&m_frwlSessions);
	DWORD              dwThreadID;
	Sys::UUID          uuid;
	SessionData        *pSession = NULL;
	Status             status;

	if (!m_bRunning)
	{
		return Status_NotInitialized;
	}
	if (MAX_SESSIONS_COUNT <= m_mapSessions.size())
	{
		return Status_InvalidCall;
	}
	for (;;)
	{
		if (NULL == (pSession = new (std::nothrow) SessionData()))
		{
			status = Status(Status_MemAllocFailed, "Failed to allocate session");

			break;
		}
		pSession->m_pSessions           = this;
		pSession->m_session.pCustomData = pCustomData;
		pSession->m_session.vectorPaths = vectorPaths;
		pSession->m_session.config      = config;
		pSession->m_bRunning            = True;
		if (!(status = uuid.Generate()))
		{
			break;
		}
		if (!(status = uuid.ToString(&pSession->m_session.sID)))
		{
			break;
		}
		*psID = pSession->m_session.sID;
		memset(&pSession->m_session.lastStats, 0, sizeof(pSession->m_session.lastStats));
		m_mapSessions[pSession->m_session.sID] = pSession;
		ResetEvent(pSession->m_pSessions->m_hEventReady);
		if (NULL == (pSession->m_hThread = CreateThread(NULL, 0, &FileEnumSessions::WorkerThread, pSession, 0, 
		                                                &dwThreadID)))
		{
			m_mapSessions.erase(m_mapSessions.find(pSession->m_session.sID));
			status = Status(Status_OperationFailed, "Failed to create session thread with error {1}", GetLastError());

			break;
		}

		break;
	}
	if (!status)
	{
		if (NULL != pSession)
		{
			delete pSession;
		}
	}

	return status;
}

Status FileEnumSessions::StopFileEnumSession(const Char *szID)
{
	HANDLE   hThread = NULL;

	{
		Sys::FastWLocker   locker(&m_frwlSessions);
		auto               iter = m_mapSessions.find(szID);

		if (m_mapSessions.end() == iter)
		{
			return Status_NotFound;
		}
		iter->second->m_bRunning = False;
		hThread                  = iter->second->m_hThread;
	}

	if (NULL != hThread)
	{
		WaitForSingleObject(hThread, INFINITE);
	}

	return Status();
}

Size FileEnumSessions::GetFileEnumSessionsCount() const
{
	Sys::FastRLocker   locker(&m_frwlSessions);

	return m_mapSessions.size();
}

DWORD WINAPI FileEnumSessions::WorkerThread(void *pThreadArg)
{
	SessionData   *pSession = (SessionData *)pThreadArg;
	Status        status;

	pSession->m_pSessions->m_pHandler->OnBegin(&pSession->m_session);
	
	status = FileEnumerator2::Run(pSession->m_session.vectorPaths, pSession, pSession->m_session.config);

	pSession->m_pSessions->m_pHandler->OnEnd(&pSession->m_session, status, pSession->m_session.lastStats);

	{
		Sys::FastWLocker   locker(&pSession->m_pSessions->m_frwlSessions);
		auto               iter = pSession->m_pSessions->m_mapSessions.find(pSession->m_session.sID);

		if (pSession->m_pSessions->m_mapSessions.end() != iter)
		{
			pSession->m_pSessions->m_mapSessions.erase(iter);
		}
		if (pSession->m_pSessions->m_mapSessions.empty())
		{
			SetEvent(pSession->m_pSessions->m_hEventReady);
		}
	}

	delete pSession;

	return 0;
}

Bool FileEnumSessions::SessionData::OnBegin()
{
	return True;
}

Bool FileEnumSessions::SessionData::OnFile(FileEnumerator2::IFile *pFile, void **ppResult, 
                                       const FileEnumerator2::Stats *pStats)
{
	ppResult;

	m_pSessions->m_pHandler->OnFile(&m_session, pFile, *pStats);

	return (m_bRunning && m_pSessions->m_bRunning);
}

Bool FileEnumSessions::SessionData::OnEnd(const FileEnumerator2::Stats *pStats)
{
	m_session.lastStats = *pStats;

	return (m_bRunning && m_pSessions->m_bRunning);
}

Bool FileEnumSessions::SessionData::OnBeginResults(void **ppContext)
{
	ppContext;

	return (m_bRunning && m_pSessions->m_bRunning);
}

Bool FileEnumSessions::SessionData::OnResult(void *pResult, void *pContext)
{
	pResult;
	pContext;

	return (m_bRunning && m_pSessions->m_bRunning);
}

Bool FileEnumSessions::SessionData::OnEndResults(void *pContext)
{
	pContext;

	return (m_bRunning && m_pSessions->m_bRunning);
}

Bool FileEnumSessions::SessionData::OnError(const Status &status)
{
	status;

	return (m_bRunning && m_pSessions->m_bRunning);
}

Bool FileEnumSessions::SessionData::OnFolder(const WChar *wszPath)
{
	wszPath;

	return (m_bRunning && m_pSessions->m_bRunning);
}

Bool FileEnumSessions::SessionData::OnBeginPath(const WChar *wszPath, Size cIndex)
{
	wszPath;
	cIndex;

	return (m_bRunning && m_pSessions->m_bRunning);
}

Bool FileEnumSessions::SessionData::OnEndPath(const WChar *wszPath, Size cIndex)
{
	wszPath;
	cIndex;

	return (m_bRunning && m_pSessions->m_bRunning);
}

Bool FileEnumSessions::SessionData::OnListLineError(const WChar *wszLine, Size cPathIndex, const Status &status)
{
	wszLine;
	cPathIndex;
	status;

	return (m_bRunning && m_pSessions->m_bRunning);
}

}//namespace IO

}//namespace CX


#endif
