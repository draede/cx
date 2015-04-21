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

#include "CX/KVDB/MemTable.hpp"
#include "CX/KVDB/MemIterator.hpp"
#include "MemDBSerialize.hpp"


namespace CX
{

namespace KVDB
{

MemTable::MemTable(IDatabase *pDatabase, const Char *szName)
{
	m_pDB    = pDatabase;
	m_sName  = szName;
}

MemTable::~MemTable()
{
}

const Char *MemTable::GetName()
{
	return m_sName.c_str();
}

Status MemTable::Get(const void *pKey, Size cbKeySize, void **ppValue, Size *pcbValueSize)
{
	Util::RCMemBuffer      key(pKey, cbKeySize, true);
	RecordsMap::iterator   iter;

	if (m_mapRecords.end() == (iter = m_mapRecords.find(key)))
	{
		return Status(Status_NotFound, "Not found");
	}
	if (NULL == (*ppValue = Mem::Alloc(iter->second.GetSize())))
	{
		return Status(Status_MemAllocFailed, "Failed to allocate {1} bytes", iter->second.GetSize());
	}
	memcpy(*ppValue, iter->second.GetMem(), iter->second.GetSize());
	*pcbValueSize = iter->second.GetSize();

	return Status();
}

Status MemTable::Set(const void *pKey, Size cbKeySize, const void *pValue, Size cbValueSize)
{
	m_mapRecords[Util::RCMemBuffer(pKey, cbKeySize)] = Util::RCMemBuffer(pValue, cbValueSize);

	return Status();
}

Status MemTable::Remove(const void *pKey, Size cbKeySize)
{
	Util::RCMemBuffer      key(pKey, cbKeySize, true);
	RecordsMap::iterator   iter;

	if (m_mapRecords.end() == (iter = m_mapRecords.find(key)))
	{
		return Status(Status_NotFound, "Not found");
	}
	m_mapRecords.erase(iter);

	return Status();
}

Status MemTable::OpenIterator(IIterator **ppIterator, const void *pKeyMin, Size cbKeyMinSize,
                              const void *pKeyMax, Size cbKeyMaxSize)
{
	pKeyMin;
	cbKeyMinSize;
	pKeyMax;
	cbKeyMaxSize;

	*ppIterator = new MemIterator(this, &m_mapRecords);

	return Status();
}

Status MemTable::CloseIterator(IIterator *pIterator)
{
	MemIterator *pRealIterator = dynamic_cast<MemIterator *>(pIterator);

	if (NULL == pRealIterator)
	{
		return Status(Status_InvalidArg, "Not a valid iterator");
	}
	delete pRealIterator;

	return Status();
}

Status MemTable::FreeValueMem(void *pValue)
{
	Mem::Free(pValue);

	return Status();
}

IDatabase *MemTable::GetDB()
{
	return m_pDB;
}

Status MemTable::Save(IO::IOutputStream *pOutputStream, Hash::IHash *pHash)
{
	Status   status;

	if ((status = MemDBSerialize::WriteString(pOutputStream, m_sName, pHash)).IsNOK())
	{
		return status;
	}
	if ((status = MemDBSerialize::WriteCount(pOutputStream, m_mapRecords.size(), pHash)).IsNOK())
	{
		return status;
	}
	for (RecordsMap::iterator iter = m_mapRecords.begin(); iter != m_mapRecords.end(); iter++)
	{
		if ((status = MemDBSerialize::WriteMemBuffer(pOutputStream, iter->first, pHash)).IsNOK())
		{
			return status;
		}
		if ((status = MemDBSerialize::WriteMemBuffer(pOutputStream, iter->second, pHash)).IsNOK())
		{
			return status;
		}
	}

	return Status();
}

Status MemTable::Load(IO::IInputStream *pInputStream, Hash::IHash *pHash)
{
	Size     cRecordsCount;
	Status   status;

	m_mapRecords.clear();
	if ((status = MemDBSerialize::ReadString(pInputStream, m_sName, pHash)).IsNOK())
	{
		return status;
	}
	if ((status = MemDBSerialize::ReadCount(pInputStream, cRecordsCount, pHash)).IsNOK())
	{
		return status;
	}
	if ((UInt64)MAX_RECORDS_COUNT < cRecordsCount)
	{
		return Status(Status_ReadFailed, "Too many record for table '{1}' (max is {2}, {3} found))", 
		              m_sName, MAX_RECORDS_COUNT, cRecordsCount);
	}
	for (Size i = 0; i < cRecordsCount; i++)
	{
		/*
		Util::RCMemBuffer key;
		Util::RCMemBuffer val;

		if ((status = MemDBSerialize::ReadMemBuffer(pInputStream, key, pHash)).IsNOK())
		{
			return status;
		}
		if ((status = MemDBSerialize::ReadMemBuffer(pInputStream, val, pHash)).IsNOK())
		{
			return status;
		}
		m_mapRecords[key] = val;
		*/
	}

	return Status();
}

}//namespace KVDB

}//namespace CX

