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

#pragma once


#include "CX/KVDB/ITable.hpp"
#include "CX/KVDB/MemTableConfig.hpp"
#include "CX/Util/RCMemBuffer.hpp"
#include "CX/IO/IInputStream.hpp"
#include "CX/IO/IOutputStream.hpp"
#include "CX/Hash/xxHash32.hpp"
#include "CX/Hash/xxHash64.hpp"
#include "CX/SparseHashMap.hpp"


namespace CX
{

namespace KVDB
{

class CX_API MemTable : public ITable
{
public:

	//free with FreeValueMem
	virtual Status Get(const void *pKey, Size cbKeySize, void **ppValue, Size *pcbValueSize);

	virtual Status Set(const void *pKey, Size cbKeySize, const void *pValue, Size cbValueSize);

	virtual Status Remove(const void *pKey, Size cbKeySize);

	//min & max keys are ignored
	virtual Status OpenIterator(IIterator **ppIterator, const void *pKeyMin, Size cbKeyMinSize, 
	                            const void *pKeyMax, Size cbKeyMaxSize);

	virtual Status CloseIterator(IIterator *pIterator);

	virtual Status FreeValueMem(void *pValue);

	virtual IDatabase *GetDB();

protected:
	
	friend class MemDatabase;
	friend class MemIterator;

	typedef Util::RCMemBuffer   Key;
	typedef Util::RCMemBuffer   Value;

	struct Hasher
	{
		size_t operator()(const Key &p) const
		{
#ifdef CX_64BIT_ARCH
			return (size_t)Hash::xxHash64::DoHash(p.GetMem(), p.GetSize(), 0);
#else
			return (size_t)Hash::xxHash32::DoHash(p.GetMem(), p.GetSize(), 0);
#endif
		}
	};

	struct Comparator
	{
		bool operator() (const Key &a, const Key &b) const
		{
			if (a.GetSize() != b.GetSize())
			{
				return false;
			}

			return (0 == memcmp(a.GetMem(), b.GetMem(), a.GetSize()));
		}
	};

	typedef SparseHashMap<Key, Value, Hasher, Comparator>::Type   RecordsMap;

	MemTable(IDatabase *pDatabase, const Char *szName);

	~MemTable();

	const Char *GetName();

	Status Save(IO::IOutputStream *pOutputStream, Hash::IHash *pHash);

	Status Load(IO::IInputStream *pInputStream, Hash::IHash *pHash);

private:

	static const Size MAX_RECORDS_COUNT = 2000000000;

	IDatabase    *m_pDB;
#pragma warning(push)
#pragma warning(disable: 4251)
	RecordsMap   m_mapRecords;
	String       m_sName;
#pragma warning(pop)

};

}//namespace KVDB

}//namespace CX

