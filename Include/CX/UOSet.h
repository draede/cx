
#pragma once


#include "CX/STLAlloc.h"
#include <unordered_set>


namespace CX
{

template <typename K, typename H = std::hash<K>, typename E = std::equal_to<K> >
struct UOSet
{
	typedef std::unordered_set<K, H, E, STLAlloc<K> >     Type;
};

}//namespace CX
