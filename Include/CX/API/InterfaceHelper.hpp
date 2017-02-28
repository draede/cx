/* 
 * The author disclaims copyright to this source code.
 */ 
 

#pragma once


#include "CX/API/IInterface.hpp"
#ifdef _WIN32
	#include <windows.h>
#endif
#include <string>
#include <map>
#include <new>


namespace CX
{

template <typename INTERFACE>
class InterfaceHelper : public INTERFACE
{
public:

	typedef void (IInterface::*InterfaceDestroyFunc)();

	virtual void Retain() const
	{
		if (NULL == m_pParent)
		{
#ifdef _WIN32
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

	virtual void Release() const
	{
		if (NULL == m_pParent)
		{
#ifdef _WIN32
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

	virtual IInterface *Acquire(const char *szName)
	{
		InterfacesMap::iterator iter;

		if (0 == strcmp(szName, GetName()))
		{
			Retain();

			return (INTERFACE *)this;
		}
		if (m_mapInterfaces.end() == (iter = m_mapInterfaces.find(szName)))
		{
			return NULL;
		}
		Retain();

		return iter->second.pInstance;
	}

	virtual const IInterface *Acquire(const char *szName) const
	{
		InterfacesMap::const_iterator iter;

		if (0 == strcmp(szName, GetName()))
		{
			Retain();

			return (const INTERFACE *)this;
		}
		if (m_mapInterfaces.end() == (iter = m_mapInterfaces.find(szName)))
		{
			return NULL;
		}
		Retain();

		return iter->second.pInstance;
	}

	virtual bool Implements(const char *szName) const
	{
		if (0 == strcmp(szName, GetName()))
		{
			return True;
		}

		return (m_mapInterfaces.end() != m_mapInterfaces.find(szName));
	}

	void Init(IInterface *pParent)
	{
		m_cRefCount = 1;
		m_pParent   = pParent;
	}

	void AddInterface(IInterface *pInterface, InterfaceDestroyFunc pfnDestroy)
	{
		Interface iface;

		iface.pInstance                        = pInterface;
		iface.pfnDestroy                       = pfnDestroy;
		m_mapInterfaces[pInterface->GetName()] = iface;
	}

	void Destroy() const
	{
		for (InterfacesMap::const_iterator iter = m_mapInterfaces.begin(); iter != m_mapInterfaces.end(); ++iter)
		{
			(iter->second.pInstance->*iter->second.pfnDestroy)();
		}

		delete this;
	}

protected:

	InterfaceHelper()
	{
	}

	virtual ~InterfaceHelper()
	{
	}

private:

	struct Interface
	{
		IInterface             *pInstance;
		InterfaceDestroyFunc   pfnDestroy;
	};

	typedef std::map<std::string, Interface>    InterfacesMap;

	mutable long        m_cRefCount;
	IInterface          *m_pParent;
	InterfacesMap       m_mapInterfaces;

};

}//namespace CX

#define CX_BEGIN_INTERFACE(CLASSNAME)                                                                                  \
	static CLASSNAME *Create(IInterface *pParent = NULL)                                                                \
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

