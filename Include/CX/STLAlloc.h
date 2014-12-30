
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

	typedef void			*pointer;
	
	typedef const void	*const_pointer;
	
	typedef void 			value_type;
	
	template <class U> 
	struct rebind 
	{
		typedef STLAlloc <U> other;
	};
};

template <class T> 
class STLAlloc 
{
public:

	typedef size_t			size_type;
	
	typedef ptrdiff_t		difference_type;
	
	typedef T				*pointer;
	
	typedef const T		*const_pointer;
	
	typedef T				&reference;
	
	typedef const T		&const_reference;
	
	typedef T				value_type;
	
	template <class U> 
	struct rebind 
	{
		typedef STLAlloc <U>	other;
	};

	STLAlloc()
	{
	}

	STLAlloc(const STLAlloc &al)
	{
	}
	
	template <class U> 
	STLAlloc(const STLAlloc<U> &al)
	{
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
		return (pointer)Alloc(n * sizeof(T));
	}

	void deallocate(pointer p, size_type n)
	{
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
		p->~T();
	}

};

template <class T1, class T2>
bool operator==(const STLAlloc<T1 >&p1, const STLAlloc<T2> &p2)
{
	return true;
}

template <class T1, class T2>
bool operator!=(const STLAlloc<T1> &p1, const STLAlloc<T2> &p2)
{
	return false;
}

}//namespace CX
