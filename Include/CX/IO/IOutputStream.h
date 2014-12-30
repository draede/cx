
#pragma once


#include "CX/Types.h"


namespace CX
{

namespace IO
{

class IOutputStream
{
public:

	virtual ~IOutputStream() { }

	virtual StatusCode Write(const void *pBuffer, Size cbReqSize, Size *pcbAckSize) = 0;

	virtual StatusCode GetSize(UInt64 *pcbSize) const = 0;

	virtual bool IsOK() const = 0;

	virtual const Char *GetPath() const = 0;

};

}//namespace IO

}//namespace CX

