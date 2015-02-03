
#include "CX/CX.h"
#include "CX/Alloc.h"

#pragma warning(push)
#pragma warning(disable: 4702)
#include "../../../Contrib/Catch/Include/catch.hpp"
#pragma warning(pop)


using namespace CX;


class NewCls
{
public:

	char        m_p1;
	short       m_p2;
	int         m_p3;
	Size        m_p4;
	Int8        m_p5;
	UInt8       m_p6;
	Int16       m_p7;
	UInt16      m_p8;
	Int32       m_p9;
	UInt32      m_p10;
	Int64       m_p11;
	UInt64      m_p12;
	const Char *m_p13;
	String      m_p14;
	Float       m_p15;
	Double      m_p16;

	NewCls()
	{
		m_p1  = '1';
		m_p2  = 2;
		m_p3  = 3;
		m_p4  = 4;
		m_p5  = 5;
		m_p6  = 6;
		m_p7  = 7;
		m_p8  = 8;
		m_p9  = 9;
		m_p10 = 10;
		m_p11 = 11;
		m_p12 = 12;
		m_p13 = "13";
		m_p14 = "14";
		m_p15 = 15.123f;
		m_p16 = 16.123;
	}

	NewCls(char p1)
	{
		m_p1  = p1;
		m_p2  = 2;
		m_p3  = 3;
		m_p4  = 4;
		m_p5  = 5;
		m_p6  = 6;
		m_p7  = 7;
		m_p8  = 8;
		m_p9  = 9;
		m_p10 = 10;
		m_p11 = 11;
		m_p12 = 12;
		m_p13 = "13";
		m_p14 = "14";
		m_p15 = 15.123f;
		m_p16 = 16.123;
	}

	NewCls(char p1, short p2)
	{
		m_p1  = p1;
		m_p2  = p2;
		m_p3  = 3;
		m_p4  = 4;
		m_p5  = 5;
		m_p6  = 6;
		m_p7  = 7;
		m_p8  = 8;
		m_p9  = 9;
		m_p10 = 10;
		m_p11 = 11;
		m_p12 = 12;
		m_p13 = "13";
		m_p14 = "14";
		m_p15 = 15.123f;
		m_p16 = 16.123;
	}

	NewCls(char p1, short p2, int p3)
	{
		m_p1  = p1;
		m_p2  = p2;
		m_p3  = p3;
		m_p4  = 4;
		m_p5  = 5;
		m_p6  = 6;
		m_p7  = 7;
		m_p8  = 8;
		m_p9  = 9;
		m_p10 = 10;
		m_p11 = 11;
		m_p12 = 12;
		m_p13 = "13";
		m_p14 = "14";
		m_p15 = 15.123f;
		m_p16 = 16.123;
	}

	NewCls(char p1, short p2, int p3, Size p4)
	{
		m_p1  = p1;
		m_p2  = p2;
		m_p3  = p3;
		m_p4  = p4;
		m_p5  = 5;
		m_p6  = 6;
		m_p7  = 7;
		m_p8  = 8;
		m_p9  = 9;
		m_p10 = 10;
		m_p11 = 11;
		m_p12 = 12;
		m_p13 = "13";
		m_p14 = "14";
		m_p15 = 15.123f;
		m_p16 = 16.123;
	}

	NewCls(char p1, short p2, int p3, Size p4, Int8 p5)
	{
		m_p1  = p1;
		m_p2  = p2;
		m_p3  = p3;
		m_p4  = p4;
		m_p5  = p5;
		m_p6  = 6;
		m_p7  = 7;
		m_p8  = 8;
		m_p9  = 9;
		m_p10 = 10;
		m_p11 = 11;
		m_p12 = 12;
		m_p13 = "13";
		m_p14 = "14";
		m_p15 = 15.123f;
		m_p16 = 16.123;
	}

	NewCls(char p1, short p2, int p3, Size p4, Int8 p5, UInt8 p6)
	{
		m_p1  = p1;
		m_p2  = p2;
		m_p3  = p3;
		m_p4  = p4;
		m_p5  = p5;
		m_p6  = p6;
		m_p7  = 7;
		m_p8  = 8;
		m_p9  = 9;
		m_p10 = 10;
		m_p11 = 11;
		m_p12 = 12;
		m_p13 = "13";
		m_p14 = "14";
		m_p15 = 15.123f;
		m_p16 = 16.123;
	}

	NewCls(char p1, short p2, int p3, Size p4, Int8 p5, UInt8 p6, Int16 p7)
	{
		m_p1  = p1;
		m_p2  = p2;
		m_p3  = p3;
		m_p4  = p4;
		m_p5  = p5;
		m_p6  = p6;
		m_p7  = p7;
		m_p8  = 8;
		m_p9  = 9;
		m_p10 = 10;
		m_p11 = 11;
		m_p12 = 12;
		m_p13 = "13";
		m_p14 = "14";
		m_p15 = 15.123f;
		m_p16 = 16.123;
	}

	NewCls(char p1, short p2, int p3, Size p4, Int8 p5, UInt8 p6, Int16 p7, UInt16 p8)
	{
		m_p1  = p1;
		m_p2  = p2;
		m_p3  = p3;
		m_p4  = p4;
		m_p5  = p5;
		m_p6  = p6;
		m_p7  = p7;
		m_p8  = p8;
		m_p9  = 9;
		m_p10 = 10;
		m_p11 = 11;
		m_p12 = 12;
		m_p13 = "13";
		m_p14 = "14";
		m_p15 = 15.123f;
		m_p16 = 16.123;
	}

	NewCls(char p1, short p2, int p3, Size p4, Int8 p5, UInt8 p6, Int16 p7, UInt16 p8, Int32 p9)
	{
		m_p1  = p1;
		m_p2  = p2;
		m_p3  = p3;
		m_p4  = p4;
		m_p5  = p5;
		m_p6  = p6;
		m_p7  = p7;
		m_p8  = p8;
		m_p9  = p9;
		m_p10 = 10;
		m_p11 = 11;
		m_p12 = 12;
		m_p13 = "13";
		m_p14 = "14";
		m_p15 = 15.123f;
		m_p16 = 16.123;
	}

	NewCls(char p1, short p2, int p3, Size p4, Int8 p5, UInt8 p6, Int16 p7, UInt16 p8, Int32 p9, 
	       UInt32 p10)
	{
		m_p1  = p1;
		m_p2  = p2;
		m_p3  = p3;
		m_p4  = p4;
		m_p5  = p5;
		m_p6  = p6;
		m_p7  = p7;
		m_p8  = p8;
		m_p9  = p9;
		m_p10 = p10;
		m_p11 = 11;
		m_p12 = 12;
		m_p13 = "13";
		m_p14 = "14";
		m_p15 = 15.123f;
		m_p16 = 16.123;
	}

