/* 
 * CX - C++ framework for general purpose development
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

#pragma once


#include "CX/Types.hpp"
#include "CX/IObject.hpp"
#include "CX/C/string.h"
#include "CX/Platform.hpp"
#include "CX/APIDefs.hpp"


#define CX_MEM_NEW_HELPER(...)                                                                                                   \
T *pInst;                                                                                                                        \
                                                                                                                                 \
if (NULL == (pInst = (T *)Mem::Alloc(sizeof(T))))                                                                                \
{                                                                                                                                \
	return NULL;                                                                                                                  \
}                                                                                                                                \
new (pInst)T(__VA_ARGS__);                                                                                                       \
                                                                                                                                 \
return pInst;


#define CX_MEM_NEWARR_HELPER(...)                                                                                                \
if (0 == cCount)                                                                                                                 \
{                                                                                                                                \
	return NULL;                                                                                                                  \
}                                                                                                                                \
                                                                                                                                 \
Byte *pPtr;                                                                                                                      \
T    *pInsts;                                                                                                                    \
                                                                                                                                 \
if (NULL == (pPtr = (Byte *)Mem::Alloc(sizeof(Size) + cCount * sizeof(T))))                                                      \
{                                                                                                                                \
	return NULL;                                                                                                                  \
}                                                                                                                                \
memcpy(pPtr, &cCount, sizeof(Size));                                                                                             \
pInsts = (T *)(pPtr + sizeof(Size));                                                                                             \
for (Size i = 0; i < cCount; i++)                                                                                                \
{                                                                                                                                \
	new (pInsts + i) T(__VA_ARGS__);                                                                                              \
}                                                                                                                                \
                                                                                                                                 \
return pInsts;


namespace CX
{

class CX_API Mem : public IObject
{
public:

	static void *Alloc(Size cbSize);

	static void *Realloc(void *pPtr, Size cbSize);

	static void Free(void *pPtr);

	template <typename T>
	static T *New()
	{
		CX_MEM_NEW_HELPER();
	}

	template <typename T, typename P1>
	static T *New(P1 p1)
	{
		CX_MEM_NEW_HELPER(p1);
	}

	template <typename T, typename P1, typename P2>
	static T *New(P1 p1, P2 p2)
	{
		CX_MEM_NEW_HELPER(p1, p2);
	}

	template <typename T, typename P1, typename P2, typename P3>
	static T *New(P1 p1, P2 p2, P3 p3)
	{
		CX_MEM_NEW_HELPER(p1, p2, p3);
	}

	template <typename T, typename P1, typename P2, typename P3, typename P4>
	static T *New(P1 p1, P2 p2, P3 p3, P4 p4)
	{
		CX_MEM_NEW_HELPER(p1, p2, p3, p4);
	}

	template <typename T, typename P1, typename P2, typename P3, typename P4, typename P5>
	static T *New(P1 p1, P2 p2, P3 p3, P4 p4, P5 p5)
	{
		CX_MEM_NEW_HELPER(p1, p2, p3, p4, p5);
	}

	template <typename T, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6>
	static T *New(P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6)
	{
		CX_MEM_NEW_HELPER(p1, p2, p3, p4, p5, p6);
	}

	template <typename T, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7>
	static T *New(P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6, P7 p7)
	{
		CX_MEM_NEW_HELPER(p1, p2, p3, p4, p5, p6, p7);
	}

	template <typename T, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7, typename P8>
	static T *New(P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6, P7 p7, P8 p8)
	{
		CX_MEM_NEW_HELPER(p1, p2, p3, p4, p5, p6, p7, p8);
	}

	template <typename T, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7, typename P8,
		typename P9>
		static T *New(P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6, P7 p7, P8 p8, P9 p9)
	{
		CX_MEM_NEW_HELPER(p1, p2, p3, p4, p5, p6, p7, p8, p9);
	}

	template <typename T, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7, typename P8,
		typename P9, typename P10>
		static T *New(P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6, P7 p7, P8 p8, P9 p9, P10 p10)
	{
		CX_MEM_NEW_HELPER(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10);
	}

	template <typename T, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7, typename P8,
		typename P9, typename P10, typename P11>
		static T *New(P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6, P7 p7, P8 p8, P9 p9, P10 p10, P11 p11)
	{
		CX_MEM_NEW_HELPER(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11);
	}

	template <typename T, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7, typename P8,
		typename P9, typename P10, typename P11, typename P12>
		static T *New(P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6, P7 p7, P8 p8, P9 p9, P10 p10, P11 p11, P12 p12)
	{
		CX_MEM_NEW_HELPER(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12);
	}

	template <typename T, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7, typename P8,
		typename P9, typename P10, typename P11, typename P12, typename P13>
		static T *New(P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6, P7 p7, P8 p8, P9 p9, P10 p10, P11 p11, P12 p12, P13 p13)
	{
		CX_MEM_NEW_HELPER(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13);
	}

	template <typename T, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7, typename P8,
		typename P9, typename P10, typename P11, typename P12, typename P13, typename P14>
		static T *New(P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6, P7 p7, P8 p8, P9 p9, P10 p10, P11 p11, P12 p12, P13 p13, P14 p14)
	{
		CX_MEM_NEW_HELPER(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14);
	}

	template <typename T, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7, typename P8,
		typename P9, typename P10, typename P11, typename P12, typename P13, typename P14, typename P15>
		static T *New(P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6, P7 p7, P8 p8, P9 p9, P10 p10, P11 p11, P12 p12, P13 p13, P14 p14,
		P15 p15)
	{
		CX_MEM_NEW_HELPER(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15);
	}

	template <typename T, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7, typename P8,
		typename P9, typename P10, typename P11, typename P12, typename P13, typename P14, typename P15, typename P16>
		static T *New(P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6, P7 p7, P8 p8, P9 p9, P10 p10, P11 p11, P12 p12, P13 p13, P14 p14,
		P15 p15, P16 p16)
	{
		CX_MEM_NEW_HELPER(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16);
	}

	template <typename T>
	static void Delete(T *pInst)
	{
		pInst->~T();
		Mem::Free(pInst);
	}

	template <typename T>
	static T *NewArr(Size cCount)
	{
		CX_MEM_NEWARR_HELPER();
	}

	template <typename T, typename P1>
	static T *NewArr(Size cCount, P1 p1)
	{
		CX_MEM_NEWARR_HELPER(p1);
	}

	template <typename T, typename P1, typename P2>
	static T *NewArr(Size cCount, P1 p1, P2 p2)
	{
		CX_MEM_NEWARR_HELPER(p1, p2);
	}

	template <typename T, typename P1, typename P2, typename P3>
	static T *NewArr(Size cCount, P1 p1, P2 p2, P3 p3)
	{
		CX_MEM_NEWARR_HELPER(p1, p2, p3);
	}

	template <typename T, typename P1, typename P2, typename P3, typename P4>
	static T *NewArr(Size cCount, P1 p1, P2 p2, P3 p3, P4 p4)
	{
		CX_MEM_NEWARR_HELPER(p1, p2, p3, p4);
	}

	template <typename T, typename P1, typename P2, typename P3, typename P4, typename P5>
	static T *NewArr(Size cCount, P1 p1, P2 p2, P3 p3, P4 p4, P5 p5)
	{
		CX_MEM_NEWARR_HELPER(p1, p2, p3, p4, p5);
	}

	template <typename T, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6>
	static T *NewArr(Size cCount, P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6)
	{
		CX_MEM_NEWARR_HELPER(p1, p2, p3, p4, p5, p6);
	}

	template <typename T, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7>
	static T *NewArr(Size cCount, P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6, P7 p7)
	{
		CX_MEM_NEWARR_HELPER(p1, p2, p3, p4, p5, p6, p7);
	}

	template <typename T, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7, typename P8>
	static T *NewArr(Size cCount, P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6, P7 p7, P8 p8)
	{
		CX_MEM_NEWARR_HELPER(p1, p2, p3, p4, p5, p6, p7, p8);
	}

	template <typename T, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7, typename P8,
		typename P9>
		static T *NewArr(Size cCount, P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6, P7 p7, P8 p8, P9 p9)
	{
		CX_MEM_NEWARR_HELPER(p1, p2, p3, p4, p5, p6, p7, p8, p9);
	}

	template <typename T, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7, typename P8,
		typename P9, typename P10>
		static T *NewArr(Size cCount, P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6, P7 p7, P8 p8, P9 p9, P10 p10)
	{
		CX_MEM_NEWARR_HELPER(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10);
	}

	template <typename T, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7, typename P8,
		typename P9, typename P10, typename P11>
		static T *NewArr(Size cCount, P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6, P7 p7, P8 p8, P9 p9, P10 p10, P11 p11)
	{
		CX_MEM_NEWARR_HELPER(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11);
	}

	template <typename T, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7, typename P8,
		typename P9, typename P10, typename P11, typename P12>
		static T *NewArr(Size cCount, P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6, P7 p7, P8 p8, P9 p9, P10 p10, P11 p11, P12 p12)
	{
		CX_MEM_NEWARR_HELPER(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12);
	}

	template <typename T, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7, typename P8,
		typename P9, typename P10, typename P11, typename P12, typename P13>
		static T *NewArr(Size cCount, P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6, P7 p7, P8 p8, P9 p9, P10 p10, P11 p11, P12 p12,
		P13 p13)
	{
		CX_MEM_NEWARR_HELPER(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13);
	}

	template <typename T, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7, typename P8,
		typename P9, typename P10, typename P11, typename P12, typename P13, typename P14>
		static T *NewArr(Size cCount, P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6, P7 p7, P8 p8, P9 p9, P10 p10, P11 p11, P12 p12,
		P13 p13, P14 p14)
	{
		CX_MEM_NEWARR_HELPER(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14);
	}

	template <typename T, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7, typename P8,
		typename P9, typename P10, typename P11, typename P12, typename P13, typename P14, typename P15>
		static T *NewArr(Size cCount, P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6, P7 p7, P8 p8, P9 p9, P10 p10, P11 p11, P12 p12,
		P13 p13, P14 p14, P15 p15)
	{
		CX_MEM_NEWARR_HELPER(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15);
	}

	template <typename T, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7, typename P8,
		typename P9, typename P10, typename P11, typename P12, typename P13, typename P14, typename P15, typename P16>
		static T *NewArr(Size cCount, P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6, P7 p7, P8 p8, P9 p9, P10 p10, P11 p11, P12 p12,
		P13 p13, P14 p14, P15 p15, P16 p16)
	{
		CX_MEM_NEWARR_HELPER(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16);
	}

	template <typename T>
	static void DeleteArr(T *pInsts)
	{
		Byte *pPtr;
		Size cCount;

		pPtr = (Byte *)pInsts - sizeof(Size);
		memcpy(&cCount, pPtr, sizeof(Size));
		for (Size i = 0; i < cCount; i++)
		{
			(pInsts + i)->~T();
		}

		Mem::Free(pPtr);
	}

private:

	Mem();

	~Mem();

};

}//CX 

