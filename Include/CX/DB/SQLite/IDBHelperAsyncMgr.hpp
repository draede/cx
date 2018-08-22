
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

	virtual Bindings *CreateBindings(const Char *szArgsType, ...) = 0;

	virtual Bindings *CreateBindings(const Char *szArgsType, va_list vl) = 0;

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
