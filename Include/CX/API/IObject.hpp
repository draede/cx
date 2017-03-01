/* 
 * The author disclaims copyright to this source code.
 */ 

#pragma once


namespace CX
{

class IInterface;
class IObject
{
public:

	virtual long Retain() const = 0;

	virtual long Release() const = 0;

	virtual bool Implements(const char *szName) const = 0;

	virtual IInterface *Acquire(const char *szName) = 0;

	virtual const IInterface *Acquire(const char *szName) const = 0;

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
		return (T *)Acquire(T::NAME());
	}

	template <typename T>
	const T *Acquire() const
	{
		return (const T *)Acquire(T::NAME());
	}

protected:

	virtual ~IObject() { }

};

}//namespace CX

