
#pragma once


#include "CX/Types.h"


namespace CX
{

namespace IO
{

class IInputStream
{
public:

	virtual ~IInputStream() { }

	virtual StatusCode Read(void *pBuffer, Size cbReqSize, Size *pcbAckSize) = 0;

	virtual StatusCode SetPos(UInt64 cbPos) = 0;

	virtual StatusCode GetPos(UInt64 *pcbPos) const = 0;

	virtual StatusCode GetSize(UInt64 *pcbSize) const = 0;

	virtual bool IsEOF() const = 0;

	virtual bool IsOK() const = 0;

	virtual const Char *GetPath() const = 0;

};

}//namespace IO

}//namespace CX

