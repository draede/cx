/* 
 * CX - C++ framework for general purpose development
 *
 * https://github.com/draede/cx
 * 
 * Copyright (C) 2014 - 2017 draede - draede [at] outlook [dot] com
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

#include "CX/DB/SQLite/DBHelper.hpp"


namespace CX
{

namespace DB
{

namespace SQLite
{

DBHelper::StatementScope::StatementScope(DBHelper *pDBHelper, Size cIndex)
{
	Status status;

	m_pDBHelper = pDBHelper;
	m_cIndex    = cIndex;
	if ((status = pDBHelper->AcquireStatement(cIndex, &m_pStatement)).IsNOK())
	{
		m_pStatement = NULL;
	}
}

DBHelper::StatementScope::~StatementScope()
{
	if (NULL != m_pStatement)
	{
		m_pDBHelper->ReleaseStatement(m_pStatement);
	}
}

DBHelper *DBHelper::StatementScope::GetDBHelper()
{
	return m_pDBHelper;
}

Statement *DBHelper::StatementScope::GetStatement()
{
	return m_pStatement;
}

Size DBHelper::StatementScope::GetIndex()
{
	return m_cIndex;
}

DBHelper::DBHelper()
{
	m_cMaxAsyncOperations   = MAX_ASYNC_OPERATIONS;
	m_cMaxAsyncFlushTimeout = MAX_ASYNC_FLUSH_TIMEOUT;
	m_hAsyncThread          = NULL;
	m_hStopEvent            = NULL;
	m_hFlushEvent           = NULL;
}

DBHelper::DBHelper(const Char *szPath, 
                   Size cMaxAsyncOperations/* = MAX_ASYNC_OPERATIONS*/, 
                   Size cMaxAsyncFlushTimeout/* = MAX_ASYNC_FLUSH_TIMEOUT*/, 
                   unsigned int nFlags/* = DB::SQLite::Database::OPEN_DEFAULT*/)
{
	m_initStatus = Open(szPath, cMaxAsyncOperations, cMaxAsyncFlushTimeout, nFlags);
}

DBHelper::~DBHelper()
{
	m_cMaxAsyncOperations   = MAX_ASYNC_OPERATIONS;
	m_cMaxAsyncFlushTimeout = MAX_ASYNC_FLUSH_TIMEOUT;
	m_hAsyncThread          = NULL;
	m_hStopEvent            = NULL;
	m_hFlushEvent           = NULL;
	Close();
}

Status DBHelper::Open(const Char *szPath, 
                      Size cMaxAsyncOperations/* = MAX_ASYNC_OPERATIONS*/,
                      Size cMaxAsyncFlushTimeout/* = MAX_ASYNC_FLUSH_TIMEOUT*/,
                      unsigned int nFlags/* = DB::SQLite::Database::OPEN_DEFAULT*/)
{
	DWORD  dwID;
	Status status;

	Close();
	if ((status = OnPreOpen()).IsNOK())
	{
		return status;
	}
	m_cMaxAsyncOperations   = cMaxAsyncOperations;
	m_cMaxAsyncFlushTimeout = cMaxAsyncFlushTimeout;
	if ((status = m_db.Open(szPath, nFlags)).IsNOK())
	{
		OnPostOpen(status);

		return status;
	}
	if ((status = m_db.Exec(GetDDL())).IsNOK())
	{
		m_db.Close();
		OnPostOpen(status);

		return status;
	}
	if (NULL == (m_hStopEvent = ::CreateEventW(NULL, FALSE, FALSE, NULL)))
	{
		m_db.Close();
		OnPostOpen(status);

		return Status(Status_OperationFailed, "Failed to create event");
	}
	if (NULL == (m_hFlushEvent = ::CreateEventW(NULL, FALSE, FALSE, NULL)))
	{
		CloseHandle(m_hStopEvent);
		m_hStopEvent = NULL;
		m_db.Close();
		OnPostOpen(status);

		return Status(Status_OperationFailed, "Failed to create event");
	}
	if (NULL == (m_hAsyncThread = ::CreateThread(NULL, 0, &DBHelper::AsyncThread, this, 0, &dwID)))
	{
		CloseHandle(m_hFlushEvent);
		m_hFlushEvent = NULL;
		CloseHandle(m_hStopEvent);
		m_hStopEvent = NULL;
		m_db.Close();
		OnPostOpen(status);

		return Status(Status_OperationFailed, "Failed to create thread");
	}
	OnPostOpen(Status());

	return Status();
}

Status DBHelper::Close()
{
	Statement   *pStatement;
	Status      status;

	if (!m_db.IsOK())
	{
		return Status_NotInitialized;
	}
	if ((status = OnPreClose()).IsNOK())
	{
		return status;
	}
	if (NULL != m_hAsyncThread)
	{
		::SetEvent(m_hStopEvent);
		WaitForSingleObject(m_hAsyncThread, INFINITE);
		CloseHandle(m_hAsyncThread);
		CloseHandle(m_hStopEvent);
		CloseHandle(m_hFlushEvent);
		m_hAsyncThread = NULL;
		m_hStopEvent   = NULL;
	}
	if (!m_mapUsedStatements.empty())
	{
		OnPostClose(Status_Busy);

		return Status_Busy;
	}
	{
		Sys::Locker locker(&m_lockStatements);

		for (auto iter = m_mapAvailStatements.begin(); iter != m_mapAvailStatements.end(); ++iter)
		{
			while (!iter->second.empty())
			{
				pStatement = iter->second.front();
				iter->second.pop();
				pStatement->Close();
				delete pStatement;
			}
		}
		m_mapAvailStatements.clear();
	}
	m_db.Close();
	OnPostClose(Status());

	return Status();
}

