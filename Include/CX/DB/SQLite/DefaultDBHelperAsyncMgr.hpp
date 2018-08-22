/*
 * CX - C++ framework for general purpose development
 *
 * https://github.com/draede/cx
 *
 * Copyright (C) 2014 - 2018 draede - draede [at] outlook [dot] com
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


#include "CX/DB/SQLite/IDBHelperAsyncMgr.hpp"
#include "CX/Sys/FastRWLock.hpp"
#include "CX/Vector.hpp"
#include "CX/Map.hpp"
#include "CX/Set.hpp"
#include "CX/Sys/Event.hpp"
#include "CX/Sys/Thread.hpp"


namespace CX
{

namespace DB
{

namespace SQLite
{

class DefaultDBHelperAsyncMgr : public IDBHelperAsyncMgr
{
public:

	DefaultDBHelperAsyncMgr();

	virtual ~DefaultDBHelperAsyncMgr();

	virtual Status Start(Size cMaxAsyncOperations = MAX_ASYNC_OPERATIONS,
	                     UInt32 cMaxAsyncFlushTimeout = MAX_ASYNC_FLUSH_TIMEOUT);

	virtual Status Stop();

	virtual Bool IsRunning();

	virtual Bindings *CreateBindings();

	virtual Bindings *CreateBindings(const Char *szArgsType, ...);

	virtual Bindings *CreateBindings(const Char *szArgsType, va_list vl);

	virtual void DestroyBindings(Bindings *pBindings);

	//pBindings must be create with CreateBindings (after this call the ownership of pBindings is taken by DBHelper)
	//pAsyncOperationHandler will be released with pAsyncOperationHandler->Release()
	virtual Status AddAsyncOperation(DBHelper *pDBHelper, Size cStatementIndex, Bindings *pBindings,
	                                 DBHelper::IAsyncOperationHandler *pAsyncOperationHandler = NULL);

	virtual Status AddAsyncOperations(DBHelper *pDBHelper, const DBHelper::OperationsVector &vectorOperations,
	                                  DBHelper::IAsyncOperationHandler *pAsyncOperationHandler = NULL);

	virtual Status FlushAsyncOperations(DBHelper *pDBHelper);

	virtual Status FlushAsyncOperations();


private:

	struct OperationsBatch
	{
		DBHelper::OperationsVector         vectorOperations;
		DBHelper::IAsyncOperationHandler   *pAsyncOperationHandler;
	};

	typedef Vector<OperationsBatch>::Type   OperationsBatchesVector;

	struct Operations
	{
		OperationsBatchesVector    vectorOperationsBatches;
		Size                       cTotalCount;
	};

	typedef Map<DBHelper *, Operations>::Type   OperationsMap;

	typedef Set<DBHelper *>::Type               FlushDBHelpersSet;

	Size                   m_cMaxAsyncOperations;
	UInt32                 m_cMaxAsyncFlushTimeout;
	OperationsMap          m_mapOperations;
	FlushDBHelpersSet      m_setFlushDBHelpers;
	Bool                   m_bFlushAll;
	Sys::Thread            m_thread;
	Sys::Event             m_eventStop;
	Sys::Event             m_eventFlush;
	Sys::FastRWLock        m_frwLock;

	void AsyncOperationsThread();

};

}//namespace SQLite

}//namespace DB

}//namespace CX
