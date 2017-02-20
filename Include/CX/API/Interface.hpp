/* 
 * The author disclaims copyright to this source code.
 */ 
 
#ifndef __CX__INTERFACE__H__
#define __CX__INTERFACE__H__


#include "CX/API/IInterface.hpp"
#ifdef WIN32
#include <windows.h>
#endif
#include <string>
#include <map>


template <typename INTERFACE>
class CX_Interface : public INTERFACE
{
public:

	virtual long Retain()
	{
		if (NULL != m_pParent)
		{
			m_pParent->Retain();
		}
#ifdef WIN32
		return InterlockedIncrement(&m_cRefCount);
#else
		return __sync_fetch_and_add(&m_cRefCount, 1);
#endif
	}

	virtual long Release()
	{
		long cRefCount;

		if (NULL != m_pParent)
		{
			m_pParent->Release();
		}
#ifdef WIN32
		if ((long)m_cAllInterfaces == (cRefCount = InterlockedDecrement(&m_cRefCount)))
#else
		if ((long)m_cAllInterfaces == (cRefCount = __sync_fetch_and_sub(&m_cRefCount, 1)))
#endif
		{
			for (InterfacesMap::iterator iter = m_mapInterfaces.begin(); iter != m_mapInterfaces.end(); ++iter)
			{
				iter->second->Release();
			}
			m_mapInterfaces.clear();

			delete this;
		}

		return cRefCount;
	}

	virtual CX_IInterface *AcquireInterface(const char *szName)
	{
		InterfacesMap::iterator iter;

		if (m_mapInterfaces.end() != (iter = m_mapInterfaces.find(szName)))
		{
			iter->second->Retain();

			return iter->second;
		}
		else
		{
			return NULL;
		}
	}

	virtual bool ImplementsInterface(const char *szName) const
	{
		return (m_mapInterfaces.end() != m_mapInterfaces.find(szName));
	}

	virtual size_t GetDirectInterfacesCount() const
	{
		return m_mapInterfaces.size();
	}

	virtual size_t GetAllInterfacesCount() const
	{
		return m_cAllInterfaces;
	}

protected:

	virtual ~CX_Interface() { }

	void Init(CX_IInterface *pIParent)
	{
		m_pParent        = pIParent;
		m_cRefCount      = 1;
		m_cAllInterfaces = 0;
	}

	void AddInterface(CX_IInterface *pInterface)
	{
		m_cAllInterfaces += 1 + pInterface->GetAllInterfacesCount();
		m_mapInterfaces[pInterface->GetName()] = pInterface;
		Retain();
	}

private:

	typedef std::map<std::string, CX_IInterface *>   InterfacesMap;

	CX_IInterface   *m_pParent;
	volatile long   m_cRefCount;
	InterfacesMap   m_mapInterfaces;
	size_t          m_cAllInterfaces;

	virtual void ReleaseChildInterface(const char *szName)
	{
		Retain();
	}

};

#define CX_BEGIN_INTERFACE(CLASSNAME)                                                                                  \
	static CX_IInterface *Create(CX_IInterface *pParent = NULL)                                                         \
	{                                                                                                                   \
		CLASSNAME *pInstance = new (std::nothrow) CLASSNAME();                                                           \
                                                                                                                       \
		pInstance->Init(pParent);

#define CX_END_INTERFACE()                                                                                             \
                                                                                                                       \
		return pInstance;                                                                                                \
	}

#define CX_IMPLEMENT_INTERFACE(CLASSNAME)                                                                                           \
		pInstance->AddInterface(CLASSNAME::Create(pInstance));


#endif

