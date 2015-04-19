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

#include "CX/Status.hpp"
#include "libforestdb/forestdb.h"


namespace CX
{

namespace KVDB
{

class FDBStatus : public Status
{
public:

	FDBStatus(fdb_status nStatus)
	{
		Set(Status_OperationFailed, GetMsgFromFDBStatus(nStatus));
	}

	static const Char *GetMsgFromFDBStatus(fdb_status nStatus)
	{
		if (FDB_RESULT_SUCCESS == nStatus)
		{
			return "ForestDB operation success";
		}
		else
		if (FDB_RESULT_INVALID_ARGS == nStatus)
		{
			return "Invalid parameters to ForestDB APIs";
		}
		else
		if (FDB_RESULT_OPEN_FAIL == nStatus)
		{
			return "Database open operation fails";
		}
		else
		if (FDB_RESULT_NO_SUCH_FILE == nStatus)
		{
			return "Database file not found";
		}
		else
		if (FDB_RESULT_WRITE_FAIL == nStatus)
		{
			return "Database write operation fails";
		}
		else
		if (FDB_RESULT_READ_FAIL == nStatus)
		{
			return "Database read operation fails";
		}
		else
		if (FDB_RESULT_CLOSE_FAIL == nStatus)
		{
			return "Database close operation fails";
		}
		else
		if (FDB_RESULT_COMMIT_FAIL == nStatus)
		{
			return "Database commit operation fails";
		}
		else
		if (FDB_RESULT_ALLOC_FAIL == nStatus)
		{
			return "Memory allocation fails";
		}
		else
		if (FDB_RESULT_KEY_NOT_FOUND == nStatus)
		{
			return "A key not found in database";
		}
		else
		if (FDB_RESULT_RONLY_VIOLATION == nStatus)
		{
			return "Read-only access violation";
		}
		else
		if (FDB_RESULT_COMPACTION_FAIL == nStatus)
		{
			return "Database compaction fails";
		}
		else
		if (FDB_RESULT_ITERATOR_FAIL == nStatus)
		{
			return "Database iterator operation fails";
		}
		else
		if (FDB_RESULT_SEEK_FAIL == nStatus)
		{
			return "ForestDB I/O seek failure";
		}
		else
		if (FDB_RESULT_FSYNC_FAIL == nStatus)
		{
			return "ForestDB I/O fsync failure";
		}
		else
		if (FDB_RESULT_CHECKSUM_ERROR == nStatus)
		{
			return "ForestDB I/O checksum error";
		}
		else
		if (FDB_RESULT_FILE_CORRUPTION == nStatus)
		{
			return "ForestDB I/O file corruption";
		}
		else
		if (FDB_RESULT_COMPRESSION_FAIL == nStatus)
		{
			return "ForestDB I/O compression error";
		}
		else
		if (FDB_RESULT_NO_DB_INSTANCE == nStatus)
		{
			return "A database instance with a given sequence number was not found";
		}
		else
		if (FDB_RESULT_FAIL_BY_ROLLBACK == nStatus)
		{
			return "Requested FDB operation failed as rollback is currently being executed";
		}
		else
		if (FDB_RESULT_INVALID_CONFIG == nStatus)
		{
			return "ForestDB config value is invalid";
		}
		else
		if (FDB_RESULT_MANUAL_COMPACTION_FAIL == nStatus)
		{
			return "Try to perform manual compaction when compaction daemon is enabled";
		}
		else
		if (FDB_RESULT_INVALID_COMPACTION_MODE == nStatus)
		{
			return "Open a file with invalid compaction mode";
		}
		else
		if (FDB_RESULT_FILE_IS_BUSY == nStatus)
		{
			return "Operation cannot be performed as file handle has not been closed";
		}
		else
		if (FDB_RESULT_FILE_REMOVE_FAIL == nStatus)
		{
			return "Database file remove operation fails";
		}
		else
		if (FDB_RESULT_FILE_RENAME_FAIL == nStatus)
		{
			return "Database file rename operation fails";
		}
		else
		if (FDB_RESULT_TRANSACTION_FAIL == nStatus)
		{
			return "Transaction operation fails";
		}
		else
		if (FDB_RESULT_FAIL_BY_TRANSACTION == nStatus)
		{
			return "Requested FDB operation failed due to active transactions";
		}
		else
		if (FDB_RESULT_FAIL_BY_COMPACTION == nStatus)
		{
			return "Requested FDB operation failed due to an active compaction task";
		}
		else
		if (FDB_RESULT_TOO_LONG_FILENAME == nStatus)
		{
			return "Filename is too long";
		}
		else
		if (FDB_RESULT_INVALID_HANDLE == nStatus)
		{
			return "Passed ForestDB handle is Invalid";
		}
		else
		if (FDB_RESULT_KV_STORE_NOT_FOUND == nStatus)
		{
			return "A KV store not found in database";
		}
		else
		if (FDB_RESULT_KV_STORE_BUSY == nStatus)
		{
			return "There is an opened handle of the KV store";
		}
		else
		if (FDB_RESULT_INVALID_KV_INSTANCE_NAME == nStatus)
		{
			return "Same KV instance name already exists";
		}
		else
		if (FDB_RESULT_INVALID_CMP_FUNCTION == nStatus)
		{
			return "Custom compare function is assigned incorrectly";
		}
		else
		if (FDB_RESULT_IN_USE_BY_COMPACTOR == nStatus)
		{
			return "DB file can't be destroyed as the file is being compacted. Please retry in sometime";
		}
		else
		if (FDB_RESULT_FILE_NOT_OPEN == nStatus)
		{
			return "DB file used in this operation has not been opened";
		}
		else
		{
			return "Unknown error";
		}
	}

};

}//namespace KVDB

}//namespace CX

