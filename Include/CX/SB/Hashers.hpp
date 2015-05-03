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

#pragma once


#include "CX/Types.hpp"
#include "CX/Status.hpp"
#include "CX/Vector.hpp"
#include "CX/Set.hpp"
#include "CX/Map.hpp"
#include "sparsehash/sparse_hash_set"
#include "sparsehash/sparse_hash_map"
#include "SparseHashAllocator.hpp"
#include "CX/SB/BoolForVector.hpp"
#include "CX/Hash/xxHash32.hpp"
#include "CX/Hash/xxHash64.hpp"
#include "CX/IObject.hpp"
#include "CX/SB/Comparators.hpp"


namespace CX
{

namespace SB
{

class HasherHelperSeed : public IObject
{
public:

#ifdef CX_32BIT_ARCH
	static UInt32 GetSeed()
	{
		static HasherHelperSeed hs;

		return hs.m_nSeed;
	}
#else
	static UInt64 GetSeed()
	{
		static HasherHelperSeed hs;

		return hs.m_nSeed;
	}
#endif

private:

#ifdef CX_32BIT_ARCH
	UInt32 m_nSeed;
#else
	UInt64 m_nSeed;
#endif

	HasherHelperSeed()
	{
#ifdef CX_32BIT_ARCH
		m_nSeed = 0; //randomize this
#else
		m_nSeed = 0; //randomize this
#endif
	}

	~HasherHelperSeed()
	{
	}

};

class HasherHelper : public IObject
{
public:

	static Size Hash(const void *pData, Size cbSize)
	{
#ifdef CX_32BIT_ARCH
		return (Size)CX::Hash::xxHash32::DoHash(pData, cbSize, HasherHelperSeed::GetSeed());
#else
		return (Size)CX::Hash::xxHash64::DoHash(pData, cbSize, HasherHelperSeed::GetSeed());
#endif
	}

	void Init()
	{
#ifdef CX_32BIT_ARCH
		UInt32 nSeed = HasherHelperSeed::GetSeed();
#else
		UInt64 nSeed = HasherHelperSeed::GetSeed();
#endif

		m_hash.Init(&nSeed);
	}

	void Update(const void *pData, Size cbSize)
	{
		m_hash.Update(pData, cbSize);
	}

