
#pragma once


#include "CX/Types.h"
#include "CX/Detail/DetailPrint/DetailPrint.h"


namespace CX
{

template <typename O>
static inline StatusCode Print(O o, const Char *szFormat)
{
	CX::EmptyType et;

	return Detail::DetailPrint::Print<0>(o, szFormat, et, et, et, et, et, et, et, et, et, et, et, 
	                                     et, et, et, et, et);
}

template <typename O, typename T1>
static inline StatusCode Print(O o, const Char *szFormat, T1 p1)
{
	CX::EmptyType et;

	return Detail::DetailPrint::Print<1>(o, szFormat, p1, et, et, et, et, et, et, et, et, et, et, 
	                                     et, et, et, et, et);
}

template <typename O, typename T1, typename T2>
static inline StatusCode Print(O o, const Char *szFormat, T1 p1, T2 p2)
{
	CX::EmptyType et;

	return Detail::DetailPrint::Print<2>(o, szFormat, p1, p2, et, et, et, et, et, et, et, et, et, 
	                                     et, et, et, et, et);
}

template <typename O, typename T1, typename T2, typename T3>
static inline StatusCode Print(O o, const Char *szFormat, T1 p1, T2 p2, T3 p3)
{
	CX::EmptyType et;

	return Detail::DetailPrint::Print<3>(o, szFormat, p1, p2, p3, et, et, et, et, et, et, et, et, 
	                                     et, et, et, et, et);
}

template <typename O, typename T1, typename T2, typename T3, typename T4>
static inline StatusCode Print(O o, const Char *szFormat, T1 p1, T2 p2, T3 p3, T4 p4)
{
	CX::EmptyType et;

	return Detail::DetailPrint::Print<4>(o, szFormat, p1, p2, p3, p4, et, et, et, et, et, et, et, 
	                                     et, et, et, et, et);
}

template <typename O, typename T1, typename T2, typename T3, typename T4, typename T5>
static inline StatusCode Print(O o, const Char *szFormat, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5)
{
	CX::EmptyType et;

	return Detail::DetailPrint::Print<5>(o, szFormat, p1, p2, p3, p4, p5, et, et, et, et, et, et, 
	                                     et, et, et, et, et);
}

template <typename O, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6>
static inline StatusCode Print(O o, const Char *szFormat, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6)
{
	CX::EmptyType et;

	return Detail::DetailPrint::Print<6>(o, szFormat, p1, p2, p3, p4, p5, p6, et, et, et, et, et, 
	                                     et, et, et, et, et);
}

template <typename O, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, 
          typename T7>
static inline StatusCode Print(O o, const Char *szFormat, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, 
                               T7 p7)
{
	CX::EmptyType et;

	return Detail::DetailPrint::Print<7>(o, szFormat, p1, p2, p3, p4, p5, p6, p7, et, et, et, et, 
	                                     et, et, et, et, et);
}

template <typename O, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, 
          typename T7, typename T8>
static inline StatusCode Print(O o, const Char *szFormat, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, 
                               T7 p7, T8 p8)
{
	CX::EmptyType et;

	return Detail::DetailPrint::Print<8>(o, szFormat, p1, p2, p3, p4, p5, p6, p7, p8, et, et, et, 
	                                     et, et, et, et, et);
}

template <typename O, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, 
          typename T7, typename T8, typename T9>
static inline StatusCode Print(O o, const Char *szFormat, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, 
                               T7 p7, T8 p8, T9 p9)
{
	CX::EmptyType et;

	return Detail::DetailPrint::Print<9>(o, szFormat, p1, p2, p3, p4, p5, p6, p7, p8, p9, et, et, 
	                                     et, et, et, et, et);
}

template <typename O, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, 
          typename T7, typename T8, typename T9, typename T10>
static inline StatusCode Print(O o, const Char *szFormat, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, 
                               T7 p7, T8 p8, T9 p9, T10 p10)
{
	CX::EmptyType et;

	return Detail::DetailPrint::Print<10>(o, szFormat, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, et, 
	                                      et, et, et, et, et);
}

template <typename O, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, 
          typename T7, typename T8, typename T9, typename T10, typename T11>
static inline StatusCode Print(O o, const Char *szFormat, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, 
                               T7 p7, T8 p8, T9 p9, T10 p10, T11 p11)
{
	CX::EmptyType et;

	return Detail::DetailPrint::Print<11>(o, szFormat, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, 
	                                      et, et, et, et, et);
}

template <typename O, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, 
          typename T7, typename T8, typename T9, typename T10, typename T11, typename T12>
static inline StatusCode Print(O o, const Char *szFormat, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, 
                               T7 p7, T8 p8, T9 p9, T10 p10, T11 p11, T12 p12)
{
	CX::EmptyType et;

	return Detail::DetailPrint::Print<12>(o, szFormat, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, 
	                                      p12, et, et, et, et);
}

template <typename O, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, 
          typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, 
          typename T13>
static inline StatusCode Print(O o, const Char *szFormat, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, 
                               T7 p7, T8 p8, T9 p9, T10 p10, T11 p11, T12 p12, T13 p13)
{
	CX::EmptyType et;

	return Detail::DetailPrint::Print<13>(o, szFormat, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, 
	                                      p12, p13, et, et, et);
}

template <typename O, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, 
          typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, 
          typename T13, typename T14>
static inline StatusCode Print(O o, const Char *szFormat, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, 
                               T7 p7, T8 p8, T9 p9, T10 p10, T11 p11, T12 p12, T13 p13, T14 p14)
{
	CX::EmptyType et;

	return Detail::DetailPrint::Print<14>(o, szFormat, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, 
	                                      p12, p13, p14, et, et);
}

template <typename O, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, 
          typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, 
          typename T13, typename T14, typename T15>
static inline StatusCode Print(O o, const Char *szFormat, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, 
                               T7 p7, T8 p8, T9 p9, T10 p10, T11 p11, T12 p12, T13 p13, T14 p14, 
                               T15 p15)
{
	CX::EmptyType et;

	return Detail::DetailPrint::Print<15>(o, szFormat, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, 
	                                      p12, p13, p14, p15, et);
}

template <typename O, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, 
          typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, 
          typename T13, typename T14, typename T15, typename T16>
static inline StatusCode Print(O o, const Char *szFormat, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, 
                               T7 p7, T8 p8, T9 p9, T10 p10, T11 p11, T12 p12, T13 p13, T14 p14, 
                               T15 p15, T16 p16)
{
	return Detail::DetailPrint::Print<16>(o, szFormat, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, 
	                                      p12, p13, p14, p15, p16);
}

}//namespace CX

