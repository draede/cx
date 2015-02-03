
#include "CX/CX.h"
#include "CX/Var.h"
#include "CX/Print.h"
#include "CX/IO/FileInputStream.h"
#include "CX/Util/MemPool.h"


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
	SECTION("Constructor (bool)")
	{
		Var v(true);

		REQUIRE(v.IsBool());
		REQUIRE(true == v.GetBool());
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
	SECTION("var = false")
	{
		Var v;

		v = false;
		REQUIRE(v.IsBool());
		REQUIRE(false == v.GetBool());
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
	SECTION("var[\"a\"] = false")
	{
		Var v;

		v["a"] = false;
		REQUIRE(v.IsObject());
		REQUIRE(true == v.IsObjectMember("a"));
		REQUIRE(v["a"].IsBool());
		REQUIRE(false == v["a"].GetBool());
	}
	SECTION("var[\"a\"][\"b\"] = \"abc\"")
	{
		Var v;

		v["a"]["b"] = "abc";
		REQUIRE(v.IsObject());
		REQUIRE(true == v.IsObjectMember("a"));
		REQUIRE(v["a"].IsObject());
		REQUIRE(true == v["a"].IsObjectMember("b"));
		REQUIRE(v["a"]["b"].IsString());
		REQUIRE(String("abc") == v["a"]["b"].GetString());
	}
	SECTION("var[-1] = \"abc\"")
	{
		Var v;

		v[-1] = "abc";
		REQUIRE(v.IsArray());
		REQUIRE(true == v.IsArrayItem(0));
		REQUIRE(v[0].IsString());
		REQUIRE(String("abc") == v[0].GetString());
	}
	SECTION("var[\"a\"][-1][\"b\"][-1] = \"abc\"")
	{
		Var v;

		v["a"][-1]["b"][-1] = "abc";
		REQUIRE(v.IsObject());
		REQUIRE(true == v.IsObjectMember("a"));
		REQUIRE(v["a"].IsArray());
		REQUIRE(true == v["a"].IsArrayItem(0));
		REQUIRE(v["a"][0].IsObject());
		REQUIRE(true == v["a"][0].IsObjectMember("b"));
		REQUIRE(v["a"][0]["b"].IsArray());
		REQUIRE(true == v["a"][0]["b"].IsArrayItem(0));
		REQUIRE(v["a"][0]["b"][0].IsString());
		REQUIRE(String("abc") == v["a"][0]["b"][0].GetString());
	}
	SECTION("var1 << string1 && var1 >> string2 && var2 << string2 && var1 == var2")
	{
		Var v1;
		Var v2;
		String sS1 = "{ \"a\": 123, \"b\": { \"c\": \"abc\", \"d\": [1, 2, 3]}, \"e\": [ { \"g\": 100 }, 123 ] }";
		String sS2;

		v1.Read(sS1);
		v1.Write(sS2);
		v2.Read(sS2);

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

