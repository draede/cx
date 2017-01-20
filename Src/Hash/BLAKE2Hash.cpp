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

#include "CX/Hash/BLAKE2Hash.hpp"
#include "CX/Status.hpp"
#include "CX/Mem.hpp"
#pragma warning(push)
#pragma warning(disable: 4324 4245)
#include "../../Contrib/BLAKE2/Include/blake2.h"
#pragma warning(pop)


namespace CX
{

namespace Hash
{

const Char BLAKE2HashHelper::NAME[] = "BLAKE2";

void *BLAKE2HashHelper::CreateState()
{
	blake2b_state *pB2S = (blake2b_state *)Mem::Alloc(sizeof(blake2b_state));

	return pB2S;
}

void BLAKE2HashHelper::DestroyState(void *pState)
{
	blake2b_state *pB2S = (blake2b_state *)pState;

	Mem::Free(pB2S);
}

Status BLAKE2HashHelper::Init(void *pState, Size cbSize, const void *pSeed/* = NULL*/)
{
	blake2b_state *pB2S = (blake2b_state *)pState;
	int           nRes; 

	if (NULL == pSeed)
	{
		nRes = blake2b_init(pB2S, (uint8_t)cbSize);
	}
	else
	{
		nRes = blake2b_init_key(pB2S, (uint8_t)cbSize, pSeed, (uint8_t)cbSize);
	}
	if (0 != nRes)
	{
		return Status(Status_InvalidArg);
	}

	return Status();
}

Status BLAKE2HashHelper::Update(void *pState, const void *pBuffer, Size cbSize)
{
	blake2b_state *pB2S = (blake2b_state *)pState;
	int           nRes;

	nRes = blake2b_update(pB2S, (const uint8_t *)pBuffer, cbSize);
	if (0 != nRes)
	{
		return Status(Status_InvalidArg);
	}

	return Status();
}

Status BLAKE2HashHelper::Done(void *pState, void *pCrypt, Size cbSize)
{
	blake2b_state *pB2S = (blake2b_state *)pState;
	int           nRes;

	nRes = blake2b_final(pB2S, (uint8_t *)pCrypt, (uint8_t)cbSize);
	if (0 != nRes)
	{
		return Status(Status_InvalidArg);
	}

	return Status();
}

}//namespace Hash

}//namespace CX

