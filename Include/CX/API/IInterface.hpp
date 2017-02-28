/* 
 * The author disclaims copyright to this source code.
 */ 
 
#pragma once


namespace CX
{

class IInterface
{
public:

	virtual const char *GetName() const = 0;

	virtual void Retain() const = 0;

	virtual void Release() const = 0;

	virtual IInterface *Acquire(const char *szName) = 0;

	virtual const IInterface *Acquire(const char *szName) const = 0;

	virtual bool Implements(const char *szName) const = 0;

protected:

	virtual ~IInterface() { }

};

}//namespace CX


#define CX_DECLARE_INTERFACE(IFNAME)                                                                                   \
	static const char *NAME() { return IFNAME; }                                                                        \
	                                                                                                                    \
	virtual const char *GetName() const                                                                                 \
	{                                                                                                                   \
		return NAME();                                                                                                   \
	}

