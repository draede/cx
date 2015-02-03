
#include "CX/CX.h"
#include "CX/Util/Function.h"

#pragma warning(push)
#pragma warning(disable: 4702)
#include "../../../Contrib/Catch/Include/catch.hpp"
#pragma warning(pop)


using namespace CX;


namespace Util_Function
{

typedef int            MyT1;
typedef Bool           MyT2;
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

Bool   g_bRun = False;

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
	g_bRun = True;
}

void fnr1(MyT1 p1)
{
	g_bRun = True;
	g_p1   = p1;
}

void fnr2(MyT1 p1, MyT2 p2)
{
	g_bRun = True;
	g_p1   = p1;
	g_p2   = p2;
}

void fnr3(MyT1 p1, MyT2 p2, MyT3 p3)
{
	g_bRun = True;
	g_p1   = p1;
	g_p2   = p2;
	g_p3   = p3;
}

void fnr4(MyT1 p1, MyT2 p2, MyT3 p3, MyT4 p4)
{
	g_bRun = True;
	g_p1   = p1;
	g_p2   = p2;
	g_p3   = p3;
	g_p4   = p4;
}

void fnr5(MyT1 p1, MyT2 p2, MyT3 p3, MyT4 p4, MyT5 p5)
{
	g_bRun = True;
	g_p1   = p1;
	g_p2   = p2;
	g_p3   = p3;
	g_p4   = p4;
	g_p5   = p5;
}

void fnr6(MyT1 p1, MyT2 p2, MyT3 p3, MyT4 p4, MyT5 p5, MyT6 p6)
{
	g_bRun = True;
	g_p1   = p1;
	g_p2   = p2;
	g_p3   = p3;
	g_p4   = p4;
	g_p5   = p5;
	g_p6   = p6;
}

void fnr7(MyT1 p1, MyT2 p2, MyT3 p3, MyT4 p4, MyT5 p5, MyT6 p6, MyT7 p7)
{
	g_bRun = True;
	g_p1   = p1;
	g_p2   = p2;
	g_p3   = p3;
	g_p4   = p4;
	g_p5   = p5;
	g_p6   = p6;
	g_p7   = p7;
}

void fnr8(MyT1 p1, MyT2 p2, MyT3 p3, MyT4 p4, MyT5 p5, MyT6 p6, MyT7 p7, MyT8 p8)
{
	g_bRun = True;
	g_p1   = p1;
	g_p2   = p2;
	g_p3   = p3;
	g_p4   = p4;
	g_p5   = p5;
	g_p6   = p6;
	g_p7   = p7;
	g_p8   = p8;
}

void fnr9(MyT1 p1, MyT2 p2, MyT3 p3, MyT4 p4, MyT5 p5, MyT6 p6, MyT7 p7, MyT8 p8, MyT9 p9)
{
	g_bRun = True;
	g_p1   = p1;
	g_p2   = p2;
	g_p3   = p3;
	g_p4   = p4;
	g_p5   = p5;
	g_p6   = p6;
	g_p7   = p7;
	g_p8   = p8;
	g_p9   = p9;
}

void fnr10(MyT1 p1, MyT2 p2, MyT3 p3, MyT4 p4, MyT5 p5, MyT6 p6, MyT7 p7, MyT8 p8, MyT9 p9, 
           MyT10 p10)
{
	g_bRun = True;
	g_p1   = p1;
	g_p2   = p2;
	g_p3   = p3;
	g_p4   = p4;
	g_p5   = p5;
	g_p6   = p6;
	g_p7   = p7;
	g_p8   = p8;
	g_p9   = p9;
	g_p10  = p10;
}

void fnr11(MyT1 p1, MyT2 p2, MyT3 p3, MyT4 p4, MyT5 p5, MyT6 p6, MyT7 p7, MyT8 p8, MyT9 p9, 
           MyT10 p10, MyT11 p11)
{
	g_bRun = True;
	g_p1   = p1;
	g_p2   = p2;
	g_p3   = p3;
	g_p4   = p4;
	g_p5   = p5;
	g_p6   = p6;
	g_p7   = p7;
	g_p8   = p8;
	g_p9   = p9;
	g_p10  = p10;
	g_p11  = p11;
}

void fnr12(MyT1 p1, MyT2 p2, MyT3 p3, MyT4 p4, MyT5 p5, MyT6 p6, MyT7 p7, MyT8 p8, MyT9 p9, 
           MyT10 p10, MyT11 p11, MyT12 p12)
{
	g_bRun = True;
	g_p1   = p1;
	g_p2   = p2;
	g_p3   = p3;
	g_p4   = p4;
	g_p5   = p5;
	g_p6   = p6;
	g_p7   = p7;
	g_p8   = p8;
	g_p9   = p9;
	g_p10  = p10;
	g_p11  = p11;
	g_p12  = p12;
}

void fnr13(MyT1 p1, MyT2 p2, MyT3 p3, MyT4 p4, MyT5 p5, MyT6 p6, MyT7 p7, MyT8 p8, MyT9 p9, 
           MyT10 p10, MyT11 p11, MyT12 p12, MyT13 p13)
{
	g_bRun = True;
	g_p1   = p1;
	g_p2   = p2;
	g_p3   = p3;
	g_p4   = p4;
	g_p5   = p5;
	g_p6   = p6;
	g_p7   = p7;
	g_p8   = p8;
	g_p9   = p9;
	g_p10  = p10;
	g_p11  = p11;
	g_p12  = p12;
	g_p13  = p13;
}

void fnr14(MyT1 p1, MyT2 p2, MyT3 p3, MyT4 p4, MyT5 p5, MyT6 p6, MyT7 p7, MyT8 p8, MyT9 p9, 
           MyT10 p10, MyT11 p11, MyT12 p12, MyT13 p13, MyT14 p14)
{
	g_bRun = True;
	g_p1   = p1;
	g_p2   = p2;
	g_p3   = p3;
	g_p4   = p4;
	g_p5   = p5;
	g_p6   = p6;
	g_p7   = p7;
	g_p8   = p8;
	g_p9   = p9;
	g_p10  = p10;
	g_p11  = p11;
	g_p12  = p12;
	g_p13  = p13;
	g_p14  = p14;
}

void fnr15(MyT1 p1, MyT2 p2, MyT3 p3, MyT4 p4, MyT5 p5, MyT6 p6, MyT7 p7, MyT8 p8, MyT9 p9, 
           MyT10 p10, MyT11 p11, MyT12 p12, MyT13 p13, MyT14 p14, MyT15 p15)
{
	g_bRun = True;
	g_p1   = p1;
	g_p2   = p2;
	g_p3   = p3;
	g_p4   = p4;
	g_p5   = p5;
	g_p6   = p6;
	g_p7   = p7;
	g_p8   = p8;
	g_p9   = p9;
	g_p10  = p10;
	g_p11  = p11;
	g_p12  = p12;
	g_p13  = p13;
	g_p14  = p14;
	g_p15  = p15;
}

void fnr16(MyT1 p1, MyT2 p2, MyT3 p3, MyT4 p4, MyT5 p5, MyT6 p6, MyT7 p7, MyT8 p8, MyT9 p9, 
           MyT10 p10, MyT11 p11, MyT12 p12, MyT13 p13, MyT14 p14, MyT15 p15, MyT16 p16)
{
	g_bRun = True;
	g_p1   = p1;
	g_p2   = p2;
	g_p3   = p3;
	g_p4   = p4;
	g_p5   = p5;
	g_p6   = p6;
	g_p7   = p7;
	g_p8   = p8;
	g_p9   = p9;
	g_p10  = p10;
	g_p11  = p11;
	g_p12  = p12;
	g_p13  = p13;
	g_p14  = p14;
	g_p15  = p15;
	g_p16  = p16;
}

int fr0()
{
	g_bRun = True;

	return 123;
}

int fr1(int p1)
{
	g_bRun = True;
	g_p1   = p1;

	return 123;
}

int fr2(MyT1 p1, MyT2 p2)
{
	g_bRun = True;
	g_p1   = p1;
	g_p2   = p2;

	return 123;
}

int fr3(MyT1 p1, MyT2 p2, MyT3 p3)
{
	g_bRun = True;
	g_p1   = p1;
	g_p2   = p2;
	g_p3   = p3;

	return 123;
}

int fr4(MyT1 p1, MyT2 p2, MyT3 p3, MyT4 p4)
{
	g_bRun = True;
	g_p1   = p1;
	g_p2   = p2;
	g_p3   = p3;
	g_p4   = p4;

	return 123;
}

int fr5(MyT1 p1, MyT2 p2, MyT3 p3, MyT4 p4, MyT5 p5)
{
	g_bRun = True;
	g_p1   = p1;
	g_p2   = p2;
	g_p3   = p3;
	g_p4   = p4;
	g_p5   = p5;

	return 123;
}

int fr6(MyT1 p1, MyT2 p2, MyT3 p3, MyT4 p4, MyT5 p5, MyT6 p6)
{
	g_bRun = True;
	g_p1   = p1;
	g_p2   = p2;
	g_p3   = p3;
	g_p4   = p4;
	g_p5   = p5;
	g_p6   = p6;

	return 123;
}

int fr7(MyT1 p1, MyT2 p2, MyT3 p3, MyT4 p4, MyT5 p5, MyT6 p6, MyT7 p7)
{
	g_bRun = True;
	g_p1   = p1;
	g_p2   = p2;
	g_p3   = p3;
	g_p4   = p4;
	g_p5   = p5;
	g_p6   = p6;
	g_p7   = p7;

	return 123;
}

