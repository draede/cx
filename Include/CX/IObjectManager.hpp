/* 
 * The author disclaims copyright to this source code.
 */ 

#pragma once


#include "CX/Types.hpp"
#include "CX/IInterface.hpp"
#include <functional>


namespace CX
{

class IObjectManager : public IInterface
{
public:

	CX_DECLARE_INTERFACE("CX.ObjectManager")

	typedef std::function<StatusCode (IObject *)>         ObjectsEnumFunction;
	typedef std::function<StatusCode (const IObject *)>   ConstObjectsEnumFunction;

	class IIterator
	{
	public:

		virtual ~IIterator() { }

		virtual Bool Next(IObject **ppObject) = 0;

	};

	class IConstIterator
	{
	public:

		virtual ~IConstIterator() { }

		virtual Bool Next(const IObject **ppObject) = 0;

	};

	virtual ~IObjectManager() { }

	//recursive locks are mandatory

	//Retain will be called on pObject on success
	//During this call the internal objects list will be locked
	virtual StatusCode AddObject(IObject *pObject) = 0;

	//Release will be called on object
	//During this call the internal objects list will be locked
	virtual StatusCode RemoveObject(const char *szObjectName) = 0;

	//Release will be called on every object
	//During this call the internal objects list will be locked
	virtual StatusCode RemoveAllObjects() = 0;

	//After this call the internal objects list will be locked
	virtual StatusCode BeginEnum(IIterator **ppIterator) = 0;

	//After this call the internal objects list will be unlocked
	virtual StatusCode EndEnum(IIterator *pIterator) = 0;

	//After this call the internal objects list will be locked
	virtual StatusCode BeginEnum(IConstIterator **ppConstIterator) const = 0;

	//After this call the internal objects list will be unlocked
	virtual StatusCode EndEnum(IConstIterator *pConstIterator) const = 0;

	//During this call the internal objects list will be locked
	virtual StatusCode EnumObjects(const std::function<StatusCode (IObject *)> &pfnEnumObjects) = 0;

	//During this call the internal objects list will be locked
	virtual StatusCode EnumObjects(const std::function<StatusCode (const IObject *)> &pfnEnumConstObjects) const = 0;

};

}//namespace CX
