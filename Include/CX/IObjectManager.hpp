/* 
 * The author disclaims copyright to this source code.
 */ 

#pragma once


#include "CX/IInterface.hpp"


namespace CX
{

class IObjectManager : public IInterface
{
public:

	CX_DECLARE_INTERFACE("ObjectManager")

	class IIterator
	{
	public:

		virtual ~IIterator() { }

		virtual bool Next(IObject **ppObject) = 0;

	};

	class IConstIterator
	{
	public:

		virtual ~IConstIterator() { }

		virtual bool Next(const IObject **ppObject) = 0;

	};

	virtual ~IObjectManager() { }

	//Retain will be called on pObject on success
	//During this call the internal objects list will be locked
	virtual bool AddObject(IObject *pObject) = 0;

	//Release will be called on object
	//During this call the internal objects list will be locked
	virtual bool RemoveObject(const char *szObjectName) = 0;

	//Release will be called on every object
	//During this call the internal objects list will be locked
	virtual bool RemoveAllObjects() = 0;

	//After this call the internal objects list will be locked
	virtual bool BeginEnum(IIterator **ppIterator) = 0;

	//After this call the internal objects list will be unlocked
	virtual bool EndEnum(IIterator *pIterator) = 0;

	//After this call the internal objects list will be locked
	virtual bool BeginEnum(IConstIterator **ppConstIterator) const = 0;

	//After this call the internal objects list will be unlocked
	virtual bool EndEnum(IConstIterator *pConstIterator) const = 0;

};

}//namespace CX
