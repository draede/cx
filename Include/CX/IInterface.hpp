/* 
 * The author disclaims copyright to this source code.
 */ 

#pragma once


#include "CX/Types.hpp"


#define CX_DECLARE_INTERFACE(name)                                                                                     \
	static const CX::Char *INTERFACE() { return name; }                                                                 \
	                                                                                                                    \
	virtual const CX::Char *GetInterfaceName() const { return INTERFACE(); }


namespace CX
{

class IObject;
class IInterface
{
public:

	IInterface();

	virtual ~IInterface();

	virtual const Char *GetInterfaceName() const = 0;

	virtual long Retain();

	virtual long Retain() const;

	virtual long Release();

	virtual long Release() const;

	void SetObject(IObject *pObject);

	void SetObject(const IObject *pConstObject) const;

private:

	IObject               *m_pObject;
	mutable const IObject *m_pConstObject;

};

}//namespace CX

