/* 
 * CX - C++ framework for general purpose development
 *
 * https://github.com/draede/cx
 * 
 * Copyright (C) 2014 - 2020 draede - draede [at] outlook [dot] com
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


#include "CX/Platform.hpp"


#if defined(CX_OS_WINDOWS)


#include "CX/Types.hpp"
#include "CX/Status.hpp"
#include "CX/String.hpp"
#include "CX/Vector.hpp"
#include "CX/Map.hpp"
#include "CX/APIDefs.hpp"
#include "CX/IO/Platform/Windows/FileEnumerator2.hpp"
#include "CX/Sys/FastRWLock.hpp"


namespace CX
{

namespace IO
{

class CX_API FileEnumSessions
{
public:

	static const Size   MAX_SESSIONS_COUNT = 64;

	struct Session
	{
		String                         sID;
		FileEnumerator2::PathsVector   vectorPaths;
		FileEnumerator2::Config        config;
		FileEnumerator2::Stats         lastStats;
		void                           *pCustomData;
	};

	class IHandler
	{
	public:

		virtual ~IHandler() { }

		virtual void OnBegin(const Session *pSession) = 0;

		virtual void OnEnd(const Session *pSession, const Status &status, const FileEnumerator2::Stats &stats) = 0;

		virtual void OnFile(const Session *pSession, FileEnumerator2::IFile *pFile, 
		                    const FileEnumerator2::Stats &stats) = 0;

	};

	FileEnumSessions();

	~FileEnumSessions();

	Status Start(IHandler *pHandler);

	Status Stop();

	Status Wait();

	Bool IsRunning() const;

	Status StartFileEnumSession(String *psID, void *pCustomData, const WChar *wszPath, 
	                            const FileEnumerator2::Config &config = FileEnumerator2::Config::GetDefault());

	Status StartFileEnumSession(String *psID, void *pCustomData, const FileEnumerator2::PathsVector &vectorPaths, 
	                            const FileEnumerator2::Config &config = FileEnumerator2::Config::GetDefault());

	Status StopFileEnumSession(const Char *szID);

	Size GetFileEnumSessionsCount() const;

private:

	class SessionData : public FileEnumerator2::IHandler
	{
	public:

		FileEnumSessions               *m_pSessions;
		HANDLE                         m_hThread;
		Bool                           m_bRunning;
		Session                        m_session;

		virtual Bool OnBegin();

		virtual Bool OnFile(FileEnumerator2::IFile *pFile, void **ppResult, const FileEnumerator2::Stats *pStats);

		virtual Bool OnBeginResults(void **ppContext);

		virtual Bool OnResult(void *pResult, void *pContext);

		virtual Bool OnEndResults(void *pContext);

		virtual Bool OnEnd(const FileEnumerator2::Stats *pStats);

		virtual Bool OnError(const Status &status);

		virtual Bool OnFolder(const WChar *wszPath);

		virtual Bool OnBeginPath(const WChar *wszPath, Size cIndex);

		virtual Bool OnEndPath(const WChar *wszPath, Size cIndex);

		virtual Bool OnListLineError(const WChar *wszLine, Size cPathIndex, const Status &status);
	};

	typedef Map<String, SessionData *>::Type   SessionsMap;

	typedef Vector<HANDLE>::Type           ThreadsVector;

	IHandler                  *m_pHandler;
	HANDLE                    m_hEventReady;
	SessionsMap               m_mapSessions;
	mutable Sys::FastRWLock   m_frwlSessions;
	Bool                      m_bRunning;

	static DWORD WINAPI WorkerThread(void *pThreadArg);

};

}//namespace IO

}//namespace CX


#endif

