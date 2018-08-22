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

#include "CX/DB/SQLite/DefaultDBHelperAsyncMgr.hpp"
#include <functional>


namespace CX
{

namespace DB
{

namespace SQLite
{

DefaultDBHelperAsyncMgr::DefaultDBHelperAsyncMgr()
{
	m_cMaxAsyncOperations   = MAX_ASYNC_OPERATIONS;
	m_cMaxAsyncFlushTimeout = MAX_ASYNC_FLUSH_TIMEOUT;
	m_bFlushAll             = False;
	m_eventStop.Create();
	m_eventFlush.Create();
}

DefaultDBHelperAsyncMgr::~DefaultDBHelperAsyncMgr()
{
	Stop();

	m_eventFlush.Destroy();
	m_eventStop.Destroy();
}

Status DefaultDBHelperAsyncMgr::Start(Size cMaxAsyncOperations/* = DBHelper::MAX_ASYNC_OPERATIONS*/,
                                      UInt32 cMaxAsyncFlushTimeout/* = DBHelper::MAX_ASYNC_FLUSH_TIMEOUT*/)
{
	if (m_thread.IsRunning())
	{
		return Status_Busy;
	}
	m_cMaxAsyncOperations   = cMaxAsyncOperations;
	m_cMaxAsyncFlushTimeout = cMaxAsyncFlushTimeout;
	m_bFlushAll             = False;
	m_setFlushDBHelpers.clear();
	m_mapOperations.clear();

	Status   status;

	if ((status = m_thread.Run(std::bind(&DefaultDBHelperAsyncMgr::AsyncOperationsThread, this))).IsNOK())
	{
		return status;
	}

	return Status();
}

Status DefaultDBHelperAsyncMgr::Stop()
{
	if (!m_thread.IsRunning())
	{
		return Status_InvalidCall;
	}
	m_eventStop.Set();
	m_thread.Wait();
	m_cMaxAsyncOperations   = MAX_ASYNC_OPERATIONS;
	m_cMaxAsyncFlushTimeout = MAX_ASYNC_FLUSH_TIMEOUT;
	m_bFlushAll             = False;
	m_setFlushDBHelpers.clear();
	m_mapOperations.clear();

	return Status();
}

Bool DefaultDBHelperAsyncMgr::IsRunning()
{
	return m_thread.IsRunning();
}

Bindings *DefaultDBHelperAsyncMgr::CreateBindings()
{
	return new (std::nothrow) Bindings();
}

Bindings *DefaultDBHelperAsyncMgr::CreateBindings(const Char *szArgsType, ...)
{
	Bindings *pBindings;
	va_list  vl;

	va_start(vl, szArgsType);
	pBindings = CreateBindings(szArgsType, vl);
	va_end(vl);

	return pBindings;
}

Bindings *DefaultDBHelperAsyncMgr::CreateBindings(const Char *szArgsType, va_list vl)
{
	return new (std::nothrow) Bindings(szArgsType, vl);
}

void DefaultDBHelperAsyncMgr::DestroyBindings(Bindings *pBindings)
{
	delete pBindings;
}

//pBindings must be create with CreateBindings (after this call the ownership of pBindings is taken by DBHelper)
//pAsyncOperationHandler will be released with pAsyncOperationHandler->Release()
Status DefaultDBHelperAsyncMgr::AddAsyncOperation(DBHelper *pDBHelper, Size cStatementIndex, Bindings *pBindings,
                                                  DBHelper::IAsyncOperationHandler *pAsyncOperationHandler/* = NULL*/)
{
	if (!m_thread.IsRunning())
	{
		return Status_InvalidCall;
	}

	DBHelper::Operation        oper;
	DBHelper::OperationsVector vectorOperations;

	oper.cStatementIndex = cStatementIndex;
	oper.pBindings       = pBindings;
	vectorOperations.push_back(oper);

	return AddAsyncOperations(pDBHelper, vectorOperations, pAsyncOperationHandler);
}

Status DefaultDBHelperAsyncMgr::AddAsyncOperations(DBHelper *pDBHelper, 
                                                   const DBHelper::OperationsVector &vectorOperations,
                                                   DBHelper::IAsyncOperationHandler *pAsyncOperationHandler/* = NULL*/)
{
	Sys::FastWLocker   locker(&m_frwLock);
	OperationsBatch    batch;

	if (!m_thread.IsRunning())
	{
		return Status_InvalidCall;
	}
	if (vectorOperations.empty())
	{
		return Status_InvalidArg;
	}
	batch.vectorOperations       = vectorOperations;
	batch.pAsyncOperationHandler = pAsyncOperationHandler;
	m_mapOperations[pDBHelper].vectorOperationsBatches.push_back(batch);
	m_mapOperations[pDBHelper].cTotalCount += vectorOperations.size();
	if (m_mapOperations[pDBHelper].cTotalCount >= m_cMaxAsyncOperations)
	{
		m_setFlushDBHelpers.insert(pDBHelper);
		m_eventFlush.Set();
	}

	return Status();
}

Status DefaultDBHelperAsyncMgr::FlushAsyncOperations(DBHelper *pDBHelper)
{
	Sys::FastWLocker   locker(&m_frwLock);

	m_setFlushDBHelpers.insert(pDBHelper);
	m_eventFlush.Set();

	return Status();
}

Status DefaultDBHelperAsyncMgr::FlushAsyncOperations()
{
	Sys::FastWLocker   locker(&m_frwLock);

	m_bFlushAll = True;
	m_eventFlush.Set();

	return Status();
}

void DefaultDBHelperAsyncMgr::AsyncOperationsThread()
{
	Sys::Event             *events[2] = { &m_eventStop, &m_eventFlush };
	Sys::Event::WaitResult nWaitRes;
	Size                   cEventIndex;
	Status                 status;

	for (;;)
	{
		nWaitRes = Sys::Event::WaitForMultipleEvents(events, 2, false, &cEventIndex, m_cMaxAsyncFlushTimeout);
		if (Sys::Event::Wait_OK != nWaitRes)
		{
			continue;
		}

		{
			Sys::FastWLocker   locker(&m_frwLock);

			auto iterHelpers = m_mapOperations.begin();

			while (m_mapOperations.end() != iterHelpers)
			{
				if (0 == cEventIndex || m_bFlushAll || 
				    m_setFlushDBHelpers.end() != m_setFlushDBHelpers.find(iterHelpers->first))
				{
					Transaction   tr(&iterHelpers->first->GetDB());

					if (0 < iterHelpers->second.cTotalCount)
					{
						for (auto iterBatches = iterHelpers->second.vectorOperationsBatches.begin(); 
						     iterBatches != iterHelpers->second.vectorOperationsBatches.end(); ++iterBatches)
						{
							Status   statusTmp;

							status.Clear();
							for (auto iterOpers = iterBatches->vectorOperations.begin();
							     iterOpers != iterBatches->vectorOperations.end(); ++iterOpers)
							{
								DBHelper::StatementScope   scope(iterHelpers->first, iterOpers->cStatementIndex);

								if (NULL != scope.GetStatement())
								{
									if ((statusTmp = scope.GetStatement()->Bind(*iterOpers->pBindings)).IsOK())
									{
										if (Statement::Result_Done == scope.GetStatement()->Step())
										{
											statusTmp = scope.GetStatement()->Reset();
										}
									}
								}
								else
								{
									statusTmp = Status(Status_OperationFailed, "Failed to get statement");
								}
								if (!statusTmp)
								{
									status = statusTmp;
								}

								DestroyBindings(iterOpers->pBindings);
							}

							if (NULL != iterBatches->pAsyncOperationHandler)
							{
								iterBatches->pAsyncOperationHandler->OnCompletion(status);
								iterBatches->pAsyncOperationHandler->Release();
							}
						}
					}
					iterHelpers = m_mapOperations.erase(iterHelpers);
				}
				else
				{
					iterHelpers++;
				}
			}
			m_bFlushAll = False;
		}

		if (0 == cEventIndex)
		{
			break;
		}
	}
}
}//namespace SQLite

}//namespace DB

}//namespace CX
