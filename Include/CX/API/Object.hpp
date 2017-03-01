/* 
 * The author disclaims copyright to this source code.
 */ 
 
#pragma once


#include "IObject.hpp"
#include <string.h>
#ifdef _WIN32
	#include <windows.h>
#endif
#include <new>


namespace CX
{

template 
<
	typename OBJ, 
	typename I1  = IInterface::Null, typename I2  = IInterface::Null, typename I3  = IInterface::Null, 
	typename I4  = IInterface::Null, typename I5  = IInterface::Null, typename I6  = IInterface::Null, 
	typename I7  = IInterface::Null, typename I8  = IInterface::Null, typename I9  = IInterface::Null,
	typename I10 = IInterface::Null, typename I11 = IInterface::Null, typename I12 = IInterface::Null,
	typename I13 = IInterface::Null, typename I14 = IInterface::Null, typename I15 = IInterface::Null,
	typename I16 = IInterface::Null, typename I17 = IInterface::Null 
>
class Object;

template 
<
	typename OBJ, 
	typename I1, typename I2,  typename I3,  typename I4,  typename I5,  typename I6,  typename I7,  typename I8, 
	typename I9, typename I10, typename I11, typename I12, typename I13, typename I14, typename I15, typename I16 
>
class Object<OBJ, I1, I2, I3, I4, I5, I6, I7, I8, I9, I10, I11, I12, I13, I14, I15, I16> : 
	public IObject, 
	public I1, public I2,  public I3,  public I4,  public I5,  public I6,  public I7,  public I8, 
	public I9, public I10, public I11, public I12, public I13, public I14, public I15, public I16 
{
public:

	static OBJ *Create()
	{
		OBJ *pObject = new (std::nothrow) OBJ();

		pObject->m_cRefCount = 1;
		((I1 *)pObject)->SetObject(pObject);
		((I2 *)pObject)->SetObject(pObject);
		((I3 *)pObject)->SetObject(pObject);
		((I4 *)pObject)->SetObject(pObject);
		((I5 *)pObject)->SetObject(pObject);
		((I6 *)pObject)->SetObject(pObject);
		((I7 *)pObject)->SetObject(pObject);
		((I8 *)pObject)->SetObject(pObject);
		((I9 *)pObject)->SetObject(pObject);
		((I10 *)pObject)->SetObject(pObject);
		((I11 *)pObject)->SetObject(pObject);
		((I12 *)pObject)->SetObject(pObject);
		((I13 *)pObject)->SetObject(pObject);
		((I14 *)pObject)->SetObject(pObject);
		((I15 *)pObject)->SetObject(pObject);
		((I16 *)pObject)->SetObject(pObject);

		return pObject;
	}

	virtual long Retain() const
	{
#ifdef _WIN32
		return InterlockedIncrement(&m_cRefCount);
#else
		return __sync_fetch_and_add(&m_cRefCount, 1);
#endif
	}

	virtual long Release() const
	{
		long cRefCount;

#ifdef _WIN32
		if (0 == (cRefCount = InterlockedDecrement(&m_cRefCount)))
#else
		if (0 == (cRefCount = __sync_fetch_and_sub(&m_cRefCount, 1)))
#endif
		{
			delete this;
		}

		return m_cRefCount;
	}

	virtual bool Implements(const char *szName) const
	{
		     if (0 == strcmp(I1::GetName(),  szName)) return true;
		else if (0 == strcmp(I2::GetName(),  szName)) return true;
		else if (0 == strcmp(I3::GetName(),  szName)) return true;
		else if (0 == strcmp(I4::GetName(),  szName)) return true;
		else if (0 == strcmp(I5::GetName(),  szName)) return true;
		else if (0 == strcmp(I6::GetName(),  szName)) return true;
		else if (0 == strcmp(I7::GetName(),  szName)) return true;
		else if (0 == strcmp(I8::GetName(),  szName)) return true;
		else if (0 == strcmp(I9::GetName(),  szName)) return true;
		else if (0 == strcmp(I10::GetName(), szName)) return true;
		else if (0 == strcmp(I11::GetName(), szName)) return true;
		else if (0 == strcmp(I12::GetName(), szName)) return true;
		else if (0 == strcmp(I13::GetName(), szName)) return true;
		else if (0 == strcmp(I14::GetName(), szName)) return true;
		else if (0 == strcmp(I15::GetName(), szName)) return true;
		else if (0 == strcmp(I16::GetName(), szName)) return true;
		else return false;
	}

	virtual IInterface *Acquire(const char *szName)
	{
		     if (0 == strcmp(I1::GetName(),  szName)) { Retain(); return (I1 *)this;  }
		else if (0 == strcmp(I2::GetName(),  szName)) { Retain(); return (I2 *)this;  }
		else if (0 == strcmp(I3::GetName(),  szName)) { Retain(); return (I3 *)this;  }
		else if (0 == strcmp(I4::GetName(),  szName)) { Retain(); return (I4 *)this;  }
		else if (0 == strcmp(I5::GetName(),  szName)) { Retain(); return (I5 *)this;  }
		else if (0 == strcmp(I6::GetName(),  szName)) { Retain(); return (I6 *)this;  }
		else if (0 == strcmp(I7::GetName(),  szName)) { Retain(); return (I7 *)this;  }
		else if (0 == strcmp(I8::GetName(),  szName)) { Retain(); return (I8 *)this;  }
		else if (0 == strcmp(I9::GetName(),  szName)) { Retain(); return (I9 *)this;  }
		else if (0 == strcmp(I10::GetName(), szName)) { Retain(); return (I10 *)this; }
		else if (0 == strcmp(I11::GetName(), szName)) { Retain(); return (I11 *)this; }
		else if (0 == strcmp(I12::GetName(), szName)) { Retain(); return (I12 *)this; }
		else if (0 == strcmp(I13::GetName(), szName)) { Retain(); return (I13 *)this; }
		else if (0 == strcmp(I14::GetName(), szName)) { Retain(); return (I14 *)this; }
		else if (0 == strcmp(I15::GetName(), szName)) { Retain(); return (I15 *)this; }
		else if (0 == strcmp(I16::GetName(), szName)) { Retain(); return (I16 *)this; }
		else return NULL;
	}

	virtual const IInterface *Acquire(const char *szName) const
	{
		     if (0 == strcmp(I1::GetName(),  szName)) { Retain(); return (const I1 *)this;  }
		else if (0 == strcmp(I2::GetName(),  szName)) { Retain(); return (const I2 *)this;  }
		else if (0 == strcmp(I3::GetName(),  szName)) { Retain(); return (const I3 *)this;  }
		else if (0 == strcmp(I4::GetName(),  szName)) { Retain(); return (const I4 *)this;  }
		else if (0 == strcmp(I5::GetName(),  szName)) { Retain(); return (const I5 *)this;  }
		else if (0 == strcmp(I6::GetName(),  szName)) { Retain(); return (const I6 *)this;  }
		else if (0 == strcmp(I7::GetName(),  szName)) { Retain(); return (const I7 *)this;  }
		else if (0 == strcmp(I8::GetName(),  szName)) { Retain(); return (const I8 *)this;  }
		else if (0 == strcmp(I9::GetName(),  szName)) { Retain(); return (const I9 *)this;  }
		else if (0 == strcmp(I10::GetName(), szName)) { Retain(); return (const I10 *)this; }
		else if (0 == strcmp(I11::GetName(), szName)) { Retain(); return (const I11 *)this; }
		else if (0 == strcmp(I12::GetName(), szName)) { Retain(); return (const I12 *)this; }
		else if (0 == strcmp(I13::GetName(), szName)) { Retain(); return (const I13 *)this; }
		else if (0 == strcmp(I14::GetName(), szName)) { Retain(); return (const I14 *)this; }
		else if (0 == strcmp(I15::GetName(), szName)) { Retain(); return (const I15 *)this; }
		else if (0 == strcmp(I16::GetName(), szName)) { Retain(); return (const I16 *)this; }
		else return NULL;
	}

private:

	mutable volatile long m_cRefCount;

};

template 
<
	typename OBJ, 
	typename I1, typename I2,  typename I3,  typename I4,  typename I5,  typename I6,  typename I7,  typename I8, 
	typename I9, typename I10, typename I11, typename I12, typename I13, typename I14, typename I15 
>
class Object<OBJ, I1, I2, I3, I4, I5, I6, I7, I8, I9, I10, I11, I12, I13, I14, I15> : 
	public IObject, 
	public I1,  public I2,  public I3,  public I4,  public I5,  public I6,  public I7,  public I8, public I9, 
	public I10, public I11, public I12, public I13, public I14, public I15 
{
public:

	static OBJ *Create()
	{
		OBJ *pObject = new (std::nothrow) OBJ();

		pObject->m_cRefCount = 1;
		((I1 *)pObject)->SetObject(pObject);
		((I2 *)pObject)->SetObject(pObject);
		((I3 *)pObject)->SetObject(pObject);
		((I4 *)pObject)->SetObject(pObject);
		((I5 *)pObject)->SetObject(pObject);
		((I6 *)pObject)->SetObject(pObject);
		((I7 *)pObject)->SetObject(pObject);
		((I8 *)pObject)->SetObject(pObject);
		((I9 *)pObject)->SetObject(pObject);
		((I10 *)pObject)->SetObject(pObject);
		((I11 *)pObject)->SetObject(pObject);
		((I12 *)pObject)->SetObject(pObject);
		((I13 *)pObject)->SetObject(pObject);
		((I14 *)pObject)->SetObject(pObject);
		((I15 *)pObject)->SetObject(pObject);

		return pObject;
	}

	virtual long Retain() const
	{
#ifdef _WIN32
		return InterlockedIncrement(&m_cRefCount);
#else
		return __sync_fetch_and_add(&m_cRefCount, 1);
#endif
	}

	virtual long Release() const
	{
		long cRefCount;

#ifdef _WIN32
		if (0 == (cRefCount = InterlockedDecrement(&m_cRefCount)))
#else
		if (0 == (cRefCount = __sync_fetch_and_sub(&m_cRefCount, 1)))
#endif
		{
			delete this;
		}

		return m_cRefCount;
	}

	virtual bool Implements(const char *szName) const
	{
		     if (0 == strcmp(I1::GetName(),  szName)) return true;
		else if (0 == strcmp(I2::GetName(),  szName)) return true;
		else if (0 == strcmp(I3::GetName(),  szName)) return true;
		else if (0 == strcmp(I4::GetName(),  szName)) return true;
		else if (0 == strcmp(I5::GetName(),  szName)) return true;
		else if (0 == strcmp(I6::GetName(),  szName)) return true;
		else if (0 == strcmp(I7::GetName(),  szName)) return true;
		else if (0 == strcmp(I8::GetName(),  szName)) return true;
		else if (0 == strcmp(I9::GetName(),  szName)) return true;
		else if (0 == strcmp(I10::GetName(), szName)) return true;
		else if (0 == strcmp(I11::GetName(), szName)) return true;
		else if (0 == strcmp(I12::GetName(), szName)) return true;
		else if (0 == strcmp(I13::GetName(), szName)) return true;
		else if (0 == strcmp(I14::GetName(), szName)) return true;
		else if (0 == strcmp(I15::GetName(), szName)) return true;
		else return false;
	}

	virtual IInterface *Acquire(const char *szName)
	{
		     if (0 == strcmp(I1::GetName(),  szName)) { Retain(); return (I1 *)this;  }
		else if (0 == strcmp(I2::GetName(),  szName)) { Retain(); return (I2 *)this;  }
		else if (0 == strcmp(I3::GetName(),  szName)) { Retain(); return (I3 *)this;  }
		else if (0 == strcmp(I4::GetName(),  szName)) { Retain(); return (I4 *)this;  }
		else if (0 == strcmp(I5::GetName(),  szName)) { Retain(); return (I5 *)this;  }
		else if (0 == strcmp(I6::GetName(),  szName)) { Retain(); return (I6 *)this;  }
		else if (0 == strcmp(I7::GetName(),  szName)) { Retain(); return (I7 *)this;  }
		else if (0 == strcmp(I8::GetName(),  szName)) { Retain(); return (I8 *)this;  }
		else if (0 == strcmp(I9::GetName(),  szName)) { Retain(); return (I9 *)this;  }
		else if (0 == strcmp(I10::GetName(), szName)) { Retain(); return (I10 *)this; }
		else if (0 == strcmp(I11::GetName(), szName)) { Retain(); return (I11 *)this; }
		else if (0 == strcmp(I12::GetName(), szName)) { Retain(); return (I12 *)this; }
		else if (0 == strcmp(I13::GetName(), szName)) { Retain(); return (I13 *)this; }
		else if (0 == strcmp(I14::GetName(), szName)) { Retain(); return (I14 *)this; }
		else if (0 == strcmp(I15::GetName(), szName)) { Retain(); return (I15 *)this; }
		else return NULL;
	}

	virtual const IInterface *Acquire(const char *szName) const
	{
		     if (0 == strcmp(I1::GetName(),  szName)) { Retain(); return (const I1 *)this;  }
		else if (0 == strcmp(I2::GetName(),  szName)) { Retain(); return (const I2 *)this;  }
		else if (0 == strcmp(I3::GetName(),  szName)) { Retain(); return (const I3 *)this;  }
		else if (0 == strcmp(I4::GetName(),  szName)) { Retain(); return (const I4 *)this;  }
		else if (0 == strcmp(I5::GetName(),  szName)) { Retain(); return (const I5 *)this;  }
		else if (0 == strcmp(I6::GetName(),  szName)) { Retain(); return (const I6 *)this;  }
		else if (0 == strcmp(I7::GetName(),  szName)) { Retain(); return (const I7 *)this;  }
		else if (0 == strcmp(I8::GetName(),  szName)) { Retain(); return (const I8 *)this;  }
		else if (0 == strcmp(I9::GetName(),  szName)) { Retain(); return (const I9 *)this;  }
		else if (0 == strcmp(I10::GetName(), szName)) { Retain(); return (const I10 *)this; }
		else if (0 == strcmp(I11::GetName(), szName)) { Retain(); return (const I11 *)this; }
		else if (0 == strcmp(I12::GetName(), szName)) { Retain(); return (const I12 *)this; }
		else if (0 == strcmp(I13::GetName(), szName)) { Retain(); return (const I13 *)this; }
		else if (0 == strcmp(I14::GetName(), szName)) { Retain(); return (const I14 *)this; }
		else if (0 == strcmp(I15::GetName(), szName)) { Retain(); return (const I15 *)this; }
		else return NULL;
	}

private:

	mutable volatile long m_cRefCount;

};

template 
<
	typename OBJ, 
	typename I1, typename I2,  typename I3,  typename I4,  typename I5,  typename I6,  typename I7, typename I8, 
	typename I9, typename I10, typename I11, typename I12, typename I13, typename I14 
>
class Object<OBJ, I1, I2, I3, I4, I5, I6, I7, I8, I9, I10, I11, I12, I13, I14> : 
	public IObject, 
	public I1,  public I2,  public I3,  public I4,  public I5,  public I6,  public I7, public I8, public I9, public I10, 
	public I11, public I12, public I13, public I14 
{
public:

	static OBJ *Create()
	{
		OBJ *pObject = new (std::nothrow) OBJ();

		pObject->m_cRefCount = 1;
		((I1 *)pObject)->SetObject(pObject);
		((I2 *)pObject)->SetObject(pObject);
		((I3 *)pObject)->SetObject(pObject);
		((I4 *)pObject)->SetObject(pObject);
		((I5 *)pObject)->SetObject(pObject);
		((I6 *)pObject)->SetObject(pObject);
		((I7 *)pObject)->SetObject(pObject);
		((I8 *)pObject)->SetObject(pObject);
		((I9 *)pObject)->SetObject(pObject);
		((I10 *)pObject)->SetObject(pObject);
		((I11 *)pObject)->SetObject(pObject);
		((I12 *)pObject)->SetObject(pObject);
		((I13 *)pObject)->SetObject(pObject);
		((I14 *)pObject)->SetObject(pObject);

		return pObject;
	}

	virtual long Retain() const
	{
#ifdef _WIN32
		return InterlockedIncrement(&m_cRefCount);
#else
		return __sync_fetch_and_add(&m_cRefCount, 1);
#endif
	}

	virtual long Release() const
	{
		long cRefCount;

#ifdef _WIN32
		if (0 == (cRefCount = InterlockedDecrement(&m_cRefCount)))
#else
		if (0 == (cRefCount = __sync_fetch_and_sub(&m_cRefCount, 1)))
#endif
		{
			delete this;
		}

		return m_cRefCount;
	}

	virtual bool Implements(const char *szName) const
	{
		     if (0 == strcmp(I1::GetName(),  szName)) return true;
		else if (0 == strcmp(I2::GetName(),  szName)) return true;
		else if (0 == strcmp(I3::GetName(),  szName)) return true;
		else if (0 == strcmp(I4::GetName(),  szName)) return true;
		else if (0 == strcmp(I5::GetName(),  szName)) return true;
		else if (0 == strcmp(I6::GetName(),  szName)) return true;
		else if (0 == strcmp(I7::GetName(),  szName)) return true;
		else if (0 == strcmp(I8::GetName(),  szName)) return true;
		else if (0 == strcmp(I9::GetName(),  szName)) return true;
		else if (0 == strcmp(I10::GetName(), szName)) return true;
		else if (0 == strcmp(I11::GetName(), szName)) return true;
		else if (0 == strcmp(I12::GetName(), szName)) return true;
		else if (0 == strcmp(I13::GetName(), szName)) return true;
		else if (0 == strcmp(I14::GetName(), szName)) return true;
		else return false;
	}

	virtual IInterface *Acquire(const char *szName)
	{
		     if (0 == strcmp(I1::GetName(),  szName)) { Retain(); return (I1 *)this;  }
		else if (0 == strcmp(I2::GetName(),  szName)) { Retain(); return (I2 *)this;  }
		else if (0 == strcmp(I3::GetName(),  szName)) { Retain(); return (I3 *)this;  }
		else if (0 == strcmp(I4::GetName(),  szName)) { Retain(); return (I4 *)this;  }
		else if (0 == strcmp(I5::GetName(),  szName)) { Retain(); return (I5 *)this;  }
		else if (0 == strcmp(I6::GetName(),  szName)) { Retain(); return (I6 *)this;  }
		else if (0 == strcmp(I7::GetName(),  szName)) { Retain(); return (I7 *)this;  }
		else if (0 == strcmp(I8::GetName(),  szName)) { Retain(); return (I8 *)this;  }
		else if (0 == strcmp(I9::GetName(),  szName)) { Retain(); return (I9 *)this;  }
		else if (0 == strcmp(I10::GetName(), szName)) { Retain(); return (I10 *)this; }
		else if (0 == strcmp(I11::GetName(), szName)) { Retain(); return (I11 *)this; }
		else if (0 == strcmp(I12::GetName(), szName)) { Retain(); return (I12 *)this; }
		else if (0 == strcmp(I13::GetName(), szName)) { Retain(); return (I13 *)this; }
		else if (0 == strcmp(I14::GetName(), szName)) { Retain(); return (I14 *)this; }
		else return NULL;
	}

	virtual const IInterface *Acquire(const char *szName) const
	{
		     if (0 == strcmp(I1::GetName(),  szName)) { Retain(); return (const I1 *)this;  }
		else if (0 == strcmp(I2::GetName(),  szName)) { Retain(); return (const I2 *)this;  }
		else if (0 == strcmp(I3::GetName(),  szName)) { Retain(); return (const I3 *)this;  }
		else if (0 == strcmp(I4::GetName(),  szName)) { Retain(); return (const I4 *)this;  }
		else if (0 == strcmp(I5::GetName(),  szName)) { Retain(); return (const I5 *)this;  }
		else if (0 == strcmp(I6::GetName(),  szName)) { Retain(); return (const I6 *)this;  }
		else if (0 == strcmp(I7::GetName(),  szName)) { Retain(); return (const I7 *)this;  }
		else if (0 == strcmp(I8::GetName(),  szName)) { Retain(); return (const I8 *)this;  }
		else if (0 == strcmp(I9::GetName(),  szName)) { Retain(); return (const I9 *)this;  }
		else if (0 == strcmp(I10::GetName(), szName)) { Retain(); return (const I10 *)this; }
		else if (0 == strcmp(I11::GetName(), szName)) { Retain(); return (const I11 *)this; }
		else if (0 == strcmp(I12::GetName(), szName)) { Retain(); return (const I12 *)this; }
		else if (0 == strcmp(I13::GetName(), szName)) { Retain(); return (const I13 *)this; }
		else if (0 == strcmp(I14::GetName(), szName)) { Retain(); return (const I14 *)this; }
		else return NULL;
	}

private:

	mutable volatile long m_cRefCount;

};

template 
<
	typename OBJ, 
	typename I1, typename I2,  typename I3,  typename I4,  typename I5,  typename I6, typename I7, typename I8, 
	typename I9, typename I10, typename I11, typename I12, typename I13 
>
class Object<OBJ, I1, I2, I3, I4, I5, I6, I7, I8, I9, I10, I11, I12, I13> : 
	public IObject, 
	public I1,  public I2,  public I3,  public I4,  public I5,  public I6, public I7, public I8, public I9, public I10, 
	public I11, public I12, public I13 
{
public:

	static OBJ *Create()
	{
		OBJ *pObject = new (std::nothrow) OBJ();

		pObject->m_cRefCount = 1;
		((I1 *)pObject)->SetObject(pObject);
		((I2 *)pObject)->SetObject(pObject);
		((I3 *)pObject)->SetObject(pObject);
		((I4 *)pObject)->SetObject(pObject);
		((I5 *)pObject)->SetObject(pObject);
		((I6 *)pObject)->SetObject(pObject);
		((I7 *)pObject)->SetObject(pObject);
		((I8 *)pObject)->SetObject(pObject);
		((I9 *)pObject)->SetObject(pObject);
		((I10 *)pObject)->SetObject(pObject);
		((I11 *)pObject)->SetObject(pObject);
		((I12 *)pObject)->SetObject(pObject);
		((I13 *)pObject)->SetObject(pObject);

		return pObject;
	}

	virtual long Retain() const
	{
#ifdef _WIN32
		return InterlockedIncrement(&m_cRefCount);
#else
		return __sync_fetch_and_add(&m_cRefCount, 1);
#endif
	}

	virtual long Release() const
	{
		long cRefCount;

#ifdef _WIN32
		if (0 == (cRefCount = InterlockedDecrement(&m_cRefCount)))
#else
		if (0 == (cRefCount = __sync_fetch_and_sub(&m_cRefCount, 1)))
#endif
		{
			delete this;
		}

		return m_cRefCount;
	}

	virtual bool Implements(const char *szName) const
	{
		     if (0 == strcmp(I1::GetName(),  szName)) return true;
		else if (0 == strcmp(I2::GetName(),  szName)) return true;
		else if (0 == strcmp(I3::GetName(),  szName)) return true;
		else if (0 == strcmp(I4::GetName(),  szName)) return true;
		else if (0 == strcmp(I5::GetName(),  szName)) return true;
		else if (0 == strcmp(I6::GetName(),  szName)) return true;
		else if (0 == strcmp(I7::GetName(),  szName)) return true;
		else if (0 == strcmp(I8::GetName(),  szName)) return true;
		else if (0 == strcmp(I9::GetName(),  szName)) return true;
		else if (0 == strcmp(I10::GetName(), szName)) return true;
		else if (0 == strcmp(I11::GetName(), szName)) return true;
		else if (0 == strcmp(I12::GetName(), szName)) return true;
		else if (0 == strcmp(I13::GetName(), szName)) return true;
		else return false;
	}

	virtual IInterface *Acquire(const char *szName)
	{
		     if (0 == strcmp(I1::GetName(),  szName)) { Retain(); return (I1 *)this;  }
		else if (0 == strcmp(I2::GetName(),  szName)) { Retain(); return (I2 *)this;  }
		else if (0 == strcmp(I3::GetName(),  szName)) { Retain(); return (I3 *)this;  }
		else if (0 == strcmp(I4::GetName(),  szName)) { Retain(); return (I4 *)this;  }
		else if (0 == strcmp(I5::GetName(),  szName)) { Retain(); return (I5 *)this;  }
		else if (0 == strcmp(I6::GetName(),  szName)) { Retain(); return (I6 *)this;  }
		else if (0 == strcmp(I7::GetName(),  szName)) { Retain(); return (I7 *)this;  }
		else if (0 == strcmp(I8::GetName(),  szName)) { Retain(); return (I8 *)this;  }
		else if (0 == strcmp(I9::GetName(),  szName)) { Retain(); return (I9 *)this;  }
		else if (0 == strcmp(I10::GetName(), szName)) { Retain(); return (I10 *)this; }
		else if (0 == strcmp(I11::GetName(), szName)) { Retain(); return (I11 *)this; }
		else if (0 == strcmp(I12::GetName(), szName)) { Retain(); return (I12 *)this; }
		else if (0 == strcmp(I13::GetName(), szName)) { Retain(); return (I13 *)this; }
		else return NULL;
	}

	virtual const IInterface *Acquire(const char *szName) const
	{
		     if (0 == strcmp(I1::GetName(),  szName)) { Retain(); return (const I1 *)this;  }
		else if (0 == strcmp(I2::GetName(),  szName)) { Retain(); return (const I2 *)this;  }
		else if (0 == strcmp(I3::GetName(),  szName)) { Retain(); return (const I3 *)this;  }
		else if (0 == strcmp(I4::GetName(),  szName)) { Retain(); return (const I4 *)this;  }
		else if (0 == strcmp(I5::GetName(),  szName)) { Retain(); return (const I5 *)this;  }
		else if (0 == strcmp(I6::GetName(),  szName)) { Retain(); return (const I6 *)this;  }
		else if (0 == strcmp(I7::GetName(),  szName)) { Retain(); return (const I7 *)this;  }
		else if (0 == strcmp(I8::GetName(),  szName)) { Retain(); return (const I8 *)this;  }
		else if (0 == strcmp(I9::GetName(),  szName)) { Retain(); return (const I9 *)this;  }
		else if (0 == strcmp(I10::GetName(), szName)) { Retain(); return (const I10 *)this; }
		else if (0 == strcmp(I11::GetName(), szName)) { Retain(); return (const I11 *)this; }
		else if (0 == strcmp(I12::GetName(), szName)) { Retain(); return (const I12 *)this; }
		else if (0 == strcmp(I13::GetName(), szName)) { Retain(); return (const I13 *)this; }
		else return NULL;
	}

private:

	mutable volatile long m_cRefCount;

};

template 
<
	typename OBJ, 
	typename I1, typename I2,  typename I3,  typename I4,  typename I5, typename I6, typename I7, typename I8, 
	typename I9, typename I10, typename I11, typename I12 
>
class Object<OBJ, I1, I2, I3, I4, I5, I6, I7, I8, I9, I10, I11, I12> : 
	public IObject, 
	public I1,  public I2,  public I3,  public I4,  public I5, public I6, public I7, public I8, public I9, public I10, 
	public I11, public I12 
{
public:

	static OBJ *Create()
	{
		OBJ *pObject = new (std::nothrow) OBJ();

		pObject->m_cRefCount = 1;
		((I1 *)pObject)->SetObject(pObject);
		((I2 *)pObject)->SetObject(pObject);
		((I3 *)pObject)->SetObject(pObject);
		((I4 *)pObject)->SetObject(pObject);
		((I5 *)pObject)->SetObject(pObject);
		((I6 *)pObject)->SetObject(pObject);
		((I7 *)pObject)->SetObject(pObject);
		((I8 *)pObject)->SetObject(pObject);
		((I9 *)pObject)->SetObject(pObject);
		((I10 *)pObject)->SetObject(pObject);
		((I11 *)pObject)->SetObject(pObject);
		((I12 *)pObject)->SetObject(pObject);

		return pObject;
	}

	virtual long Retain() const
	{
#ifdef _WIN32
		return InterlockedIncrement(&m_cRefCount);
#else
		return __sync_fetch_and_add(&m_cRefCount, 1);
#endif
	}

	virtual long Release() const
	{
		long cRefCount;

#ifdef _WIN32
		if (0 == (cRefCount = InterlockedDecrement(&m_cRefCount)))
#else
		if (0 == (cRefCount = __sync_fetch_and_sub(&m_cRefCount, 1)))
#endif
		{
			delete this;
		}

		return m_cRefCount;
	}

	virtual bool Implements(const char *szName) const
	{
		     if (0 == strcmp(I1::GetName(),  szName)) return true;
		else if (0 == strcmp(I2::GetName(),  szName)) return true;
		else if (0 == strcmp(I3::GetName(),  szName)) return true;
		else if (0 == strcmp(I4::GetName(),  szName)) return true;
		else if (0 == strcmp(I5::GetName(),  szName)) return true;
		else if (0 == strcmp(I6::GetName(),  szName)) return true;
		else if (0 == strcmp(I7::GetName(),  szName)) return true;
		else if (0 == strcmp(I8::GetName(),  szName)) return true;
		else if (0 == strcmp(I9::GetName(),  szName)) return true;
		else if (0 == strcmp(I10::GetName(), szName)) return true;
		else if (0 == strcmp(I11::GetName(), szName)) return true;
		else if (0 == strcmp(I12::GetName(), szName)) return true;
		else return false;
	}

	virtual IInterface *Acquire(const char *szName)
	{
		     if (0 == strcmp(I1::GetName(),  szName)) { Retain(); return (I1 *)this;  }
		else if (0 == strcmp(I2::GetName(),  szName)) { Retain(); return (I2 *)this;  }
		else if (0 == strcmp(I3::GetName(),  szName)) { Retain(); return (I3 *)this;  }
		else if (0 == strcmp(I4::GetName(),  szName)) { Retain(); return (I4 *)this;  }
		else if (0 == strcmp(I5::GetName(),  szName)) { Retain(); return (I5 *)this;  }
		else if (0 == strcmp(I6::GetName(),  szName)) { Retain(); return (I6 *)this;  }
		else if (0 == strcmp(I7::GetName(),  szName)) { Retain(); return (I7 *)this;  }
		else if (0 == strcmp(I8::GetName(),  szName)) { Retain(); return (I8 *)this;  }
		else if (0 == strcmp(I9::GetName(),  szName)) { Retain(); return (I9 *)this;  }
		else if (0 == strcmp(I10::GetName(), szName)) { Retain(); return (I10 *)this; }
		else if (0 == strcmp(I11::GetName(), szName)) { Retain(); return (I11 *)this; }
		else if (0 == strcmp(I12::GetName(), szName)) { Retain(); return (I12 *)this; }
		else return NULL;
	}

	virtual const IInterface *Acquire(const char *szName) const
	{
		     if (0 == strcmp(I1::GetName(),  szName)) { Retain(); return (const I1 *)this;  }
		else if (0 == strcmp(I2::GetName(),  szName)) { Retain(); return (const I2 *)this;  }
		else if (0 == strcmp(I3::GetName(),  szName)) { Retain(); return (const I3 *)this;  }
		else if (0 == strcmp(I4::GetName(),  szName)) { Retain(); return (const I4 *)this;  }
		else if (0 == strcmp(I5::GetName(),  szName)) { Retain(); return (const I5 *)this;  }
		else if (0 == strcmp(I6::GetName(),  szName)) { Retain(); return (const I6 *)this;  }
		else if (0 == strcmp(I7::GetName(),  szName)) { Retain(); return (const I7 *)this;  }
		else if (0 == strcmp(I8::GetName(),  szName)) { Retain(); return (const I8 *)this;  }
		else if (0 == strcmp(I9::GetName(),  szName)) { Retain(); return (const I9 *)this;  }
		else if (0 == strcmp(I10::GetName(), szName)) { Retain(); return (const I10 *)this; }
		else if (0 == strcmp(I11::GetName(), szName)) { Retain(); return (const I11 *)this; }
		else if (0 == strcmp(I12::GetName(), szName)) { Retain(); return (const I12 *)this; }
		else return NULL;
	}

private:

	mutable volatile long m_cRefCount;

};

template 
<
	typename OBJ, 
	typename I1, typename I2,  typename I3,  typename I4, typename I5, typename I6, typename I7, typename I8, 
	typename I9, typename I10, typename I11 
>
class Object<OBJ, I1, I2, I3, I4, I5, I6, I7, I8, I9, I10, I11> : 
	public IObject, 
	public I1, public I2, public I3, public I4, public I5, public I6, public I7, public I8, public I9, public I10, 
	public I11 
{
public:

	static OBJ *Create()
	{
		OBJ *pObject = new (std::nothrow) OBJ();

		pObject->m_cRefCount = 1;
		((I1 *)pObject)->SetObject(pObject);
		((I2 *)pObject)->SetObject(pObject);
		((I3 *)pObject)->SetObject(pObject);
		((I4 *)pObject)->SetObject(pObject);
		((I5 *)pObject)->SetObject(pObject);
		((I6 *)pObject)->SetObject(pObject);
		((I7 *)pObject)->SetObject(pObject);
		((I8 *)pObject)->SetObject(pObject);
		((I9 *)pObject)->SetObject(pObject);
		((I10 *)pObject)->SetObject(pObject);
		((I11 *)pObject)->SetObject(pObject);

		return pObject;
	}

	virtual long Retain() const
	{
#ifdef _WIN32
		return InterlockedIncrement(&m_cRefCount);
#else
		return __sync_fetch_and_add(&m_cRefCount, 1);
#endif
	}

	virtual long Release() const
	{
		long cRefCount;

#ifdef _WIN32
		if (0 == (cRefCount = InterlockedDecrement(&m_cRefCount)))
#else
		if (0 == (cRefCount = __sync_fetch_and_sub(&m_cRefCount, 1)))
#endif
		{
			delete this;
		}

		return m_cRefCount;
	}

	virtual bool Implements(const char *szName) const
	{
		     if (0 == strcmp(I1::GetName(),  szName)) return true;
		else if (0 == strcmp(I2::GetName(),  szName)) return true;
		else if (0 == strcmp(I3::GetName(),  szName)) return true;
		else if (0 == strcmp(I4::GetName(),  szName)) return true;
		else if (0 == strcmp(I5::GetName(),  szName)) return true;
		else if (0 == strcmp(I6::GetName(),  szName)) return true;
		else if (0 == strcmp(I7::GetName(),  szName)) return true;
		else if (0 == strcmp(I8::GetName(),  szName)) return true;
		else if (0 == strcmp(I9::GetName(),  szName)) return true;
		else if (0 == strcmp(I10::GetName(), szName)) return true;
		else if (0 == strcmp(I11::GetName(), szName)) return true;
		else return false;
	}

	virtual IInterface *Acquire(const char *szName)
	{
		     if (0 == strcmp(I1::GetName(),  szName)) { Retain(); return (I1 *)this;  }
		else if (0 == strcmp(I2::GetName(),  szName)) { Retain(); return (I2 *)this;  }
		else if (0 == strcmp(I3::GetName(),  szName)) { Retain(); return (I3 *)this;  }
		else if (0 == strcmp(I4::GetName(),  szName)) { Retain(); return (I4 *)this;  }
		else if (0 == strcmp(I5::GetName(),  szName)) { Retain(); return (I5 *)this;  }
		else if (0 == strcmp(I6::GetName(),  szName)) { Retain(); return (I6 *)this;  }
		else if (0 == strcmp(I7::GetName(),  szName)) { Retain(); return (I7 *)this;  }
		else if (0 == strcmp(I8::GetName(),  szName)) { Retain(); return (I8 *)this;  }
		else if (0 == strcmp(I9::GetName(),  szName)) { Retain(); return (I9 *)this;  }
		else if (0 == strcmp(I10::GetName(), szName)) { Retain(); return (I10 *)this; }
		else if (0 == strcmp(I11::GetName(), szName)) { Retain(); return (I11 *)this; }
		else return NULL;
	}

	virtual const IInterface *Acquire(const char *szName) const
	{
		     if (0 == strcmp(I1::GetName(),  szName)) { Retain(); return (const I1 *)this;  }
		else if (0 == strcmp(I2::GetName(),  szName)) { Retain(); return (const I2 *)this;  }
		else if (0 == strcmp(I3::GetName(),  szName)) { Retain(); return (const I3 *)this;  }
		else if (0 == strcmp(I4::GetName(),  szName)) { Retain(); return (const I4 *)this;  }
		else if (0 == strcmp(I5::GetName(),  szName)) { Retain(); return (const I5 *)this;  }
		else if (0 == strcmp(I6::GetName(),  szName)) { Retain(); return (const I6 *)this;  }
		else if (0 == strcmp(I7::GetName(),  szName)) { Retain(); return (const I7 *)this;  }
		else if (0 == strcmp(I8::GetName(),  szName)) { Retain(); return (const I8 *)this;  }
		else if (0 == strcmp(I9::GetName(),  szName)) { Retain(); return (const I9 *)this;  }
		else if (0 == strcmp(I10::GetName(), szName)) { Retain(); return (const I10 *)this; }
		else if (0 == strcmp(I11::GetName(), szName)) { Retain(); return (const I11 *)this; }
		else return NULL;
	}

private:

	mutable volatile long m_cRefCount;

};

template 
<
	typename OBJ, 
	typename I1, typename I2,  typename I3,  typename I4,  typename I5,  typename I6,  typename I7,  typename I8, 
	typename I9, typename I10 
>
class Object<OBJ, I1, I2, I3, I4, I5, I6, I7, I8, I9, I10> : 
	public IObject, 
	public I1, public I2, public I3, public I4, public I5, public I6, public I7, public I8, public I9, public I10     
{
public:

	static OBJ *Create()
	{
		OBJ *pObject = new (std::nothrow) OBJ();

		pObject->m_cRefCount = 1;
		((I1 *)pObject)->SetObject(pObject);
		((I2 *)pObject)->SetObject(pObject);
		((I3 *)pObject)->SetObject(pObject);
		((I4 *)pObject)->SetObject(pObject);
		((I5 *)pObject)->SetObject(pObject);
		((I6 *)pObject)->SetObject(pObject);
		((I7 *)pObject)->SetObject(pObject);
		((I8 *)pObject)->SetObject(pObject);
		((I9 *)pObject)->SetObject(pObject);
		((I10 *)pObject)->SetObject(pObject);

		return pObject;
	}

	virtual long Retain() const
	{
#ifdef _WIN32
		return InterlockedIncrement(&m_cRefCount);
#else
		return __sync_fetch_and_add(&m_cRefCount, 1);
#endif
	}

	virtual long Release() const
	{
		long cRefCount;

#ifdef _WIN32
		if (0 == (cRefCount = InterlockedDecrement(&m_cRefCount)))
#else
		if (0 == (cRefCount = __sync_fetch_and_sub(&m_cRefCount, 1)))
#endif
		{
			delete this;
		}

		return m_cRefCount;
	}

	virtual bool Implements(const char *szName) const
	{
		     if (0 == strcmp(I1::GetName(),  szName)) return true;
		else if (0 == strcmp(I2::GetName(),  szName)) return true;
		else if (0 == strcmp(I3::GetName(),  szName)) return true;
		else if (0 == strcmp(I4::GetName(),  szName)) return true;
		else if (0 == strcmp(I5::GetName(),  szName)) return true;
		else if (0 == strcmp(I6::GetName(),  szName)) return true;
		else if (0 == strcmp(I7::GetName(),  szName)) return true;
		else if (0 == strcmp(I8::GetName(),  szName)) return true;
		else if (0 == strcmp(I9::GetName(),  szName)) return true;
		else if (0 == strcmp(I10::GetName(), szName)) return true;
		else return false;
	}

	virtual IInterface *Acquire(const char *szName)
	{
		     if (0 == strcmp(I1::GetName(),  szName)) { Retain(); return (I1 *)this;  }
		else if (0 == strcmp(I2::GetName(),  szName)) { Retain(); return (I2 *)this;  }
		else if (0 == strcmp(I3::GetName(),  szName)) { Retain(); return (I3 *)this;  }
		else if (0 == strcmp(I4::GetName(),  szName)) { Retain(); return (I4 *)this;  }
		else if (0 == strcmp(I5::GetName(),  szName)) { Retain(); return (I5 *)this;  }
		else if (0 == strcmp(I6::GetName(),  szName)) { Retain(); return (I6 *)this;  }
		else if (0 == strcmp(I7::GetName(),  szName)) { Retain(); return (I7 *)this;  }
		else if (0 == strcmp(I8::GetName(),  szName)) { Retain(); return (I8 *)this;  }
		else if (0 == strcmp(I9::GetName(),  szName)) { Retain(); return (I9 *)this;  }
		else if (0 == strcmp(I10::GetName(), szName)) { Retain(); return (I10 *)this; }
		else return NULL;
	}

	virtual const IInterface *Acquire(const char *szName) const
	{
		     if (0 == strcmp(I1::GetName(),  szName)) { Retain(); return (const I1 *)this;  }
		else if (0 == strcmp(I2::GetName(),  szName)) { Retain(); return (const I2 *)this;  }
		else if (0 == strcmp(I3::GetName(),  szName)) { Retain(); return (const I3 *)this;  }
		else if (0 == strcmp(I4::GetName(),  szName)) { Retain(); return (const I4 *)this;  }
		else if (0 == strcmp(I5::GetName(),  szName)) { Retain(); return (const I5 *)this;  }
		else if (0 == strcmp(I6::GetName(),  szName)) { Retain(); return (const I6 *)this;  }
		else if (0 == strcmp(I7::GetName(),  szName)) { Retain(); return (const I7 *)this;  }
		else if (0 == strcmp(I8::GetName(),  szName)) { Retain(); return (const I8 *)this;  }
		else if (0 == strcmp(I9::GetName(),  szName)) { Retain(); return (const I9 *)this;  }
		else if (0 == strcmp(I10::GetName(), szName)) { Retain(); return (const I10 *)this; }
		else return NULL;
	}

private:

	mutable volatile long m_cRefCount;

};

template 
<
	typename OBJ, 
	typename I1, typename I2, typename I3, typename I4, typename I5, typename I6, typename I7, typename I8, 
	typename I9 
>
class Object<OBJ, I1, I2, I3, I4, I5, I6, I7, I8, I9> : 
	public IObject, 
	public I1, public I2, public I3, public I4, public I5, public I6, public I7, public I8, public I9 
{
public:

	static OBJ *Create()
	{
		OBJ *pObject = new (std::nothrow) OBJ();

		pObject->m_cRefCount = 1;
		((I1 *)pObject)->SetObject(pObject);
		((I2 *)pObject)->SetObject(pObject);
		((I3 *)pObject)->SetObject(pObject);
		((I4 *)pObject)->SetObject(pObject);
		((I5 *)pObject)->SetObject(pObject);
		((I6 *)pObject)->SetObject(pObject);
		((I7 *)pObject)->SetObject(pObject);
		((I8 *)pObject)->SetObject(pObject);
		((I9 *)pObject)->SetObject(pObject);

		return pObject;
	}

	virtual long Retain() const
	{
#ifdef _WIN32
		return InterlockedIncrement(&m_cRefCount);
#else
		return __sync_fetch_and_add(&m_cRefCount, 1);
#endif
	}

	virtual long Release() const
	{
		long cRefCount;

#ifdef _WIN32
		if (0 == (cRefCount = InterlockedDecrement(&m_cRefCount)))
#else
		if (0 == (cRefCount = __sync_fetch_and_sub(&m_cRefCount, 1)))
#endif
		{
			delete this;
		}

		return m_cRefCount;
	}

	virtual bool Implements(const char *szName) const
	{
		     if (0 == strcmp(I1::GetName(), szName)) return true;
		else if (0 == strcmp(I2::GetName(), szName)) return true;
		else if (0 == strcmp(I3::GetName(), szName)) return true;
		else if (0 == strcmp(I4::GetName(), szName)) return true;
		else if (0 == strcmp(I5::GetName(), szName)) return true;
		else if (0 == strcmp(I6::GetName(), szName)) return true;
		else if (0 == strcmp(I7::GetName(), szName)) return true;
		else if (0 == strcmp(I8::GetName(), szName)) return true;
		else if (0 == strcmp(I9::GetName(), szName)) return true;
		else return false;
	}

	virtual IInterface *Acquire(const char *szName)
	{
		     if (0 == strcmp(I1::GetName(), szName)) { Retain(); return (I1 *)this; }
		else if (0 == strcmp(I2::GetName(), szName)) { Retain(); return (I2 *)this; }
		else if (0 == strcmp(I3::GetName(), szName)) { Retain(); return (I3 *)this; }
		else if (0 == strcmp(I4::GetName(), szName)) { Retain(); return (I4 *)this; }
		else if (0 == strcmp(I5::GetName(), szName)) { Retain(); return (I5 *)this; }
		else if (0 == strcmp(I6::GetName(), szName)) { Retain(); return (I6 *)this; }
		else if (0 == strcmp(I7::GetName(), szName)) { Retain(); return (I7 *)this; }
		else if (0 == strcmp(I8::GetName(), szName)) { Retain(); return (I8 *)this; }
		else if (0 == strcmp(I9::GetName(), szName)) { Retain(); return (I9 *)this; }
		else return NULL;
	}

	virtual const IInterface *Acquire(const char *szName) const
	{
		     if (0 == strcmp(I1::GetName(),  szName)) { Retain(); return (const I1 *)this;  }
		else if (0 == strcmp(I2::GetName(),  szName)) { Retain(); return (const I2 *)this;  }
		else if (0 == strcmp(I3::GetName(),  szName)) { Retain(); return (const I3 *)this;  }
		else if (0 == strcmp(I4::GetName(),  szName)) { Retain(); return (const I4 *)this;  }
		else if (0 == strcmp(I5::GetName(),  szName)) { Retain(); return (const I5 *)this;  }
		else if (0 == strcmp(I6::GetName(),  szName)) { Retain(); return (const I6 *)this;  }
		else if (0 == strcmp(I7::GetName(),  szName)) { Retain(); return (const I7 *)this;  }
		else if (0 == strcmp(I8::GetName(),  szName)) { Retain(); return (const I8 *)this;  }
		else if (0 == strcmp(I9::GetName(),  szName)) { Retain(); return (const I9 *)this;  }
		else return NULL;
	}

private:

	mutable volatile long m_cRefCount;

};

template
<
	typename OBJ,
	typename I1, typename I2, typename I3, typename I4, typename I5, typename I6, typename I7, typename I8
>
class Object<OBJ, I1, I2, I3, I4, I5, I6, I7, I8> :
	public IObject,
	public I1, public I2, public I3, public I4, public I5, public I6, public I7, public I8
{
public:

	static OBJ *Create()
	{
		OBJ *pObject = new (std::nothrow) OBJ();

		pObject->m_cRefCount = 1;
		((I1 *)pObject)->SetObject(pObject);
		((I2 *)pObject)->SetObject(pObject);
		((I3 *)pObject)->SetObject(pObject);
		((I4 *)pObject)->SetObject(pObject);
		((I5 *)pObject)->SetObject(pObject);
		((I6 *)pObject)->SetObject(pObject);
		((I7 *)pObject)->SetObject(pObject);
		((I8 *)pObject)->SetObject(pObject);

		return pObject;
	}

	virtual long Retain() const
	{
#ifdef _WIN32
		return InterlockedIncrement(&m_cRefCount);
#else
		return __sync_fetch_and_add(&m_cRefCount, 1);
#endif
	}

	virtual long Release() const
	{
		long cRefCount;

#ifdef _WIN32
		if (0 == (cRefCount = InterlockedDecrement(&m_cRefCount)))
#else
		if (0 == (cRefCount = __sync_fetch_and_sub(&m_cRefCount, 1)))
#endif
		{
			delete this;
		}

		return m_cRefCount;
	}

	virtual bool Implements(const char *szName) const
	{
		if (0 == strcmp(I1::GetName(), szName)) return true;
		else if (0 == strcmp(I2::GetName(), szName)) return true;
		else if (0 == strcmp(I3::GetName(), szName)) return true;
		else if (0 == strcmp(I4::GetName(), szName)) return true;
		else if (0 == strcmp(I5::GetName(), szName)) return true;
		else if (0 == strcmp(I6::GetName(), szName)) return true;
		else if (0 == strcmp(I7::GetName(), szName)) return true;
		else if (0 == strcmp(I8::GetName(), szName)) return true;
		else return false;
	}

	virtual IInterface *Acquire(const char *szName)
	{
		if (0 == strcmp(I1::GetName(), szName)) { Retain(); return (I1 *)this; }
		else if (0 == strcmp(I2::GetName(), szName)) { Retain(); return (I2 *)this; }
		else if (0 == strcmp(I3::GetName(), szName)) { Retain(); return (I3 *)this; }
		else if (0 == strcmp(I4::GetName(), szName)) { Retain(); return (I4 *)this; }
		else if (0 == strcmp(I5::GetName(), szName)) { Retain(); return (I5 *)this; }
		else if (0 == strcmp(I6::GetName(), szName)) { Retain(); return (I6 *)this; }
		else if (0 == strcmp(I7::GetName(), szName)) { Retain(); return (I7 *)this; }
		else if (0 == strcmp(I8::GetName(), szName)) { Retain(); return (I8 *)this; }
		else return NULL;
	}

	virtual const IInterface *Acquire(const char *szName) const
	{
		if (0 == strcmp(I1::GetName(), szName)) { Retain(); return (const I1 *)this; }
		else if (0 == strcmp(I2::GetName(), szName)) { Retain(); return (const I2 *)this; }
		else if (0 == strcmp(I3::GetName(), szName)) { Retain(); return (const I3 *)this; }
		else if (0 == strcmp(I4::GetName(), szName)) { Retain(); return (const I4 *)this; }
		else if (0 == strcmp(I5::GetName(), szName)) { Retain(); return (const I5 *)this; }
		else if (0 == strcmp(I6::GetName(), szName)) { Retain(); return (const I6 *)this; }
		else if (0 == strcmp(I7::GetName(), szName)) { Retain(); return (const I7 *)this; }
		else if (0 == strcmp(I8::GetName(), szName)) { Retain(); return (const I8 *)this; }
		else return NULL;
	}

private:

	mutable volatile long m_cRefCount;

};

template 
<
	typename OBJ, 
	typename I1, typename I2, typename I3, typename I4, typename I5, typename I6, typename I7  
>
class Object<OBJ, I1, I2, I3, I4, I5, I6, I7> : 
	public IObject, 
	public I1, public I2, public I3, public I4, public I5, public I6, public I7 
{
public:

	static OBJ *Create()
	{
		OBJ *pObject = new (std::nothrow) OBJ();

		pObject->m_cRefCount = 1;
		((I1 *)pObject)->SetObject(pObject);
		((I2 *)pObject)->SetObject(pObject);
		((I3 *)pObject)->SetObject(pObject);
		((I4 *)pObject)->SetObject(pObject);
		((I5 *)pObject)->SetObject(pObject);
		((I6 *)pObject)->SetObject(pObject);
		((I7 *)pObject)->SetObject(pObject);

		return pObject;
	}

	virtual long Retain() const
	{
#ifdef _WIN32
		return InterlockedIncrement(&m_cRefCount);
#else
		return __sync_fetch_and_add(&m_cRefCount, 1);
#endif
	}

	virtual long Release() const
	{
		long cRefCount;

#ifdef _WIN32
		if (0 == (cRefCount = InterlockedDecrement(&m_cRefCount)))
#else
		if (0 == (cRefCount = __sync_fetch_and_sub(&m_cRefCount, 1)))
#endif
		{
			delete this;
		}

		return m_cRefCount;
	}

	virtual bool Implements(const char *szName) const
	{
		     if (0 == strcmp(I1::GetName(), szName)) return true;
		else if (0 == strcmp(I2::GetName(), szName)) return true;
		else if (0 == strcmp(I3::GetName(), szName)) return true;
		else if (0 == strcmp(I4::GetName(), szName)) return true;
		else if (0 == strcmp(I5::GetName(), szName)) return true;
		else if (0 == strcmp(I6::GetName(), szName)) return true;
		else if (0 == strcmp(I7::GetName(), szName)) return true;
		else return false;
	}

	virtual IInterface *Acquire(const char *szName)
	{
		     if (0 == strcmp(I1::GetName(), szName)) { Retain(); return (I1 *)this; }
		else if (0 == strcmp(I2::GetName(), szName)) { Retain(); return (I2 *)this; }
		else if (0 == strcmp(I3::GetName(), szName)) { Retain(); return (I3 *)this; }
		else if (0 == strcmp(I4::GetName(), szName)) { Retain(); return (I4 *)this; }
		else if (0 == strcmp(I5::GetName(), szName)) { Retain(); return (I5 *)this; }
		else if (0 == strcmp(I6::GetName(), szName)) { Retain(); return (I6 *)this; }
		else if (0 == strcmp(I7::GetName(), szName)) { Retain(); return (I7 *)this; }
		else return NULL;
	}

	virtual const IInterface *Acquire(const char *szName) const
	{
		     if (0 == strcmp(I1::GetName(), szName)) { Retain(); return (const I1 *)this; }
		else if (0 == strcmp(I2::GetName(), szName)) { Retain(); return (const I2 *)this; }
		else if (0 == strcmp(I3::GetName(), szName)) { Retain(); return (const I3 *)this; }
		else if (0 == strcmp(I4::GetName(), szName)) { Retain(); return (const I4 *)this; }
		else if (0 == strcmp(I5::GetName(), szName)) { Retain(); return (const I5 *)this; }
		else if (0 == strcmp(I6::GetName(), szName)) { Retain(); return (const I6 *)this; }
		else if (0 == strcmp(I7::GetName(), szName)) { Retain(); return (const I7 *)this; }
		else return NULL;
	}

private:

	mutable volatile long m_cRefCount;

};

template
<
	typename OBJ,
	typename I1, typename I2, typename I3, typename I4, typename I5, typename I6
>
class Object<OBJ, I1, I2, I3, I4, I5, I6> :
	public IObject,
	public I1, public I2, public I3, public I4, public I5, public I6
{
public:

	static OBJ *Create()
	{
		OBJ *pObject = new (std::nothrow) OBJ();

		pObject->m_cRefCount = 1;
		((I1 *)pObject)->SetObject(pObject);
		((I2 *)pObject)->SetObject(pObject);
		((I3 *)pObject)->SetObject(pObject);
		((I4 *)pObject)->SetObject(pObject);
		((I5 *)pObject)->SetObject(pObject);
		((I6 *)pObject)->SetObject(pObject);

		return pObject;
	}

	virtual long Retain() const
	{
#ifdef _WIN32
		return InterlockedIncrement(&m_cRefCount);
#else
		return __sync_fetch_and_add(&m_cRefCount, 1);
#endif
	}

	virtual long Release() const
	{
		long cRefCount;

#ifdef _WIN32
		if (0 == (cRefCount = InterlockedDecrement(&m_cRefCount)))
#else
		if (0 == (cRefCount = __sync_fetch_and_sub(&m_cRefCount, 1)))
#endif
		{
			delete this;
		}

		return m_cRefCount;
	}

	virtual bool Implements(const char *szName) const
	{
		if (0 == strcmp(I1::GetName(), szName)) return true;
		else if (0 == strcmp(I2::GetName(), szName)) return true;
		else if (0 == strcmp(I3::GetName(), szName)) return true;
		else if (0 == strcmp(I4::GetName(), szName)) return true;
		else if (0 == strcmp(I5::GetName(), szName)) return true;
		else if (0 == strcmp(I6::GetName(), szName)) return true;
		else return false;
	}

	virtual IInterface *Acquire(const char *szName)
	{
		if (0 == strcmp(I1::GetName(), szName)) { Retain(); return (I1 *)this; }
		else if (0 == strcmp(I2::GetName(), szName)) { Retain(); return (I2 *)this; }
		else if (0 == strcmp(I3::GetName(), szName)) { Retain(); return (I3 *)this; }
		else if (0 == strcmp(I4::GetName(), szName)) { Retain(); return (I4 *)this; }
		else if (0 == strcmp(I5::GetName(), szName)) { Retain(); return (I5 *)this; }
		else if (0 == strcmp(I6::GetName(), szName)) { Retain(); return (I6 *)this; }
		else return NULL;
	}

	virtual const IInterface *Acquire(const char *szName) const
	{
		if (0 == strcmp(I1::GetName(), szName)) { Retain(); return (const I1 *)this; }
		else if (0 == strcmp(I2::GetName(), szName)) { Retain(); return (const I2 *)this; }
		else if (0 == strcmp(I3::GetName(), szName)) { Retain(); return (const I3 *)this; }
		else if (0 == strcmp(I4::GetName(), szName)) { Retain(); return (const I4 *)this; }
		else if (0 == strcmp(I5::GetName(), szName)) { Retain(); return (const I5 *)this; }
		else if (0 == strcmp(I6::GetName(), szName)) { Retain(); return (const I6 *)this; }
		else return NULL;
	}

private:

	mutable volatile long m_cRefCount;

};

template 
<
	typename OBJ, 
	typename I1, typename I2, typename I3, typename I4, typename I5 
>
class Object<OBJ, I1, I2, I3, I4, I5> : 
	public IObject, 
	public I1, public I2, public I3, public I4, public I5 
{
public:

	static OBJ *Create()
	{
		OBJ *pObject = new (std::nothrow) OBJ();

		pObject->m_cRefCount = 1;
		((I1 *)pObject)->SetObject(pObject);
		((I2 *)pObject)->SetObject(pObject);
		((I3 *)pObject)->SetObject(pObject);
		((I4 *)pObject)->SetObject(pObject);
		((I5 *)pObject)->SetObject(pObject);

		return pObject;
	}

	virtual long Retain() const
	{
#ifdef _WIN32
		return InterlockedIncrement(&m_cRefCount);
#else
		return __sync_fetch_and_add(&m_cRefCount, 1);
#endif
	}

	virtual long Release() const
	{
		long cRefCount;

#ifdef _WIN32
		if (0 == (cRefCount = InterlockedDecrement(&m_cRefCount)))
#else
		if (0 == (cRefCount = __sync_fetch_and_sub(&m_cRefCount, 1)))
#endif
		{
			delete this;
		}

		return m_cRefCount;
	}

	virtual bool Implements(const char *szName) const
	{
		     if (0 == strcmp(I1::GetName(), szName)) return true;
		else if (0 == strcmp(I2::GetName(), szName)) return true;
		else if (0 == strcmp(I3::GetName(), szName)) return true;
		else if (0 == strcmp(I4::GetName(), szName)) return true;
		else if (0 == strcmp(I5::GetName(), szName)) return true;
		else return false;
	}

	virtual IInterface *Acquire(const char *szName)
	{
		     if (0 == strcmp(I1::GetName(), szName)) { Retain(); return (I1 *)this; }
		else if (0 == strcmp(I2::GetName(), szName)) { Retain(); return (I2 *)this; }
		else if (0 == strcmp(I3::GetName(), szName)) { Retain(); return (I3 *)this; }
		else if (0 == strcmp(I4::GetName(), szName)) { Retain(); return (I4 *)this; }
		else if (0 == strcmp(I5::GetName(), szName)) { Retain(); return (I5 *)this; }
		else return NULL;
	}

	virtual const IInterface *Acquire(const char *szName) const
	{
		     if (0 == strcmp(I1::GetName(), szName)) { Retain(); return (const I1 *)this; }
		else if (0 == strcmp(I2::GetName(), szName)) { Retain(); return (const I2 *)this; }
		else if (0 == strcmp(I3::GetName(), szName)) { Retain(); return (const I3 *)this; }
		else if (0 == strcmp(I4::GetName(), szName)) { Retain(); return (const I4 *)this; }
		else if (0 == strcmp(I5::GetName(), szName)) { Retain(); return (const I5 *)this; }
		else return NULL;
	}

private:

	mutable volatile long m_cRefCount;

};

template 
<
	typename OBJ, 
	typename I1, typename I2, typename I3, typename I4   
>
class Object<OBJ, I1, I2, I3, I4> : 
	public IObject, 
	public I1, public I2, public I3, public I4 
{
public:

	static OBJ *Create()
	{
		OBJ *pObject = new (std::nothrow) OBJ();

		pObject->m_cRefCount = 1;
		((I1 *)pObject)->SetObject(pObject);
		((I2 *)pObject)->SetObject(pObject);
		((I3 *)pObject)->SetObject(pObject);
		((I4 *)pObject)->SetObject(pObject);

		return pObject;
	}

	virtual long Retain() const
	{
#ifdef _WIN32
		return InterlockedIncrement(&m_cRefCount);
#else
		return __sync_fetch_and_add(&m_cRefCount, 1);
#endif
	}

	virtual long Release() const
	{
		long cRefCount;

#ifdef _WIN32
		if (0 == (cRefCount = InterlockedDecrement(&m_cRefCount)))
#else
		if (0 == (cRefCount = __sync_fetch_and_sub(&m_cRefCount, 1)))
#endif
		{
			delete this;
		}

		return m_cRefCount;
	}

	virtual bool Implements(const char *szName) const
	{
		     if (0 == strcmp(I1::GetName(), szName)) return true;
		else if (0 == strcmp(I2::GetName(), szName)) return true;
		else if (0 == strcmp(I3::GetName(), szName)) return true;
		else if (0 == strcmp(I4::GetName(), szName)) return true;
		else return false;
	}

	virtual IInterface *Acquire(const char *szName)
	{
		     if (0 == strcmp(I1::GetName(), szName)) { Retain(); return (I1 *)this; }
		else if (0 == strcmp(I2::GetName(), szName)) { Retain(); return (I2 *)this; }
		else if (0 == strcmp(I3::GetName(), szName)) { Retain(); return (I3 *)this; }
		else if (0 == strcmp(I4::GetName(), szName)) { Retain(); return (I4 *)this; }
		else return NULL;
	}

	virtual const IInterface *Acquire(const char *szName) const
	{
		     if (0 == strcmp(I1::GetName(), szName)) { Retain(); return (const I1 *)this; }
		else if (0 == strcmp(I2::GetName(), szName)) { Retain(); return (const I2 *)this; }
		else if (0 == strcmp(I3::GetName(), szName)) { Retain(); return (const I3 *)this; }
		else if (0 == strcmp(I4::GetName(), szName)) { Retain(); return (const I4 *)this; }
		else return NULL;
	}

private:

	mutable volatile long m_cRefCount;

};

template 
<
	typename OBJ, 
	typename I1, typename I2, typename I3  
>
class Object<OBJ, I1, I2, I3> : 
	public IObject, 
	public I1, public I2, public I3 
{
public:

	static OBJ *Create()
	{
		OBJ *pObject = new (std::nothrow) OBJ();

		pObject->m_cRefCount = 1;
		((I1 *)pObject)->SetObject(pObject);
		((I2 *)pObject)->SetObject(pObject);
		((I3 *)pObject)->SetObject(pObject);

		return pObject;
	}

	virtual long Retain() const
	{
#ifdef _WIN32
		return InterlockedIncrement(&m_cRefCount);
#else
		return __sync_fetch_and_add(&m_cRefCount, 1);
#endif
	}

	virtual long Release() const
	{
		long cRefCount;

#ifdef _WIN32
		if (0 == (cRefCount = InterlockedDecrement(&m_cRefCount)))
#else
		if (0 == (cRefCount = __sync_fetch_and_sub(&m_cRefCount, 1)))
#endif
		{
			delete this;
		}

		return m_cRefCount;
	}

	virtual bool Implements(const char *szName) const
	{
		     if (0 == strcmp(I1::GetName(), szName)) return true;
		else if (0 == strcmp(I2::GetName(), szName)) return true;
		else if (0 == strcmp(I3::GetName(), szName)) return true;
		else return false;
	}

	virtual IInterface *Acquire(const char *szName)
	{
		     if (0 == strcmp(I1::GetName(), szName)) { Retain(); return (I1 *)this; }
		else if (0 == strcmp(I2::GetName(), szName)) { Retain(); return (I2 *)this; }
		else if (0 == strcmp(I3::GetName(), szName)) { Retain(); return (I3 *)this; }
		else return NULL;
	}

	virtual const IInterface *Acquire(const char *szName) const
	{
		     if (0 == strcmp(I1::GetName(), szName)) { Retain(); return (const I1 *)this; }
		else if (0 == strcmp(I2::GetName(), szName)) { Retain(); return (const I2 *)this; }
		else if (0 == strcmp(I3::GetName(), szName)) { Retain(); return (const I3 *)this; }
		else return NULL;
	}

private:

	mutable volatile long m_cRefCount;

};

template 
<
	typename OBJ, 
	typename I1, typename I2 
>
class Object<OBJ, I1, I2> : 
	public IObject, 
	public I1, public I2
{
public:

	static OBJ *Create()
	{
		OBJ *pObject = new (std::nothrow) OBJ();

		pObject->m_cRefCount = 1;
		((I1 *)pObject)->SetObject(pObject);
		((I2 *)pObject)->SetObject(pObject);

		return pObject;
	}

	virtual long Retain() const
	{
#ifdef _WIN32
		return InterlockedIncrement(&m_cRefCount);
#else
		return __sync_fetch_and_add(&m_cRefCount, 1);
#endif
	}

	virtual long Release() const
	{
		long cRefCount;

#ifdef _WIN32
		if (0 == (cRefCount = InterlockedDecrement(&m_cRefCount)))
#else
		if (0 == (cRefCount = __sync_fetch_and_sub(&m_cRefCount, 1)))
#endif
		{
			delete this;
		}

		return m_cRefCount;
	}

	virtual bool Implements(const char *szName) const
	{
		     if (0 == strcmp(I1::GetName(), szName)) return true;
		else if (0 == strcmp(I2::GetName(), szName)) return true;
		else return false;
	}

	virtual IInterface *Acquire(const char *szName)
	{
		     if (0 == strcmp(I1::GetName(), szName)) { Retain(); return (I1 *)this; }
		else if (0 == strcmp(I2::GetName(), szName)) { Retain(); return (I2 *)this; }
		else return NULL;
	}

	virtual const IInterface *Acquire(const char *szName) const
	{
		     if (0 == strcmp(I1::GetName(), szName)) { Retain(); return (const I1 *)this; }
		else if (0 == strcmp(I2::GetName(), szName)) { Retain(); return (const I2 *)this; }
		else return NULL;
	}

private:

	mutable volatile long m_cRefCount;

};

template 
<
	typename OBJ, 
	typename I1 
>
class Object<OBJ, I1> : public IObject, public I1
{
public:

