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

#pragma once


#include "CX/Hash/IHash.hpp"
#include "CX/APIDefs.hpp"
#include "../../../Contrib/xxHash/Include/xxHash.h"


namespace CX
{

namespace Hash
{

class CX_API xxHash64 : public IHash
{
public:

	static const Char     NAME[];
	static const Size     SIZE   = 8;

	xxHash64();

	virtual ~xxHash64();

	virtual const Char *GetName();

	virtual Size GetSize();

	virtual Status Init(const void *pCrypt = NULL);

	virtual Status Update(const void *pBuffer, Size cbSize);

	virtual Status Done(void *pCrypt);

	static UInt64 Hash(const void *pData, Size cbSize, UInt64 nSeed = 0);

private:

	XXH64_state_t   m_state;

};

}//namespace Hash

}//namespace CX
