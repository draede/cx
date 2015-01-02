
#include "CX/CX.h"
#include "CX/C/string.h"
#include "../../../Contrib/Catch/Include/catch.hpp"


using namespace CX;

#define TOSTRTSTP(name, type, actval, strval, prec)		\
SECTION(name)												\
{																\
	Char szOutput[100];									\
	String sOutput;										\
	Size cLen;												\
	type x = actval;										\
																\
	ToString(x, szOutput, 100, &cLen, prec);		\
	sOutput = szOutput;									\
																\
	REQUIRE(sOutput == strval);						\
}

#define TOSTRTST(name, type, actval, strval)		TOSTRTSTP(name, type, actval, strval, 0)


TEST_CASE("ToString tests", "[CX::ToString]")
{
	TOSTRTST("char", char, 'x', "x");
	TOSTRTST("signed char", signed char, -123, "-123");
	TOSTRTST("unsigned char", unsigned char, 123, "123");

	TOSTRTST("short", short, 1234, "1234");
	TOSTRTST("signed short", signed short, -1234, "-1234");
	TOSTRTST("unsigned short", unsigned short, 1234, "1234");

	TOSTRTST("int", int, 12345, "12345");
	TOSTRTST("signed int", signed int, -12345, "-12345");
	TOSTRTST("unsigned int", unsigned int, 12345, "12345");

	TOSTRTST("long", long, 123456, "123456");
	TOSTRTST("signed long", signed long, -123456, "-123456");
	TOSTRTST("unsigned long", unsigned long, 123456, "123456");

	TOSTRTST("bool - false", bool, false, "false");
	TOSTRTST("bool - true", bool, true, "true");

	TOSTRTSTP("float", float, -123.456f, "-123.456", 3);
	TOSTRTSTP("double", double, 12345.54321, "12345.54321", 5);

	TOSTRTST("Int8", Int8, -123, "-123");
	TOSTRTST("UInt8", UInt8, 123, "123");
	TOSTRTST("Int16", Int16, -1234, "-1234");
	TOSTRTST("UInt16", UInt16, 1234, "1234");
	TOSTRTST("Int32", Int32, -12345, "-12345");
	TOSTRTST("UInt32", UInt32, 12345, "12345");
	TOSTRTST("Int64", Int64, -123456, "-123456");
	TOSTRTST("UInt64", UInt64, 123456, "123456");

	TOSTRTSTP("Float", Float, -123.456f, "-123.456", 3);
	TOSTRTSTP("Double", Double, 12345.54321, "12345.54321", 5);
}

TEST_CASE("Print tests", "[CX::Print]")
{
	SECTION("0 args")
	{
		String sTmp;

		Print(&sTmp, "this is a test");
		REQUIRE(sTmp == "this is a test");
	}
	SECTION("1 arg")
	{
		String sTmp;

		Print(&sTmp, "p1 = {1}", 123);
		REQUIRE(sTmp == "p1 = 123");
	}
	SECTION("2 args")
	{
		String sTmp;

		Print(&sTmp, "p1 = {1}, p2 = {2}", 100, -145);
		REQUIRE(sTmp == "p1 = 100, p2 = -145");
	}
	SECTION("3 args")
	{
		String sTmp;

		Print(&sTmp, "p1 = {1}, p2 = {2:.3}, p3 = {3:.6}", 'x', 123.456f, -123.321567);
		REQUIRE(sTmp == "p1 = x, p2 = 123.456, p3 = -123.321567");
	}
	SECTION("4 args")
	{
		String sTmp;

		Print(&sTmp, "p1 = {1}, p2 = {2:.3}, p3 = {3:.6}, p4 = {4}", 'x', 123.456f, -123.321567, "abc");
		REQUIRE(sTmp == "p1 = x, p2 = 123.456, p3 = -123.321567, p4 = abc");
	}
	SECTION("5 args")
	{
		String sTmp;

		Print(&sTmp, "p1 = {1}, p2 = {2:.3}, p3 = {3:.6}, p4 = {4}, p5 = {5}", 'x', 123.456f, -123.321567, "abc", String("xyz"));
		REQUIRE(sTmp == "p1 = x, p2 = 123.456, p3 = -123.321567, p4 = abc, p5 = xyz");
	}
	SECTION("6 args")
	{
		String sTmp;

		Print(&sTmp, "p1 = {1}, p2 = {2:.3}, p3 = {3:.6}, p4 = {4}, p5 = {5}, p6 = {6:<'*'10}", 'x', 123.456f, -123.321567, "abc", String("xyz"), "123");
		REQUIRE(sTmp == "p1 = x, p2 = 123.456, p3 = -123.321567, p4 = abc, p5 = xyz, p6 = 123*******");
	}
	SECTION("7 args")
	{
		String sTmp;

		Print(&sTmp, "p1 = {1}, p2 = {2:.3}, p3 = {3:.6}, p4 = {4}, p5 = {5}, p6 = {6:<'*'10}, p7 = {7:>'#'12}", 'x', 123.456f, -123.321567, "abc", String("xyz"), "123", 123);
		REQUIRE(sTmp == "p1 = x, p2 = 123.456, p3 = -123.321567, p4 = abc, p5 = xyz, p6 = 123*******, p7 = #########123");
	}
	SECTION("8 args")
	{
		String sTmp;

		Print(&sTmp, "p1 = {1}, p2 = {2:.3}, p3 = {3:.6}, p4 = {4}, p5 = {5}, p6 = {6:<'*'10}, p7 = {7:>'#'12}, p8 = {8:|'$'9}", 'x', 123.456f, -123.321567, "abc", String("xyz"), "123", 123, 123);
		REQUIRE(sTmp == "p1 = x, p2 = 123.456, p3 = -123.321567, p4 = abc, p5 = xyz, p6 = 123*******, p7 = #########123, p8 = $$$123$$$");
	}
}

