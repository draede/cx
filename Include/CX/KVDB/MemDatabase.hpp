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


#include "CX/KVDB/IDatabase.hpp"
#include "CX/KVDB/MemTable.hpp"
#include "CX/KVDB/MemDatabaseConfig.hpp"
#include "CX/KVDB/MemTableConfig.hpp"
#include "CX/IO/IInputStream.hpp"
#include "CX/IO/IOutputStream.hpp"
#include "CX/SparseHashMap.hpp"


namespace CX
{

namespace KVDB
{

class CX_API MemDatabase : public IDatabase
{
public:

	typedef UInt64   MagicType;

	static const MagicType MAGIC; //CXMDB1.0

	static const MemDatabaseConfig   DEFAULT_DB_CONFIG;
	static const MemTableConfig      DEFAULT_TABLE_CONFIG;

	MemDatabase();

	~MemDatabase();

	virtual Status Open(const Char *szPath, const IDatabaseConfig &config);

	virtual Status Close();

	virtual Status OpenTable(ITable **ppTable, const ITableConfig &config, const Char *szName);

	virtual Status CloseTable(ITable *pTable);

	virtual Status DropTable(const Char *szName);

	virtual Status BeginTransaction();

	virtual Status EndTransaction(bool bAbort = false);

	virtual Status Commit();

	virtual bool IsOK() const;

	virtual const Char *GetPath() const;

	virtual bool IsReadOnly() const;

	Status Save();

	Status Save(const Char *szPath);

	Status Save(IO::IOutputStream *pOutputStream);

	Status Load();

	Status Load(const Char *szPath);

	Status Load(IO::IInputStream *pInputStream);

private:

	static const Size MAX_TABLES_COUNT = 65536;

	typedef SparseHashMap<String, MemTable *>::Type   TablesMap;

#pragma warning(push)
#pragma warning(disable: 4251)
	String      m_sPath;
	TablesMap   m_mapTables;
#pragma warning(pop)

	void FreeMem();

	Status WriteData(IO::IOutputStream *pOutputStream, const void *pData, Size cbSize, Hash::IHash *pHash) const;

};

}//namespace KVDB

}//namespace CX

