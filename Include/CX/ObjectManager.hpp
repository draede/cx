/* 
 * The author disclaims copyright to this source code.
 */ 

#pragma once


#include "CX/IObjectManager.hpp"
#include "CX/Object.hpp"
#include "CX/Sys/RWLock.hpp"
#include "CX/Map.hpp"
#include "CX/String.hpp"


namespace CX
{

class ObjectManager : public Object<ObjectManager, IObjectManager>
{
public:

	CX_DECLARE_OBJECT("ObjectManager")

	ObjectManager()
	{
	}

	~ObjectManager()
	{
		RemoveAllObjects();
	}

	//Retain will be called on pObject on success
	//During this call the internal objects list will be locked
	virtual bool AddObject(IObject *pObject)
	{
		Sys::WLocker         locker(&m_rwlObjects);
		ObjectsMap::iterator iter;

		if (m_mapObjects.end() != (iter = m_mapObjects.find(pObject->GetObjectName())))
		{
			return false;
		}
		m_mapObjects[pObject->GetObjectName()] = pObject;
		pObject->Retain();
		pObject->SetObjectManager(this);

		return true;
	}

	//Release will be called on object
	//During this call the internal objects list will be locked
	virtual bool RemoveObject(const char *szObjectName)
	{
		Sys::WLocker         locker(&m_rwlObjects);
		ObjectsMap::iterator iter;

		if (m_mapObjects.end() == (iter = m_mapObjects.find(szObjectName)))
		{
			return false;
		}
		iter->second->SetObjectManager(NULL);
		iter->second->Release();
		m_mapObjects.erase(iter);

		return true;
	}

	//Release will be called on every object
	//During this call the internal objects list will be locked
	virtual bool RemoveAllObjects()
	{
		Sys::WLocker locker(&m_rwlObjects);
		
		for (ObjectsMap::iterator iter = m_mapObjects.begin(); iter != m_mapObjects.end(); ++iter)
		{
			iter->second->SetObjectManager(NULL);
			iter->second->Release();
		}
		m_mapObjects.clear();

		return true;
	}

	//After this call the internal objects list will be locked
	virtual bool BeginEnum(IIterator **ppIterator)
	{
		Iterator *pIterator;

		if (NULL == (pIterator = new (std::nothrow) Iterator(this)))
		{
			return false;
		}
		*ppIterator = pIterator;
		m_rwlObjects.EnterRead();

		return true;
	}

	//After this call the internal objects list will be unlocked
	virtual bool EndEnum(IIterator *pIterator)
	{
		if (NULL == pIterator)
		{
			return false;
		}
		delete pIterator;
		m_rwlObjects.LeaveRead();

		return true;
	}

	//After this call the internal objects list will be locked
	virtual bool BeginEnum(IConstIterator **ppConstIterator) const
	{
		ConstIterator *pConstIterator;

		if (NULL == (pConstIterator = new (std::nothrow) ConstIterator(this)))
		{
			return false;
		}
		*ppConstIterator = pConstIterator;
		m_rwlObjects.EnterRead();

		return true;
	}

	//After this call the internal objects list will be unlocked
	virtual bool EndEnum(IConstIterator *pConstIterator) const
	{
		if (NULL == pConstIterator)
		{
			return false;
		}
		delete pConstIterator;
		m_rwlObjects.LeaveRead();

		return true;
	}

private:

	typedef Map<String, IObject *>::Type   ObjectsMap;

	mutable Sys::RWLock   m_rwlObjects;
	ObjectsMap            m_mapObjects;

	class Iterator : public IIterator
	{
	public:

		Iterator(ObjectManager *pObjectManager)
		{
			m_pObjectManager = pObjectManager;
			m_bInitialized   = false;
		}

		virtual bool Next(IObject **ppObject)
		{
			if (!m_bInitialized)
			{
				m_iter         = m_pObjectManager->m_mapObjects.begin();
				m_bInitialized = true;
			}
			else
			{
				if (m_pObjectManager->m_mapObjects.end() == m_iter)
				{
					return false;
				}

				m_iter++;
			}
			if (m_pObjectManager->m_mapObjects.end() == m_iter)
			{
				return false;
			}
			*ppObject = m_iter->second;

			return true;
		}

	private:

		ObjectManager                         *m_pObjectManager;
		ObjectManager::ObjectsMap::iterator   m_iter;
		bool                                  m_bInitialized;

	};

	class ConstIterator : public IConstIterator
	{
	public:

		ConstIterator(const ObjectManager *pObjectManager)
		{
			m_pObjectManager = pObjectManager;
			m_bInitialized   = false;
		}

		virtual bool Next(const IObject **ppObject)
		{
			if (!m_bInitialized)
			{
				m_iter         = m_pObjectManager->m_mapObjects.begin();
				m_bInitialized = true;
			}
			else
			{
				if (m_pObjectManager->m_mapObjects.end() == m_iter)
				{
					return false;
				}

				m_iter++;
			}
			if (m_pObjectManager->m_mapObjects.end() == m_iter)
			{
				return false;
			}
			*ppObject = m_iter->second;

			return true;
		}

	private:

		const ObjectManager                         *m_pObjectManager;
		ObjectManager::ObjectsMap::const_iterator   m_iter;
		bool                                        m_bInitialized;
	};

};

}//namespace CX
