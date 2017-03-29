/* 
 * The author disclaims copyright to this source code.
 */ 
 
#pragma once


#include "IObject.hpp"
#include <string.h>


namespace CX
{

template
<
	typename O1  = IObject::NullObject, typename O2  = IObject::NullObject, typename O3  = IObject::NullObject, 
	typename O4  = IObject::NullObject, typename O5  = IObject::NullObject, typename O6  = IObject::NullObject, 
	typename O7  = IObject::NullObject, typename O8  = IObject::NullObject, typename O9  = IObject::NullObject, 
	typename O10 = IObject::NullObject, typename O11 = IObject::NullObject, typename O12 = IObject::NullObject, 
	typename O13 = IObject::NullObject, typename O14 = IObject::NullObject, typename O15 = IObject::NullObject, 
	typename O16 = IObject::NullObject, typename O17 = IObject::NullObject 
>
struct ObjectCounter;

template
<
	typename O1, typename O2,  typename O3,  typename O4,  typename O5,  typename O6,  typename O7,  typename O8, 
	typename O9, typename O10, typename O11, typename O12, typename O13, typename O14, typename O15, typename O16 
>
struct ObjectCounter<O1, O2, O3, O4, O5, O6, O7, O8, O9, O10, O11, O12, O13, O14, O15, O16>
{
	static const size_t COUNT = 16;
};

template
<
	typename O1, typename O2,  typename O3,  typename O4,  typename O5,  typename O6,  typename O7,  typename O8, 
	typename O9, typename O10, typename O11, typename O12, typename O13, typename O14, typename O15 
>
struct ObjectCounter<O1, O2, O3, O4, O5, O6, O7, O8, O9, O10, O11, O12, O13, O14, O15>
{
	static const size_t COUNT = 15;
};

template
<
	typename O1, typename O2,  typename O3,  typename O4,  typename O5,  typename O6,  typename O7,  typename O8, 
	typename O9, typename O10, typename O11, typename O12, typename O13, typename O14 
>
struct ObjectCounter<O1, O2, O3, O4, O5, O6, O7, O8, O9, O10, O11, O12, O13, O14>
{
	static const size_t COUNT = 14;
};

template
<
	typename O1, typename O2,  typename O3,  typename O4,  typename O5,  typename O6,  typename O7,  typename O8, 
	typename O9, typename O10, typename O11, typename O12, typename O13 
>
struct ObjectCounter<O1, O2, O3, O4, O5, O6, O7, O8, O9, O10, O11, O12, O13>
{
	static const size_t COUNT = 13;
};

template
<
	typename O1, typename O2,  typename O3,  typename O4,  typename O5,  typename O6,  typename O7,  typename O8, 
	typename O9, typename O10, typename O11, typename O12 
>
struct ObjectCounter<O1, O2, O3, O4, O5, O6, O7, O8, O9, O10, O11, O12>
{
	static const size_t COUNT = 12;
};

template
<
	typename O1, typename O2,  typename O3,  typename O4,  typename O5,  typename O6,  typename O7,  typename O8, 
	typename O9, typename O10, typename O11 
>
struct ObjectCounter<O1, O2, O3, O4, O5, O6, O7, O8, O9, O10, O11>
{
	static const size_t COUNT = 11;
};

template
<
	typename O1, typename O2,  typename O3,  typename O4,  typename O5,  typename O6,  typename O7,  typename O8, 
	typename O9, typename O10 
>
struct ObjectCounter<O1, O2, O3, O4, O5, O6, O7, O8, O9, O10>
{
	static const size_t COUNT = 10;
};

template
<
	typename O1, typename O2,  typename O3,  typename O4,  typename O5,  typename O6,  typename O7,  typename O8, 
	typename O9 
>
struct ObjectCounter<O1, O2, O3, O4, O5, O6, O7, O8, O9>
{
	static const size_t COUNT = 9;
};

template
<
	typename O1, typename O2,  typename O3,  typename O4,  typename O5,  typename O6,  typename O7,  typename O8
>
struct ObjectCounter<O1, O2, O3, O4, O5, O6, O7, O8>
{
	static const size_t COUNT = 8;
};

template
<
	typename O1, typename O2,  typename O3,  typename O4,  typename O5,  typename O6,  typename O7
>
struct ObjectCounter<O1, O2, O3, O4, O5, O6, O7>
{
	static const size_t COUNT = 7;
};

template
<
	typename O1, typename O2,  typename O3,  typename O4,  typename O5,  typename O6
>
struct ObjectCounter<O1, O2, O3, O4, O5, O6>
{
	static const size_t COUNT = 6;
};

template
<
	typename O1, typename O2,  typename O3,  typename O4,  typename O5
>
struct ObjectCounter<O1, O2, O3, O4, O5>
{
	static const size_t COUNT = 5;
};

template
<
	typename O1, typename O2,  typename O3,  typename O4
>
struct ObjectCounter<O1, O2, O3, O4>
{
	static const size_t COUNT = 4;
};

template
<
	typename O1, typename O2,  typename O3
>
struct ObjectCounter<O1, O2, O3>
{
	static const size_t COUNT = 3;
};

template
<
	typename O1, typename O2
>
struct ObjectCounter<O1, O2>
{
	static const size_t COUNT = 2;
};

template
<
	typename O1
>
struct ObjectCounter<O1>
{
	static const size_t COUNT = 1;
};

template
<
>
struct ObjectCounter< >
{
	static const size_t COUNT = 0;
};

template
<
	typename OBJECT_CLASS, typename OBJECT_INTERFACE,
	typename O1  = IObject::NullObject, typename O2  = IObject::NullObject, typename O3  = IObject::NullObject, 
	typename O4  = IObject::NullObject, typename O5  = IObject::NullObject, typename O6  = IObject::NullObject,
	typename O7  = IObject::NullObject, typename O8  = IObject::NullObject, typename O9  = IObject::NullObject,
	typename O10 = IObject::NullObject, typename O11 = IObject::NullObject, typename O12 = IObject::NullObject,
	typename O13 = IObject::NullObject, typename O14 = IObject::NullObject, typename O15 = IObject::NullObject,
	typename O16 = IObject::NullObject 
>
class Object : public OBJECT_INTERFACE
{
public:

