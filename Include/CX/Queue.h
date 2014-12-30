
#pragma once


#include "CX/Deque.h"
#include <queue>


namespace CX
{

template <typename T>
struct Queue
{
	typedef std::queue<T, Deque<T> >	Type;
};

}//namespace CX

