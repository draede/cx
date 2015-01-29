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
#include "CX/Platform.h"
#include "CX/String.h"
#include "CX/Print.h"
#include "CX/Slice.h"
#include "CX/StatusCodes.h"


namespace CX
{

class CX_API Status
{
public:

	static const Size    MAX_MSG_LEN = 1024;

	static StatusCode Set(StatusCode nStatus);

	static StatusCode Set(StatusCode nStatus, const Char *szMsg);

	template <typename T1>
	static inline StatusCode Set(StatusCode nStatus, const Char *szFormat, T1 p1)
	{
		Slice slice(GetMsgPtr(), GetMsgSize());

		if (CXNOK(Print(&slice, szFormat, p1)))
		{
			return GetCode();
		}

		return SetOnlyCode(nStatus);
	}

	template <typename T1, typename T2>
	static inline StatusCode Set(StatusCode nStatus, const Char *szFormat, T1 p1, T2 p2)
	{
		Slice slice(GetMsgPtr(), GetMsgSize());

		return Print(&slice, szFormat, p1, p2);
	}

	template <typename T1, typename T2, typename T3>
	static inline StatusCode Set(StatusCode nStatus, const Char *szFormat, T1 p1, T2 p2, T3 p3)
	{
		Slice slice(GetMsgPtr(), GetMsgSize());

		return Print(&slice, szFormat, p1, p2, p3);
	}

	template <typename T1, typename T2, typename T3, typename T4>
	static inline StatusCode Set(StatusCode nStatus, const Char *szFormat, T1 p1, T2 p2, T3 p3, 
	                             T4 p4)
	{
		Slice slice(GetMsgPtr(), GetMsgSize());

		if (CXNOK(Print(&slice, szFormat, p1, p2, p3, p4)))
		{
			return GetCode();
		}

		return SetOnlyCode(nStatus);
	}

	template <typename T1, typename T2, typename T3, typename T4, typename T5>
	static inline StatusCode Set(StatusCode nStatus, const Char *szFormat, T1 p1, T2 p2, T3 p3, 
	                             T4 p4, T5 p5)
	{
		Slice slice(GetMsgPtr(), GetMsgSize());

		if (CXNOK(Print(&slice, szFormat, p1, p2, p3, p4, p5)))
		{
			return GetCode();
		}

		return SetOnlyCode(nStatus);
	}

	template <typename T1, typename T2, typename T3, typename T4, typename T5, typename T6>
	static inline StatusCode Set(StatusCode nStatus, const Char *szFormat, T1 p1, T2 p2, T3 p3, 
	                             T4 p4, T5 p5, T6 p6)
	{
		Slice slice(GetMsgPtr(), GetMsgSize());

		if (CXNOK(Print(&slice, szFormat, p1, p2, p3, p4, p5, p6)))
		{
			return GetCode();
		}

		return SetOnlyCode(nStatus);
	}

	template <typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, 
	          typename T7>
	static inline StatusCode Set(StatusCode nStatus, const Char *szFormat, T1 p1, T2 p2, T3 p3, 
	                             T4 p4, T5 p5, T6 p6, T7 p7)
	{
		Slice slice(GetMsgPtr(), GetMsgSize());

		if (CXNOK(Print(&slice, szFormat, p1, p2, p3, p4, p5, p6, p7)))
		{
			return GetCode();
		}

		return SetOnlyCode(nStatus);
	}

	template <typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, 
	          typename T7, typename T8>
	static inline StatusCode Set(StatusCode nStatus, const Char *szFormat, T1 p1, T2 p2, T3 p3, 
	                             T4 p4, T5 p5, T6 p6, T7 p7, T8 p8)
	{
		Slice slice(GetMsgPtr(), GetMsgSize());

		if (CXNOK(Print(&slice, szFormat, p1, p2, p3, p4, p5, p6, p7, p8)))
		{
			return GetCode();
		}

		return SetOnlyCode(nStatus);
	}

	template <typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, 
	          typename T7, typename T8, typename T9>
	static inline StatusCode Set(StatusCode nStatus, const Char *szFormat, T1 p1, T2 p2, T3 p3, 
	                             T4 p4, T5 p5, T6 p6, T7 p7, T8 p8, T9 p9)
	{
		Slice slice(GetMsgPtr(), GetMsgSize());

		if (CXNOK(Print(&slice, szFormat, p1, p2, p3, p4, p5, p6, p7, p8, p9)))
		{
			return GetCode();
		}

		return SetOnlyCode(nStatus);
	}

