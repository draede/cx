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

#include "CX/Types.hpp"
#include "CX/EmptyType.hpp"


namespace CX
{

namespace Util
{

template <typename T1 = EmptyType, typename T2 = EmptyType, typename T3 = EmptyType, 
          typename T4 = EmptyType, typename T5 = EmptyType, typename T6 = EmptyType, 
          typename T7 = EmptyType, typename T8 = EmptyType, typename T9 = EmptyType, 
          typename T10 = EmptyType, typename T11 = EmptyType, typename T12 = EmptyType, 
          typename T13 = EmptyType, typename T14 = EmptyType, typename T15 = EmptyType, 
          typename T16 = EmptyType>
class Container;

template < >
class Container< >
{
public:

	typedef Container< > SelfType;

	Container()
	{
	}

	Container(const SelfType &con)
	{
		Copy(con);
	}

	SelfType &operator=(const SelfType &con)
	{
		Copy(con);

		return *this;
	}

	void Copy(const SelfType &con)
	{
		con;
	}

	Size GetCount()
	{
		return 0;
	}

};

template <typename T1>
class Container<T1>
{
public:

	typedef Container<T1> SelfType;

	T1    m_p1;

	Container(T1 p1)
	{
		m_p1   = p1;
	}

	Container(const SelfType &con)
	{
		Copy(con);
	}

	SelfType &operator=(const SelfType &con)
	{
		Copy(con);

		return *this;
	}

	void Copy(const SelfType &con)
	{
		m_p1 = con.m_p1;
	}

	Size GetCount()
	{
		return 1;
	}

	template <Size cIndex>
	struct TypeAt
	{
	};

	template <>
	struct TypeAt<0>
	{
		typedef T1 Type;
	};

};

template <typename T1, typename T2>
class Container<T1, T2>
{
public:

	typedef Container<T1, T2> SelfType;

	T1    m_p1;
	T2    m_p2;

	Container(T1 p1, T2 p2)
	{
		m_p1   = p1;
		m_p2   = p2;
	}

	Container(const SelfType &con)
	{
		Copy(con);
	}

	SelfType &operator=(const SelfType &con)
	{
		Copy(con);

		return *this;
	}

	void Copy(const SelfType &con)
	{
		m_p1 = con.m_p1;
		m_p2 = con.m_p2;
	}

	Size GetCount()
	{
		return 2;
	}

	template <Size cIndex>
	struct TypeAt
	{
	};

	template <>
	struct TypeAt<0>
	{
		typedef T1 Type;
	};

	template <>
	struct TypeAt<1>
	{
		typedef T2 Type;
	};

};

template <typename T1, typename T2, typename T3>
class Container<T1, T2, T3>
{
public:

	typedef Container<T1, T2, T3> SelfType;

	T1    m_p1;
	T2    m_p2;
	T3    m_p3;

	Container(T1 p1, T2 p2, T3 p3)
	{
		m_p1 = p1;
		m_p2 = p2;
		m_p3 = p3;
	}

	Container(const SelfType &con)
	{
		Copy(con);
	}

	SelfType operator=(const SelfType &con)
	{
		Copy(con);

		return *this;
	}

	void Copy(const SelfType &con)
	{
		m_p1 = con.m_p1;
		m_p2 = con.m_p2;
		m_p3 = con.m_p3;
	}

	Size GetCount()
	{
		return 3;
	}

	template <Size cIndex>
	struct TypeAt
	{
	};

	template <>
	struct TypeAt<0>
	{
		typedef T1 Type;
	};

	template <>
	struct TypeAt<1>
	{
		typedef T2 Type;
	};

	template <>
	struct TypeAt<2>
	{
		typedef T3 Type;
	};

};

template <typename T1, typename T2, typename T3, typename T4>
class Container<T1, T2, T3, T4>
{
public:

	typedef Container<T1, T2, T3, T4> SelfType;

	T1    m_p1;
	T2    m_p2;
	T3    m_p3;
	T4    m_p4;

