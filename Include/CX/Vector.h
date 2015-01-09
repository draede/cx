
#pragma once


#include "CX/STLAlloc.h"
#include <vector>


namespace CX
{

template <typename T>
struct Vector
{
	typedef std::vector<T, STLAlloc<T> >     Type;
};

}//namespace CX

