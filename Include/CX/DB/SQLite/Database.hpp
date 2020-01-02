/* 
 * CX - C++ framework for general purpose development
 *
 * https://github.com/draede/cx
 * 
 * Copyright (C) 2014 - 2020 draede - draede [at] outlook [dot] com
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


namespace CX
{

namespace DB
{

namespace SQLite
{

class Database
{
public:
	
	static const unsigned int OPEN_READONLY;
	static const unsigned int OPEN_READWRITE;
	static const unsigned int OPEN_CREATE;
	static const unsigned int OPEN_DELETEONCLOSE;
	static const unsigned int OPEN_EXCLUSIVE;
	static const unsigned int OPEN_AUTOPROXY;
	static const unsigned int OPEN_URI;
	static const unsigned int OPEN_MEMORY;
	static const unsigned int OPEN_MAIN_DB;
	static const unsigned int OPEN_TEMP_DB;
	static const unsigned int OPEN_TRANSIENT_DB;
	static const unsigned int OPEN_MAIN_JOURNAL;
	static const unsigned int OPEN_TEMP_JOURNAL;
	static const unsigned int OPEN_SUBJOURNAL;
	static const unsigned int OPEN_MASTER_JOURNAL;
	static const unsigned int OPEN_NOMUTEX;
	static const unsigned int OPEN_FULLMUTEX;
	static const unsigned int OPEN_SHAREDCACHE;
	static const unsigned int OPEN_PRIVATECACHE;
	static const unsigned int OPEN_WAL;

	static const unsigned int OPEN_DEFAULT;

	static const unsigned int VERSION_NUMBER;
	static const Char         *VERSION_STRING;

	Database();

	~Database();

	Database(const Char *szPath, unsigned int nFlags = OPEN_DEFAULT);

	static Status Initialize();

	static Status Shutdown();

	Status Open(const Char *szPath, unsigned int nFlags = OPEN_DEFAULT);

	Status Close();

	bool IsOK() const;

	Status GetInitStatus() const; //status from constructor with args

	Status Exec(const Char *szSimpleSQL);

	Int64 GetLastInsertID();

	Size GetLastExecChangesCount();

	Status GetPragma(const Char *szName, Int64 *pnValue);

	Status SetPragma(const Char *szName, Int64 nValue);

	Status GetPragma(const Char *szName, Double *plfValue);

	Status SetPragma(const Char *szName, Double lfValue);

	Status GetPragma(const Char *szName, String *psValue);

	Status SetPragma(const Char *szName, const Char *szValue);

	void *GetHandle() const;

private:

	void   *m_pDB;
	Status m_initStatus;

};

}//namespace SQLite

}//namespace DB

}//namespace CX

