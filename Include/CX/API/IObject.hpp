/* 
 * The author disclaims copyright to this source code.
 */ 
 
#pragma once


#include <stdlib.h>
#ifdef _WIN32
	#include <windows.h>
#endif


#define CX_DECLARE_INTERFACE(name)                                                                                     \
	static const char *OBJECT_NAME() { return name; }                                                                   \
                                                                                                                       \
	virtual const char *GetObjectName() const                                                                           \
	{                                                                                                                   \
		return OBJECT_NAME();                                                                                            \
	}


namespace CX
{

class IObject
{
public:

	struct NullObject 
	{

		static IObject *Create(IObject *pObject)
		{
			(void)pObject;

			return NULL;
		}

	};

	virtual const char *GetObjectName() const = 0;

	virtual size_t GetImplementationsCount() const = 0;

	virtual const char *GetImplementationName(size_t cIndex) const = 0;

	virtual size_t GetImplementationIndex(const char *szName) const = 0;

	virtual bool Implements(const char *szName) const = 0;

	virtual IObject *Acquire(const char *szName) = 0;

	virtual const IObject *Acquire(const char *szName) const = 0;

	template <typename T>
	T *Acquire(const char *szName)
	{
		return (T *)Acquire(szName);
	}

	template <typename T>
	const T *Acquire(const char *szName) const
	{
		return (const T *)Acquire(szName);
	}

	template <typename T>
	T *Acquire()
	{
		return (T *)Acquire(T::OBJECT_NAME());
	}

	template <typename T>
	const T *Acquire() const
	{
		return (const T *)Acquire(T::OBJECT_NAME());
	}

	void Retain() const
	{
		if (NULL != m_pParent)
		{
			m_pParent->Retain();
		}
		else
		{
#ifdef _WIN32
			InterlockedIncrement(&m_cRefCount);
#else
			__sync_fetch_and_add(&m_cRefCount, 1);
#endif
		}
	}

	void Release() const
	{
		if (NULL != m_pParent)
		{
			m_pParent->Release();
		}
		else
		{
#ifdef _WIN32
			if (0 == InterlockedDecrement(&m_cRefCount))
#else
			if (0 == (cRefCount = __sync_fetch_and_sub(&m_cRefCount, 1)))
#endif
			{
				UninitObject();
			}
		}
	}

	void SetupObject(IObject *pParent)
	{
		m_pParent = pParent;
	}

	virtual void InitObject(IObject *pParent = NULL) = 0;

	virtual void UninitObject() const = 0;

	virtual void OnInitObject()  { }

	//do not use this to release own objects
	virtual void OnUninitObject() { }

protected:

	IObject()
	{
		m_pParent = NULL;
		m_cRefCount = 1;
	}

	virtual ~IObject()
	{
	}

private:

	IObject               *m_pParent;
	mutable volatile long m_cRefCount;

};

}//namespace CX
