
#pragma once


#include "CX/Archive/IReader.hpp"
#include "CX/Archive/IWriter.hpp"


namespace CX
{

namespace Archive
{

class IArchivable
{
public:

	virtual ~IArchivable() { }

	virtual Status Read(IReader *pReader, const Char *szName = NULL) = 0;

	virtual Status Write(IWriter *pWriter, const Char *szName = NULL) const = 0;

};

}//namespace Archive

}//namespace CX