	Container(T1 p1, T2 p2, T3 p3, T4 p4)
	{
		m_p1 = p1;
		m_p2 = p2;
		m_p3 = p3;
		m_p4 = p4;
	}

	Container(const SelfType &con)
	{
		Copy(con);
	}

	SelfType operator=(const SelfType &con)
	{
		Copy(con);

		return *this;
	}

	void Copy(const SelfType &con)
	{
		m_p1 = con.m_p1;
		m_p2 = con.m_p2;
		m_p3 = con.m_p3;
		m_p4 = con.m_p4;
	}

	Size GetCount()
	{
		return 4;
	}

	template <Size cIndex>
	struct TypeAt
	{
	};

	template <>
	struct TypeAt<0>
	{
		typedef T1 Type;
	};

	template <>
	struct TypeAt<1>
	{
		typedef T2 Type;
	};

	template <>
	struct TypeAt<2>
	{
		typedef T3 Type;
	};

	template <>
	struct TypeAt<3>
	{
		typedef T4 Type;
	};

};

template <typename T1, typename T2, typename T3, typename T4, typename T5>
class Container<T1, T2, T3, T4, T5>
{
public:

	typedef Container<T1, T2, T3, T4, T5> SelfType;

	T1    m_p1;
	T2    m_p2;
	T3    m_p3;
	T4    m_p4;
	T5    m_p5;

	Container(T1 p1, T2 p2, T3 p3, T4 p4, T5 p5)
	{
		m_p1 = p1;
		m_p2 = p2;
		m_p3 = p3;
		m_p4 = p4;
		m_p5 = p5;
	}

	Container(const SelfType &con)
	{
		Copy(con);
	}

	SelfType operator=(const SelfType &con)
	{
		Copy(con);

		return *this;
	}

	void Copy(const SelfType &con)
	{
		m_p1 = con.m_p1;
		m_p2 = con.m_p2;
		m_p3 = con.m_p3;
		m_p4 = con.m_p4;
		m_p5 = con.m_p5;
	}

	Size GetCount()
	{
		return 5;
	}

	template <Size cIndex>
	struct TypeAt
	{
	};

	template <>
	struct TypeAt<0>
	{
		typedef T1 Type;
	};

	template <>
	struct TypeAt<1>
	{
		typedef T2 Type;
	};

	template <>
	struct TypeAt<2>
	{
		typedef T3 Type;
	};

	template <>
	struct TypeAt<3>
	{
		typedef T4 Type;
	};

	template <>
	struct TypeAt<4>
	{
		typedef T5 Type;
	};

};

template <typename T1, typename T2, typename T3, typename T4, typename T5, typename T6>
class Container<T1, T2, T3, T4, T5, T6>
{
public:

	typedef Container<T1, T2, T3, T4, T5, T6> SelfType;

	T1    m_p1;
	T2    m_p2;
	T3    m_p3;
	T4    m_p4;
	T5    m_p5;
	T6    m_p6;

	Container(T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6)
	{
		m_p1 = p1;
		m_p2 = p2;
		m_p3 = p3;
		m_p4 = p4;
		m_p5 = p5;
		m_p6 = p6;
	}

	Container(const SelfType &con)
	{
		Copy(con);
	}

	SelfType operator=(const SelfType &con)
	{
		Copy(con);

		return *this;
	}

	void Copy(const SelfType &con)
	{
		m_p1 = con.m_p1;
		m_p2 = con.m_p2;
		m_p3 = con.m_p3;
		m_p4 = con.m_p4;
		m_p5 = con.m_p5;
		m_p6 = con.m_p6;
	}

	Size GetCount()
	{
		return 6;
	}

	template <Size cIndex>
	struct TypeAt
	{
	};

	template <>
	struct TypeAt<0>
	{
		typedef T1 Type;
	};

	template <>
	struct TypeAt<1>
	{
		typedef T2 Type;
	};

	template <>
	struct TypeAt<2>
	{
		typedef T3 Type;
	};

	template <>
	struct TypeAt<3>
	{
		typedef T4 Type;
	};

