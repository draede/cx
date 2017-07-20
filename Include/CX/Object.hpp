/* 
 * The author disclaims copyright to this source code.
 */ 

#pragma once


#include "CX/Types.hpp"
#include "CX/IObject.hpp"
#include <string.h>
#include <stdio.h>
#include <new>
#include "CX/C/Platform/Windows/windows.h"


namespace CX
{

template <typename CLASS, typename... INTERFACES>
class Object : public IObject, public INTERFACES...
{
public:

	static CLASS *Create(IObject *pParent = NULL)
	{
		CLASS *pInstance = new (std::nothrow) CLASS();

		if (NULL != pParent && NULL != pInstance)
		{
			pInstance->SetParent(pParent);
		}

		return pInstance;
	}

	virtual Bool Implements(const Char *szInterface) const
	{
		if (ImplementsHelper<INTERFACES...>::Implements(szInterface))
		{
			return True;
		}
		
		BaseObject *pBaseObject = m_pFirstBaseObject;

		while (NULL != pBaseObject)
		{
			if (pBaseObject->pObject->Implements(szInterface))
			{
				return True;
			}
			pBaseObject = pBaseObject->pNext;
		}

		return False;
	}
	
	template <typename T>
	Bool Implements() const
	{
		return Implements(T::INTERFACE());
	}

	virtual long Retain()
	{
		if (NULL == m_pParent)
		{
#ifdef _WIN32
			return InterlockedIncrement(&m_cRefCount);
#else
			return __sync_add_and_fetch(&m_cRefCount, 1);
#endif
		}
		else
		{
			return m_pParent->Retain();
		}
	}

	virtual long Retain() const
	{
		if (NULL == m_pParent)
		{
#ifdef _WIN32
			return InterlockedIncrement(&m_cRefCount);
#else
			return __sync_add_and_fetch(&m_cRefCount, 1);
#endif
		}
		else
		{
			return m_pParent->Retain();
		}
	}

