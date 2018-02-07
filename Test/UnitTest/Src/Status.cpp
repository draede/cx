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

#include "CX/Types.hpp"
#include "CX/Status.hpp"

#pragma warning(push)
#pragma warning(disable: 4702)
#include "../../../Contrib/Catch/Include/catch.hpp"
#pragma warning(pop)


using namespace CX;


TEST_CASE("Status tests", "[CX::Status]")
{
	SECTION("IsOK")
	{
		Status status(Status_OperationFailed);

		REQUIRE(False == status.IsOK());
	}
	SECTION("IsNOK")
	{
		Status status(Status_OperationFailed);

		REQUIRE(True == status.IsNOK());
	}
	SECTION("Set code only")
	{
		Status status(Status_OperationFailed);

		REQUIRE(Status_OperationFailed == status.GetCode());
		REQUIRE(String("") == status.GetMsg());
	}
	SECTION("0 args")
	{
		Status status(Status_OperationFailed, "");

		REQUIRE(Status_OperationFailed == status.GetCode());
		REQUIRE(String("") == status.GetMsg());
	}
	SECTION("1 args")
	{
		Status status(Status_OperationFailed, "p1 = {1}", 1);

		REQUIRE(Status_OperationFailed == status.GetCode());
		REQUIRE(String("p1 = 1") == status.GetMsg());
	}
	SECTION("2 args")
	{
		Status status(Status_OperationFailed, "p1 = {1}, p2 = {2}", 1, 2);

		REQUIRE(Status_OperationFailed == status.GetCode());
		REQUIRE(String("p1 = 1, p2 = 2") == status.GetMsg());
	}
	SECTION("3 args")
	{
		Status status(Status_OperationFailed, "p1 = {1}, p2 = {2}, p3 = {3}", 1, 2, 3);

		REQUIRE(Status_OperationFailed == status.GetCode());
		REQUIRE(String("p1 = 1, p2 = 2, p3 = 3") == status.GetMsg());
	}
	SECTION("4 args")
	{
		Status status(Status_OperationFailed, "p1 = {1}, p2 = {2}, p3 = {3}, p4 = {4}", 1, 2, 3, 4);

		REQUIRE(Status_OperationFailed == status.GetCode());
		REQUIRE(String("p1 = 1, p2 = 2, p3 = 3, p4 = 4") == status.GetMsg());
	}
	SECTION("5 args")
	{
		Status status(Status_OperationFailed, "p1 = {1}, p2 = {2}, p3 = {3}, p4 = {4}, p5 = {5}", 
		            1, 2, 3, 4, 5);

		REQUIRE(Status_OperationFailed == status.GetCode());
		REQUIRE(String("p1 = 1, p2 = 2, p3 = 3, p4 = 4, p5 = 5") == status.GetMsg());
	}
	SECTION("6 args")
	{
		Status status(Status_OperationFailed, "p1 = {1}, p2 = {2}, p3 = {3}, p4 = {4}, p5 = {5}, "
		            "p6 = {6}", 1, 2, 3, 4, 5, 6);

		REQUIRE(Status_OperationFailed == status.GetCode());
		REQUIRE(String("p1 = 1, p2 = 2, p3 = 3, p4 = 4, p5 = 5, p6 = 6") == status.GetMsg());
	}
	SECTION("7 args")
	{
		Status status(Status_OperationFailed, "p1 = {1}, p2 = {2}, p3 = {3}, p4 = {4}, p5 = {5}, "
		              "p6 = {6}, p7 = {7}", 1, 2, 3, 4, 5, 6, 7);

		REQUIRE(Status_OperationFailed == status.GetCode());
		REQUIRE(String("p1 = 1, p2 = 2, p3 = 3, p4 = 4, p5 = 5, p6 = 6, p7 = 7") == status.GetMsg());
	}
	SECTION("8 args")
	{
		Status status(Status_OperationFailed, "p1 = {1}, p2 = {2}, p3 = {3}, p4 = {4}, p5 = {5}, "
		              "p6 = {6}, p7 = {7}, p8 = {8}", 1, 2, 3, 4, 5, 6, 7, 8);

		REQUIRE(Status_OperationFailed == status.GetCode());
		REQUIRE(String("p1 = 1, p2 = 2, p3 = 3, p4 = 4, p5 = 5, p6 = 6, p7 = 7, p8 = 8") == 
		        status.GetMsg());
	}
	SECTION("9 args")
	{
		Status status(Status_OperationFailed, "p1 = {1}, p2 = {2}, p3 = {3}, p4 = {4}, p5 = {5}, "
		            "p6 = {6}, p7 = {7}, p8 = {8}, p9 = {9}", 1, 2, 3, 4, 5, 6, 7, 8, 9);

		REQUIRE(Status_OperationFailed == status.GetCode());
		REQUIRE(String("p1 = 1, p2 = 2, p3 = 3, p4 = 4, p5 = 5, p6 = 6, p7 = 7, p8 = 8, p9 = 9") == 
		        status.GetMsg());
	}
	SECTION("10 args")
	{
		Status status(Status_OperationFailed, "p1 = {1}, p2 = {2}, p3 = {3}, p4 = {4}, p5 = {5}, "
		            "p6 = {6}, p7 = {7}, p8 = {8}, p9 = {9}, p10 = {10}", 1, 2, 3, 4, 5, 6, 7, 8, 9, 
		            10);

		REQUIRE(Status_OperationFailed == status.GetCode());
		REQUIRE(String("p1 = 1, p2 = 2, p3 = 3, p4 = 4, p5 = 5, p6 = 6, p7 = 7, p8 = 8, p9 = 9, "
		        "p10 = 10") == status.GetMsg());
	}
	SECTION("11 args")
	{
		Status status(Status_OperationFailed, "p1 = {1}, p2 = {2}, p3 = {3}, p4 = {4}, p5 = {5}, "
		            "p6 = {6}, p7 = {7}, p8 = {8}, p9 = {9}, p10 = {10}, p11 = {11}", 1, 2, 3, 4, 5, 
		            6, 7, 8, 9, 10, 11);

		REQUIRE(Status_OperationFailed == status.GetCode());
		REQUIRE(String("p1 = 1, p2 = 2, p3 = 3, p4 = 4, p5 = 5, p6 = 6, p7 = 7, p8 = 8, p9 = 9, "
		        "p10 = 10, p11 = 11") == status.GetMsg());
	}
	SECTION("12 args")
	{
		Status status(Status_OperationFailed, "p1 = {1}, p2 = {2}, p3 = {3}, p4 = {4}, p5 = {5}, "
		            "p6 = {6}, p7 = {7}, p8 = {8}, p9 = {9}, p10 = {10}, p11 = {11}, p12 = {12}", 1, 
		            2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12);

		REQUIRE(Status_OperationFailed == status.GetCode());
		REQUIRE(String("p1 = 1, p2 = 2, p3 = 3, p4 = 4, p5 = 5, p6 = 6, p7 = 7, p8 = 8, p9 = 9, "
		        "p10 = 10, p11 = 11, p12 = 12") == status.GetMsg());
	}
	SECTION("13 args")
	{
		Status status(Status_OperationFailed, "p1 = {1}, p2 = {2}, p3 = {3}, p4 = {4}, p5 = {5}, "
		            "p6 = {6}, p7 = {7}, p8 = {8}, p9 = {9}, p10 = {10}, p11 = {11}, p12 = {12}, "
		            "p13 = {13}", 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13);

		REQUIRE(Status_OperationFailed == status.GetCode());
		REQUIRE(String("p1 = 1, p2 = 2, p3 = 3, p4 = 4, p5 = 5, p6 = 6, p7 = 7, p8 = 8, p9 = 9, "
		        "p10 = 10, p11 = 11, p12 = 12, p13 = 13") == status.GetMsg());
	}
	SECTION("14 args")
	{
		Status status(Status_OperationFailed, "p1 = {1}, p2 = {2}, p3 = {3}, p4 = {4}, p5 = {5}, "
		            "p6 = {6}, p7 = {7}, p8 = {8}, p9 = {9}, p10 = {10}, p11 = {11}, p12 = {12}, "
		            "p13 = {13}, p14 = {14}", 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14);

		REQUIRE(Status_OperationFailed == status.GetCode());
		REQUIRE(String("p1 = 1, p2 = 2, p3 = 3, p4 = 4, p5 = 5, p6 = 6, p7 = 7, p8 = 8, p9 = 9, "
		        "p10 = 10, p11 = 11, p12 = 12, p13 = 13, p14 = 14") == status.GetMsg());
	}
	SECTION("15 args")
	{
		Status status(Status_OperationFailed, "p1 = {1}, p2 = {2}, p3 = {3}, p4 = {4}, p5 = {5}, "
		            "p6 = {6}, p7 = {7}, p8 = {8}, p9 = {9}, p10 = {10}, p11 = {11}, p12 = {12}, "
		            "p13 = {13}, p14 = {14}, p15 = {15}", 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 
		            14, 15);

		REQUIRE(Status_OperationFailed == status.GetCode());
		REQUIRE(String("p1 = 1, p2 = 2, p3 = 3, p4 = 4, p5 = 5, p6 = 6, p7 = 7, p8 = 8, p9 = 9, "
		        "p10 = 10, p11 = 11, p12 = 12, p13 = 13, p14 = 14, p15 = 15") == status.GetMsg());
	}
	SECTION("16 args")
	{
		Status status(Status_OperationFailed, "p1 = {1}, p2 = {2}, p3 = {3}, p4 = {4}, p5 = {5}, "
		            "p6 = {6}, p7 = {7}, p8 = {8}, p9 = {9}, p10 = {10}, p11 = {11}, p12 = {12}, "
		            "p13 = {13}, p14 = {14}, p15 = {15}, p16 = {16}", 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 
		             11, 12, 13, 14, 15, 16);

		REQUIRE(Status_OperationFailed == status.GetCode());
		REQUIRE(String("p1 = 1, p2 = 2, p3 = 3, p4 = 4, p5 = 5, p6 = 6, p7 = 7, p8 = 8, p9 = 9, "
		        "p10 = 10, p11 = 11, p12 = 12, p13 = 13, p14 = 14, p15 = 15, p16 = 16") == 
		        status.GetMsg());
	}
}

