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
#include "CX/Data/JSON/DataReader.hpp"
#include "CX/Data/BINJSON/DataWriter.hpp"
#include "CX/Data/BINJSON/DataReader.hpp"
#include "CX/IO/Helper.hpp"
#include "CX/IO/MemInputStream.hpp"
#include "CX/IO/MemOutputStream.hpp"
#include "CX/IO/FileInputStream.hpp"
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

TEST_CASE("Data BINJSON DataWriter", "[CX::Data::BINJSON::DataWriter]")
{
	{
		String   sOutput1;
		String   sOutput2;
		Status   status;

		{
			IO::MemInputStream         mis(INPUT_JSON, cx_strlen(INPUT_JSON));
			Data::JSON::DataReader     json_reader(&mis);
			IO::MemOutputStream        mos(&sOutput1);
			Data::BINJSON::DataWriter  binjson_writer(&mos);

			status = IO::Helper::CopyData(&json_reader, &binjson_writer);
		}

		SECTION("CopyData1")
		{
			REQUIRE(Status_OK == status.GetCode());
		}

		{
			IO::MemInputStream         mis(&sOutput1);
			Data::BINJSON::DataReader  binjson_reader(&mis);
			IO::MemOutputStream        mos(&sOutput2);
			Data::BINJSON::DataWriter  binjson_writer(&mos);

			status = IO::Helper::CopyData(&binjson_reader, &binjson_writer);
		}

		SECTION("CopyData2")
		{
			REQUIRE(Status_OK == status.GetCode());
		}

		SECTION("Compare output")
		{
			REQUIRE(sOutput1 == sOutput2);
		}
	}

	SECTION("testbed")
	{
		const Size cStart = 1;
		const Size cEnd   = 40;

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
				Data::BINJSON::DataWriter  binjson_writer(&mos);

				status = IO::Helper::CopyData(&json_reader, &binjson_writer);
			}

			Print(&sName, "TestData_CopyData1_{1}", i);
			SECTION(sName.c_str())
			{
				REQUIRE(Status_OK == status.GetCode());
			}

			{
				IO::MemInputStream         mis(&sOutput1);
				Data::BINJSON::DataReader  binjson_reader(&mis);
				IO::MemOutputStream        mos(&sOutput2);
				Data::BINJSON::DataWriter  binjson_writer(&mos);

				status = IO::Helper::CopyData(&binjson_reader, &binjson_writer);
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