bool DBHelper::IsOK() const
{
	return m_db.IsOK();
}

Database &DBHelper::GetDB()
{
	return m_db;
}

const Database &DBHelper::GetDB() const
{
	return m_db;
}

Status DBHelper::GetInitStatus() const
{
	return m_initStatus;
}

Bindings *DBHelper::CreateBindings()
{
	return new (std::nothrow) Bindings();
}

Bindings *DBHelper::CreateBindings(const Char *szArgsType, ...)
{
	Bindings *pBindings;
	va_list  vl;

	va_start(vl, szArgsType);
	pBindings = CreateBindings(szArgsType, vl);
	va_end(vl);

	return pBindings;
}

Bindings *DBHelper::CreateBindings(const Char *szArgsType, va_list vl)
{
	return new (std::nothrow) Bindings(szArgsType, vl);
}

void DBHelper::DestroyBindings(Bindings *pBindings)
{
	delete pBindings;
}

Status DBHelper::AddAsyncOperation(Size cStatementIndex, Bindings *pBindings)
{
	Sys::Locker locker(&m_lockOperations);
	Operation   oper;

	if (!m_db.IsOK())
	{
		return Status_NotInitialized;
	}
	oper.cStatementIndex = cStatementIndex;
	oper.pBindings       = pBindings;
	m_queueOperations.push(oper);
	if (m_queueOperations.size() >= m_cMaxAsyncOperations)
	{
		::SetEvent(m_hFlushEvent);
	}

	return Status();
}

Status DBHelper::FlushAsyncOperations()
{
	if (!m_db.IsOK())
	{
		return Status_NotInitialized;
	}
	::SetEvent(m_hFlushEvent);

	return Status();
}

Status DBHelper::AcquireStatement(Size cIndex, DB::SQLite::Statement **ppStatement)
{
	Sys::Locker                  locker(&m_lockStatements);
	AvailStatementsMap::iterator iterAvailStatements;

	if (!m_db.IsOK())
	{
		return Status_NotInitialized;
	}
	if (cIndex >= GetStatementsCount())
	{
		return Status_InvalidArg;
	}
	if (m_mapAvailStatements.end() != (iterAvailStatements = m_mapAvailStatements.find(cIndex)))
	{
		*ppStatement = iterAvailStatements->second.front();
		iterAvailStatements->second.pop();
	}
	else
	{
		*ppStatement = new (std::nothrow) Statement(&m_db, GetStatements()[cIndex]);

		if (NULL == *ppStatement)
		{
			return Status_MemAllocFailed;
		}
		if ((*ppStatement)->GetInitStatus().IsNOK())
		{
			Status status = (*ppStatement)->GetInitStatus();

			delete *ppStatement;

			return status;
		}
	}
	m_mapUsedStatements[*ppStatement] = cIndex;

	return Status();
}

Status DBHelper::ReleaseStatement(DB::SQLite::Statement *pStatement)
{
	Sys::Locker                  locker(&m_lockStatements);
	AvailStatementsMap::iterator iterAvailStatements;
	UsedStatementsMap::iterator  iterUsedStatements;

	if (!m_db.IsOK())
	{
		return Status_NotInitialized;
	}
	if (m_mapUsedStatements.end() == (iterUsedStatements = m_mapUsedStatements.find(pStatement)))
	{
		return Status_NotFound;
	}
	if (m_mapAvailStatements.end() != (iterAvailStatements = m_mapAvailStatements.find(iterUsedStatements->second)))
	{
		iterAvailStatements->second.push(pStatement);
	}
	else
	{
		m_mapAvailStatements[iterUsedStatements->second].push(pStatement);
	}

	return Status();
}

DWORD WINAPI DBHelper::AsyncThread(void *pArgs)
{
	DBHelper *pThis = (DBHelper *)pArgs;

	return pThis->AsyncThreadProc();
}

DWORD DBHelper::AsyncThreadProc()
{
	HANDLE events[] = { m_hStopEvent, m_hFlushEvent };
	DWORD  dwWait;
	Status status;

	for (;;)
	{
		dwWait = WaitForMultipleObjects((DWORD)sizeof(events) / sizeof(events[0]), events, FALSE, 
		                                (DWORD)m_cMaxAsyncFlushTimeout);
		{
			Transaction tr(&m_db);

			Sys::Locker locker(&m_lockOperations);

			while (!m_queueOperations.empty())
			{
				Operation oper = m_queueOperations.front();
				m_queueOperations.pop();
				
				{
					StatementScope scope(this, oper.cStatementIndex);

					if (NULL != scope.GetStatement())
					{
						if ((status = scope.GetStatement()->Bind(*oper.pBindings)).IsOK())
						{
							scope.GetStatement()->Step();
							scope.GetStatement()->Reset();
						}
					}
				}

				delete oper.pBindings;
			}
		}
		if (WAIT_OBJECT_0 == dwWait)
		{
			break;
		}
	}

	return 0;
}

}//namespace SQLite

}//namespace DB

}//namespace CX

