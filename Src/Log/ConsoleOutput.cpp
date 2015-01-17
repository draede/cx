
#include "CX/Log/ConsoleOutput.h"
#include "CX/C/stdio.h"
#include "CX/Status.h"


namespace CX
{

namespace Log
{

ConsoleOutput::ConsoleOutput()
{
	Status::Clear();
}

ConsoleOutput::~ConsoleOutput()
{
	Status::Clear();
}

StatusCode ConsoleOutput::Write(Level nLevel, const Char *szTag, const Char *pBuffer, Size cLen)
{
	Status::Clear();

	cx_printf("%s", pBuffer);

	return Status_OK;
}

}//namespace Log

}//namespace CX

