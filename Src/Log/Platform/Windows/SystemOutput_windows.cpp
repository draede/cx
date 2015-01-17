

#include "CX/Platform.h"


#if defined(CX_OS_WINDOWS)


#include "CX/Log/SystemOutput.h"
#include "CX/Status.h"
#include "CX/Str/UTF8.h"
#include <windows.h>


namespace CX
{

namespace Log
{

SystemOutput::SystemOutput()
{
	Status::Clear();
}

SystemOutput::~SystemOutput()
{
	Status::Clear();
}

StatusCode SystemOutput::Write(Level nLevel, const Char *szTag, const Char *pBuffer, Size cLen)
{
	Status::Clear();

	WString wsBuffer;

	if (CXNOK(Str::UTF8::ToWChar(pBuffer, &wsBuffer, cLen)))
	{
		return Status::GetCode();
	}

	OutputDebugStringW(wsBuffer.c_str());

	return Status_OK;
}

}//namespace Log

}//namespace CX


#endif


