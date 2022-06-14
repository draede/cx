/*
 * CX - C++ framework for general purpose development
 *
 * https://github.com/draede/cx
 *
 * Copyright (C) 2014 - 2022 draede
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
#include "CX/Util/Timer.hpp"
#include "CX/Vector.hpp"
#include "CX/Map.hpp"
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

	struct Helper
	{
		OperationsBatchesVector    vectorOperationsBatches;
		Size                       cTotalCount;
		Util::Timer                timer;
		Bool                       bFlush;
	};

	typedef Map<DBHelper *, Helper>::Type   HelpersMap;

	Size                   m_cMaxAsyncOperations;
	UInt32                 m_cMaxAsyncFlushTimeout;
	HelpersMap             m_mapHelpers;
	Sys::Thread            m_thread;
	Sys::Event             m_eventStop;
	Sys::Event             m_eventFlush;
	Sys::Event             m_eventFlushReady;
	Sys::FastRWLock        m_frwLock;

	void AsyncOperationsThread();

};

}//namespace SQLite

}//namespace DB

}//namespace CX
