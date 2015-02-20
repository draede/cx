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

#pragma once


#include "CX/Platform.hpp"


#if defined(CX_OS_WINDOWS)


#include "CX/Types.hpp"
#include "CX/Status.hpp"
#include "CX/APIDefs.hpp"
#include "CX/Util/IFunction.hpp"
#include "CX/Util/Function.hpp"
#include "CX/Platform/Windows/windows.h"


namespace CX
{

namespace Sys
{

typedef UInt64       TID;
typedef UInt64       PID;

class CX_API Thread
{
public:

	Thread();

	virtual ~Thread();

	Status Wait();

	Bool IsRunning();

	TID GetID();

	static PID GetCurrentProcessID();

	static TID GetCurrentThreadID();

	static void Sleep(Size cMilliseconds);

	//the pFunction will be deleted with Delete<pFunction> when done
	template <typename R>
	Status Run(Util::IFunction<R> *pFunction)
	{
		if (NULL != m_hThread)
		{
			return Status(Status_Busy, "Thread already started");
		}

		DWORD      dwID;
		Helper<R>  *pHelper = New<Helper<R> >(pFunction);

		if (NULL != (m_hThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)&ThreadProc, 
		                                      pHelper, 0, &dwID)))
		{
			m_nID = (TID)dwID;

			return Status();
		}
		else
		{
			Delete(pHelper);

			return Status(Status_OpenFailed, "CreateThread failed with error {1}", GetLastError());
		}
	}

	template <typename R>
	void Run(R (*pfnProc)())
	{
		Run(New<Util::Function<R> >(pfnProc));
	}

	template <typename R, typename T1>
	void Run(R (*pfnProc)(T1), T1 p1)
	{
		Run(New<Util::Function<R, T1> >(pfnProc, p1));
	}

	template <typename R, typename T1, typename T2>
	void Run(R (*pfnProc)(T1, T2), T1 p1, T2 p2)
	{
		Run(New<Util::Function<R, T1, T2> >(pfnProc, p1, p2));
	}

	template <typename R, typename T1, typename T2, typename T3>
	void Run(R (*pfnProc)(T1, T2, T3), T1 p1, T2 p2, T3 p3)
	{
		Run(New<Util::Function<R, T1, T2, T3> >(pfnProc, p1, p2, p3));
	}

	template <typename R, typename T1, typename T2, typename T3, typename T4>
	void Run(R (*pfnProc)(T1, T2, T3, T4), T1 p1, T2 p2, T3 p3, T4 p4)
	{
		Run(New<Util::Function<R, T1, T2, T3, T4> >(pfnProc, p1, p2, p3, p4));
	}

	template <typename R, typename T1, typename T2, typename T3, typename T4, typename T5>
	void Run(R (*pfnProc)(T1, T2, T3, T4, T5), T1 p1, T2 p2, T3 p3, T4 p4, T5 p5)
	{
		Run(New<Util::Function<R, T1, T2, T3, T4, T5> >(pfnProc, p1, p2, p3, p4, p5));
	}

	template <typename R, typename T1, typename T2, typename T3, typename T4, typename T5, 
	          typename T6>
	void Run(R (*pfnProc)(T1, T2, T3, T4, T5, T6), T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6)
	{
		Run(New<Util::Function<R, T1, T2, T3, T4, T5, T6> >(pfnProc, p1, p2, p3, p4, p5, p6));
	}

	template <typename R, typename T1, typename T2, typename T3, typename T4, typename T5, 
	          typename T6, typename T7>
	void Run(R (*pfnProc)(T1, T2, T3, T4, T5, T6, T7), T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, 
	         T7 p7)
	{
		Run(New<Util::Function<R, T1, T2, T3, T4, T5, T6, T7> >
			(pfnProc, p1, p2, p3, p4, p5, p6, p7));
	}

	template <typename R, typename T1, typename T2, typename T3, typename T4, typename T5, 
	          typename T6, typename T7, typename T8>
	void Run(R (*pfnProc)(T1, T2, T3, T4, T5, T6, T7, T8), T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, 
	         T7 p7, T8 p8)
	{
		Run(New<Util::Function<R, T1, T2, T3, T4, T5, T6, T7, T8> >
			(pfnProc, p1, p2, p3, p4, p5, p6, p7, p8));
	}

