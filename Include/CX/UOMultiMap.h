
#pragma once


#include "CX/STLAlloc.h"
#include <unordered_map>


namespace CX
{

template <typename K, typename V, typename H = std::hash<K>, typename E = std::equal_to<K> >
struct UOMultiMap
{
	typedef std::unordered_multimap<K, V, H, E, STLAlloc<std::pair<const K, V> > >     Type;
};

}//namespace CX

