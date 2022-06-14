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
#include "CX/DB/SQLite/Statement.hpp"
#include "sqlite3.h"


#define SQLSTATUS(status, func, ret)                                                                                   \
Status(status, "{1} failed with code {2}, message '{3}'", func, ret, sqlite3_errstr(ret))


namespace CX
{

namespace DB
{

namespace SQLite
{

const unsigned int Statement::TYPE_INTEGER = SQLITE_INTEGER;
const unsigned int Statement::TYPE_REAL    = SQLITE_FLOAT;
const unsigned int Statement::TYPE_BLOB    = SQLITE_BLOB;
const unsigned int Statement::TYPE_NULL    = SQLITE_NULL;
const unsigned int Statement::TYPE_TEXT    = SQLITE_TEXT;

Statement::Statement()
{
	m_pDB        = NULL;
	m_pSTMT      = NULL;
	m_cBindIndex = 1;
}

Statement::Statement(Database *pDB, const Char *szSQL)
{
	m_pDB        = NULL;
	m_pSTMT      = NULL;
	m_cBindIndex = 1;

	m_initStatus = Open(pDB, szSQL);
}

Statement::~Statement()
{
	if (NULL != m_pSTMT)
	{
		sqlite3_finalize((sqlite3_stmt *)m_pSTMT);
	}
}

Status Statement::Open(Database *pDB, const Char *szSQL)
{
	int    nRet;
	Status status;

	if ((status = Close()).IsNOK())
	{
		return status;
	}
	if (SQLITE_OK != (nRet = sqlite3_prepare_v2((sqlite3 *)pDB->GetHandle(), szSQL, -1, (sqlite3_stmt **)&m_pSTMT, 
	                                            NULL)))
	{
		return SQLSTATUS(Status_OpenFailed, "sqlite3_prepare_v2", nRet);
	}
	m_pDB        = pDB;
	m_sSQL       = szSQL;
	m_cBindIndex = 1;

	return Status();
}

Status Statement::Close()
{
	int nRet;

	if (NULL == m_pDB)
	{
		return Status();
	}
	if (SQLITE_OK != (nRet = sqlite3_finalize((sqlite3_stmt *)m_pSTMT)))
	{
		return SQLSTATUS(Status_OperationFailed, "sqlite3_finalize", nRet);
	}
	m_pDB        = NULL;
	m_pSTMT      = NULL;
	m_cBindIndex = 1;
	m_sSQL.clear();

	return Status();
}

Database *Statement::GetDatabase() const
{
	return m_pDB;
}

const Char *Statement::GetSQL() const
{
	return m_sSQL.c_str();
}

const Char *Statement::GetExpandedSQL()
{
	if (NULL == m_pSTMT)
	{
		return NULL;
	}

	return sqlite3_expanded_sql((sqlite3_stmt *)m_pSTMT);
}

bool Statement::IsOK() const
{
	return (NULL != m_pSTMT);
}

Status Statement::GetInitStatus() const
{
	return m_initStatus;
}

void *Statement::GetHandle() const
{
	return m_pSTMT;
}

Size Statement::GetArgsCount()
{
	if (NULL == m_pSTMT)
	{
		return 0;
	}

	return (Size)sqlite3_bind_parameter_count((sqlite3_stmt *)m_pSTMT);
}

Size Statement::GetArgIndex(const Char *szName, Status *pnStatus/* = NULL*/)
{
	if (NULL == m_pSTMT)
	{
		if (NULL != pnStatus)
		{
			*pnStatus = Status_NotInitialized;
		}

		return 0;
	}

	return (Size)sqlite3_bind_parameter_index((sqlite3_stmt *)m_pSTMT, szName);
}

const Char *Statement::GetArgName(Size cIndex, Status *pnStatus/* = NULL*/)
{
	if (NULL == m_pSTMT)
	{
		if (NULL != pnStatus)
		{
			*pnStatus = Status_NotInitialized;
		}

		return NULL;
	}

	return sqlite3_bind_parameter_name((sqlite3_stmt *)m_pSTMT, (int)cIndex);
}

Status Statement::ClearBindings()
{
	int nRet;

	if (NULL == m_pSTMT)
	{
		return Status(Status_NotInitialized, "Statement was not created");
	}

	if (SQLITE_OK != (nRet = sqlite3_clear_bindings((sqlite3_stmt *)m_pSTMT)))
	{
		return SQLSTATUS(Status_OperationFailed, "sqlite3_clear_bindings", nRet);
	}
	m_cBindIndex = 1;

	return Status();
}

Status Statement::BindInt(Int64 nValue)
{
	int nRet;

	if (NULL == m_pSTMT)
	{
		return Status(Status_NotInitialized, "Statement was not created");
	}
	if (SQLITE_OK != (nRet = sqlite3_bind_int64((sqlite3_stmt *)m_pSTMT, (int)m_cBindIndex, nValue)))
	{
		return SQLSTATUS(Status_OperationFailed, "sqlite3_bind_int64", nRet);
	}
	m_cBindIndex++;

	return Status();
}

Status Statement::BindReal(Double lfValue)
{
	int nRet;

	if (NULL == m_pSTMT)
	{
		return Status(Status_NotInitialized, "Statement was not created");
	}
	if (SQLITE_OK != (nRet = sqlite3_bind_double((sqlite3_stmt *)m_pSTMT, (int)m_cBindIndex, lfValue)))
	{
		return SQLSTATUS(Status_OperationFailed, "sqlite3_bind_double", nRet);
	}
	m_cBindIndex++;

	return Status();
}

Status Statement::BindNull()
{
	int nRet;

	if (NULL == m_pSTMT)
	{
		return Status(Status_NotInitialized, "Statement was not created");
	}
	if (SQLITE_OK != (nRet = sqlite3_bind_null((sqlite3_stmt *)m_pSTMT, (int)m_cBindIndex)))
	{
		return SQLSTATUS(Status_OperationFailed, "sqlite3_bind_null", nRet);
	}
	m_cBindIndex++;

	return Status();
}

Status Statement::BindString(const Char *szString, Size cLen/* = (Size)-1*/, 
                             ArgStoreType nArgStoreType/* = ArgStore_Default*/, 
                             FreeArgStoreProc pfnFreeArgStore/* = NULL*/)
{
	sqlite3_destructor_type nDestructorType;
	int                     nRet;

	if (NULL == m_pSTMT)
	{
		return Status(Status_NotInitialized, "Statement was not created");
	}
	if ((Size)-1 == cLen)
	{
		cLen = cx_strlen(szString);
	}
	if (ArgStore_Static == nArgStoreType)
	{
		nDestructorType = SQLITE_STATIC;
	}
	else
	if (ArgStore_Transient == nArgStoreType)
	{
		nDestructorType = SQLITE_TRANSIENT;
	}
	else
	if (ArgStore_Custom == nArgStoreType)
	{
		nDestructorType = pfnFreeArgStore;
	}
	else
	{
		return Status(Status_InvalidArg, "Invalid arg store type");
	}
	if (SQLITE_OK != (nRet = sqlite3_bind_text((sqlite3_stmt *)m_pSTMT, (int)m_cBindIndex, szString, (int)cLen, 
	                                           nDestructorType)))
	{
		return SQLSTATUS(Status_OperationFailed, "sqlite3_bind_text", nRet);
	}
	m_cBindIndex++;

	return Status();
}

Status Statement::BindWString(const WChar *wszString, Size cLen/* = (Size)-1*/, 
                              ArgStoreType nArgStoreType/* = ArgStore_Static*/, 
                              FreeArgStoreProc pfnFreeArgStore/* = NULL*/)
{
	sqlite3_destructor_type nDestructorType;
	int                     nRet;

	if (NULL == m_pSTMT)
	{
		return Status(Status_NotInitialized, "Statement was not created");
	}
	if ((Size)-1 == cLen)
	{
		cLen = cxw_strlen(wszString);
	}
	if (ArgStore_Static == nArgStoreType)
	{
		nDestructorType = SQLITE_STATIC;
	}
	else
	if (ArgStore_Transient == nArgStoreType)
	{
		nDestructorType = SQLITE_TRANSIENT;
	}
	else
	if (ArgStore_Custom == nArgStoreType)
	{
		nDestructorType = pfnFreeArgStore;
	}
	else
	{
		return Status(Status_InvalidArg, "Invalid arg store type");
	}
	if (SQLITE_OK != (nRet = sqlite3_bind_text16((sqlite3_stmt *)m_pSTMT, (int)m_cBindIndex, wszString, 
	                                             (int)(sizeof(WChar) * cLen), nDestructorType)))
	{
		return SQLSTATUS(Status_OperationFailed, "sqlite3_bind_text", nRet);
	}
	m_cBindIndex++;

	return Status();
}

Status Statement::BindBLOB(const void *pData, Size cbSize, ArgStoreType nArgStoreType/* = ArgStore_Static*/, 
                           FreeArgStoreProc pfnFreeArgStore/* = NULL*/)
{
	sqlite3_destructor_type nDestructorType;
	int                     nRet;

	if (NULL == m_pSTMT)
	{
		return Status(Status_NotInitialized, "Statement was not created");
	}
	if (ArgStore_Static == nArgStoreType)
	{
		nDestructorType = SQLITE_STATIC;
	}
	else
	if (ArgStore_Transient == nArgStoreType)
	{
		nDestructorType = SQLITE_TRANSIENT;
	}
	else
	if (ArgStore_Custom == nArgStoreType)
	{
		nDestructorType = pfnFreeArgStore;
	}
	else
	{
		return Status(Status_InvalidArg, "Invalid arg store type");
	}
	if (SQLITE_OK != (nRet = sqlite3_bind_blob((sqlite3_stmt *)m_pSTMT, (int)m_cBindIndex, pData, (int)cbSize,
	                                           nDestructorType)))
	{
		return SQLSTATUS(Status_OperationFailed, "sqlite3_bind_blob", nRet);
	}
	m_cBindIndex++;

	return Status();
}

Status Statement::BindZeroBLOB(Size cbSize)
{
	int nRet;

	if (NULL == m_pSTMT)
	{
		return Status(Status_NotInitialized, "Statement was not created");
	}
	if (SQLITE_OK != (nRet = sqlite3_bind_zeroblob((sqlite3_stmt *)m_pSTMT, (int)m_cBindIndex, (int)cbSize)))
	{
		return SQLSTATUS(Status_OperationFailed, "sqlite3_bind_zeroblob", nRet);
	}
	m_cBindIndex++;

	return Status();
}

Status Statement::Bind(Bindings &bindings)
{
	Size     cCount;
	Status   status;

	if ((status = ClearBindings()).IsNOK())
	{
		return status;
	}
	cCount = bindings.GetArgsCount();
	for (Size i = 0; i < cCount; i++)
	{
		Arg   *pArg = bindings.GetArg(i);

		if (ArgType_Null == pArg->nType)
		{
			if ((status = BindNull()).IsNOK())
			{
				return status;
			}
		}
		else
		if (ArgType_Int == pArg->nType)
		{
			if ((status = BindInt(pArg->nIntValue)).IsNOK())
			{
				return status;
			}
		}
		else
		if (ArgType_Real == pArg->nType)
		{
			if ((status = BindReal(pArg->lfRealValue)).IsNOK())
			{
				return status;
			}
		}
		else
		if (ArgType_String == pArg->nType)
		{
			if ((status = BindString(pArg->str.szString, pArg->str.cLen, pArg->nStoreType, pArg->pfnFreeArgStore)).IsNOK())
			{
				return status;
			}
			pArg->str.szString = NULL;
			pArg->str.cLen     = 0;
		}
		else
		if (ArgType_WString == pArg->nType)
		{
			if ((status = BindWString(pArg->wstr.wszString, pArg->wstr.cLen, pArg->nStoreType, 
			                          pArg->pfnFreeArgStore)).IsNOK())
			{
				return status;
			}
			pArg->wstr.wszString = NULL;
			pArg->wstr.cLen     = 0;
		}
		else
		if (ArgType_BLOB == pArg->nType)
		{
			if ((status = BindBLOB(pArg->blob.pBLOB, pArg->blob.cbSize, pArg->nStoreType, pArg->pfnFreeArgStore)).IsNOK())
			{
				return status;
			}
			pArg->blob.pBLOB  = NULL;
			pArg->blob.cbSize = 0;
		}
		else
		if (ArgType_ZeroBLOB == pArg->nType)
		{
			if ((status = BindZeroBLOB(pArg->cbZeroBLOB)).IsNOK())
			{
				return status;
			}
		}
		else
		{
			return Status_InvalidArg;
		}
	}

	return Status();
}

Size Statement::GetColumnsCount()
{
	if (NULL == m_pSTMT)
	{
		return 0;
	}

	return (Size)sqlite3_column_count((sqlite3_stmt *)m_pSTMT);
}

unsigned int Statement::GetColumnType(Size cIndex, Status *pnStatus/* = NULL*/)
{
	if (NULL == m_pSTMT)
	{
		if (NULL != pnStatus)
		{
			*pnStatus = Status_NotInitialized;
		}

		return 0;
	}

	return (unsigned int)sqlite3_column_type((sqlite3_stmt *)m_pSTMT, (int)cIndex);
}

const Char *Statement::GetColumnDeclaredType(Size cIndex, Status *pnStatus/* = NULL*/)
{
	if (NULL == m_pSTMT)
	{
		if (NULL != pnStatus)
		{
			*pnStatus = Status_NotInitialized;
		}

		return NULL;
	}

	return sqlite3_column_decltype((sqlite3_stmt *)m_pSTMT, (int)cIndex);
}

const Char *Statement::GetColumnName(Size cIndex, Status *pnStatus/* = NULL*/)
{
	if (NULL == m_pSTMT)
	{
		if (NULL != pnStatus)
		{
			*pnStatus = Status_NotInitialized;
		}

		return NULL;
	}

	return sqlite3_column_name((sqlite3_stmt *)m_pSTMT, (int)cIndex);
}

Int64 Statement::GetInt(Size cIndex, Status *pnStatus/* = NULL*/)
{
	if (NULL == m_pSTMT)
	{
		if (NULL != pnStatus)
		{
			*pnStatus = Status_NotInitialized;
		}

		return 0;
	}

	return sqlite3_column_int64((sqlite3_stmt *)m_pSTMT, (int)cIndex);
}

Double Statement::GetReal(Size cIndex, Status *pnStatus/* = NULL*/)
{
	if (NULL == m_pSTMT)
	{
		if (NULL != pnStatus)
		{
			*pnStatus = Status_NotInitialized;
		}

		return 0;
	}

	return sqlite3_column_double((sqlite3_stmt *)m_pSTMT, (int)cIndex);
}

const Char *Statement::GetString(Size cIndex, Status *pnStatus/* = NULL*/)
{
	if (NULL == m_pSTMT)
	{
		if (NULL != pnStatus)
		{
			*pnStatus = Status_NotInitialized;
		}

		return 0;
	}

	return (const Char *)sqlite3_column_text((sqlite3_stmt *)m_pSTMT, (int)cIndex);
}

Size Statement::GetStringLen(Size cIndex, Status *pnStatus/* = NULL*/)
{
	if (NULL == m_pSTMT)
	{
		if (NULL != pnStatus)
		{
			*pnStatus = Status_NotInitialized;
		}

		return 0;
	}

	return (Size)sqlite3_column_bytes((sqlite3_stmt *)m_pSTMT, (int)cIndex);
}

const WChar *Statement::GetWString(Size cIndex, Status *pnStatus/* = NULL*/)
{
	if (NULL == m_pSTMT)
	{
		if (NULL != pnStatus)
		{
			*pnStatus = Status_NotInitialized;
		}

		return 0;
	}

	return (const WChar *)sqlite3_column_text16((sqlite3_stmt *)m_pSTMT, (int)cIndex);
}

Size Statement::GetWStringLen(Size cIndex, Status *pnStatus/* = NULL*/)
{
	if (NULL == m_pSTMT)
	{
		if (NULL != pnStatus)
		{
			*pnStatus = Status_NotInitialized;
		}

		return 0;
	}

	return (Size)sqlite3_column_bytes16((sqlite3_stmt *)m_pSTMT, (int)cIndex) / 2;
}

const void *Statement::GetBLOB(Size cIndex, Status *pnStatus/* = NULL*/)
{
	if (NULL == m_pSTMT)
	{
		if (NULL != pnStatus)
		{
			*pnStatus = Status_NotInitialized;
		}

		return 0;
	}

	return sqlite3_column_blob((sqlite3_stmt *)m_pSTMT, (int)cIndex);
}

Size Statement::GetBLOBSize(Size cIndex, Status *pnStatus/* = NULL*/)
{
	if (NULL == m_pSTMT)
	{
		if (NULL != pnStatus)
		{
			*pnStatus = Status_NotInitialized;
		}

		return 0;
	}

	return sqlite3_column_bytes((sqlite3_stmt *)m_pSTMT, (int)cIndex);
}

Statement::Result Statement::Step(Status *pStatus/* = NULL*/)
{
	int nRet;

	if (NULL == m_pSTMT)
	{
		if (NULL != pStatus)
		{
			*pStatus = Status(Status_NotInitialized, "Statement was not created");
		}

		return Result_Error;
	}
	
	nRet = sqlite3_step((sqlite3_stmt *)m_pSTMT);
	if (SQLITE_ROW == nRet)
	{
		if (NULL != pStatus)
		{
			*pStatus = Status();
		}

		return Result_Row;
	}
	else
	if (SQLITE_DONE == nRet)
	{
		if (NULL != pStatus)
		{
			*pStatus = Status();
		}

		return Result_Done;
	}
	else
	{
		if (NULL != pStatus)
		{
			*pStatus = SQLSTATUS(Status_OperationFailed, "sqlite3_step", nRet);
		}

		return Result_Error;
	}
}

Status Statement::Reset()
{
	int nRet;

	if (NULL == m_pSTMT)
	{
		return Status(Status_NotInitialized, "Statement was not created");
	}
	if (SQLITE_OK != (nRet = sqlite3_reset((sqlite3_stmt *)m_pSTMT)))
	{
		return SQLSTATUS(Status_OperationFailed, "sqlite3_reset", nRet);
	}
	
	return Status();
}

}//namespace SQLite

}//namespace DB

}//namespace CX
