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
#include "CX/DB/SQLite/Database.hpp"
#include "CX/DB/SQLite/Statement.hpp"
#include "CX/Print.hpp"
#include "sqlite3.h"


#define SQLSTATUS(status, func, ret)                                                                                   \
Status(status, "{1} failed with code {2}, message '{3}'", func, ret, sqlite3_errstr(ret))


namespace CX
{

namespace DB
{

namespace SQLite
{

const unsigned int Database::OPEN_READONLY       = SQLITE_OPEN_READONLY;
const unsigned int Database::OPEN_READWRITE      = SQLITE_OPEN_READWRITE;
const unsigned int Database::OPEN_CREATE         = SQLITE_OPEN_CREATE;
const unsigned int Database::OPEN_DELETEONCLOSE  = SQLITE_OPEN_DELETEONCLOSE;
const unsigned int Database::OPEN_EXCLUSIVE      = SQLITE_OPEN_EXCLUSIVE;
const unsigned int Database::OPEN_AUTOPROXY      = SQLITE_OPEN_AUTOPROXY;
const unsigned int Database::OPEN_URI            = SQLITE_OPEN_URI;
const unsigned int Database::OPEN_MEMORY         = SQLITE_OPEN_MEMORY;
const unsigned int Database::OPEN_MAIN_DB        = SQLITE_OPEN_MAIN_DB;
const unsigned int Database::OPEN_TEMP_DB        = SQLITE_OPEN_TEMP_DB;
const unsigned int Database::OPEN_TRANSIENT_DB   = SQLITE_OPEN_TRANSIENT_DB;
const unsigned int Database::OPEN_MAIN_JOURNAL   = SQLITE_OPEN_MAIN_JOURNAL;
const unsigned int Database::OPEN_TEMP_JOURNAL   = SQLITE_OPEN_TEMP_JOURNAL;
const unsigned int Database::OPEN_SUBJOURNAL     = SQLITE_OPEN_SUBJOURNAL;
const unsigned int Database::OPEN_MASTER_JOURNAL = SQLITE_OPEN_MASTER_JOURNAL;
const unsigned int Database::OPEN_NOMUTEX        = SQLITE_OPEN_NOMUTEX;
const unsigned int Database::OPEN_FULLMUTEX      = SQLITE_OPEN_FULLMUTEX;
const unsigned int Database::OPEN_SHAREDCACHE    = SQLITE_OPEN_SHAREDCACHE;
const unsigned int Database::OPEN_PRIVATECACHE   = SQLITE_OPEN_PRIVATECACHE;
const unsigned int Database::OPEN_WAL            = SQLITE_OPEN_WAL;
const unsigned int Database::OPEN_DEFAULT        = SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE | SQLITE_OPEN_FULLMUTEX;

const unsigned int Database::VERSION_NUMBER      = SQLITE_VERSION_NUMBER;
const Char         *Database::VERSION_STRING     = SQLITE_VERSION;


Database::Database()
{
	m_pDB = NULL;
}

Database::~Database()
{
	Close();
}

Database::Database(const Char *szPath, unsigned int nFlags/* = OPEN_DEFAULT*/)
{
	m_pDB        = NULL;
	m_initStatus = Open(szPath, nFlags);
}

Database::Database(const WChar *wszPath)
{
	m_pDB        = NULL;
	m_initStatus = Open(wszPath);
}

Status Database::Initialize()
{
	int nRet;

	if (SQLITE_OK != (nRet = sqlite3_initialize()))
	{
		return SQLSTATUS(Status_OperationFailed, "sqlite3_initialize", nRet);
	}

	return Status();
}

Status Database::Shutdown()
{
	int nRet;

	if (SQLITE_OK != (nRet = sqlite3_shutdown()))
	{
		return SQLSTATUS(Status_OperationFailed, "sqlite3_shutdown", nRet);
	}

	return Status();
}

Status Database::Open(const Char *szPath, unsigned int nFlags/* = OPEN_DEFAULT*/)
{
	int    nRet;
	Status status;

	if ((status = Close()).IsNOK())
	{
		return status;
	}
	if (SQLITE_OK != (nRet = sqlite3_open_v2(szPath, (sqlite3 **)&m_pDB, nFlags, NULL)))
	{
		m_pDB = NULL;

		return SQLSTATUS(Status_OpenFailed, "sqlite3_open_v2", nRet);
	}

	return Status();
}

Status Database::Open(const WChar *wszPath)
{
	int    nRet;
	Status status;

	if ((status = Close()).IsNOK())
	{
		return status;
	}
	if (SQLITE_OK != (nRet = sqlite3_open16(wszPath, (sqlite3 **)&m_pDB)))
	{
		m_pDB = NULL;

		return SQLSTATUS(Status_OpenFailed, "sqlite3_open16", nRet);
	}

	return Status();
}

Status Database::Close()
{
	int nRet;

	if (NULL == m_pDB)
	{
		return Status();
	}
	if (SQLITE_OK != (nRet = sqlite3_close((sqlite3 *)m_pDB)))
	{
		return SQLSTATUS(Status_OperationFailed, "sqlite3_close", nRet);
	}
	m_pDB = NULL;

	return Status();
}

bool Database::IsOK() const
{
	return (NULL != m_pDB);
}

Status Database::GetInitStatus() const
{
	return m_initStatus;
}

Status Database::Exec(const Char *szSimpleSQL)
{
	int nRet;

	if (NULL == m_pDB)
	{
		return Status(Status_NotInitialized, "Database was not opened");
	}
	if (SQLITE_OK != (nRet = sqlite3_exec((sqlite3 *)m_pDB, szSimpleSQL, NULL, NULL, NULL)))
	{
		return SQLSTATUS(Status_OperationFailed, "sqlite3_exec", nRet);
	}

	return Status();
}

Int64 Database::GetLastInsertID()
{
	if (NULL == m_pDB)
	{
		return 0;
	}

	return sqlite3_last_insert_rowid((sqlite3 *)m_pDB);
}

Size Database::GetLastExecChangesCount()
{
	if (NULL == m_pDB)
	{
		return 0;
	}

	return (Size)sqlite3_changes((sqlite3 *)m_pDB);
}

Status Database::GetPragma(const Char *szName, Int64 *pnValue)
{
	if (NULL == m_pDB)
	{
		return Status_NotInitialized;
	}

	String sSQL;
	Status status;

	Print(&sSQL, "PRAGMA {1};", szName);

	Statement select(this, sSQL.c_str());

	if (!select.IsOK())
	{
		return select.GetInitStatus();
	}
	if (Statement::Result_Row != select.Step(&status))
	{
		return status;
	}
	*pnValue = select.GetInt(0);

	return Status();
}

Status Database::SetPragma(const Char *szName, Int64 nValue)
{
	if (NULL == m_pDB)
	{
		return Status_NotInitialized;
	}

	String sSQL;
	Status status;

	Print(&sSQL, "PRAGMA {1}={2};", szName, nValue);

	Statement update(this, sSQL.c_str());

	if (!update.IsOK())
	{
		return update.GetInitStatus();
	}
	if (Statement::Result_Done != update.Step(&status))
	{
		return status;
	}

	return Status();
}

Status Database::GetPragma(const Char *szName, Double *plfValue)
{
	if (NULL == m_pDB)
	{
		return Status_NotInitialized;
	}

	String sSQL;
	Status status;

	Print(&sSQL, "PRAGMA {1};", szName);

	Statement select(this, sSQL.c_str());

	if (!select.IsOK())
	{
		return select.GetInitStatus();
	}
	if (Statement::Result_Row != select.Step(&status))
	{
		return status;
	}
	*plfValue = select.GetReal(0);

	return Status();
}

Status Database::SetPragma(const Char *szName, Double lfValue)
{
	if (NULL == m_pDB)
	{
		return Status_NotInitialized;
	}

	String sSQL;
	Status status;

	Print(&sSQL, "PRAGMA {1}={2};", szName, lfValue);

	Statement update(this, sSQL.c_str());

	if (!update.IsOK())
	{
		return update.GetInitStatus();
	}
	if (Statement::Result_Done != update.Step(&status))
	{
		return status;
	}

	return Status();
}

Status Database::GetPragma(const Char *szName, String *psValue)
{
	if (NULL == m_pDB)
	{
		return Status_NotInitialized;
	}

	String sSQL;
	Status status;

	Print(&sSQL, "PRAGMA {1};", szName);

	Statement select(this, sSQL.c_str());

	if (!select.IsOK())
	{
		return select.GetInitStatus();
	}
	if (Statement::Result_Row != select.Step(&status))
	{
		return status;
	}
	psValue->assign(select.GetString(0), select.GetStringLen(0));

	return Status();
}

Status Database::SetPragma(const Char *szName, const Char *szValue)
{
	if (NULL == m_pDB)
	{
		return Status_NotInitialized;
	}

	String sSQL;
	Status status;

	Print(&sSQL, "PRAGMA {1}=\"{2}\";", szName, szValue);

	Statement update(this, sSQL.c_str());

	if (!update.IsOK())
	{
		return update.GetInitStatus();
	}
	if (Statement::Result_Done != update.Step(&status))
	{
		return status;
	}

	return Status();
}

void *Database::GetHandle() const
{
	return m_pDB;
}

}//namespace SQLite

}//namespace DB

}//namespace CX
