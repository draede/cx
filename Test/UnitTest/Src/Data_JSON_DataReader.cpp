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
#include "CX/Data/JSON/DataReader.hpp"
#include "CX/Data/JSON/DataWriter.hpp"
#include "CX/IO/MemInputStream.hpp"
#include "CX/IO/MemOutputStream.hpp"
#include "CX/IO/FileInputStream.hpp"
#include "CX/IO/Helper.hpp"
#include "CX/Util/MemPool.hpp"
#include "CX/Print.hpp"
#include "CX/C/stdio.h"

#pragma warning(push)
#pragma warning(disable: 4702)
#include "../../../Contrib/Catch/Include/catch.hpp"
#pragma warning(pop)


using namespace CX;


const Char INPUT_JSON[] =
"{\n"
"   \"null\": null,\n"
"   \"bool\": true,\n"
"   \"int\": 123,\n"
"   \"real\": 123.123000,\n"
"   \"string\": \"teststr\",\n"
"   \"wstring\": \"testwstr\",\n"
"   \"blob\": \"blob://4HelloWorld\",\n"
"   \"obj1\":\n"
"   {\n"
"      \"null\": null,\n"
"      \"bool\": true,\n"
"      \"int\": 123,\n"
"      \"real\": 123.123000,\n"
"      \"string\": \"teststr\",\n"
"      \"wstring\": \"testwstr\",\n"
"      \"blob\": \"blob://4HelloWorld\",\n"
"      \"arr1\":\n"
"      [\n"
"         null,\n"
"         true,\n"
"         123,\n"
"         123.123000,\n"
"         \"teststr\",\n"
"         \"testwstr\",\n"
"         \"blob://4HelloWorld\"\n"
"      ],\n"
"      \"obj1\":\n"
"      {\n"
"         \"null\": null,\n"
"         \"bool\": true,\n"
"         \"int\": 123,\n"
"         \"real\": 123.123000,\n"
"         \"string\": \"teststr\",\n"
"         \"wstring\": \"testwstr\",\n"
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
"      \"testwstr\",\n"
"      \"blob://4HelloWorld\",\n"
"      {\n"
"         \"null\": null,\n"
"         \"bool\": true,\n"
"         \"int\": 123,\n"
"         \"real\": 123.123000,\n"
"         \"string\": \"teststr\",\n"
"         \"wstring\": \"testwstr\",\n"
"         \"blob\": \"blob://4HelloWorld\"\n"
"      },\n"
"      [\n"
"         null,\n"
"         true,\n"
"         123,\n"
"         123.123000,\n"
"         \"teststr\",\n"
"         \"testwstr\",\n"
"         \"blob://4HelloWorld\"\n"
"      ]\n"
"   ]\n"
"}";

Status HandleObject(Data::JSON::DataReader &r, Data::JSON::DataWriter &w);
Status HandleArray(Data::JSON::DataReader &r, Data::JSON::DataWriter &w);

