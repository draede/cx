
#pragma once


#include "CX/STLAlloc.h"
#include <stack>


namespace CX
{

template <typename T>
struct Stack
{
	typedef std::stack<T, STLAlloc<T> >	Type;
};

}//namespace CX

