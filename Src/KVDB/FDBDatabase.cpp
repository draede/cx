/* 
 * CX - C++ framework for general purpose developement
 *
 * https://github.com/draede/cx
 * 
 * Copyright (C) 2014-2015 draede - draede [at] outlook [dot] com
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

#include "CX/KVDB/FDBDatabase.hpp"
#include "CX/KVDB/FDBTable.hpp"
#include "FDBStatus.hpp"
#include "libforestdb/forestdb.h"


namespace CX
{

namespace KVDB
{

const FDBDatabaseConfig   FDBDatabase::DEFAULT_DB_CONFIG;
const FDBTableConfig      FDBDatabase::DEFAULT_TABLE_CONFIG;

FDBDatabase::FDBDatabase()
{
	m_pDB = NULL;
}

FDBDatabase::~FDBDatabase()
{
	Close();
}

Status FDBDatabase::Open(const Char *szPath, const IDatabaseConfig &config)
{
	Close();

	const FDBDatabaseConfig &conf = dynamic_cast<const FDBDatabaseConfig &>(config);

	fdb_config cfg;
	fdb_status nStatus;

	cfg = fdb_get_default_config();
	if (conf.m_bReadOnly)
	{
		cfg.flags   = FDB_OPEN_FLAG_RDONLY;
		m_bReadOnly = true;
	}
	else
	{
		cfg.flags   = FDB_OPEN_FLAG_CREATE;
		m_bReadOnly = false;
	}
	cfg.buffercache_size = conf.m_cbBufferCacheSize;
	if (FDBDatabaseConfig::Durability_None == conf.m_nDurability)
	{
		cfg.durability_opt = FDB_DRB_ASYNC;
	}
	else
	if (FDBDatabaseConfig::Durability_Direct == conf.m_nDurability)
	{
		cfg.durability_opt = FDB_DRB_ODIRECT;
	}
	else
	if (FDBDatabaseConfig::Durability_DirectAsync == conf.m_nDurability)
	{
		cfg.durability_opt = FDB_DRB_ODIRECT_ASYNC;
	}
	else
	{
		cfg.durability_opt = FDB_DRB_ASYNC;
	}
	if (FDB_RESULT_SUCCESS != (nStatus = fdb_open((fdb_file_handle **)&m_pDB, szPath, &cfg)))
	{
		m_pDB = NULL;

		return FDBStatus(nStatus);
	}
	m_sPath = szPath;

	return Status();
}

Status FDBDatabase::Close()
{
	if (NULL != m_pDB)
	{
		fdb_status nStatus;

		if (FDB_RESULT_SUCCESS != (nStatus = fdb_close((fdb_file_handle *)m_pDB)))
		{
			return FDBStatus(nStatus);
		}
		m_pDB = NULL;
		m_sPath.clear();
	}

	return Status();
}

Status FDBDatabase::OpenTable(ITable **ppTable, const ITableConfig &config, const Char *szName)
{
	if (NULL == m_pDB)
	{
		return Status(Status_InvalidCall, "Database is not opened");
	}

	const FDBTableConfig &conf = dynamic_cast<const FDBTableConfig &>(config);

	fdb_kvs_handle   *pKV;
	fdb_kvs_config   cfg;
	fdb_status       nStatus;

	cfg = fdb_get_default_kvs_config();
	if (m_bReadOnly)
	{
		cfg.create_if_missing = false;
	}
	else
	{
		cfg.create_if_missing = conf.m_bCreateIfMissing;
	}
	if (NULL != conf.m_bCreateIfMissing)
	{
		cfg.custom_cmp = conf.m_pfnKeyComparator;
	}
	if (FDB_RESULT_SUCCESS != (nStatus = fdb_kvs_open((fdb_file_handle *)m_pDB, &pKV, szName, &cfg)))
	{
		return FDBStatus(nStatus);
	}
	if (NULL == (*ppTable = new FDBTable(this, pKV)))
	{
		fdb_kvs_close(pKV);

		return Status(Status_MemAllocFailed, "Failed to allocate table");
	}

	return Status();
}

Status FDBDatabase::CloseTable(ITable *pTable)
{
	if (NULL == m_pDB)
	{
		return Status(Status_InvalidCall, "Database is not opened");
	}

	FDBTable *pRealTable = dynamic_cast<FDBTable *>(pTable);

	if (NULL == pRealTable)
	{
		return Status(Status_InvalidArg, "Not a valid table");
	}

	fdb_status   nStatus;

	if (FDB_RESULT_SUCCESS != (nStatus = fdb_kvs_close((fdb_kvs_handle *)pRealTable->GetKV())))
	{
		return FDBStatus(nStatus);
	}
	delete pRealTable;

	return Status();
}

Status FDBDatabase::DropTable(const Char *szName)
{
	if (NULL == m_pDB)
	{
		return Status(Status_InvalidCall, "Database is not opened");
	}

	fdb_status   nStatus;

	if (FDB_RESULT_SUCCESS != (nStatus = fdb_kvs_remove((fdb_file_handle *)m_pDB, szName)))
	{
		return FDBStatus(nStatus);
	}

	return Status();
}

Status FDBDatabase::BeginTransaction()
{
	if (NULL == m_pDB)
	{
		return Status(Status_InvalidCall, "Database is not opened");
	}

	fdb_status   nStatus;

	if (FDB_RESULT_SUCCESS != (nStatus = fdb_begin_transaction((fdb_file_handle *)m_pDB, FDB_ISOLATION_READ_COMMITTED)))
	{
		return FDBStatus(nStatus);
	}

	return Status();
}

Status FDBDatabase::EndTransaction(bool bAbort/* = false*/)
{
	if (NULL == m_pDB)
	{
		return Status(Status_InvalidCall, "Database is not opened");
	}

	fdb_status   nStatus;

	if (!bAbort)
	{
		if (FDB_RESULT_SUCCESS != (nStatus = fdb_end_transaction((fdb_file_handle *)m_pDB, FDB_COMMIT_NORMAL)))
		{
			return FDBStatus(nStatus);
		}
	}
	else
	{
		if (FDB_RESULT_SUCCESS != (nStatus = fdb_abort_transaction((fdb_file_handle *)m_pDB)))
		{
			return FDBStatus(nStatus);
		}
	}

	return Status();
}

Status FDBDatabase::Commit()
{
	if (NULL == m_pDB)
	{
		return Status(Status_InvalidCall, "Database is not opened");
	}

	fdb_status   nStatus;

	if (FDB_RESULT_SUCCESS != (nStatus = fdb_commit((fdb_file_handle *)m_pDB, FDB_COMMIT_NORMAL)))
	{
		return FDBStatus(nStatus);
	}

	return Status();
}

bool FDBDatabase::IsOK() const
{
	return (NULL != m_pDB);
}

const Char *FDBDatabase::GetPath() const
{
	return m_sPath.c_str();
}

bool FDBDatabase::IsReadOnly() const
{
	return m_bReadOnly;
}

}//namespace KVDB

}//namespace CX

