/* 
 * CX - C++ framework for general purpose development
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

#include "CX/Hash/SHA256Hash.hpp"
#include "CX/Status.hpp"
#include "../../Contrib/SHA/Include/sha2.h"


namespace CX
{

namespace Hash
{

const Char SHA256Hash::NAME[] = "SHA256";

SHA256Hash::SHA256Hash()
{
	if (NULL == (m_pCTX = new SHA256_CTX()))
	{
		return;
	}

	Init();
}

SHA256Hash::~SHA256Hash()
{
	delete (SHA256_CTX *)m_pCTX;
}

const Char *SHA256Hash::GetName()
{
	return NAME;
}

Size SHA256Hash::GetSize()
{
	return SIZE;
}

Status SHA256Hash::Init(const void *pHash/* = NULL*/)
{
	(void)(pHash);

	if (NULL == m_pCTX)
	{
		return Status(Status_NotInitialized, "Context not initialized");
	}

	SHA256_Init((SHA256_CTX *)m_pCTX);

	return Status();
}

Status SHA256Hash::Update(const void *pBuffer, Size cbSize)
{
	if (NULL == m_pCTX)
	{
		return Status(Status_NotInitialized, "Context not initialized");
	}

	SHA256_Update((SHA256_CTX *)m_pCTX, (const u_int8_t *)pBuffer, cbSize);

	return Status();
}

Status SHA256Hash::Done(void *pHash)
{
	if (NULL == m_pCTX)
	{
		return Status(Status_NotInitialized, "Context not initialized");
	}

	SHA256_Final((u_int8_t *)pHash, (SHA256_CTX *)m_pCTX);

	return Status();
}

}//namespace Hash

}//namespace CX

