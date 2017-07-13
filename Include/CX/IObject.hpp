/* 
 * The author disclaims copyright to this source code.
 */ 

#pragma once


#include "CX/IInterfaceList.hpp"


#define CX_DECLARE_OBJECT(name)                                                                                        \
	static const char *OBJECT() { return name; }                                                                        \
	                                                                                                                    \
	virtual const char *GetObjectName() const { return OBJECT(); }


namespace CX
{

class IInterface;
class IObjectManager;
class IObject
{
public:

	virtual ~IObject() { }

	virtual const char *GetObjectName() const = 0;

	virtual bool Implements(const char *szInterface) const = 0;

	template <typename T>
	bool Implements() const
	{
		return Implements(T::INTERFACE());
	}

	virtual IInterface *Acquire(const char *szInterface) = 0;

	virtual const IInterface *Acquire(const char *szInterface) const = 0;

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

	virtual void GetImplementedInterfaces(IInterfaceList *pListInterfaces) const = 0;

};

}//namespace CX