	NewCls(char p1, short p2, int p3, Size p4, Int8 p5, UInt8 p6, Int16 p7, UInt16 p8, Int32 p9, 
	       UInt32 p10, Int64 p11)
	{
		m_p1  = p1;
		m_p2  = p2;
		m_p3  = p3;
		m_p4  = p4;
		m_p5  = p5;
		m_p6  = p6;
		m_p7  = p7;
		m_p8  = p8;
		m_p9  = p9;
		m_p10 = p10;
		m_p11 = p11;
		m_p12 = 12;
		m_p13 = "13";
		m_p14 = "14";
		m_p15 = 15.123f;
		m_p16 = 16.123;
	}

	NewCls(char p1, short p2, int p3, Size p4, Int8 p5, UInt8 p6, Int16 p7, UInt16 p8, Int32 p9, 
	       UInt32 p10, Int64 p11, UInt64 p12)
	{
		m_p1  = p1;
		m_p2  = p2;
		m_p3  = p3;
		m_p4  = p4;
		m_p5  = p5;
		m_p6  = p6;
		m_p7  = p7;
		m_p8  = p8;
		m_p9  = p9;
		m_p10 = p10;
		m_p11 = p11;
		m_p12 = p12;
		m_p13 = "13";
		m_p14 = "14";
		m_p15 = 15.123f;
		m_p16 = 16.123;
	}

	NewCls(char p1, short p2, int p3, Size p4, Int8 p5, UInt8 p6, Int16 p7, UInt16 p8, Int32 p9, 
	       UInt32 p10, Int64 p11, UInt64 p12, const Char *p13)
	{
		m_p1  = p1;
		m_p2  = p2;
		m_p3  = p3;
		m_p4  = p4;
		m_p5  = p5;
		m_p6  = p6;
		m_p7  = p7;
		m_p8  = p8;
		m_p9  = p9;
		m_p10 = p10;
		m_p11 = p11;
		m_p12 = p12;
		m_p13 = p13;
		m_p14 = "14";
		m_p15 = 15.123f;
		m_p16 = 16.123;
	}

	NewCls(char p1, short p2, int p3, Size p4, Int8 p5, UInt8 p6, Int16 p7, UInt16 p8, Int32 p9, 
	       UInt32 p10, Int64 p11, UInt64 p12, const Char *p13, const Char *p14)
	{
		m_p1  = p1;
		m_p2  = p2;
		m_p3  = p3;
		m_p4  = p4;
		m_p5  = p5;
		m_p6  = p6;
		m_p7  = p7;
		m_p8  = p8;
		m_p9  = p9;
		m_p10 = p10;
		m_p11 = p11;
		m_p12 = p12;
		m_p13 = p13;
		m_p14 = p14;
		m_p15 = 15.123f;
		m_p16 = 16.123;
	}

	NewCls(char p1, short p2, int p3, Size p4, Int8 p5, UInt8 p6, Int16 p7, UInt16 p8, Int32 p9, 
	       UInt32 p10, Int64 p11, UInt64 p12, const Char *p13, const Char *p14, Float p15)
	{
		m_p1  = p1;
		m_p2  = p2;
		m_p3  = p3;
		m_p4  = p4;
		m_p5  = p5;
		m_p6  = p6;
		m_p7  = p7;
		m_p8  = p8;
		m_p9  = p9;
		m_p10 = p10;
		m_p11 = p11;
		m_p12 = p12;
		m_p13 = p13;
		m_p14 = p14;
		m_p15 = p15;
		m_p16 = 16.123;
	}

	NewCls(char p1, short p2, int p3, Size p4, Int8 p5, UInt8 p6, Int16 p7, UInt16 p8, Int32 p9, 
	       UInt32 p10, Int64 p11, UInt64 p12, const Char *p13, const Char *p14, Float p15, 
	       Double p16)
	{
		m_p1  = p1;
		m_p2  = p2;
		m_p3  = p3;
		m_p4  = p4;
		m_p5  = p5;
		m_p6  = p6;
		m_p7  = p7;
		m_p8  = p8;
		m_p9  = p9;
		m_p10 = p10;
		m_p11 = p11;
		m_p12 = p12;
		m_p13 = p13;
		m_p14 = p14;
		m_p15 = p15;
		m_p16 = p16;
	}

};


