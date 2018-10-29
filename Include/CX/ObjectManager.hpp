/* 
 * The author disclaims copyright to this source code.
 */ 

#pragma once


#include "CX/Types.hpp"
#include "CX/IObjectManager.hpp"
#include "CX/Object.hpp"
#include "CX/Sys/Lock.hpp"
#include "CX/Vector.hpp"
#include "CX/String.hpp"


namespace CX
{

class ObjectManager : public Object<ObjectManager, IObjectManager>
{
public:

	CX_DECLARE_OBJECT("CX.ObjectManager")

	ObjectManager()
	{
	}

	~ObjectManager()
	{
		RemoveAllObjects();
	}

	//Retain will be called on pObject on success
	//During this call the internal objects list will be locked
	virtual StatusCode AddObject(IObject *pObject)
	{
		CX::Sys::Locker         locker(&m_lockObjects);
		ObjectsVector::iterator iter;

		if (m_vectorObjects.end() != (iter = FindObject(pObject->GetObjectName())))
		{
			return Status_Duplicate;
		}

		m_vectorObjects.push_back(pObject);
		pObject->Retain();
		pObject->SetObjectManager(this);

		return Status_OK;
	}

	//Release will be called on object
	//During this call the internal objects list will be locked
	virtual StatusCode RemoveObject(const Char *szObjectName)
	{
		CX::Sys::Locker         locker(&m_lockObjects);
		ObjectsVector::iterator iter;

		if (m_vectorObjects.end() == (iter = FindObject(szObjectName)))
		{
			return Status_NotFound;
		}

		(*iter)->SetObjectManager(NULL);
		(*iter)->Release();
		m_vectorObjects.erase(iter);

		return Status_OK;
	}

	//Release will be called on every object
	//During this call the internal objects list will be locked
	virtual StatusCode RemoveAllObjects()
	{
		CX::Sys::Locker           locker(&m_lockObjects);
		ObjectsVector::iterator   iter;
		
		while (!m_vectorObjects.empty())
		{
			iter = m_vectorObjects.end();
			iter--;

			(*iter)->SetObjectManager(NULL);
			(*iter)->Release();

			m_vectorObjects.erase(iter);
		}

		return Status_OK;
	}

	//After this call the internal objects list will be locked
	virtual StatusCode BeginEnum(IIterator **ppIterator)
	{
		Iterator *pIterator;

		if (NULL == (pIterator = new (std::nothrow) Iterator(this)))
		{
			return Status_MemAllocFailed;
		}
		*ppIterator = pIterator;
		m_lockObjects.Enter();

		return Status_OK;
	}

	//After this call the internal objects list will be unlocked
	virtual StatusCode EndEnum(IIterator *pIterator)
	{
		if (NULL == pIterator)
		{
			return Status_InvalidArg;
		}
		delete pIterator;
		m_lockObjects.Leave();

		return Status_OK;
	}

	//After this call the internal objects list will be locked
	virtual StatusCode BeginEnum(IConstIterator **ppConstIterator) const
	{
		ConstIterator *pConstIterator;

		if (NULL == (pConstIterator = new (std::nothrow) ConstIterator(this)))
		{
			return Status_MemAllocFailed;
		}
		*ppConstIterator = pConstIterator;
		m_lockObjects.Enter();

		return Status_OK;
	}

	//After this call the internal objects list will be unlocked
	virtual StatusCode EndEnum(IConstIterator *pConstIterator) const
	{
		if (NULL == pConstIterator)
		{
			return Status_InvalidArg;
		}
		delete pConstIterator;
		m_lockObjects.Leave();

		return Status_OK;
	}

	//During this call the internal objects list will be locked
	virtual StatusCode EnumObjects(const std::function<StatusCode (IObject *)> &pfnEnumObjects)
	{
		CX::Sys::Locker           locker(&m_lockObjects);
		StatusCode                nStatus;

		for (ObjectsVector::iterator iter = m_vectorObjects.begin(); iter != m_vectorObjects.end(); ++iter)
		{
			if (CXNOK(nStatus = pfnEnumObjects(*iter)))
			{
				return nStatus;
			}
		}

		return Status_OK;
	}

	//During this call the internal objects list will be locked
	virtual StatusCode EnumObjects(const std::function<StatusCode (const IObject *)> &pfnEnumConstObjects) const
	{
		CX::Sys::Locker           locker(&m_lockObjects);
		StatusCode                nStatus;

		for (ObjectsVector::const_iterator iter = m_vectorObjects.begin(); iter != m_vectorObjects.end(); ++iter)
		{
			if (CXNOK(nStatus = pfnEnumConstObjects(*iter)))
			{
				return nStatus;
			}
		}

		return Status_OK;
	}

private:

	typedef CX::Vector<IObject *>::Type   ObjectsVector;

	mutable CX::Sys::Lock     m_lockObjects;
	ObjectsVector             m_vectorObjects;

	ObjectsVector::iterator FindObject(const Char *szName)
	{
		ObjectsVector::iterator iter;

		iter = m_vectorObjects.begin();
		while (m_vectorObjects.end() != iter)
		{
			if (0 == strcmp((*iter)->GetObjectName(), szName))
			{
				return iter;
			}
			iter++;
		}

		return m_vectorObjects.end();
	}

	class Iterator : public IIterator
	{
	public:

		Iterator(ObjectManager *pObjectManager)
		{
			m_pObjectManager = pObjectManager;
			m_bInitialized   = False;
		}

		virtual Bool Next(IObject **ppObject)
		{
			if (!m_bInitialized)
			{
				m_iter         = m_pObjectManager->m_vectorObjects.begin();
				m_bInitialized = True;
			}
			else
			{
				if (m_pObjectManager->m_vectorObjects.end() == m_iter)
				{
					return False;
				}

				m_iter++;
			}
			if (m_pObjectManager->m_vectorObjects.end() == m_iter)
			{
				return False;
			}
			*ppObject = *m_iter;

			return True;
		}

	private:

		ObjectManager                            *m_pObjectManager;
		ObjectManager::ObjectsVector::iterator   m_iter;
		Bool                                     m_bInitialized;

	};

	class ConstIterator : public IConstIterator
	{
	public:

		ConstIterator(const ObjectManager *pObjectManager)
		{
			m_pObjectManager = pObjectManager;
			m_bInitialized   = False;
		}

		virtual Bool Next(const IObject **ppObject)
		{
			if (!m_bInitialized)
			{
				m_iter         = m_pObjectManager->m_vectorObjects.begin();
				m_bInitialized = True;
			}
			else
			{
				if (m_pObjectManager->m_vectorObjects.end() == m_iter)
				{
					return False;
				}

				m_iter++;
			}
			if (m_pObjectManager->m_vectorObjects.end() == m_iter)
			{
				return False;
			}
			*ppObject = *m_iter;

			return True;
		}

	private:

		const ObjectManager                            *m_pObjectManager;
		ObjectManager::ObjectsVector::const_iterator   m_iter;
		Bool                                           m_bInitialized;
	};

};

}//namespace CX
