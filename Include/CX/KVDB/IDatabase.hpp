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


#include "CX/Types.hpp"
#include "CX/Status.hpp"
#include "CX/KVDB/ITable.hpp"
#include "CX/KVDB/IDatabaseConfig.hpp"
#include "CX/IObject.hpp"
#include "CX/APIDefs.hpp"


namespace CX
{

namespace KVDB
{

class CX_API IDatabase : public IObject
{
public:

	virtual ~IDatabase() { }

	virtual Status Open(const Char *szPath, const IDatabaseConfig &config) = 0;

	virtual Status Close() = 0;

	virtual Status OpenTable(ITable **ppTable, const ITableConfig &config, const Char *szName) = 0;

	virtual Status CloseTable(ITable *pTable) = 0;

	virtual Status DropTable(const Char *szName) = 0;

	virtual Status BeginTransaction() = 0;

	virtual Status EndTransaction(bool bAbort = false) = 0;

	virtual Status Commit() = 0;

	virtual bool IsOK() const = 0;

	virtual const Char *GetPath() const = 0;

	virtual bool IsReadOnly() const = 0;

};

}//namespace KVDB

}//namespace CX

