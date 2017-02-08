/* 
 * CX - C++ framework for general purpose development
 *
 * https://github.com/draede/cx
 * 
 * Copyright (C) 2014 - 2017 draede - draede [at] outlook [dot] com
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
#include "CX/DB/SQLite/Database.hpp"
#include "CX/DB/SQLite/Statement.hpp"
#include "CX/DB/SQLite/Transaction.hpp"
#include "CX/Map.hpp"
#include "CX/Queue.hpp"
#include "CX/Sys/Lock.hpp"
#include "CX/C/Platform/Windows/windows.h"
#include "CX/C/stdarg.h"


namespace CX
{

namespace DB
{

namespace SQLite
{

class DBHelper
{
public:

	static const Size MAX_ASYNC_OPERATIONS       = 5000; //5000 inserts + updates + deletes
	static const Size MAX_ASYNC_FLUSH_TIMEOUT    = 500;  //500 ms

	class StatementScope
	{
	public:

		StatementScope(DBHelper *pDBHelper, Size cIndex);

		~StatementScope();

		DBHelper *GetDBHelper();

		Statement *GetStatement();

		Size GetIndex();

	private:

		DBHelper    *m_pDBHelper;
		Statement   *m_pStatement;
		Size        m_cIndex;

	};

	class IAsyncOperationHandler
	{
	public:

		virtual ~IAsyncOperationHandler() { }

		virtual void OnCompletion(Statement *pStatement, const Status &status) = 0;

		virtual void Release() = 0;

	};

	DBHelper();

	DBHelper(const Char *szPath, 
	         Size cMaxAsyncOperations = MAX_ASYNC_OPERATIONS, 
	         Size cMaxAsyncFlushTimeout = MAX_ASYNC_FLUSH_TIMEOUT, 
	         unsigned int nFlags = DB::SQLite::Database::OPEN_DEFAULT);

	~DBHelper();

	Status Open(const Char *szPath, 
	            Size cMaxAsyncOperations = MAX_ASYNC_OPERATIONS,
	            Size cMaxAsyncFlushTimeout = MAX_ASYNC_FLUSH_TIMEOUT,
	            unsigned int nFlags = DB::SQLite::Database::OPEN_DEFAULT);

	//statements must not be in use here!
	Status Close();

	bool IsOK() const;

	Database &GetDB();

	const Database &GetDB() const;

	Status GetInitStatus() const; //status from constructor with args

	Bindings *CreateBindings();

	Bindings *CreateBindings(const Char *szArgsType, ...);

	Bindings *CreateBindings(const Char *szArgsType, va_list vl);

	void DestroyBindings(Bindings *pBindings);

	//pBindings must be create with CreateBindings (after this call the ownership of pBindings is taken by DBHelper)
	//pAsyncOperationHandler will be released with pAsyncOperationHandler->Release()
	Status AddAsyncOperation(Size cStatementIndex, Bindings *pBindings,
	                         IAsyncOperationHandler *pAsyncOperationHandler = NULL);

	Status FlushAsyncOperations();
	
	//do not use this statement from multiple threads!!!
	Status AcquireStatement(Size cIndex, DB::SQLite::Statement **ppStatement);

	Status ReleaseStatement(DB::SQLite::Statement *pStatement);

	virtual const Char *GetDDL() const = 0;//DDL must use 'if not exists' on every create (DDL will be run on every Open)

	virtual const Char **GetStatements() const = 0;

	virtual Size GetStatementsCount() const = 0;

	virtual Status OnPreOpen() { return Status(); }

	virtual Status OnPostOpen() { return Status(); }

	virtual Status OnPreClose() { return Status(); }
	
	virtual Status OnPostClose() { return Status(); }

private:

	typedef CX::Queue<Statement *>::Type            StatementsQueue;

	typedef CX::Map<Size, StatementsQueue>::Type    AvailStatementsMap;

	typedef CX::Map<Statement *, Size>::Type        UsedStatementsMap;

	struct Operation
	{
		Size                   cStatementIndex;
		Bindings               *pBindings;
		IAsyncOperationHandler *pAsyncOperationHandler;
	};

	typedef CX::Queue<Operation>::Type              OperationsQueue;

	Database             m_db;
	Status               m_initStatus;

	Size                 m_cMaxAsyncOperations;
	Size                 m_cMaxAsyncFlushTimeout;
	HANDLE               m_hAsyncThread;
	HANDLE               m_hStopEvent;
	HANDLE               m_hFlushEvent;

	AvailStatementsMap   m_mapAvailStatements;
	UsedStatementsMap    m_mapUsedStatements;
	Sys::Lock            m_lockStatements;

	OperationsQueue      m_queueOperations;
	Sys::Lock            m_lockOperations;

	static DWORD WINAPI AsyncThread(void *pArgs);

	DWORD AsyncThreadProc();

};

}//namespace SQLite

}//namespace DB

}//namespace CX