	template <typename R, typename T1, typename T2, typename T3, typename T4, typename T5, 
	          typename T6, typename T7, typename T8, typename T9>
	void Run(R (*pfnProc)(T1, T2, T3, T4, T5, T6, T7, T8, T9), T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, 
	         T6 p6, T7 p7, T8 p8, T9 p9)
	{
		Run(New<Util::Function<R, T1, T2, T3, T4, T5, T6, T7, T8, T9> >
			(pfnProc, p1, p2, p3, p4, p5, p6, p7, p8, p9));
	}

	template <typename R, typename T1, typename T2, typename T3, typename T4, typename T5, 
	          typename T6, typename T7, typename T8, typename T9, typename T10>
	void Run(R (*pfnProc)(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10), T1 p1, T2 p2, T3 p3, T4 p4, T5 
	         p5, T6 p6, T7 p7, T8 p8, T9 p9, T10 p10)
	{
		Run(New<Util::Function<R, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10> >
			(pfnProc, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10));
	}

	template <typename R, typename T1, typename T2, typename T3, typename T4, typename T5, 
	          typename T6, typename T7, typename T8, typename T9, typename T10, typename T11>
	void Run(R (*pfnProc)(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11), T1 p1, T2 p2, T3 p3, T4 p4, 
	         T5 p5, T6 p6, T7 p7, T8 p8, T9 p9, T10 p10, T11 p11)
	{
		Run(New<Util::Function<R, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11> >
			(pfnProc, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11));
	}

	template <typename R, typename T1, typename T2, typename T3, typename T4, typename T5, 
	          typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, 
	          typename T12>
	void Run(R (*pfnProc)(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12), T1 p1, T2 p2, T3 p3, 
	         T4 p4, T5 p5, T6 p6, T7 p7, T8 p8, T9 p9, T10 p10, T11 p11, T12 p12)
	{
		Run(New<Util::Function<R, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12> >
			(pfnProc, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12));
	}

	template <typename R, typename T1, typename T2, typename T3, typename T4, typename T5, 
	          typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, 
	          typename T12, typename T13>
	void Run(R (*pfnProc)(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13), T1 p1, T2 p2, 
	         T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, T8 p8, T9 p9, T10 p10, T11 p11, T12 p12, T13 p13)
	{
		Run(New<Util::Function<R, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13> >
			(pfnProc, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13));
	}

	template <typename R, typename T1, typename T2, typename T3, typename T4, typename T5, 
	          typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, 
	          typename T12, typename T13, typename T14>
	void Run(R (*pfnProc)(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14), T1 p1, T2 
	         p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, T8 p8, T9 p9, T10 p10, T11 p11, T12 p12, T13 
	         p13, T14 p14)
	{
		Run(New<Util::Function<R, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14> >
			(pfnProc, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14));
	}

	template <typename R, typename T1, typename T2, typename T3, typename T4, typename T5, 
	          typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, 
	          typename T12, typename T13, typename T14, typename T15>
	void Run(R (*pfnProc)(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15), T1 p1, 
	         T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, T8 p8, T9 p9, T10 p10, T11 p11, T12 p12, 
	         T13 p13, T14 p14, T15 p15)
	{
		Run(New<Util::Function<R, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, 
		                       T15> >
			(pfnProc, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15));
	}

	template <typename R, typename T1, typename T2, typename T3, typename T4, typename T5, 
	          typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, 
	          typename T12, typename T13, typename T14, typename T15, typename T16>
	void Run(R (*pfnProc)(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16), 
	         T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, T8 p8, T9 p9, T10 p10, T11 p11, 
	         T12 p12, T13 p13, T14 p14, T15 p15, T16 p16)
	{
		Run(New<Util::Function<R, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, 
		                       T15, T16> >
			(pfnProc, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16));
	}

	template <typename T, typename R>
	void Run(T *pInst, R (T::*pfnProc)())
	{
		Run(New<Util::MemberFunction<T, R> >(pInst, pfnProc));
	}

