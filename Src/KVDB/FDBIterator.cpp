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

#include "CX/KVDB/FDBIterator.hpp"
#include "CX/KVDB/FDBRecord.hpp"
#include "FDBStatus.hpp"
#include "libforestdb/forestdb.h"


namespace CX
{

namespace KVDB
{

FDBIterator::FDBIterator(ITable *pTable, void *pIter)
{
	m_pTable = pTable;
	m_pIter  = pIter;
}

FDBIterator::~FDBIterator()
{
}

void *FDBIterator::GetIter()
{
	return m_pIter;
}

Status FDBIterator::Get(IRecord **ppRecord)
{	
	fdb_doc    *pDoc = NULL;
	fdb_status nStatus;

	if (FDB_RESULT_SUCCESS != (nStatus = fdb_iterator_get((fdb_iterator *)m_pIter, &pDoc)))
	{
		return FDBStatus(nStatus);
	}

	if (NULL == (*ppRecord = new FDBRecord(this, pDoc)))
	{
		fdb_doc_free(pDoc);

		return Status(Status_MemAllocFailed, "Failed to allocate record");
	}

	return Status();
}

Status FDBIterator::FreeRecordMem(IRecord *pRecord)
{
	FDBRecord *pRealRecord = dynamic_cast<FDBRecord *>(pRecord);

	if (NULL == pRealRecord)
	{
		return Status(Status_InvalidArg, "Not a valid record");
	}

	fdb_status nStatus;

	if (FDB_RESULT_SUCCESS != (nStatus = fdb_doc_free((fdb_doc *)pRealRecord->GetDoc())))
	{
		return FDBStatus(nStatus);
	}
	
	delete pRecord;

	return Status();
}

Status FDBIterator::Prev()
{
	fdb_status nStatus;

	if (FDB_RESULT_SUCCESS != (nStatus = fdb_iterator_prev((fdb_iterator *)m_pIter)))
	{
		return FDBStatus(nStatus);
	}

	return Status();
}

Status FDBIterator::Next()
{
	fdb_status nStatus;

	if (FDB_RESULT_SUCCESS != (nStatus = fdb_iterator_next((fdb_iterator *)m_pIter)))
	{
		return FDBStatus(nStatus);
	}

	return Status();
}

Status FDBIterator::First()
{
	fdb_status nStatus;

	if (FDB_RESULT_SUCCESS != (nStatus = fdb_iterator_seek_to_min((fdb_iterator *)m_pIter)))
	{
		return FDBStatus(nStatus);
	}

	return Status();
}

Status FDBIterator::Last()
{
	fdb_status nStatus;

	if (FDB_RESULT_SUCCESS != (nStatus = fdb_iterator_seek_to_max((fdb_iterator *)m_pIter)))
	{
		return FDBStatus(nStatus);
	}

	return Status();
}

Status FDBIterator::Seek(const void *pKey, Size cbKeySize, SeekAprox nSeekAprox/* = SeekAprox_Higher*/)
{
	fdb_status nStatus;

	fdb_iterator_seek_opt_t nDir;

	if (SeekAprox_Lower == nSeekAprox)
	{
		nDir = FDB_ITR_SEEK_LOWER;
	}
	else
	{
		nDir = FDB_ITR_SEEK_HIGHER;
	}

	if (FDB_RESULT_SUCCESS != (nStatus = fdb_iterator_seek((fdb_iterator *)m_pIter, pKey, cbKeySize, nDir)))
	{
		return FDBStatus(nStatus);
	}

	return Status();
}

ITable *FDBIterator::GetTable()
{
	return m_pTable;
}

}//namespace KVDB

}//namespace CX

