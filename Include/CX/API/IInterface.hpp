/* 
 * The author disclaims copyright to this source code.
 */ 
 
#ifndef __CX__IINTERFACE__H__
#define __CX__IINTERFACE__H__


class CX_IInterface
{
public:

	virtual const char *GetName() = 0;

	virtual long Retain() = 0;

	virtual long Release() = 0;

	virtual CX_IInterface *AcquireInterface(const char *szName) = 0;

	virtual bool ImplementsInterface(const char *szName) const = 0;

	virtual size_t GetDirectInterfacesCount() const = 0;

	virtual size_t GetAllInterfacesCount() const = 0;

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

