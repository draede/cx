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


#include "sparsehash/internal/sparseconfig.h"
#include "CX/Alloc.hpp"


namespace CX
{

namespace SB
{

template<class T>
class SparseHashAllocator 
{
public:

	typedef T           value_type;
	typedef size_t      size_type;
	typedef ptrdiff_t   difference_type;

	typedef T           *pointer;
	typedef const T     *const_pointer;
	typedef T           &reference;
	typedef const T     &const_reference;

	SparseHashAllocator() 
	{
	}

	SparseHashAllocator(const SparseHashAllocator &) 
	{
	}

	~SparseHashAllocator()
	{
	}

	pointer address(reference r) const 
	{ 
		return &r; 
	}

	const_pointer address(const_reference r) const 
	{ 
		return &r; 
	}

	pointer allocate(size_type n, const_pointer = 0) 
	{
		return static_cast<pointer>(Alloc(n * sizeof(value_type)));
	}

	void deallocate(pointer p, size_type) 
	{
		Free(p);
	}

	pointer reallocate(pointer p, size_type n) 
	{
		return static_cast<pointer>(Realloc(p, n * sizeof(value_type)));
	}

	size_type max_size() const  
	{
		return static_cast<size_type>(-1) / sizeof(value_type);
	}

	void construct(pointer p, const value_type& val) 
	{
		new(p) value_type(val);
	}
  
	void destroy(pointer p) 
	{ 
		p;

		p->~value_type(); 
	}

	template <class U>
	SparseHashAllocator(const SparseHashAllocator<U>&) 
	{
	}

	template<class U>
	struct rebind 
	{
		typedef SparseHashAllocator<U> other;
	};

};

template<>
class SparseHashAllocator<void> 
{
public:

	typedef void         value_type;
	typedef size_t       size_type;
	typedef ptrdiff_t    difference_type;
	typedef void         *pointer;
	typedef const void   *const_pointer;

	template<class U>
	struct rebind 
	{
		typedef SparseHashAllocator<U> other;
	};

};

template<class T>
inline bool operator==(const SparseHashAllocator<T> &, const SparseHashAllocator<T> &) 
{
	return true;
}

template<class T>
inline bool operator!=(const SparseHashAllocator<T> &, const SparseHashAllocator<T> &) 
{
	return false;
}

}//namespace SB

}//namespace CX

