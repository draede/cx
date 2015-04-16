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

#pragma once


#include "CX/KVDB/IDatabase.hpp"
#include "CX/KVDB/FDBTable.hpp"
#include "CX/KVDB/FDBDatabaseConfig.hpp"
#include "CX/KVDB/FDBTableConfig.hpp"


namespace CX
{

namespace KVDB
{

class CX_API FDBDatabase : public IDatabase
{
public:

	static const FDBDatabaseConfig   DEFAULT_DB_CONFIG;
	static const FDBTableConfig      DEFAULT_TABLE_CONFIG;

	FDBDatabase();

	~FDBDatabase();

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

private:

	void     *m_pDB;
	bool     m_bReadOnly;
#pragma warning(push)
#pragma warning(disable: 4251)
	String   m_sPath;
#pragma warning(pop)

};

}//namespace KVDB

}//namespace CX

