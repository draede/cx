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
#include "CX/Mem.hpp"
#include "CX/SB/Comparators.hpp"
#include "CX/SB/Hashers.hpp"
#include "CX/SB/BoolForVector.hpp"


namespace CX
{

namespace SB
{

template <typename T, typename A = STLAlloc<T> >
struct Vector
{
	typedef std::vector<T, A>   Type;
};

template <>
struct Vector<Bool>
{
	typedef std::vector<BoolForVector, STLAlloc<BoolForVector> >     Type;
};

template <typename K, typename C = Less<K>, typename A = STLAlloc<K> >
struct Set
{
	typedef std::set<K, C, A>   Type;
};

template <typename K, typename V, typename C = Less<K>, typename A = STLAlloc<std::pair<const K, V> > > 
struct Map
{
	typedef std::map<K, V, C, A>   Type;
};


template <typename K, typename H = Hasher<K>, typename E = Comparator<K>, typename A = SparseHashAllocator<K> >
struct HashSet
{
	typedef google::sparse_hash_set<K, H, E, A>   Type;
};

template <typename K, typename V, typename H = Hasher<K>, typename E = Comparator<K>, 
          typename A = SparseHashAllocator<std::pair<const K, V> > > 
struct HashMap
{
	typedef google::sparse_hash_map<K, V, H, E, A>   Type;
};

}//namespace SB

}//namespace CX

