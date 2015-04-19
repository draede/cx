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
#include "CX/SB/SparseHashAllocator.hpp"
#include "CX/SB/BoolForVector.hpp"


namespace CX
{

namespace SB
{

static inline void DefInit(Bool &p)
{
	p = false;
}

static inline void DefInit(BoolForVector &p)
{
	p = 0;
}

static inline void DefInit(Int8 &p)
{
	p = 0;
}

static inline void DefInit(UInt8 &p)
{
	p = 0;
}

static inline void DefInit(Int16 &p)
{
	p = 0;
}

static inline void DefInit(UInt16 &p)
{
	p = 0;
}

static inline void DefInit(Int32 &p)
{
	p = 0;
}

static inline void DefInit(UInt32 &p)
{
	p = 0;
}

static inline void DefInit(Int64 &p)
{
	p = 0;
}

static inline void DefInit(UInt64 &p)
{
	p = 0;
}

static inline void DefInit(Float &p)
{
	p = 0.0f;
}

static inline void DefInit(Double &p)
{
	p = 0.0;
}

static inline void DefInit(String &p)
{
	p.clear();
}

static inline void DefInit(WString &p)
{
	p.clear();
}

template <typename T, typename A = STLAlloc<T> > static inline void DefInit(std::vector<T, A> &p)
{
	p.clear();
}

template <typename K, typename C = Less<K>, typename A = STLAlloc<K> > 
static inline void DefInit(std::set<K, C, A> &p)
{
	p.clear();
}

template <typename K, typename V, typename C = Less<K>, typename A = STLAlloc<std::pair<const K, V> > > 
static inline void DefInit(std::map<K, V, C, A> &p)
{
	p.clear();
}

template <typename K, typename H = Hasher<K>, typename E = Comparator<K>, typename A = SparseHashAllocator<K> > 
static inline void DefInit(google::sparse_hash_set<K, H, E, A> &p)
{
	p.clear();
}

template <typename K, typename V, typename H = Hasher<K>, typename E = Comparator<K>, 
          typename A = SparseHashAllocator<std::pair<const K, V> > > 
static inline void DefInit(google::sparse_hash_map<K, V, H, E, A> &p)
{
	p.clear();
}

}//namespace SB

}//namespace CX

