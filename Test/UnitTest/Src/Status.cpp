
#include "CX/CX.h"
#include "CX/Status.h"
#include "../../../Contrib/Catch/Include/catch.hpp"


using namespace CX;


TEST_CASE("Status tests", "[CX::Status]")
{
	SECTION("Clear")
	{
		Status::Set(Status_OperationFailed);
		Status::Clear();

		REQUIRE(Status_OK == Status::GetCode());
		REQUIRE(String("") == Status::GetMsg());
	}
	SECTION("IsOK")
	{
		Status::Set(Status_OperationFailed);

		REQUIRE(false == Status::IsOK());
	}
	SECTION("IsNOK")
	{
		Status::Set(Status_OperationFailed);

		REQUIRE(true == Status::IsNOK());
	}
	SECTION("Set code only")
	{
		Status::Set(Status_OperationFailed);

		REQUIRE(Status_OperationFailed == Status::GetCode());
		REQUIRE(String("") == Status::GetMsg());
	}
	SECTION("0 args")
	{
		Status::Set(Status_OperationFailed, "");

		REQUIRE(Status_OperationFailed == Status::GetCode());
		REQUIRE(String("") == Status::GetMsg());
	}
	SECTION("1 args")
	{
		Status::Set(Status_OperationFailed, "p1 = {1}", 1);

		REQUIRE(Status_OperationFailed == Status::GetCode());
		REQUIRE(String("p1 = 1") == Status::GetMsg());
	}
	SECTION("2 args")
	{
		Status::Set(Status_OperationFailed, "p1 = {1}, p2 = {2}", 1, 2);

		REQUIRE(Status_OperationFailed == Status::GetCode());
		REQUIRE(String("p1 = 1, p2 = 2") == Status::GetMsg());
	}
	SECTION("3 args")
	{
		Status::Set(Status_OperationFailed, "p1 = {1}, p2 = {2}, p3 = {3}", 1, 2, 3);

		REQUIRE(Status_OperationFailed == Status::GetCode());
		REQUIRE(String("p1 = 1, p2 = 2, p3 = 3") == Status::GetMsg());
	}
	SECTION("4 args")
	{
		Status::Set(Status_OperationFailed, "p1 = {1}, p2 = {2}, p3 = {3}, p4 = {4}", 1, 2, 3, 4);

		REQUIRE(Status_OperationFailed == Status::GetCode());
		REQUIRE(String("p1 = 1, p2 = 2, p3 = 3, p4 = 4") == Status::GetMsg());
	}
	SECTION("5 args")
	{
		Status::Set(Status_OperationFailed, "p1 = {1}, p2 = {2}, p3 = {3}, p4 = {4}, p5 = {5}", 
		            1, 2, 3, 4, 5);

		REQUIRE(Status_OperationFailed == Status::GetCode());
		REQUIRE(String("p1 = 1, p2 = 2, p3 = 3, p4 = 4, p5 = 5") == Status::GetMsg());
	}
	SECTION("6 args")
	{
		Status::Set(Status_OperationFailed, "p1 = {1}, p2 = {2}, p3 = {3}, p4 = {4}, p5 = {5}, "
		            "p6 = {6}", 1, 2, 3, 4, 5, 6);

		REQUIRE(Status_OperationFailed == Status::GetCode());
		REQUIRE(String("p1 = 1, p2 = 2, p3 = 3, p4 = 4, p5 = 5, p6 = 6") == Status::GetMsg());
	}
	SECTION("7 args")
	{
		Status::Set(Status_OperationFailed, "p1 = {1}, p2 = {2}, p3 = {3}, p4 = {4}, p5 = {5}, "
		            "p6 = {6}, p7 = {7}", 1, 2, 3, 4, 5, 6, 7);

		REQUIRE(Status_OperationFailed == Status::GetCode());
		REQUIRE(String("p1 = 1, p2 = 2, p3 = 3, p4 = 4, p5 = 5, p6 = 6, p7 = 7") == Status::GetMsg());
	}
	SECTION("8 args")
	{
		Status::Set(Status_OperationFailed, "p1 = {1}, p2 = {2}, p3 = {3}, p4 = {4}, p5 = {5}, "
		            "p6 = {6}, p7 = {7}, p8 = {8}", 1, 2, 3, 4, 5, 6, 7, 8);

		REQUIRE(Status_OperationFailed == Status::GetCode());
		REQUIRE(String("p1 = 1, p2 = 2, p3 = 3, p4 = 4, p5 = 5, p6 = 6, p7 = 7, p8 = 8") == 
		        Status::GetMsg());
	}
	SECTION("9 args")
	{
		Status::Set(Status_OperationFailed, "p1 = {1}, p2 = {2}, p3 = {3}, p4 = {4}, p5 = {5}, "
		            "p6 = {6}, p7 = {7}, p8 = {8}, p9 = {9}", 1, 2, 3, 4, 5, 6, 7, 8, 9);

		REQUIRE(Status_OperationFailed == Status::GetCode());
		REQUIRE(String("p1 = 1, p2 = 2, p3 = 3, p4 = 4, p5 = 5, p6 = 6, p7 = 7, p8 = 8, p9 = 9") == 
		        Status::GetMsg());
	}
	SECTION("10 args")
	{
		Status::Set(Status_OperationFailed, "p1 = {1}, p2 = {2}, p3 = {3}, p4 = {4}, p5 = {5}, "
		            "p6 = {6}, p7 = {7}, p8 = {8}, p9 = {9}, p10 = {10}", 1, 2, 3, 4, 5, 6, 7, 8, 9, 
		            10);

		REQUIRE(Status_OperationFailed == Status::GetCode());
		REQUIRE(String("p1 = 1, p2 = 2, p3 = 3, p4 = 4, p5 = 5, p6 = 6, p7 = 7, p8 = 8, p9 = 9, "
		        "p10 = 10") == Status::GetMsg());
	}
	SECTION("11 args")
	{
		Status::Set(Status_OperationFailed, "p1 = {1}, p2 = {2}, p3 = {3}, p4 = {4}, p5 = {5}, "
		            "p6 = {6}, p7 = {7}, p8 = {8}, p9 = {9}, p10 = {10}, p11 = {11}", 1, 2, 3, 4, 5, 
		            6, 7, 8, 9, 10, 11);

		REQUIRE(Status_OperationFailed == Status::GetCode());
		REQUIRE(String("p1 = 1, p2 = 2, p3 = 3, p4 = 4, p5 = 5, p6 = 6, p7 = 7, p8 = 8, p9 = 9, "
		        "p10 = 10, p11 = 11") == Status::GetMsg());
	}
	SECTION("12 args")
	{
		Status::Set(Status_OperationFailed, "p1 = {1}, p2 = {2}, p3 = {3}, p4 = {4}, p5 = {5}, "
		            "p6 = {6}, p7 = {7}, p8 = {8}, p9 = {9}, p10 = {10}, p11 = {11}, p12 = {12}", 1, 
		            2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12);

		REQUIRE(Status_OperationFailed == Status::GetCode());
		REQUIRE(String("p1 = 1, p2 = 2, p3 = 3, p4 = 4, p5 = 5, p6 = 6, p7 = 7, p8 = 8, p9 = 9, "
		        "p10 = 10, p11 = 11, p12 = 12") == Status::GetMsg());
	}
	SECTION("13 args")
	{
		Status::Set(Status_OperationFailed, "p1 = {1}, p2 = {2}, p3 = {3}, p4 = {4}, p5 = {5}, "
		            "p6 = {6}, p7 = {7}, p8 = {8}, p9 = {9}, p10 = {10}, p11 = {11}, p12 = {12}, "
		            "p13 = {13}", 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13);

		REQUIRE(Status_OperationFailed == Status::GetCode());
		REQUIRE(String("p1 = 1, p2 = 2, p3 = 3, p4 = 4, p5 = 5, p6 = 6, p7 = 7, p8 = 8, p9 = 9, "
		        "p10 = 10, p11 = 11, p12 = 12, p13 = 13") == Status::GetMsg());
	}
	SECTION("14 args")
	{
		Status::Set(Status_OperationFailed, "p1 = {1}, p2 = {2}, p3 = {3}, p4 = {4}, p5 = {5}, "
		            "p6 = {6}, p7 = {7}, p8 = {8}, p9 = {9}, p10 = {10}, p11 = {11}, p12 = {12}, "
		            "p13 = {13}, p14 = {14}", 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14);

		REQUIRE(Status_OperationFailed == Status::GetCode());
		REQUIRE(String("p1 = 1, p2 = 2, p3 = 3, p4 = 4, p5 = 5, p6 = 6, p7 = 7, p8 = 8, p9 = 9, "
		        "p10 = 10, p11 = 11, p12 = 12, p13 = 13, p14 = 14") == Status::GetMsg());
	}
	SECTION("15 args")
	{
		Status::Set(Status_OperationFailed, "p1 = {1}, p2 = {2}, p3 = {3}, p4 = {4}, p5 = {5}, "
		            "p6 = {6}, p7 = {7}, p8 = {8}, p9 = {9}, p10 = {10}, p11 = {11}, p12 = {12}, "
		            "p13 = {13}, p14 = {14}, p15 = {15}", 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 
		            14, 15);

		REQUIRE(Status_OperationFailed == Status::GetCode());
		REQUIRE(String("p1 = 1, p2 = 2, p3 = 3, p4 = 4, p5 = 5, p6 = 6, p7 = 7, p8 = 8, p9 = 9, "
		        "p10 = 10, p11 = 11, p12 = 12, p13 = 13, p14 = 14, p15 = 15") == Status::GetMsg());
	}
	SECTION("16 args")
	{
		Status::Set(Status_OperationFailed, "p1 = {1}, p2 = {2}, p3 = {3}, p4 = {4}, p5 = {5}, "
		            "p6 = {6}, p7 = {7}, p8 = {8}, p9 = {9}, p10 = {10}, p11 = {11}, p12 = {12}, "
		            "p13 = {13}, p14 = {14}, p15 = {15}, p16 = {16}", 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 
		             11, 12, 13, 14, 15, 16);

		REQUIRE(Status_OperationFailed == Status::GetCode());
		REQUIRE(String("p1 = 1, p2 = 2, p3 = 3, p4 = 4, p5 = 5, p6 = 6, p7 = 7, p8 = 8, p9 = 9, "
		        "p10 = 10, p11 = 11, p12 = 12, p13 = 13, p14 = 14, p15 = 15, p16 = 16") == 
		        Status::GetMsg());
	}
}

