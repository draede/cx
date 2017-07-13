/* 
 * The author disclaims copyright to this source code.
 */ 

#pragma once


#include "CX/IObject.hpp"
#include <stdlib.h>


#define CX_DECLARE_INTERFACE(name)                                                                                     \
	static const char *INTERFACE() { return name; }                                                                     \
	                                                                                                                    \
	virtual const char *GetInterfaceName() const { return INTERFACE(); }


namespace CX
{

class IObject;
class IInterface
{
public:

	IInterface()
	{
		m_pObject      = NULL;
		m_pConstObject = NULL;
	}

	virtual ~IInterface()
	{
	}

	virtual const char *GetInterfaceName() const = 0;

	virtual long Retain()
	{
		return m_pObject->Retain();
	}

	virtual long Retain() const
	{
		return m_pConstObject->Retain();
	}

	virtual long Release()
	{
		return m_pObject->Release();
	}

	virtual long Release() const
	{
		return m_pConstObject->Release();
	}

	IObject *GetObject()
	{
		return m_pObject;
	}

	const IObject *GetObject() const
	{
		return m_pConstObject;
	}

	void SetObject(IObject *pObject)
	{
		if (NULL != m_pObject)
		{
			return;
		}
		m_pObject = pObject;
	}

	void SetObject(const IObject *pConstObject) const
	{
		if (NULL != m_pConstObject)
		{
			return;
		}
		m_pConstObject = pConstObject;
	}

private:

	IObject               *m_pObject;
	mutable const IObject *m_pConstObject;

};

}//namespace CX

