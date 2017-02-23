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
#include <new>


template <typename INTERFACE>
class CX_InterfaceHelper : public INTERFACE
{
public:

	typedef void (CX_IInterface::*InterfaceDestroyFunc)();

	virtual void Retain()
	{
		if (NULL == m_pParent)
		{
#ifdef WIN32
			InterlockedIncrement(&m_cRefCount);
#else
			__sync_fetch_and_add(&m_cRefCount, 1);
#endif
		}
		else
		{
			m_pParent->Retain();
		}
	}

	virtual void Release()
	{
		if (NULL == m_pParent)
		{
#ifdef WIN32
			if (0 == InterlockedDecrement(&m_cRefCount))
#else
			if (0 == __sync_fetch_and_sub(&m_cRefCount, 1))
#endif
			{
				Destroy();
			}
		}
		else
		{
			m_pParent->Release();
		}
	}

	virtual CX_IInterface *Acquire(const char *szName)
	{
		InterfacesMap::iterator iter;

		if (m_mapInterfaces.end() == (iter = m_mapInterfaces.find(szName)))
		{
			return NULL;
		}
		Retain();

		return iter->second.pInstance;
	}

	virtual bool Implements(const char *szName) const
	{
		return (m_mapInterfaces.end() != m_mapInterfaces.find(szName));
	}

	void Init(CX_IInterface *pParent)
	{
		m_cRefCount = 1;
		m_pParent   = pParent;
	}

	void AddInterface(CX_IInterface *pInterface, InterfaceDestroyFunc pfnDestroy)
	{
		Interface iface;

		iface.pInstance                        = pInterface;
		iface.pfnDestroy                       = pfnDestroy;
		m_mapInterfaces[pInterface->GetName()] = iface;
	}

	void Destroy()
	{
		for (InterfacesMap::iterator iter = m_mapInterfaces.begin(); iter != m_mapInterfaces.end(); ++iter)
		{
			(iter->second.pInstance->*iter->second.pfnDestroy)();
		}

		delete this;
	}

protected:

	CX_InterfaceHelper()
	{
	}

	virtual ~CX_InterfaceHelper()
	{
	}

private:

	struct Interface
	{
		CX_IInterface          *pInstance;
		InterfaceDestroyFunc   pfnDestroy;
	};

	typedef std::map<std::string, Interface>    InterfacesMap;

	CX_IInterface       *m_pParent;
	long                m_cRefCount;
	InterfacesMap       m_mapInterfaces;

};

#define CX_BEGIN_INTERFACE(CLASSNAME)                                                                                  \
	static CLASSNAME *Create(CX_IInterface *pParent = NULL)                                                             \
	{                                                                                                                   \
		CLASSNAME *pInstance = new (std::nothrow) CLASSNAME();                                                           \
                                                                                                                       \
		pInstance->Init(pParent);

#define CX_END_INTERFACE()                                                                                             \
                                                                                                                       \
		return pInstance;                                                                                                \
	}

#define CX_IMPLEMENT_INTERFACE(CLASSNAME)                                                                              \
		pInstance->AddInterface(CLASSNAME::Create(pInstance), (InterfaceDestroyFunc)&CLASSNAME::Destroy);

#endif

