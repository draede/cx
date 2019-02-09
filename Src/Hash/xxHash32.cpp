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

#include "CX/precomp.hpp"
#include "CX/Hash/xxHash32.hpp"
#include "CX/Status.hpp"


namespace CX
{

namespace Hash
{

const Char xxHash32::NAME[] = "xxHash32";

xxHash32::xxHash32()
{
	m_pState = XXH32_createState();

	Init();
}

xxHash32::~xxHash32()
{
	XXH32_freeState(m_pState);
	m_pState = NULL;
}

const Char *xxHash32::GetName()
{
	return NAME;
}

Size xxHash32::GetSize()
{
	return SIZE;
}

Status xxHash32::Init(const void *pHash/* = NULL*/)
{
	XXH_errorcode ec;

	if (NULL != pHash)
	{
		ec = XXH32_reset(m_pState, *((UInt32 *)pHash));
	}
	else
	{
		ec = XXH32_reset(m_pState, 0);
	}
	if (XXH_OK != ec)
	{
		return Status(Status_OperationFailed, "XXH32_reset failed with code {1}", (int)ec);
	}

	return Status();
}

Status xxHash32::Update(const void *pBuffer, Size cbSize)
{
	XXH_errorcode ec;

	ec = XXH32_update(m_pState, pBuffer, cbSize);
	if (XXH_OK != ec)
	{
		return Status(Status_OperationFailed, "XXH32_update failed with code {1}", (int)ec);
	}

	return Status();
}

Status xxHash32::Done(void *pHash)
{
	*((UInt32 *)pHash) = XXH32_digest(m_pState);

	return Status();
}

UInt32 xxHash32::DoHash(const void *pData, Size cbSize, UInt32 nSeed/* = 0*/)
{
	return XXH32(pData, cbSize, nSeed);
}

}//namespace Hash

}//namespace CX

