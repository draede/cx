
#pragma once


#include "CX/Types.h"
#include "CX/APIDefs.h"


namespace CX
{

namespace Hash
{

class CX_API IHash
{
public:

	virtual ~IHash() { }

	virtual const Char *GetName() = 0;

	virtual Size GetSize() = 0;

	virtual StatusCode Init(const void *pHash = NULL) = 0;

	virtual StatusCode Update(const void *pBuffer, Size cbSize) = 0;

	virtual StatusCode Done(void *pHash) = 0;

};

}//namespace Hash

}//namespace CX

