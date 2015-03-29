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
#include "CX/Status.hpp"
#include "CX/Vector.hpp"
#include "CX/Set.hpp"
#include "CX/Map.hpp"
#include "sparsehash/sparse_hash_set"
#include "sparsehash/sparse_hash_map"
#include "CX/SB/SparseHashAllocator.hpp"
#include "CX/SB/BoolForVector.hpp"


namespace CX
{

namespace SB
{

template <typename T>
struct Comparator
{
	bool operator() (const T &a, const T &b) const
	{
		return (0 == Compare(a, b));
	}
};

template <typename T>
struct Less
{
	Bool operator()(const T &a, const T &b) const
	{
		return (0 > Compare(a, b));
	}
};

template <typename T> static inline int Compare(const T &a, const T &b)
{
	if (a < b)
	{
		return -1;
	}
	else
	if (a > b)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

template <> static inline int Compare<Float>(const Float &a, const Float &b)
{
	if (a < b)
	{
		if (b - a < std::numeric_limits<Float>::epsilon())
		{
			return 0;
		}
		else
		{
			return -1;
		}
	}
	else
	if (a > b)
	{
		if (a - b < std::numeric_limits<Float>::epsilon())
		{
			return 0;
		}
		else
		{
			return 1;
		}
	}
	else
	{
		return 0;
	}
}

template <> static inline int Compare<Double>(const Double &a, const Double &b)
{
	if (a < b)
	{
		if (b - a < std::numeric_limits<Double>::epsilon())
		{
			return 0;
		}
		else
		{
			return -1;
		}
	}
	else
	if (a > b)
	{
		if (a - b < std::numeric_limits<Double>::epsilon())
		{
			return 0;
		}
		else
		{
			return 1;
		}
	}
	else
	{
		return 0;
	}
}

template <> static inline int Compare<String>(const String &a, const String &b)
{
	return cx_strcmp(a.c_str(), b.c_str());
}

template <> static inline int Compare<WString>(const WString &a, const WString &b)
{
	return cxw_strcmp(a.c_str(), b.c_str());
}

template <typename T, typename A = STLAlloc<T> > 
static inline int Compare(const std::vector<T, A> &a, const std::vector<T, A> &b)
{
	std::vector<T, A>::const_iterator iterA;
	std::vector<T, A>::const_iterator iterB;
	int                               nCmp;

	iterA = a.begin();
	iterB = b.begin();
	while (a.end() != iterA && b.end() != iterB)
	{
		if (0 != (nCmp = Compare(*iterA, *iterB)))
		{
			return nCmp;
		}
		iterA++;
		iterB++;
	}
	if (a.end() != iterA)
	{
		return 1;
	}
	if (b.end() != iterB)
	{
		return -1;
	}

	return 0;
}

template <typename K, typename C = Less<K>, typename A = STLAlloc<K> > 
static inline int Compare(const std::set<K, C, A> &a, const std::set<K, C, A> &b)
{
	std::set<K, C, A>::const_iterator iterA;
	std::set<K, C, A>::const_iterator iterB;
	int                               nCmp;

	iterA = a.begin();
	iterB = b.begin();
	while (a.end() != iterA && b.end() != iterB)
	{
		if (0 != (nCmp = Compare(*iterA, *iterB)))
		{
			return nCmp;
		}
		iterA++;
		iterB++;
	}
	if (a.end() != iterA)
	{
		return 1;
	}
	if (b.end() != iterB)
	{
		return -1;
	}

	return 0;
}

template <typename K, typename V, typename C = Less<K>, typename A = STLAlloc<std::pair<const K, V> > > 
static inline int Compare(const std::map<K, V, C, A> &a, const std::map<K, V, C, A> &b)
{
	std::map<K, V, C, A>::const_iterator iterA;
	std::map<K, V, C, A>::const_iterator iterB;
	int                                  nCmp;

	iterA = a.begin();
	iterB = b.begin();
	while (a.end() != iterA && b.end() != iterB)
	{
		if (0 != (nCmp = Compare(iterA->first, iterB->first)))
		{
			return nCmp;
		}
		if (0 != (nCmp = Compare(iterA->second, iterB->second)))
		{
			return nCmp;
		}
		iterA++;
		iterB++;
	}
	if (a.end() != iterA)
	{
		return 1;
	}
	if (b.end() != iterB)
	{
		return -1;
	}

	return 0;
}

template <typename K, typename H = Hasher<K>, typename E = Comparator<K>, typename A = SparseHashAllocator<K> > 
static inline int Compare(const google::sparse_hash_set<K, H, E, A> &a, const google::sparse_hash_set<K, H, E, A> &b)
{
	if (a.size() < b.size())
	{
		return -1;
	}
	else
	if (a.size() > b.size())
	{
		return 1;
	}

	for (google::sparse_hash_set<K, H, E, A>::const_iterator iterA = a.begin(); iterA != a.end(); ++iterA)
	{
		google::sparse_hash_set<K, H, E, A>::const_iterator iterB = b.find(*iterA);

		if (b.end() == iterB)
		{
			return 1;
		}
	}

	return 0;
}

template <typename K, typename V, typename H = Hasher<K>, typename E = Comparator<K>, 
          typename A = SparseHashAllocator<std::pair<const K, V> > > 
static inline int Compare(const google::sparse_hash_map<K, V, H, E, A> &a, const google::sparse_hash_map<K, V, H, E, A> &b)
{
	if (a.size() < b.size())
	{
		return -1;
	}
	else
	if (a.size() > b.size())
	{
		return 1;
	}

	int nCmp;

	for (google::sparse_hash_map<K, V, H, E, A>::const_iterator iterA = a.begin(); iterA != a.end(); ++iterA)
	{
		google::sparse_hash_map<K, V, H, E, A>::const_iterator iterB = b.find(iterA->first);

		if (b.end() == iterB)
		{
			return 1;
		}
		if (0 != (nCmp = Compare(iterA->second, iterB->second)))
		{
			return nCmp;
		}
	}

	return 0;
}

}//namespace SB

}//namespace CX

