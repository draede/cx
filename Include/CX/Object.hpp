/* 
 * CX - C++ framework for general purpose development
 *
 * https://github.com/draede/cx
 * 
 * Copyright (C) 2014 - 2017 draede, draede [at] outlook [dot] com
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

	virtual void GetImplementedInterfaces(InterfacesVector &vectorInterfaces) const
	{
		GetImplementedInterfacesHelper<INTERFACES...>::GetImplementedInterfaces(vectorInterfaces);
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

		static void GetInterfaces(InterfacesVector &vectorInterfaces)
		{
		}

	};

	template <typename FIRST, typename... REST>
	struct GetImplementedInterfacesHelper<FIRST, REST...>
	{

		static void GetInterfaces(InterfacesVector &vectorInterfaces)
		{
			vectorInterfaces.push_back(FIRST::INTERFACE());
			GetImplementedInterfacesHelper<REST...>::GetInterfaces(vectorInterfaces);
		}

	};

};

}//namespace CX