	template <>
	struct TypeAt<4>
	{
		typedef T5 Type;
	};

	template <>
	struct TypeAt<5>
	{
		typedef T6 Type;
	};

};

template <typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, 
          typename T7>
class Container<T1, T2, T3, T4, T5, T6, T7>
{
public:

	typedef Container<T1, T2, T3, T4, T5, T6, T7> SelfType;

	T1    m_p1;
	T2    m_p2;
	T3    m_p3;
	T4    m_p4;
	T5    m_p5;
	T6    m_p6;
	T7    m_p7;

	Container(T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7)
	{
		m_p1 = p1;
		m_p2 = p2;
		m_p3 = p3;
		m_p4 = p4;
		m_p5 = p5;
		m_p6 = p6;
		m_p7 = p7;
	}

	Container(const SelfType &con)
	{
		Copy(con);
	}

	SelfType operator=(const SelfType &con)
	{
		Copy(con);

		return *this;
	}

	void Copy(const SelfType &con)
	{
		m_p1 = con.m_p1;
		m_p2 = con.m_p2;
		m_p3 = con.m_p3;
		m_p4 = con.m_p4;
		m_p5 = con.m_p5;
		m_p6 = con.m_p6;
		m_p7 = con.m_p7;
	}

	Size GetCount()
	{
		return 7;
	}

	template <Size cIndex>
	struct TypeAt
	{
	};

	template <>
	struct TypeAt<0>
	{
		typedef T1 Type;
	};

	template <>
	struct TypeAt<1>
	{
		typedef T2 Type;
	};

	template <>
	struct TypeAt<2>
	{
		typedef T3 Type;
	};

	template <>
	struct TypeAt<3>
	{
		typedef T4 Type;
	};

	template <>
	struct TypeAt<4>
	{
		typedef T5 Type;
	};

	template <>
	struct TypeAt<5>
	{
		typedef T6 Type;
	};

	template <>
	struct TypeAt<6>
	{
		typedef T7 Type;
	};

};

template <typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, 
          typename T7, typename T8>
class Container<T1, T2, T3, T4, T5, T6, T7, T8>
{
public:

	typedef Container<T1, T2, T3, T4, T5, T6, T7, T8> SelfType;

	T1    m_p1;
	T2    m_p2;
	T3    m_p3;
	T4    m_p4;
	T5    m_p5;
	T6    m_p6;
	T7    m_p7;
	T8    m_p8;

	Container(T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, T8 p8)
	{
		m_p1 = p1;
		m_p2 = p2;
		m_p3 = p3;
		m_p4 = p4;
		m_p5 = p5;
		m_p6 = p6;
		m_p7 = p7;
		m_p8 = p8;
	}

	Container(const SelfType &con)
	{
		Copy(con);
	}

	SelfType operator=(const SelfType &con)
	{
		Copy(con);

		return *this;
	}

	void Copy(const SelfType &con)
	{
		m_p1 = con.m_p1;
		m_p2 = con.m_p2;
		m_p3 = con.m_p3;
		m_p4 = con.m_p4;
		m_p5 = con.m_p5;
		m_p6 = con.m_p6;
		m_p7 = con.m_p7;
		m_p8 = con.m_p8;
	}

	Size GetCount()
	{
		return 8;
	}

	template <Size cIndex>
	struct TypeAt
	{
	};

	template <>
	struct TypeAt<0>
	{
		typedef T1 Type;
	};

	template <>
	struct TypeAt<1>
	{
		typedef T2 Type;
	};

	template <>
	struct TypeAt<2>
	{
		typedef T3 Type;
	};

	template <>
	struct TypeAt<3>
	{
		typedef T4 Type;
	};

	template <>
	struct TypeAt<4>
	{
		typedef T5 Type;
	};

	template <>
	struct TypeAt<5>
	{
		typedef T6 Type;
	};

	template <>
	struct TypeAt<6>
	{
		typedef T7 Type;
	};

	template <>
	struct TypeAt<7>
	{
		typedef T8 Type;
	};

};

template <typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, 
          typename T7, typename T8, typename T9>
class Container<T1, T2, T3, T4, T5, T6, T7, T8, T9>
{
public:

