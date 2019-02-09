/* 
 * CX - C++ framework for general purpose development
 *
 * https://github.com/draede/cx
 * 
 * Copyright (C) 2014 - 2019 draede - draede [at] outlook [dot] com
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
#include "CX/DB/SQLite/DBHelper.hpp"
#include "CX/DB/SQLite/DefaultDBHelperAsyncMgr.hpp"


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
	m_pAsyncMgr             = NULL;
	m_bAsyncMgrOwnership    = False;
}

DBHelper::DBHelper(const Char *szPath, IDBHelperAsyncMgr *pAsyncMgr/* = NULL*/, 
                   unsigned int nFlags/* = DB::SQLite::Database::OPEN_DEFAULT*/)
{
	m_pAsyncMgr          = NULL;
	m_bAsyncMgrOwnership = False;
	m_initStatus         = Open(szPath, pAsyncMgr, nFlags);
}

DBHelper::~DBHelper()
{
	Close();
}

Status DBHelper::Open(const Char *szPath, IDBHelperAsyncMgr *pAsyncMgr/* = NULL*/, 
                      unsigned int nFlags/* = DB::SQLite::Database::OPEN_DEFAULT*/)
{
	Status status;

	Close();

	if ((status = OnPreOpen()).IsNOK())
	{
		return status;
	}
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
	if (NULL == pAsyncMgr)
	{
		m_bAsyncMgrOwnership = True;
		m_pAsyncMgr          = new DefaultDBHelperAsyncMgr();
		if (!(status = m_pAsyncMgr->Start()))
		{
			m_db.Close();
			delete m_pAsyncMgr;
			m_pAsyncMgr          = NULL;
			m_bAsyncMgrOwnership = False;

			return status;
		}
	}
	else
	{
		m_bAsyncMgrOwnership = False;
		m_pAsyncMgr          = pAsyncMgr;
	}

	return Status();
}

Status DBHelper::Close()
{
	Status   status;

	if (!m_db.IsOK())
	{
		return Status_NotInitialized;
	}
	if ((status = OnPreClose()).IsNOK())
	{
		return status;
	}
	if (NULL != m_pAsyncMgr)
	{
		if (m_bAsyncMgrOwnership)
		{
			m_pAsyncMgr->Stop();
			delete m_pAsyncMgr;
			m_pAsyncMgr          = NULL;
			m_bAsyncMgrOwnership = False;
		}
		else
		{
			m_pAsyncMgr->FlushAsyncOperations(this);
			m_pAsyncMgr          = NULL;
			m_bAsyncMgrOwnership = False;
		}
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
	if (!m_db.IsOK())
	{
		return NULL;
	}

	return m_pAsyncMgr->CreateBindings();
}

void DBHelper::DestroyBindings(Bindings *pBindings)
{
	if (!m_db.IsOK())
	{
		return;
	}

	m_pAsyncMgr->DestroyBindings(pBindings);
}

Status DBHelper::AddAsyncOperation(Size cStatementIndex, Bindings *pBindings,
                                   IAsyncOperationHandler *pAsyncOperationHandler/* = NULL*/)
{
	if (!m_db.IsOK())
	{
		return Status_NotInitialized;
	}

	return m_pAsyncMgr->AddAsyncOperation(this, cStatementIndex, pBindings, pAsyncOperationHandler);
}

Status DBHelper::AddAsyncOperations(const OperationsVector &vectorOperations,
                                    IAsyncOperationHandler *pAsyncOperationHandler/* = NULL*/)
{
	if (!m_db.IsOK())
	{
		return Status_NotInitialized;
	}

	return m_pAsyncMgr->AddAsyncOperations(this, vectorOperations, pAsyncOperationHandler);
}

Status DBHelper::FlushAsyncOperations()
{
	if (!m_db.IsOK())
	{
		return Status_NotInitialized;
	}

	return m_pAsyncMgr->FlushAsyncOperations(this);
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

}//namespace SQLite

}//namespace DB

}//namespace CX

