/* 
 * CX - C++ framework for general purpose developement
 *
 * https://github.com/draede/cx
 * 
 * Copyright (C) 2014 draede - draede [at] outlook [dot] com
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


#include "CX/APIDefs.h"
#include "CX/Types.h"
#include <new>


namespace CX
{

CX_API void *Alloc(Size cbSize);

CX_API void *Realloc(void *pPtr, Size cbSize);

CX_API void Free(void *pPtr);

template <typename T>
T *New()
{
	Byte *pPtr;

	if (NULL == (pPtr = (Byte *)Alloc(sizeof(T))))
	{
		return NULL;
	}

	return new (pPtr) T;
};

template <typename T, typename T1>
T *New(T1 p1)
{
	Byte *pPtr;

	if (NULL == (pPtr = (Byte *)Alloc(sizeof(T))))
	{
		return NULL;
	}

	return new (pPtr) T(p1);
};

template <typename T, typename T1, typename T2>
T *New(T1 p1, T2 p2)
{
	Byte *pPtr;

	if (NULL == (pPtr = (Byte *)Alloc(sizeof(T))))
	{
		return NULL;
	}

	return new (pPtr)T(p1, p2);
};

template <typename T, typename T1, typename T2, typename T3>
T *New(T1 p1, T2 p2, T3 p3)
{
	Byte *pPtr;

	if (NULL == (pPtr = (Byte *)Alloc(sizeof(T))))
	{
		return NULL;
	}

	return new (pPtr)T(p1, p2, p3);
};

template <typename T, typename T1, typename T2, typename T3, typename T4>
T *New(T1 p1, T2 p2, T3 p3, T4 p4)
{
	Byte *pPtr;

	if (NULL == (pPtr = (Byte *)Alloc(sizeof(T))))
	{
		return NULL;
	}

	return new (pPtr)T(p1, p2, p3, p4);
};

template <typename T, typename T1, typename T2, typename T3, typename T4, typename T5>
T *New(T1 p1, T2 p2, T3 p3, T4 p4, T5 p5)
{
	Byte *pPtr;

	if (NULL == (pPtr = (Byte *)Alloc(sizeof(T))))
	{
		return NULL;
	}

	return new (pPtr)T(p1, p2, p3, p4, p5);
};

template <typename T, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6>
T *New(T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6)
{
	Byte *pPtr;

	if (NULL == (pPtr = (Byte *)Alloc(sizeof(T))))
	{
		return NULL;
	}

	return new (pPtr)T(p1, p2, p3, p4, p5, p6);
};

template <typename T, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, 
          typename T7>
T *New(T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7)
{
	Byte *pPtr;

	if (NULL == (pPtr = (Byte *)Alloc(sizeof(T))))
	{
		return NULL;
	}

	return new (pPtr)T(p1, p2, p3, p4, p5, p6, p7);
};

template <typename T, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, 
          typename T7, typename T8>
T *New(T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, T8 p8)
{
	Byte *pPtr;

	if (NULL == (pPtr = (Byte *)Alloc(sizeof(T))))
	{
		return NULL;
	}

	return new (pPtr)T(p1, p2, p3, p4, p5, p6, p7, p8);
};

template <typename T, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, 
          typename T7, typename T8, typename T9>
T *New(T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, T8 p8, T9 p9)
{
	Byte *pPtr;

	if (NULL == (pPtr = (Byte *)Alloc(sizeof(T))))
	{
		return NULL;
	}

	return new (pPtr)T(p1, p2, p3, p4, p5, p6, p7, p8, p9);
};

template <typename T, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, 
          typename T7, typename T8, typename T9, typename T10>
T *New(T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, T8 p8, T9 p9, T10 p10)
{
	Byte *pPtr;

	if (NULL == (pPtr = (Byte *)Alloc(sizeof(T))))
	{
		return NULL;
	}

	return new (pPtr)T(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10);
};

template <typename T, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, 
          typename T7, typename T8, typename T9, typename T10, typename T11>
T *New(T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, T8 p8, T9 p9, T10 p10, T11 p11)
{
	Byte *pPtr;

	if (NULL == (pPtr = (Byte *)Alloc(sizeof(T))))
	{
		return NULL;
	}

	return new (pPtr)T(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11);
};

template <typename T, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, 
          typename T7, typename T8, typename T9, typename T10, typename T11, typename T12>
T *New(T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, T8 p8, T9 p9, T10 p10, T11 p11, T12 p12)
{
	Byte *pPtr;

	if (NULL == (pPtr = (Byte *)Alloc(sizeof(T))))
	{
		return NULL;
	}

	return new (pPtr)T(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12);
};

template <typename T, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6,
          typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, 
          typename T13>
T *New(T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, T8 p8, T9 p9, T10 p10, T11 p11, T12 p12, 
       T13 p13)
{
	Byte *pPtr;

	if (NULL == (pPtr = (Byte *)Alloc(sizeof(T))))
	{
		return NULL;
	}

	return new (pPtr)T(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13);
};

template <typename T, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, 
          typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, 
          typename T13, typename T14>
T *New(T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, T8 p8, T9 p9, T10 p10, T11 p11, T12 p12, 
       T13 p13, T14 p14)
{
	Byte *pPtr;

	if (NULL == (pPtr = (Byte *)Alloc(sizeof(T))))
	{
		return NULL;
	}

	return new (pPtr)T(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14);
};

template <typename T, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, 
          typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, 
          typename T13, typename T14, typename T15>
T *New(T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, T8 p8, T9 p9, T10 p10, T11 p11, T12 p12, 
       T13 p13, T14 p14, T15 p15)
{
	Byte *pPtr;

	if (NULL == (pPtr = (Byte *)Alloc(sizeof(T))))
	{
		return NULL;
	}

	return new (pPtr)T(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15);
};

template <typename T, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, 
          typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, 
          typename T13, typename T14, typename T15, typename T16>
T *New(T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, T8 p8, T9 p9, T10 p10, T11 p11, T12 p12, 
       T13 p13, T14 p14, T15 p15, T16 p16)
{
	Byte *pPtr;

	if (NULL == (pPtr = (Byte *)Alloc(sizeof(T))))
	{
		return NULL;
	}

	return new (pPtr)T(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16);
};

template <typename T, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, 
          typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, 
          typename T13, typename T14, typename T15, typename T16, typename T17>
T *New(T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, T8 p8, T9 p9, T10 p10, T11 p11, T12 p12, 
       T13 p13, T14 p14, T15 p15, T16 p16, T17 p17)
{
	Byte *pPtr;

	if (NULL == (pPtr = (Byte *)Alloc(sizeof(T))))
	{
		return NULL;
	}

	return new (pPtr)T(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17);
};

template <typename T, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, 
          typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, 
          typename T13, typename T14, typename T15, typename T16, typename T17, typename T18>
T *New(T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, T8 p8, T9 p9, T10 p10, T11 p11, T12 p12, 
       T13 p13, T14 p14, T15 p15, T16 p16, T17 p17, T18 p18)
{
	Byte *pPtr;

	if (NULL == (pPtr = (Byte *)Alloc(sizeof(T))))
	{
		return NULL;
	}

	return new (pPtr)T(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, 
	                   p18);
};

template <typename T, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, 
          typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, 
          typename T13, typename T14, typename T15, typename T16, typename T17, typename T18, 
          typename T19>
T *New(T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, T8 p8, T9 p9, T10 p10, T11 p11, T12 p12, 
       T13 p13, T14 p14, T15 p15, T16 p16, T17 p17, T18 p18, T19 p19)
{
	Byte *pPtr;

	if (NULL == (pPtr = (Byte *)Alloc(sizeof(T))))
	{
		return NULL;
	}

	return new (pPtr)T(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, 
	                   p18, p19);
};

template <typename T, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, 
          typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, 
          typename T13, typename T14, typename T15, typename T16, typename T17, typename T18, 
          typename T19, typename T20>
T *New(T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, T8 p8, T9 p9, T10 p10, T11 p11, T12 p12, 
       T13 p13, T14 p14, T15 p15, T16 p16, T17 p17, T18 p18, T19 p19, T20 p20)
{
	Byte *pPtr;

	if (NULL == (pPtr = (Byte *)Alloc(sizeof(T))))
	{
		return NULL;
	}

	return new (pPtr)T(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, 
	                   p18, p19, p20);
};

template <typename T>
void Delete(T *pT)
{
	if (NULL != pT)
	{
		pT->~T();
		Free(pT);
	}
}

template <typename T>
T *NewArr(Size cCount)
{
	Byte *pPtr;

	if (NULL == (pPtr = (Byte *)Alloc(sizeof(Size) + sizeof(T) * cCount)))
	{
		return NULL;
	}

	Byte *pPosPtr = pPtr;

	memcpy(pPosPtr, &cCount, sizeof(Size));
	pPosPtr += sizeof(Size);

	Byte *pFinalPtr = pPosPtr;

	for (Size i = 0; i < cCount; i++)
	{
		new (pPosPtr)T();
		pPosPtr += sizeof(T);
	}

	return (T *)pFinalPtr;
};

template <typename T, typename T1>
T *NewArr(Size cCount, T1 p1)
{
	Byte *pPtr;

	if (NULL == (pPtr = (Byte *)Alloc(sizeof(Size) + sizeof(T) * cCount)))
	{
		return NULL;
	}

	Byte *pPosPtr = pPtr;

	memcpy(pPosPtr, &cCount, sizeof(Size));
	pPosPtr += sizeof(Size);

	Byte *pFinalPtr = pPosPtr;

	for (Size i = 0; i < cCount; i++)
	{
		new (pPosPtr)T(p1);
		pPosPtr += sizeof(T);
	}

	return (T *)pFinalPtr;
};

template <typename T, typename T1, typename T2>
T *NewArr(Size cCount, T1 p1, T2 p2)
{
	Byte *pPtr;

	if (NULL == (pPtr = (Byte *)Alloc(sizeof(Size) + sizeof(T) * cCount)))
	{
		return NULL;
	}

	Byte *pPosPtr = pPtr;

	memcpy(pPosPtr, &cCount, sizeof(Size));
	pPosPtr += sizeof(Size);

	Byte *pFinalPtr = pPosPtr;

	for (Size i = 0; i < cCount; i++)
	{
		new (pPosPtr)T(p1, p2);
		pPosPtr += sizeof(T);
	}

	return (T *)pFinalPtr;
};

template <typename T, typename T1, typename T2, typename T3>
T *NewArr(Size cCount, T1 p1, T2 p2, T3 p3)
{
	Byte *pPtr;

	if (NULL == (pPtr = (Byte *)Alloc(sizeof(Size) + sizeof(T) * cCount)))
	{
		return NULL;
	}

	Byte *pPosPtr = pPtr;

	memcpy(pPosPtr, &cCount, sizeof(Size));
	pPosPtr += sizeof(Size);

	Byte *pFinalPtr = pPosPtr;

	for (Size i = 0; i < cCount; i++)
	{
		new (pPosPtr)T(p1, p2, p3);
		pPosPtr += sizeof(T);
	}

	return (T *)pFinalPtr;
};

template <typename T, typename T1, typename T2, typename T3, typename T4>
T *NewArr(Size cCount, T1 p1, T2 p2, T3 p3, T4 p4)
{
	Byte *pPtr;

	if (NULL == (pPtr = (Byte *)Alloc(sizeof(Size) + sizeof(T) * cCount)))
	{
		return NULL;
	}

	Byte *pPosPtr = pPtr;

	memcpy(pPosPtr, &cCount, sizeof(Size));
	pPosPtr += sizeof(Size);

	Byte *pFinalPtr = pPosPtr;

	for (Size i = 0; i < cCount; i++)
	{
		new (pPosPtr)T(p1, p2, p3, p4);
		pPosPtr += sizeof(T);
	}

	return (T *)pFinalPtr;
};

template <typename T, typename T1, typename T2, typename T3, typename T4, typename T5>
T *NewArr(Size cCount, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5)
{
	Byte *pPtr;

	if (NULL == (pPtr = (Byte *)Alloc(sizeof(Size) + sizeof(T) * cCount)))
	{
		return NULL;
	}

	Byte *pPosPtr = pPtr;

	memcpy(pPosPtr, &cCount, sizeof(Size));
	pPosPtr += sizeof(Size);

	Byte *pFinalPtr = pPosPtr;

	for (Size i = 0; i < cCount; i++)
	{
		new (pPosPtr)T(p1, p2, p3, p4, p5);
		pPosPtr += sizeof(T);
	}

	return (T *)pFinalPtr;
};

template <typename T, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6>
T *NewArr(Size cCount, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6)
{
	Byte *pPtr;

	if (NULL == (pPtr = (Byte *)Alloc(sizeof(Size) + sizeof(T) * cCount)))
	{
		return NULL;
	}

	Byte *pPosPtr = pPtr;

	memcpy(pPosPtr, &cCount, sizeof(Size));
	pPosPtr += sizeof(Size);

	Byte *pFinalPtr = pPosPtr;

	for (Size i = 0; i < cCount; i++)
	{
		new (pPosPtr)T(p1, p2, p3, p4, p5, p6);
		pPosPtr += sizeof(T);
	}

	return (T *)pFinalPtr;
};

template <typename T, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, 
          typename T7>
T *NewArr(Size cCount, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7)
{
	Byte *pPtr;

	if (NULL == (pPtr = (Byte *)Alloc(sizeof(Size) + sizeof(T) * cCount)))
	{
		return NULL;
	}

	Byte *pPosPtr = pPtr;

	memcpy(pPosPtr, &cCount, sizeof(Size));
	pPosPtr += sizeof(Size);

	Byte *pFinalPtr = pPosPtr;

	for (Size i = 0; i < cCount; i++)
	{
		new (pPosPtr)T(p1, p2, p3, p4, p5, p6, p7);
		pPosPtr += sizeof(T);
	}

	return (T *)pFinalPtr;
};

template <typename T, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, 
          typename T7, typename T8>
T *NewArr(Size cCount, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, T8 p8)
{
	Byte *pPtr;

	if (NULL == (pPtr = (Byte *)Alloc(sizeof(Size) + sizeof(T) * cCount)))
	{
		return NULL;
	}

	Byte *pPosPtr = pPtr;

	memcpy(pPosPtr, &cCount, sizeof(Size));
	pPosPtr += sizeof(Size);

	Byte *pFinalPtr = pPosPtr;

	for (Size i = 0; i < cCount; i++)
	{
		new (pPosPtr)T(p1, p2, p3, p4, p5, p6, p7, p8);
		pPosPtr += sizeof(T);
	}

	return (T *)pFinalPtr;
};

template <typename T, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, 
          typename T7, typename T8, typename T9>
T *NewArr(Size cCount, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, T8 p8, T9 p9)
{
	Byte *pPtr;

	if (NULL == (pPtr = (Byte *)Alloc(sizeof(Size) + sizeof(T) * cCount)))
	{
		return NULL;
	}

	Byte *pPosPtr = pPtr;

	memcpy(pPosPtr, &cCount, sizeof(Size));
	pPosPtr += sizeof(Size);

	Byte *pFinalPtr = pPosPtr;

	for (Size i = 0; i < cCount; i++)
	{
		new (pPosPtr)T(p1, p2, p3, p4, p5, p6, p7, p8, p9);
		pPosPtr += sizeof(T);
	}

	return (T *)pFinalPtr;
};

template <typename T, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, 
          typename T7, typename T8, typename T9, typename T10>
T *NewArr(Size cCount, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, T8 p8, T9 p9, T10 p10)
{
	Byte *pPtr;

	if (NULL == (pPtr = (Byte *)Alloc(sizeof(Size) + sizeof(T) * cCount)))
	{
		return NULL;
	}

	Byte *pPosPtr = pPtr;

	memcpy(pPosPtr, &cCount, sizeof(Size));
	pPosPtr += sizeof(Size);

	Byte *pFinalPtr = pPosPtr;

	for (Size i = 0; i < cCount; i++)
	{
		new (pPosPtr)T(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10);
		pPosPtr += sizeof(T);
	}

	return (T *)pFinalPtr;
};

template <typename T, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, 
          typename T7, typename T8, typename T9, typename T10, typename T11>
T *NewArr(Size cCount, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, T8 p8, T9 p9, T10 p10, 
          T11 p11)
{
	Byte *pPtr;

	if (NULL == (pPtr = (Byte *)Alloc(sizeof(Size) + sizeof(T) * cCount)))
	{
		return NULL;
	}

	Byte *pPosPtr = pPtr;

	memcpy(pPosPtr, &cCount, sizeof(Size));
	pPosPtr += sizeof(Size);

	Byte *pFinalPtr = pPosPtr;

	for (Size i = 0; i < cCount; i++)
	{
		new (pPosPtr)T(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11);
		pPosPtr += sizeof(T);
	}

	return (T *)pFinalPtr;
};

template <typename T, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, 
          typename T7, typename T8, typename T9, typename T10, typename T11, typename T12>
T *NewArr(Size cCount, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, T8 p8, T9 p9, T10 p10, 
          T11 p11, T12 p12)
{
	Byte *pPtr;

	if (NULL == (pPtr = (Byte *)Alloc(sizeof(Size) + sizeof(T) * cCount)))
	{
		return NULL;
	}

	Byte *pPosPtr = pPtr;

	memcpy(pPosPtr, &cCount, sizeof(Size));
	pPosPtr += sizeof(Size);

	Byte *pFinalPtr = pPosPtr;

	for (Size i = 0; i < cCount; i++)
	{
		new (pPosPtr)T(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12);
		pPosPtr += sizeof(T);
	}

	return (T *)pFinalPtr;
};

template <typename T, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, 
          typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13>
T *NewArr(Size cCount, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, T8 p8, T9 p9, T10 p10, 
          T11 p11, T12 p12, T13 p13)
{
	Byte *pPtr;

	if (NULL == (pPtr = (Byte *)Alloc(sizeof(Size) + sizeof(T) * cCount)))
	{
		return NULL;
	}

	Byte *pPosPtr = pPtr;

	memcpy(pPosPtr, &cCount, sizeof(Size));
	pPosPtr += sizeof(Size);

	Byte *pFinalPtr = pPosPtr;

	for (Size i = 0; i < cCount; i++)
	{
		new (pPosPtr)T(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13);
		pPosPtr += sizeof(T);
	}

	return (T *)pFinalPtr;
};

template <typename T, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, 
          typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, 
          typename T13, typename T14>
T *NewArr(Size cCount, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, T8 p8, T9 p9, T10 p10, 
          T11 p11, T12 p12, T13 p13, T14 p14)
{
	Byte *pPtr;

	if (NULL == (pPtr = (Byte *)Alloc(sizeof(Size) + sizeof(T) * cCount)))
	{
		return NULL;
	}

	Byte *pPosPtr = pPtr;

	memcpy(pPosPtr, &cCount, sizeof(Size));
	pPosPtr += sizeof(Size);

	Byte *pFinalPtr = pPosPtr;

	for (Size i = 0; i < cCount; i++)
	{
		new (pPosPtr)T(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14);
		pPosPtr += sizeof(T);
	}

	return (T *)pFinalPtr;
};

template <typename T, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, 
          typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, 
          typename T13, typename T14, typename T15>
T *NewArr(Size cCount, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, T8 p8, T9 p9, T10 p10, 
          T11 p11, T12 p12, T13 p13, T14 p14, T15 p15)
{
	Byte *pPtr;

	if (NULL == (pPtr = (Byte *)Alloc(sizeof(Size) + sizeof(T) * cCount)))
	{
		return NULL;
	}

	Byte *pPosPtr = pPtr;

	memcpy(pPosPtr, &cCount, sizeof(Size));
	pPosPtr += sizeof(Size);

	Byte *pFinalPtr = pPosPtr;

	for (Size i = 0; i < cCount; i++)
	{
		new (pPosPtr)T(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15);
		pPosPtr += sizeof(T);
	}

	return (T *)pFinalPtr;
};

template <typename T, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, 
          typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, 
          typename T13, typename T14, typename T15, typename T16>
T *NewArr(Size cCount, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, T8 p8, T9 p9, T10 p10, 
          T11 p11, T12 p12, T13 p13, T14 p14, T15 p15, T16 p16)
{
	Byte *pPtr;

	if (NULL == (pPtr = (Byte *)Alloc(sizeof(Size) + sizeof(T) * cCount)))
	{
		return NULL;
	}

	Byte *pPosPtr = pPtr;

	memcpy(pPosPtr, &cCount, sizeof(Size));
	pPosPtr += sizeof(Size);

	Byte *pFinalPtr = pPosPtr;

	for (Size i = 0; i < cCount; i++)
	{
		new (pPosPtr)T(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16);
		pPosPtr += sizeof(T);
	}

	return (T *)pFinalPtr;
};

template <typename T, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, 
          typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, 
          typename T13, typename T14, typename T15, typename T16, typename T17>
T *NewArr(Size cCount, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, T8 p8, T9 p9, T10 p10, 
          T11 p11, T12 p12, T13 p13, T14 p14, T15 p15, T16 p16, T17 p17)
{
	Byte *pPtr;

	if (NULL == (pPtr = (Byte *)Alloc(sizeof(Size) + sizeof(T) * cCount)))
	{
		return NULL;
	}

	Byte *pPosPtr = pPtr;

	memcpy(pPosPtr, &cCount, sizeof(Size));
	pPosPtr += sizeof(Size);

	Byte *pFinalPtr = pPosPtr;

	for (Size i = 0; i < cCount; i++)
	{
		new (pPosPtr)T(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17);
		pPosPtr += sizeof(T);
	}

	return (T *)pFinalPtr;
};

template <typename T, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, 
          typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, 
          typename T13, typename T14, typename T15, typename T16, typename T17, typename T18>
T *NewArr(Size cCount, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, T8 p8, T9 p9, T10 p10, 
          T11 p11, T12 p12, T13 p13, T14 p14, T15 p15, T16 p16, T17 p17, T18 p18)
{
	Byte *pPtr;

	if (NULL == (pPtr = (Byte *)Alloc(sizeof(Size) + sizeof(T) * cCount)))
	{
		return NULL;
	}

	Byte *pPosPtr = pPtr;

	memcpy(pPosPtr, &cCount, sizeof(Size));
	pPosPtr += sizeof(Size);

	Byte *pFinalPtr = pPosPtr;

	for (Size i = 0; i < cCount; i++)
	{
		new (pPosPtr)T(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, 
		               p18);
		pPosPtr += sizeof(T);
	}

	return (T *)pFinalPtr;
};

template <typename T, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, 
          typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, 
          typename T13, typename T14, typename T15, typename T16, typename T17, typename T18, 
          typename T19>
T *NewArr(Size cCount, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, T8 p8, T9 p9, T10 p10, 
          T11 p11, T12 p12, T13 p13, T14 p14, T15 p15, T16 p16, T17 p17, T18 p18, T19 p19)
{
	Byte *pPtr;

	if (NULL == (pPtr = (Byte *)Alloc(sizeof(Size) + sizeof(T) * cCount)))
	{
		return NULL;
	}

	Byte *pPosPtr = pPtr;

	memcpy(pPosPtr, &cCount, sizeof(Size));
	pPosPtr += sizeof(Size);

	Byte *pFinalPtr = pPosPtr;

	for (Size i = 0; i < cCount; i++)
	{
		new (pPosPtr)T(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, 
		               p18, p19);
		pPosPtr += sizeof(T);
	}

	return (T *)pFinalPtr;
};

template <typename T, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, 
          typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, 
          typename T13, typename T14, typename T15, typename T16, typename T17, typename T18, 
          typename T19, typename T20>
T *NewArr(Size cCount, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, T8 p8, T9 p9, T10 p10, 
          T11 p11, T12 p12, T13 p13, T14 p14, T15 p15, T16 p16, T17 p17, T18 p18, T19 p19, T20 p20)
{
	Byte *pPtr;

	if (NULL == (pPtr = (Byte *)Alloc(sizeof(Size) + sizeof(T) * cCount)))
	{
		return NULL;
	}

	Byte *pPosPtr = pPtr;

	memcpy(pPosPtr, &cCount, sizeof(Size));
	pPosPtr += sizeof(Size);

	Byte *pFinalPtr = pPosPtr;

	for (Size i = 0; i < cCount; i++)
	{
		new (pPosPtr)T(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, 
		               p18, p19, p20);
		pPosPtr += sizeof(T);
	}

	return (T *)pFinalPtr;
};

template <typename T>
void DeleteArr(T *pT)
{
	if (NULL != pT)
	{
		Byte *pPosPtr = (Byte *)pT - sizeof(Size);
		Byte *pInitialPtr = pPosPtr;
		Size cCount;

		memcpy(&cCount, pPosPtr, sizeof(Size));
		pPosPtr += sizeof(Size);
		for (Size i = 0; i < cCount; i++)
		{
			((T *)pPosPtr)->~T();
			pPosPtr += sizeof(T);
		}
		Free(pInitialPtr);
	}
}

}//CX_API 

