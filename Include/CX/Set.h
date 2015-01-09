
#pragma once


#include "CX/STLAlloc.h"
#include <set>


namespace CX
{

template <typename K, typename C = std::less<K> >
struct Set
{
	typedef std::set<K, C, STLAlloc<K> >     Type;
};

}//namespace CX

