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


#include "CX/Types.hpp"
#include "CX/Status.hpp"
#include "CX/DB/SQLite/Database.hpp"


namespace CX
{

namespace DB
{

namespace SQLite
{

class Transaction
{
public:

	enum Mode
	{
		Mode_Deferred  = 1,
		Mode_Immediate = 2,
		Mode_Exclusive = 3,
		Mode_Default   = Mode_Deferred,
	};
	
	Transaction(Database *pDB, Mode nMode = Mode_Default);

	~Transaction();

	Database *GetDatabase();

	Mode GetMode() const;

	bool IsOK() const;

	Status GetInitStatus() const; ////status from constructor

	void SetSuccessful(bool bSuccessful = true);

	bool GetSuccessful() const;

private:

	Database   *m_pDB;
	Mode       m_nMode;
	bool       m_bSuccessful;
	Status     m_initStatus;

};

}//namespace SQLite

}//namespace DB

}//namespace CX