int fr8(MyT1 p1, MyT2 p2, MyT3 p3, MyT4 p4, MyT5 p5, MyT6 p6, MyT7 p7, MyT8 p8)
{
	g_bRun = True;
	g_p1   = p1;
	g_p2   = p2;
	g_p3   = p3;
	g_p4   = p4;
	g_p5   = p5;
	g_p6   = p6;
	g_p7   = p7;
	g_p8   = p8;

	return 123;
}

int fr9(MyT1 p1, MyT2 p2, MyT3 p3, MyT4 p4, MyT5 p5, MyT6 p6, MyT7 p7, MyT8 p8, MyT9 p9)
{
	g_bRun = True;
	g_p1   = p1;
	g_p2   = p2;
	g_p3   = p3;
	g_p4   = p4;
	g_p5   = p5;
	g_p6   = p6;
	g_p7   = p7;
	g_p8   = p8;
	g_p9   = p9;

	return 123;
}

int fr10(MyT1 p1, MyT2 p2, MyT3 p3, MyT4 p4, MyT5 p5, MyT6 p6, MyT7 p7, MyT8 p8, MyT9 p9, 
           MyT10 p10)
{
	g_bRun = True;
	g_p1   = p1;
	g_p2   = p2;
	g_p3   = p3;
	g_p4   = p4;
	g_p5   = p5;
	g_p6   = p6;
	g_p7   = p7;
	g_p8   = p8;
	g_p9   = p9;
	g_p10  = p10;

	return 123;
}

int fr11(MyT1 p1, MyT2 p2, MyT3 p3, MyT4 p4, MyT5 p5, MyT6 p6, MyT7 p7, MyT8 p8, MyT9 p9, 
           MyT10 p10, MyT11 p11)
{
	g_bRun = True;
	g_p1   = p1;
	g_p2   = p2;
	g_p3   = p3;
	g_p4   = p4;
	g_p5   = p5;
	g_p6   = p6;
	g_p7   = p7;
	g_p8   = p8;
	g_p9   = p9;
	g_p10  = p10;
	g_p11  = p11;

	return 123;
}

int fr12(MyT1 p1, MyT2 p2, MyT3 p3, MyT4 p4, MyT5 p5, MyT6 p6, MyT7 p7, MyT8 p8, MyT9 p9, 
           MyT10 p10, MyT11 p11, MyT12 p12)
{
	g_bRun = True;
	g_p1   = p1;
	g_p2   = p2;
	g_p3   = p3;
	g_p4   = p4;
	g_p5   = p5;
	g_p6   = p6;
	g_p7   = p7;
	g_p8   = p8;
	g_p9   = p9;
	g_p10  = p10;
	g_p11  = p11;
	g_p12  = p12;

	return 123;
}

int fr13(MyT1 p1, MyT2 p2, MyT3 p3, MyT4 p4, MyT5 p5, MyT6 p6, MyT7 p7, MyT8 p8, MyT9 p9, 
           MyT10 p10, MyT11 p11, MyT12 p12, MyT13 p13)
{
	g_bRun = True;
	g_p1   = p1;
	g_p2   = p2;
	g_p3   = p3;
	g_p4   = p4;
	g_p5   = p5;
	g_p6   = p6;
	g_p7   = p7;
	g_p8   = p8;
	g_p9   = p9;
	g_p10  = p10;
	g_p11  = p11;
	g_p12  = p12;
	g_p13  = p13;

	return 123;
}

int fr14(MyT1 p1, MyT2 p2, MyT3 p3, MyT4 p4, MyT5 p5, MyT6 p6, MyT7 p7, MyT8 p8, MyT9 p9, 
           MyT10 p10, MyT11 p11, MyT12 p12, MyT13 p13, MyT14 p14)
{
	g_bRun = True;
	g_p1   = p1;
	g_p2   = p2;
	g_p3   = p3;
	g_p4   = p4;
	g_p5   = p5;
	g_p6   = p6;
	g_p7   = p7;
	g_p8   = p8;
	g_p9   = p9;
	g_p10  = p10;
	g_p11  = p11;
	g_p12  = p12;
	g_p13  = p13;
	g_p14  = p14;

	return 123;
}

int fr15(MyT1 p1, MyT2 p2, MyT3 p3, MyT4 p4, MyT5 p5, MyT6 p6, MyT7 p7, MyT8 p8, MyT9 p9, 
           MyT10 p10, MyT11 p11, MyT12 p12, MyT13 p13, MyT14 p14, MyT15 p15)
{
	g_bRun = True;
	g_p1   = p1;
	g_p2   = p2;
	g_p3   = p3;
	g_p4   = p4;
	g_p5   = p5;
	g_p6   = p6;
	g_p7   = p7;
	g_p8   = p8;
	g_p9   = p9;
	g_p10  = p10;
	g_p11  = p11;
	g_p12  = p12;
	g_p13  = p13;
	g_p14  = p14;
	g_p15  = p15;

	return 123;
}

int fr16(MyT1 p1, MyT2 p2, MyT3 p3, MyT4 p4, MyT5 p5, MyT6 p6, MyT7 p7, MyT8 p8, MyT9 p9, 
           MyT10 p10, MyT11 p11, MyT12 p12, MyT13 p13, MyT14 p14, MyT15 p15, MyT16 p16)
{
	g_bRun = True;
	g_p1   = p1;
	g_p2   = p2;
	g_p3   = p3;
	g_p4   = p4;
	g_p5   = p5;
	g_p6   = p6;
	g_p7   = p7;
	g_p8   = p8;
	g_p9   = p9;
	g_p10  = p10;
	g_p11  = p11;
	g_p12  = p12;
	g_p13  = p13;
	g_p14  = p14;
	g_p15  = p15;
	g_p16  = p16;

	return 123;
}

class Test
{
public:

	void fnr0()
	{
		g_bRun = True;
	}

	void fnr1(MyT1 p1)
	{
		g_bRun = True;
		g_p1   = p1;
	}

	void fnr2(MyT1 p1, MyT2 p2)
	{
		g_bRun = True;
		g_p1   = p1;
		g_p2   = p2;
	}

	void fnr3(MyT1 p1, MyT2 p2, MyT3 p3)
	{
		g_bRun = True;
		g_p1   = p1;
		g_p2   = p2;
		g_p3   = p3;
	}

	void fnr4(MyT1 p1, MyT2 p2, MyT3 p3, MyT4 p4)
	{
		g_bRun = True;
		g_p1   = p1;
		g_p2   = p2;
		g_p3   = p3;
		g_p4   = p4;
	}

	void fnr5(MyT1 p1, MyT2 p2, MyT3 p3, MyT4 p4, MyT5 p5)
	{
		g_bRun = True;
		g_p1   = p1;
		g_p2   = p2;
		g_p3   = p3;
		g_p4   = p4;
		g_p5   = p5;
	}

	void fnr6(MyT1 p1, MyT2 p2, MyT3 p3, MyT4 p4, MyT5 p5, MyT6 p6)
	{
		g_bRun = True;
		g_p1   = p1;
		g_p2   = p2;
		g_p3   = p3;
		g_p4   = p4;
		g_p5   = p5;
		g_p6   = p6;
	}

	void fnr7(MyT1 p1, MyT2 p2, MyT3 p3, MyT4 p4, MyT5 p5, MyT6 p6, MyT7 p7)
	{
		g_bRun = True;
		g_p1   = p1;
		g_p2   = p2;
		g_p3   = p3;
		g_p4   = p4;
		g_p5   = p5;
		g_p6   = p6;
		g_p7   = p7;
	}

	void fnr8(MyT1 p1, MyT2 p2, MyT3 p3, MyT4 p4, MyT5 p5, MyT6 p6, MyT7 p7, MyT8 p8)
	{
		g_bRun = True;
		g_p1   = p1;
		g_p2   = p2;
		g_p3   = p3;
		g_p4   = p4;
		g_p5   = p5;
		g_p6   = p6;
		g_p7   = p7;
		g_p8   = p8;
	}

	void fnr9(MyT1 p1, MyT2 p2, MyT3 p3, MyT4 p4, MyT5 p5, MyT6 p6, MyT7 p7, MyT8 p8, MyT9 p9)
	{
		g_bRun = True;
		g_p1   = p1;
		g_p2   = p2;
		g_p3   = p3;
		g_p4   = p4;
		g_p5   = p5;
		g_p6   = p6;
		g_p7   = p7;
		g_p8   = p8;
		g_p9   = p9;
	}

	void fnr10(MyT1 p1, MyT2 p2, MyT3 p3, MyT4 p4, MyT5 p5, MyT6 p6, MyT7 p7, MyT8 p8, MyT9 p9, 
	           MyT10 p10)
	{
		g_bRun = True;
		g_p1   = p1;
		g_p2   = p2;
		g_p3   = p3;
		g_p4   = p4;
		g_p5   = p5;
		g_p6   = p6;
		g_p7   = p7;
		g_p8   = p8;
		g_p9   = p9;
		g_p10  = p10;
	}

	void fnr11(MyT1 p1, MyT2 p2, MyT3 p3, MyT4 p4, MyT5 p5, MyT6 p6, MyT7 p7, MyT8 p8, MyT9 p9, 
	           MyT10 p10, MyT11 p11)
	{
		g_bRun = True;
		g_p1   = p1;
		g_p2   = p2;
		g_p3   = p3;
		g_p4   = p4;
		g_p5   = p5;
		g_p6   = p6;
		g_p7   = p7;
		g_p8   = p8;
		g_p9   = p9;
		g_p10  = p10;
		g_p11  = p11;
	}

