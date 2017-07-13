/* 
 * The author disclaims copyright to this source code.
 */ 

#pragma once


#include "CX/IObject.hpp"
#include <string.h>
#include <stdio.h>
#include <new>
#ifdef _WIN32
	#ifndef WIN32_LEAN_AND_MEAN
		#define WIN32_LEAN_AND_MEAN
	#endif
	#include <windows.h>
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

	virtual bool Implements(const char *szInterface) const
	{
		return ImplementsHelper<INTERFACES...>::Implements(szInterface);
	}
	
	template <typename T>
	bool Implements() const
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

	virtual IInterface *Acquire(const char *szInterface)
	{
		IInterface *pInstance = AcquireHelper<INTERFACES...>::Acquire(this, szInterface);

		if (NULL != pInstance)
		{
			Retain();
			pInstance->SetObject(this);
		}

		return pInstance;
	}

	virtual const IInterface *Acquire(const char *szInterface) const
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

	virtual void SetObjectManager(IObjectManager *pObjectManager)
	{
		m_pObjectManager = pObjectManager;
	}

	virtual IObjectManager *GetObjectManager()
	{
		return m_pObjectManager;
	}

	virtual const IObjectManager *GetObjectManager() const
	{
		return m_pObjectManager;
	}

	virtual void GetImplementedInterfaces(IInterfaceList *pListInterfaces) const
	{
		GetImplementedInterfacesHelper<INTERFACES...>::GetImplementedInterfaces(pListInterfaces);
	}

protected:

	Object()
	{
		m_cRefCount      = 1;
		m_pObjectManager = NULL;
	}

	~Object()
	{
	}

private:

	IObjectManager        *m_pObjectManager;
	mutable volatile long m_cRefCount;

	template <typename... Args>
	struct ImplementsHelper;

	template <>
	struct ImplementsHelper<>
	{

		static bool Implements(const char *szInterface)
		{
			return false;
		}

	};

	template <typename FIRST, typename... REST>
	struct ImplementsHelper<FIRST, REST...>
	{

		static bool Implements(const char *szInterface)
		{
			if (0 == strcmp(szInterface, FIRST::INTERFACE()))
			{
				return true;
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

		static IInterface *Acquire(Object *pObject, const char *szInterface)
		{
			return NULL;
		}

		static const IInterface *Acquire(const Object *pObject, const char *szInterface)
		{
			return NULL;
		}

	};

	template <typename FIRST, typename... REST>
	struct AcquireHelper<FIRST, REST...>
	{

		static IInterface *Acquire(Object *pObject, const char *szInterface)
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

		static const IInterface *Acquire(const Object *pObject, const char *szInterface)
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
