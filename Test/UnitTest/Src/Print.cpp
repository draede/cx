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

#include "CX/CX.h"
#include "CX/Print.h"
#include "CX/C/string.h"

#pragma warning(push)
#pragma warning(disable: 4702)
#include "../../../Contrib/Catch/Include/catch.hpp"
#pragma warning(pop)


using namespace CX;


template <typename T>
const Char *ToString(T p, Size cPrec = 0)
{
	static Char   szOutput[100];
	Size          cLen;

	ToString(p, szOutput, 100, &cLen, cPrec);

	return szOutput;
}

TEST_CASE("ToString tests", "[CX::ToString]")
{
	SECTION("char")
	{
		REQUIRE(String("x") == ToString<char>('x'));
	}
	SECTION("signed char")
	{
		REQUIRE(String("-123") == ToString<signed char>(-123));
	}
	SECTION("unsigned char")
	{
		REQUIRE(String("123") == ToString<unsigned char>(123));
	}

	SECTION("short")
	{
		REQUIRE(String("1234") == ToString<short>(1234));
	}
	SECTION("signed short")
	{
		REQUIRE(String("-1234") == ToString<signed short>(-1234));
	}
	SECTION("unsigned short")
	{
		REQUIRE(String("1234") == ToString<unsigned short>(1234));
	}

	SECTION("int")
	{
		REQUIRE(String("12345") == ToString<int>(12345));
	}
	SECTION("signed int")
	{
		REQUIRE(String("-12345") == ToString<signed int>(-12345));
	}
	SECTION("unsigned int")
	{
		REQUIRE(String("12345") == ToString<unsigned int>(12345));
	}

	SECTION("long")
	{
		REQUIRE(String("123456") == ToString<long>(123456));
	}
	SECTION("signed long")
	{
		REQUIRE(String("-123456") == ToString<signed long>(-123456));
	}
	SECTION("unsigned long")
	{
		REQUIRE(String("123456") == ToString<unsigned long>(123456));
	}

	SECTION("long long")
	{
		REQUIRE(String("1234567") == ToString<long long>(1234567));
	}
	SECTION("signed long long")
	{
		REQUIRE(String("-1234567") == ToString<signed long long>(-1234567));
	}
	SECTION("unsigned long long")
	{
		REQUIRE(String("1234567") == ToString<unsigned long long>(1234567));
	}

	SECTION("Bool - False")
	{
		REQUIRE(String("False") == ToString<Bool>(False));
	}
	SECTION("Bool - True")
	{
		REQUIRE(String("True") == ToString<Bool>(True));
	}

	SECTION("float")
	{
		REQUIRE(String("123.456") == ToString<float>(123.456f, 3));
	}
	SECTION("double")
	{
		REQUIRE(String("-1234.5678") == ToString<double>(-1234.5678, 4));
	}

	SECTION("Int8")
	{
		REQUIRE(String("-123") == ToString<Int8>(-123));
	}
	SECTION("UInt8")
	{
		REQUIRE(String("123") == ToString<UInt8>(123));
	}
	SECTION("Int16")
	{
		REQUIRE(String("-1234") == ToString<Int16>(-1234));
	}
	SECTION("UInt16")
	{
		REQUIRE(String("1234") == ToString<UInt16>(1234));
	}
	SECTION("Int32")
	{
		REQUIRE(String("-12345") == ToString<Int32>(-12345));
	}
	SECTION("UInt32")
	{
		REQUIRE(String("12345") == ToString<UInt32>(12345));
	}
	SECTION("Int64")
	{
		REQUIRE(String("-123456") == ToString<Int64>(-123456));
	}
	SECTION("UInt64")
	{
		REQUIRE(String("123456") == ToString<UInt64>(123456));
	}
	SECTION("Float")
	{
		REQUIRE(String("123.456") == ToString<Float>(123.456f, 3));
	}
	SECTION("Double")
	{
		REQUIRE(String("-1234.5678") == ToString<Double>(-1234.5678, 4));
	}
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

		Print(&sTmp, "p1 = {1}, p2 = {2:.3}, p3 = {3:.6}, p4 = {4}", 'x', 123.456f, -123.321567, 
		      "abc");
		REQUIRE(sTmp == "p1 = x, p2 = 123.456, p3 = -123.321567, p4 = abc");
	}
	SECTION("5 args")
	{
		String sTmp;

		Print(&sTmp, "p1 = {1}, p2 = {2:.3}, p3 = {3:.6}, p4 = {4}, p5 = {5}", 'x', 
		      123.456f, -123.321567, "abc", String("xyz"));
		REQUIRE(sTmp == "p1 = x, p2 = 123.456, p3 = -123.321567, p4 = abc, p5 = xyz");
	}
	SECTION("6 args")
	{
		String sTmp;

		Print(&sTmp, "p1 = {1}, p2 = {2:.3}, p3 = {3:.6}, p4 = {4}, p5 = {5}, p6 = {6:<'*'10}", 'x', 
		      123.456f, -123.321567, "abc", String("xyz"), "123");
		REQUIRE(sTmp == "p1 = x, p2 = 123.456, p3 = -123.321567, p4 = abc, p5 = xyz, p6 = 123*******");
	}
	SECTION("7 args")
	{
		String sTmp;

		Print(&sTmp, "p1 = {1}, p2 = {2:.3}, p3 = {3:.6}, p4 = {4}, p5 = {5}, p6 = {6:<'*'10},"
		      " p7 = {7:>'#'12}", 'x', 123.456f, -123.321567, "abc", String("xyz"), "123", 123);
		REQUIRE(sTmp == "p1 = x, p2 = 123.456, p3 = -123.321567, p4 = abc, p5 = xyz, "
		        "p6 = 123*******, p7 = #########123");
	}
	SECTION("8 args")
	{
		String sTmp;

		Print(&sTmp, "p1 = {1}, p2 = {2:.3}, p3 = {3:.6}, p4 = {4}, p5 = {5}, p6 = {6:<'*'10},"
		      " p7 = {7:>'#'12}, p8 = {8:|'$'9}", 'x', 123.456f, -123.321567, "abc", String("xyz"), 
		      "123", (Size)123, 123);
		REQUIRE(sTmp == "p1 = x, p2 = 123.456, p3 = -123.321567, p4 = abc, p5 = xyz, "
		        "p6 = 123*******, p7 = #########123, p8 = $$$123$$$");
	}
	SECTION("9 args")
	{
		String sTmp;

		Print(&sTmp, "p1 = {1}, p2 = {2:.3}, p3 = {3:.6}, p4 = {4}, p5 = {5}, p6 = {6:<'*'10},"
		      " p7 = {7:>'#'12}, p8 = {8:|'$'9}, p9 = {9}", 'x', 123.456f, -123.321567, "abc", 
		      String("xyz"), "123", (Size)123, 123, String("abc"));
		REQUIRE(sTmp == "p1 = x, p2 = 123.456, p3 = -123.321567, p4 = abc, p5 = xyz, "
		        "p6 = 123*******, p7 = #########123, p8 = $$$123$$$, p9 = abc");
	}
	SECTION("10 args")
	{
		String sTmp;

		Print(&sTmp, "p1 = {1}, p2 = {2:.3}, p3 = {3:.6}, p4 = {4}, p5 = {5}, p6 = {6:<'*'10},"
		      " p7 = {7:>'#'12}, p8 = {8:|'$'9}, p9 = {9}, p10 = {10}", 'x', 123.456f, -123.321567, 
		      "abc", String("xyz"), "123", (Size)123, 123, String("abc"), 10);
		REQUIRE(sTmp == "p1 = x, p2 = 123.456, p3 = -123.321567, p4 = abc, p5 = xyz, "
		        "p6 = 123*******, p7 = #########123, p8 = $$$123$$$, p9 = abc, p10 = 10");
	}
	SECTION("11 args")
	{
		String sTmp;

		Print(&sTmp, "p1 = {1}, p2 = {2:.3}, p3 = {3:.6}, p4 = {4}, p5 = {5}, p6 = {6:<'*'10},"
		      " p7 = {7:>'#'12}, p8 = {8:|'$'9}, p9 = {9}, p10 = {10}, p11 = {11}", 'x', 123.456f, 
		      -123.321567, "abc", String("xyz"), "123", (Size)123, 123, String("abc"), 10, 11);
		REQUIRE(sTmp == "p1 = x, p2 = 123.456, p3 = -123.321567, p4 = abc, p5 = xyz, "
		        "p6 = 123*******, p7 = #########123, p8 = $$$123$$$, p9 = abc, p10 = 10, p11 = 11");
	}
	SECTION("12 args")
	{
		String sTmp;

		Print(&sTmp, "p1 = {1}, p2 = {2:.3}, p3 = {3:.6}, p4 = {4}, p5 = {5}, p6 = {6:<'*'10},"
		      " p7 = {7:>'#'12}, p8 = {8:|'$'9}, p9 = {9}, p10 = {10}, p11 = {11}, p12 = {12}", 'x', 
		      123.456f, -123.321567, "abc", String("xyz"), "123", (Size)123, 123, String("abc"), 10, 
		      11, 12);
		REQUIRE(sTmp == "p1 = x, p2 = 123.456, p3 = -123.321567, p4 = abc, p5 = xyz, "
		        "p6 = 123*******, p7 = #########123, p8 = $$$123$$$, p9 = abc, p10 = 10, p11 = 11, "
		        "p12 = 12");
	}
	SECTION("13 args")
	{
		String sTmp;

		Print(&sTmp, "p1 = {1}, p2 = {2:.3}, p3 = {3:.6}, p4 = {4}, p5 = {5}, p6 = {6:<'*'10},"
		      " p7 = {7:>'#'12}, p8 = {8:|'$'9}, p9 = {9}, p10 = {10}, p11 = {11}, p12 = {12}, "
		      "p13 = {13}", 'x', 123.456f, -123.321567, "abc", String("xyz"), "123", (Size)123, 123, 
		      String("abc"), 10, 11, 12, 13);
		REQUIRE(sTmp == "p1 = x, p2 = 123.456, p3 = -123.321567, p4 = abc, p5 = xyz, "
		        "p6 = 123*******, p7 = #########123, p8 = $$$123$$$, p9 = abc, p10 = 10, p11 = 11, "
		        "p12 = 12, p13 = 13");
	}
	SECTION("14 args")
	{
		String sTmp;

		Print(&sTmp, "p1 = {1}, p2 = {2:.3}, p3 = {3:.6}, p4 = {4}, p5 = {5}, p6 = {6:<'*'10},"
		      " p7 = {7:>'#'12}, p8 = {8:|'$'9}, p9 = {9}, p10 = {10}, p11 = {11}, p12 = {12}, "
		      "p13 = {13}, p14 = {14}", 'x', 123.456f, -123.321567, "abc", String("xyz"), "123", 
		      (Size)123, 123, String("abc"), 10, 11, 12, 13, 14);
		REQUIRE(sTmp == "p1 = x, p2 = 123.456, p3 = -123.321567, p4 = abc, p5 = xyz, "
		        "p6 = 123*******, p7 = #########123, p8 = $$$123$$$, p9 = abc, p10 = 10, p11 = 11, "
		        "p12 = 12, p13 = 13, p14 = 14");
	}
	SECTION("15 args")
	{
		String sTmp;

		Print(&sTmp, "p1 = {1}, p2 = {2:.3}, p3 = {3:.6}, p4 = {4}, p5 = {5}, p6 = {6:<'*'10},"
		      " p7 = {7:>'#'12}, p8 = {8:|'$'9}, p9 = {9}, p10 = {10}, p11 = {11}, p12 = {12}, "
		      "p13 = {13}, p14 = {14}, p15 = {15}", 'x', 123.456f, -123.321567, "abc", String("xyz"), 
		      "123", (Size)123, 123, String("abc"), 10, 11, 12, 13, 14, 15);
		REQUIRE(sTmp == "p1 = x, p2 = 123.456, p3 = -123.321567, p4 = abc, p5 = xyz, "
		        "p6 = 123*******, p7 = #########123, p8 = $$$123$$$, p9 = abc, p10 = 10, p11 = 11, "
		        "p12 = 12, p13 = 13, p14 = 14, p15 = 15");
	}
	SECTION("116 args")
	{
		String sTmp;

		Print(&sTmp, "p1 = {1}, p2 = {2:.3}, p3 = {3:.6}, p4 = {4}, p5 = {5}, p6 = {6:<'*'10},"
		      " p7 = {7:>'#'12}, p8 = {8:|'$'9}, p9 = {9}, p10 = {10}, p11 = {11}, p12 = {12}, "
		      "p13 = {13}, p14 = {14}, p15 = {15}, p16 = {16}", 'x', 123.456f, -123.321567, "abc", 
		      String("xyz"), "123", (Size)123, 123, String("abc"), 10, 11, 12, 13, 14, 15, 16);
		REQUIRE(sTmp == "p1 = x, p2 = 123.456, p3 = -123.321567, p4 = abc, p5 = xyz, "
		        "p6 = 123*******, p7 = #########123, p8 = $$$123$$$, p9 = abc, p10 = 10, p11 = 11, "
		        "p12 = 12, p13 = 13, p14 = 14, p15 = 15, p16 = 16");
	}
}

