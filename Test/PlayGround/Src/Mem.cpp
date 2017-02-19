/* 
 * CX - C++ framework for general purpose development
 *
 * https://github.com/draede/cx
 * 
 * Copyright (C) 2014 - 2017 draede - draede [at] outlook [dot] com
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

#include "CX/Mem.hpp"
#include "CX/Print.hpp"
#include "Tester.hpp"


using namespace CX;


class MemTest
{
};

void Mem_Test1()
{
	MemTest *pTest = new (std::nothrow) MemTest();
	char    *x     = (char *)Mem::Alloc(sizeof(char) * 100);

	memcpy(x, "this is a test", 14);

	Mem::Free(x);
	delete pTest;
}

class Cls1
{
public:

protected:

	friend class CX::Mem;

	Cls1()
	{
	}

	Cls1(int p1)
	{
		if (1 != p1) Print(stdout, "P1 invalid\n");
	}

	Cls1(int p1, int p2)
	{
		if (1 != p1) Print(stdout, "P1 invalid\n");
		if (2 != p2) Print(stdout, "P2 invalid\n");
	}

	Cls1(int p1, int p2, int p3)
	{
		if (1 != p1) Print(stdout, "P1 invalid\n");
		if (2 != p2) Print(stdout, "P2 invalid\n");
		if (3 != p3) Print(stdout, "P3 invalid\n");
	}

	Cls1(int p1, int p2, int p3, int p4)
	{
		if (1 != p1) Print(stdout, "P1 invalid\n");
		if (2 != p2) Print(stdout, "P2 invalid\n");
		if (3 != p3) Print(stdout, "P3 invalid\n");
		if (4 != p4) Print(stdout, "P4 invalid\n");
	}

	Cls1(int p1, int p2, int p3, int p4, int p5)
	{
		if (1 != p1) Print(stdout, "P1 invalid\n");
		if (2 != p2) Print(stdout, "P2 invalid\n");
		if (3 != p3) Print(stdout, "P3 invalid\n");
		if (4 != p4) Print(stdout, "P4 invalid\n");
		if (5 != p5) Print(stdout, "P5 invalid\n");
	}

	Cls1(int p1, int p2, int p3, int p4, int p5, int p6)
	{
		if (1 != p1) Print(stdout, "P1 invalid\n");
		if (2 != p2) Print(stdout, "P2 invalid\n");
		if (3 != p3) Print(stdout, "P3 invalid\n");
		if (4 != p4) Print(stdout, "P4 invalid\n");
		if (5 != p5) Print(stdout, "P5 invalid\n");
		if (6 != p6) Print(stdout, "P6 invalid\n");
	}

	Cls1(int p1, int p2, int p3, int p4, int p5, int p6, int p7)
	{
		if (1 != p1) Print(stdout, "P1 invalid\n");
		if (2 != p2) Print(stdout, "P2 invalid\n");
		if (3 != p3) Print(stdout, "P3 invalid\n");
		if (4 != p4) Print(stdout, "P4 invalid\n");
		if (5 != p5) Print(stdout, "P5 invalid\n");
		if (6 != p6) Print(stdout, "P6 invalid\n");
		if (7 != p7) Print(stdout, "P7 invalid\n");
	}

	Cls1(int p1, int p2, int p3, int p4, int p5, int p6, int p7, int p8)
	{
		if (1 != p1) Print(stdout, "P1 invalid\n");
		if (2 != p2) Print(stdout, "P2 invalid\n");
		if (3 != p3) Print(stdout, "P3 invalid\n");
		if (4 != p4) Print(stdout, "P4 invalid\n");
		if (5 != p5) Print(stdout, "P5 invalid\n");
		if (6 != p6) Print(stdout, "P6 invalid\n");
		if (7 != p7) Print(stdout, "P7 invalid\n");
		if (8 != p8) Print(stdout, "P8 invalid\n");
	}

	Cls1(int p1, int p2, int p3, int p4, int p5, int p6, int p7, int p8, int p9)
	{
		if (1 != p1) Print(stdout, "P1 invalid\n");
		if (2 != p2) Print(stdout, "P2 invalid\n");
		if (3 != p3) Print(stdout, "P3 invalid\n");
		if (4 != p4) Print(stdout, "P4 invalid\n");
		if (5 != p5) Print(stdout, "P5 invalid\n");
		if (6 != p6) Print(stdout, "P6 invalid\n");
		if (7 != p7) Print(stdout, "P7 invalid\n");
		if (8 != p8) Print(stdout, "P8 invalid\n");
		if (9 != p9) Print(stdout, "P9 invalid\n");
	}

	Cls1(int p1, int p2, int p3, int p4, int p5, int p6, int p7, int p8, int p9, int p10)
	{
		if (1 != p1) Print(stdout, "P1 invalid\n");
		if (2 != p2) Print(stdout, "P2 invalid\n");
		if (3 != p3) Print(stdout, "P3 invalid\n");
		if (4 != p4) Print(stdout, "P4 invalid\n");
		if (5 != p5) Print(stdout, "P5 invalid\n");
		if (6 != p6) Print(stdout, "P6 invalid\n");
		if (7 != p7) Print(stdout, "P7 invalid\n");
		if (8 != p8) Print(stdout, "P8 invalid\n");
		if (9 != p9) Print(stdout, "P9 invalid\n");
		if (10 != p10) Print(stdout, "P10 invalid\n");
	}

	Cls1(int p1, int p2, int p3, int p4, int p5, int p6, int p7, int p8, int p9, int p10, int p11)
	{
		if (1 != p1) Print(stdout, "P1 invalid\n");
		if (2 != p2) Print(stdout, "P2 invalid\n");
		if (3 != p3) Print(stdout, "P3 invalid\n");
		if (4 != p4) Print(stdout, "P4 invalid\n");
		if (5 != p5) Print(stdout, "P5 invalid\n");
		if (6 != p6) Print(stdout, "P6 invalid\n");
		if (7 != p7) Print(stdout, "P7 invalid\n");
		if (8 != p8) Print(stdout, "P8 invalid\n");
		if (9 != p9) Print(stdout, "P9 invalid\n");
		if (10 != p10) Print(stdout, "P10 invalid\n");
		if (11 != p11) Print(stdout, "P11 invalid\n");
	}

	Cls1(int p1, int p2, int p3, int p4, int p5, int p6, int p7, int p8, int p9, int p10, int p11, int p12)
	{
		if (1 != p1) Print(stdout, "P1 invalid\n");
		if (2 != p2) Print(stdout, "P2 invalid\n");
		if (3 != p3) Print(stdout, "P3 invalid\n");
		if (4 != p4) Print(stdout, "P4 invalid\n");
		if (5 != p5) Print(stdout, "P5 invalid\n");
		if (6 != p6) Print(stdout, "P6 invalid\n");
		if (7 != p7) Print(stdout, "P7 invalid\n");
		if (8 != p8) Print(stdout, "P8 invalid\n");
		if (9 != p9) Print(stdout, "P9 invalid\n");
		if (10 != p10) Print(stdout, "P10 invalid\n");
		if (11 != p11) Print(stdout, "P11 invalid\n");
		if (12 != p12) Print(stdout, "P12 invalid\n");
	}

	Cls1(int p1, int p2, int p3, int p4, int p5, int p6, int p7, int p8, int p9, int p10, int p11, int p12, int p13)
	{
		if (1 != p1) Print(stdout, "P1 invalid\n");
		if (2 != p2) Print(stdout, "P2 invalid\n");
		if (3 != p3) Print(stdout, "P3 invalid\n");
		if (4 != p4) Print(stdout, "P4 invalid\n");
		if (5 != p5) Print(stdout, "P5 invalid\n");
		if (6 != p6) Print(stdout, "P6 invalid\n");
		if (7 != p7) Print(stdout, "P7 invalid\n");
		if (8 != p8) Print(stdout, "P8 invalid\n");
		if (9 != p9) Print(stdout, "P9 invalid\n");
		if (10 != p10) Print(stdout, "P10 invalid\n");
		if (11 != p11) Print(stdout, "P11 invalid\n");
		if (12 != p12) Print(stdout, "P12 invalid\n");
		if (13 != p13) Print(stdout, "P13 invalid\n");
	}

	Cls1(int p1, int p2, int p3, int p4, int p5, int p6, int p7, int p8, int p9, int p10, int p11, int p12, int p13, int p14)
	{
		if (1 != p1) Print(stdout, "P1 invalid\n");
		if (2 != p2) Print(stdout, "P2 invalid\n");
		if (3 != p3) Print(stdout, "P3 invalid\n");
		if (4 != p4) Print(stdout, "P4 invalid\n");
		if (5 != p5) Print(stdout, "P5 invalid\n");
		if (6 != p6) Print(stdout, "P6 invalid\n");
		if (7 != p7) Print(stdout, "P7 invalid\n");
		if (8 != p8) Print(stdout, "P8 invalid\n");
		if (9 != p9) Print(stdout, "P9 invalid\n");
		if (10 != p10) Print(stdout, "P10 invalid\n");
		if (11 != p11) Print(stdout, "P11 invalid\n");
		if (12 != p12) Print(stdout, "P12 invalid\n");
		if (13 != p13) Print(stdout, "P13 invalid\n");
		if (14 != p14) Print(stdout, "P14 invalid\n");
	}

	Cls1(int p1, int p2, int p3, int p4, int p5, int p6, int p7, int p8, int p9, int p10, int p11, int p12, int p13, int p14, int p15)
	{
		if (1 != p1) Print(stdout, "P1 invalid\n");
		if (2 != p2) Print(stdout, "P2 invalid\n");
		if (3 != p3) Print(stdout, "P3 invalid\n");
		if (4 != p4) Print(stdout, "P4 invalid\n");
		if (5 != p5) Print(stdout, "P5 invalid\n");
		if (6 != p6) Print(stdout, "P6 invalid\n");
		if (7 != p7) Print(stdout, "P7 invalid\n");
		if (8 != p8) Print(stdout, "P8 invalid\n");
		if (9 != p9) Print(stdout, "P9 invalid\n");
		if (10 != p10) Print(stdout, "P10 invalid\n");
		if (11 != p11) Print(stdout, "P11 invalid\n");
		if (12 != p12) Print(stdout, "P12 invalid\n");
		if (13 != p13) Print(stdout, "P13 invalid\n");
		if (14 != p14) Print(stdout, "P14 invalid\n");
		if (15 != p15) Print(stdout, "P15 invalid\n");
	}

	Cls1(int p1, int p2, int p3, int p4, int p5, int p6, int p7, int p8, int p9, int p10, int p11, int p12, int p13, int p14, int p15, int p16)
	{
		if (1 != p1) Print(stdout, "P1 invalid\n");
		if (2 != p2) Print(stdout, "P2 invalid\n");
		if (3 != p3) Print(stdout, "P3 invalid\n");
		if (4 != p4) Print(stdout, "P4 invalid\n");
		if (5 != p5) Print(stdout, "P5 invalid\n");
		if (6 != p6) Print(stdout, "P6 invalid\n");
		if (7 != p7) Print(stdout, "P7 invalid\n");
		if (8 != p8) Print(stdout, "P8 invalid\n");
		if (9 != p9) Print(stdout, "P9 invalid\n");
		if (10 != p10) Print(stdout, "P10 invalid\n");
		if (11 != p11) Print(stdout, "P11 invalid\n");
		if (12 != p12) Print(stdout, "P12 invalid\n");
		if (13 != p13) Print(stdout, "P13 invalid\n");
		if (14 != p14) Print(stdout, "P14 invalid\n");
		if (15 != p15) Print(stdout, "P15 invalid\n");
		if (16 != p16) Print(stdout, "P16 invalid\n");
	}

	~Cls1()
	{
	}

};

class Cls2
{
public:

	Cls2()
	{

	}

	~Cls2()
	{
	}

private:


};

void Mem_TestNew()
{
	Cls1 *pInst0 = Mem::New<Cls1>();
	Cls1 *pInst1 = Mem::New<Cls1>(1);
	Cls1 *pInst2 = Mem::New<Cls1>(1, 2);
	Cls1 *pInst3 = Mem::New<Cls1>(1, 2, 3);
	Cls1 *pInst4 = Mem::New<Cls1>(1, 2, 3, 4);
	Cls1 *pInst5 = Mem::New<Cls1>(1, 2, 3, 4, 5);
	Cls1 *pInst6 = Mem::New<Cls1>(1, 2, 3, 4, 5, 6);
	Cls1 *pInst7 = Mem::New<Cls1>(1, 2, 3, 4, 5, 6, 7);
	Cls1 *pInst8 = Mem::New<Cls1>(1, 2, 3, 4, 5, 6, 7, 8);
	Cls1 *pInst9 = Mem::New<Cls1>(1, 2, 3, 4, 5, 6, 7, 8, 9);
	Cls1 *pInst10 = Mem::New<Cls1>(1, 2, 3, 4, 5, 6, 7, 8, 9, 10);
	Cls1 *pInst11 = Mem::New<Cls1>(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11);
	Cls1 *pInst12 = Mem::New<Cls1>(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12);
	Cls1 *pInst13 = Mem::New<Cls1>(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13);
	Cls1 *pInst14 = Mem::New<Cls1>(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14);
	Cls1 *pInst15 = Mem::New<Cls1>(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15);
	Cls1 *pInst16 = Mem::New<Cls1>(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16);

	Mem::Delete(pInst16);
	Mem::Delete(pInst15);
	Mem::Delete(pInst14);
	Mem::Delete(pInst13);
	Mem::Delete(pInst12);
	Mem::Delete(pInst11);
	Mem::Delete(pInst10);
	Mem::Delete(pInst9);
	Mem::Delete(pInst8);
	Mem::Delete(pInst7);
	Mem::Delete(pInst6);
	Mem::Delete(pInst5);
	Mem::Delete(pInst4);
	Mem::Delete(pInst3);
	Mem::Delete(pInst2);
	Mem::Delete(pInst1);
	Mem::Delete(pInst0);
}

void Mem_TestNewArr()
{
	Cls1 *pInsts0 = Mem::NewArr<Cls1>(3);
	Cls1 *pInsts1 = Mem::NewArr<Cls1>(3, 1);
	Cls1 *pInsts2 = Mem::NewArr<Cls1>(3, 1, 2);
	Cls1 *pInsts3 = Mem::NewArr<Cls1>(3, 1, 2, 3);
	Cls1 *pInsts4 = Mem::NewArr<Cls1>(3, 1, 2, 3, 4);
	Cls1 *pInsts5 = Mem::NewArr<Cls1>(3, 1, 2, 3, 4, 5);
	Cls1 *pInsts6 = Mem::NewArr<Cls1>(3, 1, 2, 3, 4, 5, 6);
	Cls1 *pInsts7 = Mem::NewArr<Cls1>(3, 1, 2, 3, 4, 5, 6, 7);
	Cls1 *pInsts8 = Mem::NewArr<Cls1>(3, 1, 2, 3, 4, 5, 6, 7, 8);
	Cls1 *pInsts9 = Mem::NewArr<Cls1>(3, 1, 2, 3, 4, 5, 6, 7, 8, 9);
	Cls1 *pInsts10 = Mem::NewArr<Cls1>(3, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10);
	Cls1 *pInsts11 = Mem::NewArr<Cls1>(3, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11);
	Cls1 *pInsts12 = Mem::NewArr<Cls1>(3, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12);
	Cls1 *pInsts13 = Mem::NewArr<Cls1>(3, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13);
	Cls1 *pInsts14 = Mem::NewArr<Cls1>(3, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14);
	Cls1 *pInsts15 = Mem::NewArr<Cls1>(3, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15);
	Cls1 *pInsts16 = Mem::NewArr<Cls1>(3, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16);

	Mem::DeleteArr(pInsts16);
	Mem::DeleteArr(pInsts15);
	Mem::DeleteArr(pInsts14);
	Mem::DeleteArr(pInsts13);
	Mem::DeleteArr(pInsts12);
	Mem::DeleteArr(pInsts11);
	Mem::DeleteArr(pInsts10);
	Mem::DeleteArr(pInsts9);
	Mem::DeleteArr(pInsts8);
	Mem::DeleteArr(pInsts7);
	Mem::DeleteArr(pInsts6);
	Mem::DeleteArr(pInsts5);
	Mem::DeleteArr(pInsts4);
	Mem::DeleteArr(pInsts3);
	Mem::DeleteArr(pInsts2);
	Mem::DeleteArr(pInsts1);
	Mem::DeleteArr(pInsts0);
}

REGISTER_TEST(Mem_Test1);
REGISTER_TEST(Mem_TestNew);
REGISTER_TEST(Mem_TestNewArr);

