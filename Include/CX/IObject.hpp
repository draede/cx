/* 
 * The author disclaims copyright to this source code.
 */ 

#pragma once


#include "CX/Types.hpp"
#include "CX/IInterfaceList.hpp"


#define CX_DECLARE_OBJECT(name)                                                                                        \
	static const CX::Char *OBJECT() { return name; }                                                                    \
	                                                                                                                    \
	virtual const CX::Char *GetObjectName() const { return OBJECT(); }


namespace CX
{

class IInterface;
class IObjectManager;
class IObject
{
public:

	virtual ~IObject() { }

	virtual const Char *GetObjectName() const = 0;

	virtual Bool Implements(const Char *szInterface) const = 0;

	template <typename T>
	Bool Implements() const
	{
		return Implements(T::INTERFACE());
	}

	virtual IInterface *Acquire(const Char *szInterface) = 0;

	virtual const IInterface *Acquire(const Char *szInterface) const = 0;

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

	virtual long Retain() = 0;

	virtual long Retain() const = 0;

	virtual long Release() = 0;

	virtual long Release() const = 0;

	virtual void SetObjectManager(IObjectManager *pObjectManager) = 0;

	virtual IObjectManager *GetObjectManager() = 0;

	virtual const IObjectManager *GetObjectManager() const = 0;

	virtual void SetParent(IObject *pParent) = 0;

	virtual IObject *GetParent() = 0;

	virtual const IObject *GetParent() const = 0;

	virtual void GetImplementedInterfaces(IInterfaceList *pListInterfaces) const = 0;

};

}//namespace CX