	template <typename T, typename R, typename T1>
	void Run(T *pInst, R (T::*pfnProc)(T1), T1 p1)
	{
		Run(New<Util::MemberFunction<T, R, T1> >(pInst, pfnProc, p1));
	}

	template <typename T, typename R, typename T1, typename T2>
	void Run(T *pInst, R (T::*pfnProc)(T1, T2), T1 p1, T2 p2)
	{
		Run(New<Util::MemberFunction<T, R, T1, T2> >(pInst, pfnProc, p1, p2));
	}

	template <typename T, typename R, typename T1, typename T2, typename T3>
	void Run(T *pInst, R (T::*pfnProc)(T1, T2, T3), T1 p1, T2 p2, T3 p3)
	{
		Run(New<Util::MemberFunction<T, R, T1, T2, T3> >(pInst, pfnProc, p1, p2, p3));
	}

	template <typename T, typename R, typename T1, typename T2, typename T3, typename T4>
	void Run(T *pInst, R (T::*pfnProc)(T1, T2, T3, T4), T1 p1, T2 p2, T3 p3, T4 p4)
	{
		Run(New<Util::MemberFunction<T, R, T1, T2, T3, T4> >(pInst, pfnProc, p1, p2, p3, p4));
	}

	template <typename T, typename R, typename T1, typename T2, typename T3, typename T4, 
	          typename T5>
	void Run(T *pInst, R (T::*pfnProc)(T1, T2, T3, T4, T5), T1 p1, T2 p2, T3 p3, T4 p4, T5 p5)
	{
		Run(New<Util::MemberFunction<T, R, T1, T2, T3, T4, T5> >
		    (pInst, pfnProc, p1, p2, p3, p4, p5));
	}

	template <typename T, typename R, typename T1, typename T2, typename T3, typename T4, 
	          typename T5, typename T6>
	void Run(T *pInst, R (T::*pfnProc)(T1, T2, T3, T4, T5, T6), T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, 
	         T6 p6)
	{
		Run(New<Util::MemberFunction<T, R, T1, T2, T3, T4, T5, T6> >
		    (pInst, pfnProc, p1, p2, p3, p4, p5, p6));
	}

	template <typename T, typename R, typename T1, typename T2, typename T3, typename T4, 
	          typename T5, typename T6, typename T7>
	void Run(T *pInst, R (T::*pfnProc)(T1, T2, T3, T4, T5, T6, T7), T1 p1, T2 p2, T3 p3, T4 p4, 
	         T5 p5, T6 p6, T7 p7)
	{
		Run(New<Util::MemberFunction<T, R, T1, T2, T3, T4, T5, T6, T7> >
		    (pInst, pfnProc, p1, p2, p3, p4, p5, p6, p7));
	}

	template <typename T, typename R, typename T1, typename T2, typename T3, typename T4, 
	          typename T5, typename T6, typename T7, typename T8>
	void Run(T *pInst, R (T::*pfnProc)(T1, T2, T3, T4, T5, T6, T7, T8), T1 p1, T2 p2, T3 p3, T4 p4, 
	         T5 p5, T6 p6, T7 p7, T8 p8)
	{
		Run(New<Util::MemberFunction<T, R, T1, T2, T3, T4, T5, T6, T7, T8> >
		    (pInst, pfnProc, p1, p2, p3, p4, p5, p6, p7, p8));
	}

	template <typename T, typename R, typename T1, typename T2, typename T3, typename T4, 
	          typename T5, typename T6, typename T7, typename T8, typename T9>
	void Run(T *pInst, R (T::*pfnProc)(T1, T2, T3, T4, T5, T6, T7, T8, T9), T1 p1, T2 p2, T3 p3, 
	         T4 p4, T5 p5, T6 p6, T7 p7, T8 p8, T9 p9)
	{
		Run(New<Util::MemberFunction<T, R, T1, T2, T3, T4, T5, T6, T7, T8, T9> >
		    (pInst, pfnProc, p1, p2, p3, p4, p5, p6, p7, p8, p9));
	}

