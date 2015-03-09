/* 
 * CX - C++ framework for general purpose developement
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

#include "CX/CX.hpp"
#include "CX/Data/JSON/DataWriter.hpp"
#include "CX/IO/MemOutputStream.hpp"
#include "CX/Print.hpp"
#include "CX/C/stdio.h"

#pragma warning(push)
#pragma warning(disable: 4702)
#include "../../../Contrib/Catch/Include/catch.hpp"
#pragma warning(pop)


using namespace CX;


const Char EXPECTED_JSON[] = 
"{\n"
"   \"null\": null,\n"
"   \"bool\": true,\n"
"   \"int\": 123,\n"
"   \"real\": 123.123000,\n"
"   \"string\": \"teststr\",\n"
"   \"wstring\": \"wstr://testwstr\",\n"
"   \"blob\": \"blob://4HelloWorld\",\n"
"   \"obj1\":\n"
"   {\n"
"      \"null\": null,\n"
"      \"bool\": true,\n"
"      \"int\": 123,\n"
"      \"real\": 123.123000,\n"
"      \"string\": \"teststr\",\n"
"      \"wstring\": \"wstr://testwstr\",\n"
"      \"blob\": \"blob://4HelloWorld\",\n"
"      \"arr1\":\n"
"      [\n"
"         null,\n"
"         true,\n"
"         123,\n"
"         123.123000,\n"
"         \"teststr\",\n"
"         \"wstr://testwstr\",\n"
"         \"blob://4HelloWorld\"\n"
"      ],\n"
"      \"obj1\":\n"
"      {\n"
"         \"null\": null,\n"
"         \"bool\": true,\n"
"         \"int\": 123,\n"
"         \"real\": 123.123000,\n"
"         \"string\": \"teststr\",\n"
"         \"wstring\": \"wstr://testwstr\",\n"
"         \"blob\": \"blob://4HelloWorld\"\n"
"      }\n"
"   },\n"
"   \"arr1\":\n"
"   [\n"
"      null,\n"
"      true,\n"
"      123,\n"
"      123.123000,\n"
"      \"teststr\",\n"
"      \"wstr://testwstr\",\n"
"      \"blob://4HelloWorld\",\n"
"      {\n"
"         \"null\": null,\n"
"         \"bool\": true,\n"
"         \"int\": 123,\n"
"         \"real\": 123.123000,\n"
"         \"string\": \"teststr\",\n"
"         \"wstring\": \"wstr://testwstr\",\n"
"         \"blob\": \"blob://4HelloWorld\"\n"
"      },\n"
"      [\n"
"         null,\n"
"         true,\n"
"         123,\n"
"         123.123000,\n"
"         \"teststr\",\n"
"         \"wstr://testwstr\",\n"
"         \"blob://4HelloWorld\"\n"
"      ]\n"
"   ]\n"
"}";

void WriteObjScalars(const Char *szName, Data::JSON::DataWriter &w)
{
	String   sTmp;
	Status   status;

	status = w.WriteNull("null");
	sTmp.clear();
	Print(&sTmp, "{1}: Write null", szName);
	SECTION(sTmp.c_str())
	{
		REQUIRE(Status_OK == status.GetCode());
	}

	status = w.WriteBool("bool", true);
	sTmp.clear();
	Print(&sTmp, "{1}: Write bool", szName);
	SECTION(sTmp.c_str())
	{
		REQUIRE(Status_OK == status.GetCode());
	}

	status = w.WriteInt("int", 123);
	sTmp.clear();
	Print(&sTmp, "{1}: Write int", szName);
	SECTION(sTmp.c_str())
	{
		REQUIRE(Status_OK == status.GetCode());
	}

	status = w.WriteReal("real", 123.123);
	sTmp.clear();
	Print(&sTmp, "{1}: Write real", szName);
	SECTION(sTmp.c_str())
	{
		REQUIRE(Status_OK == status.GetCode());
	}

	status = w.WriteString("string", "teststr");
	sTmp.clear();
	Print(&sTmp, "{1}: Write string", szName);
	SECTION(sTmp.c_str())
	{
		REQUIRE(Status_OK == status.GetCode());
	}

	status = w.WriteWString("wstring", L"testwstr");
	sTmp.clear();
	Print(&sTmp, "{1}: Write wstring", szName);
	SECTION(sTmp.c_str())
	{
		REQUIRE(Status_OK == status.GetCode());
	}

	status = w.WriteBLOB("blob", "\x86\x4F\xD2\x6F\xB5\x59\xF7\x5B", cx_strlen("\x86\x4F\xD2\x6F\xB5\x59\xF7\x5B"));
	sTmp.clear();
	Print(&sTmp, "{1}: Write blob", szName);
	SECTION(sTmp.c_str())
	{
		REQUIRE(Status_OK == status.GetCode());
	}
}

void WriteArrScalars(const Char *szName, Data::JSON::DataWriter &w)
{
	String   sTmp;
	Status   status;

	status = w.WriteNull();
	sTmp.clear();
	Print(&sTmp, "{1}: Write null", szName);
	SECTION(sTmp.c_str())
	{
		REQUIRE(Status_OK == status.GetCode());
	}

	status = w.WriteBool(true);
	sTmp.clear();
	Print(&sTmp, "{1}: Write bool", szName);
	SECTION(sTmp.c_str())
	{
		REQUIRE(Status_OK == status.GetCode());
	}

	status = w.WriteInt(123);
	sTmp.clear();
	Print(&sTmp, "{1}: Write int", szName);
	SECTION(sTmp.c_str())
	{
		REQUIRE(Status_OK == status.GetCode());
	}

	status = w.WriteReal(123.123);
	sTmp.clear();
	Print(&sTmp, "{1}: Write real", szName);
	SECTION(sTmp.c_str())
	{
		REQUIRE(Status_OK == status.GetCode());
	}

	status = w.WriteString("teststr");
	sTmp.clear();
	Print(&sTmp, "{1}: Write string", szName);
	SECTION(sTmp.c_str())
	{
		REQUIRE(Status_OK == status.GetCode());
	}

	status = w.WriteWString(L"testwstr");
	sTmp.clear();
	Print(&sTmp, "{1}: Write wstring", szName);
	SECTION(sTmp.c_str())
	{
		REQUIRE(Status_OK == status.GetCode());
	}

	status = w.WriteBLOB("\x86\x4F\xD2\x6F\xB5\x59\xF7\x5B", cx_strlen("\x86\x4F\xD2\x6F\xB5\x59\xF7\x5B"));
	sTmp.clear();
	Print(&sTmp, "{1}: Write blob", szName);
	SECTION(sTmp.c_str())
	{
		REQUIRE(Status_OK == status.GetCode());
	}
}


TEST_CASE("Data JSON DataWriter", "[CX::Data::JSON::DataWriter]")
{
	String                   sOutput;

	{
		IO::MemOutputStream      mos(&sOutput);
		Data::JSON::DataWriter   w(&mos);
		Status                   status;

		status = w.BeginRootObject();
		SECTION("Begin root object")
		{
			REQUIRE(Status_OK == status.GetCode());
		}

		WriteObjScalars(".", w);

		status = w.BeginObject("obj1");
		SECTION(".obj1: Begin object")
		{
			REQUIRE(Status_OK == status.GetCode());
		}

		WriteObjScalars(".obj1", w);

		status = w.BeginArray("arr1");
		SECTION(".obj1.arr1: Begin array")
		{
			REQUIRE(Status_OK == status.GetCode());
		}

		WriteArrScalars(".obj1.arr1", w);

		status = w.EndArray();
		SECTION(".obj1.arr1: End array")
		{
			REQUIRE(Status_OK == status.GetCode());
		}

		status = w.BeginObject("obj1");
		SECTION(".obj1.obj1: Begin object")
		{
			REQUIRE(Status_OK == status.GetCode());
		}

		WriteObjScalars(".obj1.obj1", w);

		status = w.EndObject();
		SECTION(".obj1.obj1: End object")
		{
			REQUIRE(Status_OK == status.GetCode());
		}

		status = w.EndObject();
		SECTION(".obj1: End object")
		{
			REQUIRE(Status_OK == status.GetCode());
		}

		status = w.BeginArray("arr1");
		SECTION(".arr1: Begin array")
		{
			REQUIRE(Status_OK == status.GetCode());
		}

		WriteArrScalars(".arr1", w);

		status = w.BeginObject();
		SECTION(".arr1.obj1: Begin object")
		{
			REQUIRE(Status_OK == status.GetCode());
		}

		WriteObjScalars(".arr1.obj1", w);

		status = w.EndObject();
		SECTION(".arr1.obj1: End object")
		{
			REQUIRE(Status_OK == status.GetCode());
		}

		status = w.BeginArray();
		SECTION(".arr1.arr1: Begin array")
		{
			REQUIRE(Status_OK == status.GetCode());
		}

		WriteArrScalars(".arr1.arr1", w);

		status = w.EndArray();
		SECTION(".arr1.arr1: End array")
		{
			REQUIRE(Status_OK == status.GetCode());
		}

		status = w.EndArray();
		SECTION(".arr1: End array")
		{
			REQUIRE(Status_OK == status.GetCode());
		}

		status = w.EndRootObject();
		SECTION("End root object")
		{
			REQUIRE(Status_OK == status.GetCode());
		}
	}

	SECTION("Compare output")
	{
		REQUIRE(sOutput == EXPECTED_JSON);
	}
}