	static OBJECT_INTERFACE *Create(IObject *pParent = NULL)
	{
		OBJECT_INTERFACE *pObject;

		if (NULL == (pObject = new (std::nothrow) OBJECT_CLASS()))
		{
			return NULL;
		}
		pObject->InitObject(pParent);

		return pObject;
	}

	virtual size_t GetImplementationsCount() const
	{
		return OBJECTS_COUNT;
	}

	virtual const char *GetImplementationName(size_t cIndex) const
	{
		if (OBJECTS_COUNT > cIndex)
		{
			return __objects__[cIndex - 1]->GetObjectName();
		}
		else
		{
			return "";
		}
	}

	virtual size_t GetImplementationIndex(const char *szName) const
	{
		for (size_t i = 0; i < OBJECTS_COUNT; i++)
		{
			if (0 == strcmp(szName, __objects__[i]->GetObjectName()))
			{
				return i;
			}
		}

		return (size_t)-1;
	}

	virtual bool Implements(const char *szName) const
	{
		return ((size_t)-1 != GetImplementationIndex(szName));
	}

	virtual IObject *Acquire(const char *szName)
	{
		size_t cIndex;

		if ((size_t)-1 != (cIndex = GetImplementationIndex(szName)))
		{
			Retain();

			return __objects__[cIndex];
		}
		else
		{
			return NULL;
		}
	}

	virtual const IObject *Acquire(const char *szName) const
	{
		size_t cIndex;

		if ((size_t)-1 != (cIndex = GetImplementationIndex(szName)))
		{
			Retain();

			return __objects__[cIndex];
		}
		else
		{
			return NULL;
		}
	}

protected:

	void InitObject(IObject *pParent = NULL)
	{
		SetupObject(pParent);

		__objects__[0]  = this;
		__objects__[1]  = O1::Create(this);
		__objects__[2]  = O2::Create(this);
		__objects__[3]  = O3::Create(this);
		__objects__[4]  = O4::Create(this);
		__objects__[5]  = O5::Create(this);
		__objects__[6]  = O6::Create(this);
		__objects__[7]  = O7::Create(this);
		__objects__[8]  = O8::Create(this);
		__objects__[9]  = O9::Create(this);
		__objects__[10] = O10::Create(this);
		__objects__[11] = O11::Create(this);
		__objects__[12] = O12::Create(this);
		__objects__[13] = O13::Create(this);
		__objects__[14] = O14::Create(this);
		__objects__[15] = O15::Create(this);
		__objects__[16] = O16::Create(this);

		OnInitObject();
	}

	virtual void UninitObject() const
	{
		delete this;
	}

	~Object()
	{
		OnUninitObject();

		for (size_t i = 1; i < OBJECTS_COUNT; i++)
		{
			__objects__[i]->SetupObject(NULL);
			__objects__[i]->Release();
		}
	}

private:

	static const size_t MAX_OBJECTS_COUNT = 16;

	static const size_t OBJECTS_COUNT = 
	                   ObjectCounter<O1, O2, O3, O4, O5, O6, O7, O8, O9, O10, O11, O12, O13, O14, O15, O16>::COUNT + 1;

	IObject *__objects__[MAX_OBJECTS_COUNT + 1];

};

}//namespace CX