	typedef Container<T1, T2, T3, T4, T5, T6, T7, T8, T9> SelfType;

	T1    m_p1;
	T2    m_p2;
	T3    m_p3;
	T4    m_p4;
	T5    m_p5;
	T6    m_p6;
	T7    m_p7;
	T8    m_p8;
	T9    m_p9;

	Container(T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, T8 p8, T9 p9)
	{
		m_p1 = p1;
		m_p2 = p2;
		m_p3 = p3;
		m_p4 = p4;
		m_p5 = p5;
		m_p6 = p6;
		m_p7 = p7;
		m_p8 = p8;
		m_p9 = p9;
	}

	Container(const SelfType &con)
	{
		Copy(con);
	}

	SelfType operator=(const SelfType &con)
	{
		Copy(con);

		return *this;
	}

	void Copy(const SelfType &con)
	{
		m_p1 = con.m_p1;
		m_p2 = con.m_p2;
		m_p3 = con.m_p3;
		m_p4 = con.m_p4;
		m_p5 = con.m_p5;
		m_p6 = con.m_p6;
		m_p7 = con.m_p7;
		m_p8 = con.m_p8;
		m_p9 = con.m_p9;
	}

	Size GetCount()
	{
		return 9;
	}

	template <Size cIndex>
	struct TypeAt
	{
	};

	template <>
	struct TypeAt<0>
	{
		typedef T1 Type;
	};

	template <>
	struct TypeAt<1>
	{
		typedef T2 Type;
	};

	template <>
	struct TypeAt<2>
	{
		typedef T3 Type;
	};

	template <>
	struct TypeAt<3>
	{
		typedef T4 Type;
	};

	template <>
	struct TypeAt<4>
	{
		typedef T5 Type;
	};

	template <>
	struct TypeAt<5>
	{
		typedef T6 Type;
	};

	template <>
	struct TypeAt<6>
	{
		typedef T7 Type;
	};

	template <>
	struct TypeAt<7>
	{
		typedef T8 Type;
	};

	template <>
	struct TypeAt<8>
	{
		typedef T9 Type;
	};

};

template <typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, 
          typename T7, typename T8, typename T9, typename T10>
class Container<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10>
{
public:

	typedef Container<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10> SelfType;

	T1    m_p1;
	T2    m_p2;
	T3    m_p3;
	T4    m_p4;
	T5    m_p5;
	T6    m_p6;
	T7    m_p7;
	T8    m_p8;
	T9    m_p9;
	T10   m_p10;

	Container(T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, T8 p8, T9 p9, T10 p10)
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
	}

	Container(const SelfType &con)
	{
		Copy(con);
	}

	SelfType operator=(const SelfType &con)
	{
		Copy(con);

		return *this;
	}

	void Copy(const SelfType &con)
	{
		m_p1  = con.m_p1;
		m_p2  = con.m_p2;
		m_p3  = con.m_p3;
		m_p4  = con.m_p4;
		m_p5  = con.m_p5;
		m_p6  = con.m_p6;
		m_p7  = con.m_p7;
		m_p8  = con.m_p8;
		m_p9  = con.m_p9;
		m_p10 = con.m_p10;
	}

	Size GetCount()
	{
		return 10;
	}

	template <Size cIndex>
	struct TypeAt
	{
	};

	template <>
	struct TypeAt<0>
	{
		typedef T1 Type;
	};

	template <>
	struct TypeAt<1>
	{
		typedef T2 Type;
	};

	template <>
	struct TypeAt<2>
	{
		typedef T3 Type;
	};

	template <>
	struct TypeAt<3>
	{
		typedef T4 Type;
	};

	template <>
	struct TypeAt<4>
	{
		typedef T5 Type;
	};

	template <>
	struct TypeAt<5>
	{
		typedef T6 Type;
	};

	template <>
	struct TypeAt<6>
	{
		typedef T7 Type;
	};

