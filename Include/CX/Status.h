
#pragma once


#include "CX/APIDefs.h"
#include "CX/Platform.h"
#include "CX/String.h"
#include "CX/Print.h"
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
		String sTmp;

		if (CXNOK(Print(&sTmp, szFormat, p1)))
		{
			return GetCode();
		}

		return Set(nStatus, sTmp.c_str());
	}

	template <typename T1, typename T2>
	static inline StatusCode Set(StatusCode nStatus, const Char *szFormat, T1 p1, T2 p2)
	{
		String sTmp;

		if (CXNOK(Print(&sTmp, szFormat, p1, p2)))
		{
			return GetCode();
		}

		return Set(nStatus, sTmp.c_str());
	}

	template <typename T1, typename T2, typename T3>
	static inline StatusCode Set(StatusCode nStatus, const Char *szFormat, T1 p1, T2 p2, T3 p3)
	{
		String sTmp;

		if (CXNOK(Print(&sTmp, szFormat, p1, p2, p3)))
		{
			return GetCode();
		}

		return Set(nStatus, sTmp.c_str());
	}

	template <typename T1, typename T2, typename T3, typename T4>
	static inline StatusCode Set(StatusCode nStatus, const Char *szFormat, T1 p1, T2 p2, T3 p3, 
	                             T4 p4)
	{
		String sTmp;

		if (CXNOK(Print(&sTmp, szFormat, p1, p2, p3, p4)))
		{
			return GetCode();
		}

		return Set(nStatus, sTmp.c_str());
	}

	template <typename T1, typename T2, typename T3, typename T4, typename T5>
	static inline StatusCode Set(StatusCode nStatus, const Char *szFormat, T1 p1, T2 p2, T3 p3, 
	                             T4 p4, T5 p5)
	{
		String sTmp;

		if (CXNOK(Print(&sTmp, szFormat, p1, p2, p3, p4, p5)))
		{
			return GetCode();
		}

		return Set(nStatus, sTmp.c_str());
	}

	template <typename T1, typename T2, typename T3, typename T4, typename T5, typename T6>
	static inline StatusCode Set(StatusCode nStatus, const Char *szFormat, T1 p1, T2 p2, T3 p3, 
	                             T4 p4, T5 p5, T6 p6)
	{
		String sTmp;

		if (CXNOK(Print(&sTmp, szFormat, p1, p2, p3, p4, p5, p6)))
		{
			return GetCode();
		}

		return Set(nStatus, sTmp.c_str());
	}

	template <typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, 
	          typename T7>
	static inline StatusCode Set(StatusCode nStatus, const Char *szFormat, T1 p1, T2 p2, T3 p3, 
	                             T4 p4, T5 p5, T6 p6, T7 p7)
	{
		String sTmp;

		if (CXNOK(Print(&sTmp, szFormat, p1, p2, p3, p4, p5, p6, p7)))
		{
			return GetCode();
		}

		return Set(nStatus, sTmp.c_str());
	}

	template <typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, 
	          typename T7, typename T8>
	static inline StatusCode Set(StatusCode nStatus, const Char *szFormat, T1 p1, T2 p2, T3 p3, 
	                             T4 p4, T5 p5, T6 p6, T7 p7, T8 p8)
	{
		String sTmp;

		if (CXNOK(Print(&sTmp, szFormat, p1, p2, p3, p4, p5, p6, p7, p8)))
		{
			return GetCode();
		}

		return Set(nStatus, sTmp.c_str());
	}

	template <typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, 
	          typename T7, typename T8, typename T9>
	static inline StatusCode Set(StatusCode nStatus, const Char *szFormat, T1 p1, T2 p2, T3 p3, 
	                             T4 p4, T5 p5, T6 p6, T7 p7, T8 p8, T9 p9)
	{
		String sTmp;

		if (CXNOK(Print(&sTmp, szFormat, p1, p2, p3, p4, p5, p6, p7, p8, p9)))
		{
			return GetCode();
		}

		return Set(nStatus, sTmp.c_str());
	}

	template <typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, 
	          typename T7, typename T8, typename T9, typename T10>
	static inline StatusCode Set(StatusCode nStatus, const Char *szFormat, T1 p1, T2 p2, T3 p3, 
	                             T4 p4, T5 p5, T6 p6, T7 p7, T8 p8, T9 p9, T10 p10)
	{
		String sTmp;

		if (CXNOK(Print(&sTmp, szFormat, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)))
		{
			return GetCode();
		}

		return Set(nStatus, sTmp.c_str());
	}

	template <typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, 
	          typename T7, typename T8, typename T9, typename T10, typename T11>
	static inline StatusCode Set(StatusCode nStatus, const Char *szFormat, T1 p1, T2 p2, T3 p3, 
	                             T4 p4, T5 p5, T6 p6, T7 p7, T8 p8, T9 p9, T10 p10, T11 p11)
	{
		String sTmp;

		if (CXNOK(Print(&sTmp, szFormat, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11)))
		{
			return GetCode();
		}

		return Set(nStatus, sTmp.c_str());
	}

	template <typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, 
	          typename T7, typename T8, typename T9, typename T10, typename T11, typename T12>
	static inline StatusCode Set(StatusCode nStatus, const Char *szFormat, T1 p1, T2 p2, T3 p3, 
	                             T4 p4, T5 p5, T6 p6, T7 p7, T8 p8, T9 p9, T10 p10, T11 p11, T12 p12)
	{
		String sTmp;

		if (CXNOK(Print(&sTmp, szFormat, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12)))
		{
			return GetCode();
		}

		return Set(nStatus, sTmp.c_str());
	}

	template <typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, 
	          typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, 
	          typename T13>
	static inline StatusCode Set(StatusCode nStatus, const Char *szFormat, T1 p1, T2 p2, T3 p3, 
	                             T4 p4, T5 p5, T6 p6, T7 p7, T8 p8, T9 p9, T10 p10, T11 p11, 
	                             T12 p12, T13 p13)
	{
		String sTmp;

		if (CXNOK(Print(&sTmp, szFormat, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13)))
		{
			return GetCode();
		}

		return Set(nStatus, sTmp.c_str());
	}

	template <typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, 
	          typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, 
	          typename T13, typename T14>
	static inline StatusCode Set(StatusCode nStatus, const Char *szFormat, T1 p1, T2 p2, T3 p3, 
	                             T4 p4, T5 p5, T6 p6, T7 p7, T8 p8, T9 p9, T10 p10, T11 p11, 
	                             T12 p12, T13 p13, T14 p14)
	{
		String sTmp;

		if (CXNOK(Print(&sTmp, szFormat, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, 
		                p14)))
		{
			return GetCode();
		}

		return Set(nStatus, sTmp.c_str());
	}

	template <typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, 
	          typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, 
	          typename T13, typename T14, typename T15>
	static inline StatusCode Set(StatusCode nStatus, const Char *szFormat, T1 p1, T2 p2, T3 p3, 
	                             T4 p4, T5 p5, T6 p6, T7 p7, T8 p8, T9 p9, T10 p10, T11 p11, 
	                             T12 p12, T13 p13, T14 p14, T15 p15)
	{
		String sTmp;

		if (CXNOK(Print(&sTmp, szFormat, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, 
		                p15)))
		{
			return GetCode();
		}

		return Set(nStatus, sTmp.c_str());
	}

	template <typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, 
	          typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, 
	          typename T13, typename T14, typename T15, typename T16>
	static inline StatusCode Set(StatusCode nStatus, const Char *szFormat, T1 p1, T2 p2, T3 p3, 
	                             T4 p4, T5 p5, T6 p6, T7 p7, T8 p8, T9 p9, T10 p10, T11 p11, 
	                             T12 p12, T13 p13, T14 p14, T15 p15, T16 p16)
	{
		String sTmp;

		if (CXNOK(Print(&sTmp, szFormat, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, 
		                p15, p16)))
		{
			return GetCode();
		}

		return Set(nStatus, sTmp.c_str());
	}

	static StatusCode GetCode();

	static const Char *GetMsg();

	static void Clear();

	static bool IsOK();

	static bool IsNOK();

private:

	Status();

	~Status();

};

}//CX_API 

