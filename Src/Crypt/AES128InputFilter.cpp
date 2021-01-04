/* 
 * CX - C++ framework for general purpose development
 *
 * https://github.com/draede/cx
 * 
 * Copyright (C) 2014 - 2021 draede - draede [at] outlook [dot] com
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
#include "CX/Crypt/AES128InputFilter.hpp"
#include "CX/Hash/BLAKE2Hash.hpp"
#include "CX/Util/RndGen.hpp"
#include "aes.h"


namespace CX
{

namespace Crypt
{

AES128InputFilter::AES128InputFilter(const void *pKey, Size cbKeySize/* = 0*/)
{
	Byte   key[KEY_SIZE_256];

	if (0 == cbKeySize)
	{
		cbKeySize = cx_strlen((const Char *)pKey);
	}
	if (cbKeySize <= KEY_SIZE_128)
	{
		m_cKeySize  = 128;

		Hash::BLAKE2Hash<KEY_SIZE_128> hash;

		hash.Init();
		hash.Update(pKey, cbKeySize);
		hash.Done(key);
	}
	else
	if (cbKeySize <= KEY_SIZE_192)
	{
		m_cKeySize  = 192;

		Hash::BLAKE2Hash<KEY_SIZE_192> hash;

		hash.Init();
		hash.Update(pKey, cbKeySize);
		hash.Done(key);
	}
	else
	{
		m_cKeySize  = 256;

		Hash::BLAKE2Hash<KEY_SIZE_256> hash;

		hash.Init();
		hash.Update(pKey, cbKeySize);
		hash.Done(key);
	}

	aes_key_setup(key, m_key, m_cKeySize);

	m_nState = State_IV;
}

AES128InputFilter::~AES128InputFilter()
{
}

Status AES128InputFilter::Filter(const void *pInput, Size cbInputSize, Size cbOrigInputSize, void **ppOutput, Size *pcbOutputSize)
{
	Size   cbOffset;
	Status status;

	cbOffset = 0;
	if (State_IV == m_nState)
	{
		aes_decrypt((const unsigned char *)pInput, m_iv, m_key, m_cKeySize);
		m_nState = State_Begin;
		cbOffset += BLOCK_SIZE;
	}
	if (0 != (cbInputSize % BLOCK_SIZE))
	{
		return Status_InvalidArg;
	}
	if (m_buffer.GetSize() < cbInputSize - cbOffset)
	{
		if (!(status = m_buffer.SetSize(cbInputSize - cbOffset)))
		{
			return status;
		}
	}

	String sHex1;
	String sHex2;

	*pcbOutputSize = 0;
	while (cbOffset < cbInputSize)
	{
		aes_decrypt_cbc((const unsigned char *)pInput + cbOffset, BLOCK_SIZE, (unsigned char *)m_buffer.GetMem() + *pcbOutputSize, 
		                m_key,  m_cKeySize, m_iv);
		memcpy(m_iv, (unsigned char *)m_buffer.GetMem() + *pcbOutputSize, BLOCK_SIZE);
		if (State_Begin == m_nState)
		{
			m_nState = State_Body;
		}
		*pcbOutputSize += BLOCK_SIZE;
		cbOffset += BLOCK_SIZE;
	}
	*ppOutput = m_buffer.GetMem();
	if (cbOrigInputSize > *pcbOutputSize)
	{
		return Status_OperationFailed;
	}
	*pcbOutputSize = cbOrigInputSize;

	return Status();
}

}//namespace Crypt

}//namespace CX
