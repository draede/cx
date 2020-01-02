/* 
 * CX - C++ framework for general purpose development
 *
 * https://github.com/draede/cx
 * 
 * Copyright (C) 2014 - 2020 draede - draede [at] outlook [dot] com
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


namespace CX
{

namespace Hash
{

class CX_API BLAKE2HashHelper
{
public:
        
    static const Char     NAME[];
    
    static void *CreateState();
        
    static void DestroyState(void *pState);
        
    static Status Init(void *pState, Size cbSize, const void *pSeed = NULL);
        
    static Status Update(void *pState, const void *pBuffer, Size cbSize);
        
    static Status Done(void *pState, void *pCrypt, Size cbSize);
        
};
    
template <Size HASHSIZE = 16>
class CX_API BLAKE2Hash : public IHash
{
public:

	static const Size     SIZE         = HASHSIZE;

	static const Size     MIN_HASHSIZE = 1;
	static const Size     MAX_HASHSIZE = 64;

	BLAKE2Hash()
	{
		m_pState = BLAKE2HashHelper::CreateState();
	}

	virtual ~BLAKE2Hash()
	{
		BLAKE2HashHelper::DestroyState(m_pState);
	}

	virtual const Char *GetName()
	{
		return BLAKE2HashHelper::NAME;
	}

	virtual Size GetSize()
	{
		return HASHSIZE;
	}

	virtual Status Init(const void *pCrypt = NULL)
	{
		return BLAKE2HashHelper::Init(m_pState, HASHSIZE, pCrypt);
	}

	virtual Status Update(const void *pBuffer, Size cbSize)
	{
		return BLAKE2HashHelper::Update(m_pState, pBuffer, cbSize);
	}

	virtual Status Done(void *pCrypt)
	{
		return BLAKE2HashHelper::Done(m_pState, pCrypt, HASHSIZE);
	}

private:

	void *m_pState;

};

}//namespace Hash

}//namespace CX

