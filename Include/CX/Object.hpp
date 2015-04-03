/* 
 * CX - C++ framework for general purpose developement
 *
 * https://github.com/draede/cx
 * 
 * Copyright (C) 2014-2015 draede - draede [at] outlook [dot] com
 *
 * Released under the MIT License.
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */ 

#pragma once


#include "CX/Types.hpp"
#include "CX/Sys/Atomic.hpp"
#include "CX/C/string.h"
#include "CX/APIDefs.hpp"


#define CX_OBJECT_INTERFACE(name)   static const CX::Char *ID() { return #name; }

#define CX_OBJECT_BEGIN(name)                                                                                                    \
friend name *CX::New<name>();                                                                                                    \
friend void CX::Delete<name>(name *);                                                                                            \
name()                                                                                                                           \
{                                                                                                                                \
}                                                                                                                                \
~name()                                                                                                                          \
{                                                                                                                                \
}                                                                                                                                \
virtual void *GetInterface(const CX::Char *szID)                                                                                 \
{

#define CX_OBJECT_DEF_INTERFACE(name)                                                                                            \
if (0 == cx_strcmp(szID, name::ID()))                                                                                            \
{                                                                                                                                \
	return (name *)this;                                                                                                          \
}

#define CX_OBJECT_END()                                                                                                          \
	return NULL;                                                                                                                  \
}

namespace CX
{

class CX_API Object
{
public:

	Object();

	virtual ~Object();

	template <typename T> static T *Create()
	{
		T      *pInst = New<T>();
		Object *pObj  = (Object *)dynamic_cast<T *>(pInst);

		if (NULL == pObj)
		{
			Delete(pInst);

			return NULL;
		}
		else
		{
			pObj->Init();

			return pInst;
		}
	}

	template <typename T> T *AcquireInterface()
	{
		T *pInst;

		if (NULL != (pInst = (T *)GetInterface(T::ID())))
		{
			Retain();

			return pInst;
		}
		else
		{
			return NULL;
		}
	}

	template <typename T> void ReleaseInterface(T *pInst)
	{
		Release();
	}

	bool Destroy(bool bForce = false);

	Size GetRefCount();

protected:

	virtual void *GetInterface(const Char *szID) = 0;

private:

	Sys::AtomicUInt32   m_refcnt;

	void Init();

	void Retain();

	bool Release();

};

}//namespace CX

