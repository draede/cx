
#pragma once


#include "CX/Types.hpp"
#include "CX/String.hpp"
#include "CX/Map.hpp"
#include "Field.hpp"


namespace CX
{

namespace SB
{


class Struct
{
public:

	String         m_sName;
	FieldsVector   m_vectorFields;

};

typedef Map<String, Struct>::Type   StructsMap;


}//namespace SB

}//namespace CX

