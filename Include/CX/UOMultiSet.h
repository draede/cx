
#pragma once


#include "CX/STLAlloc.h"
#include <unordered_set>


namespace CX
{

template <typename K, typename H = std::hash<K>, typename E = std::equal_to<K> >
struct UOMultiSet
{
	typedef std::unordered_multiset<T, H, E, STLAlloc<K> >	Type;
};

}//namespace CX
