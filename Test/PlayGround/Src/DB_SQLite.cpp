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

#include "CX/DB/SQLite/Database.hpp"
#include "CX/DB/SQLite/Transaction.hpp"
#include "CX/DB/SQLite/Statement.hpp"
#include "CX/DB/SQLite/Bindings.hpp"
#include "CX/Print.hpp"
#include "CX/Util/Timer.hpp"
#include <windows.h>
#include "Tester.hpp"


using namespace CX;


void DB_SQLite_Test1()
{
	Print(stdout, "Begin DB SQLite test\n");

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
						insert.Bind(DB::SQLite::Bindings("ss", "name3", "value3"));
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
						insert.ClearBindings();
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

	Print(stdout, "End DB SQLite test\n");
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
						insert.BindString(sText.c_str(), insert.ArgStore_Transient);
					}
					{
						String sText;

						Print(&sText, "value-{1}-{2}", pData->cIndex, cLocalIndex);
						insert.BindString(sText.c_str(), insert.ArgStore_Transient);
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

	unlink("test.db");
	if ((status = db.Open("test.db")).IsOK())
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

REGISTER_TEST(DB_SQLite_Test1);
//REGISTER_TEST(DB_SQLite_TestMultiThread);
