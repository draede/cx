/* 
 * CX - C++ framework for general purpose development
 *
 * https://github.com/draede/cx
 * 
 * Copyright (C) 2014-2016 draede, draede [at] outlook [dot] com
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
#include "../../../Contrib/Mintomic/Include/mintomic.h"
#include "CX/IObject.hpp"


namespace CX
{

namespace Sys
{

typedef mint_atomic32_t		AtomicUInt32;
typedef mint_atomic64_t		AtomicUInt64;
typedef mint_atomicPtr_t	AtomicPtr;


#ifdef CX_32BIT_ARCH
	typedef mint_atomic32_t		AtomicCmpExcUInt;
#else
	typedef mint_atomic64_t		AtomicCmpExcUInt;
#endif

class Atomic : public IObject
{
public:

	static inline UInt32 Load(AtomicUInt32 *pObject)
	{
		return mint_load_32_relaxed(pObject);
	}
	
	static inline UInt64 Load(AtomicUInt64 *pObject)
	{
		return mint_load_64_relaxed(pObject);
	}

	static inline void *Load(AtomicPtr *pObject)
	{
		return mint_load_ptr_relaxed(pObject);
	}

	static inline void Store(AtomicUInt32 *pObject, UInt32 nValue)
	{
		mint_store_32_relaxed(pObject, nValue);
	}

	static inline void Store(AtomicUInt64 *pObject, UInt64 nValue)
	{
		mint_store_64_relaxed(pObject, nValue);
	}

	static inline void Store(AtomicPtr *pObject, void *pPtr)
	{
		mint_store_ptr_relaxed(pObject, pPtr);
	}

	static inline UInt32 FetchAdd(AtomicUInt32 *pObject, Int32 nOperand)
	{
		return mint_fetch_add_32_relaxed(pObject, nOperand);
	}

	static inline UInt64 FetchAdd(AtomicUInt64 *pObject, Int64 nOperand)
	{
		return mint_fetch_add_64_relaxed(pObject, nOperand);
	}

	static inline void *FetchAdd(AtomicPtr *pObject, PtrDiff nOperand)
	{
		return mint_fetch_add_ptr_relaxed(pObject, nOperand);
	}

	static inline UInt32 FetchAnd(AtomicUInt32 *pObject, UInt32 nOperand)
	{
		return mint_fetch_and_32_relaxed(pObject, nOperand);
	}

	static inline UInt64 FetchAnd(AtomicUInt64 *pObject, UInt64 nOperand)
	{
		return mint_fetch_and_64_relaxed(pObject, nOperand);
	}

	static inline void *FetchAnd(AtomicPtr *pObject, Size nOperand)
	{
		return mint_fetch_and_ptr_relaxed(pObject, nOperand);
	}

	static inline UInt32 FetchOr(AtomicUInt32 *pObject, UInt32 nOperand)
	{
		return mint_fetch_or_32_relaxed(pObject, nOperand);
	}

	static inline UInt64 FetchOr(AtomicUInt64 *pObject, UInt64 nOperand)
	{
		return mint_fetch_or_64_relaxed(pObject, nOperand);
	}

	static inline void *FetchOr(AtomicPtr *pObject, Size nOperand)
	{
		return mint_fetch_or_ptr_relaxed(pObject, nOperand);
	}

#ifdef CX_32BIT_ARCH
	static inline UInt32 CompareExchange(AtomicCmpExcUInt *pObject, UInt32 nExpected, 
	                                     UInt32 nDesired)
#else
	static inline UInt64 CompareExchange(AtomicCmpExcUInt *pObject, UInt64 nExpected, 
	                                     UInt64 nDesired)
#endif
	{
#ifdef CX_32BIT_ARCH
		return mint_compare_exchange_strong_32_relaxed(pObject, nExpected, nDesired);
#else
		return mint_compare_exchange_strong_64_relaxed(pObject, nExpected, nDesired);
#endif
	}
	
	static inline void *CompareExchange(AtomicPtr *pObject, void *pExpected, void *pDesired)
	{
		return mint_compare_exchange_strong_ptr_relaxed(pObject, pExpected, pDesired);
	}

private:

	Atomic()
	{
	}

	~Atomic()
	{
	}

};

}//namespace Sys

}//namespace CX


