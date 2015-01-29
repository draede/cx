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

#include "CX/Hash/SHA512Hash.h"
#include "CX/Status.h"
#include "../../Contrib/SHA/Include/sha2.h"


namespace CX
{

namespace Hash
{

const Char SHA512Hash::NAME[] = "SHA512";

SHA512Hash::SHA512Hash()
{
	Status::Clear();

	if (NULL == (m_pCTX = New<SHA512_CTX>()))
	{
		Status::Set(Status_MemAllocFailed, "Failed to allocate context");
		return;
	}

	Init();
}

SHA512Hash::~SHA512Hash()
{
	Status::Clear();

	Delete((SHA512_CTX *)m_pCTX);
}

const Char *SHA512Hash::GetName()
{
	Status::Clear();

	return NAME;
}

Size SHA512Hash::GetSize()
{
	Status::Clear();

	return SIZE;
}

StatusCode SHA512Hash::Init(const void *pHash/* = NULL*/)
{
	(void)(pHash);

	Status::Clear();

	if (NULL == m_pCTX)
	{
		return Status::Set(Status_NotInitialized, "Context not initialized");
	}

	SHA512_Init((SHA512_CTX *)m_pCTX);

	return Status_OK;
}

StatusCode SHA512Hash::Update(const void *pBuffer, Size cbSize)
{
	Status::Clear();

	if (NULL == m_pCTX)
	{
		return Status::Set(Status_NotInitialized, "Context not initialized");
	}

	SHA512_Update((SHA512_CTX *)m_pCTX, (const u_int8_t *)pBuffer, cbSize);

	return Status_OK;
}

StatusCode SHA512Hash::Done(void *pHash)
{
	Status::Clear();

	if (NULL == m_pCTX)
	{
		return Status::Set(Status_NotInitialized, "Context not initialized");
	}

	SHA512_Final((u_int8_t *)pHash, (SHA512_CTX *)m_pCTX);

	return Status_OK;
}

}//namespace Hash

}//namespace CX

