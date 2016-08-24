/* 
 * CX - C++ framework for general purpose development
 *
 * https://github.com/draede/cx
 * 
 * Copyright (C) 2014 - 2016 draede - draede [at] outlook [dot] com
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


#include "CX/APIDefs.hpp"
#include "CX/Platform.hpp"
#include "CX/Slice.hpp"
#include "CX/Detail/DetailPrint/DetailPrint.hpp"
#include "CX/StatusCodes.hpp"


namespace CX
{

class CX_API Status
{
public:

	static const Size    MAX_MSG_LEN = 1023;

	Status();

	Status(const Status &status);

	Status(StatusCode nCode);

	Status(StatusCode nCode, const Char *szMsg);

	operator bool() const
	{
		return IsOK();
	}

	operator StatusCode() const
	{
		return m_nCode;
	}

	template <typename T1>
	Status(StatusCode nCode, const Char *szFormat, T1 p1)
	{
		m_nCode = nCode;
		Set(nCode, szFormat, p1);
	}

	template <typename T1, typename T2>
	Status(StatusCode nCode, const Char *szFormat, T1 p1, T2 p2)
	{
		m_nCode = nCode;
		Set(nCode, szFormat, p1, p2);
	}

	template <typename T1, typename T2, typename T3>
	Status(StatusCode nCode, const Char *szFormat, T1 p1, T2 p2, T3 p3)
	{
		m_nCode = nCode;
		Set(nCode, szFormat, p1, p2, p3);
	}

	template <typename T1, typename T2, typename T3, typename T4>
	Status(StatusCode nCode, const Char *szFormat, T1 p1, T2 p2, T3 p3, T4 p4)
	{
		m_nCode = nCode;
		Set(nCode, szFormat, p1, p2, p3, p4);
	}

	template <typename T1, typename T2, typename T3, typename T4, typename T5>
	Status(StatusCode nCode, const Char *szFormat, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5)
	{
		m_nCode = nCode;
		Set(nCode, szFormat, p1, p2, p3, p4, p5);
	}

	template <typename T1, typename T2, typename T3, typename T4, typename T5, typename T6>
	Status(StatusCode nCode, const Char *szFormat, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6)
	{
		m_nCode = nCode;
		Set(nCode, szFormat, p1, p2, p3, p4, p5, p6);
	}

	template <typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, 
	          typename T7>
	Status(StatusCode nCode, const Char *szFormat, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7)
	{
		m_nCode = nCode;
		Set(nCode, szFormat, p1, p2, p3, p4, p5, p6, p7);
	}

	template <typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, 
	          typename T7, typename T8>
	Status(StatusCode nCode, const Char *szFormat, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, 
	       T8 p8)
	{
		m_nCode = nCode;
		Set(nCode, szFormat, p1, p2, p3, p4, p5, p6, p7, p8);
	}

	template <typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, 
	          typename T7, typename T8, typename T9>
	Status(StatusCode nCode, const Char *szFormat, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, 
	       T8 p8, T9 p9)
	{
		m_nCode = nCode;
		Set(nCode, szFormat, p1, p2, p3, p4, p5, p6, p7, p8, p9);
	}

	template <typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, 
	          typename T7, typename T8, typename T9, typename T10>
	Status(StatusCode nCode, const Char *szFormat, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, 
	       T8 p8, T9 p9, T10 p10)
	{
		m_nCode = nCode;
		Set(nCode, szFormat, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10);
	}

	template <typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, 
	          typename T7, typename T8, typename T9, typename T10, typename T11>
	Status(StatusCode nCode, const Char *szFormat, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, 
	       T8 p8, T9 p9, T10 p10, T11 p11)
	{
		m_nCode = nCode;
		Set(nCode, szFormat, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11);
	}

	template <typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, 
	          typename T7, typename T8, typename T9, typename T10, typename T11, typename T12>
	Status(StatusCode nCode, const Char *szFormat, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, 
	       T7 p7, T8 p8, T9 p9, T10 p10, T11 p11, T12 p12)
	{
		m_nCode = nCode;
		Set(nCode, szFormat, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12);
	}

	template <typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, 
	          typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, 
	          typename T13>
	Status(StatusCode nCode, const Char *szFormat, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, 
	       T8 p8, T9 p9, T10 p10, T11 p11, T12 p12, T13 p13)
	{
		m_nCode = nCode;
		Set(nCode, szFormat, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13);
	}

	template <typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, 
	          typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, 
	          typename T13, typename T14>
	Status(StatusCode nCode, const Char *szFormat, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, 
	       T8 p8, T9 p9, T10 p10, T11 p11, T12 p12, T13 p13, T14 p14)
	{
		m_nCode = nCode;
		Set(nCode, szFormat, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14);
	}

	template <typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, 
	          typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, 
	          typename T13, typename T14, typename T15>
	Status(StatusCode nCode, const Char *szFormat, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, 
	       T8 p8, T9 p9, T10 p10, T11 p11, T12 p12, T13 p13, T14 p14, T15 p15)
	{
		m_nCode = nCode;
		Set(nCode, szFormat, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15);
	}

	template <typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, 
	          typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, 
	          typename T13, typename T14, typename T15, typename T16>
	Status(StatusCode nCode, const Char *szFormat, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, 
	       T7 p7, T8 p8, T9 p9, T10 p10, T11 p11, T12 p12, T13 p13, T14 p14, T15 p15, T16 p16)
	{
		m_nCode = nCode;
		Set(nCode, szFormat, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16);
	}

	~Status()
	{
	}

	Status &operator=(const Status &status)
	{
		m_nCode = status.m_nCode;
		memcpy(m_szMsg, status.m_szMsg, (MAX_MSG_LEN + 1) * sizeof(Char));

		return *this;
	}

	void Set(StatusCode nStatusCode, const Char *szMsg);

	StatusCode GetCode() const;

	const Char *GetMsg() const;

	Bool IsOK() const;

	Bool IsNOK() const;

	void Clear();

	template <typename T1>
	void Set(StatusCode nCode, const Char *szFormat, T1 p1)
	{
		CX::EmptyType et;
		Slice         slice(m_szMsg, MAX_MSG_LEN);

		m_nCode = nCode;
		if (CXNOK(Detail::DetailPrint::Print<1>(&slice, szFormat, p1, et, et, et, et, et, et, et,
		                                        et, et, et, et, et, et, et, et)))
		{
			m_szMsg[0] = 0;
		}
	}

	template <typename T1, typename T2>
	void Set(StatusCode nCode, const Char *szFormat, T1 p1, T2 p2)
	{
		CX::EmptyType et;
		Slice         slice(m_szMsg, MAX_MSG_LEN);

		m_nCode = nCode;
		if (CXNOK(Detail::DetailPrint::Print<2>(&slice, szFormat, p1, p2, et, et, et, et, et, et, 
		                                        et, et, et, et, et, et, et, et)))
		{
			m_szMsg[0] = 0;
		}
	}

	template <typename T1, typename T2, typename T3>
	void Set(StatusCode nCode, const Char *szFormat, T1 p1, T2 p2, T3 p3)
	{
		CX::EmptyType et;
		Slice         slice(m_szMsg, MAX_MSG_LEN);

		m_nCode = nCode;
		if (CXNOK(Detail::DetailPrint::Print<3>(&slice, szFormat, p1, p2, p3, et, et, et, et, et, 
		                                        et, et, et, et, et, et, et, et)))
		{
			m_szMsg[0] = 0;
		}
	}

	template <typename T1, typename T2, typename T3, typename T4>
	void Set(StatusCode nCode, const Char *szFormat, T1 p1, T2 p2, T3 p3, T4 p4)
	{
		CX::EmptyType et;
		Slice         slice(m_szMsg, MAX_MSG_LEN);

		m_nCode = nCode;
		if (CXNOK(Detail::DetailPrint::Print<4>(&slice, szFormat, p1, p2, p3, p4, et, et, et, et, 
		                                        et, et, et, et, et, et, et, et)))
		{
			m_szMsg[0] = 0;
		}
	}

	template <typename T1, typename T2, typename T3, typename T4, typename T5>
	void Set(StatusCode nCode, const Char *szFormat, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5)
	{
		CX::EmptyType et;
		Slice         slice(m_szMsg, MAX_MSG_LEN);

		m_nCode = nCode;
		if (CXNOK(Detail::DetailPrint::Print<5>(&slice, szFormat, p1, p2, p3, p4, p5, et, et, et, 
		                                        et, et, et, et, et, et, et, et)))
		{
			m_szMsg[0] = 0;
		}
	}

	template <typename T1, typename T2, typename T3, typename T4, typename T5, typename T6>
	void Set(StatusCode nCode, const Char *szFormat, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6)
	{
		CX::EmptyType et;
		Slice         slice(m_szMsg, MAX_MSG_LEN);

		m_nCode = nCode;
		if (CXNOK(Detail::DetailPrint::Print<6>(&slice, szFormat, p1, p2, p3, p4, p5, p6, et, et, 
		                                        et, et, et, et, et, et, et, et)))
		{
			m_szMsg[0] = 0;
		}
	}

	template <typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, 
	          typename T7>
	void Set(StatusCode nCode, const Char *szFormat, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7)
	{
		CX::EmptyType et;
		Slice         slice(m_szMsg, MAX_MSG_LEN);

		m_nCode = nCode;
		if (CXNOK(Detail::DetailPrint::Print<7>(&slice, szFormat, p1, p2, p3, p4, p5, p6, p7, et, 
		                                        et, et, et, et, et, et, et, et)))
		{
			m_szMsg[0] = 0;
		}
	}

	template <typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, 
	          typename T7, typename T8>
	void Set(StatusCode nCode, const Char *szFormat, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, 
	       T8 p8)
	{
		CX::EmptyType et;
		Slice         slice(m_szMsg, MAX_MSG_LEN);

		m_nCode = nCode;
		if (CXNOK(Detail::DetailPrint::Print<8>(&slice, szFormat, p1, p2, p3, p4, p5, p6, p7, p8, 
		                                        et, et, et, et, et, et, et, et)))
		{
			m_szMsg[0] = 0;
		}
	}

	template <typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, 
	          typename T7, typename T8, typename T9>
	void Set(StatusCode nCode, const Char *szFormat, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, 
	       T8 p8, T9 p9)
	{
		CX::EmptyType et;
		Slice         slice(m_szMsg, MAX_MSG_LEN);

		m_nCode = nCode;
		if (CXNOK(Detail::DetailPrint::Print<9>(&slice, szFormat, p1, p2, p3, p4, p5, p6, p7, p8, 
		                                        p9, et, et, et, et, et, et, et)))
		{
			m_szMsg[0] = 0;
		}
	}

	template <typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, 
	          typename T7, typename T8, typename T9, typename T10>
	void Set(StatusCode nCode, const Char *szFormat, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, 
	       T8 p8, T9 p9, T10 p10)
	{
		CX::EmptyType et;
		Slice         slice(m_szMsg, MAX_MSG_LEN);

		m_nCode = nCode;
		if (CXNOK(Detail::DetailPrint::Print<10>(&slice, szFormat, p1, p2, p3, p4, p5, p6, p7, p8, 
		                                         p9, p10, et, et, et, et, et, et)))
		{
			m_szMsg[0] = 0;
		}
	}

	template <typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, 
	          typename T7, typename T8, typename T9, typename T10, typename T11>
	void Set(StatusCode nCode, const Char *szFormat, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, 
	       T8 p8, T9 p9, T10 p10, T11 p11)
	{
		CX::EmptyType et;
		Slice         slice(m_szMsg, MAX_MSG_LEN);

		m_nCode = nCode;
		if (CXNOK(Detail::DetailPrint::Print<11>(&slice, szFormat, p1, p2, p3, p4, p5, p6, p7, p8, 
		                                         p9, p10, p11, et, et, et, et, et)))
		{
			m_szMsg[0] = 0;
		}
	}

	template <typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, 
	          typename T7, typename T8, typename T9, typename T10, typename T11, typename T12>
	void Set(StatusCode nCode, const Char *szFormat, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, 
	       T7 p7, T8 p8, T9 p9, T10 p10, T11 p11, T12 p12)
	{
		CX::EmptyType et;
		Slice         slice(m_szMsg, MAX_MSG_LEN);

		m_nCode = nCode;
		if (CXNOK(Detail::DetailPrint::Print<12>(&slice, szFormat, p1, p2, p3, p4, p5, p6, p7, p8, 
		                                         p9, p10, p11, p12, et, et, et, et)))
		{
			m_szMsg[0] = 0;
		}
	}

	template <typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, 
	          typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, 
	          typename T13>
	void Set(StatusCode nCode, const Char *szFormat, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, 
	       T8 p8, T9 p9, T10 p10, T11 p11, T12 p12, T13 p13)
	{
		CX::EmptyType et;
		Slice         slice(m_szMsg, MAX_MSG_LEN);

		m_nCode = nCode;
		if (CXNOK(Detail::DetailPrint::Print<13>(&slice, szFormat, p1, p2, p3, p4, p5, p6, p7, p8, 
		                                         p9, p10, p11, p12, p13, et, et, et)))
		{
			m_szMsg[0] = 0;
		}
	}

	template <typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, 
	          typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, 
	          typename T13, typename T14>
	void Set(StatusCode nCode, const Char *szFormat, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, 
	       T8 p8, T9 p9, T10 p10, T11 p11, T12 p12, T13 p13, T14 p14)
	{
		CX::EmptyType et;
		Slice         slice(m_szMsg, MAX_MSG_LEN);

		m_nCode = nCode;
		if (CXNOK(Detail::DetailPrint::Print<14>(&slice, szFormat, p1, p2, p3, p4, p5, p6, p7, p8, 
		                                         p9, p10, p11, p12, p13, p14, et, et)))
		{
			m_szMsg[0] = 0;
		}
	}

	template <typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, 
	          typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, 
	          typename T13, typename T14, typename T15>
	void Set(StatusCode nCode, const Char *szFormat, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, 
	       T8 p8, T9 p9, T10 p10, T11 p11, T12 p12, T13 p13, T14 p14, T15 p15)
	{
		CX::EmptyType et;
		Slice         slice(m_szMsg, MAX_MSG_LEN);

		m_nCode = nCode;
		if (CXNOK(Detail::DetailPrint::Print<15>(&slice, szFormat, p1, p2, p3, p4, p5, p6, p7, p8, 
		                                         p9, p10, p11, p12, p13, p14, p15, et)))
		{
			m_szMsg[0] = 0;
		}
	}

	template <typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, 
	          typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, 
	          typename T13, typename T14, typename T15, typename T16>
	void Set(StatusCode nCode, const Char *szFormat, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, 
	       T7 p7, T8 p8, T9 p9, T10 p10, T11 p11, T12 p12, T13 p13, T14 p14, T15 p15, T16 p16)
	{
		Slice slice(m_szMsg, MAX_MSG_LEN);

		m_nCode = nCode;
		if (CXNOK(Detail::DetailPrint::Print<16>(&slice, szFormat, p1, p2, p3, p4, p5, p6, p7, p8, 
		                                         p9, p10, p11, p12, p13, p14, p15, p16)))
		{
			m_szMsg[0] = 0;
		}
	}

private:

	StatusCode   m_nCode;
	Char         m_szMsg[MAX_MSG_LEN + 1];

};

}//CX_API 