	template <>
	struct TypeAt<7>
	{
		typedef T8 Type;
	};

	template <>
	struct TypeAt<8>
	{
		typedef T9 Type;
	};

	template <>
	struct TypeAt <9>
	{
		typedef T10 Type;
	};

};

template <typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, 
          typename T7, typename T8, typename T9, typename T10, typename T11>
class Container<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11>
{
public:

	typedef Container<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11> SelfType;

	T1    m_p1;
	T2    m_p2;
	T3    m_p3;
	T4    m_p4;
	T5    m_p5;
	T6    m_p6;
	T7    m_p7;
	T8    m_p8;
	T9    m_p9;
	T10   m_p10;
	T11   m_p11;

	Container(T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, T8 p8, T9 p9, T10 p10, T11 p11)
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
	}

	Container(const SelfType &con)
	{
		Copy(con);
	}

	SelfType operator=(const SelfType &con)
	{
		Copy(con);

		return *this;
	}

	void Copy(const SelfType &con)
	{
		m_p1  = con.m_p1;
		m_p2  = con.m_p2;
		m_p3  = con.m_p3;
		m_p4  = con.m_p4;
		m_p5  = con.m_p5;
		m_p6  = con.m_p6;
		m_p7  = con.m_p7;
		m_p8  = con.m_p8;
		m_p9  = con.m_p9;
		m_p10 = con.m_p10;
		m_p11 = con.m_p11;
	}

	Size GetCount()
	{
		return 11;
	}

	template <Size cIndex>
	struct TypeAt
	{
	};

	template <>
	struct TypeAt<0>
	{
		typedef T1 Type;
	};

	template <>
	struct TypeAt<1>
	{
		typedef T2 Type;
	};

	template <>
	struct TypeAt<2>
	{
		typedef T3 Type;
	};

	template <>
	struct TypeAt<3>
	{
		typedef T4 Type;
	};

	template <>
	struct TypeAt<4>
	{
		typedef T5 Type;
	};

	template <>
	struct TypeAt<5>
	{
		typedef T6 Type;
	};

	template <>
	struct TypeAt<6>
	{
		typedef T7 Type;
	};

	template <>
	struct TypeAt<7>
	{
		typedef T8 Type;
	};

	template <>
	struct TypeAt<8>
	{
		typedef T9 Type;
	};

	template <>
	struct TypeAt <9>
	{
		typedef T10 Type;
	};

	template <>
	struct TypeAt <10>
	{
		typedef T11 Type;
	};

};

template <typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, 
          typename T7, typename T8, typename T9, typename T10, typename T11, typename T12>
class Container<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12>
{
public:

	typedef Container<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12> SelfType;

	T1    m_p1;
	T2    m_p2;
	T3    m_p3;
	T4    m_p4;
	T5    m_p5;
	T6    m_p6;
	T7    m_p7;
	T8    m_p8;
	T9    m_p9;
	T10   m_p10;
	T11   m_p11;
	T12   m_p12;

	Container(T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, T8 p8, T9 p9, T10 p10, T11 p11, 
	          T12 p12)
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
	}

	Container(const SelfType &con)
	{
		Copy(con);
	}

	SelfType operator=(const SelfType &con)
	{
		Copy(con);

		return *this;
	}

	void Copy(const SelfType &con)
	{
		m_p1  = con.m_p1;
		m_p2  = con.m_p2;
		m_p3  = con.m_p3;
		m_p4  = con.m_p4;
		m_p5  = con.m_p5;
		m_p6  = con.m_p6;
		m_p7  = con.m_p7;
		m_p8  = con.m_p8;
		m_p9  = con.m_p9;
		m_p10 = con.m_p10;
		m_p11 = con.m_p11;
		m_p12 = con.m_p12;
	}

	Size GetCount()
	{
		return 12;
	}

	template <Size cIndex>
	struct TypeAt
	{
	};

	template <>
	struct TypeAt<0>
	{
		typedef T1 Type;
	};

	template <>
	struct TypeAt<1>
	{
		typedef T2 Type;
	};

	template <>
	struct TypeAt<2>
	{
		typedef T3 Type;
	};

	template <>
	struct TypeAt<3>
	{
		typedef T4 Type;
	};

	template <>
	struct TypeAt<4>
	{
		typedef T5 Type;
	};

	template <>
	struct TypeAt<5>
	{
		typedef T6 Type;
	};

	template <>
	struct TypeAt<6>
	{
		typedef T7 Type;
	};

	template <>
	struct TypeAt<7>
	{
		typedef T8 Type;
	};

	template <>
	struct TypeAt<8>
	{
		typedef T9 Type;
	};

	template <>
	struct TypeAt <9>
	{
		typedef T10 Type;
	};

	template <>
	struct TypeAt <10>
	{
		typedef T11 Type;
	};

	template <>
	struct TypeAt <11>
	{
		typedef T12 Type;
	};

};

