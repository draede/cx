

#include "CX/Util/IFunction.h"
#include "CX/Util/Container.h"


namespace CX
{

namespace Util
{

template <typename R, typename T1 = EmptyType, typename T2 = EmptyType, typename T3 = EmptyType, 
          typename T4 = EmptyType, typename T5 = EmptyType, typename T6 = EmptyType, 
          typename T7 = EmptyType, typename T8 = EmptyType, typename T9 = EmptyType, 
          typename T10 = EmptyType, typename T11 = EmptyType, typename T12 = EmptyType, 
          typename T13 = EmptyType, typename T14 = EmptyType, typename T15 = EmptyType, 
          typename T16 = EmptyType>
class Function;

template <typename R>
class Function <R> : public IFunction<R>
{
public:

	typedef R (* FunctionProc)();

	Function(FunctionProc pfnProc)
	{
		m_pfnProc = pfnProc;
	}

	virtual R Run()
	{
		return m_pfnProc();
	}

private:

	FunctionProc m_pfnProc;

};

template <typename R, typename T1>
class Function <R, T1> : public IFunction<R>
{
public:

	typedef R (* FunctionProc)(T1);

	Function(FunctionProc pfnProc, T1 p1)
		: m_pfnProc(pfnProc), m_params(p1)
	{
	}

	virtual R Run()
	{
		return m_pfnProc(m_params.m_p1);
	}

private:

	FunctionProc    m_pfnProc;
	Container<T1>   
	                m_params;

};

template <typename R, typename T1, typename T2>
class Function <R, T1, T2> : public IFunction<R>
{
public:

	typedef R (* FunctionProc)(T1 T2);

	Function(FunctionProc pfnProc, T1 p1, T2 p2)
		: m_pfnProc(pfnProc), m_params(p1, p2)
	{
	}

	virtual R Run()
	{
		return m_pfnProc(m_params.m_p1, m_params.m_p2);
	}

private:

	FunctionProc        m_pfnProc;
	Container<T1, T2>
	                    m_params;

};

template <typename R, typename T1, typename T2, typename T3>
class Function <R, T1, T2, T3> : public IFunction<R>
{
public:

	typedef R (* FunctionProc)(T1, T2, T3);

	Function(FunctionProc pfnProc, T1 p1, T2 p2, T3 p3)
		: m_pfnProc(pfnProc), m_params(p1, p2, p3)
	{
	}

	virtual R Run()
	{
		return m_pfnProc(m_params.m_p1, m_params.m_p2, m_params.m_p3);
	}

private:

	FunctionProc        m_pfnProc;
	Container<T1, T2, T3>
	                    m_params;

};

template <typename R, typename T1, typename T2, typename T3, typename T4>
class Function <R, T1, T2, T3, T4> : public IFunction<R>
{
public:

	typedef R (* FunctionProc)(T1, T2, T3, T4);

	Function(FunctionProc pfnProc, T1 p1, T2 p2, T3 p3, T4 p4)
		: m_pfnProc(pfnProc), m_params(p1, p2, p3, p4)
	{
	}

	virtual R Run()
	{
		return m_pfnProc(m_params.m_p1, m_params.m_p2, m_params.m_p3, m_params.m_p4);
	}

private:

	FunctionProc        m_pfnProc;
	Container<T1, T2, T3, T4>
	                    m_params;

};

template <typename R, typename T1, typename T2, typename T3, typename T4, typename T5>
class Function <R, T1, T2, T3, T4, T5> : public IFunction<R>
{
public:

	typedef R (* FunctionProc)(T1, T2, T3, T4, T5);

	Function(FunctionProc pfnProc, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5)
		: m_pfnProc(pfnProc), m_params(p1, p2, p3, p4, p5)
	{
	}

	virtual R Run()
	{
		return m_pfnProc(m_params.m_p1, m_params.m_p2, m_params.m_p3, m_params.m_p4, m_params.m_p5);
	}

private:

	FunctionProc        m_pfnProc;
	Container<T1, T2, T3, T4, T5>
	                    m_params;

};

template <typename R, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6>
class Function <R, T1, T2, T3, T4, T5, T6> : public IFunction<R>
{
public:

	typedef R (* FunctionProc)(T1, T2, T3, T4, T5, T6);

	Function(FunctionProc pfnProc, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6)
		: m_pfnProc(pfnProc), m_params(p1, p2, p3, p4, p5, p6)
	{
	}

	virtual R Run()
	{
		return m_pfnProc(m_params.m_p1, m_params.m_p2, m_params.m_p3, m_params.m_p4, m_params.m_p5, 
		                 m_params.m_p6);
	}

private:

