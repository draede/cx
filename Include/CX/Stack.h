
#pragma once


#include "CX/Deque.h"
#include <stack>


namespace CX
{

template <typename T>
struct Stack
{
	typedef std::stack<T, typename Deque<T>::Type >     Type;
};

}//namespace CX

