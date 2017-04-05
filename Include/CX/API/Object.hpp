/* 
 * The author disclaims copyright to this source code.
 */ 
 
#pragma once


#include <string.h>
#include <stdlib.h>
#include <new>


namespace CX
{

template
<
	typename OBJECT_CLASS, typename OBJECT_INTERFACE,
	typename O1  = IObject::NullObject, typename O2  = IObject::NullObject, typename O3  = IObject::NullObject, 
	typename O4  = IObject::NullObject, typename O5  = IObject::NullObject, typename O6  = IObject::NullObject,
	typename O7  = IObject::NullObject, typename O8  = IObject::NullObject, typename O9  = IObject::NullObject,
	typename O10 = IObject::NullObject, typename O11 = IObject::NullObject, typename O12 = IObject::NullObject,
	typename O13 = IObject::NullObject, typename O14 = IObject::NullObject, typename O15 = IObject::NullObject,
	typename O16 = IObject::NullObject, typename O17 = IObject::NullObject 
>
class Object;

template
<
	typename OBJECT_CLASS, typename OBJECT_INTERFACE,
	typename O1 , typename O2 , typename O3 , typename O4 , typename O5 , typename O6 , typename O7 , typename O8 , 
	typename O9 , typename O10, typename O11, typename O12, typename O13, typename O14, typename O15, typename O16 
>
class Object<OBJECT_CLASS, OBJECT_INTERFACE, O1, O2, O3, O4, O5, O6, O7, O8, O9, O10, O11, O12, O13, O14, O15, O16> : 
	public OBJECT_INTERFACE
{
public:

	static OBJECT_CLASS *Create(IObject *pParent = NULL)
	{
		OBJECT_CLASS *pObject;

		if (NULL == (pObject = new (std::nothrow) OBJECT_CLASS()))
		{
			return NULL;
		}
		pObject->InitObject(pParent);

		return pObject;
	}

	virtual bool Implements(const char *szName) const
	{
		     if (0 == strcmp(szName, GetObjectName())) return True;
		else if (0 == strcmp(szName, __p1__->GetObjectName())) return True;
		else if (0 == strcmp(szName, __p2__->GetObjectName())) return True;
		else if (0 == strcmp(szName, __p3__->GetObjectName())) return True;
		else if (0 == strcmp(szName, __p4__->GetObjectName())) return True;
		else if (0 == strcmp(szName, __p5__->GetObjectName())) return True;
		else if (0 == strcmp(szName, __p6__->GetObjectName())) return True;
		else if (0 == strcmp(szName, __p7__->GetObjectName())) return True;
		else if (0 == strcmp(szName, __p8__->GetObjectName())) return True;
		else if (0 == strcmp(szName, __p9__->GetObjectName())) return True;
		else if (0 == strcmp(szName, __p10__->GetObjectName())) return True;
		else if (0 == strcmp(szName, __p11__->GetObjectName())) return True;
		else if (0 == strcmp(szName, __p12__->GetObjectName())) return True;
		else if (0 == strcmp(szName, __p13__->GetObjectName())) return True;
		else if (0 == strcmp(szName, __p14__->GetObjectName())) return True;
		else if (0 == strcmp(szName, __p15__->GetObjectName())) return True;
		else if (0 == strcmp(szName, __p16__->GetObjectName())) return True;
		else return False;
	}

	virtual IObject *Acquire(const char *szName)
	{
		     if (0 == strcmp(szName, GetObjectName())) { Retain(); return this; }
		else if (0 == strcmp(szName, __p1__->GetObjectName())) { Retain(); return __p1__; }
		else if (0 == strcmp(szName, __p2__->GetObjectName())) { Retain(); return __p2__; }
		else if (0 == strcmp(szName, __p3__->GetObjectName())) { Retain(); return __p3__; }
		else if (0 == strcmp(szName, __p4__->GetObjectName())) { Retain(); return __p4__; }
		else if (0 == strcmp(szName, __p5__->GetObjectName())) { Retain(); return __p5__; }
		else if (0 == strcmp(szName, __p6__->GetObjectName())) { Retain(); return __p6__; }
		else if (0 == strcmp(szName, __p7__->GetObjectName())) { Retain(); return __p7__; }
		else if (0 == strcmp(szName, __p8__->GetObjectName())) { Retain(); return __p8__; }
		else if (0 == strcmp(szName, __p9__->GetObjectName())) { Retain(); return __p9__; }
		else if (0 == strcmp(szName, __p10__->GetObjectName())) { Retain(); return __p10__; }
		else if (0 == strcmp(szName, __p11__->GetObjectName())) { Retain(); return __p11__; }
		else if (0 == strcmp(szName, __p12__->GetObjectName())) { Retain(); return __p12__; }
		else if (0 == strcmp(szName, __p13__->GetObjectName())) { Retain(); return __p13__; }
		else if (0 == strcmp(szName, __p14__->GetObjectName())) { Retain(); return __p14__; }
		else if (0 == strcmp(szName, __p15__->GetObjectName())) { Retain(); return __p15__; }
		else if (0 == strcmp(szName, __p16__->GetObjectName())) { Retain(); return __p16__; }
		else return NULL;
	}

	virtual const IObject *Acquire(const char *szName) const
	{
		     if (0 == strcmp(szName, GetObjectName())) { Retain(); return this; }
		else if (0 == strcmp(szName, __p1__->GetObjectName())) { Retain(); return __p1__; }
		else if (0 == strcmp(szName, __p2__->GetObjectName())) { Retain(); return __p2__; }
		else if (0 == strcmp(szName, __p3__->GetObjectName())) { Retain(); return __p3__; }
		else if (0 == strcmp(szName, __p4__->GetObjectName())) { Retain(); return __p4__; }
		else if (0 == strcmp(szName, __p5__->GetObjectName())) { Retain(); return __p5__; }
		else if (0 == strcmp(szName, __p6__->GetObjectName())) { Retain(); return __p6__; }
		else if (0 == strcmp(szName, __p7__->GetObjectName())) { Retain(); return __p7__; }
		else if (0 == strcmp(szName, __p8__->GetObjectName())) { Retain(); return __p8__; }
		else if (0 == strcmp(szName, __p9__->GetObjectName())) { Retain(); return __p9__; }
		else if (0 == strcmp(szName, __p10__->GetObjectName())) { Retain(); return __p10__; }
		else if (0 == strcmp(szName, __p11__->GetObjectName())) { Retain(); return __p11__; }
		else if (0 == strcmp(szName, __p12__->GetObjectName())) { Retain(); return __p12__; }
		else if (0 == strcmp(szName, __p13__->GetObjectName())) { Retain(); return __p13__; }
		else if (0 == strcmp(szName, __p14__->GetObjectName())) { Retain(); return __p14__; }
		else if (0 == strcmp(szName, __p15__->GetObjectName())) { Retain(); return __p15__; }
		else if (0 == strcmp(szName, __p16__->GetObjectName())) { Retain(); return __p16__; }
		else return NULL;
	}

protected:

	void InitObject(IObject *pParent = NULL)
	{
		SetupObject(pParent);

		__p1__  = O1::Create(this);
		__p2__  = O2::Create(this);
		__p3__  = O3::Create(this);
		__p4__  = O4::Create(this);
		__p5__  = O5::Create(this);
		__p6__  = O6::Create(this);
		__p7__  = O7::Create(this);
		__p8__  = O8::Create(this);
		__p9__  = O9::Create(this);
		__p10__ = O10::Create(this);
		__p11__ = O11::Create(this);
		__p12__ = O12::Create(this);
		__p13__ = O13::Create(this);
		__p14__ = O14::Create(this);
		__p15__ = O15::Create(this);
		__p16__ = O16::Create(this);

		OnInitObject();
	}

	virtual void UninitObject() const
	{
		delete this;
	}

	~Object()
	{
		OnUninitObject();

		__p1__->SetupObject(NULL); __p1__->Release();
		__p2__->SetupObject(NULL); __p2__->Release();
		__p3__->SetupObject(NULL); __p3__->Release();
		__p4__->SetupObject(NULL); __p4__->Release();
		__p5__->SetupObject(NULL); __p5__->Release();
		__p6__->SetupObject(NULL); __p6__->Release();
		__p7__->SetupObject(NULL); __p7__->Release();
		__p8__->SetupObject(NULL); __p8__->Release();
		__p9__->SetupObject(NULL); __p9__->Release();
		__p10__->SetupObject(NULL); __p10__->Release();
		__p11__->SetupObject(NULL); __p11__->Release();
		__p12__->SetupObject(NULL); __p12__->Release();
		__p13__->SetupObject(NULL); __p13__->Release();
		__p14__->SetupObject(NULL); __p14__->Release();
		__p15__->SetupObject(NULL); __p15__->Release();
		__p16__->SetupObject(NULL); __p16__->Release();
	}

private:

	O1  *__p1__;
	O2  *__p2__;
	O3  *__p3__;
	O4  *__p4__;
	O5  *__p5__;
	O6  *__p6__;
	O7  *__p7__;
	O8  *__p8__;
	O9  *__p9__;
	O10 *__p10__;
	O11 *__p11__;
	O12 *__p12__;
	O13 *__p13__;
	O14 *__p14__;
	O15 *__p15__;
	O16 *__p16__;

	static inline void __stg__(OBJECT_INTERFACE *p) { };
	static inline void __stg__(O1 *p) { };
	static inline void __stg__(O2 *p) { };
	static inline void __stg__(O3 *p) { };
	static inline void __stg__(O4 *p) { };
	static inline void __stg__(O5 *p) { };
	static inline void __stg__(O6 *p) { };
	static inline void __stg__(O7 *p) { };
	static inline void __stg__(O8 *p) { };
	static inline void __stg__(O9 *p) { };
	static inline void __stg__(O10 *p) { };
	static inline void __stg__(O11 *p) { };
	static inline void __stg__(O12 *p) { };
	static inline void __stg__(O13 *p) { };
	static inline void __stg__(O14 *p) { };
	static inline void __stg__(O15 *p) { };
	static inline void __stg__(O16 *p) { };

};

template
<
	typename OBJECT_CLASS, typename OBJECT_INTERFACE,
	typename O1 , typename O2 , typename O3 , typename O4 , typename O5 , typename O6 , typename O7 , typename O8 , 
	typename O9 , typename O10, typename O11, typename O12, typename O13, typename O14, typename O15 
>
class Object<OBJECT_CLASS, OBJECT_INTERFACE, O1, O2, O3, O4, O5, O6, O7, O8, O9, O10, O11, O12, O13, O14, O15> : 
	public OBJECT_INTERFACE
{
public:

	static OBJECT_CLASS *Create(IObject *pParent = NULL)
	{
		OBJECT_CLASS *pObject;

		if (NULL == (pObject = new (std::nothrow) OBJECT_CLASS()))
		{
			return NULL;
		}
		pObject->InitObject(pParent);

		return pObject;
	}

	virtual bool Implements(const char *szName) const
	{
		     if (0 == strcmp(szName, GetObjectName())) return True;
		else if (0 == strcmp(szName, __p1__->GetObjectName())) return True;
		else if (0 == strcmp(szName, __p2__->GetObjectName())) return True;
		else if (0 == strcmp(szName, __p3__->GetObjectName())) return True;
		else if (0 == strcmp(szName, __p4__->GetObjectName())) return True;
		else if (0 == strcmp(szName, __p5__->GetObjectName())) return True;
		else if (0 == strcmp(szName, __p6__->GetObjectName())) return True;
		else if (0 == strcmp(szName, __p7__->GetObjectName())) return True;
		else if (0 == strcmp(szName, __p8__->GetObjectName())) return True;
		else if (0 == strcmp(szName, __p9__->GetObjectName())) return True;
		else if (0 == strcmp(szName, __p10__->GetObjectName())) return True;
		else if (0 == strcmp(szName, __p11__->GetObjectName())) return True;
		else if (0 == strcmp(szName, __p12__->GetObjectName())) return True;
		else if (0 == strcmp(szName, __p13__->GetObjectName())) return True;
		else if (0 == strcmp(szName, __p14__->GetObjectName())) return True;
		else if (0 == strcmp(szName, __p15__->GetObjectName())) return True;
		else return False;
	}

	virtual IObject *Acquire(const char *szName)
	{
		     if (0 == strcmp(szName, GetObjectName())) { Retain(); return this; }
		else if (0 == strcmp(szName, __p1__->GetObjectName())) { Retain(); return __p1__; }
		else if (0 == strcmp(szName, __p2__->GetObjectName())) { Retain(); return __p2__; }
		else if (0 == strcmp(szName, __p3__->GetObjectName())) { Retain(); return __p3__; }
		else if (0 == strcmp(szName, __p4__->GetObjectName())) { Retain(); return __p4__; }
		else if (0 == strcmp(szName, __p5__->GetObjectName())) { Retain(); return __p5__; }
		else if (0 == strcmp(szName, __p6__->GetObjectName())) { Retain(); return __p6__; }
		else if (0 == strcmp(szName, __p7__->GetObjectName())) { Retain(); return __p7__; }
		else if (0 == strcmp(szName, __p8__->GetObjectName())) { Retain(); return __p8__; }
		else if (0 == strcmp(szName, __p9__->GetObjectName())) { Retain(); return __p9__; }
		else if (0 == strcmp(szName, __p10__->GetObjectName())) { Retain(); return __p10__; }
		else if (0 == strcmp(szName, __p11__->GetObjectName())) { Retain(); return __p11__; }
		else if (0 == strcmp(szName, __p12__->GetObjectName())) { Retain(); return __p12__; }
		else if (0 == strcmp(szName, __p13__->GetObjectName())) { Retain(); return __p13__; }
		else if (0 == strcmp(szName, __p14__->GetObjectName())) { Retain(); return __p14__; }
		else if (0 == strcmp(szName, __p15__->GetObjectName())) { Retain(); return __p15__; }
		else return NULL;
	}

	virtual const IObject *Acquire(const char *szName) const
	{
		     if (0 == strcmp(szName, GetObjectName())) { Retain(); return this; }
		else if (0 == strcmp(szName, __p1__->GetObjectName())) { Retain(); return __p1__; }
		else if (0 == strcmp(szName, __p2__->GetObjectName())) { Retain(); return __p2__; }
		else if (0 == strcmp(szName, __p3__->GetObjectName())) { Retain(); return __p3__; }
		else if (0 == strcmp(szName, __p4__->GetObjectName())) { Retain(); return __p4__; }
		else if (0 == strcmp(szName, __p5__->GetObjectName())) { Retain(); return __p5__; }
		else if (0 == strcmp(szName, __p6__->GetObjectName())) { Retain(); return __p6__; }
		else if (0 == strcmp(szName, __p7__->GetObjectName())) { Retain(); return __p7__; }
		else if (0 == strcmp(szName, __p8__->GetObjectName())) { Retain(); return __p8__; }
		else if (0 == strcmp(szName, __p9__->GetObjectName())) { Retain(); return __p9__; }
		else if (0 == strcmp(szName, __p10__->GetObjectName())) { Retain(); return __p10__; }
		else if (0 == strcmp(szName, __p11__->GetObjectName())) { Retain(); return __p11__; }
		else if (0 == strcmp(szName, __p12__->GetObjectName())) { Retain(); return __p12__; }
		else if (0 == strcmp(szName, __p13__->GetObjectName())) { Retain(); return __p13__; }
		else if (0 == strcmp(szName, __p14__->GetObjectName())) { Retain(); return __p14__; }
		else if (0 == strcmp(szName, __p15__->GetObjectName())) { Retain(); return __p15__; }
		else return NULL;
	}

protected:

	void InitObject(IObject *pParent = NULL)
	{
		SetupObject(pParent);

		__p1__  = O1::Create(this);
		__p2__  = O2::Create(this);
		__p3__  = O3::Create(this);
		__p4__  = O4::Create(this);
		__p5__  = O5::Create(this);
		__p6__  = O6::Create(this);
		__p7__  = O7::Create(this);
		__p8__  = O8::Create(this);
		__p9__  = O9::Create(this);
		__p10__ = O10::Create(this);
		__p11__ = O11::Create(this);
		__p12__ = O12::Create(this);
		__p13__ = O13::Create(this);
		__p14__ = O14::Create(this);
		__p15__ = O15::Create(this);

		OnInitObject();
	}

	virtual void UninitObject() const
	{
		delete this;
	}

	~Object()
	{
		OnUninitObject();

		__p1__->SetupObject(NULL); __p1__->Release();
		__p2__->SetupObject(NULL); __p2__->Release();
		__p3__->SetupObject(NULL); __p3__->Release();
		__p4__->SetupObject(NULL); __p4__->Release();
		__p5__->SetupObject(NULL); __p5__->Release();
		__p6__->SetupObject(NULL); __p6__->Release();
		__p7__->SetupObject(NULL); __p7__->Release();
		__p8__->SetupObject(NULL); __p8__->Release();
		__p9__->SetupObject(NULL); __p9__->Release();
		__p10__->SetupObject(NULL); __p10__->Release();
		__p11__->SetupObject(NULL); __p11__->Release();
		__p12__->SetupObject(NULL); __p12__->Release();
		__p13__->SetupObject(NULL); __p13__->Release();
		__p14__->SetupObject(NULL); __p14__->Release();
		__p15__->SetupObject(NULL); __p15__->Release();
	}

private:

	O1  *__p1__;
	O2  *__p2__;
	O3  *__p3__;
	O4  *__p4__;
	O5  *__p5__;
	O6  *__p6__;
	O7  *__p7__;
	O8  *__p8__;
	O9  *__p9__;
	O10 *__p10__;
	O11 *__p11__;
	O12 *__p12__;
	O13 *__p13__;
	O14 *__p14__;
	O15 *__p15__;

	static inline void __stg__(OBJECT_INTERFACE *p) { };
	static inline void __stg__(O1 *p) { };
	static inline void __stg__(O2 *p) { };
	static inline void __stg__(O3 *p) { };
	static inline void __stg__(O4 *p) { };
	static inline void __stg__(O5 *p) { };
	static inline void __stg__(O6 *p) { };
	static inline void __stg__(O7 *p) { };
	static inline void __stg__(O8 *p) { };
	static inline void __stg__(O9 *p) { };
	static inline void __stg__(O10 *p) { };
	static inline void __stg__(O11 *p) { };
	static inline void __stg__(O12 *p) { };
	static inline void __stg__(O13 *p) { };
	static inline void __stg__(O14 *p) { };
	static inline void __stg__(O15 *p) { };

};

template
<
	typename OBJECT_CLASS, typename OBJECT_INTERFACE,
	typename O1 , typename O2 , typename O3 , typename O4 , typename O5 , typename O6 , typename O7 , typename O8 , 
	typename O9 , typename O10, typename O11, typename O12, typename O13, typename O14 
>
class Object<OBJECT_CLASS, OBJECT_INTERFACE, O1, O2, O3, O4, O5, O6, O7, O8, O9, O10, O11, O12, O13, O14> : 
	public OBJECT_INTERFACE
{
public:

	static OBJECT_CLASS *Create(IObject *pParent = NULL)
	{
		OBJECT_CLASS *pObject;

		if (NULL == (pObject = new (std::nothrow) OBJECT_CLASS()))
		{
			return NULL;
		}
		pObject->InitObject(pParent);

		return pObject;
	}

	virtual bool Implements(const char *szName) const
	{
		     if (0 == strcmp(szName, GetObjectName())) return True;
		else if (0 == strcmp(szName, __p1__->GetObjectName())) return True;
		else if (0 == strcmp(szName, __p2__->GetObjectName())) return True;
		else if (0 == strcmp(szName, __p3__->GetObjectName())) return True;
		else if (0 == strcmp(szName, __p4__->GetObjectName())) return True;
		else if (0 == strcmp(szName, __p5__->GetObjectName())) return True;
		else if (0 == strcmp(szName, __p6__->GetObjectName())) return True;
		else if (0 == strcmp(szName, __p7__->GetObjectName())) return True;
		else if (0 == strcmp(szName, __p8__->GetObjectName())) return True;
		else if (0 == strcmp(szName, __p9__->GetObjectName())) return True;
		else if (0 == strcmp(szName, __p10__->GetObjectName())) return True;
		else if (0 == strcmp(szName, __p11__->GetObjectName())) return True;
		else if (0 == strcmp(szName, __p12__->GetObjectName())) return True;
		else if (0 == strcmp(szName, __p13__->GetObjectName())) return True;
		else if (0 == strcmp(szName, __p14__->GetObjectName())) return True;
		else return False;
	}

	virtual IObject *Acquire(const char *szName)
	{
		     if (0 == strcmp(szName, GetObjectName())) { Retain(); return this; }
		else if (0 == strcmp(szName, __p1__->GetObjectName())) { Retain(); return __p1__; }
		else if (0 == strcmp(szName, __p2__->GetObjectName())) { Retain(); return __p2__; }
		else if (0 == strcmp(szName, __p3__->GetObjectName())) { Retain(); return __p3__; }
		else if (0 == strcmp(szName, __p4__->GetObjectName())) { Retain(); return __p4__; }
		else if (0 == strcmp(szName, __p5__->GetObjectName())) { Retain(); return __p5__; }
		else if (0 == strcmp(szName, __p6__->GetObjectName())) { Retain(); return __p6__; }
		else if (0 == strcmp(szName, __p7__->GetObjectName())) { Retain(); return __p7__; }
		else if (0 == strcmp(szName, __p8__->GetObjectName())) { Retain(); return __p8__; }
		else if (0 == strcmp(szName, __p9__->GetObjectName())) { Retain(); return __p9__; }
		else if (0 == strcmp(szName, __p10__->GetObjectName())) { Retain(); return __p10__; }
		else if (0 == strcmp(szName, __p11__->GetObjectName())) { Retain(); return __p11__; }
		else if (0 == strcmp(szName, __p12__->GetObjectName())) { Retain(); return __p12__; }
		else if (0 == strcmp(szName, __p13__->GetObjectName())) { Retain(); return __p13__; }
		else if (0 == strcmp(szName, __p14__->GetObjectName())) { Retain(); return __p14__; }
		else return NULL;
	}

	virtual const IObject *Acquire(const char *szName) const
	{
		     if (0 == strcmp(szName, GetObjectName())) { Retain(); return this; }
		else if (0 == strcmp(szName, __p1__->GetObjectName())) { Retain(); return __p1__; }
		else if (0 == strcmp(szName, __p2__->GetObjectName())) { Retain(); return __p2__; }
		else if (0 == strcmp(szName, __p3__->GetObjectName())) { Retain(); return __p3__; }
		else if (0 == strcmp(szName, __p4__->GetObjectName())) { Retain(); return __p4__; }
		else if (0 == strcmp(szName, __p5__->GetObjectName())) { Retain(); return __p5__; }
		else if (0 == strcmp(szName, __p6__->GetObjectName())) { Retain(); return __p6__; }
		else if (0 == strcmp(szName, __p7__->GetObjectName())) { Retain(); return __p7__; }
		else if (0 == strcmp(szName, __p8__->GetObjectName())) { Retain(); return __p8__; }
		else if (0 == strcmp(szName, __p9__->GetObjectName())) { Retain(); return __p9__; }
		else if (0 == strcmp(szName, __p10__->GetObjectName())) { Retain(); return __p10__; }
		else if (0 == strcmp(szName, __p11__->GetObjectName())) { Retain(); return __p11__; }
		else if (0 == strcmp(szName, __p12__->GetObjectName())) { Retain(); return __p12__; }
		else if (0 == strcmp(szName, __p13__->GetObjectName())) { Retain(); return __p13__; }
		else if (0 == strcmp(szName, __p14__->GetObjectName())) { Retain(); return __p14__; }
		else return NULL;
	}

protected:

	void InitObject(IObject *pParent = NULL)
	{
		SetupObject(pParent);

		__p1__  = O1::Create(this);
		__p2__  = O2::Create(this);
		__p3__  = O3::Create(this);
		__p4__  = O4::Create(this);
		__p5__  = O5::Create(this);
		__p6__  = O6::Create(this);
		__p7__  = O7::Create(this);
		__p8__  = O8::Create(this);
		__p9__  = O9::Create(this);
		__p10__ = O10::Create(this);
		__p11__ = O11::Create(this);
		__p12__ = O12::Create(this);
		__p13__ = O13::Create(this);
		__p14__ = O14::Create(this);

		OnInitObject();
	}

	virtual void UninitObject() const
	{
		delete this;
	}

	~Object()
	{
		OnUninitObject();

		__p1__->SetupObject(NULL); __p1__->Release();
		__p2__->SetupObject(NULL); __p2__->Release();
		__p3__->SetupObject(NULL); __p3__->Release();
		__p4__->SetupObject(NULL); __p4__->Release();
		__p5__->SetupObject(NULL); __p5__->Release();
		__p6__->SetupObject(NULL); __p6__->Release();
		__p7__->SetupObject(NULL); __p7__->Release();
		__p8__->SetupObject(NULL); __p8__->Release();
		__p9__->SetupObject(NULL); __p9__->Release();
		__p10__->SetupObject(NULL); __p10__->Release();
		__p11__->SetupObject(NULL); __p11__->Release();
		__p12__->SetupObject(NULL); __p12__->Release();
		__p13__->SetupObject(NULL); __p13__->Release();
		__p14__->SetupObject(NULL); __p14__->Release();
	}

private:

	O1  *__p1__;
	O2  *__p2__;
	O3  *__p3__;
	O4  *__p4__;
	O5  *__p5__;
	O6  *__p6__;
	O7  *__p7__;
	O8  *__p8__;
	O9  *__p9__;
	O10 *__p10__;
	O11 *__p11__;
	O12 *__p12__;
	O13 *__p13__;
	O14 *__p14__;

	static inline void __stg__(OBJECT_INTERFACE *p) { };
	static inline void __stg__(O1 *p) { };
	static inline void __stg__(O2 *p) { };
	static inline void __stg__(O3 *p) { };
	static inline void __stg__(O4 *p) { };
	static inline void __stg__(O5 *p) { };
	static inline void __stg__(O6 *p) { };
	static inline void __stg__(O7 *p) { };
	static inline void __stg__(O8 *p) { };
	static inline void __stg__(O9 *p) { };
	static inline void __stg__(O10 *p) { };
	static inline void __stg__(O11 *p) { };
	static inline void __stg__(O12 *p) { };
	static inline void __stg__(O13 *p) { };
	static inline void __stg__(O14 *p) { };

};

template
<
	typename OBJECT_CLASS, typename OBJECT_INTERFACE,
	typename O1 , typename O2 , typename O3 , typename O4 , typename O5 , typename O6 , typename O7 , typename O8 , 
	typename O9 , typename O10, typename O11, typename O12, typename O13 
>
class Object<OBJECT_CLASS, OBJECT_INTERFACE, O1, O2, O3, O4, O5, O6, O7, O8, O9, O10, O11, O12, O13> : 
	public OBJECT_INTERFACE
{
public:

	static OBJECT_CLASS *Create(IObject *pParent = NULL)
	{
		OBJECT_CLASS *pObject;

		if (NULL == (pObject = new (std::nothrow) OBJECT_CLASS()))
		{
			return NULL;
		}
		pObject->InitObject(pParent);

		return pObject;
	}

	virtual bool Implements(const char *szName) const
	{
		     if (0 == strcmp(szName, GetObjectName())) return True;
		else if (0 == strcmp(szName, __p1__->GetObjectName())) return True;
		else if (0 == strcmp(szName, __p2__->GetObjectName())) return True;
		else if (0 == strcmp(szName, __p3__->GetObjectName())) return True;
		else if (0 == strcmp(szName, __p4__->GetObjectName())) return True;
		else if (0 == strcmp(szName, __p5__->GetObjectName())) return True;
		else if (0 == strcmp(szName, __p6__->GetObjectName())) return True;
		else if (0 == strcmp(szName, __p7__->GetObjectName())) return True;
		else if (0 == strcmp(szName, __p8__->GetObjectName())) return True;
		else if (0 == strcmp(szName, __p9__->GetObjectName())) return True;
		else if (0 == strcmp(szName, __p10__->GetObjectName())) return True;
		else if (0 == strcmp(szName, __p11__->GetObjectName())) return True;
		else if (0 == strcmp(szName, __p12__->GetObjectName())) return True;
		else if (0 == strcmp(szName, __p13__->GetObjectName())) return True;
		else return False;
	}

	virtual IObject *Acquire(const char *szName)
	{
		     if (0 == strcmp(szName, GetObjectName())) { Retain(); return this; }
		else if (0 == strcmp(szName, __p1__->GetObjectName())) { Retain(); return __p1__; }
		else if (0 == strcmp(szName, __p2__->GetObjectName())) { Retain(); return __p2__; }
		else if (0 == strcmp(szName, __p3__->GetObjectName())) { Retain(); return __p3__; }
		else if (0 == strcmp(szName, __p4__->GetObjectName())) { Retain(); return __p4__; }
		else if (0 == strcmp(szName, __p5__->GetObjectName())) { Retain(); return __p5__; }
		else if (0 == strcmp(szName, __p6__->GetObjectName())) { Retain(); return __p6__; }
		else if (0 == strcmp(szName, __p7__->GetObjectName())) { Retain(); return __p7__; }
		else if (0 == strcmp(szName, __p8__->GetObjectName())) { Retain(); return __p8__; }
		else if (0 == strcmp(szName, __p9__->GetObjectName())) { Retain(); return __p9__; }
		else if (0 == strcmp(szName, __p10__->GetObjectName())) { Retain(); return __p10__; }
		else if (0 == strcmp(szName, __p11__->GetObjectName())) { Retain(); return __p11__; }
		else if (0 == strcmp(szName, __p12__->GetObjectName())) { Retain(); return __p12__; }
		else if (0 == strcmp(szName, __p13__->GetObjectName())) { Retain(); return __p13__; }
		else return NULL;
	}

	virtual const IObject *Acquire(const char *szName) const
	{
		     if (0 == strcmp(szName, GetObjectName())) { Retain(); return this; }
		else if (0 == strcmp(szName, __p1__->GetObjectName())) { Retain(); return __p1__; }
		else if (0 == strcmp(szName, __p2__->GetObjectName())) { Retain(); return __p2__; }
		else if (0 == strcmp(szName, __p3__->GetObjectName())) { Retain(); return __p3__; }
		else if (0 == strcmp(szName, __p4__->GetObjectName())) { Retain(); return __p4__; }
		else if (0 == strcmp(szName, __p5__->GetObjectName())) { Retain(); return __p5__; }
		else if (0 == strcmp(szName, __p6__->GetObjectName())) { Retain(); return __p6__; }
		else if (0 == strcmp(szName, __p7__->GetObjectName())) { Retain(); return __p7__; }
		else if (0 == strcmp(szName, __p8__->GetObjectName())) { Retain(); return __p8__; }
		else if (0 == strcmp(szName, __p9__->GetObjectName())) { Retain(); return __p9__; }
		else if (0 == strcmp(szName, __p10__->GetObjectName())) { Retain(); return __p10__; }
		else if (0 == strcmp(szName, __p11__->GetObjectName())) { Retain(); return __p11__; }
		else if (0 == strcmp(szName, __p12__->GetObjectName())) { Retain(); return __p12__; }
		else if (0 == strcmp(szName, __p13__->GetObjectName())) { Retain(); return __p13__; }
		else return NULL;
	}

protected:

	void InitObject(IObject *pParent = NULL)
	{
		SetupObject(pParent);

		__p1__  = O1::Create(this);
		__p2__  = O2::Create(this);
		__p3__  = O3::Create(this);
		__p4__  = O4::Create(this);
		__p5__  = O5::Create(this);
		__p6__  = O6::Create(this);
		__p7__  = O7::Create(this);
		__p8__  = O8::Create(this);
		__p9__  = O9::Create(this);
		__p10__ = O10::Create(this);
		__p11__ = O11::Create(this);
		__p12__ = O12::Create(this);
		__p13__ = O13::Create(this);

		OnInitObject();
	}

	virtual void UninitObject() const
	{
		delete this;
	}

	~Object()
	{
		OnUninitObject();

		__p1__->SetupObject(NULL); __p1__->Release();
		__p2__->SetupObject(NULL); __p2__->Release();
		__p3__->SetupObject(NULL); __p3__->Release();
		__p4__->SetupObject(NULL); __p4__->Release();
		__p5__->SetupObject(NULL); __p5__->Release();
		__p6__->SetupObject(NULL); __p6__->Release();
		__p7__->SetupObject(NULL); __p7__->Release();
		__p8__->SetupObject(NULL); __p8__->Release();
		__p9__->SetupObject(NULL); __p9__->Release();
		__p10__->SetupObject(NULL); __p10__->Release();
		__p11__->SetupObject(NULL); __p11__->Release();
		__p12__->SetupObject(NULL); __p12__->Release();
		__p13__->SetupObject(NULL); __p13__->Release();
	}

private:

	O1  *__p1__;
	O2  *__p2__;
	O3  *__p3__;
	O4  *__p4__;
	O5  *__p5__;
	O6  *__p6__;
	O7  *__p7__;
	O8  *__p8__;
	O9  *__p9__;
	O10 *__p10__;
	O11 *__p11__;
	O12 *__p12__;
	O13 *__p13__;

	static inline void __stg__(OBJECT_INTERFACE *p) { };
	static inline void __stg__(O1 *p) { };
	static inline void __stg__(O2 *p) { };
	static inline void __stg__(O3 *p) { };
	static inline void __stg__(O4 *p) { };
	static inline void __stg__(O5 *p) { };
	static inline void __stg__(O6 *p) { };
	static inline void __stg__(O7 *p) { };
	static inline void __stg__(O8 *p) { };
	static inline void __stg__(O9 *p) { };
	static inline void __stg__(O10 *p) { };
	static inline void __stg__(O11 *p) { };
	static inline void __stg__(O12 *p) { };
	static inline void __stg__(O13 *p) { };

};

template
<
	typename OBJECT_CLASS, typename OBJECT_INTERFACE,
	typename O1 , typename O2 , typename O3 , typename O4 , typename O5 , typename O6 , typename O7 , typename O8 , 
	typename O9 , typename O10, typename O11, typename O12 
>
class Object<OBJECT_CLASS, OBJECT_INTERFACE, O1, O2, O3, O4, O5, O6, O7, O8, O9, O10, O11, O12> : 
	public OBJECT_INTERFACE
{
public:

	static OBJECT_CLASS *Create(IObject *pParent = NULL)
	{
		OBJECT_CLASS *pObject;

		if (NULL == (pObject = new (std::nothrow) OBJECT_CLASS()))
		{
			return NULL;
		}
		pObject->InitObject(pParent);

		return pObject;
	}

	virtual bool Implements(const char *szName) const
	{
		     if (0 == strcmp(szName, GetObjectName())) return True;
		else if (0 == strcmp(szName, __p1__->GetObjectName())) return True;
		else if (0 == strcmp(szName, __p2__->GetObjectName())) return True;
		else if (0 == strcmp(szName, __p3__->GetObjectName())) return True;
		else if (0 == strcmp(szName, __p4__->GetObjectName())) return True;
		else if (0 == strcmp(szName, __p5__->GetObjectName())) return True;
		else if (0 == strcmp(szName, __p6__->GetObjectName())) return True;
		else if (0 == strcmp(szName, __p7__->GetObjectName())) return True;
		else if (0 == strcmp(szName, __p8__->GetObjectName())) return True;
		else if (0 == strcmp(szName, __p9__->GetObjectName())) return True;
		else if (0 == strcmp(szName, __p10__->GetObjectName())) return True;
		else if (0 == strcmp(szName, __p11__->GetObjectName())) return True;
		else if (0 == strcmp(szName, __p12__->GetObjectName())) return True;
		else return False;
	}

	virtual IObject *Acquire(const char *szName)
	{
		     if (0 == strcmp(szName, GetObjectName())) { Retain(); return this; }
		else if (0 == strcmp(szName, __p1__->GetObjectName())) { Retain(); return __p1__; }
		else if (0 == strcmp(szName, __p2__->GetObjectName())) { Retain(); return __p2__; }
		else if (0 == strcmp(szName, __p3__->GetObjectName())) { Retain(); return __p3__; }
		else if (0 == strcmp(szName, __p4__->GetObjectName())) { Retain(); return __p4__; }
		else if (0 == strcmp(szName, __p5__->GetObjectName())) { Retain(); return __p5__; }
		else if (0 == strcmp(szName, __p6__->GetObjectName())) { Retain(); return __p6__; }
		else if (0 == strcmp(szName, __p7__->GetObjectName())) { Retain(); return __p7__; }
		else if (0 == strcmp(szName, __p8__->GetObjectName())) { Retain(); return __p8__; }
		else if (0 == strcmp(szName, __p9__->GetObjectName())) { Retain(); return __p9__; }
		else if (0 == strcmp(szName, __p10__->GetObjectName())) { Retain(); return __p10__; }
		else if (0 == strcmp(szName, __p11__->GetObjectName())) { Retain(); return __p11__; }
		else if (0 == strcmp(szName, __p12__->GetObjectName())) { Retain(); return __p12__; }
		else return NULL;
	}

	virtual const IObject *Acquire(const char *szName) const
	{
		     if (0 == strcmp(szName, GetObjectName())) { Retain(); return this; }
		else if (0 == strcmp(szName, __p1__->GetObjectName())) { Retain(); return __p1__; }
		else if (0 == strcmp(szName, __p2__->GetObjectName())) { Retain(); return __p2__; }
		else if (0 == strcmp(szName, __p3__->GetObjectName())) { Retain(); return __p3__; }
		else if (0 == strcmp(szName, __p4__->GetObjectName())) { Retain(); return __p4__; }
		else if (0 == strcmp(szName, __p5__->GetObjectName())) { Retain(); return __p5__; }
		else if (0 == strcmp(szName, __p6__->GetObjectName())) { Retain(); return __p6__; }
		else if (0 == strcmp(szName, __p7__->GetObjectName())) { Retain(); return __p7__; }
		else if (0 == strcmp(szName, __p8__->GetObjectName())) { Retain(); return __p8__; }
		else if (0 == strcmp(szName, __p9__->GetObjectName())) { Retain(); return __p9__; }
		else if (0 == strcmp(szName, __p10__->GetObjectName())) { Retain(); return __p10__; }
		else if (0 == strcmp(szName, __p11__->GetObjectName())) { Retain(); return __p11__; }
		else if (0 == strcmp(szName, __p12__->GetObjectName())) { Retain(); return __p12__; }
		else return NULL;
	}

protected:

	void InitObject(IObject *pParent = NULL)
	{
		SetupObject(pParent);

		__p1__  = O1::Create(this);
		__p2__  = O2::Create(this);
		__p3__  = O3::Create(this);
		__p4__  = O4::Create(this);
		__p5__  = O5::Create(this);
		__p6__  = O6::Create(this);
		__p7__  = O7::Create(this);
		__p8__  = O8::Create(this);
		__p9__  = O9::Create(this);
		__p10__ = O10::Create(this);
		__p11__ = O11::Create(this);
		__p12__ = O12::Create(this);

		OnInitObject();
	}

	virtual void UninitObject() const
	{
		delete this;
	}

	~Object()
	{
		OnUninitObject();

		__p1__->SetupObject(NULL); __p1__->Release();
		__p2__->SetupObject(NULL); __p2__->Release();
		__p3__->SetupObject(NULL); __p3__->Release();
		__p4__->SetupObject(NULL); __p4__->Release();
		__p5__->SetupObject(NULL); __p5__->Release();
		__p6__->SetupObject(NULL); __p6__->Release();
		__p7__->SetupObject(NULL); __p7__->Release();
		__p8__->SetupObject(NULL); __p8__->Release();
		__p9__->SetupObject(NULL); __p9__->Release();
		__p10__->SetupObject(NULL); __p10__->Release();
		__p11__->SetupObject(NULL); __p11__->Release();
		__p12__->SetupObject(NULL); __p12__->Release();
	}

private:

	O1  *__p1__;
	O2  *__p2__;
	O3  *__p3__;
	O4  *__p4__;
	O5  *__p5__;
	O6  *__p6__;
	O7  *__p7__;
	O8  *__p8__;
	O9  *__p9__;
	O10 *__p10__;
	O11 *__p11__;
	O12 *__p12__;

	static inline void __stg__(OBJECT_INTERFACE *p) { };
	static inline void __stg__(O1 *p) { };
	static inline void __stg__(O2 *p) { };
	static inline void __stg__(O3 *p) { };
	static inline void __stg__(O4 *p) { };
	static inline void __stg__(O5 *p) { };
	static inline void __stg__(O6 *p) { };
	static inline void __stg__(O7 *p) { };
	static inline void __stg__(O8 *p) { };
	static inline void __stg__(O9 *p) { };
	static inline void __stg__(O10 *p) { };
	static inline void __stg__(O11 *p) { };
	static inline void __stg__(O12 *p) { };

};

template
<
	typename OBJECT_CLASS, typename OBJECT_INTERFACE,
	typename O1 , typename O2 , typename O3 , typename O4 , typename O5 , typename O6 , typename O7 , typename O8 , 
	typename O9 , typename O10, typename O11 
>
class Object<OBJECT_CLASS, OBJECT_INTERFACE, O1, O2, O3, O4, O5, O6, O7, O8, O9, O10, O11> : 
	public OBJECT_INTERFACE
{
public:

	static OBJECT_CLASS *Create(IObject *pParent = NULL)
	{
		OBJECT_CLASS *pObject;

		if (NULL == (pObject = new (std::nothrow) OBJECT_CLASS()))
		{
			return NULL;
		}
		pObject->InitObject(pParent);

		return pObject;
	}

	virtual bool Implements(const char *szName) const
	{
		     if (0 == strcmp(szName, GetObjectName())) return True;
		else if (0 == strcmp(szName, __p1__->GetObjectName())) return True;
		else if (0 == strcmp(szName, __p2__->GetObjectName())) return True;
		else if (0 == strcmp(szName, __p3__->GetObjectName())) return True;
		else if (0 == strcmp(szName, __p4__->GetObjectName())) return True;
		else if (0 == strcmp(szName, __p5__->GetObjectName())) return True;
		else if (0 == strcmp(szName, __p6__->GetObjectName())) return True;
		else if (0 == strcmp(szName, __p7__->GetObjectName())) return True;
		else if (0 == strcmp(szName, __p8__->GetObjectName())) return True;
		else if (0 == strcmp(szName, __p9__->GetObjectName())) return True;
		else if (0 == strcmp(szName, __p10__->GetObjectName())) return True;
		else if (0 == strcmp(szName, __p11__->GetObjectName())) return True;
		else return False;
	}

	virtual IObject *Acquire(const char *szName)
	{
		     if (0 == strcmp(szName, GetObjectName())) { Retain(); return this; }
		else if (0 == strcmp(szName, __p1__->GetObjectName())) { Retain(); return __p1__; }
		else if (0 == strcmp(szName, __p2__->GetObjectName())) { Retain(); return __p2__; }
		else if (0 == strcmp(szName, __p3__->GetObjectName())) { Retain(); return __p3__; }
		else if (0 == strcmp(szName, __p4__->GetObjectName())) { Retain(); return __p4__; }
		else if (0 == strcmp(szName, __p5__->GetObjectName())) { Retain(); return __p5__; }
		else if (0 == strcmp(szName, __p6__->GetObjectName())) { Retain(); return __p6__; }
		else if (0 == strcmp(szName, __p7__->GetObjectName())) { Retain(); return __p7__; }
		else if (0 == strcmp(szName, __p8__->GetObjectName())) { Retain(); return __p8__; }
		else if (0 == strcmp(szName, __p9__->GetObjectName())) { Retain(); return __p9__; }
		else if (0 == strcmp(szName, __p10__->GetObjectName())) { Retain(); return __p10__; }
		else if (0 == strcmp(szName, __p11__->GetObjectName())) { Retain(); return __p11__; }
		else return NULL;
	}

	virtual const IObject *Acquire(const char *szName) const
	{
		     if (0 == strcmp(szName, GetObjectName())) { Retain(); return this; }
		else if (0 == strcmp(szName, __p1__->GetObjectName())) { Retain(); return __p1__; }
		else if (0 == strcmp(szName, __p2__->GetObjectName())) { Retain(); return __p2__; }
		else if (0 == strcmp(szName, __p3__->GetObjectName())) { Retain(); return __p3__; }
		else if (0 == strcmp(szName, __p4__->GetObjectName())) { Retain(); return __p4__; }
		else if (0 == strcmp(szName, __p5__->GetObjectName())) { Retain(); return __p5__; }
		else if (0 == strcmp(szName, __p6__->GetObjectName())) { Retain(); return __p6__; }
		else if (0 == strcmp(szName, __p7__->GetObjectName())) { Retain(); return __p7__; }
		else if (0 == strcmp(szName, __p8__->GetObjectName())) { Retain(); return __p8__; }
		else if (0 == strcmp(szName, __p9__->GetObjectName())) { Retain(); return __p9__; }
		else if (0 == strcmp(szName, __p10__->GetObjectName())) { Retain(); return __p10__; }
		else if (0 == strcmp(szName, __p11__->GetObjectName())) { Retain(); return __p11__; }
		else return NULL;
	}

protected:

	void InitObject(IObject *pParent = NULL)
	{
		SetupObject(pParent);

		__p1__  = O1::Create(this);
		__p2__  = O2::Create(this);
		__p3__  = O3::Create(this);
		__p4__  = O4::Create(this);
		__p5__  = O5::Create(this);
		__p6__  = O6::Create(this);
		__p7__  = O7::Create(this);
		__p8__  = O8::Create(this);
		__p9__  = O9::Create(this);
		__p10__ = O10::Create(this);
		__p11__ = O11::Create(this);

		OnInitObject();
	}

	virtual void UninitObject() const
	{
		delete this;
	}

	~Object()
	{
		OnUninitObject();

		__p1__->SetupObject(NULL); __p1__->Release();
		__p2__->SetupObject(NULL); __p2__->Release();
		__p3__->SetupObject(NULL); __p3__->Release();
		__p4__->SetupObject(NULL); __p4__->Release();
		__p5__->SetupObject(NULL); __p5__->Release();
		__p6__->SetupObject(NULL); __p6__->Release();
		__p7__->SetupObject(NULL); __p7__->Release();
		__p8__->SetupObject(NULL); __p8__->Release();
		__p9__->SetupObject(NULL); __p9__->Release();
		__p10__->SetupObject(NULL); __p10__->Release();
		__p11__->SetupObject(NULL); __p11__->Release();
	}

private:

	O1  *__p1__;
	O2  *__p2__;
	O3  *__p3__;
	O4  *__p4__;
	O5  *__p5__;
	O6  *__p6__;
	O7  *__p7__;
	O8  *__p8__;
	O9  *__p9__;
	O10 *__p10__;
	O11 *__p11__;

	static inline void __stg__(OBJECT_INTERFACE *p) { };
	static inline void __stg__(O1 *p) { };
	static inline void __stg__(O2 *p) { };
	static inline void __stg__(O3 *p) { };
	static inline void __stg__(O4 *p) { };
	static inline void __stg__(O5 *p) { };
	static inline void __stg__(O6 *p) { };
	static inline void __stg__(O7 *p) { };
	static inline void __stg__(O8 *p) { };
	static inline void __stg__(O9 *p) { };
	static inline void __stg__(O10 *p) { };
	static inline void __stg__(O11 *p) { };

};

template
<
	typename OBJECT_CLASS, typename OBJECT_INTERFACE,
	typename O1 , typename O2 , typename O3 , typename O4 , typename O5 , typename O6 , typename O7 , typename O8 , 
	typename O9 , typename O10 
>
class Object<OBJECT_CLASS, OBJECT_INTERFACE, O1, O2, O3, O4, O5, O6, O7, O8, O9, O10> : 
	public OBJECT_INTERFACE
{
public:

	static OBJECT_CLASS *Create(IObject *pParent = NULL)
	{
		OBJECT_CLASS *pObject;

		if (NULL == (pObject = new (std::nothrow) OBJECT_CLASS()))
		{
			return NULL;
		}
		pObject->InitObject(pParent);

		return pObject;
	}

	virtual bool Implements(const char *szName) const
	{
		     if (0 == strcmp(szName, GetObjectName())) return True;
		else if (0 == strcmp(szName, __p1__->GetObjectName())) return True;
		else if (0 == strcmp(szName, __p2__->GetObjectName())) return True;
		else if (0 == strcmp(szName, __p3__->GetObjectName())) return True;
		else if (0 == strcmp(szName, __p4__->GetObjectName())) return True;
		else if (0 == strcmp(szName, __p5__->GetObjectName())) return True;
		else if (0 == strcmp(szName, __p6__->GetObjectName())) return True;
		else if (0 == strcmp(szName, __p7__->GetObjectName())) return True;
		else if (0 == strcmp(szName, __p8__->GetObjectName())) return True;
		else if (0 == strcmp(szName, __p9__->GetObjectName())) return True;
		else if (0 == strcmp(szName, __p10__->GetObjectName())) return True;
		else return False;
	}

	virtual IObject *Acquire(const char *szName)
	{
		     if (0 == strcmp(szName, GetObjectName())) { Retain(); return this; }
		else if (0 == strcmp(szName, __p1__->GetObjectName())) { Retain(); return __p1__; }
		else if (0 == strcmp(szName, __p2__->GetObjectName())) { Retain(); return __p2__; }
		else if (0 == strcmp(szName, __p3__->GetObjectName())) { Retain(); return __p3__; }
		else if (0 == strcmp(szName, __p4__->GetObjectName())) { Retain(); return __p4__; }
		else if (0 == strcmp(szName, __p5__->GetObjectName())) { Retain(); return __p5__; }
		else if (0 == strcmp(szName, __p6__->GetObjectName())) { Retain(); return __p6__; }
		else if (0 == strcmp(szName, __p7__->GetObjectName())) { Retain(); return __p7__; }
		else if (0 == strcmp(szName, __p8__->GetObjectName())) { Retain(); return __p8__; }
		else if (0 == strcmp(szName, __p9__->GetObjectName())) { Retain(); return __p9__; }
		else if (0 == strcmp(szName, __p10__->GetObjectName())) { Retain(); return __p10__; }
		else return NULL;
	}

	virtual const IObject *Acquire(const char *szName) const
	{
		     if (0 == strcmp(szName, GetObjectName())) { Retain(); return this; }
		else if (0 == strcmp(szName, __p1__->GetObjectName())) { Retain(); return __p1__; }
		else if (0 == strcmp(szName, __p2__->GetObjectName())) { Retain(); return __p2__; }
		else if (0 == strcmp(szName, __p3__->GetObjectName())) { Retain(); return __p3__; }
		else if (0 == strcmp(szName, __p4__->GetObjectName())) { Retain(); return __p4__; }
		else if (0 == strcmp(szName, __p5__->GetObjectName())) { Retain(); return __p5__; }
		else if (0 == strcmp(szName, __p6__->GetObjectName())) { Retain(); return __p6__; }
		else if (0 == strcmp(szName, __p7__->GetObjectName())) { Retain(); return __p7__; }
		else if (0 == strcmp(szName, __p8__->GetObjectName())) { Retain(); return __p8__; }
		else if (0 == strcmp(szName, __p9__->GetObjectName())) { Retain(); return __p9__; }
		else if (0 == strcmp(szName, __p10__->GetObjectName())) { Retain(); return __p10__; }
		else return NULL;
	}

protected:

	void InitObject(IObject *pParent = NULL)
	{
		SetupObject(pParent);

		__p1__  = O1::Create(this);
		__p2__  = O2::Create(this);
		__p3__  = O3::Create(this);
		__p4__  = O4::Create(this);
		__p5__  = O5::Create(this);
		__p6__  = O6::Create(this);
		__p7__  = O7::Create(this);
		__p8__  = O8::Create(this);
		__p9__  = O9::Create(this);
		__p10__ = O10::Create(this);

		OnInitObject();
	}

	virtual void UninitObject() const
	{
		delete this;
	}

	~Object()
	{
		OnUninitObject();

		__p1__->SetupObject(NULL); __p1__->Release();
		__p2__->SetupObject(NULL); __p2__->Release();
		__p3__->SetupObject(NULL); __p3__->Release();
		__p4__->SetupObject(NULL); __p4__->Release();
		__p5__->SetupObject(NULL); __p5__->Release();
		__p6__->SetupObject(NULL); __p6__->Release();
		__p7__->SetupObject(NULL); __p7__->Release();
		__p8__->SetupObject(NULL); __p8__->Release();
		__p9__->SetupObject(NULL); __p9__->Release();
		__p10__->SetupObject(NULL); __p10__->Release();
	}

private:

	O1  *__p1__;
	O2  *__p2__;
	O3  *__p3__;
	O4  *__p4__;
	O5  *__p5__;
	O6  *__p6__;
	O7  *__p7__;
	O8  *__p8__;
	O9  *__p9__;
	O10 *__p10__;

	static inline void __stg__(OBJECT_INTERFACE *p) { };
	static inline void __stg__(O1 *p) { };
	static inline void __stg__(O2 *p) { };
	static inline void __stg__(O3 *p) { };
	static inline void __stg__(O4 *p) { };
	static inline void __stg__(O5 *p) { };
	static inline void __stg__(O6 *p) { };
	static inline void __stg__(O7 *p) { };
	static inline void __stg__(O8 *p) { };
	static inline void __stg__(O9 *p) { };
	static inline void __stg__(O10 *p) { };

};

template
<
	typename OBJECT_CLASS, typename OBJECT_INTERFACE,
	typename O1 , typename O2 , typename O3 , typename O4 , typename O5 , typename O6 , typename O7 , typename O8 , 
	typename O9 
>
class Object<OBJECT_CLASS, OBJECT_INTERFACE, O1, O2, O3, O4, O5, O6, O7, O8, O9> : 
	public OBJECT_INTERFACE
{
public:

	static OBJECT_CLASS *Create(IObject *pParent = NULL)
	{
		OBJECT_CLASS *pObject;

		if (NULL == (pObject = new (std::nothrow) OBJECT_CLASS()))
		{
			return NULL;
		}
		pObject->InitObject(pParent);

		return pObject;
	}

	virtual bool Implements(const char *szName) const
	{
		     if (0 == strcmp(szName, GetObjectName())) { Retain(); return this; }
		else if (0 == strcmp(szName, __p1__->GetObjectName())) return True;
		else if (0 == strcmp(szName, __p2__->GetObjectName())) return True;
		else if (0 == strcmp(szName, __p3__->GetObjectName())) return True;
		else if (0 == strcmp(szName, __p4__->GetObjectName())) return True;
		else if (0 == strcmp(szName, __p5__->GetObjectName())) return True;
		else if (0 == strcmp(szName, __p6__->GetObjectName())) return True;
		else if (0 == strcmp(szName, __p7__->GetObjectName())) return True;
		else if (0 == strcmp(szName, __p8__->GetObjectName())) return True;
		else if (0 == strcmp(szName, __p9__->GetObjectName())) return True;
		else return False;
	}

	virtual IObject *Acquire(const char *szName)
	{
		     if (0 == strcmp(szName, GetObjectName())) { Retain(); return this; }
		else if (0 == strcmp(szName, __p1__->GetObjectName())) { Retain(); return __p1__; }
		else if (0 == strcmp(szName, __p2__->GetObjectName())) { Retain(); return __p2__; }
		else if (0 == strcmp(szName, __p3__->GetObjectName())) { Retain(); return __p3__; }
		else if (0 == strcmp(szName, __p4__->GetObjectName())) { Retain(); return __p4__; }
		else if (0 == strcmp(szName, __p5__->GetObjectName())) { Retain(); return __p5__; }
		else if (0 == strcmp(szName, __p6__->GetObjectName())) { Retain(); return __p6__; }
		else if (0 == strcmp(szName, __p7__->GetObjectName())) { Retain(); return __p7__; }
		else if (0 == strcmp(szName, __p8__->GetObjectName())) { Retain(); return __p8__; }
		else if (0 == strcmp(szName, __p9__->GetObjectName())) { Retain(); return __p9__; }
		else return NULL;
	}

	virtual const IObject *Acquire(const char *szName) const
	{
		     if (0 == strcmp(szName, GetObjectName())) { Retain(); return this; }
		else if (0 == strcmp(szName, __p1__->GetObjectName())) { Retain(); return __p1__; }
		else if (0 == strcmp(szName, __p2__->GetObjectName())) { Retain(); return __p2__; }
		else if (0 == strcmp(szName, __p3__->GetObjectName())) { Retain(); return __p3__; }
		else if (0 == strcmp(szName, __p4__->GetObjectName())) { Retain(); return __p4__; }
		else if (0 == strcmp(szName, __p5__->GetObjectName())) { Retain(); return __p5__; }
		else if (0 == strcmp(szName, __p6__->GetObjectName())) { Retain(); return __p6__; }
		else if (0 == strcmp(szName, __p7__->GetObjectName())) { Retain(); return __p7__; }
		else if (0 == strcmp(szName, __p8__->GetObjectName())) { Retain(); return __p8__; }
		else if (0 == strcmp(szName, __p9__->GetObjectName())) { Retain(); return __p9__; }
		else return NULL;
	}

protected:

	void InitObject(IObject *pParent = NULL)
	{
		SetupObject(pParent);

		__p1__  = O1::Create(this);
		__p2__  = O2::Create(this);
		__p3__  = O3::Create(this);
		__p4__  = O4::Create(this);
		__p5__  = O5::Create(this);
		__p6__  = O6::Create(this);
		__p7__  = O7::Create(this);
		__p8__  = O8::Create(this);
		__p9__  = O9::Create(this);

		OnInitObject();
	}

	virtual void UninitObject() const
	{
		delete this;
	}

	~Object()
	{
		OnUninitObject();

		__p1__->SetupObject(NULL); __p1__->Release();
		__p2__->SetupObject(NULL); __p2__->Release();
		__p3__->SetupObject(NULL); __p3__->Release();
		__p4__->SetupObject(NULL); __p4__->Release();
		__p5__->SetupObject(NULL); __p5__->Release();
		__p6__->SetupObject(NULL); __p6__->Release();
		__p7__->SetupObject(NULL); __p7__->Release();
		__p8__->SetupObject(NULL); __p8__->Release();
		__p9__->SetupObject(NULL); __p9__->Release();
	}

private:

	O1  *__p1__;
	O2  *__p2__;
	O3  *__p3__;
	O4  *__p4__;
	O5  *__p5__;
	O6  *__p6__;
	O7  *__p7__;
	O8  *__p8__;
	O9  *__p9__;

	static inline void __stg__(OBJECT_INTERFACE *p) { };
	static inline void __stg__(O1 *p) { };
	static inline void __stg__(O2 *p) { };
	static inline void __stg__(O3 *p) { };
	static inline void __stg__(O4 *p) { };
	static inline void __stg__(O5 *p) { };
	static inline void __stg__(O6 *p) { };
	static inline void __stg__(O7 *p) { };
	static inline void __stg__(O8 *p) { };
	static inline void __stg__(O9 *p) { };

};

template
<
	typename OBJECT_CLASS, typename OBJECT_INTERFACE,
	typename O1 , typename O2 , typename O3 , typename O4 , typename O5 , typename O6 , typename O7 , typename O8 
>
class Object<OBJECT_CLASS, OBJECT_INTERFACE, O1, O2, O3, O4, O5, O6, O7, O8> : 
	public OBJECT_INTERFACE
{
public:

	static OBJECT_CLASS *Create(IObject *pParent = NULL)
	{
		OBJECT_CLASS *pObject;

		if (NULL == (pObject = new (std::nothrow) OBJECT_CLASS()))
		{
			return NULL;
		}
		pObject->InitObject(pParent);

		return pObject;
	}

	virtual bool Implements(const char *szName) const
	{
		     if (0 == strcmp(szName, GetObjectName())) return True;
		else if (0 == strcmp(szName, __p1__->GetObjectName())) return True;
		else if (0 == strcmp(szName, __p2__->GetObjectName())) return True;
		else if (0 == strcmp(szName, __p3__->GetObjectName())) return True;
		else if (0 == strcmp(szName, __p4__->GetObjectName())) return True;
		else if (0 == strcmp(szName, __p5__->GetObjectName())) return True;
		else if (0 == strcmp(szName, __p6__->GetObjectName())) return True;
		else if (0 == strcmp(szName, __p7__->GetObjectName())) return True;
		else if (0 == strcmp(szName, __p8__->GetObjectName())) return True;
		else return False;
	}

	virtual IObject *Acquire(const char *szName)
	{
		     if (0 == strcmp(szName, GetObjectName())) { Retain(); return this; }
		else if (0 == strcmp(szName, __p1__->GetObjectName())) { Retain(); return __p1__; }
		else if (0 == strcmp(szName, __p2__->GetObjectName())) { Retain(); return __p2__; }
		else if (0 == strcmp(szName, __p3__->GetObjectName())) { Retain(); return __p3__; }
		else if (0 == strcmp(szName, __p4__->GetObjectName())) { Retain(); return __p4__; }
		else if (0 == strcmp(szName, __p5__->GetObjectName())) { Retain(); return __p5__; }
		else if (0 == strcmp(szName, __p6__->GetObjectName())) { Retain(); return __p6__; }
		else if (0 == strcmp(szName, __p7__->GetObjectName())) { Retain(); return __p7__; }
		else if (0 == strcmp(szName, __p8__->GetObjectName())) { Retain(); return __p8__; }
		else return NULL;
	}

	virtual const IObject *Acquire(const char *szName) const
	{
		     if (0 == strcmp(szName, GetObjectName())) { Retain(); return this; }
		else if (0 == strcmp(szName, __p1__->GetObjectName())) { Retain(); return __p1__; }
		else if (0 == strcmp(szName, __p2__->GetObjectName())) { Retain(); return __p2__; }
		else if (0 == strcmp(szName, __p3__->GetObjectName())) { Retain(); return __p3__; }
		else if (0 == strcmp(szName, __p4__->GetObjectName())) { Retain(); return __p4__; }
		else if (0 == strcmp(szName, __p5__->GetObjectName())) { Retain(); return __p5__; }
		else if (0 == strcmp(szName, __p6__->GetObjectName())) { Retain(); return __p6__; }
		else if (0 == strcmp(szName, __p7__->GetObjectName())) { Retain(); return __p7__; }
		else if (0 == strcmp(szName, __p8__->GetObjectName())) { Retain(); return __p8__; }
		else return NULL;
	}

protected:

	void InitObject(IObject *pParent = NULL)
	{
		SetupObject(pParent);

		__p1__  = O1::Create(this);
		__p2__  = O2::Create(this);
		__p3__  = O3::Create(this);
		__p4__  = O4::Create(this);
		__p5__  = O5::Create(this);
		__p6__  = O6::Create(this);
		__p7__  = O7::Create(this);
		__p8__  = O8::Create(this);

		OnInitObject();
	}

	virtual void UninitObject() const
	{
		delete this;
	}

	~Object()
	{
		OnUninitObject();

		__p1__->SetupObject(NULL); __p1__->Release();
		__p2__->SetupObject(NULL); __p2__->Release();
		__p3__->SetupObject(NULL); __p3__->Release();
		__p4__->SetupObject(NULL); __p4__->Release();
		__p5__->SetupObject(NULL); __p5__->Release();
		__p6__->SetupObject(NULL); __p6__->Release();
		__p7__->SetupObject(NULL); __p7__->Release();
		__p8__->SetupObject(NULL); __p8__->Release();
	}

private:

	O1  *__p1__;
	O2  *__p2__;
	O3  *__p3__;
	O4  *__p4__;
	O5  *__p5__;
	O6  *__p6__;
	O7  *__p7__;
	O8  *__p8__;

	static inline void __stg__(OBJECT_INTERFACE *p) { };
	static inline void __stg__(O1 *p) { };
	static inline void __stg__(O2 *p) { };
	static inline void __stg__(O3 *p) { };
	static inline void __stg__(O4 *p) { };
	static inline void __stg__(O5 *p) { };
	static inline void __stg__(O6 *p) { };
	static inline void __stg__(O7 *p) { };
	static inline void __stg__(O8 *p) { };

};

template
<
	typename OBJECT_CLASS, typename OBJECT_INTERFACE,
	typename O1 , typename O2 , typename O3 , typename O4 , typename O5 , typename O6 , typename O7 
>
class Object<OBJECT_CLASS, OBJECT_INTERFACE, O1, O2, O3, O4, O5, O6, O7> : 
	public OBJECT_INTERFACE
{
public:

	static OBJECT_CLASS *Create(IObject *pParent = NULL)
	{
		OBJECT_CLASS *pObject;

		if (NULL == (pObject = new (std::nothrow) OBJECT_CLASS()))
		{
			return NULL;
		}
		pObject->InitObject(pParent);

		return pObject;
	}

	virtual bool Implements(const char *szName) const
	{
		     if (0 == strcmp(szName, GetObjectName())) return True;
		else if (0 == strcmp(szName, __p1__->GetObjectName())) return True;
		else if (0 == strcmp(szName, __p2__->GetObjectName())) return True;
		else if (0 == strcmp(szName, __p3__->GetObjectName())) return True;
		else if (0 == strcmp(szName, __p4__->GetObjectName())) return True;
		else if (0 == strcmp(szName, __p5__->GetObjectName())) return True;
		else if (0 == strcmp(szName, __p6__->GetObjectName())) return True;
		else if (0 == strcmp(szName, __p7__->GetObjectName())) return True;
		else return False;
	}

	virtual IObject *Acquire(const char *szName)
	{
		     if (0 == strcmp(szName, GetObjectName())) { Retain(); return this; }
		else if (0 == strcmp(szName, __p1__->GetObjectName())) { Retain(); return __p1__; }
		else if (0 == strcmp(szName, __p2__->GetObjectName())) { Retain(); return __p2__; }
		else if (0 == strcmp(szName, __p3__->GetObjectName())) { Retain(); return __p3__; }
		else if (0 == strcmp(szName, __p4__->GetObjectName())) { Retain(); return __p4__; }
		else if (0 == strcmp(szName, __p5__->GetObjectName())) { Retain(); return __p5__; }
		else if (0 == strcmp(szName, __p6__->GetObjectName())) { Retain(); return __p6__; }
		else if (0 == strcmp(szName, __p7__->GetObjectName())) { Retain(); return __p7__; }
		else return NULL;
	}

	virtual const IObject *Acquire(const char *szName) const
	{
		     if (0 == strcmp(szName, GetObjectName())) { Retain(); return this; }
		else if (0 == strcmp(szName, __p1__->GetObjectName())) { Retain(); return __p1__; }
		else if (0 == strcmp(szName, __p2__->GetObjectName())) { Retain(); return __p2__; }
		else if (0 == strcmp(szName, __p3__->GetObjectName())) { Retain(); return __p3__; }
		else if (0 == strcmp(szName, __p4__->GetObjectName())) { Retain(); return __p4__; }
		else if (0 == strcmp(szName, __p5__->GetObjectName())) { Retain(); return __p5__; }
		else if (0 == strcmp(szName, __p6__->GetObjectName())) { Retain(); return __p6__; }
		else if (0 == strcmp(szName, __p7__->GetObjectName())) { Retain(); return __p7__; }
		else return NULL;
	}

protected:

	void InitObject(IObject *pParent = NULL)
	{
		SetupObject(pParent);

		__p1__  = O1::Create(this);
		__p2__  = O2::Create(this);
		__p3__  = O3::Create(this);
		__p4__  = O4::Create(this);
		__p5__  = O5::Create(this);
		__p6__  = O6::Create(this);
		__p7__  = O7::Create(this);

		OnInitObject();
	}

	virtual void UninitObject() const
	{
		delete this;
	}

	~Object()
	{
		OnUninitObject();

		__p1__->SetupObject(NULL); __p1__->Release();
		__p2__->SetupObject(NULL); __p2__->Release();
		__p3__->SetupObject(NULL); __p3__->Release();
		__p4__->SetupObject(NULL); __p4__->Release();
		__p5__->SetupObject(NULL); __p5__->Release();
		__p6__->SetupObject(NULL); __p6__->Release();
		__p7__->SetupObject(NULL); __p7__->Release();
	}

private:

	O1  *__p1__;
	O2  *__p2__;
	O3  *__p3__;
	O4  *__p4__;
	O5  *__p5__;
	O6  *__p6__;
	O7  *__p7__;

	static inline void __stg__(OBJECT_INTERFACE *p) { };
	static inline void __stg__(O1 *p) { };
	static inline void __stg__(O2 *p) { };
	static inline void __stg__(O3 *p) { };
	static inline void __stg__(O4 *p) { };
	static inline void __stg__(O5 *p) { };
	static inline void __stg__(O6 *p) { };
	static inline void __stg__(O7 *p) { };

};

template
<
	typename OBJECT_CLASS, typename OBJECT_INTERFACE,
	typename O1 , typename O2 , typename O3 , typename O4 , typename O5 , typename O6 
>
class Object<OBJECT_CLASS, OBJECT_INTERFACE, O1, O2, O3, O4, O5, O6> : 
	public OBJECT_INTERFACE
{
public:

	static OBJECT_CLASS *Create(IObject *pParent = NULL)
	{
		OBJECT_CLASS *pObject;

		if (NULL == (pObject = new (std::nothrow) OBJECT_CLASS()))
		{
			return NULL;
		}
		pObject->InitObject(pParent);

		return pObject;
	}

	virtual bool Implements(const char *szName) const
	{
		     if (0 == strcmp(szName, GetObjectName())) return True;
		else if (0 == strcmp(szName, __p1__->GetObjectName())) return True;
		else if (0 == strcmp(szName, __p2__->GetObjectName())) return True;
		else if (0 == strcmp(szName, __p3__->GetObjectName())) return True;
		else if (0 == strcmp(szName, __p4__->GetObjectName())) return True;
		else if (0 == strcmp(szName, __p5__->GetObjectName())) return True;
		else if (0 == strcmp(szName, __p6__->GetObjectName())) return True;
		else return False;
	}

	virtual IObject *Acquire(const char *szName)
	{
		     if (0 == strcmp(szName, GetObjectName())) { Retain(); return this; }
		else if (0 == strcmp(szName, __p1__->GetObjectName())) { Retain(); return __p1__; }
		else if (0 == strcmp(szName, __p2__->GetObjectName())) { Retain(); return __p2__; }
		else if (0 == strcmp(szName, __p3__->GetObjectName())) { Retain(); return __p3__; }
		else if (0 == strcmp(szName, __p4__->GetObjectName())) { Retain(); return __p4__; }
		else if (0 == strcmp(szName, __p5__->GetObjectName())) { Retain(); return __p5__; }
		else if (0 == strcmp(szName, __p6__->GetObjectName())) { Retain(); return __p6__; }
		else return NULL;
	}

	virtual const IObject *Acquire(const char *szName) const
	{
		     if (0 == strcmp(szName, GetObjectName())) { Retain(); return this; }
		else if (0 == strcmp(szName, __p1__->GetObjectName())) { Retain(); return __p1__; }
		else if (0 == strcmp(szName, __p2__->GetObjectName())) { Retain(); return __p2__; }
		else if (0 == strcmp(szName, __p3__->GetObjectName())) { Retain(); return __p3__; }
		else if (0 == strcmp(szName, __p4__->GetObjectName())) { Retain(); return __p4__; }
		else if (0 == strcmp(szName, __p5__->GetObjectName())) { Retain(); return __p5__; }
		else if (0 == strcmp(szName, __p6__->GetObjectName())) { Retain(); return __p6__; }
		else return NULL;
	}

protected:

	void InitObject(IObject *pParent = NULL)
	{
		SetupObject(pParent);

		__p1__  = O1::Create(this);
		__p2__  = O2::Create(this);
		__p3__  = O3::Create(this);
		__p4__  = O4::Create(this);
		__p5__  = O5::Create(this);
		__p6__  = O6::Create(this);

		OnInitObject();
	}

	virtual void UninitObject() const
	{
		delete this;
	}

	~Object()
	{
		OnUninitObject();

		__p1__->SetupObject(NULL); __p1__->Release();
		__p2__->SetupObject(NULL); __p2__->Release();
		__p3__->SetupObject(NULL); __p3__->Release();
		__p4__->SetupObject(NULL); __p4__->Release();
		__p5__->SetupObject(NULL); __p5__->Release();
		__p6__->SetupObject(NULL); __p6__->Release();
	}

private:

	O1  *__p1__;
	O2  *__p2__;
	O3  *__p3__;
	O4  *__p4__;
	O5  *__p5__;
	O6  *__p6__;

	static inline void __stg__(OBJECT_INTERFACE *p) { };
	static inline void __stg__(O1 *p) { };
	static inline void __stg__(O2 *p) { };
	static inline void __stg__(O3 *p) { };
	static inline void __stg__(O4 *p) { };
	static inline void __stg__(O5 *p) { };
	static inline void __stg__(O6 *p) { };

};

template
<
	typename OBJECT_CLASS, typename OBJECT_INTERFACE,
	typename O1 , typename O2 , typename O3 , typename O4 , typename O5 
>
class Object<OBJECT_CLASS, OBJECT_INTERFACE, O1, O2, O3, O4, O5> : 
	public OBJECT_INTERFACE
{
public:

	static OBJECT_CLASS *Create(IObject *pParent = NULL)
	{
		OBJECT_CLASS *pObject;

		if (NULL == (pObject = new (std::nothrow) OBJECT_CLASS()))
		{
			return NULL;
		}
		pObject->InitObject(pParent);

		return pObject;
	}

	virtual bool Implements(const char *szName) const
	{
		     if (0 == strcmp(szName, GetObjectName())) return True;
		else if (0 == strcmp(szName, __p1__->GetObjectName())) return True;
		else if (0 == strcmp(szName, __p2__->GetObjectName())) return True;
		else if (0 == strcmp(szName, __p3__->GetObjectName())) return True;
		else if (0 == strcmp(szName, __p4__->GetObjectName())) return True;
		else if (0 == strcmp(szName, __p5__->GetObjectName())) return True;
		else return False;
	}

	virtual IObject *Acquire(const char *szName)
	{
		     if (0 == strcmp(szName, GetObjectName())) { Retain(); return this; }
		else if (0 == strcmp(szName, __p1__->GetObjectName())) { Retain(); return __p1__; }
		else if (0 == strcmp(szName, __p2__->GetObjectName())) { Retain(); return __p2__; }
		else if (0 == strcmp(szName, __p3__->GetObjectName())) { Retain(); return __p3__; }
		else if (0 == strcmp(szName, __p4__->GetObjectName())) { Retain(); return __p4__; }
		else if (0 == strcmp(szName, __p5__->GetObjectName())) { Retain(); return __p5__; }
		else return NULL;
	}

	virtual const IObject *Acquire(const char *szName) const
	{
		     if (0 == strcmp(szName, GetObjectName())) { Retain(); return this; }
		else if (0 == strcmp(szName, __p1__->GetObjectName())) { Retain(); return __p1__; }
		else if (0 == strcmp(szName, __p2__->GetObjectName())) { Retain(); return __p2__; }
		else if (0 == strcmp(szName, __p3__->GetObjectName())) { Retain(); return __p3__; }
		else if (0 == strcmp(szName, __p4__->GetObjectName())) { Retain(); return __p4__; }
		else if (0 == strcmp(szName, __p5__->GetObjectName())) { Retain(); return __p5__; }
		else return NULL;
	}

protected:

	void InitObject(IObject *pParent = NULL)
	{
		SetupObject(pParent);

		__p1__  = O1::Create(this);
		__p2__  = O2::Create(this);
		__p3__  = O3::Create(this);
		__p4__  = O4::Create(this);
		__p5__  = O5::Create(this);

		OnInitObject();
	}

	virtual void UninitObject() const
	{
		delete this;
	}

	~Object()
	{
		OnUninitObject();

		__p1__->SetupObject(NULL); __p1__->Release();
		__p2__->SetupObject(NULL); __p2__->Release();
		__p3__->SetupObject(NULL); __p3__->Release();
		__p4__->SetupObject(NULL); __p4__->Release();
		__p5__->SetupObject(NULL); __p5__->Release();
	}

private:

	O1  *__p1__;
	O2  *__p2__;
	O3  *__p3__;
	O4  *__p4__;
	O5  *__p5__;

	static inline void __stg__(OBJECT_INTERFACE *p) { };
	static inline void __stg__(O1 *p) { };
	static inline void __stg__(O2 *p) { };
	static inline void __stg__(O3 *p) { };
	static inline void __stg__(O4 *p) { };
	static inline void __stg__(O5 *p) { };

};

template
<
	typename OBJECT_CLASS, typename OBJECT_INTERFACE,
	typename O1 , typename O2 , typename O3 , typename O4 
>
class Object<OBJECT_CLASS, OBJECT_INTERFACE, O1, O2, O3, O4> : 
	public OBJECT_INTERFACE
{
public:

	static OBJECT_CLASS *Create(IObject *pParent = NULL)
	{
		OBJECT_CLASS *pObject;

		if (NULL == (pObject = new (std::nothrow) OBJECT_CLASS()))
		{
			return NULL;
		}
		pObject->InitObject(pParent);

		return pObject;
	}

	virtual bool Implements(const char *szName) const
	{
		     if (0 == strcmp(szName, GetObjectName())) return True;
		else if (0 == strcmp(szName, __p1__->GetObjectName())) return True;
		else if (0 == strcmp(szName, __p2__->GetObjectName())) return True;
		else if (0 == strcmp(szName, __p3__->GetObjectName())) return True;
		else if (0 == strcmp(szName, __p4__->GetObjectName())) return True;
		else return False;
	}

	virtual IObject *Acquire(const char *szName)
	{
		     if (0 == strcmp(szName, GetObjectName())) { Retain(); return this; }
		else if (0 == strcmp(szName, __p1__->GetObjectName())) { Retain(); return __p1__; }
		else if (0 == strcmp(szName, __p2__->GetObjectName())) { Retain(); return __p2__; }
		else if (0 == strcmp(szName, __p3__->GetObjectName())) { Retain(); return __p3__; }
		else if (0 == strcmp(szName, __p4__->GetObjectName())) { Retain(); return __p4__; }
		else return NULL;
	}

	virtual const IObject *Acquire(const char *szName) const
	{
		     if (0 == strcmp(szName, GetObjectName())) { Retain(); return this; }
		else if (0 == strcmp(szName, __p1__->GetObjectName())) { Retain(); return __p1__; }
		else if (0 == strcmp(szName, __p2__->GetObjectName())) { Retain(); return __p2__; }
		else if (0 == strcmp(szName, __p3__->GetObjectName())) { Retain(); return __p3__; }
		else if (0 == strcmp(szName, __p4__->GetObjectName())) { Retain(); return __p4__; }
		else return NULL;
	}

protected:

	void InitObject(IObject *pParent = NULL)
	{
		SetupObject(pParent);

		__p1__  = O1::Create(this);
		__p2__  = O2::Create(this);
		__p3__  = O3::Create(this);
		__p4__  = O4::Create(this);

		OnInitObject();
	}

	virtual void UninitObject() const
	{
		delete this;
	}

	~Object()
	{
		OnUninitObject();

		__p1__->SetupObject(NULL); __p1__->Release();
		__p2__->SetupObject(NULL); __p2__->Release();
		__p3__->SetupObject(NULL); __p3__->Release();
		__p4__->SetupObject(NULL); __p4__->Release();
	}

private:

	O1  *__p1__;
	O2  *__p2__;
	O3  *__p3__;
	O4  *__p4__;

	static inline void __stg__(OBJECT_INTERFACE *p) { };
	static inline void __stg__(O1 *p) { };
	static inline void __stg__(O2 *p) { };
	static inline void __stg__(O3 *p) { };
	static inline void __stg__(O4 *p) { };

};

template
<
	typename OBJECT_CLASS, typename OBJECT_INTERFACE,
	typename O1 , typename O2 , typename O3 
>
class Object<OBJECT_CLASS, OBJECT_INTERFACE, O1, O2, O3> : 
	public OBJECT_INTERFACE
{
public:

	static OBJECT_CLASS *Create(IObject *pParent = NULL)
	{
		OBJECT_CLASS *pObject;

		if (NULL == (pObject = new (std::nothrow) OBJECT_CLASS()))
		{
			return NULL;
		}
		pObject->InitObject(pParent);

		return pObject;
	}

	virtual bool Implements(const char *szName) const
	{
		     if (0 == strcmp(szName, GetObjectName())) return True;
		else if (0 == strcmp(szName, __p1__->GetObjectName())) return True;
		else if (0 == strcmp(szName, __p2__->GetObjectName())) return True;
		else if (0 == strcmp(szName, __p3__->GetObjectName())) return True;
		else return False;
	}

	virtual IObject *Acquire(const char *szName)
	{
		     if (0 == strcmp(szName, GetObjectName())) { Retain(); return this; }
		else if (0 == strcmp(szName, __p1__->GetObjectName())) { Retain(); return __p1__; }
		else if (0 == strcmp(szName, __p2__->GetObjectName())) { Retain(); return __p2__; }
		else if (0 == strcmp(szName, __p3__->GetObjectName())) { Retain(); return __p3__; }
		else return NULL;
	}

	virtual const IObject *Acquire(const char *szName) const
	{
		     if (0 == strcmp(szName, GetObjectName())) { Retain(); return this; }
		else if (0 == strcmp(szName, __p1__->GetObjectName())) { Retain(); return __p1__; }
		else if (0 == strcmp(szName, __p2__->GetObjectName())) { Retain(); return __p2__; }
		else if (0 == strcmp(szName, __p3__->GetObjectName())) { Retain(); return __p3__; }
		else return NULL;
	}

protected:

	void InitObject(IObject *pParent = NULL)
	{
		SetupObject(pParent);

		__p1__  = O1::Create(this);
		__p2__  = O2::Create(this);
		__p3__  = O3::Create(this);

		OnInitObject();
	}

	virtual void UninitObject() const
	{
		delete this;
	}

	~Object()
	{
		OnUninitObject();

		__p1__->SetupObject(NULL); __p1__->Release();
		__p2__->SetupObject(NULL); __p2__->Release();
		__p3__->SetupObject(NULL); __p3__->Release();
	}

private:

	O1  *__p1__;
	O2  *__p2__;
	O3  *__p3__;

	static inline void __stg__(OBJECT_INTERFACE *p) { };
	static inline void __stg__(O1 *p) { };
	static inline void __stg__(O2 *p) { };
	static inline void __stg__(O3 *p) { };

};

template
<
	typename OBJECT_CLASS, typename OBJECT_INTERFACE,
	typename O1 , typename O2 
>
class Object<OBJECT_CLASS, OBJECT_INTERFACE, O1, O2> : 
	public OBJECT_INTERFACE
{
public:

	static OBJECT_CLASS *Create(IObject *pParent = NULL)
	{
		OBJECT_CLASS *pObject;

		if (NULL == (pObject = new (std::nothrow) OBJECT_CLASS()))
		{
			return NULL;
		}
		pObject->InitObject(pParent);

		return pObject;
	}

	virtual bool Implements(const char *szName) const
	{
		     if (0 == strcmp(szName, GetObjectName())) return True;
		else if (0 == strcmp(szName, __p1__->GetObjectName())) return True;
		else if (0 == strcmp(szName, __p2__->GetObjectName())) return True;
		else return False;
	}

	virtual IObject *Acquire(const char *szName)
	{
		     if (0 == strcmp(szName, GetObjectName())) { Retain(); return this; }
		else if (0 == strcmp(szName, __p1__->GetObjectName())) { Retain(); return __p1__; }
		else if (0 == strcmp(szName, __p2__->GetObjectName())) { Retain(); return __p2__; }
		else return NULL;
	}

	virtual const IObject *Acquire(const char *szName) const
	{
		     if (0 == strcmp(szName, GetObjectName())) { Retain(); return this; }
		else if (0 == strcmp(szName, __p1__->GetObjectName())) { Retain(); return __p1__; }
		else if (0 == strcmp(szName, __p2__->GetObjectName())) { Retain(); return __p2__; }
		else return NULL;
	}

protected:

	void InitObject(IObject *pParent = NULL)
	{
		SetupObject(pParent);

		__p1__  = O1::Create(this);
		__p2__  = O2::Create(this);

		OnInitObject();
	}

	virtual void UninitObject() const
	{
		delete this;
	}

	~Object()
	{
		OnUninitObject();

		__p1__->SetupObject(NULL); __p1__->Release();
		__p2__->SetupObject(NULL); __p2__->Release();
	}

private:

	O1  *__p1__;
	O2  *__p2__;

	static inline void __stg__(OBJECT_INTERFACE *p) { };
	static inline void __stg__(O1 *p) { };
	static inline void __stg__(O2 *p) { };

};

template
<
	typename OBJECT_CLASS, typename OBJECT_INTERFACE,
	typename O1 
>
class Object<OBJECT_CLASS, OBJECT_INTERFACE, O1> : 
	public OBJECT_INTERFACE
{
public:

	static OBJECT_CLASS *Create(IObject *pParent = NULL)
	{
		OBJECT_CLASS *pObject;

		if (NULL == (pObject = new (std::nothrow) OBJECT_CLASS()))
		{
			return NULL;
		}
		pObject->InitObject(pParent);

		return pObject;
	}

	virtual bool Implements(const char *szName) const
	{
		     if (0 == strcmp(szName, GetObjectName())) return True;
		else if (0 == strcmp(szName, __p1__->GetObjectName())) return True;
		else return False;
	}

	virtual IObject *Acquire(const char *szName)
	{
		     if (0 == strcmp(szName, GetObjectName())) { Retain(); return this; }
		else if (0 == strcmp(szName, __p1__->GetObjectName())) { Retain(); return __p1__; }
		else return NULL;
	}

	virtual const IObject *Acquire(const char *szName) const
	{
		     if (0 == strcmp(szName, __p1__->GetObjectName())) { Retain(); return __p1__; }
		else return NULL;
	}

protected:

	void InitObject(IObject *pParent = NULL)
	{
		SetupObject(pParent);

		__p1__  = O1::Create(this);

		OnInitObject();
	}

	virtual void UninitObject() const
	{
		delete this;
	}

	~Object()
	{
		OnUninitObject();

		__p1__->SetupObject(NULL); __p1__->Release();
	}

private:

	O1  *__p1__;

	static inline void __stg__(OBJECT_INTERFACE *p) { };
	static inline void __stg__(O1 *p) { };

};

template
<
	typename OBJECT_CLASS, typename OBJECT_INTERFACE 
>
class Object<OBJECT_CLASS, OBJECT_INTERFACE> : 
	public OBJECT_INTERFACE
{
public:

	static OBJECT_CLASS *Create(IObject *pParent = NULL)
	{
		OBJECT_CLASS *pObject;

		if (NULL == (pObject = new (std::nothrow) OBJECT_CLASS()))
		{
			return NULL;
		}
		pObject->InitObject(pParent);

		return pObject;
	}

	virtual bool Implements(const char *szName) const
	{
		if (0 == strcmp(szName, GetObjectName())) return True;
		else return False;
	}

	virtual IObject *Acquire(const char *szName)
	{
		if (0 == strcmp(szName, GetObjectName())) { Retain(); return this; }
		else return NULL;
	}

	virtual const IObject *Acquire(const char *szName) const
	{
		if (0 == strcmp(szName, GetObjectName())) { Retain(); return this; }
		else return NULL;
	}

protected:

	void InitObject(IObject *pParent = NULL)
	{
		SetupObject(pParent);

		OnInitObject();
	}

	virtual void UninitObject() const
	{
		delete this;
	}

	~Object()
	{
		OnUninitObject();
	}

private:

};

}//namespace CX