template <typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, 
          typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, 
          typename T13>
class Container<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13>
{
public:

	typedef Container<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13> SelfType;

	T1    m_p1;
	T2    m_p2;
	T3    m_p3;
	T4    m_p4;
	T5    m_p5;
	T6    m_p6;
	T7    m_p7;
	T8    m_p8;
	T9    m_p9;
	T10   m_p10;
	T11   m_p11;
	T12   m_p12;
	T13   m_p13;

	Container(T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, T8 p8, T9 p9, T10 p10, T11 p11, 
	          T12 p12, T13 p13)
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
	}

	Container(const SelfType &con)
	{
		Copy(con);
	}

	SelfType operator=(const SelfType &con)
	{
		Copy(con);

		return *this;
	}

	void Copy(const SelfType &con)
	{
		m_p1  = con.m_p1;
		m_p2  = con.m_p2;
		m_p3  = con.m_p3;
		m_p4  = con.m_p4;
		m_p5  = con.m_p5;
		m_p6  = con.m_p6;
		m_p7  = con.m_p7;
		m_p8  = con.m_p8;
		m_p9  = con.m_p9;
		m_p10 = con.m_p10;
		m_p11 = con.m_p11;
		m_p12 = con.m_p12;
		m_p13 = con.m_p13;
	}

	Size GetCount()
	{
		return 13;
	}

	template <Size cIndex>
	struct TypeAt
	{
	};

	template <>
	struct TypeAt<0>
	{
		typedef T1 Type;
	};

	template <>
	struct TypeAt<1>
	{
		typedef T2 Type;
	};

	template <>
	struct TypeAt<2>
	{
		typedef T3 Type;
	};

	template <>
	struct TypeAt<3>
	{
		typedef T4 Type;
	};

	template <>
	struct TypeAt<4>
	{
		typedef T5 Type;
	};

	template <>
	struct TypeAt<5>
	{
		typedef T6 Type;
	};

	template <>
	struct TypeAt<6>
	{
		typedef T7 Type;
	};

	template <>
	struct TypeAt<7>
	{
		typedef T8 Type;
	};

	template <>
	struct TypeAt<8>
	{
		typedef T9 Type;
	};

	template <>
	struct TypeAt <9>
	{
		typedef T10 Type;
	};

	template <>
	struct TypeAt <10>
	{
		typedef T11 Type;
	};

	template <>
	struct TypeAt <11>
	{
		typedef T12 Type;
	};

	template <>
	struct TypeAt <12>
	{
		typedef T13 Type;
	};

};

template <typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, 
          typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, 
          typename T13, typename T14>
class Container<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14>
{
public:

	typedef Container<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14> SelfType;

	T1    m_p1;
	T2    m_p2;
	T3    m_p3;
	T4    m_p4;
	T5    m_p5;
	T6    m_p6;
	T7    m_p7;
	T8    m_p8;
	T9    m_p9;
	T10   m_p10;
	T11   m_p11;
	T12   m_p12;
	T13   m_p13;
	T14   m_p14;

