/* 
 * The author disclaims copyright to this source code.
 */ 
 
#ifndef __CX__IINTERFACE__H__
#define __CX__IINTERFACE__H__


class CX_IInterface
{
public:

	virtual const char *GetName() = 0;

	virtual void Retain() = 0;

	virtual void Release() = 0;

	virtual CX_IInterface *Acquire(const char *szName) = 0;

	virtual bool Implements(const char *szName) const = 0;

protected:

	virtual ~CX_IInterface() { }

};


#define CX_DECLARE_INTERFACE(IFNAME)                                                                                   \
	static const char *NAME() { return IFNAME; }                                                                        \
	                                                                                                                    \
	virtual const char *GetName()                                                                                       \
	{                                                                                                                   \
		return NAME();                                                                                                   \
	}


#endif

