/* 
 * CX - C++ framework for general purpose development
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
 
#pragma  once


#include "CX/Mem.hpp"


namespace CX
{

template <typename T>
struct ReleaseByDelete
{
	static void Release(T *p)
	{
		delete p;
	}
};

template <typename T>
struct ReleaseByDeleteArray
{
	static void Release(T *p)
	{
		delete [] p;
	}
};

template <typename T>
struct ReleaseByMemFree
{
	static void Release(T *p)
	{
		Mem::Free(p);
	}
};

//moves ptrs on copy
template <typename T, typename ReleasePolicy = ReleaseByDelete<T> >
class ScopePtr
{
public:

	ScopePtr(T *pInst, bool bTakeOwnership = true)
	{
		m_pInst          = pInst;
		m_bTakeOwnership = bTakeOwnership;
	}

	ScopePtr(ScopePtr &sp)
	{
		m_pInst = NULL;
		Copy(sp);
	}

	~ScopePtr()
	{
		Release();
	}

	ScopePtr &operator=(ScopePtr &sp)
	{
		Copy(&sp);

		return *this;
	}

	void Copy(ScopePtr &sp)
	{
		Release();
		m_pInst          = sp.m_pInst;
		m_bTakeOwnership = sp.m_bTakeOwnership;
		sp.m_pInst       = NULL;
	}

	Bool IsValid()
	{
		return (NULL != m_pInst);
	}

	T *Get()
	{
		return m_pInst;
	}

	const T *Get() const
	{
		return m_pInst;
	}

	T *operator->()
	{
		return m_pInst;
	}

	const T *operator->() const
	{
		return m_pInst;
	}

private:

	T    *m_pInst;
	bool m_bTakeOwnership;

	void Release()
	{
		if (NULL != m_pInst && m_bTakeOwnership)
		{
			ReleasePolicy::Release(m_pInst);
		}
		m_pInst = NULL;
	}

};

}//namespace CX