	Container(T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, T8 p8, T9 p9, T10 p10, T11 p11, 
	          T12 p12, T13 p13, T14 p14)
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
	}

	Container(const SelfType &con)
	{
		Copy(con);
	}

	SelfType operator=(const SelfType &con)
	{
		Copy(con);

		return *this;
	}

	void Copy(const SelfType &con)
	{
		m_p1  = con.m_p1;
		m_p2  = con.m_p2;
		m_p3  = con.m_p3;
		m_p4  = con.m_p4;
		m_p5  = con.m_p5;
		m_p6  = con.m_p6;
		m_p7  = con.m_p7;
		m_p8  = con.m_p8;
		m_p9  = con.m_p9;
		m_p10 = con.m_p10;
		m_p11 = con.m_p11;
		m_p12 = con.m_p12;
		m_p13 = con.m_p13;
		m_p14 = con.m_p14;
	}

	Size GetCount()
	{
		return 14;
	}

	template <Size cIndex>
	struct TypeAt
	{
	};

	template <>
	struct TypeAt<0>
	{
		typedef T1 Type;
	};

	template <>
	struct TypeAt<1>
	{
		typedef T2 Type;
	};

	template <>
	struct TypeAt<2>
	{
		typedef T3 Type;
	};

	template <>
	struct TypeAt<3>
	{
		typedef T4 Type;
	};

	template <>
	struct TypeAt<4>
	{
		typedef T5 Type;
	};

	template <>
	struct TypeAt<5>
	{
		typedef T6 Type;
	};

	template <>
	struct TypeAt<6>
	{
		typedef T7 Type;
	};

	template <>
	struct TypeAt<7>
	{
		typedef T8 Type;
	};

	template <>
	struct TypeAt<8>
	{
		typedef T9 Type;
	};

	template <>
	struct TypeAt <9>
	{
		typedef T10 Type;
	};

	template <>
	struct TypeAt <10>
	{
		typedef T11 Type;
	};

	template <>
	struct TypeAt <11>
	{
		typedef T12 Type;
	};

	template <>
	struct TypeAt <12>
	{
		typedef T13 Type;
	};

	template <>
	struct TypeAt <13>
	{
		typedef T14 Type;
	};

};

template <typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, 
          typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, 
          typename T13, typename T14, typename T15>
class Container<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15>
{
public:

	typedef Container<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15> SelfType;

	T1    m_p1;
	T2    m_p2;
	T3    m_p3;
	T4    m_p4;
	T5    m_p5;
	T6    m_p6;
	T7    m_p7;
	T8    m_p8;
	T9    m_p9;
	T10   m_p10;
	T11   m_p11;
	T12   m_p12;
	T13   m_p13;
	T14   m_p14;
	T15   m_p15;

	Container(T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, T8 p8, T9 p9, T10 p10, T11 p11, 
	          T12 p12, T13 p13, T14 p14, T15 p15)
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
	}

	Container(const SelfType &con)
	{
		Copy(con);
	}

	SelfType operator=(const SelfType &con)
	{
		Copy(con);

		return *this;
	}

	void Copy(const SelfType &con)
	{
		m_p1  = con.m_p1;
		m_p2  = con.m_p2;
		m_p3  = con.m_p3;
		m_p4  = con.m_p4;
		m_p5  = con.m_p5;
		m_p6  = con.m_p6;
		m_p7  = con.m_p7;
		m_p8  = con.m_p8;
		m_p9  = con.m_p9;
		m_p10 = con.m_p10;
		m_p11 = con.m_p11;
		m_p12 = con.m_p12;
		m_p13 = con.m_p13;
		m_p14 = con.m_p14;
		m_p15 = con.m_p15;
	}

	Size GetCount()
	{
		return 15;
	}

	template <Size cIndex>
	struct TypeAt
	{
	};

	template <>
	struct TypeAt<0>
	{
		typedef T1 Type;
	};

	template <>
	struct TypeAt<1>
	{
		typedef T2 Type;
	};

	template <>
	struct TypeAt<2>
	{
		typedef T3 Type;
	};

	template <>
	struct TypeAt<3>
	{
		typedef T4 Type;
	};

	template <>
	struct TypeAt<4>
	{
		typedef T5 Type;
	};

	template <>
	struct TypeAt<5>
	{
		typedef T6 Type;
	};

	template <>
	struct TypeAt<6>
	{
		typedef T7 Type;
	};

	template <>
	struct TypeAt<7>
	{
		typedef T8 Type;
	};

	template <>
	struct TypeAt<8>
	{
		typedef T9 Type;
	};

	template <>
	struct TypeAt <9>
	{
		typedef T10 Type;
	};

	template <>
	struct TypeAt <10>
	{
		typedef T11 Type;
	};

	template <>
	struct TypeAt <11>
	{
		typedef T12 Type;
	};

	template <>
	struct TypeAt <12>
	{
		typedef T13 Type;
	};

	template <>
	struct TypeAt <13>
	{
		typedef T14 Type;
	};

	template <>
	struct TypeAt <14>
	{
		typedef T15 Type;
	};

};

