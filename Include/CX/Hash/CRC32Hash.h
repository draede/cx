
#pragma once


#include "CX/Hash/IHash.h"
#include "CX/APIDefs.h"


namespace CX
{

namespace Hash
{

class CX_API CRC32Hash : public IHash
{
public:

	static const Char     NAME[];
	static const Size     SIZE   = 4;

	CRC32Hash();

	virtual ~CRC32Hash();

	virtual const Char *GetName();

	virtual Size GetSize();

	virtual StatusCode Init(const void *pCrypt = NULL);

	virtual StatusCode Update(const void *pBuffer, Size cbSize);

	virtual StatusCode Done(void *pCrypt);

private:

	UInt32 m_nCRC32;

};

}//namespace Hash

}//namespace CX

