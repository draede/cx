
#pragma once


#include "CX/Types.h"
#include "CX/Log/Level.h"
#include "CX/Log/IOutput.h"
#include "CX/Print.h"
#include "CX/APIDefs.h"


namespace CX
{

namespace Log
{

class CX_API IFormatter
{
public:

	virtual ~IFormatter() { }

	//pBuffer is always NULL terminated
	virtual StatusCode Write(IOutput *pOutput, Level nLevel, const Char *szTag, 
	                         const Char *pBuffer, Size cLen) = 0;

};

}//namespace Log

}//namespace CX

