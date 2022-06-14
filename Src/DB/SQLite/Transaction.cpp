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

#include "CX/precomp.hpp"
#include "CX/DB/SQLite/Transaction.hpp"
#include "sqlite3.h"


#define SQLSTATUS(status, func, ret)                                                                                   \
Status(status, "{1} failed with code {2}, message '{3}'", func, ret, sqlite3_errstr(ret))


namespace CX
{

namespace DB
{

namespace SQLite
{

Transaction::Transaction(Database *pDB, Mode nMode/* = Mode_Default*/)
{
	String sSQL;

	m_pDB         = pDB;
	m_nMode       = nMode;
	m_bSuccessful = true;
	
	sSQL = "BEGIN ";
	if (Mode_Immediate == nMode)
	{
		sSQL += "IMMEDIATE ";
	}
	else
	if (Mode_Exclusive == nMode)
	{
		sSQL += "EXCLUSIVE ";
	}
	sSQL += "TRANSACTION";

	m_initStatus = m_pDB->Exec(sSQL.c_str());
}

Transaction::~Transaction()
{
	if (m_initStatus.IsOK())
	{
		if (m_bSuccessful)
		{
			m_pDB->Exec("END TRANSACTION");
		}
		else
		{
			m_pDB->Exec("ROLLBACK TRANSACTION");
		}
	}
}

Database *Transaction::GetDatabase()
{
	return m_pDB;
}

Transaction::Mode Transaction::GetMode() const
{
	return m_nMode;
}

bool Transaction::IsOK() const
{
	return m_initStatus.IsOK();
}

Status Transaction::GetInitStatus() const
{
	return m_initStatus;
}

void Transaction::SetSuccessful(bool bSuccessful/* = true*/)
{
	m_bSuccessful = bSuccessful;
}

bool Transaction::GetSuccessful() const
{
	return m_bSuccessful;
}

}//namespace SQLite

}//namespace DB

}//namespace CX
