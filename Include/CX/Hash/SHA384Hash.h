
#pragma once


#include "CX/Hash/IHash.h"
#include "CX/APIDefs.h"


namespace CX
{

namespace Hash
{

class CX_API SHA384Hash : public IHash
{
public:

	static const Char     NAME[];
	static const Size     SIZE   = 48;

	SHA384Hash();

	virtual ~SHA384Hash();

	virtual const Char *GetName();

	virtual Size GetSize();

	virtual StatusCode Init(const void *pCrypt = NULL);

	virtual StatusCode Update(const void *pBuffer, Size cbSize);

	virtual StatusCode Done(void *pCrypt);

private:

	void *m_pCTX;

};

}//namespace Hash

}//namespace CX

