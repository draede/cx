/* 
 * CX - C++ framework for general purpose development
 *
 * https://github.com/draede/cx
 * 
 * Copyright (C) 2014 - 2021 draede
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

#include "CX/DB/SQLite/Database.hpp"
#include "CX/DB/SQLite/Transaction.hpp"
#include "CX/DB/SQLite/Statement.hpp"
#include "CX/DB/SQLite/Bindings.hpp"
#include "CX/DB/SQLite/DBHelper.hpp"
#include "CX/Print.hpp"
#include "CX/Util/Timer.hpp"
#include <windows.h>
#include "Tester.hpp"


using namespace CX;


void DB_SQLite_Test()
{
	DB::SQLite::Database    db;
	Status                  status;

	unlink("test.db");
	if ((status = db.Open("test.db")).IsOK())
	{
		if ((status = db.Exec("CREATE TABLE [config] ([id] INTEGER PRIMARY KEY AUTOINCREMENT, "
		                      "[name] TEXT NOT NULL UNIQUE, [value] TEXT);")).IsOK())
		{
			DB::SQLite::Statement::Result nResult;
			bool                          bOK = false;

			{
				DB::SQLite::Transaction tr(&db);

				if (tr.IsOK())
				{
					DB::SQLite::Statement insert(&db, "INSERT INTO [config] ([name], [value]) VALUES (?, ?);");

					if (insert.IsOK())
					{
						insert.ClearBindings();
						insert.BindString("name1");
						insert.BindString("value1");
						if (DB::SQLite::Statement::Result_Done == (nResult = insert.Step(&status)))
						{
							bOK = true;
						}
						else
						{
							tr.SetSuccessful(false);
							Print(stdout, "InsertStep1 : {1}\n", status.GetMsg());
						}
						insert.Reset();

						insert.ClearBindings();
						insert.BindString("name2");
						insert.BindString("value2");
						if (DB::SQLite::Statement::Result_Done == (nResult = insert.Step(&status)))
						{
							bOK = true;
						}
						else
						{
							tr.SetSuccessful(false);
							Print(stdout, "InsertStep2 : {1}\n", status.GetMsg());
						}
						insert.Reset();

						//test bindings
						insert.ClearBindings();

						DB::SQLite::Bindings   bindings;

						bindings.AddString("name3");
						bindings.AddString("value3");

						insert.Bind(bindings);
						if (DB::SQLite::Statement::Result_Done == (nResult = insert.Step(&status)))
						{
							bOK = true;
						}
						else
						{
							tr.SetSuccessful(false);
							Print(stdout, "InsertStep3 : {1}\n", status.GetMsg());
						}
						insert.Reset();
						
					}
					else
					{
						tr.SetSuccessful(false);
						Print(stdout, "Insert : {1}\n", insert.GetInitStatus().GetMsg());
					}
				}
				else
				{
					Print(stdout, "Transaction : {1}\n", tr.GetInitStatus().GetMsg());
				}
			}

			if (bOK)
			{
				DB::SQLite::Statement select(&db, "SELECT * FROM [config];");

				if (select.IsOK())
				{
					while (DB::SQLite::Statement::Result_Row == (nResult = select.Step(&status)))
					{
						Print(stdout, "[id={1}][name={2}][value={3}]\n", select.GetInt(0), select.GetString(1), 
						      select.GetString(2));
					}
				}
				else
				{
					Print(stdout, "Select : {1}\n", select.GetInitStatus().GetMsg());
				}
			}

			Int64 cbPageSize;
			Int64 cTotalPagesCount;
			Int64 cFreePagesCount;

			status = db.GetPragma("page_size", &cbPageSize);
			status = db.GetPragma("page_count", &cTotalPagesCount);
			status = db.GetPragma("freelist_count", &cFreePagesCount);

			cbPageSize = cbPageSize;
		}
		else
		{
			Print(stdout, "Exec : {1}\n", status.GetMsg());
		}
		db.Close();
	}
	else
	{
		Print(stdout, "Open : {1}\n", status.GetMsg());
	}
}

struct WriterData
{
	DB::SQLite::Database *pDB;
	Size                 cIndex;
	Size                 cInserts;
	Size                 cErrors;
	double               lfTime;
};

struct ReaderData
{
	DB::SQLite::Database *pDB;
	Size                 cIndex;
	Size                 cSelects;
	double               lfTime;
};

DWORD WINAPI WriterThread(void *pArgs)
{
	WriterData *pData = (WriterData *)pArgs;
	Size       cLocalIndex = 0;
	Status     status;

	pData->cInserts = 0;
	pData->cErrors = 0;
	{
		DB::SQLite::Statement::Result nResult;
		bool                          bOK = false;
		Util::Timer                   timer;

		timer.ResetTimer();
		for (;;)
		{
			DB::SQLite::Transaction tr(pData->pDB);

			if (tr.IsOK())
			{
				DB::SQLite::Statement insert(pData->pDB, "INSERT INTO [config] ([name], [value]) VALUES (?, ?);");

				if (insert.IsOK())
				{
					cLocalIndex++;
					insert.Reset();
					insert.ClearBindings();
					{
						String sText;

						Print(&sText, "name-{1}-{2}", pData->cIndex, cLocalIndex);
						insert.BindString(sText.c_str(), sText.size(), DB::SQLite::ArgStore_Transient);
					}
					{
						String sText;

						Print(&sText, "value-{1}-{2}", pData->cIndex, cLocalIndex);
						insert.BindString(sText.c_str(), sText.size(), DB::SQLite::ArgStore_Transient);
					}
					if (DB::SQLite::Statement::Result_Done == (nResult = insert.Step(&status)))
					{
						pData->cInserts++;
						bOK = true;
					}
					else
					{
						pData->cErrors++;
						tr.SetSuccessful(false);
						Print(stdout, "InsertStep1 : {1}\n", status.GetMsg());
					}
				}
				else
				{
					tr.SetSuccessful(false);
					Print(stdout, "Insert : {1}\n", insert.GetInitStatus().GetMsg());
				}
			}
			else
			{
				Print(stdout, "Transaction : {1}\n", tr.GetInitStatus().GetMsg());
			}
			if (timer.GetElapsedTime() > pData->lfTime)
			{
				break;
			}
		}
	}

	return 0;
}

DWORD WINAPI ReaderThread(void *pArgs)
{
	ReaderData *pData = (ReaderData *)pArgs;
	Status     status;

	pData->cSelects = 0;
	{
		String sSQL;

		Print(&sSQL, "SELECT * FROM [config] WHERE [name] like 'name-{1}-%';", pData->cIndex);

		DB::SQLite::Statement::Result nResult;
		Util::Timer                   timer;

		timer.ResetTimer();
		for (;;)
		{
			DB::SQLite::Statement select(pData->pDB, sSQL.c_str());

			if (select.IsOK())
			{
				while (DB::SQLite::Statement::Result_Row == (nResult = select.Step(&status)))
				{
					pData->cSelects++;
				}
			}
			else
			{
				Print(stdout, "Select : {1}\n", select.GetInitStatus().GetMsg());
			}
			if (timer.GetElapsedTime() > pData->lfTime)
			{
				break;
			}
		}
	}

	return 0;
}

void DB_SQLite_TestMultiThread()
{
	DB::SQLite::Database    db;
	static const Size       WRITE_THREADS = 1;
	static const Size       READ_THREADS = 1;
	static const Size       TOTAL_THREADS = WRITE_THREADS + READ_THREADS;
	HANDLE                  threads[TOTAL_THREADS];
	WriterData              wd[WRITE_THREADS];
	ReaderData              rd[READ_THREADS];
	double                  lfReadTime = 10.0;
	double                  lfWriteTime = 10.0;
	Size                    cSelects;
	Size                    cInserts;
	Size                    cErrors;
	DWORD                   dwThreadID;
	Status                  status;

	unlink("testmt.db");
	if ((status = db.Open("testmt.db")).IsOK())
	{
		if ((status = db.Exec("CREATE TABLE [config] ([id] INTEGER PRIMARY KEY AUTOINCREMENT, "
			"[name] TEXT NOT NULL UNIQUE, [value] TEXT);")).IsOK())
		{
			Util::Timer timer;
			double      lfElapsed;

			timer.ResetTimer();
			for (Size i = 0; i < WRITE_THREADS; i++)
			{
				wd[i].pDB = &db;
				wd[i].cIndex = i + 1;
				wd[i].cInserts = 0;
				wd[i].cErrors = 0;
				wd[i].lfTime = lfWriteTime;
				threads[i] = CreateThread(NULL, 0, &WriterThread, &wd[i], 0, &dwThreadID);
			}
			for (Size i = 0; i < READ_THREADS; i++)
			{
				rd[i].pDB = &db;
				rd[i].cIndex = (i % WRITE_THREADS) + 1;
				rd[i].cSelects = 0;
				rd[i].lfTime = lfReadTime;
				threads[WRITE_THREADS + i] = CreateThread(NULL, 0, &ReaderThread, &rd[i], 0, &dwThreadID);
			}
			WaitForMultipleObjects(TOTAL_THREADS, threads, TRUE, INFINITE);
			lfElapsed = timer.GetElapsedTime();
			for (Size i = 0; i < TOTAL_THREADS; i++)
			{
				CloseHandle(threads[i]);
			}
			cInserts = 0;
			cErrors = 0;
			for (Size i = 0; i < WRITE_THREADS; i++)
			{
				cInserts += wd[i].cInserts;
				cErrors += wd[i].cErrors;
			}
			cSelects = 0;
			for (Size i = 0; i < READ_THREADS; i++)
			{
				cSelects += rd[i].cSelects;
			}
			Print(stdout, "Inserts : {1:.10} / second\n", cInserts / lfElapsed);
			Print(stdout, "Selects : {1:.10} / second\n", cSelects / lfElapsed);
			Print(stdout, "Errors  : {1}\n", cErrors);
		}
		else
		{
			Print(stdout, "Exec : {1}\n", status.GetMsg());
		}
		db.Close();
	}
	else
	{
		Print(stdout, "Open : {1}\n", status.GetMsg());
	}
}

class MyDBHelper : public DB::SQLite::DBHelper
{
public:

	static const Size STMT_INSERT = 0;
	static const Size STMT_SELECT = 1;

	virtual const Char *GetDDL() const
	{
		return DDL;
	}

	virtual const Char **GetStatements() const
	{
		return STATEMENTS;
	}

	virtual Size GetStatementsCount() const
	{
		return STATEMENTS_COUNT;
	}

	static const Char *DDL;
	static const Char *STATEMENTS[];
	static const Size STATEMENTS_COUNT;

};

const Char *MyDBHelper::DDL =
"CREATE TABLE IF NOT EXISTS [config] "
"( "
"   [id] INTEGER PRIMARY KEY AUTOINCREMENT, "
"   [name] TEXT NOT NULL UNIQUE, "
"   [value] TEXT "
");";

const Char *MyDBHelper::STATEMENTS[] =
{
	"INSERT INTO [config] ([name], [value]) VALUES (?, ?);",
	"SELECT * FROM [config] WHERE [name] like 'name-%';",
};

const Size MyDBHelper::STATEMENTS_COUNT = sizeof(MyDBHelper::STATEMENTS) / sizeof(MyDBHelper::STATEMENTS[0]);

void DB_SQLite_TestDBHelper()
{
	MyDBHelper dbh;
	Status     status;

	unlink("testdbh.db");
	if ((status = dbh.Open("testdbh.db")).IsOK())
	{
		Util::Timer          timer;
		double               lfElapsed;
		Size                 cCount     = 1000000;
		DB::SQLite::Bindings **bindings = (DB::SQLite::Bindings **)Mem::Alloc(sizeof(DB::SQLite::Bindings *) *cCount);

		for (Size i = 0; i < cCount; i++)
		{
			String sName;
			String sValue;

			Print(&sName, "name-{1}", i + 1);
			Print(&sValue, "value-{1}", i + 1);
			bindings[i] = dbh.CreateBindings();
			bindings[i]->AddString(sName.c_str());
			bindings[i]->AddString(sValue.c_str());
		}

		timer.ResetTimer();
		for (Size i = 0; i < cCount; i++)
		{
			if ((status = dbh.AddAsyncOperation(MyDBHelper::STMT_INSERT, bindings[i])).IsNOK())
			{
				dbh.DestroyBindings(bindings[i]);
			}
		}
		dbh.Close();

		Mem::Free(bindings);

		lfElapsed = timer.GetElapsedTime();
		Print(stdout, "Insert : {1:.3} inserts / second\n", cCount / lfElapsed);
	}
	else
	{
		Print(stdout, "Open failed with error {1}, message '{2}'\n", status.GetCode(), status.GetMsg());
	}
}

class MyDBHelper2 : public DB::SQLite::DBHelper
{
public:

	static const Size STMT_INSERT = 0;
	static const Size STMT_SELECT = 1;

	virtual const Char *GetDDL() const
	{
		return DDL;
	}

	virtual const Char **GetStatements() const
	{
		return STATEMENTS;
	}

	virtual Size GetStatementsCount() const
	{
		return STATEMENTS_COUNT;
	}

	static const Char *DDL;
	static const Char *STATEMENTS[];
	static const Size STATEMENTS_COUNT;

};

const Char *MyDBHelper2::DDL =
"CREATE TABLE IF NOT EXISTS [config] "
"( "
"   [id] INTEGER PRIMARY KEY AUTOINCREMENT, "
"   [name] TEXT NOT NULL UNIQUE, "
"   [value] TEXT "
");";

const Char *MyDBHelper2::STATEMENTS[] =
{
	"INSERT INTO [config] ([name], [value]) VALUES (?, ?);",
	"SELECT * FROM [config] WHERE [name] like 'name-%';",
};

const Size MyDBHelper2::STATEMENTS_COUNT = sizeof(MyDBHelper2::STATEMENTS) / sizeof(MyDBHelper2::STATEMENTS[0]);

struct ThreadData2
{
	MyDBHelper2 *pDBH;
	Size       cIndex;
	double     lfTime;
};

DWORD WINAPI WriterThread2(void *pArg)
{
	ThreadData2                *pData = (ThreadData2 *)pArg;
	MyDBHelper2::StatementScope scope(pData->pDBH, MyDBHelper2::STMT_INSERT);
	DB::SQLite::Statement::Result nResult;
	Size                       cIndex;
	Status                     status;
	Util::Timer                timer;

	cIndex = 0;
	while (timer.GetElapsedTime() < pData->lfTime)
	{
		//DB::SQLite::Transaction tr(&pData->pDBH->GetDB());

		String sName;
		String sValue;

		cIndex++;
		Print(&sName, "name-{1}-{2}", pData->cIndex, cIndex);
		Print(&sValue, "value-{1}-{2}", pData->cIndex, cIndex);

		if ((status = scope.GetStatement()->ClearBindings()).IsNOK())
		{
			Print(stdout, "ClearBindings : {1}, {2}\n", status.GetCode(), status.GetMsg());
		}
		if ((status = scope.GetStatement()->BindString(sName.c_str())).IsNOK())
		{
			Print(stdout, "Bind1 : {1}, {2}\n", status.GetCode(), status.GetMsg());
		}
		if ((status = scope.GetStatement()->BindString(sValue.c_str())).IsNOK())
		{
			Print(stdout, "Bind2 : {1}, {2}\n", status.GetCode(), status.GetMsg());
		}
		nResult = scope.GetStatement()->Step(&status);
		if (status.IsNOK())
		{
			Print(stdout, "Step : {1}, {2}\n", status.GetCode(), status.GetMsg());
		}
		if ((status = scope.GetStatement()->Reset()).IsNOK())
		{
			Print(stdout, "Reset : {1}, {2}\n", status.GetCode(), status.GetMsg());
		}
	}

	return 0;
}

DWORD WINAPI ReaderThread2(void *pArg)
{
	ThreadData2                *pData = (ThreadData2 *)pArg;
	MyDBHelper2::StatementScope scope(pData->pDBH, MyDBHelper2::STMT_SELECT);
	DB::SQLite::Statement::Result nResult;
	Status                     status;
	Util::Timer                timer;

	while (timer.GetElapsedTime() < pData->lfTime)
	{
		if ((status = scope.GetStatement()->ClearBindings()).IsNOK())
		{
			Print(stdout, "ClearBindings : {1}, {2}\n", status.GetCode(), status.GetMsg());
		}
		nResult = scope.GetStatement()->Step(&status);
		if (status.IsNOK())
		{
			Print(stdout, "Step : {1}, {2}\n", status.GetCode(), status.GetMsg());
		}
		scope.GetStatement()->GetInt(0);
		scope.GetStatement()->GetString(1);
		scope.GetStatement()->GetString(2);
		if ((status = scope.GetStatement()->Reset()).IsNOK())
		{
			Print(stdout, "Reset : {1}, {2}\n", status.GetCode(), status.GetMsg());
		}
	}

	return 0;
}

void DB_SQLite_TestDBHelperMultiThread()
{
	MyDBHelper2 dbh;
	static const Size       WRITE_THREADS = 32;
	static const Size       READ_THREADS = 32;
	static const Size       TOTAL_THREADS = WRITE_THREADS + READ_THREADS;
	HANDLE                  threads[TOTAL_THREADS];
	ThreadData2             wd[WRITE_THREADS];
	ThreadData2             rd[READ_THREADS];
	DWORD                   dwThreadID;
	Status                  status;

	unlink("testdbhmt.db");
	if ((status = dbh.Open("testdbhmt.db")).IsOK())
	{
		for (Size i = 0; i < WRITE_THREADS; i++)
		{
			wd[i].pDBH = &dbh;
			wd[i].cIndex = i + 1;
			wd[i].lfTime = 10.0;
			threads[i] = CreateThread(NULL, 0, &WriterThread2, &wd[i], 0, &dwThreadID);
		}
		for (Size i = 0; i < READ_THREADS; i++)
		{
			rd[i].pDBH = &dbh;
			rd[i].cIndex = (i % WRITE_THREADS) + 1;
			rd[i].lfTime = 10.0;
			threads[WRITE_THREADS + i] = CreateThread(NULL, 0, &ReaderThread2, &rd[i], 0, &dwThreadID);
		}
		WaitForMultipleObjects(TOTAL_THREADS, threads, TRUE, INFINITE);
		for (Size i = 0; i < TOTAL_THREADS; i++)
		{
			CloseHandle(threads[i]);
		}
		dbh.Close();
	}
	else
	{
		Print(stdout, "Open failed with error {1}, message '{2}'\n", status.GetCode(), status.GetMsg());
	}
}

REGISTER_TEST(DB_SQLite_Test);
REGISTER_TEST(DB_SQLite_TestMultiThread);
REGISTER_TEST(DB_SQLite_TestDBHelper);
REGISTER_TEST(DB_SQLite_TestDBHelperMultiThread);
