/* 
 * CX - C++ framework for general purpose development
 *
 * https://github.com/draede/cx
 * 
 * Copyright (C) 2014 - 2017 draede - draede [at] outlook [dot] com
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
#include "CX/Util/MemPool.hpp"
#include "CX/IO/IInputFilter.hpp"


namespace CX
{

namespace Crypt
{

class CX_API AES128InputFilter : public IO::IInputFilter
{
public:

	static const Size BLOCK_SIZE       = 16;
	static const Size KEY_SIZE_128     = 16;
	static const Size KEY_SIZE_192     = 24;
	static const Size KEY_SIZE_256     = 32;

	AES128InputFilter(const void *pKey, Size cbKeySize = 0);

	~AES128InputFilter();

	virtual Status Filter(const void *pInput, Size cbInputSize, Size cbOrigInputSize, void **ppOutput, Size *pcbOutputSize);

private:

	enum State
	{
		State_IV,
		State_Begin,
		State_Body,
	};
	
	unsigned int       m_key[60];
	int                m_cKeySize;
	Byte               m_iv[BLOCK_SIZE];
	Util::DynMemPool   m_buffer;
	State              m_nState;

};

}//namespace Crypt

}//namespace CX
