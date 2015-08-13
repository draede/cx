
#pragma once


#include "Object.hpp"


namespace CX
{

namespace Archive
{

template <typename OUTPUT>
class IGenerator
{
public:

	virtual ~IGenerator() { }

	virtual Status Generate(OUTPUT out, const Object &object) = 0;
	
};

}//namespace Archive

}//namespace CX
