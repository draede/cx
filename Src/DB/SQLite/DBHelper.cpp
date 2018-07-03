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
}

DBHelper::DBHelper(const Char *szPath, 
                   Size cMaxAsyncOperations/* = MAX_ASYNC_OPERATIONS*/, 
                   UInt32 cMaxAsyncFlushTimeout/* = MAX_ASYNC_FLUSH_TIMEOUT*/, 
                   unsigned int nFlags/* = DB::SQLite::Database::OPEN_DEFAULT*/)
{
	m_initStatus            = Open(szPath, cMaxAsyncOperations, cMaxAsyncFlushTimeout, nFlags);
	m_cAsyncOperationsCount = 0;
}

DBHelper::~DBHelper()
{
	m_cMaxAsyncOperations   = MAX_ASYNC_OPERATIONS;
	m_cMaxAsyncFlushTimeout = MAX_ASYNC_FLUSH_TIMEOUT;
	Close();
}

Status DBHelper::Open(const Char *szPath, 
                      Size cMaxAsyncOperations/* = MAX_ASYNC_OPERATIONS*/,
                      UInt32 cMaxAsyncFlushTimeout/* = MAX_ASYNC_FLUSH_TIMEOUT*/,
                      unsigned int nFlags/* = DB::SQLite::Database::OPEN_DEFAULT*/)
{
	Status status;

	Close();

	m_cAsyncOperationsCount = 0;
	if ((status = OnPreOpen()).IsNOK())
	{
		return status;
	}
	m_cMaxAsyncOperations   = cMaxAsyncOperations;
	m_cMaxAsyncFlushTimeout = cMaxAsyncFlushTimeout;
	if ((status = m_db.Open(szPath, nFlags)).IsNOK())
	{
		return status;
	}
	if ((status = m_db.Exec(GetDDL())).IsNOK())
	{
		m_db.Close();

		return status;
	}
	if ((status = OnPostOpen()).IsNOK())
	{
		m_db.Close();

		return status;
	}
	if ((status = m_eventStop.Create(false, false)).IsNOK())
	{
		m_db.Close();

		return status;
	}
	if ((status = m_eventFlush.Create(false, false)).IsNOK())
	{
		m_eventStop.Destroy();
		m_db.Close();

		return status;
	}
	if ((status = m_asyncThread.Run(std::bind(&DBHelper::AsyncOperationsThread, this))).IsNOK())
	{
		m_eventFlush.Destroy();
		m_eventStop.Destroy();
		m_db.Close();

		return status;
	}

	return Status();
}

Status DBHelper::Close()
{
	Status      status;

	if (!m_db.IsOK())
	{
		return Status_NotInitialized;
	}
	if ((status = OnPreClose()).IsNOK())
	{
		return status;
	}
	if (m_asyncThread.IsRunning())
	{
		m_eventStop.Set();
		m_asyncThread.Wait();
		m_eventStop.Destroy();
		m_eventFlush.Destroy();
	}
	if (!m_mapUsedStatements.empty())
	{
		return Status_Busy;
	}
	{
		Sys::Locker locker(&m_lockStatements);

		for (AvailStatementsMap::iterator iter = m_mapAvailStatements.begin(); iter != m_mapAvailStatements.end(); ++iter)
		{
			while (!iter->second.empty())
			{
				Statement   *pStatement;
				
				pStatement = iter->second.front();
				iter->second.pop();
				pStatement->Close();
				delete pStatement;
			}
		}
		m_mapAvailStatements.clear();
	}
	m_db.Close();
	OnPostClose();
	m_cAsyncOperationsCount = 0;

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

Status DBHelper::AddAsyncOperation(Size cStatementIndex, Bindings *pBindings,
                                   IAsyncOperationHandler *pAsyncOperationHandler/* = NULL*/)
{
	Operation        oper;
	OperationsVector vectorOperations;

	oper.cStatementIndex = cStatementIndex;
	oper.pBindings       = pBindings;
	vectorOperations.push_back(oper);

	return AddAsyncOperations(vectorOperations, pAsyncOperationHandler);
}

Status DBHelper::AddAsyncOperations(const OperationsVector &vectorOperations,
                                    IAsyncOperationHandler *pAsyncOperationHandler/* = NULL*/)
{
	Sys::Locker      locker(&m_lockOperationBatches);
	OperationBatch   batch;

	if (!m_db.IsOK())
	{
		return Status_NotInitialized;
	}
	if (vectorOperations.empty())
	{
		return Status_InvalidArg;
	}
	batch.vectorOperations       = vectorOperations;
	batch.pAsyncOperationHandler = pAsyncOperationHandler;
	m_queueOperationBatches.push(batch);
	m_cAsyncOperationsCount += vectorOperations.size();
	if (m_cAsyncOperationsCount >= m_cMaxAsyncOperations)
	{
		m_eventFlush.Set();
	}

	return Status();
}

Status DBHelper::FlushAsyncOperations()
{
	if (!m_db.IsOK())
	{
		return Status_NotInitialized;
	}
	m_eventFlush.Set();

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
		if (iterAvailStatements->second.empty())
		{
			m_mapAvailStatements.erase(iterAvailStatements);
		}
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
	m_mapUsedStatements.erase(iterUsedStatements);
	pStatement->Reset();

	return Status();
}

void DBHelper::AsyncOperationsThread()
{
	Sys::Event             *events[2] = { &m_eventStop, &m_eventFlush };
	Sys::Event::WaitResult nWaitRes;
	Size                   cEventIndex;
	Status                 status;

	for (;;)
	{
		nWaitRes = Sys::Event::WaitForMultipleEvents(events, 2, false, &cEventIndex, m_cMaxAsyncFlushTimeout);
		{
			Transaction tr(&m_db);

			Sys::Locker locker(&m_lockOperationBatches);

			while (!m_queueOperationBatches.empty())
			{
				OperationBatch batch = m_queueOperationBatches.front();
				m_queueOperationBatches.pop();
				
				for (OperationsVector::iterator iter = batch.vectorOperations.begin(); 
				     iter != batch.vectorOperations.end(); ++iter)
				{
					StatementScope scope(this, iter->cStatementIndex);

					if (NULL != scope.GetStatement())
					{
						if ((status = scope.GetStatement()->Bind(*iter->pBindings)).IsOK())
						{
							if (Statement::Result_Done == scope.GetStatement()->Step())
							{
								status = scope.GetStatement()->Reset();
							}
						}
					}
					else
					{
						status = Status(Status_OperationFailed, "Failed to get statement");
					}

					delete iter->pBindings;
					
					if (!status)
					{
						break;
					}

					if (0 < m_cAsyncOperationsCount)
					{
						m_cAsyncOperationsCount--;
					}
				}

				if (NULL != batch.pAsyncOperationHandler)
				{
					batch.pAsyncOperationHandler->OnCompletion(status);
					batch.pAsyncOperationHandler->Release();
				}
			}
		}
		if (Sys::Event::Wait_OK == nWaitRes && 0 == cEventIndex)
		{
			break;
		}
	}
}

}//namespace SQLite

}//namespace DB

}//namespace CX

