
#pragma once


#include "CX/Log/IOutput.h"
#include "CX/IO/IOutputStream.h"
#include "CX/APIDefs.h"


namespace CX
{

namespace Log
{

class CX_API StreamOutput : public IOutput
{
public:

	//pOutput was allocated with New...
	StreamOutput(IO::IOutputStream *pOutputStream);

	~StreamOutput();

	virtual StatusCode Write(Level nLevel, const Char *szTag, const Char *pBuffer, Size cLen);

private:

	IO::IOutputStream   *m_pOutputStream;

};

}//namespace Log

}//namespace CX

