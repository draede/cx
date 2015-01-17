
#pragma once


#include "CX/Log/IOutput.h"
#include "CX/APIDefs.h"


namespace CX
{

namespace Log
{

class CX_API SystemOutput : public IOutput
{
public:

	SystemOutput();

	~SystemOutput();

	virtual StatusCode Write(Level nLevel, const Char *szTag, const Char *pBuffer, Size cLen);

private:

};

}//namespace Log

}//namespace CX