	static OBJ *Create()
	{
		OBJ *pObject = new (std::nothrow) OBJ();

		pObject->m_cRefCount = 1;
		((I1 *)pObject)->SetObject(pObject);

		return pObject;
	}

	virtual long Retain() const
	{
#ifdef _WIN32
		return InterlockedIncrement(&m_cRefCount);
#else
		return __sync_fetch_and_add(&m_cRefCount, 1);
#endif
	}

	virtual long Release() const
	{
		long cRefCount;

#ifdef _WIN32
		if (0 == (cRefCount = InterlockedDecrement(&m_cRefCount)))
#else
		if (0 == (cRefCount = __sync_fetch_and_sub(&m_cRefCount, 1)))
#endif
		{
			delete this;
		}

		return m_cRefCount;
	}

	virtual bool Implements(const char *szName) const
	{
		     if (0 == strcmp(I1::GetName(), szName)) return true;
		else return false;
	}

	virtual IInterface *Acquire(const char *szName)
	{
		     if (0 == strcmp(I1::GetName(), szName)) { Retain(); return (I1 *)this; }
		else return NULL;
	}

	virtual const IInterface *Acquire(const char *szName) const
	{
		     if (0 == strcmp(I1::GetName(), szName)) { Retain(); return (const I1 *)this; }
		else return NULL;
	}

private:

	mutable volatile long m_cRefCount;

};

}//namespace CX
