
#include "CX/CX.h"
#include "CX/Util/Container.h"
#include "../../../Contrib/Catch/Include/catch.hpp"


using namespace CX;


TEST_CASE("Container tests", "[CX::Util::Container]")
{
	SECTION("0 args")
	{
		Util::Container<> x;

		REQUIRE(0 == x.GetCount());
	}
	SECTION("1 arg")
	{
		Util::Container<int> x(100);

		REQUIRE(1 == x.GetCount());
		REQUIRE(100 == x.m_p1);
	}
	SECTION("2 args")
	{
		Util::Container<int, float> x(100, 123.456f);

		REQUIRE(2 == x.GetCount());
		REQUIRE(100 == x.m_p1);
		REQUIRE(123.456f == x.m_p2);
	}
	SECTION("3 args")
	{
		Util::Container<int, float, double> x(100, 123.456f, 1234.5678);

		REQUIRE(3 == x.GetCount());
		REQUIRE(100 == x.m_p1);
		REQUIRE(123.456f == x.m_p2);
		REQUIRE(1234.5678 == x.m_p3);
	}
	SECTION("4 args")
	{
		Util::Container<int, float, double, long> x(100, 123.456f, 1234.5678, -123);

		REQUIRE(4 == x.GetCount());
		REQUIRE(100 == x.m_p1);
		REQUIRE(123.456f == x.m_p2);
		REQUIRE(1234.5678 == x.m_p3);
		REQUIRE(-123 == x.m_p4);
	}
	SECTION("5 args")
	{
		Util::Container<int, float, double, long, const char *> x(100, 123.456f, 1234.5678, -123, "abc");

		REQUIRE(5 == x.GetCount());
		REQUIRE(100 == x.m_p1);
		REQUIRE(123.456f == x.m_p2);
		REQUIRE(1234.5678 == x.m_p3);
		REQUIRE(-123 == x.m_p4);
		REQUIRE(String("abc") == x.m_p5);
	}
	SECTION("6 args")
	{
		Util::Container<int, float, double, long, const char *, String> x(100, 123.456f, 1234.5678, -123, 
		          "abc", String("xyz"));

		REQUIRE(6 == x.GetCount());
		REQUIRE(100 == x.m_p1);
		REQUIRE(123.456f == x.m_p2);
		REQUIRE(1234.5678 == x.m_p3);
		REQUIRE(-123 == x.m_p4);
		REQUIRE(String("abc") == x.m_p5);
		REQUIRE(String("xyz") == x.m_p6);
	}
	SECTION("7 args")
	{
		Util::Container<int, float, double, long, const char *, String, bool> x(100, 123.456f, 1234.5678, 
		          -123, "abc", String("xyz"), false);

		REQUIRE(7 == x.GetCount());
		REQUIRE(100 == x.m_p1);
		REQUIRE(123.456f == x.m_p2);
		REQUIRE(1234.5678 == x.m_p3);
		REQUIRE(-123 == x.m_p4);
		REQUIRE(String("abc") == x.m_p5);
		REQUIRE(String("xyz") == x.m_p6);
		REQUIRE(false == x.m_p7);
	}
	SECTION("8 args")
	{
		Util::Container<int, float, double, long, const char *, String, bool, char> x(100, 123.456f, 
		          1234.5678, -123, "abc", String("xyz"), false, 'x');

		REQUIRE(8 == x.GetCount());
		REQUIRE(100 == x.m_p1);
		REQUIRE(123.456f == x.m_p2);
		REQUIRE(1234.5678 == x.m_p3);
		REQUIRE(-123 == x.m_p4);
		REQUIRE(String("abc") == x.m_p5);
		REQUIRE(String("xyz") == x.m_p6);
		REQUIRE(false == x.m_p7);
		REQUIRE('x' == x.m_p8);
	}
	SECTION("9 args")
	{
		Util::Container<int, float, double, long, const char *, String, bool, char, short> x(100, 123.456f, 
		          1234.5678, -123, "abc", String("xyz"), false, 'x', 10);

		REQUIRE(9 == x.GetCount());
		REQUIRE(100 == x.m_p1);
		REQUIRE(123.456f == x.m_p2);
		REQUIRE(1234.5678 == x.m_p3);
		REQUIRE(-123 == x.m_p4);
		REQUIRE(String("abc") == x.m_p5);
		REQUIRE(String("xyz") == x.m_p6);
		REQUIRE(false == x.m_p7);
		REQUIRE('x' == x.m_p8);
		REQUIRE(10 == x.m_p9);
	}
	SECTION("10 args")
	{
		Util::Container<int, float, double, long, const char *, String, bool, char, short, unsigned char> 
		          x(100, 123.456f, 1234.5678, -123, "abc", String("xyz"), false, 'x', 10, 5);

		REQUIRE(10 == x.GetCount());
		REQUIRE(100 == x.m_p1);
		REQUIRE(123.456f == x.m_p2);
		REQUIRE(1234.5678 == x.m_p3);
		REQUIRE(-123 == x.m_p4);
		REQUIRE(String("abc") == x.m_p5);
		REQUIRE(String("xyz") == x.m_p6);
		REQUIRE(false == x.m_p7);
		REQUIRE('x' == x.m_p8);
		REQUIRE(10 == x.m_p9);
		REQUIRE(5 == x.m_p10);
	}
	SECTION("11 args")
	{
		Util::Container<int, float, double, long, const char *, String, bool, char, short, unsigned char, 
		          Size> 
		             x(100, 123.456f, 1234.5678, -123, "abc", String("xyz"), false, 'x', 10, 5, 1000);

		REQUIRE(11 == x.GetCount());
		REQUIRE(100 == x.m_p1);
		REQUIRE(123.456f == x.m_p2);
		REQUIRE(1234.5678 == x.m_p3);
		REQUIRE(-123 == x.m_p4);
		REQUIRE(String("abc") == x.m_p5);
		REQUIRE(String("xyz") == x.m_p6);
		REQUIRE(false == x.m_p7);
		REQUIRE('x' == x.m_p8);
		REQUIRE(10 == x.m_p9);
		REQUIRE(5 == x.m_p10);
		REQUIRE(1000 == x.m_p11);
	}
	SECTION("12 args")
	{
		Util::Container<int, float, double, long, const char *, String, bool, char, short, unsigned char, 
		          Size, unsigned> 
		             x(100, 123.456f, 1234.5678, -123, "abc", String("xyz"), false, 'x', 10, 5, 1000, 
		               50);

		REQUIRE(12 == x.GetCount());
		REQUIRE(100 == x.m_p1);
		REQUIRE(123.456f == x.m_p2);
		REQUIRE(1234.5678 == x.m_p3);
		REQUIRE(-123 == x.m_p4);
		REQUIRE(String("abc") == x.m_p5);
		REQUIRE(String("xyz") == x.m_p6);
		REQUIRE(false == x.m_p7);
		REQUIRE('x' == x.m_p8);
		REQUIRE(10 == x.m_p9);
		REQUIRE(5 == x.m_p10);
		REQUIRE(1000 == x.m_p11);
		REQUIRE(50 == x.m_p12);
	}
	SECTION("13 args")
	{
		Util::Container<int, float, double, long, const char *, String, bool, char, short, unsigned char, 
		          Size, unsigned, bool> 
		             x(100, 123.456f, 1234.5678, -123, "abc", String("xyz"), false, 'x', 10, 5, 1000, 
		               50, true);

		REQUIRE(13 == x.GetCount());
		REQUIRE(100 == x.m_p1);
		REQUIRE(123.456f == x.m_p2);
		REQUIRE(1234.5678 == x.m_p3);
		REQUIRE(-123 == x.m_p4);
		REQUIRE(String("abc") == x.m_p5);
		REQUIRE(String("xyz") == x.m_p6);
		REQUIRE(false == x.m_p7);
		REQUIRE('x' == x.m_p8);
		REQUIRE(10 == x.m_p9);
		REQUIRE(5 == x.m_p10);
		REQUIRE(1000 == x.m_p11);
		REQUIRE(50 == x.m_p12);
		REQUIRE(true == x.m_p13);
	}
	SECTION("14 args")
	{
		Util::Container<int, float, double, long, const char *, String, bool, char, short, unsigned char, 
		          Size, unsigned, bool, String> 
		             x(100, 123.456f, 1234.5678, -123, "abc", String("xyz"), false, 'x', 10, 5, 1000, 
		               50, true, "aaa");

		REQUIRE(14 == x.GetCount());
		REQUIRE(100 == x.m_p1);
		REQUIRE(123.456f == x.m_p2);
		REQUIRE(1234.5678 == x.m_p3);
		REQUIRE(-123 == x.m_p4);
		REQUIRE(String("abc") == x.m_p5);
		REQUIRE(String("xyz") == x.m_p6);
		REQUIRE(false == x.m_p7);
		REQUIRE('x' == x.m_p8);
		REQUIRE(10 == x.m_p9);
		REQUIRE(5 == x.m_p10);
		REQUIRE(1000 == x.m_p11);
		REQUIRE(50 == x.m_p12);
		REQUIRE(true == x.m_p13);
		REQUIRE(String("aaa") == x.m_p14);
	}
	SECTION("15 args")
	{
		Util::Container<int, float, double, long, const char *, String, bool, char, short, unsigned char, 
		          Size, unsigned, bool, String, int> 
		             x(100, 123.456f, 1234.5678, -123, "abc", String("xyz"), false, 'x', 10, 5, 1000, 
		               50, true, "aaa", 1);

		REQUIRE(15 == x.GetCount());
		REQUIRE(100 == x.m_p1);
		REQUIRE(123.456f == x.m_p2);
		REQUIRE(1234.5678 == x.m_p3);
		REQUIRE(-123 == x.m_p4);
		REQUIRE(String("abc") == x.m_p5);
		REQUIRE(String("xyz") == x.m_p6);
		REQUIRE(false == x.m_p7);
		REQUIRE('x' == x.m_p8);
		REQUIRE(10 == x.m_p9);
		REQUIRE(5 == x.m_p10);
		REQUIRE(1000 == x.m_p11);
		REQUIRE(50 == x.m_p12);
		REQUIRE(true == x.m_p13);
		REQUIRE(String("aaa") == x.m_p14);
		REQUIRE(1 == x.m_p15);
	}
	SECTION("16 args")
	{
		Util::Container<int, float, double, long, const char *, String, bool, char, short, unsigned char, 
		          Size, unsigned, bool, String, int, float> 
		             x(100, 123.456f, 1234.5678, -123, "abc", String("xyz"), false, 'x', 10, 5, 1000, 
		               50, true, "aaa", 1, 1.2f);

		REQUIRE(16 == x.GetCount());
		REQUIRE(100 == x.m_p1);
		REQUIRE(123.456f == x.m_p2);
		REQUIRE(1234.5678 == x.m_p3);
		REQUIRE(-123 == x.m_p4);
		REQUIRE(String("abc") == x.m_p5);
		REQUIRE(String("xyz") == x.m_p6);
		REQUIRE(false == x.m_p7);
		REQUIRE('x' == x.m_p8);
		REQUIRE(10 == x.m_p9);
		REQUIRE(5 == x.m_p10);
		REQUIRE(1000 == x.m_p11);
		REQUIRE(50 == x.m_p12);
		REQUIRE(true == x.m_p13);
		REQUIRE(String("aaa") == x.m_p14);
		REQUIRE(1 == x.m_p15);
		REQUIRE(1.2f == x.m_p16);
	}
}

