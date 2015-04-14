
#include "CX/IObject.hpp"
#include "CX/Mem.hpp"


namespace CX
{

IObject::~IObject()
{
}

#ifdef _MSC_VER
void *IObject::operator new(std::size_t cbSize)
#else
void *IObject::operator new(std::size_t cbSize) throw (std::bad_alloc)
#endif

{
	void *pPtr;

	if (NULL != (pPtr = Mem::Alloc(cbSize)))
	{
		return pPtr;
	}
	else
	{
		throw std::bad_alloc();
	}
}

void IObject::operator delete(void *pPtr, std::size_t cbSize) throw ()
{
	cbSize;

	Mem::Free(pPtr);
}

void *IObject::operator new(std::size_t cbSize, const std::nothrow_t &) throw ()
{
	void *pPtr;

	if (NULL != (pPtr = Mem::Alloc(cbSize)))
	{
		return pPtr;
	}
	else
	{
		return NULL;
	}
}

void IObject::operator delete(void *pPtr, const std::nothrow_t &) throw()
{
	Mem::Free(pPtr);
}

}//namespace CX
