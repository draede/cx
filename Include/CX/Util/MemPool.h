/* 
 * CX - C++ framework for general purpose developement
 *
 * https://github.com/draede/cx
 * 
 * Copyright (C) 2014 draede - draede [at] outlook [dot] com
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


#include "CX/Types.h"
#include "CX/Limits.h"
#include "CX/String.h"
#include "CX/APIDefs.h"


namespace CX
{

namespace Util
{

class CX_API IMemPool
{
public:

	virtual ~IMemPool() { }

	virtual void *GetMem() = 0;

	virtual const void *GetMem() const = 0;

	//pool used size
	virtual Size GetSize() const = 0;

	//pool total size
	virtual Size GetTotalSize() const = 0;

	//pool max size
	virtual Size GetMaxSize() const = 0;

	virtual StatusCode SetSize(Size cbSize) = 0;
		
	virtual StatusCode Add(Size cbSize) = 0;

	virtual StatusCode Add(const void *pMem, Size cbSize) = 0;

};

class CX_API DynMemPool : IMemPool
{
public:

	DynMemPool(Size cbMaxSize = SIZET_MAX);

	~DynMemPool();

	virtual void *GetMem();

	virtual const void *GetMem() const;

	//pool used size
	virtual Size GetSize() const;

	//pool total size
	virtual Size GetTotalSize() const;

	//pool max size
	virtual Size GetMaxSize() const;

	virtual StatusCode SetSize(Size cbSize);

	virtual StatusCode Add(Size cbSize);

	virtual StatusCode Add(const void *pMem, Size cbSize);

private:

	void *m_pMem;
	Size m_cbSize;
	Size m_cbTotalSize;
	Size m_cbMaxSize;

};

class CX_API StaticMemPool : IMemPool
{
public:

	StaticMemPool(void *pMem, Size cbSize);

	~StaticMemPool();

	virtual void *GetMem();

	virtual const void *GetMem() const;

	//pool used size
	virtual Size GetSize() const;

	//pool total size
	virtual Size GetTotalSize() const;

	//pool max size
	virtual Size GetMaxSize() const;

	virtual StatusCode SetSize(Size cbSize);

	virtual StatusCode Add(Size cbSize);

	virtual StatusCode Add(const void *pMem, Size cbSize);

private:

	void *m_pMem;
	Size m_cbSize;
	Size m_cbTotalSize;

};

}//namespace Util

}//namespace CX

