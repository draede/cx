
#pragma once


namespace CX
{

namespace Util
{

template <typename R>
class IFunction
{
public:

	virtual ~IFunction() { }

	virtual R Run() = 0;

};

}//namespace Util

}//namespace CX