Status HandleObject(Data::JSON::DataReader &r, Data::JSON::DataWriter &w)
{
	Data::JSON::DataReader::EntryType nEntryType;
	bool                              bOK;
	Status                            status;

	bOK = true;
	while (bOK)
	{
		nEntryType = r.GetEntryType();
		switch (nEntryType)
		{
			case Data::JSON::DataReader::EntryType_Invalid:
			{
				bOK = false;
			}
			break;
			case Data::JSON::DataReader::EntryType_EOG:
			{
				bOK = false;
			}
			break;
			case Data::JSON::DataReader::EntryType_Null:
			{
				String sName;

				status = r.ReadNull(&sName);
				SECTION("Object ReadNull")
				{
					REQUIRE(Status_OK == status.GetCode());
				}
				status = w.WriteNull(sName.c_str());
				SECTION("Object WriteNull")
				{
					REQUIRE(Status_OK == status.GetCode());
				}
			}
			break;
			case Data::JSON::DataReader::EntryType_Bool:
			{
				String sName;
				bool   val;

				status = r.ReadBool(&sName, &val);
				SECTION("Object ReadBool")
				{
					REQUIRE(Status_OK == status.GetCode());
				}
				status = w.WriteBool(sName.c_str(), val);
				SECTION("Object WriteBool")
				{
					REQUIRE(Status_OK == status.GetCode());
				}
			}
			break;
			case Data::JSON::DataReader::EntryType_Int:
			{
				String sName;
				Int64  val;

				status = r.ReadInt(&sName, &val);
				SECTION("Object ReadInt")
				{
					REQUIRE(Status_OK == status.GetCode());
				}
				status = w.WriteInt(sName.c_str(), val);
				SECTION("Object WriteInt")
				{
					REQUIRE(Status_OK == status.GetCode());
				}
			}
			break;
			case Data::JSON::DataReader::EntryType_Real:
			{
				String sName;
				Double val;

				status = r.ReadReal(&sName, &val);
				SECTION("Object ReadReal")
				{
					REQUIRE(Status_OK == status.GetCode());
				}
				status = w.WriteReal(sName.c_str(), val);
				SECTION("Object WriteReal")
				{
					REQUIRE(Status_OK == status.GetCode());
				}
			}
			break;
			case Data::JSON::DataReader::EntryType_String:
			{
				String sName;
				String val;

				status = r.ReadString(&sName, &val);
				SECTION("Object ReadString")
				{
					REQUIRE(Status_OK == status.GetCode());
				}
				status = w.WriteString(sName.c_str(), val.c_str());
				SECTION("Object WriteString")
				{
					REQUIRE(Status_OK == status.GetCode());
				}
			}
			break;
			case Data::JSON::DataReader::EntryType_BLOB:
			{
				String   sName;
				void     *pBLOB;
				Size     cbBLOBSize;
			
				status = r.ReadBLOB(&sName, &pBLOB, &cbBLOBSize);
				SECTION("Object ReadBLOB")
				{
					REQUIRE(Status_OK == status.GetCode());
				}
				status = w.WriteBLOB(sName.c_str(), pBLOB, cbBLOBSize);
				SECTION("Object WriteBLOB")
				{
					REQUIRE(Status_OK == status.GetCode());
				}
			}
			break;
			case Data::JSON::DataReader::EntryType_Object:
			{
				String sName;

				status = r.BeginObject(&sName);
				SECTION("Object BeginObject")
				{
					REQUIRE(Status_OK == status.GetCode());
				}
				status = w.BeginObject(sName.c_str());
				SECTION("Object BeginObject")
				{
					REQUIRE(Status_OK == status.GetCode());
				}
				if ((status = HandleObject(r, w)).IsNOK())
				{
					return status;
				}
				status = r.EndObject();
				SECTION("Object EndObject")
				{
					REQUIRE(Status_OK == status.GetCode());
				}
				status = w.EndObject();
				SECTION("Object EndObject")
				{
					REQUIRE(Status_OK == status.GetCode());
				}
			}
			break;
			case Data::JSON::DataReader::EntryType_Array:
			{
				String sName;

				status = r.BeginArray(&sName);
				SECTION("Object BeginArray")
				{
					REQUIRE(Status_OK == status.GetCode());
				}
				status = w.BeginArray(sName.c_str());
				SECTION("Object BeginArray")
				{
					REQUIRE(Status_OK == status.GetCode());
				}
				if ((status = HandleArray(r, w)).IsNOK())
				{
					return status;
				}
				status = r.EndArray();
				SECTION("Object EndArray")
				{
					REQUIRE(Status_OK == status.GetCode());
				}
				status = w.EndArray();
				SECTION("Object EndArray")
				{
					REQUIRE(Status_OK == status.GetCode());
				}
			}
			break;
			default:
			{
				return Status(Status_InvalidArg);
			}
			break;
		}
	}

	return Status();
}

