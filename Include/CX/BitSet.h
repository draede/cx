
#pragma once


#include "CX/STLAlloc.h"
#include <bitset>


namespace CX
{

template <typename T>
struct BitSet
{
	typedef std::bitset<T, STLAlloc<T> >	Type;
};

}//namespace CX

