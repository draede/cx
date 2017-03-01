/* 
 * The author disclaims copyright to this source code.
 */ 
 
#pragma once


#include "IObject.hpp"


namespace CX
{

class IInterface
{
public:

	struct Null { };

	virtual const char *GetName() const = 0;

	long Release() const
	{
		return m_pObject->Release();
	}

	void SetObject(IObject *pObject)
	{
		m_pObject = pObject;
	}

protected:

	virtual ~IInterface() { }

private:

	IObject *m_pObject;

};

}//namespace CX

#define CX_DECLARE_INTERFACE(name)                                                                                     \
	static const char *NAME() { return name; }                                                                          \
	                                                                                                                    \
	virtual const char *GetName() const                                                                                 \
	{                                                                                                                   \
		return NAME();                                                                                                   \
	}
