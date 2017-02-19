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

#include "CX/CX.hpp"
#include "CX/Var.hpp"
#include "CX/Print.hpp"
#include "CX/IO/FileInputStream.hpp"
#include "CX/Util/MemPool.hpp"


#pragma warning(push)
#pragma warning(disable: 4702)
#include "../../../Contrib/Catch/Include/catch.hpp"
#pragma warning(pop)


using namespace CX;


TEST_CASE("Var tests", "[CX::Var]")
{
	SECTION("Constructor ()")
	{
		Var v;
		
		REQUIRE(v.IsNull());
	}
	SECTION("Constructor (Bool)")
	{
		Var v(True);

		REQUIRE(v.IsBool());
		REQUIRE(True == v.GetBool());
	}
	SECTION("Constructor (Int64)")
	{
		Var v((Int64)12345);

		REQUIRE(v.IsInt());
		REQUIRE(12345 == v.GetInt());
	}
	SECTION("Constructor (Double)")
	{
		Var v(123.456);

		REQUIRE(v.IsReal());
		REQUIRE(123.456 == v.GetReal());
	}

	SECTION("Constructor (const Char *)")
	{
		Var v("abc");

		REQUIRE(v.IsString());
		REQUIRE(String("abc") == v.GetString());
	}
	SECTION("Constructor (String)")
	{
		Var v(String("abc"));

		REQUIRE(v.IsString());
		REQUIRE(String("abc") == v.GetString());
	}
	SECTION("var = False")
	{
		Var v;

		v = False;
		REQUIRE(v.IsBool());
		REQUIRE(False == v.GetBool());
	}
	SECTION("var = 123")
	{
		Var v;

		v = (Int64)123;
		REQUIRE(v.IsInt());
		REQUIRE(123 == v.GetInt());
	}
	SECTION("var = 123.456")
	{
		Var v;

		v = 123.456;
		REQUIRE(v.IsReal());
		REQUIRE(123.456 == v.GetReal());
	}
	SECTION("var = \"abc\"")
	{
		Var v;

		v = "abc";
		REQUIRE(v.IsString());
		REQUIRE(String("abc") == v.GetString());
	}
	SECTION("var = String(\"abc\")")
	{
		Var v;

		v = String("abc");
		REQUIRE(v.IsString());
		REQUIRE(String("abc") == v.GetString());
	}
	SECTION("var[\"a\"] = False")
	{
		Var v;

		v["a"] = False;
		REQUIRE(v.IsObject());
		REQUIRE(True == v.IsObjectMember("a"));
		REQUIRE(v["a"].IsBool());
		REQUIRE(False == v["a"].GetBool());
	}
	SECTION("var[\"a\"][\"b\"] = \"abc\"")
	{
		Var v;

		v["a"]["b"] = "abc";
		REQUIRE(v.IsObject());
		REQUIRE(True == v.IsObjectMember("a"));
		REQUIRE(v["a"].IsObject());
		REQUIRE(True == v["a"].IsObjectMember("b"));
		REQUIRE(v["a"]["b"].IsString());
		REQUIRE(String("abc") == v["a"]["b"].GetString());
	}
	SECTION("var[-1] = \"abc\"")
	{
		Var v;

		v[-1] = "abc";
		REQUIRE(v.IsArray());
		REQUIRE(True == v.IsArrayItem(0));
		REQUIRE(v[0].IsString());
		REQUIRE(String("abc") == v[0].GetString());
	}
	SECTION("var[\"a\"][-1][\"b\"][-1] = \"abc\"")
	{
		Var v;

		v["a"][-1]["b"][-1] = "abc";
		REQUIRE(v.IsObject());
		REQUIRE(True == v.IsObjectMember("a"));
		REQUIRE(v["a"].IsArray());
		REQUIRE(True == v["a"].IsArrayItem(0));
		REQUIRE(v["a"][0].IsObject());
		REQUIRE(True == v["a"][0].IsObjectMember("b"));
		REQUIRE(v["a"][0]["b"].IsArray());
		REQUIRE(True == v["a"][0]["b"].IsArrayItem(0));
		REQUIRE(v["a"][0]["b"][0].IsString());
		REQUIRE(String("abc") == v["a"][0]["b"][0].GetString());
	}
	SECTION("var1 << string1 && var1 >> string2 && var2 << string2 && var1 == var2")
	{
		Var v1;
		Var v2;
		//String sS1 = "{ \"a\": 123, \"b\": { \"c\": \"abc\", \"d\": [1, 2, 3]}, \"e\": [ { \"g\": 100 }, 123 ] }";
		String sS1 = "{ \"a\": 123, \"b\": \"123\", \"c\": 123.321, \"d\": null, \"e\": false, \"ee\": [ 123, \"123\", 123.321, null, false ], \"f\": { \"a\": 123, \"b\": \"123\", \"c\": 123.321, \"d\": null, \"e\": false, \"ee\": [ 123, \"123\", 123.321, null, false ] }, \"g\": false, \"h\": { \"a\": 123, \"b\": \"123\", \"c\": 123.321, \"d\": null, \"e\": false, \"ee\": [ 123, \"123\", 123.321, null, false, { \"a\": 123, \"b\": \"123\", \"c\": 123.321, \"d\": null, \"e\": false, \"ee\": [ 123, \"123\", 123.321, null, false ] } ], \"dummy\": 100 } }";
		String sS2;

		

		v1.Read(sS1);
		v1.Write(sS2);
		v2.Read(sS2);

		//printf("S1 = \n<\n%s\n>\n", sS1.c_str());
		//printf("S2 = \n<\n%s\n>\n", sS2.c_str());

		REQUIRE(v1.Equals(v2));
	}
	SECTION("testbed")
	{
		const Size cStart = 1;
		const Size cEnd   = 40;

		for (Size i = cStart; i <= cEnd; i++)
		{
			//Print(stdout, "JSON {1}\n", i);

			String sPath;

			Print(&sPath, "{1}.json", i);

			IO::FileInputStream fis(sPath.c_str());

			REQUIRE(fis.IsOK());

			Util::DynMemPool mem;

			UInt64 cbSize;
			Size   cbAckSize;

			REQUIRE(fis.GetSize(&cbSize).IsOK());

			REQUIRE(mem.SetSize((Size)cbSize).IsOK());
			
			REQUIRE(fis.Read(mem.GetMem(), mem.GetSize(), &cbAckSize).IsOK());

			String s1((const Char *)mem.GetMem(), mem.GetSize());

			//Print(stdout, "s1 = '{1}'\n", s1);

			Var v1;

			REQUIRE(v1.Read(s1).IsOK());
			
			String s2;

			REQUIRE(v1.Write(s2).IsOK());

			//Print(stdout, "s2 = '{1}'\n", s2);

			Var v2;

			REQUIRE(v2.Read(s2).IsOK());

			REQUIRE(v1.Equals(v2));
		}
	}
}

