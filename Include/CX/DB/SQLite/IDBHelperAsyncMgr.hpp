/*
 * CX - C++ framework for general purpose development
 *
 * https://github.com/draede/cx
 *
 * Copyright (C) 2014 - 2022 draede - draede [at] outlook [dot] com
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


#include "CX/DB/SQLite/DBHelper.hpp"


namespace CX
{

namespace DB
{

namespace SQLite
{

class IDBHelperAsyncMgr
{
public:

	static const Size   MAX_ASYNC_OPERATIONS       = 5000; //5000 inserts + updates + deletes
	static const UInt32 MAX_ASYNC_FLUSH_TIMEOUT    = 500;  //500 ms

	virtual ~IDBHelperAsyncMgr() { }

	virtual Status Start(Size cMaxAsyncOperations = MAX_ASYNC_OPERATIONS,
	                     UInt32 cMaxAsyncFlushTimeout = MAX_ASYNC_FLUSH_TIMEOUT) = 0;

	virtual Status Stop() = 0;

	virtual Bool IsRunning() = 0;

	virtual Bindings *CreateBindings() = 0;

	virtual void DestroyBindings(Bindings *pBindings) = 0;

	//pBindings must be create with CreateBindings (after this call the ownership of pBindings is taken by DBHelper)
	//pAsyncOperationHandler will be released with pAsyncOperationHandler->Release()
	virtual Status AddAsyncOperation(DBHelper *pDBHelper, Size cStatementIndex, Bindings *pBindings,
	                                 DBHelper::IAsyncOperationHandler *pAsyncOperationHandler = NULL) = 0;

	virtual Status AddAsyncOperations(DBHelper *pDBHelper, const DBHelper::OperationsVector &vectorOperations,
	                                  DBHelper::IAsyncOperationHandler *pAsyncOperationHandler = NULL) = 0;

	virtual Status FlushAsyncOperations(DBHelper *pDBHelper) = 0;

	virtual Status FlushAsyncOperations() = 0;

};

}//namespace SQLite

}//namespace DB

}//namespace CX
