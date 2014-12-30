

#pragma once


#include "CX/Types.h"
#include "../../../Contrib/Mintomic/Include/mintomic.h"


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

class Atomic
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

	static inline UInt32 CompareExchange(AtomicCmpExcUInt *pObject, UInt32 nExpected, UInt32 nDesired)
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


