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

#include "CX/Platform.hpp"


#if defined(CX_OS_WINDOWS)


#include "CX/Util/StackTrace.hpp"
#include "CX/Status.hpp"
#include "CX/C/Platform/Windows/windows.h"
#include "CX/C/Alloc.h"
#include <dbghelp.h>


namespace CX
{

namespace Util
{

bool StackTrace::m_bSymInitialized = false;

StackTrace::StackTrace()
{
}

StackTrace::~StackTrace()
{
}

Status StackTrace::GetStackTrace(CallsVector &vectorCalls, Size cMaxEntries/* = MAX_STACK_ENTRIES*/, 
                                 unsigned int nFlags/* = Flag_IgnoreRuntimeInitCalls*/)
{
	vectorCalls.clear();

	HANDLE   hProcess = GetCurrentProcess();

	if (!m_bSymInitialized)
	{
		if (!SymInitialize(hProcess, NULL, TRUE))
		{
			return Status(Status_OperationFailed, "SymInitialize failed with error {1}", (unsigned int)GetLastError());
		}
		m_bSymInitialized = true;
	}

	void     **entries;
	UInt16   cFrames;

	if (NULL == (entries = (void **)CX_Alloc(cMaxEntries * sizeof(void *))))
	{
		return Status(Status_MemAllocFailed, "Failed to allocate stack entries");
	}
	if (0 == (cFrames = CaptureStackBackTrace(0, (UInt16)cMaxEntries, entries, NULL)))
	{
		CX_Free(entries);

		return Status(Status_OperationFailed, "CaptureStackBackTrace failed with error {1}", (unsigned int)GetLastError());
	}

	SYMBOL_INFO       *pSym;
	IMAGEHLP_LINE64   line;
	DWORD             dwDisplacement;

	if (NULL == (pSym = (SYMBOL_INFO *)CX_Alloc(sizeof(SYMBOL_INFO) + (MAX_SYMBOL_NAME + 1)* sizeof(Char))))
	{
		CX_Free(entries);

		return Status(Status_MemAllocFailed, "Failed to allocate symbol");
	}
	pSym->MaxNameLen = MAX_SYMBOL_NAME;
	pSym->SizeOfStruct = sizeof(SYMBOL_INFO);
	for (UInt16 i = 0; i < cFrames; i++)
	{
		if (!SymFromAddr(hProcess, (DWORD64)(entries[i]), 0, pSym))
		{
			CX_Free(pSym);
			CX_Free(entries);

			return Status(Status_OperationFailed, "SymFromAddr failed with error {1}", (unsigned int)GetLastError());
		}
		if (0 == cx_strncmp(pSym->Name, "CX::Util::StackTrace", sizeof("CX::Util::StackTrace") - 1) || 
		    0 == cx_strncmp(pSym->Name, "CX::Mem", sizeof("CX::Mem") - 1) || 
		    0 == cx_strncmp(pSym->Name, "CX::IObject", sizeof("CX::IObject") - 1) ||
		    0 == cx_strncmp(pSym->Name, "CX::STLAlloc", sizeof("CX::STLAlloc") - 1) ||
			 0 == cx_strncmp(pSym->Name, "std::", sizeof("std::") - 1))
		{
			continue;
		}
		if ((nFlags & Flag_IgnoreRuntimeInitCalls))
		{
			if (0 == cx_strcmp(pSym->Name, "__tmainCRTStartup"))
			{
				break;
			}
		}
		if (!SymGetLineFromAddr64(hProcess, (DWORD)(entries[i]), &dwDisplacement, &line))
		{
			DWORD dwError = GetLastError();

			if (487 == dwError)
			{
				line.FileName    = "<nosymbols>";
				line.LineNumber  = 0;
			}
			else
			{
				CX_Free(pSym);
				CX_Free(entries);

				return Status(Status_OperationFailed, "SymGetLineFromAddr64 failed with error {1}", (unsigned int)dwError);
			}
		}

		Call call;

		call.sFunction = pSym->Name;
		call.sFile     = line.FileName;
		call.cLine     = (Size)line.LineNumber;

		vectorCalls.push_back(call);
	}

	CX_Free(pSym);
	CX_Free(entries);

	return Status();
}

}//namespace Util

}//namespace CX


#endif