	Size Done()
	{
#ifdef CX_32BIT_ARCH
		UInt32 nHash;
#else
		UInt64 nHash;
#endif
		m_hash.Done(&nHash);

		return (Size)nHash;
	}

private:

#ifdef CX_32BIT_ARCH
	Hash::xxHash32 m_hash;
#else
	Hash::xxHash32 m_hash;
#endif

};

template <typename T> inline Size Hash(const T &p, HasherHelper *pHasher)
{
	if (NULL == pHasher)
	{
		return HasherHelper::Hash(&p, sizeof(p));
	}
	else
	{
		pHasher->Update(&p, sizeof(p));

		return 0;
	}
}

template <> inline Size Hash<Bool>(const Bool &p, HasherHelper *pHasher)
{
	if (NULL == pHasher)
	{
		return (Size)p;
	}
	else
	{
		pHasher->Update(&p, sizeof(p));

		return 0;
	}
}

template <> inline Size Hash<BoolForVector>(const BoolForVector &p, HasherHelper *pHasher)
{
	return Hash((Bool)p, pHasher);
}

template <> inline Size Hash<Int8>(const Int8 &p, HasherHelper *pHasher)
{
	if (NULL == pHasher)
	{
		return (Size)p;
	}
	else
	{
		pHasher->Update(&p, sizeof(p));

		return 0;
	}
}

template <> inline Size Hash<UInt8>(const UInt8 &p, HasherHelper *pHasher)
{
	if (NULL == pHasher)
	{
		return (Size)p;
	}
	else
	{
		pHasher->Update(&p, sizeof(p));

		return 0;
	}
}

template <> inline Size Hash<Int16>(const Int16 &p, HasherHelper *pHasher)
{
	if (NULL == pHasher)
	{
		return (Size)p;
	}
	else
	{
		pHasher->Update(&p, sizeof(p));

		return 0;
	}
}

template <> inline Size Hash<UInt16>(const UInt16 &p, HasherHelper *pHasher)
{
	if (NULL == pHasher)
	{
		return (Size)p;
	}
	else
	{
		pHasher->Update(&p, sizeof(p));

		return 0;
	};
}

template <> inline Size Hash<Int32>(const Int32 &p, HasherHelper *pHasher)
{
	if (NULL == pHasher)
	{
		return (Size)p;
	}
	else
	{
		pHasher->Update(&p, sizeof(p));

		return 0;
	}
}

template <> inline Size Hash<UInt32>(const UInt32 &p, HasherHelper *pHasher)
{
	if (NULL == pHasher)
	{
		return (Size)p;
	}
	else
	{
		pHasher->Update(&p, sizeof(p));

		return 0;
	}
}

template <> inline Size Hash<String>(const String &p, HasherHelper *pHasher)
{
	if (NULL == pHasher)
	{
		return (Size)HasherHelper::Hash(p.c_str(), p.size() * sizeof(Char));
	}
	else
	{
		pHasher->Update(p.c_str(), p.size() * sizeof(Char));

		return 0;
	}
}

template <> inline Size Hash<WString>(const WString &p, HasherHelper *pHasher)
{
	if (NULL == pHasher)
	{
		return (Size)HasherHelper::Hash(p.c_str(), p.size() * sizeof(WChar));
	}
	else
	{
		pHasher->Update(p.c_str(), p.size() * sizeof(WChar));

		return 0;
	}
}

template <typename T> 
inline Size Hash(const std::vector<T, STLAlloc<T> > &p, HasherHelper *pHasher)
{
	if (NULL == pHasher)
	{
		HasherHelper hasher;

		hasher.Init();
		
		for (typename std::vector<T, STLAlloc<T> >::const_iterator iter = p.begin(); iter != p.end(); ++iter)
		{
			Hash(*iter, &hasher);
		}

		return hasher.Done();
	}
	else
	{
		for (typename std::vector<T, STLAlloc<T> >::const_iterator iter = p.begin(); iter != p.end(); ++iter)
		{
			Hash(*iter, pHasher);
		}

		return 0;
	}
}

template <typename K> 
inline Size Hash(std::set<K, Less<K>, STLAlloc<K> > &p, HasherHelper *pHasher)
{
	if (NULL == pHasher)
	{
		HasherHelper hasher;

		hasher.Init();

		for (typename std::set<K, Less<K>, STLAlloc<K> >::const_iterator iter = p.begin(); iter != p.end(); ++iter)
		{
			Hash(*iter, &hasher);
		}

		return hasher.Done();
	}
	else
	{
		for (typename std::set<K, Less<K>, STLAlloc<K> >::const_iterator iter = p.begin(); iter != p.end(); ++iter)
		{
			Hash(*iter, pHasher);
		}

		return 0;
	}
}

template <typename K, typename V> 
inline Size Hash(std::map<K, V, Less<K>, STLAlloc<std::pair<const K, V> > > &p, HasherHelper *pHasher)
{
	if (NULL == pHasher)
	{
		HasherHelper hasher;

		hasher.Init();

		for (typename std::map<K, V, Less<K>, STLAlloc<std::pair<const K, V> > >::const_iterator iter = p.begin(); 
		     iter != p.end(); ++iter)
		{
			Hash(iter->first, &hasher);
			Hash(iter->second, &hasher);
		}

		return hasher.Done();
	}
	else
	{
		for (typename std::map<K, V, Less<K>, STLAlloc<std::pair<const K, V> > >::const_iterator iter = p.begin(); 
		     iter != p.end(); ++iter)
		{
			Hash(iter->first, pHasher);
			Hash(iter->second, pHasher);
		}

		return 0;
	}
}

//not a good idea to use this as a key
template <typename K> 
inline Size Hash(const google::sparse_hash_set<K, Hasher<K>, Comparator<K>, SparseHashAllocator<K> > &p, HasherHelper *pHasher)
{
	Size cChkSum = 0;

	for (typename google::sparse_hash_set<K, Hasher<K>, Comparator<K>, SparseHashAllocator<K> >::const_iterator iter = p.begin(); 
	     iter != p.end(); ++iter)
	{
		HasherHelper hh;

		hh.Init();
		Hash(*iter, &hh);
		cChkSum += hh.Done();
	}

	if (NULL == pHasher)
	{
		return cChkSum;
	}
	else
	{
		Hash(cChkSum, pHasher);

		return 0;
	}
}

//not a good idea to use this as a key
template <typename K, typename V> 
inline Size Hash(const google::sparse_hash_map<K, V, Hasher<K>, Comparator<K>, SparseHashAllocator<std::pair<const K, V> > > &p, 
                 HasherHelper *pHasher)
{
	Size cChkSum = 0;

	for (typename google::sparse_hash_map<K, V, Hasher<K>, Comparator<K>, 
	     SparseHashAllocator<std::pair<const K, V> > >::const_iterator iter = p.begin(); iter != p.end(); ++iter)
	{
		HasherHelper hh;

		hh.Init();
		Hash(iter->first, &hh);
		Hash(iter->second, &hh);
		cChkSum += hh.Done();
	}

	if (NULL == pHasher)
	{
		return cChkSum;
	}
	else
	{
		Hash(cChkSum, pHasher);

		return 0;
	}
}

}//namespace SB

}//namespace CX

