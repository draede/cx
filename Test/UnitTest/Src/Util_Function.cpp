
#include "CX/CX.h"
#include "CX/Util/Function.h"
#include "../../../Contrib/Catch/Include/catch.hpp"


using namespace CX;

typedef int            MyT1;
typedef bool           MyT2;
typedef Size           MyT3;
typedef String         MyT4;
typedef Float          MyT5;
typedef Double         MyT6;
typedef Int8           MyT7;
typedef UInt8          MyT8;
typedef Int16          MyT9;
typedef UInt16         MyT10;
typedef Int32          MyT11;
typedef UInt32         MyT12;
typedef Int64          MyT13;
typedef UInt64         MyT14;
typedef char           MyT15;
typedef const Char *   MyT16;

bool   g_bRun = false;

MyT1   g_p1   = 0;
MyT2   g_p2   = 0;
MyT3   g_p3   = 0;
MyT4   g_p4   = "";
MyT5   g_p5   = 0;
MyT6   g_p6   = 0;
MyT7   g_p7   = 0;
MyT8   g_p8   = 0;
MyT9   g_p9   = 0;
MyT10  g_p10  = 0;
MyT11  g_p11  = 0;
MyT12  g_p12  = 0;
MyT13  g_p13  = 0;
MyT14  g_p14  = 0;
MyT15  g_p15  = ' ';
MyT16  g_p16  = "";


void fnr0()
{
	g_bRun = true;
}

void fnr1(MyT1 p1)
{
	g_bRun = true;
	g_p1   = p1;
}

void fnr2(MyT1 p1, MyT2 p2)
{
	g_bRun = true;
	g_p1   = p1;
	g_p2   = p2;
}

void fnr2(MyT1 p1, MyT2 p2, MyT3 p3)
{
	g_bRun = true;
	g_p1   = p1;
	g_p2   = p2;
	g_p3   = p3;
}

int fr0()
{
	g_bRun = true;

	return 123;
}

int fr1(int p1)
{
	g_bRun = true;
	g_p1 = p1;

	return 123;
}

template <typename R>
R RunR(Util::IFunction<R> *pFunction)
{
	g_bRun = false;
	return pFunction->Run();
}

void RunNR(Util::IFunction<void> *pFunction)
{
	g_bRun = false;
	pFunction->Run();
}

TEST_CASE("Function tests", "[CX::Util::Function]")
{
	Util::Function<void> f(&fnr0);

	f.Run();

	SECTION("0 args no return")
	{
		Util::Function<void> f(&fnr0);

		RunNR(&f);
		REQUIRE(true == g_bRun);
	}
	SECTION("1 arg no return")
	{
		Util::Function<void, int> f(&fnr1, 1);

		RunNR(&f);
		REQUIRE(true == g_bRun);
		REQUIRE(1 == g_p1);
	}
	SECTION("2 args no return")
	{
		Util::Function<void, int> f(&fnr1, 1);

		RunNR(&f);
		REQUIRE(true == g_bRun);
		REQUIRE(1 == g_p1);
	}

	SECTION("0 args return")
	{
		Util::Function<int> f(&fr0);

		REQUIRE(123 == RunR(&f));
		REQUIRE(true == g_bRun);
	}

	SECTION("1 arg return")
	{
		Util::Function<int, int> f(&fr1, 1);

		REQUIRE(123 == RunR(&f));
		REQUIRE(true == g_bRun);
		REQUIRE(1 == g_p1);
	}
}