	virtual IInterface *Acquire(const Char *szInterface)
	{
		IInterface *pInstance = AcquireHelper<INTERFACES...>::Acquire(this, szInterface);

		if (NULL != pInstance)
		{
			pInstance->SetObject(this);
			Retain();
		}
		else
		{
			BaseObject *pBaseObject = m_pFirstBaseObject;

			while (NULL != pBaseObject)
			{
				if (NULL != (pInstance = pBaseObject->pObject->Acquire(szInterface)))
				{
					break;
				}
				pBaseObject = pBaseObject->pNext;
			}
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
		else
		{
			BaseObject *pBaseObject = m_pFirstBaseObject;

			while (NULL != pBaseObject)
			{
				if (NULL != (pInstance = pBaseObject->pObject->Acquire(szInterface)))
				{
					break;
				}
				pBaseObject = pBaseObject->pNext;
			}
		}
		if (NULL != pInstance)
		{
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
		if (NULL == m_pParent)
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
		else
		{
			return m_pParent->Release();
		}
	}

	virtual long Release() const
	{
		if (NULL == m_pParent)
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
		else
		{
			return m_pParent->Release();
		}
	}

	virtual void GetImplementedInterfaces(IInterfaceList *pListInterfaces) const
	{
		GetImplementedInterfacesHelper<INTERFACES...>::GetImplementedInterfaces(pListInterfaces);
	
		BaseObject *pBaseObject = m_pFirstBaseObject;

		while (NULL != pBaseObject)
		{
			pBaseObject->pObject->GetImplementedInterfaces(pListInterfaces);
			pBaseObject = pBaseObject->pNext;
		}
	}

	//===

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

	//===

	virtual void SetParent(IObject *pParent)
	{
		m_pParent = pParent;
	}

	virtual IObject *GetParent()
	{
		return m_pParent;
	}

	virtual const IObject *GetParent() const
	{
		return m_pParent;
	}

	template <typename... OBJECTS>
	void AddBaseObjects()
	{
		AddBaseObjectsHelper<OBJECTS...>::AddBaseObjects(this);
	}

protected:

	Object()
	{
		m_pParent          = NULL;
		m_pFirstBaseObject = NULL;
		m_pLastBaseObject  = NULL;
		m_cRefCount        = 1;
		m_pObjectManager   = NULL;
	}

	~Object()
	{
		BaseObject *pBaseObject;
		BaseObject *pTmpBaseObject;

		pBaseObject = m_pFirstBaseObject;
		while (NULL != pBaseObject)
		{
			pTmpBaseObject = pBaseObject;
			pBaseObject    = pBaseObject->pNext;
			pTmpBaseObject->pObject->SetParent(NULL);
			pTmpBaseObject->pObject->Release();
			delete pTmpBaseObject;
		}
		m_pFirstBaseObject = NULL;
		m_pLastBaseObject  = NULL;
	}

	IObject *GetBaseObject(const Char *szObjectName)
	{
		BaseObject *pBaseObject;

		pBaseObject = m_pFirstBaseObject;
		while (NULL != pBaseObject)
		{
			if (0 == strcmp(szObjectName, pBaseObject->pObject->GetObjectName()))
			{
				return pBaseObject->pObject;
			}
			pBaseObject = pBaseObject->pNext;
		}

		return NULL;
	}

	const IObject *GetBaseObject(const Char *szObjectName) const
	{
		BaseObject *pBaseObject;

		pBaseObject = m_pFirstBaseObject;
		while (NULL != pBaseObject)
		{
			if (0 == strcmp(szObjectName, pBaseObject->pObject->GetObjectName()))
			{
				return pBaseObject->pObject;
			}
			pBaseObject = pBaseObject->pNext;
		}

		return NULL;
	}

	template <typename T>
	T *GetBaseObject()
	{
		return (T *)GetBaseObject(T::OBJECT());
	}

	template <typename T>
	const T *GetBaseObject() const
	{
		return (const T *)GetBaseObject(T::OBJECT());
	}

private:

	struct BaseObject
	{
		IObject    *pObject;
		BaseObject *pNext;
	};

	IObject               *m_pParent;
	BaseObject            *m_pFirstBaseObject;
	BaseObject            *m_pLastBaseObject;
	IObjectManager        *m_pObjectManager;
	mutable volatile long m_cRefCount;

	template <typename... Args>
	struct ImplementsHelper;

	template <>
	struct ImplementsHelper<>
	{

		static Bool Implements(const Char *szInterface)
		{
			(void)szInterface;

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
			(void)pObject;
			(void)szInterface;

			return NULL;
		}

		static const IInterface *Acquire(const Object *pObject, const Char *szInterface)
		{
			(void)pObject;
			(void)szInterface;

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
			(void)pListInterfaces;
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

	template <typename... Args>
	struct AddBaseObjectsHelper;

	template <>
	struct AddBaseObjectsHelper<>
	{

		static void AddBaseObjects(Object *pObject)
		{
			(void)pObject;
		}

	};

	template <typename FIRST, typename... REST>
	struct AddBaseObjectsHelper<FIRST, REST...>
	{

		static void AddBaseObjects(Object *pObject)
		{
			BaseObject *pBaseObject;

			if (NULL != (pBaseObject = new (std::nothrow) BaseObject()))
			{
				if (NULL != (pBaseObject->pObject = FIRST::Create(pObject)))
				{
					pBaseObject->pNext = NULL;
					if (NULL == pObject->m_pLastBaseObject)
					{
						pObject->m_pFirstBaseObject = pObject->m_pLastBaseObject = pBaseObject;
					}
					else
					{
						pObject->m_pLastBaseObject->pNext = pBaseObject;
						pObject->m_pLastBaseObject        = pBaseObject;
					}
				}
				else
				{
					delete pBaseObject;
				}
			}
			AddBaseObjectsHelper<REST...>::AddBaseObjects(pObject);
		}

	};

};

}//namespace CX
