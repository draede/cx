/* 
 * The author disclaims copyright to this source code.
 */ 

#pragma once


#include "CX/IObject.hpp"
#include <string.h>
#include <new>
#ifdef CX_OS_WINDOWS
	#include "CX/C/Platform/Windows/windows.h"
#endif


namespace CX
{

template <typename CLASS, typename... INTERFACES>
class Object : public IObject, public INTERFACES...
{
public:

	static CLASS *Create()
	{
		return new (std::nothrow) CLASS();
	}

	virtual Bool Implements(const Char *szInterface) const
	{
		return ImplementsHelper<INTERFACES...>::Implements(szInterface);
	}
	
	template <typename T>
	Bool Implements() const
	{
		return Implements(T::INTERFACE());
	}

	virtual long Retain()
	{
#ifdef _WIN32
		return InterlockedIncrement(&m_cRefCount);
#else
		return __sync_add_and_fetch(&m_cRefCount, 1);
#endif
	}

	virtual long Retain() const
	{
#ifdef _WIN32
		return InterlockedIncrement(&m_cRefCount);
#else
		return __sync_add_and_fetch(&m_cRefCount, 1);
#endif
	}

	virtual IInterface *Acquire(const Char *szInterface)
	{
		IInterface *pInstance = AcquireHelper<INTERFACES...>::Acquire(this, szInterface);

		if (NULL != pInstance)
		{
			Retain();
			pInstance->SetObject(this);
		}

		return pInstance;
	}

	virtual const IInterface *Acquire(const Char *szInterface) const
	{
		const IInterface *pInstance = AcquireHelper<INTERFACES...>::Acquire(this, szInterface);

		if (NULL != pInstance)
		{
			Retain();
			pInstance->SetObject(this);
		}

		return pInstance;
	}

	template <typename T>
	T *Acquire()
	{
		return (T *)Acquire(T::INTERFACE());
	}

	template <typename T>
	const T *Acquire() const
	{
		return (const T *)Acquire(T::INTERFACE());
	}

	virtual long Release()
	{
		long cRefCount;

#ifdef _WIN32
		if (0 == (cRefCount = InterlockedDecrement(&m_cRefCount)))
#else
		if (0 == (cRefCount = __sync_sub_and_fetch(&m_cRefCount, 1)))
#endif
		{
			delete this;
		}

		return cRefCount;
	}

	virtual long Release() const
	{
		long cRefCount;

#ifdef _WIN32
		if (0 == (cRefCount = InterlockedDecrement(&m_cRefCount)))
#else
		if (0 == (cRefCount = __sync_sub_and_fetch(&m_cRefCount, 1)))
#endif
		{
			delete this;
		}

		return cRefCount;
	}

	virtual void GetImplementedInterfaces(IInterfaceList *pListInterfaces) const
	{
		GetImplementedInterfacesHelper<INTERFACES...>::GetImplementedInterfaces(pListInterfaces);
	}

protected:

	Object()
	{
		m_cRefCount = 1;
	}

	~Object()
	{
	}

private:

	mutable volatile long m_cRefCount;

	template <typename... Args>
	struct ImplementsHelper;

	template <>
	struct ImplementsHelper<>
	{

		static Bool Implements(const Char *szInterface)
		{
			return False;
		}

	};

	template <typename FIRST, typename... REST>
	struct ImplementsHelper<FIRST, REST...>
	{

		static Bool Implements(const Char *szInterface)
		{
			if (0 == strcmp(szInterface, FIRST::INTERFACE()))
			{
				return True;
			}
			else
			{
				return ImplementsHelper<REST...>::Implements(szInterface);
			}
		}

	};

	template <typename... Args>
	struct AcquireHelper;

	template <>
	struct AcquireHelper<>
	{

		static IInterface *Acquire(Object *pObject, const Char *szInterface)
		{
			return NULL;
		}

		static const IInterface *Acquire(const Object *pObject, const Char *szInterface)
		{
			return NULL;
		}

	};

	template <typename FIRST, typename... REST>
	struct AcquireHelper<FIRST, REST...>
	{

		static IInterface *Acquire(Object *pObject, const Char *szInterface)
		{
			if (0 == strcmp(szInterface, FIRST::INTERFACE()))
			{
				return (FIRST *)pObject;
			}
			else
			{
				return AcquireHelper<REST...>::Acquire(pObject, szInterface);
			}
		}

		static const IInterface *Acquire(const Object *pObject, const Char *szInterface)
		{
			if (0 == strcmp(szInterface, FIRST::INTERFACE()))
			{
				return (const FIRST *)pObject;
			}
			else
			{
				return AcquireHelper<REST...>::Acquire(pObject, szInterface);
			}
		}

	};

	template <typename... Args>
	struct GetImplementedInterfacesHelper;

	template <>
	struct GetImplementedInterfacesHelper<>
	{

		static void GetImplementedInterfaces(IInterfaceList *pListInterfaces)
		{
		}

	};

	template <typename FIRST, typename... REST>
	struct GetImplementedInterfacesHelper<FIRST, REST...>
	{

		static void GetImplementedInterfaces(IInterfaceList *pListInterfaces)
		{
			pListInterfaces->AddInterface(FIRST::INTERFACE());
			GetImplementedInterfacesHelper<REST...>::GetImplementedInterfaces(pListInterfaces);
		}

	};

};

}//namespace CX
