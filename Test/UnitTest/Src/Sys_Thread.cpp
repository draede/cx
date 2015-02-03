
#include "CX/CX.h"
#include "CX/Sys/Thread.h"

#pragma warning(push)
#pragma warning(disable: 4702)
#include "../../../Contrib/Catch/Include/catch.hpp"
#pragma warning(pop)


using namespace CX;


namespace Sys_Thread
{

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

void fnr3(MyT1 p1, MyT2 p2, MyT3 p3)
{
	g_bRun = true;
	g_p1   = p1;
	g_p2   = p2;
	g_p3   = p3;
}

void fnr4(MyT1 p1, MyT2 p2, MyT3 p3, MyT4 p4)
{
	g_bRun = true;
	g_p1   = p1;
	g_p2   = p2;
	g_p3   = p3;
	g_p4   = p4;
}

void fnr5(MyT1 p1, MyT2 p2, MyT3 p3, MyT4 p4, MyT5 p5)
{
	g_bRun = true;
	g_p1   = p1;
	g_p2   = p2;
	g_p3   = p3;
	g_p4   = p4;
	g_p5   = p5;
}

void fnr6(MyT1 p1, MyT2 p2, MyT3 p3, MyT4 p4, MyT5 p5, MyT6 p6)
{
	g_bRun = true;
	g_p1   = p1;
	g_p2   = p2;
	g_p3   = p3;
	g_p4   = p4;
	g_p5   = p5;
	g_p6   = p6;
}

void fnr7(MyT1 p1, MyT2 p2, MyT3 p3, MyT4 p4, MyT5 p5, MyT6 p6, MyT7 p7)
{
	g_bRun = true;
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
	g_bRun = true;
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
	g_bRun = true;
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
	g_bRun = true;
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
	g_bRun = true;
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
	g_bRun = true;
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
	g_bRun = true;
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
	g_bRun = true;
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
	g_bRun = true;
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
	g_bRun = true;
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
	g_bRun = true;

	return 123;
}

int fr1(int p1)
{
	g_bRun = true;
	g_p1   = p1;

	return 123;
}

int fr2(MyT1 p1, MyT2 p2)
{
	g_bRun = true;
	g_p1   = p1;
	g_p2   = p2;

	return 123;
}

int fr3(MyT1 p1, MyT2 p2, MyT3 p3)
{
	g_bRun = true;
	g_p1   = p1;
	g_p2   = p2;
	g_p3   = p3;

	return 123;
}

int fr4(MyT1 p1, MyT2 p2, MyT3 p3, MyT4 p4)
{
	g_bRun = true;
	g_p1   = p1;
	g_p2   = p2;
	g_p3   = p3;
	g_p4   = p4;

	return 123;
}

int fr5(MyT1 p1, MyT2 p2, MyT3 p3, MyT4 p4, MyT5 p5)
{
	g_bRun = true;
	g_p1   = p1;
	g_p2   = p2;
	g_p3   = p3;
	g_p4   = p4;
	g_p5   = p5;

	return 123;
}

int fr6(MyT1 p1, MyT2 p2, MyT3 p3, MyT4 p4, MyT5 p5, MyT6 p6)
{
	g_bRun = true;
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
	g_bRun = true;
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
	g_bRun = true;
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
	g_bRun = true;
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
	g_bRun = true;
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
	g_bRun = true;
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
	g_bRun = true;
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
	g_bRun = true;
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
	g_bRun = true;
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
	g_bRun = true;
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
	g_bRun = true;
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

	void fnr3(MyT1 p1, MyT2 p2, MyT3 p3)
	{
		g_bRun = true;
		g_p1   = p1;
		g_p2   = p2;
		g_p3   = p3;
	}

	void fnr4(MyT1 p1, MyT2 p2, MyT3 p3, MyT4 p4)
	{
		g_bRun = true;
		g_p1   = p1;
		g_p2   = p2;
		g_p3   = p3;
		g_p4   = p4;
	}

	void fnr5(MyT1 p1, MyT2 p2, MyT3 p3, MyT4 p4, MyT5 p5)
	{
		g_bRun = true;
		g_p1   = p1;
		g_p2   = p2;
		g_p3   = p3;
		g_p4   = p4;
		g_p5   = p5;
	}

	void fnr6(MyT1 p1, MyT2 p2, MyT3 p3, MyT4 p4, MyT5 p5, MyT6 p6)
	{
		g_bRun = true;
		g_p1   = p1;
		g_p2   = p2;
		g_p3   = p3;
		g_p4   = p4;
		g_p5   = p5;
		g_p6   = p6;
	}

	void fnr7(MyT1 p1, MyT2 p2, MyT3 p3, MyT4 p4, MyT5 p5, MyT6 p6, MyT7 p7)
	{
		g_bRun = true;
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
		g_bRun = true;
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
		g_bRun = true;
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
		g_bRun = true;
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
		g_bRun = true;
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
		g_bRun = true;
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
		g_bRun = true;
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
		g_bRun = true;
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
		g_bRun = true;
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
		g_bRun = true;
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
		g_bRun = true;

		return 123;
	}

	int fr1(int p1)
	{
		g_bRun = true;
		g_p1   = p1;

		return 123;
	}

	int fr2(MyT1 p1, MyT2 p2)
	{
		g_bRun = true;
		g_p1   = p1;
		g_p2   = p2;

		return 123;
	}

	int fr3(MyT1 p1, MyT2 p2, MyT3 p3)
	{
		g_bRun = true;
		g_p1   = p1;
		g_p2   = p2;
		g_p3   = p3;

		return 123;
	}

	int fr4(MyT1 p1, MyT2 p2, MyT3 p3, MyT4 p4)
	{
		g_bRun = true;
		g_p1   = p1;
		g_p2   = p2;
		g_p3   = p3;
		g_p4   = p4;

		return 123;
	}

	int fr5(MyT1 p1, MyT2 p2, MyT3 p3, MyT4 p4, MyT5 p5)
	{
		g_bRun = true;
		g_p1   = p1;
		g_p2   = p2;
		g_p3   = p3;
		g_p4   = p4;
		g_p5   = p5;

		return 123;
	}

	int fr6(MyT1 p1, MyT2 p2, MyT3 p3, MyT4 p4, MyT5 p5, MyT6 p6)
	{
		g_bRun = true;
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
		g_bRun = true;
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
		g_bRun = true;
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
		g_bRun = true;
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
		g_bRun = true;
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
		g_bRun = true;
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
		g_bRun = true;
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
		g_bRun = true;
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
		g_bRun = true;
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
		g_bRun = true;
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
		g_bRun = true;
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

TEST_CASE("Thread Function tests", "[CX::Sys::Thread : Function]")
{
	SECTION("0 args no return")
	{
		Sys::Thread thread;

		g_bRun = false;
		thread.Run(&fnr0);
		thread.Wait();
		REQUIRE(true == g_bRun);
	}
	SECTION("1 arg no return")
	{
		Sys::Thread thread;

		g_bRun = false;
		thread.Run(&fnr1, 1);
		thread.Wait();
		REQUIRE(true == g_bRun);
		REQUIRE(1 == g_p1);
	}
	SECTION("2 args no return")
	{
		Sys::Thread thread;

		g_bRun = false;
		thread.Run(&fnr2, 1, true);
		thread.Wait();
		REQUIRE(true == g_bRun);
		REQUIRE(1 == g_p1);
		REQUIRE(true == g_p2);
	}
	SECTION("3 args no return")
	{
		Sys::Thread thread;

		g_bRun = false;
		thread.Run(&fnr3, 1, true, (MyT3)3);
		thread.Wait();
		REQUIRE(true == g_bRun);
		REQUIRE(1 == g_p1);
		REQUIRE(true == g_p2);
		REQUIRE(3 == g_p3);
	}
	SECTION("4 args no return")
	{
		Sys::Thread thread;

		g_bRun = false;
		thread.Run(&fnr4, 1, true, (MyT3)3, (MyT4)"4");
		thread.Wait();
		REQUIRE(true == g_bRun);
		REQUIRE(1 == g_p1);
		REQUIRE(true == g_p2);
		REQUIRE(3 == g_p3);
		REQUIRE(String("4") == g_p4);
	}
	SECTION("5 args no return")
	{
		Sys::Thread thread;

		g_bRun = false;
		thread.Run(&fnr5, 1, true, (MyT3)3, (MyT4)"4", (MyT5)5);
		thread.Wait();
		REQUIRE(true == g_bRun);
		REQUIRE(1 == g_p1);
		REQUIRE(true == g_p2);
		REQUIRE(3 == g_p3);
		REQUIRE(String("4") == g_p4);
		REQUIRE(5 == g_p5);
	}
	SECTION("6 args no return")
	{
		Sys::Thread thread;

		g_bRun = false;
		thread.Run(&fnr6, 1, true, (MyT3)3, (MyT4)"4", (MyT5)5, (MyT6)6);
		thread.Wait();
		REQUIRE(true == g_bRun);
		REQUIRE(1 == g_p1);
		REQUIRE(true == g_p2);
		REQUIRE(3 == g_p3);
		REQUIRE(String("4") == g_p4);
		REQUIRE(5 == g_p5);
		REQUIRE(6 == g_p6);
	}
	SECTION("7 args no return")
	{
		Sys::Thread thread;

		g_bRun = false;
		thread.Run(&fnr7, 1, true, (MyT3)3, (MyT4)"4", (MyT5)5, (MyT6)6, (MyT7)7);
		thread.Wait();
		REQUIRE(true == g_bRun);
		REQUIRE(1 == g_p1);
		REQUIRE(true == g_p2);
		REQUIRE(3 == g_p3);
		REQUIRE(String("4") == g_p4);
		REQUIRE(5 == g_p5);
		REQUIRE(6 == g_p6);
		REQUIRE(7 == g_p7);
	}
	SECTION("8 args no return")
	{
		Sys::Thread thread;

		g_bRun = false;
		thread.Run(&fnr8, 1, true, (MyT3)3, (MyT4)"4", (MyT5)5, (MyT6)6, (MyT7)7, (MyT8)8);
		thread.Wait();
		REQUIRE(true == g_bRun);
		REQUIRE(1 == g_p1);
		REQUIRE(true == g_p2);
		REQUIRE(3 == g_p3);
		REQUIRE(String("4") == g_p4);
		REQUIRE(5 == g_p5);
		REQUIRE(6 == g_p6);
		REQUIRE(7 == g_p7);
		REQUIRE(8 == g_p8);
	}
	SECTION("9 args no return")
	{
		Sys::Thread thread;

		g_bRun = false;
		thread.Run(&fnr9, 1, true, (MyT3)3, (MyT4)"4", (MyT5)5, (MyT6)6, (MyT7)7, (MyT8)8, (MyT9)9);
		thread.Wait();
		REQUIRE(true == g_bRun);
		REQUIRE(1 == g_p1);
		REQUIRE(true == g_p2);
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
		Sys::Thread thread;

		g_bRun = false;
		thread.Run(&fnr10, 1, true, (MyT3)3, (MyT4)"4", (MyT5)5, (MyT6)6, (MyT7)7, (MyT8)8, (MyT9)9, 
		           (MyT10)10);
		thread.Wait();
		REQUIRE(true == g_bRun);
		REQUIRE(1 == g_p1);
		REQUIRE(true == g_p2);
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
		Sys::Thread thread;

		g_bRun = false;
		thread.Run(&fnr11, 1, true, (MyT3)3, (MyT4)"4", (MyT5)5, (MyT6)6, (MyT7)7, (MyT8)8, (MyT9)9, 
		           (MyT10)10, (MyT11)11);
		thread.Wait();
		REQUIRE(true == g_bRun);
		REQUIRE(1 == g_p1);
		REQUIRE(true == g_p2);
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
		Sys::Thread thread;

		g_bRun = false;
		thread.Run(&fnr12, 1, true, (MyT3)3, (MyT4)"4", (MyT5)5, (MyT6)6, (MyT7)7, (MyT8)8, (MyT9)9, 
		           (MyT10)10, (MyT11)11, (MyT12)12);
		thread.Wait();
		REQUIRE(true == g_bRun);
		REQUIRE(1 == g_p1);
		REQUIRE(true == g_p2);
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
		Sys::Thread thread;

		g_bRun = false;
		thread.Run(&fnr13, 1, true, (MyT3)3, (MyT4)"4", (MyT5)5, (MyT6)6, (MyT7)7, (MyT8)8, (MyT9)9, 
		           (MyT10)10, (MyT11)11, (MyT12)12, (MyT13)13);
		thread.Wait();
		REQUIRE(true == g_bRun);
		REQUIRE(1 == g_p1);
		REQUIRE(true == g_p2);
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
		Sys::Thread thread;

		g_bRun = false;
		thread.Run(&fnr14, 1, true, (MyT3)3, (MyT4)"4", (MyT5)5, (MyT6)6, (MyT7)7, (MyT8)8, (MyT9)9, 
		           (MyT10)10, (MyT11)11, (MyT12)12, (MyT13)13, (MyT14)14);
		thread.Wait();
		REQUIRE(true == g_bRun);
		REQUIRE(1 == g_p1);
		REQUIRE(true == g_p2);
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
		Sys::Thread thread;

		g_bRun = false;
		thread.Run(&fnr15, 1, true, (MyT3)3, (MyT4)"4", (MyT5)5, (MyT6)6, (MyT7)7, (MyT8)8, (MyT9)9, 
		           (MyT10)10, (MyT11)11, (MyT12)12, (MyT13)13, (MyT14)14, (MyT15)'x');
		thread.Wait();
		REQUIRE(true == g_bRun);
		REQUIRE(1 == g_p1);
		REQUIRE(true == g_p2);
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
		Sys::Thread thread;

		g_bRun = false;
		thread.Run(&fnr16, 1, true, (MyT3)3, (MyT4)"4", (MyT5)5, (MyT6)6, (MyT7)7, (MyT8)8, (MyT9)9, 
		           (MyT10)10, (MyT11)11, (MyT12)12, (MyT13)13, (MyT14)14, (MyT15)'x', (MyT16)"16");
		thread.Wait();
		REQUIRE(true == g_bRun);
		REQUIRE(1 == g_p1);
		REQUIRE(true == g_p2);
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
		Sys::Thread thread;

		g_bRun = false;
		thread.Run(&fr0);
		thread.Wait();
		REQUIRE(true == g_bRun);
	}

	SECTION("1 arg with return")
	{
		Sys::Thread thread;

		g_bRun = false;
		thread.Run(&fr1, 1);
		thread.Wait();
		REQUIRE(true == g_bRun);
		REQUIRE(1 == g_p1);
	}
	SECTION("2 args with return")
	{
		Sys::Thread thread;

		g_bRun = false;
		thread.Run(&fr2, 1, true);
		thread.Wait();
		REQUIRE(true == g_bRun);
		REQUIRE(1 == g_p1);
		REQUIRE(true == g_p2);
	}
	SECTION("3 args with return")
	{
		Sys::Thread thread;

		g_bRun = false;
		thread.Run(&fr3, 1, true, (MyT3)3);
		thread.Wait();
		REQUIRE(true == g_bRun);
		REQUIRE(1 == g_p1);
		REQUIRE(true == g_p2);
		REQUIRE(3 == g_p3);
	}
	SECTION("4 args with return")
	{
		Sys::Thread thread;

		g_bRun = false;
		thread.Run(&fr4, 1, true, (MyT3)3, (MyT4)"4");
		thread.Wait();
		REQUIRE(true == g_bRun);
		REQUIRE(1 == g_p1);
		REQUIRE(true == g_p2);
		REQUIRE(3 == g_p3);
		REQUIRE(String("4") == g_p4);
	}
	SECTION("5 args with return")
	{
		Sys::Thread thread;

		g_bRun = false;
		thread.Run(&fr5, 1, true, (MyT3)3, (MyT4)"4", (MyT5)5);
		thread.Wait();
		REQUIRE(true == g_bRun);
		REQUIRE(1 == g_p1);
		REQUIRE(true == g_p2);
		REQUIRE(3 == g_p3);
		REQUIRE(String("4") == g_p4);
		REQUIRE(5 == g_p5);
	}
	SECTION("6 args with return")
	{
		Sys::Thread thread;

		g_bRun = false;
		thread.Run(&fr6, 1, true, (MyT3)3, (MyT4)"4", (MyT5)5, (MyT6)6);
		thread.Wait();
		REQUIRE(true == g_bRun);
		REQUIRE(1 == g_p1);
		REQUIRE(true == g_p2);
		REQUIRE(3 == g_p3);
		REQUIRE(String("4") == g_p4);
		REQUIRE(5 == g_p5);
		REQUIRE(6 == g_p6);
	}
	SECTION("7 args with return")
	{
		Sys::Thread thread;

		g_bRun = false;
		thread.Run(&fr7, 1, true, (MyT3)3, (MyT4)"4", (MyT5)5, (MyT6)6, (MyT7)7);
		thread.Wait();
		REQUIRE(true == g_bRun);
		REQUIRE(1 == g_p1);
		REQUIRE(true == g_p2);
		REQUIRE(3 == g_p3);
		REQUIRE(String("4") == g_p4);
		REQUIRE(5 == g_p5);
		REQUIRE(6 == g_p6);
		REQUIRE(7 == g_p7);
	}
	SECTION("8 args with return")
	{
		Sys::Thread thread;

		g_bRun = false;
		thread.Run(&fr8, 1, true, (MyT3)3, (MyT4)"4", (MyT5)5, (MyT6)6, (MyT7)7, (MyT8)8);
		thread.Wait();
		REQUIRE(true == g_bRun);
		REQUIRE(1 == g_p1);
		REQUIRE(true == g_p2);
		REQUIRE(3 == g_p3);
		REQUIRE(String("4") == g_p4);
		REQUIRE(5 == g_p5);
		REQUIRE(6 == g_p6);
		REQUIRE(7 == g_p7);
		REQUIRE(8 == g_p8);
	}
	SECTION("9 args with return")
	{
		Sys::Thread thread;

		g_bRun = false;
		thread.Run(&fr9, 1, true, (MyT3)3, (MyT4)"4", (MyT5)5, (MyT6)6, (MyT7)7, (MyT8)8, (MyT9)9);
		thread.Wait();
		REQUIRE(true == g_bRun);
		REQUIRE(1 == g_p1);
		REQUIRE(true == g_p2);
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
		Sys::Thread thread;

		g_bRun = false;
		thread.Run(&fr10, 1, true, (MyT3)3, (MyT4)"4", (MyT5)5, (MyT6)6, (MyT7)7, (MyT8)8, (MyT9)9, 
		           (MyT10)10);
		thread.Wait();
		REQUIRE(true == g_bRun);
		REQUIRE(1 == g_p1);
		REQUIRE(true == g_p2);
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
		Sys::Thread thread;

		g_bRun = false;
		thread.Run(&fr11, 1, true, (MyT3)3, (MyT4)"4", (MyT5)5, (MyT6)6, (MyT7)7, (MyT8)8, (MyT9)9, 
		           (MyT10)10, (MyT11)11);
		thread.Wait();
		REQUIRE(true == g_bRun);
		REQUIRE(1 == g_p1);
		REQUIRE(true == g_p2);
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
		Sys::Thread thread;

		g_bRun = false;
		thread.Run(&fr12, 1, true, (MyT3)3, (MyT4)"4", (MyT5)5, (MyT6)6, (MyT7)7, (MyT8)8, (MyT9)9, 
		           (MyT10)10, (MyT11)11, (MyT12)12);
		thread.Wait();
		REQUIRE(true == g_bRun);
		REQUIRE(1 == g_p1);
		REQUIRE(true == g_p2);
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
		Sys::Thread thread;

		g_bRun = false;
		thread.Run(&fr13, 1, true, (MyT3)3, (MyT4)"4", (MyT5)5, (MyT6)6, (MyT7)7, (MyT8)8, (MyT9)9, 
		           (MyT10)10, (MyT11)11, (MyT12)12, (MyT13)13);
		thread.Wait();
		REQUIRE(true == g_bRun);
		REQUIRE(1 == g_p1);
		REQUIRE(true == g_p2);
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
		Sys::Thread thread;

		g_bRun = false;
		thread.Run(&fr14, 1, true, (MyT3)3, (MyT4)"4", (MyT5)5, (MyT6)6, (MyT7)7, (MyT8)8, (MyT9)9, 
		           (MyT10)10, (MyT11)11, (MyT12)12, (MyT13)13, (MyT14)14);
		thread.Wait();
		REQUIRE(true == g_bRun);
		REQUIRE(1 == g_p1);
		REQUIRE(true == g_p2);
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
		Sys::Thread thread;

		g_bRun = false;
		thread.Run(&fr15, 1, true, (MyT3)3, (MyT4)"4", (MyT5)5, (MyT6)6, (MyT7)7, (MyT8)8, (MyT9)9, 
		           (MyT10)10, (MyT11)11, (MyT12)12, (MyT13)13, (MyT14)14, (MyT15)'x');
		thread.Wait();
		REQUIRE(true == g_bRun);
		REQUIRE(1 == g_p1);
		REQUIRE(true == g_p2);
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
		Sys::Thread thread;

		g_bRun = false;
		thread.Run(&fr16, 1, true, (MyT3)3, (MyT4)"4", (MyT5)5, (MyT6)6, (MyT7)7, (MyT8)8, (MyT9)9, 
		           (MyT10)10, (MyT11)11, (MyT12)12, (MyT13)13, (MyT14)14, (MyT15)'x', (MyT16)"16");
		thread.Wait();
		REQUIRE(true == g_bRun);
		REQUIRE(1 == g_p1);
		REQUIRE(true == g_p2);
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
TEST_CASE("Thread MemberFunction tests", "[CX::Sys::Thread : MemberFunction]")
{
	SECTION("0 args no return")
	{
		Sys::Thread thread;
		Test test;

		g_bRun = false;
		thread.Run(&test, &Test::fnr0);
		thread.Wait();
		REQUIRE(true == g_bRun);
	}
	SECTION("1 arg no return")
	{
		Sys::Thread thread;
		Test test;

		g_bRun = false;
		thread.Run(&test, &Test::fnr1, 1);
		thread.Wait();
		REQUIRE(true == g_bRun);
		REQUIRE(1 == g_p1);
	}
	SECTION("2 args no return")
	{
		Sys::Thread thread;
		Test test;

		g_bRun = false;
		thread.Run(&test, &Test::fnr2, 1, true);
		thread.Wait();
		REQUIRE(true == g_bRun);
		REQUIRE(1 == g_p1);
		REQUIRE(true == g_p2);
	}
	SECTION("3 args no return")
	{
		Sys::Thread thread;
		Test test;

		g_bRun = false;
		thread.Run(&test, &Test::fnr3, 1, true, (MyT3)3);
		thread.Wait();
		REQUIRE(true == g_bRun);
		REQUIRE(1 == g_p1);
		REQUIRE(true == g_p2);
		REQUIRE(3 == g_p3);
	}
	SECTION("4 args no return")
	{
		Sys::Thread thread;
		Test test;

		g_bRun = false;
		thread.Run(&test, &Test::fnr4, 1, true, (MyT3)3, (MyT4)"4");
		thread.Wait();
		REQUIRE(true == g_bRun);
		REQUIRE(1 == g_p1);
		REQUIRE(true == g_p2);
		REQUIRE(3 == g_p3);
		REQUIRE(String("4") == g_p4);
	}
	SECTION("5 args no return")
	{
		Sys::Thread thread;
		Test test;

		g_bRun = false;
		thread.Run(&test, &Test::fnr5, 1, true, (MyT3)3, (MyT4)"4", (MyT5)5);
		thread.Wait();
		REQUIRE(true == g_bRun);
		REQUIRE(1 == g_p1);
		REQUIRE(true == g_p2);
		REQUIRE(3 == g_p3);
		REQUIRE(String("4") == g_p4);
		REQUIRE(5 == g_p5);
	}
	SECTION("6 args no return")
	{
		Sys::Thread thread;
		Test test;

		g_bRun = false;
		thread.Run(&test, &Test::fnr6, 1, true, (MyT3)3, (MyT4)"4", (MyT5)5, (MyT6)6);
		thread.Wait();
		REQUIRE(true == g_bRun);
		REQUIRE(1 == g_p1);
		REQUIRE(true == g_p2);
		REQUIRE(3 == g_p3);
		REQUIRE(String("4") == g_p4);
		REQUIRE(5 == g_p5);
		REQUIRE(6 == g_p6);
	}
	SECTION("7 args no return")
	{
		Sys::Thread thread;
		Test test;

		g_bRun = false;
		thread.Run(&test, &Test::fnr7, 1, true, (MyT3)3, (MyT4)"4", (MyT5)5, (MyT6)6, (MyT7)7);
		thread.Wait();
		REQUIRE(true == g_bRun);
		REQUIRE(1 == g_p1);
		REQUIRE(true == g_p2);
		REQUIRE(3 == g_p3);
		REQUIRE(String("4") == g_p4);
		REQUIRE(5 == g_p5);
		REQUIRE(6 == g_p6);
		REQUIRE(7 == g_p7);
	}
	SECTION("8 args no return")
	{
		Sys::Thread thread;
		Test test;

		g_bRun = false;
		thread.Run(&test, &Test::fnr8, 1, true, (MyT3)3, (MyT4)"4", (MyT5)5, (MyT6)6, (MyT7)7, (MyT8)8);
		thread.Wait();
		REQUIRE(true == g_bRun);
		REQUIRE(1 == g_p1);
		REQUIRE(true == g_p2);
		REQUIRE(3 == g_p3);
		REQUIRE(String("4") == g_p4);
		REQUIRE(5 == g_p5);
		REQUIRE(6 == g_p6);
		REQUIRE(7 == g_p7);
		REQUIRE(8 == g_p8);
	}
	SECTION("9 args no return")
	{
		Sys::Thread thread;
		Test test;

		g_bRun = false;
		thread.Run(&test, &Test::fnr9, 1, true, (MyT3)3, (MyT4)"4", (MyT5)5, (MyT6)6, (MyT7)7, (MyT8)8, (MyT9)9);
		thread.Wait();
		REQUIRE(true == g_bRun);
		REQUIRE(1 == g_p1);
		REQUIRE(true == g_p2);
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
		Sys::Thread thread;
		Test test;

		g_bRun = false;
		thread.Run(&test, &Test::fnr10, 1, true, (MyT3)3, (MyT4)"4", (MyT5)5, (MyT6)6, (MyT7)7, (MyT8)8, (MyT9)9, 
		           (MyT10)10);
		thread.Wait();
		REQUIRE(true == g_bRun);
		REQUIRE(1 == g_p1);
		REQUIRE(true == g_p2);
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
		Sys::Thread thread;
		Test test;

		g_bRun = false;
		thread.Run(&test, &Test::fnr11, 1, true, (MyT3)3, (MyT4)"4", (MyT5)5, (MyT6)6, (MyT7)7, (MyT8)8, (MyT9)9, 
		           (MyT10)10, (MyT11)11);
		thread.Wait();
		REQUIRE(true == g_bRun);
		REQUIRE(1 == g_p1);
		REQUIRE(true == g_p2);
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
		Sys::Thread thread;
		Test test;

		g_bRun = false;
		thread.Run(&test, &Test::fnr12, 1, true, (MyT3)3, (MyT4)"4", (MyT5)5, (MyT6)6, (MyT7)7, (MyT8)8, (MyT9)9, 
		           (MyT10)10, (MyT11)11, (MyT12)12);
		thread.Wait();
		REQUIRE(true == g_bRun);
		REQUIRE(1 == g_p1);
		REQUIRE(true == g_p2);
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
		Sys::Thread thread;
		Test test;

		g_bRun = false;
		thread.Run(&test, &Test::fnr13, 1, true, (MyT3)3, (MyT4)"4", (MyT5)5, (MyT6)6, (MyT7)7, (MyT8)8, (MyT9)9, 
		           (MyT10)10, (MyT11)11, (MyT12)12, (MyT13)13);
		thread.Wait();
		REQUIRE(true == g_bRun);
		REQUIRE(1 == g_p1);
		REQUIRE(true == g_p2);
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
		Sys::Thread thread;
		Test test;

		g_bRun = false;
		thread.Run(&test, &Test::fnr14, 1, true, (MyT3)3, (MyT4)"4", (MyT5)5, (MyT6)6, (MyT7)7, (MyT8)8, (MyT9)9, 
		           (MyT10)10, (MyT11)11, (MyT12)12, (MyT13)13, (MyT14)14);
		thread.Wait();
		REQUIRE(true == g_bRun);
		REQUIRE(1 == g_p1);
		REQUIRE(true == g_p2);
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
		Sys::Thread thread;
		Test test;

		g_bRun = false;
		thread.Run(&test, &Test::fnr15, 1, true, (MyT3)3, (MyT4)"4", (MyT5)5, (MyT6)6, (MyT7)7, (MyT8)8, (MyT9)9, 
		           (MyT10)10, (MyT11)11, (MyT12)12, (MyT13)13, (MyT14)14, (MyT15)'x');
		thread.Wait();
		REQUIRE(true == g_bRun);
		REQUIRE(1 == g_p1);
		REQUIRE(true == g_p2);
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
		Sys::Thread thread;
		Test test;

		g_bRun = false;
		thread.Run(&test, &Test::fnr16, 1, true, (MyT3)3, (MyT4)"4", (MyT5)5, (MyT6)6, (MyT7)7, (MyT8)8, (MyT9)9, 
		           (MyT10)10, (MyT11)11, (MyT12)12, (MyT13)13, (MyT14)14, (MyT15)'x', (MyT16)"16");
		thread.Wait();
		REQUIRE(true == g_bRun);
		REQUIRE(1 == g_p1);
		REQUIRE(true == g_p2);
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
		Sys::Thread thread;
		Test test;

		g_bRun = false;
		thread.Run(&test, &Test::fr0);
		thread.Wait();
		REQUIRE(true == g_bRun);
	}

	SECTION("1 arg with return")
	{
		Sys::Thread thread;
		Test test;

		g_bRun = false;
		thread.Run(&test, &Test::fr1, 1);
		thread.Wait();
		REQUIRE(true == g_bRun);
		REQUIRE(1 == g_p1);
	}
	SECTION("2 args with return")
	{
		Sys::Thread thread;
		Test test;

		g_bRun = false;
		thread.Run(&test, &Test::fr2, 1, true);
		thread.Wait();
		REQUIRE(true == g_bRun);
		REQUIRE(1 == g_p1);
		REQUIRE(true == g_p2);
	}
	SECTION("3 args with return")
	{
		Sys::Thread thread;
		Test test;

		g_bRun = false;
		thread.Run(&test, &Test::fr3, 1, true, (MyT3)3);
		thread.Wait();
		REQUIRE(true == g_bRun);
		REQUIRE(1 == g_p1);
		REQUIRE(true == g_p2);
		REQUIRE(3 == g_p3);
	}
	SECTION("4 args with return")
	{
		Sys::Thread thread;
		Test test;

		g_bRun = false;
		thread.Run(&test, &Test::fr4, 1, true, (MyT3)3, (MyT4)"4");
		thread.Wait();
		REQUIRE(true == g_bRun);
		REQUIRE(1 == g_p1);
		REQUIRE(true == g_p2);
		REQUIRE(3 == g_p3);
		REQUIRE(String("4") == g_p4);
	}
	SECTION("5 args with return")
	{
		Sys::Thread thread;
		Test test;

		g_bRun = false;
		thread.Run(&test, &Test::fr5, 1, true, (MyT3)3, (MyT4)"4", (MyT5)5);
		thread.Wait();
		REQUIRE(true == g_bRun);
		REQUIRE(1 == g_p1);
		REQUIRE(true == g_p2);
		REQUIRE(3 == g_p3);
		REQUIRE(String("4") == g_p4);
		REQUIRE(5 == g_p5);
	}
	SECTION("6 args with return")
	{
		Sys::Thread thread;
		Test test;

		g_bRun = false;
		thread.Run(&test, &Test::fr6, 1, true, (MyT3)3, (MyT4)"4", (MyT5)5, (MyT6)6);
		thread.Wait();
		REQUIRE(true == g_bRun);
		REQUIRE(1 == g_p1);
		REQUIRE(true == g_p2);
		REQUIRE(3 == g_p3);
		REQUIRE(String("4") == g_p4);
		REQUIRE(5 == g_p5);
		REQUIRE(6 == g_p6);
	}
	SECTION("7 args with return")
	{
		Sys::Thread thread;
		Test test;

		g_bRun = false;
		thread.Run(&test, &Test::fr7, 1, true, (MyT3)3, (MyT4)"4", (MyT5)5, (MyT6)6, (MyT7)7);
		thread.Wait();
		REQUIRE(true == g_bRun);
		REQUIRE(1 == g_p1);
		REQUIRE(true == g_p2);
		REQUIRE(3 == g_p3);
		REQUIRE(String("4") == g_p4);
		REQUIRE(5 == g_p5);
		REQUIRE(6 == g_p6);
		REQUIRE(7 == g_p7);
	}
	SECTION("8 args with return")
	{
		Sys::Thread thread;
		Test test;

		g_bRun = false;
		thread.Run(&test, &Test::fr8, 1, true, (MyT3)3, (MyT4)"4", (MyT5)5, (MyT6)6, (MyT7)7, (MyT8)8);
		thread.Wait();
		REQUIRE(true == g_bRun);
		REQUIRE(1 == g_p1);
		REQUIRE(true == g_p2);
		REQUIRE(3 == g_p3);
		REQUIRE(String("4") == g_p4);
		REQUIRE(5 == g_p5);
		REQUIRE(6 == g_p6);
		REQUIRE(7 == g_p7);
		REQUIRE(8 == g_p8);
	}
	SECTION("9 args with return")
	{
		Sys::Thread thread;
		Test test;

		g_bRun = false;
		thread.Run(&test, &Test::fr9, 1, true, (MyT3)3, (MyT4)"4", (MyT5)5, (MyT6)6, (MyT7)7, (MyT8)8, (MyT9)9);
		thread.Wait();
		REQUIRE(true == g_bRun);
		REQUIRE(1 == g_p1);
		REQUIRE(true == g_p2);
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
		Sys::Thread thread;
		Test test;

		g_bRun = false;
		thread.Run(&test, &Test::fr10, 1, true, (MyT3)3, (MyT4)"4", (MyT5)5, (MyT6)6, (MyT7)7, (MyT8)8, (MyT9)9, 
		           (MyT10)10);
		thread.Wait();
		REQUIRE(true == g_bRun);
		REQUIRE(1 == g_p1);
		REQUIRE(true == g_p2);
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
		Sys::Thread thread;
		Test test;

		g_bRun = false;
		thread.Run(&test, &Test::fr11, 1, true, (MyT3)3, (MyT4)"4", (MyT5)5, (MyT6)6, (MyT7)7, (MyT8)8, (MyT9)9, 
		           (MyT10)10, (MyT11)11);
		thread.Wait();
		REQUIRE(true == g_bRun);
		REQUIRE(1 == g_p1);
		REQUIRE(true == g_p2);
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
		Sys::Thread thread;
		Test test;

		g_bRun = false;
		thread.Run(&test, &Test::fr12, 1, true, (MyT3)3, (MyT4)"4", (MyT5)5, (MyT6)6, (MyT7)7, (MyT8)8, (MyT9)9, 
		           (MyT10)10, (MyT11)11, (MyT12)12);
		thread.Wait();
		REQUIRE(true == g_bRun);
		REQUIRE(1 == g_p1);
		REQUIRE(true == g_p2);
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
		Sys::Thread thread;
		Test test;

		g_bRun = false;
		thread.Run(&test, &Test::fr13, 1, true, (MyT3)3, (MyT4)"4", (MyT5)5, (MyT6)6, (MyT7)7, (MyT8)8, (MyT9)9, 
		           (MyT10)10, (MyT11)11, (MyT12)12, (MyT13)13);
		thread.Wait();
		REQUIRE(true == g_bRun);
		REQUIRE(1 == g_p1);
		REQUIRE(true == g_p2);
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
		Sys::Thread thread;
		Test test;

		g_bRun = false;
		thread.Run(&test, &Test::fr14, 1, true, (MyT3)3, (MyT4)"4", (MyT5)5, (MyT6)6, (MyT7)7, (MyT8)8, (MyT9)9, 
		           (MyT10)10, (MyT11)11, (MyT12)12, (MyT13)13, (MyT14)14);
		thread.Wait();
		REQUIRE(true == g_bRun);
		REQUIRE(1 == g_p1);
		REQUIRE(true == g_p2);
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
		Sys::Thread thread;
		Test test;

		g_bRun = false;
		thread.Run(&test, &Test::fr15, 1, true, (MyT3)3, (MyT4)"4", (MyT5)5, (MyT6)6, (MyT7)7, (MyT8)8, (MyT9)9, 
		           (MyT10)10, (MyT11)11, (MyT12)12, (MyT13)13, (MyT14)14, (MyT15)'x');
		thread.Wait();
		REQUIRE(true == g_bRun);
		REQUIRE(1 == g_p1);
		REQUIRE(true == g_p2);
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
		Sys::Thread thread;
		Test test;

		g_bRun = false;
		thread.Run(&test, &Test::fr16, 1, true, (MyT3)3, (MyT4)"4", (MyT5)5, (MyT6)6, (MyT7)7, (MyT8)8, (MyT9)9, 
		           (MyT10)10, (MyT11)11, (MyT12)12, (MyT13)13, (MyT14)14, (MyT15)'x', (MyT16)"16");
		thread.Wait();
		REQUIRE(true == g_bRun);
		REQUIRE(1 == g_p1);
		REQUIRE(true == g_p2);
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

}//namespace Sys_Thread