	FunctionProc        m_pfnProc;
	Container<T1, T2, T3, T4, T5, T6>
	                    m_params;

};

template <typename R, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, 
          typename T7>
class Function <R, T1, T2, T3, T4, T5, T6, T7> : public IFunction<R>
{
public:

	typedef R (* FunctionProc)(T1, T2, T3, T4, T5, T6, T7);

	Function(FunctionProc pfnProc, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7)
		: m_pfnProc(pfnProc), m_params(p1, p2, p3, p4, p5, p6, p7)
	{
	}

	virtual R Run()
	{
		return m_pfnProc(m_params.m_p1, m_params.m_p2, m_params.m_p3, m_params.m_p4, m_params.m_p5, 
		                 m_params.m_p6, m_params.m_p7);
	}

private:

	FunctionProc        m_pfnProc;
	Container<T1, T2, T3, T4, T5, T6, T7>
	                    m_params;

};

template <typename R, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, 
          typename T7, typename T8>
class Function <R, T1, T2, T3, T4, T5, T6, T7, T8> : public IFunction<R>
{
public:

	typedef R (* FunctionProc)(T1, T2, T3, T4, T5, T6, T7, T8);

	Function(FunctionProc pfnProc, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, T8 p8)
		: m_pfnProc(pfnProc), m_params(p1, p2, p3, p4, p5, p6, p7, p8)
	{
	}

	virtual R Run()
	{
		return m_pfnProc(m_params.m_p1, m_params.m_p2, m_params.m_p3, m_params.m_p4, m_params.m_p5, 
		                 m_params.m_p6, m_params.m_p7, m_params.m_p8);
	}

private:

	FunctionProc        m_pfnProc;
	Container<T1, T2, T3, T4, T5, T6, T7, T8>
	                    m_params;

};

template <typename R, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, 
          typename T7, typename T8, typename T9>
class Function <R, T1, T2, T3, T4, T5, T6, T7, T8, T9> : public IFunction<R>
{
public:

	typedef R (* FunctionProc)(T1, T2, T3, T4, T5, T6, T7, T8, T9);

	Function(FunctionProc pfnProc, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, T8 p8, T9 p9)
		: m_pfnProc(pfnProc), m_params(p1, p2, p3, p4, p5, p6, p7, p8, p9)
	{
	}

	virtual R Run()
	{
		return m_pfnProc(m_params.m_p1, m_params.m_p2, m_params.m_p3, m_params.m_p4, m_params.m_p5, 
		                 m_params.m_p6, m_params.m_p7, m_params.m_p8, m_params.m_p9);
	}

private:

	FunctionProc        m_pfnProc;
	Container<T1, T2, T3, T4, T5, T6, T7, T8, T9>
	                    m_params;

};

template <typename R, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, 
          typename T7, typename T8, typename T9, typename T10>
class Function <R, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10> : public IFunction<R>
{
public:

	typedef R (* FunctionProc)(T1, T2, T3, T4, T5, T6, T7, T8, T9);

	Function(FunctionProc pfnProc, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, T8 p8, T9 p9, 
	                               T10 p10)
		: m_pfnProc(pfnProc), m_params(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)
	{
	}

	virtual R Run()
	{
		return m_pfnProc(m_params.m_p1, m_params.m_p2, m_params.m_p3, m_params.m_p4, m_params.m_p5, 
		                 m_params.m_p6, m_params.m_p7, m_params.m_p8, m_params.m_p9, m_params.m_p10);
	}

private:

	FunctionProc        m_pfnProc;
	Container<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10>
	                    m_params;

};

template <typename R, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, 
          typename T7, typename T8, typename T9, typename T10, typename T11>
class Function <R, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11> : public IFunction<R>
{
public:

	typedef R (* FunctionProc)(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11);

	Function(FunctionProc pfnProc, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, T8 p8, T9 p9, 
	                               T10 p10, T11 p11)
		: m_pfnProc(pfnProc), m_params(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11)
	{
	}

	virtual R Run()
	{
		return m_pfnProc(m_params.m_p1, m_params.m_p2, m_params.m_p3, m_params.m_p4, m_params.m_p5, 
		                 m_params.m_p6, m_params.m_p7, m_params.m_p8, m_params.m_p9, m_params.m_p10, 
		                 m_params.m_p11);
	}

private:

	FunctionProc        m_pfnProc;
	Container<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11>
	                    m_params;

};

template <typename R, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, 
          typename T7, typename T8, typename T9, typename T10, typename T11, typename T12>
class Function <R, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12> : public IFunction<R>
{
public:

	typedef R (* FunctionProc)(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12);

