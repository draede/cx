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

#include "Tester.hpp"


using namespace CX;


#include "CX/DB/SQLite/DBHelper.hpp"
#include "CX/Print.hpp"
#include "CX/Util/Timer.hpp"


class MyDBHelper : public DB::SQLite::DBHelper
{
public:

	static const Size STMT_INSERT = 0;
	static const Size STMT_SELECT = 0;

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

int main(int argc, char *argv[])
{
	CX_UNUSED(argc);
	CX_UNUSED(argv);

#if 1

	MyDBHelper dbh;
	Status     status;

	unlink("testdbh.db");
	if ((status = dbh.Open("testdbh.db")).IsOK())
	{
		Util::Timer timer;
		double      lfElapsed;
		Size        cCount = 1000000;
		DB::SQLite::Bindings **bindings = (DB::SQLite::Bindings **)malloc(sizeof(DB::SQLite::Bindings *) *cCount);

		for (Size i = 0; i < cCount; i++)
		{
			String sName;
			String sValue;

			Print(&sName, "name-{1}", i + 1);
			Print(&sValue, "value-{1}", i + 1);
			bindings[i] = dbh.CreateBindings("ss", sName.c_str(), sValue.c_str());
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
		
		free(bindings);
		
		lfElapsed = timer.GetElapsedTime();
		Print(stdout, "Insert : {1:.3} inserts / second\n", cCount / lfElapsed);
	}
	else
	{
		Print(stdout, "Open failed with error {1}, message '{2}'\n", status.GetCode(), status.GetMsg());
	}

#endif

	//Tester::Run();

	return 0;
}

