
#pragma once


#include "CX/Log/IOutput.h"
#include "CX/APIDefs.h"


namespace CX
{

namespace Log
{

class CX_API ConsoleOutput : public IOutput
{
public:

	ConsoleOutput();

	~ConsoleOutput();

	virtual StatusCode Write(Level nLevel, const Char *szTag, const Char *pBuffer, Size cLen);

private:

};

}//namespace Log

}//namespace CX

