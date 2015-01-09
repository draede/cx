
#pragma once


#include "CX/STLAlloc.h"
#include <list>


namespace CX
{

template <typename T>
struct List
{
	typedef std::list<T, STLAlloc<T> >     Type;
};

}//namespace CX

