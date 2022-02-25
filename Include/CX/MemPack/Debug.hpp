/* 
 * CX - C++ framework for general purpose development
 *
 * https://github.com/draede/cx
 * 
 * Copyright (C) 2014 - 2022 draede - draede [at] outlook [dot] com
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
 
#pragma once


#include "CX/Types.hpp"
#include "CX/C/Platform/Windows/windows.h"
#include "CX/Print.hpp"


#ifdef CX_DEBUG
#define CXASSERT(out, cond)   if (!(cond)) { CX::Print(out, "FAIL AT {1}:{2} => {3}\n", __FILE__, __LINE__, #cond); fflush(out); DebugBreak(); }
#else
#define CXASSERT(out, cond)   if (!(cond)) { CX::Print(out, "FAIL AT {1}:{2} => {3}\n", __FILE__, __LINE__, #cond); fflush(out); exit(12345); }
#endif

//#define CXMP_PROF_SCOPE_ENABLED

#ifdef CXMP_PROF_SCOPE_ENABLED
#define CXMP_PROF_SCOPE(scope)   CX::MemPack::Profiler::Scope   __scope__(scope);
#else
#define CXMP_PROF_SCOPE(scope)
#endif


namespace CX
{

namespace MemPack
{

struct Profiler
{
	enum Slot
	{
		Slot_Alloc,
		Slot_Realloc,
		Slot_Free,
		Slot_Unused_Add,
		Slot_Unused_Remove,
		Slot_Unused_RemoveBestFit,
		Slot_Unused_RemoveBestFitFromLastBlock,
		Slot_Unused_GetVarBlocksIndexFromSize,

		Slot_MAX,
	};

	struct SlotData
	{
		const char      *szName;
		LARGE_INTEGER   liElapsed;
		Size            cCount;
	};

	static SlotData   slots[Slot_MAX];

	static inline void Init()
	{
		for (Size i = 0; i < (Size)Slot_MAX; i++)
		{
			slots[i].cCount             = 0;
			slots[i].liElapsed.QuadPart = 0;
		}
	}

	static inline void Dump()
	{
		LARGE_INTEGER   liFreq;

		QueryPerformanceFrequency(&liFreq);
		for (Size i = 0; i < (Size)Slot_MAX; i++)
		{
			Print(stdout, "{1} : {2:.5} seconds ({3} calls, {4:.10} seconds / call)\n", slots[i].szName, 
			      (double)slots[i].liElapsed.QuadPart / (double)liFreq.QuadPart, slots[i].cCount, 
			      ((double)slots[i].liElapsed.QuadPart / (double)liFreq.QuadPart) / (double)slots[i].cCount);
		}
	}

	struct Scope
	{
		Slot            uSlot;
		LARGE_INTEGER   liBegin;

		Scope(Slot uSlot)
		{
			this->uSlot = uSlot;
			QueryPerformanceCounter(&liBegin);
		}

		~Scope()
		{
			LARGE_INTEGER   liEnd;

			QueryPerformanceCounter(&liEnd);
			Profiler::slots[uSlot].cCount++;
			Profiler::slots[uSlot].liElapsed.QuadPart += liEnd.QuadPart - liBegin.QuadPart;
		}

	};

};

}//namespace MemPack

}//namespace CX

