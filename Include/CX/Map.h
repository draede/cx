
#pragma once


#include "CX/STLAlloc.h"
#include <map>


namespace CX
{

template <typename K, typename V, typename C = std::less<K> >
struct Map
{
	typedef std::map<K, V, C, STLAlloc<std::pair<const K, V> > >	Type;
};

}//namespace CX

