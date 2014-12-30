
#pragma once


#include "CX/STLAlloc.h"
#include <deque>


namespace CX
{

template <typename T>
struct Deque
{
	typedef std::deque<T, STLAlloc<T> >	Type;
};

}//namespace CX

