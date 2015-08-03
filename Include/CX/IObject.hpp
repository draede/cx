
#pragma once


#include "CX/Types.hpp"
#include "CX/APIDefs.hpp"
#include <cstddef>
#include <new>


namespace CX
{

class CX_API IObject
{
public:

	virtual ~IObject();

#ifdef _MSC_VER
	static void *operator new (std::size_t cbSize);
#else
	static void *operator new (std::size_t cbSize) throw (std::bad_alloc);
#endif

	static void operator delete(void *pMem, std::size_t cbSize) throw ();

	static void *operator new(std::size_t cbSize, const std::nothrow_t &) throw ();

	static void operator delete(void *pMem, const std::nothrow_t &) throw();

	inline static void *operator new(std::size_t cbSize, void *pMem)
	{
		return ::operator new(cbSize, pMem);
	}

	inline static void operator delete(void *pPtr, void *pMem)
	{
		::operator delete (pPtr, pMem);
	}

};

}//namespace CX