	template <typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, 
	          typename T7, typename T8, typename T9, typename T10>
	static inline StatusCode Set(StatusCode nStatus, const Char *szFormat, T1 p1, T2 p2, T3 p3, 
	                             T4 p4, T5 p5, T6 p6, T7 p7, T8 p8, T9 p9, T10 p10)
	{
		Slice slice(GetMsgPtr(), GetMsgSize());

		if (CXNOK(Print(&slice, szFormat, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)))
		{
			return GetCode();
		}

		return SetOnlyCode(nStatus);
	}

	template <typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, 
	          typename T7, typename T8, typename T9, typename T10, typename T11>
	static inline StatusCode Set(StatusCode nStatus, const Char *szFormat, T1 p1, T2 p2, T3 p3, 
	                             T4 p4, T5 p5, T6 p6, T7 p7, T8 p8, T9 p9, T10 p10, T11 p11)
	{
		Slice slice(GetMsgPtr(), GetMsgSize());

		if (CXNOK(Print(&slice, szFormat, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11)))
		{
			return GetCode();
		}

		return SetOnlyCode(nStatus);
	}

	template <typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, 
	          typename T7, typename T8, typename T9, typename T10, typename T11, typename T12>
	static inline StatusCode Set(StatusCode nStatus, const Char *szFormat, T1 p1, T2 p2, T3 p3, 
	                             T4 p4, T5 p5, T6 p6, T7 p7, T8 p8, T9 p9, T10 p10, T11 p11, T12 p12)
	{
		Slice slice(GetMsgPtr(), GetMsgSize());

		if (CXNOK(Print(&slice, szFormat, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12)))
		{
			return GetCode();
		}

		return SetOnlyCode(nStatus);
	}

	template <typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, 
	          typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, 
	          typename T13>
	static inline StatusCode Set(StatusCode nStatus, const Char *szFormat, T1 p1, T2 p2, T3 p3, 
	                             T4 p4, T5 p5, T6 p6, T7 p7, T8 p8, T9 p9, T10 p10, T11 p11, 
	                             T12 p12, T13 p13)
	{
		Slice slice(GetMsgPtr(), GetMsgSize());

		if (CXNOK(Print(&slice, szFormat, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13)))
		{
			return GetCode();
		}

		return SetOnlyCode(nStatus);
	}

	template <typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, 
	          typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, 
	          typename T13, typename T14>
	static inline StatusCode Set(StatusCode nStatus, const Char *szFormat, T1 p1, T2 p2, T3 p3, 
	                             T4 p4, T5 p5, T6 p6, T7 p7, T8 p8, T9 p9, T10 p10, T11 p11, 
	                             T12 p12, T13 p13, T14 p14)
	{
		Slice slice(GetMsgPtr(), GetMsgSize());

		if (CXNOK(Print(&slice, szFormat, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, 
		                p14)))
		{
			return GetCode();
		}

		return SetOnlyCode(nStatus);
	}

	template <typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, 
	          typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, 
	          typename T13, typename T14, typename T15>
	static inline StatusCode Set(StatusCode nStatus, const Char *szFormat, T1 p1, T2 p2, T3 p3, 
	                             T4 p4, T5 p5, T6 p6, T7 p7, T8 p8, T9 p9, T10 p10, T11 p11, 
	                             T12 p12, T13 p13, T14 p14, T15 p15)
	{
		Slice slice(GetMsgPtr(), GetMsgSize());

		if (CXNOK(Print(&slice, szFormat, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, 
		                p15)))
		{
			return GetCode();
		}

		return SetOnlyCode(nStatus);
	}

	template <typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, 
	          typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, 
	          typename T13, typename T14, typename T15, typename T16>
	static inline StatusCode Set(StatusCode nStatus, const Char *szFormat, T1 p1, T2 p2, T3 p3, 
	                             T4 p4, T5 p5, T6 p6, T7 p7, T8 p8, T9 p9, T10 p10, T11 p11, 
	                             T12 p12, T13 p13, T14 p14, T15 p15, T16 p16)
	{
		Slice slice(GetMsgPtr(), GetMsgSize());

		if (CXNOK(Print(&slice, szFormat, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, 
		                p15, p16)))
		{
			return GetCode();
		}

		return SetOnlyCode(nStatus);
	}

	static StatusCode GetCode();

	static const Char *GetMsg();

	static void Clear();

	static bool IsOK();

	static bool IsNOK();

private:

	Status();

	~Status();

	static Char *GetMsgPtr();

	static Size GetMsgSize();

	static StatusCode SetOnlyCode(StatusCode nStatus);

};

}//CX_API 

