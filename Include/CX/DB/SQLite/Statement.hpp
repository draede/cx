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
#include "CX/String.hpp"
#include "CX/DB/SQLite/Database.hpp"
#include "CX/DB/SQLite/Bindings.hpp"


namespace CX
{

namespace DB
{

namespace SQLite
{

class Statement
{
public:

	enum ArgStoreType
	{
		ArgStore_Static    = 1,
		ArgStore_Transient = 2,
		ArgStore_Default   = ArgStore_Static,
	};

	enum Result
	{
		Result_Error,
		Result_Row,
		Result_Done,
	};

	static const unsigned int TYPE_INTEGER;
	static const unsigned int TYPE_REAL;
	static const unsigned int TYPE_BLOB;
	static const unsigned int TYPE_NULL;
	static const unsigned int TYPE_TEXT;

	Statement();

	Statement(Database *pDB, const Char *szSQL);

	~Statement();

	Status Open(Database *pDB, const Char *szSQL);

	Status Close();

	Database *GetDatabase() const;

	const Char *GetSQL() const;

	const Char *GetExpandedSQL();

	bool IsOK() const;

	Status GetInitStatus() const; ////status from constructor with args

	void *GetHandle() const;

	Size GetArgsCount();

	Size GetArgIndex(const Char *szName);

	const Char *GetArgName(Size cIndex);

	Status ClearBindings();

	Status BindNull();

	Status BindInt(Int64 nValue);

	Status BindReal(Double lfValue);

	Status BindString(const Char *szString, ArgStoreType nArgStoreType = ArgStore_Default);

	Status BindWString(const WChar *wszString, ArgStoreType nArgStoreType = ArgStore_Default);

	Status BindBLOB(const void *pData, Size cbSize, ArgStoreType nArgStoreType = ArgStore_Default);

	Status BindZeroBLOB(Size cbSize);

	Status Bind(const Bindings &bindings); //will call ClearBindings first

	Size GetColumnsCount();

	unsigned int GetColumnType(Size cIndex);

	const Char *GetColumnDeclaredType(Size cIndex);

	const Char *GetColumnName(Size cIndex);

	Int64 GetInt(Size cIndex);

	Double GetReal(Size cIndex);

	const Char *GetString(Size cIndex);

	Size GetStringLen(Size cIndex);

	const WChar *GetWString(Size cIndex);

	Size GetWStringLen(Size cIndex);

	const void *GetBLOB(Size cIndex);

	Size GetBLOBSize(Size cIndex);

	Result Step(Status *pStatus = NULL);

	Status Reset();

private:

	Database   *m_pDB;
	void       *m_pSTMT;
	String     m_sSQL;
	Status     m_initStatus;
	Size       m_cBindIndex;

};

}//namespace SQLite

}//namespace DB

}//namespace CX