Status HandleArray(Data::JSON::DataReader &r, Data::JSON::DataWriter &w)
{
	Data::JSON::DataReader::EntryType nEntryType;
	bool                              bOK;
	Status                            status;

	bOK = true;
	while (bOK)
	{
		nEntryType = r.GetEntryType();
		switch (nEntryType)
		{
			case Data::JSON::DataReader::EntryType_Invalid:
			{
				bOK = false;
			}
			break;
			case Data::JSON::DataReader::EntryType_EOG:
			{
				bOK = false;
			}
			break;
			case Data::JSON::DataReader::EntryType_Null:
			{
				status = r.ReadNull();
				SECTION("Array ReadNull")
				{
					REQUIRE(Status_OK == status.GetCode());
				}
				status = w.WriteNull();
				SECTION("Array WriteNull")
				{
					REQUIRE(Status_OK == status.GetCode());
				}
			}
			break;
			case Data::JSON::DataReader::EntryType_Bool:
			{
				bool   val;

				status = r.ReadBool(&val);
				SECTION("Array ReadBool")
				{
					REQUIRE(Status_OK == status.GetCode());
				}
				status = w.WriteBool(val);
				SECTION("Array WriteBool")
				{
					REQUIRE(Status_OK == status.GetCode());
				}
			}
			break;
			case Data::JSON::DataReader::EntryType_Int:
			{
				Int64  val;

				status = r.ReadInt(&val);
				SECTION("Array ReadInt")
				{
					REQUIRE(Status_OK == status.GetCode());
				}
				status = w.WriteInt(val);
				SECTION("Array WriteInt")
				{
					REQUIRE(Status_OK == status.GetCode());
				}
			}
			break;
			case Data::JSON::DataReader::EntryType_Real:
			{
				Double val;

				status = r.ReadReal(&val);
				SECTION("Array ReadReal")
				{
					REQUIRE(Status_OK == status.GetCode());
				}
				status = w.WriteReal(val);
				SECTION("Array WriteReal")
				{
					REQUIRE(Status_OK == status.GetCode());
				}
			}
			break;
			case Data::JSON::DataReader::EntryType_String:
			{
				String val;

				status = r.ReadString(&val);
				SECTION("Array ReadString")
				{
					REQUIRE(Status_OK == status.GetCode());
				}
				status = w.WriteString(val.c_str());
				SECTION("Array WriteString")
				{
					REQUIRE(Status_OK == status.GetCode());
				}
			}
			break;
			case Data::JSON::DataReader::EntryType_BLOB:
			{
				void     *pBLOB;
				Size     cbBLOBSize;

				status = r.ReadBLOB(&pBLOB, &cbBLOBSize);
				SECTION("Object ReadBLOB")
				{
					REQUIRE(Status_OK == status.GetCode());
				}
				status = w.WriteBLOB(pBLOB, cbBLOBSize);
				SECTION("Object WriteBLOB")
				{
					REQUIRE(Status_OK == status.GetCode());
				}
			}
			break;
			case Data::JSON::DataReader::EntryType_Object:
			{
				status = r.BeginObject();
				SECTION("Array BeginObject")
				{
					REQUIRE(Status_OK == status.GetCode());
				}
				status = w.BeginObject();
				SECTION("Array BeginObject")
				{
					REQUIRE(Status_OK == status.GetCode());
				}
				if ((status = HandleObject(r, w)).IsNOK())
				{
					return status;
				}
				status = r.EndObject();
				SECTION("Array EndObject")
				{
					REQUIRE(Status_OK == status.GetCode());
				}
				status = w.EndObject();
				SECTION("Array EndObject")
				{
					REQUIRE(Status_OK == status.GetCode());
				}
			}
			break;
			case Data::JSON::DataReader::EntryType_Array:
			{
				status = r.BeginArray();
				SECTION("Array BeginArray")
				{
					REQUIRE(Status_OK == status.GetCode());
				}
				status = w.BeginArray();
				SECTION("Array BeginArray")
				{
					REQUIRE(Status_OK == status.GetCode());
				}
				if ((status = HandleArray(r, w)).IsNOK())
				{
					return status;
				}
				status = r.EndArray();
				SECTION("Array EndArray")
				{
					REQUIRE(Status_OK == status.GetCode());
				}
				status = w.EndArray();
				SECTION("Array EndArray")
				{
					REQUIRE(Status_OK == status.GetCode());
				}
			}
			break;
			default:
			{
				return Status(Status_InvalidArg);
			}
			break;
		}
	}

	return Status();
}

enum Type
{
	Type_Invalid,
	Type_Null,
	Type_Bool,
	Type_Int,
	Type_Real,
	Type_String,
	Type_Object,
	Type_Array,
};

