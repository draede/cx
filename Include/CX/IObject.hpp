/* 
 * The author disclaims copyright to this source code.
 */ 

#pragma once


#include "CX/Types.hpp"
#include "CX/IInterface.hpp"
#include "CX/IInterfaceList.hpp"


namespace CX
{

class IObject : public IInterface
{
public:

	CX_DECLARE_INTERFACE("IObject")

	virtual Bool Implements(const Char *szInterface) const = 0;

	template <typename T>
	Bool Implements() const
	{
		return Implements(T::INTERFACE());
	}

	virtual long Retain() = 0;

	virtual long Retain() const = 0;

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

	virtual long Release() = 0;

	virtual long Release() const = 0;

	virtual void GetImplementedInterfaces(IInterfaceList *pListInterfaces) const = 0;

};

}//namespace CX