template <typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, 
          typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, 
          typename T13, typename T14, typename T15, typename T16>
class Container
{
public:

	typedef Container<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16> 
	                  SelfType;

	T1    m_p1;
	T2    m_p2;
	T3    m_p3;
	T4    m_p4;
	T5    m_p5;
	T6    m_p6;
	T7    m_p7;
	T8    m_p8;
	T9    m_p9;
	T10   m_p10;
	T11   m_p11;
	T12   m_p12;
	T13   m_p13;
	T14   m_p14;
	T15   m_p15;
	T16   m_p16;

	Container(T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, T8 p8, T9 p9, T10 p10, T11 p11, 
	          T12 p12, T13 p13, T14 p14, T15 p15, T16 p16)
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

	Container(const SelfType &con)
	{
		Copy(con);
	}

	SelfType operator=(const SelfType &con)
	{
		Copy(con);

		return *this;
	}

	void Copy(const SelfType &con)
	{
		m_p1  = con.m_p1;
		m_p2  = con.m_p2;
		m_p3  = con.m_p3;
		m_p4  = con.m_p4;
		m_p5  = con.m_p5;
		m_p6  = con.m_p6;
		m_p7  = con.m_p7;
		m_p8  = con.m_p8;
		m_p9  = con.m_p9;
		m_p10 = con.m_p10;
		m_p11 = con.m_p11;
		m_p12 = con.m_p12;
		m_p13 = con.m_p13;
		m_p14 = con.m_p14;
		m_p15 = con.m_p15;
		m_p16 = con.m_p16;
	}

	Size GetCount()
	{
		return 16;
	}

	template <Size cIndex>
	struct TypeAt
	{
	};

	template <>
	struct TypeAt<0>
	{
		typedef T1 Type;
	};

	template <>
	struct TypeAt<1>
	{
		typedef T2 Type;
	};

	template <>
	struct TypeAt<2>
	{
		typedef T3 Type;
	};

	template <>
	struct TypeAt<3>
	{
		typedef T4 Type;
	};

	template <>
	struct TypeAt<4>
	{
		typedef T5 Type;
	};

	template <>
	struct TypeAt<5>
	{
		typedef T6 Type;
	};

	template <>
	struct TypeAt<6>
	{
		typedef T7 Type;
	};

	template <>
	struct TypeAt<7>
	{
		typedef T8 Type;
	};

	template <>
	struct TypeAt<8>
	{
		typedef T9 Type;
	};

	template <>
	struct TypeAt <9>
	{
		typedef T10 Type;
	};

	template <>
	struct TypeAt <10>
	{
		typedef T11 Type;
	};

	template <>
	struct TypeAt <11>
	{
		typedef T12 Type;
	};

	template <>
	struct TypeAt <12>
	{
		typedef T13 Type;
	};

	template <>
	struct TypeAt <13>
	{
		typedef T14 Type;
	};

	template <>
	struct TypeAt <14>
	{
		typedef T15 Type;
	};

	template <>
	struct TypeAt <15>
	{
		typedef T16 Type;
	};

};

}//namespace Util

}//namespace CX