	template <typename T, typename R, typename T1, typename T2, typename T3, typename T4, 
	          typename T5, typename T6, typename T7, typename T8, typename T9, typename T10>
	void Run(T *pInst, R (T::*pfnProc)(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10), T1 p1, T2 p2, 
	         T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, T8 p8, T9 p9, T10 p10)
	{
		Run(New<Util::MemberFunction<T, R, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10> >
		    (pInst, pfnProc, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10));
	}

	template <typename T, typename R, typename T1, typename T2, typename T3, typename T4, 
	          typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, 
	          typename T11>
	void Run(T *pInst, R (T::*pfnProc)(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11), T1 p1, T2 p2, 
	         T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, T8 p8, T9 p9, T10 p10, T11 p11)
	{
		Run(New<Util::MemberFunction<T, R, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11> >
		    (pInst, pfnProc, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11));
	}

	template <typename T, typename R, typename T1, typename T2, typename T3, typename T4, 
	          typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, 
	          typename T11, typename T12>
	void Run(T *pInst, R (T::*pfnProc)(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12), T1 p1, 
	         T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, T8 p8, T9 p9, T10 p10, T11 p11, T12 p12)
	{
		Run(New<Util::MemberFunction<T, R, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12> >
		    (pInst, pfnProc, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12));
	}

	template <typename T, typename R, typename T1, typename T2, typename T3, typename T4, 
	          typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, 
	          typename T11, typename T12, typename T13>
	void Run(T *pInst, R (T::*pfnProc)(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13), 
	         T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, T8 p8, T9 p9, T10 p10, T11 p11, 
	         T12 p12, T13 p13)
	{
		Run(New<Util::MemberFunction<T, R, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, 
		                             T13> >
		    (pInst, pfnProc, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13));
	}

	template <typename T, typename R, typename T1, typename T2, typename T3, typename T4, 
	          typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, 
	          typename T11, typename T12, typename T13, typename T14>
	void Run(T *pInst, R (T::*pfnProc)(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14), 
	         T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, T8 p8, T9 p9, T10 p10, T11 p11, 
	         T12 p12, T13 p13, T14 p14)
	{
		Run(New<Util::MemberFunction<T, R, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, 
		                             T14> >
		    (pInst, pfnProc, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14));
	}

	template <typename T, typename R, typename T1, typename T2, typename T3, typename T4, 
	          typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, 
	          typename T11, typename T12, typename T13, typename T14, typename T15>
	void Run(T *pInst, R (T::*pfnProc)(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, 
	                                T15), 
	         T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, T8 p8, T9 p9, T10 p10, T11 p11, 
	         T12 p12, T13 p13, T14 p14, T15 p15)
	{
		Run(New<Util::MemberFunction<T, R, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, 
		                             T14, T15> >
		    (pInst, pfnProc, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15));
	}

	template <typename T, typename R, typename T1, typename T2, typename T3, typename T4, 
	          typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, 
	          typename T11, typename T12, typename T13, typename T14, typename T15, typename T16>
	void Run(T *pInst, R (T::*pfnProc)(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, 
	                                T15, T16), 
	         T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, T8 p8, T9 p9, T10 p10, T11 p11, 
	         T12 p12, T13 p13, T14 p14, T15 p15, T16 p16)
	{
		Run(New<Util::MemberFunction<T, R, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, 
		                             T14, T15, T16> >
			(pInst, pfnProc, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16));
	}

private:

	HANDLE   m_hThread;
	TID      m_nID;

	class IHelper
	{
	public:

		virtual ~IHelper() { }

		virtual void Run() = 0;

	};
	
	template <typename R>
	class Helper : public IHelper
	{
	public:

		Helper(Util::IFunction<R> *pFunction)
		{
			m_pFunction = pFunction;
		}

		~Helper() 
		{
			Delete(m_pFunction);
		}

		virtual void Run()
		{
			m_pFunction->Run();
		}

	private:

		Util::IFunction<R> *m_pFunction;

	};

	static DWORD ThreadProc(void *pArg);

};

}//namespace Sys

}//namespace CX


#endif

