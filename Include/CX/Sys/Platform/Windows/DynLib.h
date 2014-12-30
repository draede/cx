
#pragma once


#include "CX/Platform.h"


#if defined(CX_OS_WINDOWS)


#include "CX/Types.h"
#include "CX/APIDefs.h"
#include <windows.h>


namespace CX
{

namespace Sys
{

class CX_API DynLib
{
public:

	typedef HMODULE		Handle;

	DynLib();

	virtual ~DynLib();

	bool IsOK();

	StatusCode Load(const Char *szPath);

	StatusCode Unload();

	void *GetFunc(const Char *szName);

private:

	Handle m_hHandle;

};

}//namespace Sys

}//namespace CX


#endif

