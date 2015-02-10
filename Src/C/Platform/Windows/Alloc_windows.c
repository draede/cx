/* 
 * CX - C++ framework for general purpose developement
 *
 * https://github.com/draede/cx
 * 
 * Copyright (C) 2014-2015 draede - draede [at] outlook [dot] com
 *
 * Released under the MIT License.
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */ 

#include "CX/C/Platform.h"


#if defined(CX_OS_WINDOWS)


#include "CX/C/Alloc.h"
#include <windows.h>


void *CX_Alloc(CX_Size cbSize)
{
	return HeapAlloc(GetProcessHeap(), 0, cbSize);
}

void *CX_Realloc(void *pPtr, CX_Size cbSize)
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

void CX_Free(void *pPtr)
{
	if (NULL != pPtr)
	{
		HeapFree(GetProcessHeap(), 0, pPtr);
	}
}


#endif
