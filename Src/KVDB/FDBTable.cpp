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

#include "CX/KVDB/FDBTable.hpp"
#include "CX/KVDB/FDBIterator.hpp"
#include "FDBStatus.hpp"
#include "libforestdb/forestdb.h"


namespace CX
{

namespace KVDB
{

FDBTable::FDBTable(IDatabase *pDatabase, void *pKV)
{
	m_pDB  = pDatabase;
	m_pKVS = pKV;
}

FDBTable::~FDBTable()
{
}

void *FDBTable::GetKV()
{
	return m_pKVS;
}

Status FDBTable::Get(const void *pKey, Size cbKeySize, void **ppValue, Size *pcbValueSize)
{
	fdb_status nStatus;

	if (FDB_RESULT_SUCCESS != (nStatus = fdb_get_kv((fdb_kvs_handle *)m_pKVS, (void *)pKey, cbKeySize, ppValue, pcbValueSize)))
	{
		return FDBStatus(nStatus);
	}

	return Status();
}

Status FDBTable::Set(const void *pKey, Size cbKeySize, const void *pValue, Size cbValueSize)
{
	fdb_status nStatus;

	if (FDB_RESULT_SUCCESS != (nStatus = fdb_set_kv((fdb_kvs_handle *)m_pKVS, (void *)pKey, cbKeySize, 
	                                                (void *)pValue, cbValueSize)))
	{
		return FDBStatus(nStatus);
	}

	return Status();
}

Status FDBTable::Remove(const void *pKey, Size cbKeySize)
{
	fdb_status nStatus;

	if (FDB_RESULT_SUCCESS != (nStatus = fdb_del_kv((fdb_kvs_handle *)m_pKVS, (void *)pKey, cbKeySize)))
	{
		return FDBStatus(nStatus);
	}

	return Status();
}

Status FDBTable::OpenIterator(IIterator **ppIterator, const void *pKeyMin, Size cbKeyMinSize,
                              const void *pKeyMax, Size cbKeyMaxSize)
{
	fdb_status nStatus;

	fdb_iterator *pIter;

	if (FDB_RESULT_SUCCESS != (nStatus = fdb_iterator_init((fdb_kvs_handle *)m_pKVS, &pIter, pKeyMin, cbKeyMinSize, 
	                                                       pKeyMax, cbKeyMaxSize, FDB_ITR_NO_DELETES)))
	{
		return FDBStatus(nStatus);
	}
	if (NULL == (*ppIterator = new FDBIterator(this, pIter)))
	{
		fdb_iterator_close(pIter);

		return Status(Status_MemAllocFailed, "Failed to allocate iterator");
	}

	return Status();
}

Status FDBTable::CloseIterator(IIterator *pIterator)
{
	FDBIterator *pRealIterator = dynamic_cast<FDBIterator *>(pIterator);

	if (NULL == pRealIterator)
	{
		return Status(Status_InvalidArg, "Not a valid iterator");
	}

	fdb_status nStatus;
	
	if (FDB_RESULT_SUCCESS != (nStatus = fdb_iterator_close((fdb_iterator *)pRealIterator->GetIter())))
	{
		return FDBStatus(nStatus);
	}

	return Status();
}

Status FDBTable::FreeValueMem(void *pValue)
{
	fdb_status nStatus;
	
	if (FDB_RESULT_SUCCESS != (nStatus = fdb_free_block(pValue)))
	{
		return FDBStatus(nStatus);
	}

	return Status();
}

IDatabase *FDBTable::GetDB()
{
	return m_pDB;
}

}//namespace KVDB

}//namespace CX

