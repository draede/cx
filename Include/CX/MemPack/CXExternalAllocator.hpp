/* 
 * CX - C++ framework for general purpose development
 *
 * https://github.com/draede/cx
 * 
 * Copyright (C) 2014 - 2022 draede
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
#include "CX/MemPack/ExternalAllocator.hpp"
#include "CX/Mem.hpp"


namespace CX
{

namespace MemPack
{

struct CXExternalAllocator
{

	static ExternalAllocator *Get()
	{
		static ExternalAllocator   extalloc = { &Alloc, &Realloc, &Free };

		return &extalloc;
	}

	static inline void *Alloc(void *pAllocCxt, Size cbSize)
	{
		pAllocCxt;

		return Mem::Alloc(cbSize);
	}

	static inline void *Realloc(void *pAllocCxt, void *pPtr, Size cbSize)
	{
		pAllocCxt;

		return Mem::Realloc(pPtr, cbSize);
	}

	static inline void Free(void *pAllocCxt, void *pPtr)
	{
		pAllocCxt;

		Mem::Free(pPtr);
	}

};

}//namespace MemPack

}//namespace CX