TEST_CASE("Alloc tests", "[CX::Alloc]")
{
	SECTION("New 0 args")
	{
		NewCls *x = New<NewCls>();

		REQUIRE(x->m_p1 == (Char)'1');
		REQUIRE(x->m_p2 == (short)2);
		REQUIRE(x->m_p3 == (int)3);
		REQUIRE(x->m_p4 == (Size)4);
		REQUIRE(x->m_p5 == (Int8)5);
		REQUIRE(x->m_p6 == (UInt8)6);
		REQUIRE(x->m_p7 == (Int16)7);
		REQUIRE(x->m_p8 == (UInt16)8);
		REQUIRE(x->m_p9 == (Int32)9);
		REQUIRE(x->m_p10 == (UInt32)10);
		REQUIRE(x->m_p11 == (Int64)11);
		REQUIRE(x->m_p12 == (UInt64)12);
		REQUIRE(x->m_p13 == String("13"));
		REQUIRE(x->m_p14 == String("14"));
		REQUIRE(x->m_p15 == (Float)15.123f);
		REQUIRE(x->m_p16 == (Double)16.123);
		
		Delete(x);
	}
	SECTION("New 1 args")
	{
		NewCls *x = New<NewCls>('0');

		REQUIRE(x->m_p1 == (Char)'0');
		REQUIRE(x->m_p2 == (short)2);
		REQUIRE(x->m_p3 == (int)3);
		REQUIRE(x->m_p4 == (Size)4);
		REQUIRE(x->m_p5 == (Int8)5);
		REQUIRE(x->m_p6 == (UInt8)6);
		REQUIRE(x->m_p7 == (Int16)7);
		REQUIRE(x->m_p8 == (UInt16)8);
		REQUIRE(x->m_p9 == (Int32)9);
		REQUIRE(x->m_p10 == (UInt32)10);
		REQUIRE(x->m_p11 == (Int64)11);
		REQUIRE(x->m_p12 == (UInt64)12);
		REQUIRE(x->m_p13 == String("13"));
		REQUIRE(x->m_p14 == String("14"));
		REQUIRE(x->m_p15 == (Float)15.123f);
		REQUIRE(x->m_p16 == (Double)16.123);
		
		Delete(x);
	}
	SECTION("New 2 args")
	{
		NewCls *x = New<NewCls>((Char)'0', (short)12);

		REQUIRE(x->m_p1 == (Char)'0');
		REQUIRE(x->m_p2 == (short)12);
		REQUIRE(x->m_p3 == (int)3);
		REQUIRE(x->m_p4 == (Size)4);
		REQUIRE(x->m_p5 == (Int8)5);
		REQUIRE(x->m_p6 == (UInt8)6);
		REQUIRE(x->m_p7 == (Int16)7);
		REQUIRE(x->m_p8 == (UInt16)8);
		REQUIRE(x->m_p9 == (Int32)9);
		REQUIRE(x->m_p10 == (UInt32)10);
		REQUIRE(x->m_p11 == (Int64)11);
		REQUIRE(x->m_p12 == (UInt64)12);
		REQUIRE(x->m_p13 == String("13"));
		REQUIRE(x->m_p14 == String("14"));
		REQUIRE(x->m_p15 == (Float)15.123f);
		REQUIRE(x->m_p16 == (Double)16.123);
		
		Delete(x);
	}
	SECTION("New 3 args")
	{
		NewCls *x = New<NewCls>((Char)'0', (short)12, (int)13);

		REQUIRE(x->m_p1 == (Char)'0');
		REQUIRE(x->m_p2 == (short)12);
		REQUIRE(x->m_p3 == (int)13);
		REQUIRE(x->m_p4 == (Size)4);
		REQUIRE(x->m_p5 == (Int8)5);
		REQUIRE(x->m_p6 == (UInt8)6);
		REQUIRE(x->m_p7 == (Int16)7);
		REQUIRE(x->m_p8 == (UInt16)8);
		REQUIRE(x->m_p9 == (Int32)9);
		REQUIRE(x->m_p10 == (UInt32)10);
		REQUIRE(x->m_p11 == (Int64)11);
		REQUIRE(x->m_p12 == (UInt64)12);
		REQUIRE(x->m_p13 == String("13"));
		REQUIRE(x->m_p14 == String("14"));
		REQUIRE(x->m_p15 == (Float)15.123f);
		REQUIRE(x->m_p16 == (Double)16.123);
		
		Delete(x);
	}
	SECTION("New 4 args")
	{
		NewCls *x = New<NewCls>((Char)'0', (short)12, (int)13, (Size)14);

		REQUIRE(x->m_p1 == (Char)'0');
		REQUIRE(x->m_p2 == (short)12);
		REQUIRE(x->m_p3 == (int)13);
		REQUIRE(x->m_p4 == (Size)14);
		REQUIRE(x->m_p5 == (Int8)5);
		REQUIRE(x->m_p6 == (UInt8)6);
		REQUIRE(x->m_p7 == (Int16)7);
		REQUIRE(x->m_p8 == (UInt16)8);
		REQUIRE(x->m_p9 == (Int32)9);
		REQUIRE(x->m_p10 == (UInt32)10);
		REQUIRE(x->m_p11 == (Int64)11);
		REQUIRE(x->m_p12 == (UInt64)12);
		REQUIRE(x->m_p13 == String("13"));
		REQUIRE(x->m_p14 == String("14"));
		REQUIRE(x->m_p15 == (Float)15.123f);
		REQUIRE(x->m_p16 == (Double)16.123);
		
		Delete(x);
	}
	SECTION("New 5 args")
	{
		NewCls *x = New<NewCls>((Char)'0', (short)12, (int)13, (Size)14, (Int8)15);

		REQUIRE(x->m_p1 == (Char)'0');
		REQUIRE(x->m_p2 == (short)12);
		REQUIRE(x->m_p3 == (int)13);
		REQUIRE(x->m_p4 == (Size)14);
		REQUIRE(x->m_p5 == (Int8)15);
		REQUIRE(x->m_p6 == (UInt8)6);
		REQUIRE(x->m_p7 == (Int16)7);
		REQUIRE(x->m_p8 == (UInt16)8);
		REQUIRE(x->m_p9 == (Int32)9);
		REQUIRE(x->m_p10 == (UInt32)10);
		REQUIRE(x->m_p11 == (Int64)11);
		REQUIRE(x->m_p12 == (UInt64)12);
		REQUIRE(x->m_p13 == String("13"));
		REQUIRE(x->m_p14 == String("14"));
		REQUIRE(x->m_p15 == (Float)15.123f);
		REQUIRE(x->m_p16 == (Double)16.123);
		
		Delete(x);
	}
	SECTION("New 6 args")
	{
		NewCls *x = New<NewCls>((Char)'0', (short)12, (int)13, (Size)14, (Int8)15, (UInt8)16);

		REQUIRE(x->m_p1 == (Char)'0');
		REQUIRE(x->m_p2 == (short)12);
		REQUIRE(x->m_p3 == (int)13);
		REQUIRE(x->m_p4 == (Size)14);
		REQUIRE(x->m_p5 == (Int8)15);
		REQUIRE(x->m_p6 == (UInt8)16);
		REQUIRE(x->m_p7 == (Int16)7);
		REQUIRE(x->m_p8 == (UInt16)8);
		REQUIRE(x->m_p9 == (Int32)9);
		REQUIRE(x->m_p10 == (UInt32)10);
		REQUIRE(x->m_p11 == (Int64)11);
		REQUIRE(x->m_p12 == (UInt64)12);
		REQUIRE(x->m_p13 == String("13"));
		REQUIRE(x->m_p14 == String("14"));
		REQUIRE(x->m_p15 == (Float)15.123f);
		REQUIRE(x->m_p16 == (Double)16.123);
		
		Delete(x);
	}
	SECTION("New 7 args")
	{
		NewCls *x = New<NewCls>((Char)'0', (short)12, (int)13, (Size)14, (Int8)15, (UInt8)16, 
		                        (Int16)17);

		REQUIRE(x->m_p1 == (Char)'0');
		REQUIRE(x->m_p2 == (short)12);
		REQUIRE(x->m_p3 == (int)13);
		REQUIRE(x->m_p4 == (Size)14);
		REQUIRE(x->m_p5 == (Int8)15);
		REQUIRE(x->m_p6 == (UInt8)16);
		REQUIRE(x->m_p7 == (Int16)17);
		REQUIRE(x->m_p8 == (UInt16)8);
		REQUIRE(x->m_p9 == (Int32)9);
		REQUIRE(x->m_p10 == (UInt32)10);
		REQUIRE(x->m_p11 == (Int64)11);
		REQUIRE(x->m_p12 == (UInt64)12);
		REQUIRE(x->m_p13 == String("13"));
		REQUIRE(x->m_p14 == String("14"));
		REQUIRE(x->m_p15 == (Float)15.123f);
		REQUIRE(x->m_p16 == (Double)16.123);
		
		Delete(x);
	}
	SECTION("New 8 args")
	{
		NewCls *x = New<NewCls>((Char)'0', (short)12, (int)13, (Size)14, (Int8)15, (UInt8)16, 
		                        (Int16)17, (UInt16)18);

		REQUIRE(x->m_p1 == (Char)'0');
		REQUIRE(x->m_p2 == (short)12);
		REQUIRE(x->m_p3 == (int)13);
		REQUIRE(x->m_p4 == (Size)14);
		REQUIRE(x->m_p5 == (Int8)15);
		REQUIRE(x->m_p6 == (UInt8)16);
		REQUIRE(x->m_p7 == (Int16)17);
		REQUIRE(x->m_p8 == (UInt16)18);
		REQUIRE(x->m_p9 == (Int32)9);
		REQUIRE(x->m_p10 == (UInt32)10);
		REQUIRE(x->m_p11 == (Int64)11);
		REQUIRE(x->m_p12 == (UInt64)12);
		REQUIRE(x->m_p13 == String("13"));
		REQUIRE(x->m_p14 == String("14"));
		REQUIRE(x->m_p15 == (Float)15.123f);
		REQUIRE(x->m_p16 == (Double)16.123);
		
		Delete(x);
	}
	SECTION("New 9 args")
	{
		NewCls *x = New<NewCls>((Char)'0', (short)12, (int)13, (Size)14, (Int8)15, (UInt8)16, 
		                        (Int16)17, (UInt16)18, (Int32)19);

		REQUIRE(x->m_p1 == (Char)'0');
		REQUIRE(x->m_p2 == (short)12);
		REQUIRE(x->m_p3 == (int)13);
		REQUIRE(x->m_p4 == (Size)14);
		REQUIRE(x->m_p5 == (Int8)15);
		REQUIRE(x->m_p6 == (UInt8)16);
		REQUIRE(x->m_p7 == (Int16)17);
		REQUIRE(x->m_p8 == (UInt16)18);
		REQUIRE(x->m_p9 == (Int32)19);
		REQUIRE(x->m_p10 == (UInt32)10);
		REQUIRE(x->m_p11 == (Int64)11);
		REQUIRE(x->m_p12 == (UInt64)12);
		REQUIRE(x->m_p13 == String("13"));
		REQUIRE(x->m_p14 == String("14"));
		REQUIRE(x->m_p15 == (Float)15.123f);
		REQUIRE(x->m_p16 == (Double)16.123);
		
		Delete(x);
	}
	SECTION("New 10 args")
	{
		NewCls *x = New<NewCls>((Char)'0', (short)12, (int)13, (Size)14, (Int8)15, (UInt8)16, 
		                        (Int16)17, (UInt16)18, (Int32)19, (UInt32)110);

		REQUIRE(x->m_p1 == (Char)'0');
		REQUIRE(x->m_p2 == (short)12);
		REQUIRE(x->m_p3 == (int)13);
		REQUIRE(x->m_p4 == (Size)14);
		REQUIRE(x->m_p5 == (Int8)15);
		REQUIRE(x->m_p6 == (UInt8)16);
		REQUIRE(x->m_p7 == (Int16)17);
		REQUIRE(x->m_p8 == (UInt16)18);
		REQUIRE(x->m_p9 == (Int32)19);
		REQUIRE(x->m_p10 == (UInt32)110);
		REQUIRE(x->m_p11 == (Int64)11);
		REQUIRE(x->m_p12 == (UInt64)12);
		REQUIRE(x->m_p13 == String("13"));
		REQUIRE(x->m_p14 == String("14"));
		REQUIRE(x->m_p15 == (Float)15.123f);
		REQUIRE(x->m_p16 == (Double)16.123);
		
		Delete(x);
	}
	SECTION("New 11 args")
	{
		NewCls *x = New<NewCls>((Char)'0', (short)12, (int)13, (Size)14, (Int8)15, (UInt8)16, 
		                        (Int16)17, (UInt16)18, (Int32)19, (UInt32)110, (Int64)111);

		REQUIRE(x->m_p1 == (Char)'0');
		REQUIRE(x->m_p2 == (short)12);
		REQUIRE(x->m_p3 == (int)13);
		REQUIRE(x->m_p4 == (Size)14);
		REQUIRE(x->m_p5 == (Int8)15);
		REQUIRE(x->m_p6 == (UInt8)16);
		REQUIRE(x->m_p7 == (Int16)17);
		REQUIRE(x->m_p8 == (UInt16)18);
		REQUIRE(x->m_p9 == (Int32)19);
		REQUIRE(x->m_p10 == (UInt32)110);
		REQUIRE(x->m_p11 == (Int64)111);
		REQUIRE(x->m_p12 == (UInt64)12);
		REQUIRE(x->m_p13 == String("13"));
		REQUIRE(x->m_p14 == String("14"));
		REQUIRE(x->m_p15 == (Float)15.123f);
		REQUIRE(x->m_p16 == (Double)16.123);

		Delete(x);
	}
	SECTION("New 12 args")
	{
		NewCls *x = New<NewCls>((Char)'0', (short)12, (int)13, (Size)14, (Int8)15, (UInt8)16, 
		                        (Int16)17, (UInt16)18, (Int32)19, (UInt32)110, (Int64)111, 
		                        (UInt64)112);

		REQUIRE(x->m_p1 == (Char)'0');
		REQUIRE(x->m_p2 == (short)12);
		REQUIRE(x->m_p3 == (int)13);
		REQUIRE(x->m_p4 == (Size)14);
		REQUIRE(x->m_p5 == (Int8)15);
		REQUIRE(x->m_p6 == (UInt8)16);
		REQUIRE(x->m_p7 == (Int16)17);
		REQUIRE(x->m_p8 == (UInt16)18);
		REQUIRE(x->m_p9 == (Int32)19);
		REQUIRE(x->m_p10 == (UInt32)110);
		REQUIRE(x->m_p11 == (Int64)111);
		REQUIRE(x->m_p12 == (UInt64)112);
		REQUIRE(x->m_p13 == String("13"));
		REQUIRE(x->m_p14 == String("14"));
		REQUIRE(x->m_p15 == (Float)15.123f);
		REQUIRE(x->m_p16 == (Double)16.123);
		
		Delete(x);
	}
	SECTION("New 13 args")
	{
		NewCls *x = New<NewCls>((Char)'0', (short)12, (int)13, (Size)14, (Int8)15, (UInt8)16, 
		                        (Int16)17, (UInt16)18, (Int32)19, (UInt32)110, (Int64)111, 
		                        (UInt64)112, (const Char *)"113");

		REQUIRE(x->m_p1 == (Char)'0');
		REQUIRE(x->m_p2 == (short)12);
		REQUIRE(x->m_p3 == (int)13);
		REQUIRE(x->m_p4 == (Size)14);
		REQUIRE(x->m_p5 == (Int8)15);
		REQUIRE(x->m_p6 == (UInt8)16);
		REQUIRE(x->m_p7 == (Int16)17);
		REQUIRE(x->m_p8 == (UInt16)18);
		REQUIRE(x->m_p9 == (Int32)19);
		REQUIRE(x->m_p10 == (UInt32)110);
		REQUIRE(x->m_p11 == (Int64)111);
		REQUIRE(x->m_p12 == (UInt64)112);
		REQUIRE(x->m_p13 == String("113"));
		REQUIRE(x->m_p14 == String("14"));
		REQUIRE(x->m_p15 == (Float)15.123f);
		REQUIRE(x->m_p16 == (Double)16.123);
		
		Delete(x);
	}
	SECTION("New 14 args")
	{
		NewCls *x = New<NewCls>((Char)'0', (short)12, (int)13, (Size)14, (Int8)15, (UInt8)16, 
		                        (Int16)17, (UInt16)18, (Int32)19, (UInt32)110, (Int64)111, 
		                        (UInt64)112, (const Char *)"113", (const Char *)"114");

		REQUIRE(x->m_p1 == (Char)'0');
		REQUIRE(x->m_p2 == (short)12);
		REQUIRE(x->m_p3 == (int)13);
		REQUIRE(x->m_p4 == (Size)14);
		REQUIRE(x->m_p5 == (Int8)15);
		REQUIRE(x->m_p6 == (UInt8)16);
		REQUIRE(x->m_p7 == (Int16)17);
		REQUIRE(x->m_p8 == (UInt16)18);
		REQUIRE(x->m_p9 == (Int32)19);
		REQUIRE(x->m_p10 == (UInt32)110);
		REQUIRE(x->m_p11 == (Int64)111);
		REQUIRE(x->m_p12 == (UInt64)112);
		REQUIRE(x->m_p13 == String("113"));
		REQUIRE(x->m_p14 == String("114"));
		REQUIRE(x->m_p15 == (Float)15.123f);
		REQUIRE(x->m_p16 == (Double)16.123);
		
		Delete(x);
	}
	SECTION("New 15 args")
	{
		NewCls *x = New<NewCls>((Char)'0', (short)12, (int)13, (Size)14, (Int8)15, (UInt8)16, 
		                        (Int16)17, (UInt16)18, (Int32)19, (UInt32)110, (Int64)111, 
		                        (UInt64)112, (const Char *)"113", (const Char *)"114", 
		                        (Float)115.123f);

		REQUIRE(x->m_p1 == (Char)'0');
		REQUIRE(x->m_p2 == (short)12);
		REQUIRE(x->m_p3 == (int)13);
		REQUIRE(x->m_p4 == (Size)14);
		REQUIRE(x->m_p5 == (Int8)15);
		REQUIRE(x->m_p6 == (UInt8)16);
		REQUIRE(x->m_p7 == (Int16)17);
		REQUIRE(x->m_p8 == (UInt16)18);
		REQUIRE(x->m_p9 == (Int32)19);
		REQUIRE(x->m_p10 == (UInt32)110);
		REQUIRE(x->m_p11 == (Int64)111);
		REQUIRE(x->m_p12 == (UInt64)112);
		REQUIRE(x->m_p13 == String("113"));
		REQUIRE(x->m_p14 == String("114"));
		REQUIRE(x->m_p15 == (Float)115.123f);
		REQUIRE(x->m_p16 == (Double)16.123);
		
		Delete(x);
	}
	SECTION("New 16 args")
	{
		NewCls *x = New<NewCls>((Char)'0', (short)12, (int)13, (Size)14, (Int8)15, (UInt8)16, 
		                        (Int16)17, (UInt16)18, (Int32)19, (UInt32)110, (Int64)111, 
		                        (UInt64)112, (const Char *)"113", (const Char *)"114", 
		                        (Float)115.123f, (Double)116.123);

		REQUIRE(x->m_p1 == (Char)'0');
		REQUIRE(x->m_p2 == (short)12);
		REQUIRE(x->m_p3 == (int)13);
		REQUIRE(x->m_p4 == (Size)14);
		REQUIRE(x->m_p5 == (Int8)15);
		REQUIRE(x->m_p6 == (UInt8)16);
		REQUIRE(x->m_p7 == (Int16)17);
		REQUIRE(x->m_p8 == (UInt16)18);
		REQUIRE(x->m_p9 == (Int32)19);
		REQUIRE(x->m_p10 == (UInt32)110);
		REQUIRE(x->m_p11 == (Int64)111);
		REQUIRE(x->m_p12 == (UInt64)112);
		REQUIRE(x->m_p13 == String("113"));
		REQUIRE(x->m_p14 == String("114"));
		REQUIRE(x->m_p15 == (Float)115.123f);
		REQUIRE(x->m_p16 == (Double)116.123);
		
		Delete(x);
	}
	SECTION("NewArr 0 args")
	{
		NewCls *x = NewArr<NewCls>(10);

		REQUIRE(x->m_p1 == (Char)'1');
		REQUIRE(x->m_p2 == (short)2);
		REQUIRE(x->m_p3 == (int)3);
		REQUIRE(x->m_p4 == (Size)4);
		REQUIRE(x->m_p5 == (Int8)5);
		REQUIRE(x->m_p6 == (UInt8)6);
		REQUIRE(x->m_p7 == (Int16)7);
		REQUIRE(x->m_p8 == (UInt16)8);
		REQUIRE(x->m_p9 == (Int32)9);
		REQUIRE(x->m_p10 == (UInt32)10);
		REQUIRE(x->m_p11 == (Int64)11);
		REQUIRE(x->m_p12 == (UInt64)12);
		REQUIRE(x->m_p13 == String("13"));
		REQUIRE(x->m_p14 == String("14"));
		REQUIRE(x->m_p15 == (Float)15.123f);
		REQUIRE(x->m_p16 == (Double)16.123);
		
		DeleteArr(x);
	}
	SECTION("NewArr 1 args")
	{
		NewCls *x = NewArr<NewCls>(10, '0');

		REQUIRE(x->m_p1 == (Char)'0');
		REQUIRE(x->m_p2 == (short)2);
		REQUIRE(x->m_p3 == (int)3);
		REQUIRE(x->m_p4 == (Size)4);
		REQUIRE(x->m_p5 == (Int8)5);
		REQUIRE(x->m_p6 == (UInt8)6);
		REQUIRE(x->m_p7 == (Int16)7);
		REQUIRE(x->m_p8 == (UInt16)8);
		REQUIRE(x->m_p9 == (Int32)9);
		REQUIRE(x->m_p10 == (UInt32)10);
		REQUIRE(x->m_p11 == (Int64)11);
		REQUIRE(x->m_p12 == (UInt64)12);
		REQUIRE(x->m_p13 == String("13"));
		REQUIRE(x->m_p14 == String("14"));
		REQUIRE(x->m_p15 == (Float)15.123f);
		REQUIRE(x->m_p16 == (Double)16.123);
		
		DeleteArr(x);
	}
	SECTION("NewArr 2 args")
	{
		NewCls *x = NewArr<NewCls>(10, (Char)(Char)'0', (short)12);

		REQUIRE(x->m_p1 == (Char)'0');
		REQUIRE(x->m_p2 == (short)12);
		REQUIRE(x->m_p3 == (int)3);
		REQUIRE(x->m_p4 == (Size)4);
		REQUIRE(x->m_p5 == (Int8)5);
		REQUIRE(x->m_p6 == (UInt8)6);
		REQUIRE(x->m_p7 == (Int16)7);
		REQUIRE(x->m_p8 == (UInt16)8);
		REQUIRE(x->m_p9 == (Int32)9);
		REQUIRE(x->m_p10 == (UInt32)10);
		REQUIRE(x->m_p11 == (Int64)11);
		REQUIRE(x->m_p12 == (UInt64)12);
		REQUIRE(x->m_p13 == String("13"));
		REQUIRE(x->m_p14 == String("14"));
		REQUIRE(x->m_p15 == (Float)15.123f);
		REQUIRE(x->m_p16 == (Double)16.123);
		
		DeleteArr(x);
	}
	SECTION("NewArr 3 args")
	{
		NewCls *x = NewArr<NewCls>(10, (Char)(Char)'0', (short)12, (int)13);

		REQUIRE(x->m_p1 == (Char)'0');
		REQUIRE(x->m_p2 == (short)12);
		REQUIRE(x->m_p3 == (int)13);
		REQUIRE(x->m_p4 == (Size)4);
		REQUIRE(x->m_p5 == (Int8)5);
		REQUIRE(x->m_p6 == (UInt8)6);
		REQUIRE(x->m_p7 == (Int16)7);
		REQUIRE(x->m_p8 == (UInt16)8);
		REQUIRE(x->m_p9 == (Int32)9);
		REQUIRE(x->m_p10 == (UInt32)10);
		REQUIRE(x->m_p11 == (Int64)11);
		REQUIRE(x->m_p12 == (UInt64)12);
		REQUIRE(x->m_p13 == String("13"));
		REQUIRE(x->m_p14 == String("14"));
		REQUIRE(x->m_p15 == (Float)15.123f);
		REQUIRE(x->m_p16 == (Double)16.123);
		
		DeleteArr(x);
	}
	SECTION("NewArr 4 args")
	{
		NewCls *x = NewArr<NewCls>(10, (Char)(Char)'0', (short)12, (int)13, (Size)14);

		REQUIRE(x->m_p1 == (Char)'0');
		REQUIRE(x->m_p2 == (short)12);
		REQUIRE(x->m_p3 == (int)13);
		REQUIRE(x->m_p4 == (Size)14);
		REQUIRE(x->m_p5 == (Int8)5);
		REQUIRE(x->m_p6 == (UInt8)6);
		REQUIRE(x->m_p7 == (Int16)7);
		REQUIRE(x->m_p8 == (UInt16)8);
		REQUIRE(x->m_p9 == (Int32)9);
		REQUIRE(x->m_p10 == (UInt32)10);
		REQUIRE(x->m_p11 == (Int64)11);
		REQUIRE(x->m_p12 == (UInt64)12);
		REQUIRE(x->m_p13 == String("13"));
		REQUIRE(x->m_p14 == String("14"));
		REQUIRE(x->m_p15 == (Float)15.123f);
		REQUIRE(x->m_p16 == (Double)16.123);
		
		DeleteArr(x);
	}
	SECTION("NewArr 5 args")
	{
		NewCls *x = NewArr<NewCls>(10, (Char)(Char)'0', (short)12, (int)13, (Size)14, (Int8)15);

		REQUIRE(x->m_p1 == (Char)'0');
		REQUIRE(x->m_p2 == (short)12);
		REQUIRE(x->m_p3 == (int)13);
		REQUIRE(x->m_p4 == (Size)14);
		REQUIRE(x->m_p5 == (Int8)15);
		REQUIRE(x->m_p6 == (UInt8)6);
		REQUIRE(x->m_p7 == (Int16)7);
		REQUIRE(x->m_p8 == (UInt16)8);
		REQUIRE(x->m_p9 == (Int32)9);
		REQUIRE(x->m_p10 == (UInt32)10);
		REQUIRE(x->m_p11 == (Int64)11);
		REQUIRE(x->m_p12 == (UInt64)12);
		REQUIRE(x->m_p13 == String("13"));
		REQUIRE(x->m_p14 == String("14"));
		REQUIRE(x->m_p15 == (Float)15.123f);
		REQUIRE(x->m_p16 == (Double)16.123);
		
		DeleteArr(x);
	}
	SECTION("New 6 args")
	{
		NewCls *x = NewArr<NewCls>(10, (Char)(Char)'0', (short)12, (int)13, (Size)14, (Int8)15, 
		                           (UInt8)16);

		REQUIRE(x->m_p1 == (Char)'0');
		REQUIRE(x->m_p2 == (short)12);
		REQUIRE(x->m_p3 == (int)13);
		REQUIRE(x->m_p4 == (Size)14);
		REQUIRE(x->m_p5 == (Int8)15);
		REQUIRE(x->m_p6 == (UInt8)16);
		REQUIRE(x->m_p7 == (Int16)7);
		REQUIRE(x->m_p8 == (UInt16)8);
		REQUIRE(x->m_p9 == (Int32)9);
		REQUIRE(x->m_p10 == (UInt32)10);
		REQUIRE(x->m_p11 == (Int64)11);
		REQUIRE(x->m_p12 == (UInt64)12);
		REQUIRE(x->m_p13 == String("13"));
		REQUIRE(x->m_p14 == String("14"));
		REQUIRE(x->m_p15 == (Float)15.123f);
		REQUIRE(x->m_p16 == (Double)16.123);
		
		DeleteArr(x);
	}
	SECTION("NewArr 7 args")
	{
		NewCls *x = NewArr<NewCls>(10, (Char)(Char)'0', (short)12, (int)13, (Size)14, (Int8)15, 
		                           (UInt8)16, (Int16)17);

		REQUIRE(x->m_p1 == (Char)'0');
		REQUIRE(x->m_p2 == (short)12);
		REQUIRE(x->m_p3 == (int)13);
		REQUIRE(x->m_p4 == (Size)14);
		REQUIRE(x->m_p5 == (Int8)15);
		REQUIRE(x->m_p6 == (UInt8)16);
		REQUIRE(x->m_p7 == (Int16)17);
		REQUIRE(x->m_p8 == (UInt16)8);
		REQUIRE(x->m_p9 == (Int32)9);
		REQUIRE(x->m_p10 == (UInt32)10);
		REQUIRE(x->m_p11 == (Int64)11);
		REQUIRE(x->m_p12 == (UInt64)12);
		REQUIRE(x->m_p13 == String("13"));
		REQUIRE(x->m_p14 == String("14"));
		REQUIRE(x->m_p15 == (Float)15.123f);
		REQUIRE(x->m_p16 == (Double)16.123);

		DeleteArr(x);
	}
	SECTION("NewArr 8 args")
	{
		NewCls *x = NewArr<NewCls>(10, (Char)(Char)'0', (short)12, (int)13, (Size)14, (Int8)15, 
		                           (UInt8)16, (Int16)17, (UInt16)18);

		REQUIRE(x->m_p1 == (Char)'0');
		REQUIRE(x->m_p2 == (short)12);
		REQUIRE(x->m_p3 == (int)13);
		REQUIRE(x->m_p4 == (Size)14);
		REQUIRE(x->m_p5 == (Int8)15);
		REQUIRE(x->m_p6 == (UInt8)16);
		REQUIRE(x->m_p7 == (Int16)17);
		REQUIRE(x->m_p8 == (UInt16)18);
		REQUIRE(x->m_p9 == (Int32)9);
		REQUIRE(x->m_p10 == (UInt32)10);
		REQUIRE(x->m_p11 == (Int64)11);
		REQUIRE(x->m_p12 == (UInt64)12);
		REQUIRE(x->m_p13 == String("13"));
		REQUIRE(x->m_p14 == String("14"));
		REQUIRE(x->m_p15 == (Float)15.123f);
		REQUIRE(x->m_p16 == (Double)16.123);

		DeleteArr(x);
	}
	SECTION("NewArr 9 args")
	{
		NewCls *x = NewArr<NewCls>(10, (Char)(Char)'0', (short)12, (int)13, (Size)14, (Int8)15, 
		                           (UInt8)16, (Int16)17, (UInt16)18, (Int32)19);

		REQUIRE(x->m_p1 == (Char)'0');
		REQUIRE(x->m_p2 == (short)12);
		REQUIRE(x->m_p3 == (int)13);
		REQUIRE(x->m_p4 == (Size)14);
		REQUIRE(x->m_p5 == (Int8)15);
		REQUIRE(x->m_p6 == (UInt8)16);
		REQUIRE(x->m_p7 == (Int16)17);
		REQUIRE(x->m_p8 == (UInt16)18);
		REQUIRE(x->m_p9 == (Int32)19);
		REQUIRE(x->m_p10 == (UInt32)10);
		REQUIRE(x->m_p11 == (Int64)11);
		REQUIRE(x->m_p12 == (UInt64)12);
		REQUIRE(x->m_p13 == String("13"));
		REQUIRE(x->m_p14 == String("14"));
		REQUIRE(x->m_p15 == (Float)15.123f);
		REQUIRE(x->m_p16 == (Double)16.123);

		DeleteArr(x);
	}
	SECTION("NewArr 10 args")
	{
		NewCls *x = NewArr<NewCls>(10, (Char)(Char)'0', (short)12, (int)13, (Size)14, (Int8)15, 
		                           (UInt8)16, (Int16)17, (UInt16)18, (Int32)19, (UInt32)110);

		REQUIRE(x->m_p1 == (Char)'0');
		REQUIRE(x->m_p2 == (short)12);
		REQUIRE(x->m_p3 == (int)13);
		REQUIRE(x->m_p4 == (Size)14);
		REQUIRE(x->m_p5 == (Int8)15);
		REQUIRE(x->m_p6 == (UInt8)16);
		REQUIRE(x->m_p7 == (Int16)17);
		REQUIRE(x->m_p8 == (UInt16)18);
		REQUIRE(x->m_p9 == (Int32)19);
		REQUIRE(x->m_p10 == (UInt32)110);
		REQUIRE(x->m_p11 == (Int64)11);
		REQUIRE(x->m_p12 == (UInt64)12);
		REQUIRE(x->m_p13 == String("13"));
		REQUIRE(x->m_p14 == String("14"));
		REQUIRE(x->m_p15 == (Float)15.123f);
		REQUIRE(x->m_p16 == (Double)16.123);

		DeleteArr(x);
	}
	SECTION("NewArr 11 args")
	{
		NewCls *x = NewArr<NewCls>(10, (Char)(Char)'0', (short)12, (int)13, (Size)14, (Int8)15, 
		                           (UInt8)16, (Int16)17, (UInt16)18, (Int32)19, (UInt32)110, 
		                           (Int64)111);

		REQUIRE(x->m_p1 == (Char)'0');
		REQUIRE(x->m_p2 == (short)12);
		REQUIRE(x->m_p3 == (int)13);
		REQUIRE(x->m_p4 == (Size)14);
		REQUIRE(x->m_p5 == (Int8)15);
		REQUIRE(x->m_p6 == (UInt8)16);
		REQUIRE(x->m_p7 == (Int16)17);
		REQUIRE(x->m_p8 == (UInt16)18);
		REQUIRE(x->m_p9 == (Int32)19);
		REQUIRE(x->m_p10 == (UInt32)110);
		REQUIRE(x->m_p11 == (Int64)111);
		REQUIRE(x->m_p12 == (UInt64)12);
		REQUIRE(x->m_p13 == String("13"));
		REQUIRE(x->m_p14 == String("14"));
		REQUIRE(x->m_p15 == (Float)15.123f);
		REQUIRE(x->m_p16 == (Double)16.123);

		DeleteArr(x);
	}
	SECTION("NewArr 12 args")
	{
		NewCls *x = NewArr<NewCls>(10, (Char)(Char)'0', (short)12, (int)13, (Size)14, (Int8)15, 
		                           (UInt8)16, (Int16)17, (UInt16)18, (Int32)19, (UInt32)110, 
		                           (Int64)111, (UInt64)112);

		REQUIRE(x->m_p1 == (Char)'0');
		REQUIRE(x->m_p2 == (short)12);
		REQUIRE(x->m_p3 == (int)13);
		REQUIRE(x->m_p4 == (Size)14);
		REQUIRE(x->m_p5 == (Int8)15);
		REQUIRE(x->m_p6 == (UInt8)16);
		REQUIRE(x->m_p7 == (Int16)17);
		REQUIRE(x->m_p8 == (UInt16)18);
		REQUIRE(x->m_p9 == (Int32)19);
		REQUIRE(x->m_p10 == (UInt32)110);
		REQUIRE(x->m_p11 == (Int64)111);
		REQUIRE(x->m_p12 == (UInt64)112);
		REQUIRE(x->m_p13 == String("13"));
		REQUIRE(x->m_p14 == String("14"));
		REQUIRE(x->m_p15 == (Float)15.123f);
		REQUIRE(x->m_p16 == (Double)16.123);

		DeleteArr(x);
	}
	SECTION("New 13 args")
	{
		NewCls *x = NewArr<NewCls>(10, (Char)(Char)'0', (short)12, (int)13, (Size)14, (Int8)15, 
		                           (UInt8)16, (Int16)17, (UInt16)18, (Int32)19, (UInt32)110, 
		                           (Int64)111, (UInt64)112, (const Char *)"113");

		REQUIRE(x->m_p1 == (Char)'0');
		REQUIRE(x->m_p2 == (short)12);
		REQUIRE(x->m_p3 == (int)13);
		REQUIRE(x->m_p4 == (Size)14);
		REQUIRE(x->m_p5 == (Int8)15);
		REQUIRE(x->m_p6 == (UInt8)16);
		REQUIRE(x->m_p7 == (Int16)17);
		REQUIRE(x->m_p8 == (UInt16)18);
		REQUIRE(x->m_p9 == (Int32)19);
		REQUIRE(x->m_p10 == (UInt32)110);
		REQUIRE(x->m_p11 == (Int64)111);
		REQUIRE(x->m_p12 == (UInt64)112);
		REQUIRE(x->m_p13 == String("113"));
		REQUIRE(x->m_p14 == String("14"));
		REQUIRE(x->m_p15 == (Float)15.123f);
		REQUIRE(x->m_p16 == (Double)16.123);

		DeleteArr(x);
	}
	SECTION("NewArr 14 args")
	{
		NewCls *x = NewArr<NewCls>(10, (Char)(Char)'0', (short)12, (int)13, (Size)14, (Int8)15, 
		                           (UInt8)16, (Int16)17, (UInt16)18, (Int32)19, (UInt32)110, 
		                           (Int64)111, (UInt64)112, (const Char *)"113", (const Char *)"114");

		REQUIRE(x->m_p1 == (Char)'0');
		REQUIRE(x->m_p2 == (short)12);
		REQUIRE(x->m_p3 == (int)13);
		REQUIRE(x->m_p4 == (Size)14);
		REQUIRE(x->m_p5 == (Int8)15);
		REQUIRE(x->m_p6 == (UInt8)16);
		REQUIRE(x->m_p7 == (Int16)17);
		REQUIRE(x->m_p8 == (UInt16)18);
		REQUIRE(x->m_p9 == (Int32)19);
		REQUIRE(x->m_p10 == (UInt32)110);
		REQUIRE(x->m_p11 == (Int64)111);
		REQUIRE(x->m_p12 == (UInt64)112);
		REQUIRE(x->m_p13 == String("113"));
		REQUIRE(x->m_p14 == String("114"));
		REQUIRE(x->m_p15 == (Float)15.123f);
		REQUIRE(x->m_p16 == (Double)16.123);

		DeleteArr(x);
	}
	SECTION("NewArr 15 args")
	{
		NewCls *x = NewArr<NewCls>(10, (Char)(Char)'0', (short)12, (int)13, (Size)14, (Int8)15, 
		                           (UInt8)16, (Int16)17, (UInt16)18, (Int32)19, (UInt32)110, 
		                           (Int64)111, (UInt64)112, (const Char *)"113", (const Char *)"114", 
		                           (Float)115.123f);

		REQUIRE(x->m_p1 == (Char)'0');
		REQUIRE(x->m_p2 == (short)12);
		REQUIRE(x->m_p3 == (int)13);
		REQUIRE(x->m_p4 == (Size)14);
		REQUIRE(x->m_p5 == (Int8)15);
		REQUIRE(x->m_p6 == (UInt8)16);
		REQUIRE(x->m_p7 == (Int16)17);
		REQUIRE(x->m_p8 == (UInt16)18);
		REQUIRE(x->m_p9 == (Int32)19);
		REQUIRE(x->m_p10 == (UInt32)110);
		REQUIRE(x->m_p11 == (Int64)111);
		REQUIRE(x->m_p12 == (UInt64)112);
		REQUIRE(x->m_p13 == String("113"));
		REQUIRE(x->m_p14 == String("114"));
		REQUIRE(x->m_p15 == (Float)115.123f);
		REQUIRE(x->m_p16 == (Double)16.123);

		DeleteArr(x);
	}
	SECTION("NewArr 16 args")
	{
		NewCls *x = NewArr<NewCls>(10, (Char)(Char)'0', (short)12, (int)13, (Size)14, (Int8)15, 
		                           (UInt8)16, (Int16)17, (UInt16)18, (Int32)19, (UInt32)110, 
		                           (Int64)111, (UInt64)112, (const Char *)"113", (const Char *)"114", 
		                           (Float)115.123f, (Double)116.123);

		REQUIRE(x->m_p1 == (Char)'0');
		REQUIRE(x->m_p2 == (short)12);
		REQUIRE(x->m_p3 == (int)13);
		REQUIRE(x->m_p4 == (Size)14);
		REQUIRE(x->m_p5 == (Int8)15);
		REQUIRE(x->m_p6 == (UInt8)16);
		REQUIRE(x->m_p7 == (Int16)17);
		REQUIRE(x->m_p8 == (UInt16)18);
		REQUIRE(x->m_p9 == (Int32)19);
		REQUIRE(x->m_p10 == (UInt32)110);
		REQUIRE(x->m_p11 == (Int64)111);
		REQUIRE(x->m_p12 == (UInt64)112);
		REQUIRE(x->m_p13 == String("113"));
		REQUIRE(x->m_p14 == String("114"));
		REQUIRE(x->m_p15 == (Float)115.123f);
		REQUIRE(x->m_p16 == (Double)116.123);

		DeleteArr(x);
	}
}

