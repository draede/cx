
#pragma once


#include "CX/Platform.h"


#if defined(CX_OS_WINDOWS)


#include "CX/Alloc.h"
#include <windows.h>


namespace CX
{

void *Alloc(Size cbSize)
{
	return HeapAlloc(GetProcessHeap(), 0, cbSize);
}

void *Realloc(void *pPtr, Size cbSize)
{
	if (NULL == pPtr)
	{
		return HeapAlloc(GetProcessHeap(), 0, cbSize);
	}
	else
	{
		return HeapReAlloc(GetProcessHeap(), 0, pPtr, cbSize);
	}
}

void Free(void *pPtr)
{
	if (NULL != pPtr)
	{
		HeapFree(GetProcessHeap(), 0, pPtr);
	}
}

}//namespace CX


#endif

