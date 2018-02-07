/* 
 * CX - C++ framework for general purpose development
 *
 * https://github.com/draede/cx
 * 
 * Copyright (C) 2014 - 2018 draede - draede [at] outlook [dot] com
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


#include "CX/SparseHashMap.hpp"
#include "CX/APIDefs.hpp"
#include "CX/Types.hpp"


namespace CX
{

namespace Util
{

class CX_API RCMemBufferMgr
{
public:

	static RCMemBufferMgr &Get();

	void *Alloc(Size cbSize);

	void Retain(void *pMem);

	void Release(void *pMem);

	//will free all mem (even if used)
	void Cleanup();

	Size GetAllocsSize() const;

private:

	struct Hasher
	{
		size_t operator()(const void *p) const
		{
			return (size_t)p;
		}
	};

	struct Comparator
	{
		bool operator() (const void *a, const void *b) const
		{
			return (a == b);
		}
	};

	typedef struct _MemBuffer
	{
		Size cbSize;
		Size cRefCount;
	}MemBuffer;

	typedef SparseHashMap<void *, MemBuffer, Hasher, Comparator>::Type   MemBuffersMap;

#pragma warning(push)
#pragma warning(disable: 4251)
	MemBuffersMap   m_mapMemBuffers;
#pragma warning(pop)
	Size            m_cbAllocsSize;

	RCMemBufferMgr();

	~RCMemBufferMgr();

};


}//namespace Util

}//namespace CX