	void fnr12(MyT1 p1, MyT2 p2, MyT3 p3, MyT4 p4, MyT5 p5, MyT6 p6, MyT7 p7, MyT8 p8, MyT9 p9, 
	           MyT10 p10, MyT11 p11, MyT12 p12)
	{
		g_bRun = True;
		g_p1   = p1;
		g_p2   = p2;
		g_p3   = p3;
		g_p4   = p4;
		g_p5   = p5;
		g_p6   = p6;
		g_p7   = p7;
		g_p8   = p8;
		g_p9   = p9;
		g_p10  = p10;
		g_p11  = p11;
		g_p12  = p12;
	}

	void fnr13(MyT1 p1, MyT2 p2, MyT3 p3, MyT4 p4, MyT5 p5, MyT6 p6, MyT7 p7, MyT8 p8, MyT9 p9, 
	           MyT10 p10, MyT11 p11, MyT12 p12, MyT13 p13)
	{
		g_bRun = True;
		g_p1   = p1;
		g_p2   = p2;
		g_p3   = p3;
		g_p4   = p4;
		g_p5   = p5;
		g_p6   = p6;
		g_p7   = p7;
		g_p8   = p8;
		g_p9   = p9;
		g_p10  = p10;
		g_p11  = p11;
		g_p12  = p12;
		g_p13  = p13;
	}

	void fnr14(MyT1 p1, MyT2 p2, MyT3 p3, MyT4 p4, MyT5 p5, MyT6 p6, MyT7 p7, MyT8 p8, MyT9 p9, 
	           MyT10 p10, MyT11 p11, MyT12 p12, MyT13 p13, MyT14 p14)
	{
		g_bRun = True;
		g_p1   = p1;
		g_p2   = p2;
		g_p3   = p3;
		g_p4   = p4;
		g_p5   = p5;
		g_p6   = p6;
		g_p7   = p7;
		g_p8   = p8;
		g_p9   = p9;
		g_p10  = p10;
		g_p11  = p11;
		g_p12  = p12;
		g_p13  = p13;
		g_p14  = p14;
	}

	void fnr15(MyT1 p1, MyT2 p2, MyT3 p3, MyT4 p4, MyT5 p5, MyT6 p6, MyT7 p7, MyT8 p8, MyT9 p9, 
	           MyT10 p10, MyT11 p11, MyT12 p12, MyT13 p13, MyT14 p14, MyT15 p15)
	{
		g_bRun = True;
		g_p1   = p1;
		g_p2   = p2;
		g_p3   = p3;
		g_p4   = p4;
		g_p5   = p5;
		g_p6   = p6;
		g_p7   = p7;
		g_p8   = p8;
		g_p9   = p9;
		g_p10  = p10;
		g_p11  = p11;
		g_p12  = p12;
		g_p13  = p13;
		g_p14  = p14;
		g_p15  = p15;
	}

	void fnr16(MyT1 p1, MyT2 p2, MyT3 p3, MyT4 p4, MyT5 p5, MyT6 p6, MyT7 p7, MyT8 p8, MyT9 p9, 
	           MyT10 p10, MyT11 p11, MyT12 p12, MyT13 p13, MyT14 p14, MyT15 p15, MyT16 p16)
	{
		g_bRun = True;
		g_p1   = p1;
		g_p2   = p2;
		g_p3   = p3;
		g_p4   = p4;
		g_p5   = p5;
		g_p6   = p6;
		g_p7   = p7;
		g_p8   = p8;
		g_p9   = p9;
		g_p10  = p10;
		g_p11  = p11;
		g_p12  = p12;
		g_p13  = p13;
		g_p14  = p14;
		g_p15  = p15;
		g_p16  = p16;
	}

	int fr0()
	{
		g_bRun = True;

		return 123;
	}

	int fr1(int p1)
	{
		g_bRun = True;
		g_p1   = p1;

		return 123;
	}

	int fr2(MyT1 p1, MyT2 p2)
	{
		g_bRun = True;
		g_p1   = p1;
		g_p2   = p2;

		return 123;
	}

	int fr3(MyT1 p1, MyT2 p2, MyT3 p3)
	{
		g_bRun = True;
		g_p1   = p1;
		g_p2   = p2;
		g_p3   = p3;

		return 123;
	}

	int fr4(MyT1 p1, MyT2 p2, MyT3 p3, MyT4 p4)
	{
		g_bRun = True;
		g_p1   = p1;
		g_p2   = p2;
		g_p3   = p3;
		g_p4   = p4;

		return 123;
	}

	int fr5(MyT1 p1, MyT2 p2, MyT3 p3, MyT4 p4, MyT5 p5)
	{
		g_bRun = True;
		g_p1   = p1;
		g_p2   = p2;
		g_p3   = p3;
		g_p4   = p4;
		g_p5   = p5;

		return 123;
	}

	int fr6(MyT1 p1, MyT2 p2, MyT3 p3, MyT4 p4, MyT5 p5, MyT6 p6)
	{
		g_bRun = True;
		g_p1   = p1;
		g_p2   = p2;
		g_p3   = p3;
		g_p4   = p4;
		g_p5   = p5;
		g_p6   = p6;

		return 123;
	}

	int fr7(MyT1 p1, MyT2 p2, MyT3 p3, MyT4 p4, MyT5 p5, MyT6 p6, MyT7 p7)
	{
		g_bRun = True;
		g_p1   = p1;
		g_p2   = p2;
		g_p3   = p3;
		g_p4   = p4;
		g_p5   = p5;
		g_p6   = p6;
		g_p7   = p7;

		return 123;
	}

	int fr8(MyT1 p1, MyT2 p2, MyT3 p3, MyT4 p4, MyT5 p5, MyT6 p6, MyT7 p7, MyT8 p8)
	{
		g_bRun = True;
		g_p1   = p1;
		g_p2   = p2;
		g_p3   = p3;
		g_p4   = p4;
		g_p5   = p5;
		g_p6   = p6;
		g_p7   = p7;
		g_p8   = p8;

		return 123;
	}

	int fr9(MyT1 p1, MyT2 p2, MyT3 p3, MyT4 p4, MyT5 p5, MyT6 p6, MyT7 p7, MyT8 p8, MyT9 p9)
	{
		g_bRun = True;
		g_p1   = p1;
		g_p2   = p2;
		g_p3   = p3;
		g_p4   = p4;
		g_p5   = p5;
		g_p6   = p6;
		g_p7   = p7;
		g_p8   = p8;
		g_p9   = p9;

		return 123;
	}

	int fr10(MyT1 p1, MyT2 p2, MyT3 p3, MyT4 p4, MyT5 p5, MyT6 p6, MyT7 p7, MyT8 p8, MyT9 p9, 
	           MyT10 p10)
	{
		g_bRun = True;
		g_p1   = p1;
		g_p2   = p2;
		g_p3   = p3;
		g_p4   = p4;
		g_p5   = p5;
		g_p6   = p6;
		g_p7   = p7;
		g_p8   = p8;
		g_p9   = p9;
		g_p10  = p10;

		return 123;
	}

	int fr11(MyT1 p1, MyT2 p2, MyT3 p3, MyT4 p4, MyT5 p5, MyT6 p6, MyT7 p7, MyT8 p8, MyT9 p9, 
	           MyT10 p10, MyT11 p11)
	{
		g_bRun = True;
		g_p1   = p1;
		g_p2   = p2;
		g_p3   = p3;
		g_p4   = p4;
		g_p5   = p5;
		g_p6   = p6;
		g_p7   = p7;
		g_p8   = p8;
		g_p9   = p9;
		g_p10  = p10;
		g_p11  = p11;

		return 123;
	}

	int fr12(MyT1 p1, MyT2 p2, MyT3 p3, MyT4 p4, MyT5 p5, MyT6 p6, MyT7 p7, MyT8 p8, MyT9 p9, 
	           MyT10 p10, MyT11 p11, MyT12 p12)
	{
		g_bRun = True;
		g_p1   = p1;
		g_p2   = p2;
		g_p3   = p3;
		g_p4   = p4;
		g_p5   = p5;
		g_p6   = p6;
		g_p7   = p7;
		g_p8   = p8;
		g_p9   = p9;
		g_p10  = p10;
		g_p11  = p11;
		g_p12  = p12;

		return 123;
	}

	int fr13(MyT1 p1, MyT2 p2, MyT3 p3, MyT4 p4, MyT5 p5, MyT6 p6, MyT7 p7, MyT8 p8, MyT9 p9, 
	           MyT10 p10, MyT11 p11, MyT12 p12, MyT13 p13)
	{
		g_bRun = True;
		g_p1   = p1;
		g_p2   = p2;
		g_p3   = p3;
		g_p4   = p4;
		g_p5   = p5;
		g_p6   = p6;
		g_p7   = p7;
		g_p8   = p8;
		g_p9   = p9;
		g_p10  = p10;
		g_p11  = p11;
		g_p12  = p12;
		g_p13  = p13;

		return 123;
	}

	int fr14(MyT1 p1, MyT2 p2, MyT3 p3, MyT4 p4, MyT5 p5, MyT6 p6, MyT7 p7, MyT8 p8, MyT9 p9, 
	           MyT10 p10, MyT11 p11, MyT12 p12, MyT13 p13, MyT14 p14)
	{
		g_bRun = True;
		g_p1   = p1;
		g_p2   = p2;
		g_p3   = p3;
		g_p4   = p4;
		g_p5   = p5;
		g_p6   = p6;
		g_p7   = p7;
		g_p8   = p8;
		g_p9   = p9;
		g_p10  = p10;
		g_p11  = p11;
		g_p12  = p12;
		g_p13  = p13;
		g_p14  = p14;

		return 123;
	}

