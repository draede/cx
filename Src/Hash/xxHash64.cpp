/* 
 * CX - C++ framework for general purpose development
 *
 * https://github.com/draede/cx
 * 
 * Copyright (C) 2014 - 2017 draede, draede [at] outlook [dot] com
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

#include "CX/Hash/xxHash64.hpp"
#include "CX/Status.hpp"


namespace CX
{

namespace Hash
{

const Char xxHash64::NAME[] = "xxHash64";

xxHash64::xxHash64()
{
	Init();
}

xxHash64::~xxHash64()
{
}

const Char *xxHash64::GetName()
{
	return NAME;
}

Size xxHash64::GetSize()
{
	return SIZE;
}

Status xxHash64::Init(const void *pHash/* = NULL*/)
{
	XXH_errorcode ec;

	if (NULL != pHash)
	{
		ec = XXH64_reset(&m_state, *((UInt64 *)pHash));
	}
	else
	{
		ec = XXH64_reset(&m_state, 0);
	}
	if (XXH_OK != ec)
	{
		return Status(Status_OperationFailed, "XXH64_reset failed with code {1}", (int)ec);
	}

	return Status();
}

Status xxHash64::Update(const void *pBuffer, Size cbSize)
{
	XXH_errorcode ec;

	ec = XXH64_update(&m_state, pBuffer, cbSize);
	if (XXH_OK != ec)
	{
		return Status(Status_OperationFailed, "XXH64_update failed with code {1}", (int)ec);
	}

	return Status();
}

Status xxHash64::Done(void *pHash)
{
	*((UInt64 *)pHash) = XXH64_digest(&m_state);

	return Status();
}

UInt64 xxHash64::DoHash(const void *pData, Size cbSize, UInt64 nSeed/* = 0*/)
{
	return XXH64(pData, cbSize, nSeed);
}

}//namespace Hash

}//namespace CX