	Function(FunctionProc pfnProc, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, T8 p8, T9 p9, 
	                               T10 p10, T11 p11, T12 p12)
		: m_pfnProc(pfnProc), m_params(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12)
	{
	}

	virtual R Run()
	{
		return m_pfnProc(m_params.m_p1, m_params.m_p2, m_params.m_p3, m_params.m_p4, m_params.m_p5, 
		                 m_params.m_p6, m_params.m_p7, m_params.m_p8, m_params.m_p9, m_params.m_p10, 
		                 m_params.m_p11, m_params.m_p12);
	}

private:

	FunctionProc        m_pfnProc;
	Container<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12>
	                    m_params;

};

template <typename R, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, 
          typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, 
          typename T13>
class Function <R, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13> : public IFunction<R>
{
public:

	typedef R (* FunctionProc)(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13);

	Function(FunctionProc pfnProc, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, T8 p8, T9 p9, 
	                               T10 p10, T11 p11, T12 p12, T13 p13)
		: m_pfnProc(pfnProc), m_params(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13)
	{
	}

	virtual R Run()
	{
		return m_pfnProc(m_params.m_p1, m_params.m_p2, m_params.m_p3, m_params.m_p4, m_params.m_p5, 
		                 m_params.m_p6, m_params.m_p7, m_params.m_p8, m_params.m_p9, m_params.m_p10, 
		                 m_params.m_p11, m_params.m_p12, m_params.m_p13);
	}

private:

	FunctionProc        m_pfnProc;
	Container<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13>
	                    m_params;

};

template <typename R, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, 
          typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, 
          typename T13, typename T14>
class Function <R, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14> : 
          public IFunction<R>
{
public:

	typedef R (* FunctionProc)(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14);

	Function(FunctionProc pfnProc, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, T8 p8, T9 p9, 
	                               T10 p10, T11 p11, T12 p12, T13 p13, T14 p14)
		: m_pfnProc(pfnProc), m_params(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14)
	{
	}

	virtual R Run()
	{
		return m_pfnProc(m_params.m_p1, m_params.m_p2, m_params.m_p3, m_params.m_p4, m_params.m_p5, 
		                 m_params.m_p6, m_params.m_p7, m_params.m_p8, m_params.m_p9, m_params.m_p10, 
		                 m_params.m_p11, m_params.m_p12, m_params.m_p13, m_params.m_p14);
	}

private:

	FunctionProc        m_pfnProc;
	Container<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14>
	                    m_params;

};

template <typename R, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, 
          typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, 
          typename T13, typename T14, typename T15>
class Function <R, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15> : 
          public IFunction<R>
{
public:

	typedef R (* FunctionProc)(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15);

	Function(FunctionProc pfnProc, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, T8 p8, T9 p9, 
	                               T10 p10, T11 p11, T12 p12, T13 p13, T14 p14, T15 p15)
		: m_pfnProc(pfnProc), m_params(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, 
	                                  p15)
	{
	}

	virtual R Run()
	{
		return m_pfnProc(m_params.m_p1, m_params.m_p2, m_params.m_p3, m_params.m_p4, m_params.m_p5, 
		                 m_params.m_p6, m_params.m_p7, m_params.m_p8, m_params.m_p9, m_params.m_p10, 
		                 m_params.m_p11, m_params.m_p12, m_params.m_p13, m_params.m_p14, 
		                 m_params.m_p15);
	}

private:

	FunctionProc        m_pfnProc;
	Container<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15>
	                    m_params;

};

template <typename R, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, 
          typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, 
          typename T13, typename T14, typename T15, typename T16>
class Function : public IFunction<R>
{
public:

	typedef R (* FunctionProc)(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, 
	                           T16);

	Function(FunctionProc pfnProc, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, T8 p8, T9 p9, 
	                               T10 p10, T11 p11, T12 p12, T13 p13, T14 p14, T15 p15, T16 p16)
		: m_pfnProc(pfnProc), m_params(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, 
	                                  p15, p16)
	{
	}

	virtual R Run()
	{
		return m_pfnProc(m_params.m_p1, m_params.m_p2, m_params.m_p3, m_params.m_p4, m_params.m_p5, 
		                 m_params.m_p6, m_params.m_p7, m_params.m_p8, m_params.m_p9, m_params.m_p10, 
		                 m_params.m_p11, m_params.m_p12, m_params.m_p13, m_params.m_p14, 
		                 m_params.m_p15, m_params.m_p16);
	}

private:

	FunctionProc        m_pfnProc;
	Container<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16>
	                    m_params;

};

}//namespace Util

}//namespace CX

