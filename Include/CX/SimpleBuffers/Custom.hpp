/* 
 * CX - C++ framework for general purpose development
 *
 * https://github.com/draede/cx
 * 
 * Copyright (C) 2014 - 2019 draede - draede [at] outlook [dot] com
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
#include "CX/Status.hpp"
#include "CX/APIDefs.hpp"


namespace CX
{

namespace SimpleBuffers
{

class ICustom
{
public:

	enum Type
	{
		Type_BSON = 1,
	};

	struct Allocator
	{
		void *pCTX;

		void * (*Alloc)(void *pOldPtr, Size cbSize, void *pCtx);
		void(*Free)(void *pPtr, void *pCtx);

		Allocator()
		{
			Alloc = &DefaultAlloc;
			Free  = &DefaultFree;
		}

		static void *DefaultAlloc(void *pOldPtr, Size cbSize, void *pAllocatorCtx)
		{
			CX_UNUSED(pAllocatorCtx);

			return Mem::Realloc(pOldPtr, cbSize);
		}

		static void DefaultFree(void *pPtr, void *pAllocatorCtx)
		{
			CX_UNUSED(pAllocatorCtx);

			Mem::Free(pPtr);
		}

	};

	virtual ~ICustom() { }

	virtual Status OnBeginObject(const Char *szName = NULL) = 0;

	virtual Status OnEndObject() = 0;

	virtual Status OnBeginArray(const Char *szName = NULL) = 0;

	virtual Status OnEndArray() = 0;

	virtual Status OnBool(Bool v, const Char *szName = NULL) = 0;

	virtual Status OnInt(Int64 v, const Char *szName = NULL) = 0;

	virtual Status OnDouble(Double v, const Char *szName = NULL) = 0;

	virtual Status OnString(const Char *v, const Char *szName = NULL) = 0;

};

}//namespace SimpleBuffers

}//namespace CX

