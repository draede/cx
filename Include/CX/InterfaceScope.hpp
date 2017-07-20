/* 
 * The author disclaims copyright to this source code.
 */ 

#pragma once


#include "CX/Types.hpp"
#include "CX/IInterface.hpp"
#include "CX/IObject.hpp"


namespace CX
{

template <typename T>
class InterfaceScope
{
public:

	InterfaceScope(IObject *pObject)
	{
		m_pInstance = pObject->Acquire<T>();
	}

	~InterfaceScope()
	{
		if (NULL != m_pInstance)
		{
			m_pInstance->Release();
		}
	}

	bool IsOK()
	{
		return m_pInstance;
	}

	T *Get()
	{
		return m_pInstance;
	}

	T *operator->()
	{
		return Get();
	}

private:

	T *m_pInstance;

};

template <typename T>
class ConstInterfaceScope
{
public:

	ConstInterfaceScope(const IObject *pObject)
	{
		m_pInstance = pObject->Acquire<T>();
	}

	~ConstInterfaceScope()
	{
		if (NULL != m_pInstance)
		{
			m_pInstance->Release();
		}
	}

	bool IsOK()
	{
		return m_pInstance;
	}

	const T *Get()
	{
		return m_pInstance;
	}

	const T *operator->()
	{
		return Get();
	}

private:

	const T *m_pInstance;

};

}//namespace CX