	int fr15(MyT1 p1, MyT2 p2, MyT3 p3, MyT4 p4, MyT5 p5, MyT6 p6, MyT7 p7, MyT8 p8, MyT9 p9, 
	           MyT10 p10, MyT11 p11, MyT12 p12, MyT13 p13, MyT14 p14, MyT15 p15)
	{
		g_bRun = True;
		g_p1   = p1;
		g_p2   = p2;
		g_p3   = p3;
		g_p4   = p4;
		g_p5   = p5;
		g_p6   = p6;
		g_p7   = p7;
		g_p8   = p8;
		g_p9   = p9;
		g_p10  = p10;
		g_p11  = p11;
		g_p12  = p12;
		g_p13  = p13;
		g_p14  = p14;
		g_p15  = p15;

		return 123;
	}

	int fr16(MyT1 p1, MyT2 p2, MyT3 p3, MyT4 p4, MyT5 p5, MyT6 p6, MyT7 p7, MyT8 p8, MyT9 p9, 
	           MyT10 p10, MyT11 p11, MyT12 p12, MyT13 p13, MyT14 p14, MyT15 p15, MyT16 p16)
	{
		g_bRun = True;
		g_p1   = p1;
		g_p2   = p2;
		g_p3   = p3;
		g_p4   = p4;
		g_p5   = p5;
		g_p6   = p6;
		g_p7   = p7;
		g_p8   = p8;
		g_p9   = p9;
		g_p10  = p10;
		g_p11  = p11;
		g_p12  = p12;
		g_p13  = p13;
		g_p14  = p14;
		g_p15  = p15;
		g_p16  = p16;

		return 123;
	}

};

template <typename R>
R RunR(Util::IFunction<R> *pFunction)
{
	g_bRun = False;
	return pFunction->Run();
}

void RunNR(Util::IFunction<void> *pFunction)
{
	g_bRun = False;
	pFunction->Run();
}