TEST_CASE("Data JSON DataReader", "[CX::Data::JSON::DataReader]")
{
	{
		String                 sOutput;

		{
			IO::MemInputStream     mis(INPUT_JSON, cx_strlen(INPUT_JSON));
			Data::JSON::DataReader r(&mis);
			IO::MemOutputStream    mos(&sOutput);
			Data::JSON::DataWriter w(&mos);
			Status                 status;

			SECTION("Check parsing")
			{
				REQUIRE(Data::JSON::DataReader::EntryType_Invalid != r.GetRootEntryType());
			}

			if (Data::JSON::DataReader::EntryType_Object == r.GetRootEntryType())
			{
				status = r.BeginRootObject();
				SECTION("Reader BeginRootObject")
				{
					REQUIRE(Status_OK == status.GetCode());
				}
				status = w.BeginRootObject();
				SECTION("Writer BeginRootObject")
				{
					REQUIRE(Status_OK == status.GetCode());
				}
				status = HandleObject(r, w);
				SECTION("HandleObject")
				{
					REQUIRE(Status_OK == status.GetCode());
				}
			}
			else
			if (Data::JSON::DataReader::EntryType_Array == r.GetRootEntryType())
			{
				status = r.BeginRootArray();
				SECTION("Reader BeginRootArray")
				{
					REQUIRE(Status_OK == status.GetCode());
				}
				status = w.BeginRootArray();
				SECTION("Writer BeginRootArray")
				{
					REQUIRE(Status_OK == status.GetCode());
				}
				status = HandleArray(r, w);
				SECTION("HandleArray")
				{
					REQUIRE(Status_OK == status.GetCode());
				}
			}

			if (Data::JSON::DataReader::EntryType_Object == r.GetRootEntryType())
			{
				status = r.EndRootObject();
				SECTION("Reader EndRootObject")
				{
					REQUIRE(Status_OK == status.GetCode());
				}
				status = w.EndRootObject();
				SECTION("Writer EndRootObject")
				{
					REQUIRE(Status_OK == status.GetCode());
				}
			}
			else
			if (Data::JSON::DataReader::EntryType_Array == r.GetRootEntryType())
			{
				status = r.EndRootArray();
				SECTION("Reader EndRootArray")
				{
					REQUIRE(Status_OK == status.GetCode());
				}
				status = w.EndRootArray();
				SECTION("Writer EndRootArray")
				{
					REQUIRE(Status_OK == status.GetCode());
				}
			}
		}

		SECTION("Compare output")
		{
			REQUIRE(sOutput == INPUT_JSON);
		}

	}

	SECTION("testbed")
	{
		const Size cStart = 1;
		const Size cEnd = 40;

		for (Size i = cStart; i <= cEnd; i++)
		{
			String     sOutput1;
			String     sOutput2;
			Status     status;

			//Print(stdout, "JSON {1}\n", i);

			String sName;
			String sPath;

			Print(&sPath, "{1}.json", i);

			IO::FileInputStream fis(sPath.c_str());

			{
				Data::JSON::DataReader     json_reader(&fis);
				IO::MemOutputStream        mos(&sOutput1);
				Data::JSON::DataWriter     json_writer(&mos);

				status = IO::Helper::CopyData(&json_reader, &json_writer);
			}

			Print(&sName, "TestData_CopyData1_{1}", i);
			SECTION(sName.c_str())
			{
				REQUIRE(Status_OK == status.GetCode());
			}

			{
				IO::MemInputStream         mis(&sOutput1);
				Data::JSON::DataReader     json_reader(&mis);
				IO::MemOutputStream        mos(&sOutput2);
				Data::JSON::DataWriter     json_writer(&mos);

				status = IO::Helper::CopyData(&json_reader, &json_writer);
			}

			Print(&sName, "TestData_CopyData2_{1}", i);
			SECTION(sName.c_str())
			{
				REQUIRE(Status_OK == status.GetCode());
			}

			Print(&sName, "TestData_Compareoutput_{1}", i);
			SECTION(sName.c_str())
			{
				REQUIRE(sOutput1 == sOutput2);
			}
		}
	}
}

