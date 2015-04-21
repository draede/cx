/* 
 * CX - C++ framework for general purpose development
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

#include "CX/KVDB/MemIterator.hpp"
#include "CX/KVDB/MemRecord.hpp"
#include "CX/KVDB/MemTable.hpp"


namespace CX
{

namespace KVDB
{

MemIterator::MemIterator(ITable *pTable, void *pMap)
{
	m_pTable = pTable;
	m_pMap   = pMap;
	m_pIter  = new MemTable::RecordsMap::iterator();
	*((MemTable::RecordsMap::iterator *)m_pIter) = ((MemTable::RecordsMap *)m_pMap)->begin();
}

MemIterator::~MemIterator()
{
	delete (MemTable::RecordsMap::iterator *)m_pIter;
}

Status MemIterator::Get(IRecord **ppRecord)
{
	MemTable::RecordsMap::iterator *pIter = (MemTable::RecordsMap::iterator *)m_pIter;

	if (NULL == (*ppRecord = new MemRecord(this, (*pIter)->first.GetMem(), (*pIter)->first.GetSize(), 
	                                         (*pIter)->second.GetMem(), (*pIter)->second.GetSize())))
	{
		return Status(Status_MemAllocFailed, "Failed to allocate record");
	}

	return Status();
}

Status MemIterator::FreeRecordMem(IRecord *pRecord)
{
	MemRecord *pRealRecord = dynamic_cast<MemRecord *>(pRecord);

	if (NULL == pRealRecord)
	{
		return Status(Status_InvalidArg, "Not a valid record");
	}
	delete pRealRecord;

	return Status();
}

Status MemIterator::Next()
{
	MemTable::RecordsMap::iterator iter = *((MemTable::RecordsMap::iterator *)m_pIter);
	
	if (((MemTable::RecordsMap *)m_pMap)->end() == *((MemTable::RecordsMap::iterator *)m_pIter))
	{
		return Status(Status_NoMoreItems, "No more records");
	}
	(*((MemTable::RecordsMap::iterator *)m_pIter))++;
	if (((MemTable::RecordsMap *)m_pMap)->end() == *((MemTable::RecordsMap::iterator *)m_pIter))
	{
		return Status(Status_NoMoreItems, "No more records");
	}

	return Status();
}

Status MemIterator::Reset()
{
	*((MemTable::RecordsMap::iterator *)m_pIter) = ((MemTable::RecordsMap *)m_pMap)->begin();

	return Status();
}

ITable *MemIterator::GetTable()
{
	return m_pTable;
}

}//namespace KVDB

}//namespace CX