TEST_CASE("Function tests", "[CX::Util::Function]")
{
	SECTION("0 args no return")
	{
		Util::Function<void> f(&fnr0);

		RunNR(&f);
		REQUIRE(True == g_bRun);
	}
	SECTION("1 arg no return")
	{
		Util::Function<void, MyT1> f(&fnr1, 1);

		RunNR(&f);
		REQUIRE(True == g_bRun);
		REQUIRE(1 == g_p1);
	}
	SECTION("2 args no return")
	{
		Util::Function<void, MyT1, MyT2> f(&fnr2, 1, True);

		RunNR(&f);
		REQUIRE(True == g_bRun);
		REQUIRE(1 == g_p1);
		REQUIRE(True == g_p2);
	}
	SECTION("3 args no return")
	{
		Util::Function<void, MyT1, MyT2, MyT3> f(&fnr3, 1, True, 3);

		RunNR(&f);
		REQUIRE(True == g_bRun);
		REQUIRE(1 == g_p1);
		REQUIRE(True == g_p2);
		REQUIRE(3 == g_p3);
	}
	SECTION("4 args no return")
	{
		Util::Function<void, MyT1, MyT2, MyT3, MyT4> f(&fnr4, 1, True, 3, "4");

		RunNR(&f);
		REQUIRE(True == g_bRun);
		REQUIRE(1 == g_p1);
		REQUIRE(True == g_p2);
		REQUIRE(3 == g_p3);
		REQUIRE(String("4") == g_p4);
	}
	SECTION("5 args no return")
	{
		Util::Function<void, MyT1, MyT2, MyT3, MyT4, MyT5> f(&fnr5, 1, True, 3, "4", 5);

		RunNR(&f);
		REQUIRE(True == g_bRun);
		REQUIRE(1 == g_p1);
		REQUIRE(True == g_p2);
		REQUIRE(3 == g_p3);
		REQUIRE(String("4") == g_p4);
		REQUIRE(5 == g_p5);
	}
	SECTION("6 args no return")
	{
		Util::Function<void, MyT1, MyT2, MyT3, MyT4, MyT5, MyT6> f(&fnr6, 1, True, 3, "4", 5, 6);

		RunNR(&f);
		REQUIRE(True == g_bRun);
		REQUIRE(1 == g_p1);
		REQUIRE(True == g_p2);
		REQUIRE(3 == g_p3);
		REQUIRE(String("4") == g_p4);
		REQUIRE(5 == g_p5);
		REQUIRE(6 == g_p6);
	}
	SECTION("7 args no return")
	{
		Util::Function<void, MyT1, MyT2, MyT3, MyT4, MyT5, MyT6, MyT7> 
		         f(&fnr7, 1, True, 3, "4", 5, 6, 7);

		RunNR(&f);
		REQUIRE(True == g_bRun);
		REQUIRE(1 == g_p1);
		REQUIRE(True == g_p2);
		REQUIRE(3 == g_p3);
		REQUIRE(String("4") == g_p4);
		REQUIRE(5 == g_p5);
		REQUIRE(6 == g_p6);
		REQUIRE(7 == g_p7);
	}
	SECTION("8 args no return")
	{
		Util::Function<void, MyT1, MyT2, MyT3, MyT4, MyT5, MyT6, MyT7, MyT8> 
		         f(&fnr8, 1, True, 3, "4", 5, 6, 7, 8);

		RunNR(&f);
		REQUIRE(True == g_bRun);
		REQUIRE(1 == g_p1);
		REQUIRE(True == g_p2);
		REQUIRE(3 == g_p3);
		REQUIRE(String("4") == g_p4);
		REQUIRE(5 == g_p5);
		REQUIRE(6 == g_p6);
		REQUIRE(7 == g_p7);
		REQUIRE(8 == g_p8);
	}
	SECTION("9 args no return")
	{
		Util::Function<void, MyT1, MyT2, MyT3, MyT4, MyT5, MyT6, MyT7, MyT8, MyT9>
		         f(&fnr9, 1, True, 3, "4", 5, 6, 7, 8, 9);

		RunNR(&f);
		REQUIRE(True == g_bRun);
		REQUIRE(1 == g_p1);
		REQUIRE(True == g_p2);
		REQUIRE(3 == g_p3);
		REQUIRE(String("4") == g_p4);
		REQUIRE(5 == g_p5);
		REQUIRE(6 == g_p6);
		REQUIRE(7 == g_p7);
		REQUIRE(8 == g_p8);
		REQUIRE(9 == g_p9);
	}
	SECTION("10 args no return")
	{
		Util::Function<void, MyT1, MyT2, MyT3, MyT4, MyT5, MyT6, MyT7, MyT8, MyT9, MyT10>
		         f(&fnr10, 1, True, 3, "4", 5, 6, 7, 8, 9, 10);

		RunNR(&f);
		REQUIRE(True == g_bRun);
		REQUIRE(1 == g_p1);
		REQUIRE(True == g_p2);
		REQUIRE(3 == g_p3);
		REQUIRE(String("4") == g_p4);
		REQUIRE(5 == g_p5);
		REQUIRE(6 == g_p6);
		REQUIRE(7 == g_p7);
		REQUIRE(8 == g_p8);
		REQUIRE(9 == g_p9);
		REQUIRE(10 == g_p10);
	}
	SECTION("11 args no return")
	{
		Util::Function<void, MyT1, MyT2, MyT3, MyT4, MyT5, MyT6, MyT7, MyT8, MyT9, MyT10, MyT11>
		         f(&fnr11, 1, True, 3, "4", 5, 6, 7, 8, 9, 10, 11);

		RunNR(&f);
		REQUIRE(True == g_bRun);
		REQUIRE(1 == g_p1);
		REQUIRE(True == g_p2);
		REQUIRE(3 == g_p3);
		REQUIRE(String("4") == g_p4);
		REQUIRE(5 == g_p5);
		REQUIRE(6 == g_p6);
		REQUIRE(7 == g_p7);
		REQUIRE(8 == g_p8);
		REQUIRE(9 == g_p9);
		REQUIRE(10 == g_p10);
		REQUIRE(11 == g_p11);
	}
	SECTION("12 args no return")
	{
		Util::Function<void, MyT1, MyT2, MyT3, MyT4, MyT5, MyT6, MyT7, MyT8, MyT9, MyT10, MyT11, 
		              MyT12>
		         f(&fnr12, 1, True, 3, "4", 5, 6, 7, 8, 9, 10, 11, 12);

		RunNR(&f);
		REQUIRE(True == g_bRun);
		REQUIRE(1 == g_p1);
		REQUIRE(True == g_p2);
		REQUIRE(3 == g_p3);
		REQUIRE(String("4") == g_p4);
		REQUIRE(5 == g_p5);
		REQUIRE(6 == g_p6);
		REQUIRE(7 == g_p7);
		REQUIRE(8 == g_p8);
		REQUIRE(9 == g_p9);
		REQUIRE(10 == g_p10);
		REQUIRE(11 == g_p11);
		REQUIRE(12 == g_p12);
	}
	SECTION("13 args no return")
	{
		Util::Function<void, MyT1, MyT2, MyT3, MyT4, MyT5, MyT6, MyT7, MyT8, MyT9, MyT10, MyT11,
		               MyT12, MyT13>
		         f(&fnr13, 1, True, 3, "4", 5, 6, 7, 8, 9, 10, 11, 12, 13);

		RunNR(&f);
		REQUIRE(True == g_bRun);
		REQUIRE(1 == g_p1);
		REQUIRE(True == g_p2);
		REQUIRE(3 == g_p3);
		REQUIRE(String("4") == g_p4);
		REQUIRE(5 == g_p5);
		REQUIRE(6 == g_p6);
		REQUIRE(7 == g_p7);
		REQUIRE(8 == g_p8);
		REQUIRE(9 == g_p9);
		REQUIRE(10 == g_p10);
		REQUIRE(11 == g_p11);
		REQUIRE(12 == g_p12);
		REQUIRE(13 == g_p13);
	}
	SECTION("14 args no return")
	{
		Util::Function<void, MyT1, MyT2, MyT3, MyT4, MyT5, MyT6, MyT7, MyT8, MyT9, MyT10, MyT11,
		               MyT12, MyT13, MyT14>
		          f(&fnr14, 1, True, 3, "4", 5, 6, 7, 8, 9, 10, 11, 12, 13, 14);

		RunNR(&f);
		REQUIRE(True == g_bRun);
		REQUIRE(1 == g_p1);
		REQUIRE(True == g_p2);
		REQUIRE(3 == g_p3);
		REQUIRE(String("4") == g_p4);
		REQUIRE(5 == g_p5);
		REQUIRE(6 == g_p6);
		REQUIRE(7 == g_p7);
		REQUIRE(8 == g_p8);
		REQUIRE(9 == g_p9);
		REQUIRE(10 == g_p10);
		REQUIRE(11 == g_p11);
		REQUIRE(12 == g_p12);
		REQUIRE(13 == g_p13);
		REQUIRE(14 == g_p14);
	}
	SECTION("15 args no return")
	{
		Util::Function<void, MyT1, MyT2, MyT3, MyT4, MyT5, MyT6, MyT7, MyT8, MyT9, MyT10, MyT11,
		               MyT12, MyT13, MyT14, MyT15>
		          f(&fnr15, 1, True, 3, "4", 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 'x');

		RunNR(&f);
		REQUIRE(True == g_bRun);
		REQUIRE(1 == g_p1);
		REQUIRE(True == g_p2);
		REQUIRE(3 == g_p3);
		REQUIRE(String("4") == g_p4);
		REQUIRE(5 == g_p5);
		REQUIRE(6 == g_p6);
		REQUIRE(7 == g_p7);
		REQUIRE(8 == g_p8);
		REQUIRE(9 == g_p9);
		REQUIRE(10 == g_p10);
		REQUIRE(11 == g_p11);
		REQUIRE(12 == g_p12);
		REQUIRE(13 == g_p13);
		REQUIRE(14 == g_p14);
		REQUIRE('x' == g_p15);
	}
	SECTION("16 args no return")
	{
		Util::Function<void, MyT1, MyT2, MyT3, MyT4, MyT5, MyT6, MyT7, MyT8, MyT9, MyT10, MyT11,
		               MyT12, MyT13, MyT14, MyT15, MyT16>
		          f(&fnr16, 1, True, 3, "4", 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 'x', "16");

		RunNR(&f);
		REQUIRE(True == g_bRun);
		REQUIRE(1 == g_p1);
		REQUIRE(True == g_p2);
		REQUIRE(3 == g_p3);
		REQUIRE(String("4") == g_p4);
		REQUIRE(5 == g_p5);
		REQUIRE(6 == g_p6);
		REQUIRE(7 == g_p7);
		REQUIRE(8 == g_p8);
		REQUIRE(9 == g_p9);
		REQUIRE(10 == g_p10);
		REQUIRE(11 == g_p11);
		REQUIRE(12 == g_p12);
		REQUIRE(13 == g_p13);
		REQUIRE(14 == g_p14);
		REQUIRE('x' == g_p15);
		REQUIRE(String("16") == g_p16);
	}

	SECTION("0 args return")
	{
		Util::Function<int> f(&fr0);

		REQUIRE(123 == RunR(&f));
		REQUIRE(True == g_bRun);
	}

	SECTION("1 arg with return")
	{
		Util::Function<int, MyT1> f(&fr1, 1);

		REQUIRE(123 == RunR(&f));
		REQUIRE(True == g_bRun);
		REQUIRE(1 == g_p1);
	}
	SECTION("2 args with return")
	{
		Util::Function<int, MyT1, MyT2> f(&fr2, 1, True);

		REQUIRE(123 == RunR(&f));
		REQUIRE(True == g_bRun);
		REQUIRE(1 == g_p1);
		REQUIRE(True == g_p2);
	}
	SECTION("3 args with return")
	{
		Util::Function<int, MyT1, MyT2, MyT3> f(&fr3, 1, True, 3);

		REQUIRE(123 == RunR(&f));
		REQUIRE(True == g_bRun);
		REQUIRE(1 == g_p1);
		REQUIRE(True == g_p2);
		REQUIRE(3 == g_p3);
	}
	SECTION("4 args with return")
	{
		Util::Function<int, MyT1, MyT2, MyT3, MyT4> f(&fr4, 1, True, 3, "4");

		REQUIRE(123 == RunR(&f));
		REQUIRE(True == g_bRun);
		REQUIRE(1 == g_p1);
		REQUIRE(True == g_p2);
		REQUIRE(3 == g_p3);
		REQUIRE(String("4") == g_p4);
	}
	SECTION("5 args with return")
	{
		Util::Function<int, MyT1, MyT2, MyT3, MyT4, MyT5> f(&fr5, 1, True, 3, "4", 5);

		REQUIRE(123 == RunR(&f));
		REQUIRE(True == g_bRun);
		REQUIRE(1 == g_p1);
		REQUIRE(True == g_p2);
		REQUIRE(3 == g_p3);
		REQUIRE(String("4") == g_p4);
		REQUIRE(5 == g_p5);
	}
	SECTION("6 args with return")
	{
		Util::Function<int, MyT1, MyT2, MyT3, MyT4, MyT5, MyT6> f(&fr6, 1, True, 3, "4", 5, 6);

		REQUIRE(123 == RunR(&f));
		REQUIRE(True == g_bRun);
		REQUIRE(1 == g_p1);
		REQUIRE(True == g_p2);
		REQUIRE(3 == g_p3);
		REQUIRE(String("4") == g_p4);
		REQUIRE(5 == g_p5);
		REQUIRE(6 == g_p6);
	}
	SECTION("7 args with return")
	{
		Util::Function<int, MyT1, MyT2, MyT3, MyT4, MyT5, MyT6, MyT7> 
		         f(&fr7, 1, True, 3, "4", 5, 6, 7);

		REQUIRE(123 == RunR(&f));
		REQUIRE(True == g_bRun);
		REQUIRE(1 == g_p1);
		REQUIRE(True == g_p2);
		REQUIRE(3 == g_p3);
		REQUIRE(String("4") == g_p4);
		REQUIRE(5 == g_p5);
		REQUIRE(6 == g_p6);
		REQUIRE(7 == g_p7);
	}
	SECTION("8 args with return")
	{
		Util::Function<int, MyT1, MyT2, MyT3, MyT4, MyT5, MyT6, MyT7, MyT8> 
		         f(&fr8, 1, True, 3, "4", 5, 6, 7, 8);

		REQUIRE(123 == RunR(&f));
		REQUIRE(True == g_bRun);
		REQUIRE(1 == g_p1);
		REQUIRE(True == g_p2);
		REQUIRE(3 == g_p3);
		REQUIRE(String("4") == g_p4);
		REQUIRE(5 == g_p5);
		REQUIRE(6 == g_p6);
		REQUIRE(7 == g_p7);
		REQUIRE(8 == g_p8);
	}
	SECTION("9 args with return")
	{
		Util::Function<int, MyT1, MyT2, MyT3, MyT4, MyT5, MyT6, MyT7, MyT8, MyT9>
		         f(&fr9, 1, True, 3, "4", 5, 6, 7, 8, 9);

		REQUIRE(123 == RunR(&f));
		REQUIRE(True == g_bRun);
		REQUIRE(1 == g_p1);
		REQUIRE(True == g_p2);
		REQUIRE(3 == g_p3);
		REQUIRE(String("4") == g_p4);
		REQUIRE(5 == g_p5);
		REQUIRE(6 == g_p6);
		REQUIRE(7 == g_p7);
		REQUIRE(8 == g_p8);
		REQUIRE(9 == g_p9);
	}
	SECTION("10 args with return")
	{
		Util::Function<int, MyT1, MyT2, MyT3, MyT4, MyT5, MyT6, MyT7, MyT8, MyT9, MyT10>
		         f(&fr10, 1, True, 3, "4", 5, 6, 7, 8, 9, 10);

		REQUIRE(123 == RunR(&f));
		REQUIRE(True == g_bRun);
		REQUIRE(1 == g_p1);
		REQUIRE(True == g_p2);
		REQUIRE(3 == g_p3);
		REQUIRE(String("4") == g_p4);
		REQUIRE(5 == g_p5);
		REQUIRE(6 == g_p6);
		REQUIRE(7 == g_p7);
		REQUIRE(8 == g_p8);
		REQUIRE(9 == g_p9);
		REQUIRE(10 == g_p10);
	}
	SECTION("11 args with return")
	{
		Util::Function<int, MyT1, MyT2, MyT3, MyT4, MyT5, MyT6, MyT7, MyT8, MyT9, MyT10, MyT11>
		         f(&fr11, 1, True, 3, "4", 5, 6, 7, 8, 9, 10, 11);

		REQUIRE(123 == RunR(&f));
		REQUIRE(True == g_bRun);
		REQUIRE(1 == g_p1);
		REQUIRE(True == g_p2);
		REQUIRE(3 == g_p3);
		REQUIRE(String("4") == g_p4);
		REQUIRE(5 == g_p5);
		REQUIRE(6 == g_p6);
		REQUIRE(7 == g_p7);
		REQUIRE(8 == g_p8);
		REQUIRE(9 == g_p9);
		REQUIRE(10 == g_p10);
		REQUIRE(11 == g_p11);
	}
	SECTION("12 args with return")
	{
		Util::Function<int, MyT1, MyT2, MyT3, MyT4, MyT5, MyT6, MyT7, MyT8, MyT9, MyT10, MyT11, 
		              MyT12>
		         f(&fr12, 1, True, 3, "4", 5, 6, 7, 8, 9, 10, 11, 12);

		REQUIRE(123 == RunR(&f));
		REQUIRE(True == g_bRun);
		REQUIRE(1 == g_p1);
		REQUIRE(True == g_p2);
		REQUIRE(3 == g_p3);
		REQUIRE(String("4") == g_p4);
		REQUIRE(5 == g_p5);
		REQUIRE(6 == g_p6);
		REQUIRE(7 == g_p7);
		REQUIRE(8 == g_p8);
		REQUIRE(9 == g_p9);
		REQUIRE(10 == g_p10);
		REQUIRE(11 == g_p11);
		REQUIRE(12 == g_p12);
	}
	SECTION("13 args with return")
	{
		Util::Function<int, MyT1, MyT2, MyT3, MyT4, MyT5, MyT6, MyT7, MyT8, MyT9, MyT10, MyT11,
		               MyT12, MyT13>
		         f(&fr13, 1, True, 3, "4", 5, 6, 7, 8, 9, 10, 11, 12, 13);

		REQUIRE(123 == RunR(&f));
		REQUIRE(True == g_bRun);
		REQUIRE(1 == g_p1);
		REQUIRE(True == g_p2);
		REQUIRE(3 == g_p3);
		REQUIRE(String("4") == g_p4);
		REQUIRE(5 == g_p5);
		REQUIRE(6 == g_p6);
		REQUIRE(7 == g_p7);
		REQUIRE(8 == g_p8);
		REQUIRE(9 == g_p9);
		REQUIRE(10 == g_p10);
		REQUIRE(11 == g_p11);
		REQUIRE(12 == g_p12);
		REQUIRE(13 == g_p13);
	}
	SECTION("14 args with return")
	{
		Util::Function<int, MyT1, MyT2, MyT3, MyT4, MyT5, MyT6, MyT7, MyT8, MyT9, MyT10, MyT11,
		               MyT12, MyT13, MyT14>
		          f(&fr14, 1, True, 3, "4", 5, 6, 7, 8, 9, 10, 11, 12, 13, 14);

		REQUIRE(123 == RunR(&f));
		REQUIRE(True == g_bRun);
		REQUIRE(1 == g_p1);
		REQUIRE(True == g_p2);
		REQUIRE(3 == g_p3);
		REQUIRE(String("4") == g_p4);
		REQUIRE(5 == g_p5);
		REQUIRE(6 == g_p6);
		REQUIRE(7 == g_p7);
		REQUIRE(8 == g_p8);
		REQUIRE(9 == g_p9);
		REQUIRE(10 == g_p10);
		REQUIRE(11 == g_p11);
		REQUIRE(12 == g_p12);
		REQUIRE(13 == g_p13);
		REQUIRE(14 == g_p14);
	}
	SECTION("15 args with return")
	{
		Util::Function<int, MyT1, MyT2, MyT3, MyT4, MyT5, MyT6, MyT7, MyT8, MyT9, MyT10, MyT11,
		               MyT12, MyT13, MyT14, MyT15>
		          f(&fr15, 1, True, 3, "4", 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 'x');

		REQUIRE(123 == RunR(&f));
		REQUIRE(True == g_bRun);
		REQUIRE(1 == g_p1);
		REQUIRE(True == g_p2);
		REQUIRE(3 == g_p3);
		REQUIRE(String("4") == g_p4);
		REQUIRE(5 == g_p5);
		REQUIRE(6 == g_p6);
		REQUIRE(7 == g_p7);
		REQUIRE(8 == g_p8);
		REQUIRE(9 == g_p9);
		REQUIRE(10 == g_p10);
		REQUIRE(11 == g_p11);
		REQUIRE(12 == g_p12);
		REQUIRE(13 == g_p13);
		REQUIRE(14 == g_p14);
		REQUIRE('x' == g_p15);
	}
	SECTION("16 args with return")
	{
		Util::Function<int, MyT1, MyT2, MyT3, MyT4, MyT5, MyT6, MyT7, MyT8, MyT9, MyT10, MyT11,
		               MyT12, MyT13, MyT14, MyT15, MyT16>
		          f(&fr16, 1, True, 3, "4", 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 'x', "16");

		REQUIRE(123 == RunR(&f));
		REQUIRE(True == g_bRun);
		REQUIRE(1 == g_p1);
		REQUIRE(True == g_p2);
		REQUIRE(3 == g_p3);
		REQUIRE(String("4") == g_p4);
		REQUIRE(5 == g_p5);
		REQUIRE(6 == g_p6);
		REQUIRE(7 == g_p7);
		REQUIRE(8 == g_p8);
		REQUIRE(9 == g_p9);
		REQUIRE(10 == g_p10);
		REQUIRE(11 == g_p11);
		REQUIRE(12 == g_p12);
		REQUIRE(13 == g_p13);
		REQUIRE(14 == g_p14);
		REQUIRE('x' == g_p15);
		REQUIRE(String("16") == g_p16);
	}
}

