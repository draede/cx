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


#include "CX/Alloc.h"
#include <new>
#include <cstddef>
#include <limits>


#undef max


namespace CX
{

template <class T> 
class STLAlloc;

template <> 
class STLAlloc <void> 
{
public:

	typedef void            *pointer;
	
	typedef const void      *const_pointer;
	
	typedef void            value_type;
	
	template <class U> 
	struct rebind 
	{
		typedef STLAlloc<U>  other;
	};
};

template <class T> 
class STLAlloc 
{
public:

	typedef size_t          size_type;
	
	typedef ptrdiff_t       difference_type;
	
	typedef T               *pointer;
	
	typedef const T         *const_pointer;
	
	typedef T               &reference;
	
	typedef const T         &const_reference;
	
	typedef T               value_type;
	
	template <class U> 
	struct rebind 
	{
		typedef STLAlloc<U>  other;
	};

	STLAlloc()
	{
	}

	STLAlloc(const STLAlloc &al)
	{
		al;
	}
	
	template <class U> 
	STLAlloc(const STLAlloc<U> &al)
	{
		al;
	}
	
	~STLAlloc()
	{
	}

	pointer address(reference x) const
	{
		return &x;
	}
	
	const_pointer address(const_reference x) const
	{
		return &x;
	}

	pointer allocate(size_type n, STLAlloc<void>::const_pointer hint = 0)
	{
		hint;

		return (pointer)Alloc(n * sizeof(T));
	}

	void deallocate(pointer p, size_type n)
	{
		n;

		Free(p);
	}

	size_type max_size() const
	{
		return std::numeric_limits<std::size_t>::max() / sizeof(T);
	}

	void construct(pointer p, const T &val)
	{
		new ((void *)p) T(val);
	}

	void destroy(pointer p)
	{
		p; //wth

		p->~T();
	}

};

template <class T1, class T2>
bool operator==(const STLAlloc<T1 >&p1, const STLAlloc<T2> &p2)
{
	p1;
	p2;

	return true;
}

template <class T1, class T2>
bool operator!=(const STLAlloc<T1> &p1, const STLAlloc<T2> &p2)
{
	p1;
	p2;

	return false;
}

}//namespace CX
