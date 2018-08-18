/* 
 * CX - C++ framework for general purpose development
 *
 * https://github.com/draede/cx
 * 
 * Copyright (C) 2014 - 2018 draede - draede [at] outlook [dot] com
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

#include "CX/DB/Flat/Writer.hpp"
#include "CX/DB/Flat/Reader.hpp"
#include "Tester.hpp"
#include "CX/Print.hpp"


using namespace CX;


Status EnumRecordsHandler(void *pUsrCtx, UInt32 cIndex, const void *pData, UInt32 cbSize)
{
	cIndex;
	pData;
	cbSize;

	(*(UInt32 *)pUsrCtx)++;

	return Status();
}

void DB_Flat_Test_NoCustomHeader_FixedRecordSize()
{
	Status   status;

	{
		DB::Flat::Writer   writer;

		if ((status = writer.CreateOrOpen("flat_fixed.db", 0x34333231, 0, 4)))
		{
			if (!(status = writer.AddRecord("1111", 4)))
			{
				Print(stdout, "AddRecord : {1} ({2})\n", status.GetCode(), status.GetMsg());
			}
			if (!(status = writer.AddRecord("2222", 4)))
			{
				Print(stdout, "AddRecord : {1} ({2})\n", status.GetCode(), status.GetMsg());
			}
			if (!(status = writer.AddRecord("3333", 4)))
			{
				Print(stdout, "AddRecord : {1} ({2})\n", status.GetCode(), status.GetMsg());
			}
			if (!(status = writer.AddRecord("4444", 4)))
			{
				Print(stdout, "AddRecord : {1} ({2})\n", status.GetCode(), status.GetMsg());
			}

			if (!(status = writer.Close()))
			{
				Print(stdout, "Close : {1} ({2})\n", status.GetCode(), status.GetMsg());
			}
		}
		else
		{
			Print(stdout, "CreateOrOpen : {1} ({2})\n", status.GetCode(), status.GetMsg());
		}
	}
	{
		DB::Flat::Reader   reader;
		UInt32             cCount;
		UInt32             cIndex;
		const void         *pData;
		UInt32             cbSize;

		if ((status = reader.Open("flat_fixed.db")))
		{
			cCount = reader.GetRecordCount();

			for (cIndex = 0; cIndex < cCount; cIndex++)
			{
				if (NULL == (pData = reader.GetRecord(cIndex, &cbSize)))
				{
					Print(stdout, "GetRecord({1}) : {2} ({3})\n", cIndex, status.GetCode(), status.GetMsg());
				}
			}

			cIndex = 0;
			if (!(status = reader.EnumerateRecords(&EnumRecordsHandler, &cIndex)))
			{
				Print(stdout, "EnumerateRecords : {1} ({2})\n", status.GetCode(), status.GetMsg());
			}
			if (cIndex != cCount)
			{
				Print(stdout, "EnumerateRecords : {1} != {2}\n", cIndex, cCount);
			}

			if (!(status = reader.Close()))
			{
				Print(stdout, "Close : {1} ({2})\n", status.GetCode(), status.GetMsg());
			}
		}
		else
		{
			Print(stdout, "CreateOrOpen : {1} ({2})\n", status.GetCode(), status.GetMsg());
		}
	}

}

void DB_Flat_Test_CustomHeader_VariableRecordSize()
{
	Status   status;

	{
		DB::Flat::Writer   writer;

		if ((status = writer.CreateOrOpen("flat_variable.db", 0x34333231, 10, 4)))
		{
			if (!(status = writer.AddRecord("1111", 4)))
			{
				Print(stdout, "AddRecord : {1} ({2})\n", status.GetCode(), status.GetMsg());
			}
			if (!(status = writer.AddRecord("2222", 4)))
			{
				Print(stdout, "AddRecord : {1} ({2})\n", status.GetCode(), status.GetMsg());
			}
			if (!(status = writer.AddRecord("3333", 4)))
			{
				Print(stdout, "AddRecord : {1} ({2})\n", status.GetCode(), status.GetMsg());
			}
			if (!(status = writer.AddRecord("4444", 4)))
			{
				Print(stdout, "AddRecord : {1} ({2})\n", status.GetCode(), status.GetMsg());
			}

			if (!(status = writer.Close("0123456789", 10)))
			{
				Print(stdout, "Close : {1} ({2})\n", status.GetCode(), status.GetMsg());
			}
		}
		else
		{
			Print(stdout, "CreateOrOpen : {1} ({2})\n", status.GetCode(), status.GetMsg());
		}
	}
	{
		DB::Flat::Reader   reader;
		UInt32             cCount;
		UInt32             cIndex;
		const void         *pData;
		UInt32             cbSize;

		if ((status = reader.Open("flat_variable.db")))
		{
			cCount = reader.GetRecordCount();

			for (cIndex = 0; cIndex < cCount; cIndex++)
			{
				if (NULL == (pData = reader.GetRecord(cIndex, &cbSize)))
				{
					Print(stdout, "GetRecord({1}) : {2} ({3})\n", cIndex, status.GetCode(), status.GetMsg());
				}
			}

			cIndex = 0;
			if (!(status = reader.EnumerateRecords(&EnumRecordsHandler, &cIndex)))
			{
				Print(stdout, "EnumerateRecords : {1} ({2})\n", status.GetCode(), status.GetMsg());
			}
			if (cIndex != cCount)
			{
				Print(stdout, "EnumerateRecords : {1} != {2}\n", cIndex, cCount);
			}

			if (!(status = reader.Close()))
			{
				Print(stdout, "Close : {1} ({2})\n", status.GetCode(), status.GetMsg());
			}
		}
		else
		{
			Print(stdout, "CreateOrOpen : {1} ({2})\n", status.GetCode(), status.GetMsg());
		}
	}

}

REGISTER_TEST(DB_Flat_Test_NoCustomHeader_FixedRecordSize);
REGISTER_TEST(DB_Flat_Test_CustomHeader_VariableRecordSize);