TEST_CASE("MemberFunction tests", "[CX::Util::MemberFunction]")
{
	SECTION("0 args no return")
	{
		Test test;
		Util::MemberFunction<Test, void> f(&test, &Test::fnr0);

		RunNR(&f);
		REQUIRE(True == g_bRun);
	}
	SECTION("1 arg no return")
	{
		Test test;
		Util::MemberFunction<Test, void, MyT1> f(&test, &Test::fnr1, 1);

		RunNR(&f);
		REQUIRE(True == g_bRun);
		REQUIRE(1 == g_p1);
	}
	SECTION("2 args no return")
	{
		Test test;
		Util::MemberFunction<Test, void, MyT1, MyT2> f(&test, &Test::fnr2, 1, True);

		RunNR(&f);
		REQUIRE(True == g_bRun);
		REQUIRE(1 == g_p1);
		REQUIRE(True == g_p2);
	}
	SECTION("3 args no return")
	{
		Test test;
		Util::MemberFunction<Test, void, MyT1, MyT2, MyT3> f(&test, &Test::fnr3, 1, True, 3);

		RunNR(&f);
		REQUIRE(True == g_bRun);
		REQUIRE(1 == g_p1);
		REQUIRE(True == g_p2);
		REQUIRE(3 == g_p3);
	}
	SECTION("4 args no return")
	{
		Test test;
		Util::MemberFunction<Test, void, MyT1, MyT2, MyT3, MyT4> 
		         f(&test, &Test::fnr4, 1, True, 3, "4");

		RunNR(&f);
		REQUIRE(True == g_bRun);
		REQUIRE(1 == g_p1);
		REQUIRE(True == g_p2);
		REQUIRE(3 == g_p3);
		REQUIRE(String("4") == g_p4);
	}
	SECTION("5 args no return")
	{
		Test test;
		Util::MemberFunction<Test, void, MyT1, MyT2, MyT3, MyT4, MyT5> 
		         f(&test, &Test::fnr5, 1, True, 3, "4", 5);

		RunNR(&f);
		REQUIRE(True == g_bRun);
		REQUIRE(1 == g_p1);
		REQUIRE(True == g_p2);
		REQUIRE(3 == g_p3);
		REQUIRE(String("4") == g_p4);
		REQUIRE(5 == g_p5);
	}
	SECTION("6 args no return")
	{
		Test test;
		Util::MemberFunction<Test, void, MyT1, MyT2, MyT3, MyT4, MyT5, MyT6> 
		         f(&test, &Test::fnr6, 1, True, 3, "4", 5, 6);

		RunNR(&f);
		REQUIRE(True == g_bRun);
		REQUIRE(1 == g_p1);
		REQUIRE(True == g_p2);
		REQUIRE(3 == g_p3);
		REQUIRE(String("4") == g_p4);
		REQUIRE(5 == g_p5);
		REQUIRE(6 == g_p6);
	}
	SECTION("7 args no return")
	{
		Test test;
		Util::MemberFunction<Test, void, MyT1, MyT2, MyT3, MyT4, MyT5, MyT6, MyT7> 
		         f(&test, &Test::fnr7, 1, True, 3, "4", 5, 6, 7);

		RunNR(&f);
		REQUIRE(True == g_bRun);
		REQUIRE(1 == g_p1);
		REQUIRE(True == g_p2);
		REQUIRE(3 == g_p3);
		REQUIRE(String("4") == g_p4);
		REQUIRE(5 == g_p5);
		REQUIRE(6 == g_p6);
		REQUIRE(7 == g_p7);
	}
	SECTION("8 args no return")
	{
		Test test;
		Util::MemberFunction<Test, void, MyT1, MyT2, MyT3, MyT4, MyT5, MyT6, MyT7, MyT8> 
		         f(&test, &Test::fnr8, 1, True, 3, "4", 5, 6, 7, 8);

		RunNR(&f);
		REQUIRE(True == g_bRun);
		REQUIRE(1 == g_p1);
		REQUIRE(True == g_p2);
		REQUIRE(3 == g_p3);
		REQUIRE(String("4") == g_p4);
		REQUIRE(5 == g_p5);
		REQUIRE(6 == g_p6);
		REQUIRE(7 == g_p7);
		REQUIRE(8 == g_p8);
	}
	SECTION("9 args no return")
	{
		Test test;
		Util::MemberFunction<Test, void, MyT1, MyT2, MyT3, MyT4, MyT5, MyT6, MyT7, MyT8, MyT9>
		         f(&test, &Test::fnr9, 1, True, 3, "4", 5, 6, 7, 8, 9);

		RunNR(&f);
		REQUIRE(True == g_bRun);
		REQUIRE(1 == g_p1);
		REQUIRE(True == g_p2);
		REQUIRE(3 == g_p3);
		REQUIRE(String("4") == g_p4);
		REQUIRE(5 == g_p5);
		REQUIRE(6 == g_p6);
		REQUIRE(7 == g_p7);
		REQUIRE(8 == g_p8);
		REQUIRE(9 == g_p9);
	}
	SECTION("10 args no return")
	{
		Test test;
		Util::MemberFunction<Test, void, MyT1, MyT2, MyT3, MyT4, MyT5, MyT6, MyT7, MyT8, MyT9, MyT10>
		         f(&test, &Test::fnr10, 1, True, 3, "4", 5, 6, 7, 8, 9, 10);

		RunNR(&f);
		REQUIRE(True == g_bRun);
		REQUIRE(1 == g_p1);
		REQUIRE(True == g_p2);
		REQUIRE(3 == g_p3);
		REQUIRE(String("4") == g_p4);
		REQUIRE(5 == g_p5);
		REQUIRE(6 == g_p6);
		REQUIRE(7 == g_p7);
		REQUIRE(8 == g_p8);
		REQUIRE(9 == g_p9);
		REQUIRE(10 == g_p10);
	}
	SECTION("11 args no return")
	{
		Test test;
		Util::MemberFunction<Test, void, MyT1, MyT2, MyT3, MyT4, MyT5, MyT6, MyT7, MyT8, MyT9, MyT10, 
		                     MyT11>
		         f(&test, &Test::fnr11, 1, True, 3, "4", 5, 6, 7, 8, 9, 10, 11);

		RunNR(&f);
		REQUIRE(True == g_bRun);
		REQUIRE(1 == g_p1);
		REQUIRE(True == g_p2);
		REQUIRE(3 == g_p3);
		REQUIRE(String("4") == g_p4);
		REQUIRE(5 == g_p5);
		REQUIRE(6 == g_p6);
		REQUIRE(7 == g_p7);
		REQUIRE(8 == g_p8);
		REQUIRE(9 == g_p9);
		REQUIRE(10 == g_p10);
		REQUIRE(11 == g_p11);
	}
	SECTION("12 args no return")
	{
		Test test;
		Util::MemberFunction<Test, void, MyT1, MyT2, MyT3, MyT4, MyT5, MyT6, MyT7, MyT8, MyT9, MyT10, 
		                     MyT11, MyT12>
		         f(&test, &Test::fnr12, 1, True, 3, "4", 5, 6, 7, 8, 9, 10, 11, 12);

		RunNR(&f);
		REQUIRE(True == g_bRun);
		REQUIRE(1 == g_p1);
		REQUIRE(True == g_p2);
		REQUIRE(3 == g_p3);
		REQUIRE(String("4") == g_p4);
		REQUIRE(5 == g_p5);
		REQUIRE(6 == g_p6);
		REQUIRE(7 == g_p7);
		REQUIRE(8 == g_p8);
		REQUIRE(9 == g_p9);
		REQUIRE(10 == g_p10);
		REQUIRE(11 == g_p11);
		REQUIRE(12 == g_p12);
	}
	SECTION("13 args no return")
	{
		Test test;
		Util::MemberFunction<Test, void, MyT1, MyT2, MyT3, MyT4, MyT5, MyT6, MyT7, MyT8, MyT9, MyT10, 
		                     MyT11, MyT12, MyT13>
		         f(&test, &Test::fnr13, 1, True, 3, "4", 5, 6, 7, 8, 9, 10, 11, 12, 13);

		RunNR(&f);
		REQUIRE(True == g_bRun);
		REQUIRE(1 == g_p1);
		REQUIRE(True == g_p2);
		REQUIRE(3 == g_p3);
		REQUIRE(String("4") == g_p4);
		REQUIRE(5 == g_p5);
		REQUIRE(6 == g_p6);
		REQUIRE(7 == g_p7);
		REQUIRE(8 == g_p8);
		REQUIRE(9 == g_p9);
		REQUIRE(10 == g_p10);
		REQUIRE(11 == g_p11);
		REQUIRE(12 == g_p12);
		REQUIRE(13 == g_p13);
	}
	SECTION("14 args no return")
	{
		Test test;
		Util::MemberFunction<Test, void, MyT1, MyT2, MyT3, MyT4, MyT5, MyT6, MyT7, MyT8, MyT9, MyT10, 
		                     MyT11, MyT12, MyT13, MyT14>
		          f(&test, &Test::fnr14, 1, True, 3, "4", 5, 6, 7, 8, 9, 10, 11, 12, 13, 14);

		RunNR(&f);
		REQUIRE(True == g_bRun);
		REQUIRE(1 == g_p1);
		REQUIRE(True == g_p2);
		REQUIRE(3 == g_p3);
		REQUIRE(String("4") == g_p4);
		REQUIRE(5 == g_p5);
		REQUIRE(6 == g_p6);
		REQUIRE(7 == g_p7);
		REQUIRE(8 == g_p8);
		REQUIRE(9 == g_p9);
		REQUIRE(10 == g_p10);
		REQUIRE(11 == g_p11);
		REQUIRE(12 == g_p12);
		REQUIRE(13 == g_p13);
		REQUIRE(14 == g_p14);
	}
	SECTION("15 args no return")
	{
		Test test;
		Util::MemberFunction<Test, void, MyT1, MyT2, MyT3, MyT4, MyT5, MyT6, MyT7, MyT8, MyT9, MyT10, 
		                     MyT11, MyT12, MyT13, MyT14, MyT15>
		          f(&test, &Test::fnr15, 1, True, 3, "4", 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 'x');

		RunNR(&f);
		REQUIRE(True == g_bRun);
		REQUIRE(1 == g_p1);
		REQUIRE(True == g_p2);
		REQUIRE(3 == g_p3);
		REQUIRE(String("4") == g_p4);
		REQUIRE(5 == g_p5);
		REQUIRE(6 == g_p6);
		REQUIRE(7 == g_p7);
		REQUIRE(8 == g_p8);
		REQUIRE(9 == g_p9);
		REQUIRE(10 == g_p10);
		REQUIRE(11 == g_p11);
		REQUIRE(12 == g_p12);
		REQUIRE(13 == g_p13);
		REQUIRE(14 == g_p14);
		REQUIRE('x' == g_p15);
	}
	SECTION("16 args no return")
	{
		Test test;
		Util::MemberFunction<Test, void, MyT1, MyT2, MyT3, MyT4, MyT5, MyT6, MyT7, MyT8, MyT9, MyT10, 
		                     MyT11, MyT12, MyT13, MyT14, MyT15, MyT16>
		          f(&test, &Test::fnr16, 1, True, 3, "4", 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 'x', 
		            "16");

		RunNR(&f);
		REQUIRE(True == g_bRun);
		REQUIRE(1 == g_p1);
		REQUIRE(True == g_p2);
		REQUIRE(3 == g_p3);
		REQUIRE(String("4") == g_p4);
		REQUIRE(5 == g_p5);
		REQUIRE(6 == g_p6);
		REQUIRE(7 == g_p7);
		REQUIRE(8 == g_p8);
		REQUIRE(9 == g_p9);
		REQUIRE(10 == g_p10);
		REQUIRE(11 == g_p11);
		REQUIRE(12 == g_p12);
		REQUIRE(13 == g_p13);
		REQUIRE(14 == g_p14);
		REQUIRE('x' == g_p15);
		REQUIRE(String("16") == g_p16);
	}

	SECTION("0 args return")
	{
		Test test;
		Util::MemberFunction<Test, int> f(&test, &Test::fr0);

		REQUIRE(123 == RunR(&f));
		REQUIRE(True == g_bRun);
	}

	SECTION("1 arg with return")
	{
		Test test;
		Util::MemberFunction<Test, int, MyT1> f(&test, &Test::fr1, 1);

		REQUIRE(123 == RunR(&f));
		REQUIRE(True == g_bRun);
		REQUIRE(1 == g_p1);
	}
	SECTION("2 args with return")
	{
		Test test;
		Util::MemberFunction<Test, int, MyT1, MyT2> f(&test, &Test::fr2, 1, True);

		REQUIRE(123 == RunR(&f));
		REQUIRE(True == g_bRun);
		REQUIRE(1 == g_p1);
		REQUIRE(True == g_p2);
	}
	SECTION("3 args with return")
	{
		Test test;
		Util::MemberFunction<Test, int, MyT1, MyT2, MyT3> f(&test, &Test::fr3, 1, True, 3);

		REQUIRE(123 == RunR(&f));
		REQUIRE(True == g_bRun);
		REQUIRE(1 == g_p1);
		REQUIRE(True == g_p2);
		REQUIRE(3 == g_p3);
	}
	SECTION("4 args with return")
	{
		Test test;
		Util::MemberFunction<Test, int, MyT1, MyT2, MyT3, MyT4> f(&test, &Test::fr4, 1, True, 3, "4");

		REQUIRE(123 == RunR(&f));
		REQUIRE(True == g_bRun);
		REQUIRE(1 == g_p1);
		REQUIRE(True == g_p2);
		REQUIRE(3 == g_p3);
		REQUIRE(String("4") == g_p4);
	}
	SECTION("5 args with return")
	{
		Test test;
		Util::MemberFunction<Test, int, MyT1, MyT2, MyT3, MyT4, MyT5> 
		         f(&test, &Test::fr5, 1, True, 3, "4", 5);

		REQUIRE(123 == RunR(&f));
		REQUIRE(True == g_bRun);
		REQUIRE(1 == g_p1);
		REQUIRE(True == g_p2);
		REQUIRE(3 == g_p3);
		REQUIRE(String("4") == g_p4);
		REQUIRE(5 == g_p5);
	}
	SECTION("6 args with return")
	{
		Test test;
		Util::MemberFunction<Test, int, MyT1, MyT2, MyT3, MyT4, MyT5, MyT6> 
		         f(&test, &Test::fr6, 1, True, 3, "4", 5, 6);

		REQUIRE(123 == RunR(&f));
		REQUIRE(True == g_bRun);
		REQUIRE(1 == g_p1);
		REQUIRE(True == g_p2);
		REQUIRE(3 == g_p3);
		REQUIRE(String("4") == g_p4);
		REQUIRE(5 == g_p5);
		REQUIRE(6 == g_p6);
	}
	SECTION("7 args with return")
	{
		Test test;
		Util::MemberFunction<Test, int, MyT1, MyT2, MyT3, MyT4, MyT5, MyT6, MyT7> 
		         f(&test, &Test::fr7, 1, True, 3, "4", 5, 6, 7);

		REQUIRE(123 == RunR(&f));
		REQUIRE(True == g_bRun);
		REQUIRE(1 == g_p1);
		REQUIRE(True == g_p2);
		REQUIRE(3 == g_p3);
		REQUIRE(String("4") == g_p4);
		REQUIRE(5 == g_p5);
		REQUIRE(6 == g_p6);
		REQUIRE(7 == g_p7);
	}
	SECTION("8 args with return")
	{
		Test test;
		Util::MemberFunction<Test, int, MyT1, MyT2, MyT3, MyT4, MyT5, MyT6, MyT7, MyT8> 
		         f(&test, &Test::fr8, 1, True, 3, "4", 5, 6, 7, 8);

		REQUIRE(123 == RunR(&f));
		REQUIRE(True == g_bRun);
		REQUIRE(1 == g_p1);
		REQUIRE(True == g_p2);
		REQUIRE(3 == g_p3);
		REQUIRE(String("4") == g_p4);
		REQUIRE(5 == g_p5);
		REQUIRE(6 == g_p6);
		REQUIRE(7 == g_p7);
		REQUIRE(8 == g_p8);
	}
	SECTION("9 args with return")
	{
		Test test;
		Util::MemberFunction<Test, int, MyT1, MyT2, MyT3, MyT4, MyT5, MyT6, MyT7, MyT8, MyT9>
		         f(&test, &Test::fr9, 1, True, 3, "4", 5, 6, 7, 8, 9);

		REQUIRE(123 == RunR(&f));
		REQUIRE(True == g_bRun);
		REQUIRE(1 == g_p1);
		REQUIRE(True == g_p2);
		REQUIRE(3 == g_p3);
		REQUIRE(String("4") == g_p4);
		REQUIRE(5 == g_p5);
		REQUIRE(6 == g_p6);
		REQUIRE(7 == g_p7);
		REQUIRE(8 == g_p8);
		REQUIRE(9 == g_p9);
	}
	SECTION("10 args with return")
	{
		Test test;
		Util::MemberFunction<Test, int, MyT1, MyT2, MyT3, MyT4, MyT5, MyT6, MyT7, MyT8, MyT9, MyT10>
		         f(&test, &Test::fr10, 1, True, 3, "4", 5, 6, 7, 8, 9, 10);

		REQUIRE(123 == RunR(&f));
		REQUIRE(True == g_bRun);
		REQUIRE(1 == g_p1);
		REQUIRE(True == g_p2);
		REQUIRE(3 == g_p3);
		REQUIRE(String("4") == g_p4);
		REQUIRE(5 == g_p5);
		REQUIRE(6 == g_p6);
		REQUIRE(7 == g_p7);
		REQUIRE(8 == g_p8);
		REQUIRE(9 == g_p9);
		REQUIRE(10 == g_p10);
	}
	SECTION("11 args with return")
	{
		Test test;
		Util::MemberFunction<Test, int, MyT1, MyT2, MyT3, MyT4, MyT5, MyT6, MyT7, MyT8, MyT9, MyT10, 
		                     MyT11>
		         f(&test, &Test::fr11, 1, True, 3, "4", 5, 6, 7, 8, 9, 10, 11);

		REQUIRE(123 == RunR(&f));
		REQUIRE(True == g_bRun);
		REQUIRE(1 == g_p1);
		REQUIRE(True == g_p2);
		REQUIRE(3 == g_p3);
		REQUIRE(String("4") == g_p4);
		REQUIRE(5 == g_p5);
		REQUIRE(6 == g_p6);
		REQUIRE(7 == g_p7);
		REQUIRE(8 == g_p8);
		REQUIRE(9 == g_p9);
		REQUIRE(10 == g_p10);
		REQUIRE(11 == g_p11);
	}
	SECTION("12 args with return")
	{
		Test test;
		Util::MemberFunction<Test, int, MyT1, MyT2, MyT3, MyT4, MyT5, MyT6, MyT7, MyT8, MyT9, MyT10, 
		                     MyT11, MyT12>
		         f(&test, &Test::fr12, 1, True, 3, "4", 5, 6, 7, 8, 9, 10, 11, 12);

		REQUIRE(123 == RunR(&f));
		REQUIRE(True == g_bRun);
		REQUIRE(1 == g_p1);
		REQUIRE(True == g_p2);
		REQUIRE(3 == g_p3);
		REQUIRE(String("4") == g_p4);
		REQUIRE(5 == g_p5);
		REQUIRE(6 == g_p6);
		REQUIRE(7 == g_p7);
		REQUIRE(8 == g_p8);
		REQUIRE(9 == g_p9);
		REQUIRE(10 == g_p10);
		REQUIRE(11 == g_p11);
		REQUIRE(12 == g_p12);
	}
	SECTION("13 args with return")
	{
		Test test;
		Util::MemberFunction<Test, int, MyT1, MyT2, MyT3, MyT4, MyT5, MyT6, MyT7, MyT8, MyT9, MyT10, 
		                     MyT11, MyT12, MyT13>
		         f(&test, &Test::fr13, 1, True, 3, "4", 5, 6, 7, 8, 9, 10, 11, 12, 13);

		REQUIRE(123 == RunR(&f));
		REQUIRE(True == g_bRun);
		REQUIRE(1 == g_p1);
		REQUIRE(True == g_p2);
		REQUIRE(3 == g_p3);
		REQUIRE(String("4") == g_p4);
		REQUIRE(5 == g_p5);
		REQUIRE(6 == g_p6);
		REQUIRE(7 == g_p7);
		REQUIRE(8 == g_p8);
		REQUIRE(9 == g_p9);
		REQUIRE(10 == g_p10);
		REQUIRE(11 == g_p11);
		REQUIRE(12 == g_p12);
		REQUIRE(13 == g_p13);
	}
	SECTION("14 args with return")
	{
		Test test;
		Util::MemberFunction<Test, int, MyT1, MyT2, MyT3, MyT4, MyT5, MyT6, MyT7, MyT8, MyT9, MyT10, 
		                     MyT11, MyT12, MyT13, MyT14>
		          f(&test, &Test::fr14, 1, True, 3, "4", 5, 6, 7, 8, 9, 10, 11, 12, 13, 14);

		REQUIRE(123 == RunR(&f));
		REQUIRE(True == g_bRun);
		REQUIRE(1 == g_p1);
		REQUIRE(True == g_p2);
		REQUIRE(3 == g_p3);
		REQUIRE(String("4") == g_p4);
		REQUIRE(5 == g_p5);
		REQUIRE(6 == g_p6);
		REQUIRE(7 == g_p7);
		REQUIRE(8 == g_p8);
		REQUIRE(9 == g_p9);
		REQUIRE(10 == g_p10);
		REQUIRE(11 == g_p11);
		REQUIRE(12 == g_p12);
		REQUIRE(13 == g_p13);
		REQUIRE(14 == g_p14);
	}
	SECTION("15 args with return")
	{
		Test test;
		Util::MemberFunction<Test, int, MyT1, MyT2, MyT3, MyT4, MyT5, MyT6, MyT7, MyT8, MyT9, MyT10, 
		                     MyT11, MyT12, MyT13, MyT14, MyT15>
		          f(&test, &Test::fr15, 1, True, 3, "4", 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 'x');

		REQUIRE(123 == RunR(&f));
		REQUIRE(True == g_bRun);
		REQUIRE(1 == g_p1);
		REQUIRE(True == g_p2);
		REQUIRE(3 == g_p3);
		REQUIRE(String("4") == g_p4);
		REQUIRE(5 == g_p5);
		REQUIRE(6 == g_p6);
		REQUIRE(7 == g_p7);
		REQUIRE(8 == g_p8);
		REQUIRE(9 == g_p9);
		REQUIRE(10 == g_p10);
		REQUIRE(11 == g_p11);
		REQUIRE(12 == g_p12);
		REQUIRE(13 == g_p13);
		REQUIRE(14 == g_p14);
		REQUIRE('x' == g_p15);
	}
	SECTION("16 args with return")
	{
		Test test;
		Util::MemberFunction<Test, int, MyT1, MyT2, MyT3, MyT4, MyT5, MyT6, MyT7, MyT8, MyT9, MyT10, 
		                     MyT11, MyT12, MyT13, MyT14, MyT15, MyT16>
		          f(&test, &Test::fr16, 1, True, 3, "4", 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 'x', 
		            "16");

		REQUIRE(123 == RunR(&f));
		REQUIRE(True == g_bRun);
		REQUIRE(1 == g_p1);
		REQUIRE(True == g_p2);
		REQUIRE(3 == g_p3);
		REQUIRE(String("4") == g_p4);
		REQUIRE(5 == g_p5);
		REQUIRE(6 == g_p6);
		REQUIRE(7 == g_p7);
		REQUIRE(8 == g_p8);
		REQUIRE(9 == g_p9);
		REQUIRE(10 == g_p10);
		REQUIRE(11 == g_p11);
		REQUIRE(12 == g_p12);
		REQUIRE(13 == g_p13);
		REQUIRE(14 == g_p14);
		REQUIRE('x' == g_p15);
		REQUIRE(String("16") == g_p16);
	}
}

}//namespace Util_Function


