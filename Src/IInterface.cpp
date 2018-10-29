/* 
 * The author disclaims copyright to this source code.
 */ 

#include "CX/precomp.hpp"
#include "CX/IInterface.hpp"
#include "CX/IObject.hpp"
#include <stdio.h>


namespace CX
{

IInterface::IInterface()
{
	m_pObject      = NULL;
	m_pConstObject = NULL;
}

IInterface::~IInterface()
{
}

long IInterface::Retain()
{
	return m_pObject->Retain();
}

long IInterface::Retain() const
{
	return m_pConstObject->Retain();
}

long IInterface::Release()
{
	return m_pObject->Release();
}

long IInterface::Release() const
{
	return m_pConstObject->Release();
}

void IInterface::SetObject(IObject *pObject)
{
	if (NULL != m_pObject)
	{
		return;
	}
	m_pObject = pObject;
}

void IInterface::SetObject(const IObject *pConstObject) const
{
	if (NULL != m_pConstObject)
	{
		return;
	}
	m_pConstObject = pConstObject;
}

}//namespace CX
