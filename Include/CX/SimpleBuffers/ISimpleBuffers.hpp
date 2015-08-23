
#pragma once


#include "CX/SimpleBuffers/IReader.hpp"
#include "CX/SimpleBuffers/IWriter.hpp"


namespace CX
{

namespace SimpleBuffers
{

class ISimpleBuffer
{
public:

	virtual ~ISimpleBuffer() { }

	virtual Status Read(IReader *pReader, const Char *szName = NULL) = 0;

	virtual Status Write(IWriter *pWriter, const Char *szName = NULL) const = 0;

};

}//namespace SimpleBuffers

}//namespace CX
