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
 
#include "CX/Crypt/AES128Filter.hpp"
#include "CX/Hash/BLAKE2Hash.hpp"
#include "CX/Util/RndGen.hpp"
#include "aes.h"


namespace CX
{

namespace Crypt
{

AES128Filter::AES128Filter(Dir nDir, const void *pKey, Size cbKeySize/* = 0*/)
	: m_blkflt(Dir_Encrypt == nDir ? IO::BlockFilter::Dir_Encode : IO::BlockFilter::Dir_Decode, this)
{
	m_nDir = nDir;

	if (0 == cbKeySize)
	{
		cbKeySize = cx_strlen((const Char *)pKey);
	}

	Hash::BLAKE2Hash<KEY_SIZE> hash;

	hash.Init();
	hash.Update(pKey, cbKeySize);
	hash.Done(m_key);
	
	Reset();
}

AES128Filter::~AES128Filter()
{
}

Status AES128Filter::InitEncrypt()
{
	Size cbIVOffset = 0;

	while (cbIVOffset < BLOCK_SIZE)
	{
		UInt64 nRand = Util::RndGen::Get().GetUInt64();
		Size   cbSize = BLOCK_SIZE - cbIVOffset;

		if (cbSize > sizeof(nRand))
		{
			cbSize = sizeof(nRand);
		}
		memcpy(m_iv + cbIVOffset, &nRand, cbSize);
		cbIVOffset += cbSize;
	}

	AES128_ECB_encrypt(m_iv, m_key, m_buffer);

	m_nMode    = Mode_Header;
	m_cbOffset = 0;

	return Status();
}

Status AES128Filter::InitDecrypt()
{
	m_nMode    = Mode_Header;
	m_cbOffset = 0;

	return Status();
}

Size AES128Filter::GetBlockSize()
{
	return BLOCK_SIZE;
}

Status AES128Filter::OnBlocks(Size cbSize, const void *pIn, void *pOut)
{
	Byte *pInput  = (Byte *)pIn;
	Byte *pOutput = (Byte *)pOut;

	if (Dir_Encrypt == m_nDir)
	{
		while (0 < cbSize)
		{
			AES128_CBC_encrypt_buffer(pOutput, pInput, BLOCK_SIZE, m_key, m_bFirstBlock ? m_iv : NULL);
			//memcpy(pOutput, pInput, BLOCK_SIZE);
			cbSize -= BLOCK_SIZE;
			pInput += BLOCK_SIZE;
			pOutput += BLOCK_SIZE;
			m_bFirstBlock = false;
		}
	}
	else
	{
		while (0 < cbSize)
		{
			AES128_CBC_decrypt_buffer(pOutput, pInput, BLOCK_SIZE, m_key, m_bFirstBlock ? m_iv : NULL);
			//memcpy(pOutput, pInput, BLOCK_SIZE);
			cbSize -= BLOCK_SIZE;
			pInput += BLOCK_SIZE;
			pOutput += BLOCK_SIZE;
			m_bFirstBlock = false;
		}
	}

	return Status();
}

Status AES128Filter::Filter(Buffers *pBuffers)
{
	if (Dir_Encrypt == m_nDir)
	{
		return FilterEncrypt(pBuffers);
	}
	else
	{
		return FilterDecrypt(pBuffers);
	}
}

Status AES128Filter::FilterEncrypt(Buffers *pBuffers)
{
	if (Mode_Header == m_nMode)
	{
		Size cbSize;

		cbSize = BLOCK_SIZE - m_cbOffset;
		if (cbSize > pBuffers->cbInSize)
		{
			cbSize = pBuffers->cbInSize;
		}
		memcpy(pBuffers->pOutBuffer, m_buffer + m_cbOffset, cbSize);
		m_cbOffset += cbSize;
		pBuffers->cbOutSize -= cbSize;
		pBuffers->pOutBuffer = (Byte *)pBuffers->pOutBuffer + cbSize;
		if (BLOCK_SIZE == m_cbOffset)
		{
			m_nMode = Mode_Body;
		}
		else
		{
			pBuffers->nState = State_Continue;

			return Status();
		}
	}
	if (Mode_Body == m_nMode)
	{
		return m_blkflt.Filter(pBuffers);
	}

	return Status();
}

Status AES128Filter::FilterDecrypt(Buffers *pBuffers)
{
	if (Mode_Header == m_nMode)
	{
		Size cbSize;

		cbSize = BLOCK_SIZE - m_cbOffset;
		if (cbSize > pBuffers->cbInSize)
		{
			cbSize = pBuffers->cbInSize;
		}
		memcpy(m_buffer + m_cbOffset, pBuffers->pInBuffer, cbSize);
		m_cbOffset += cbSize;
		pBuffers->cbInSize -= cbSize;
		pBuffers->pInBuffer = (const Byte *)pBuffers->pInBuffer + cbSize;
		if (BLOCK_SIZE == m_cbOffset)
		{
			AES128_ECB_decrypt(m_buffer, m_key, m_iv);
			m_nMode = Mode_Body;
		}
		else
		{
			pBuffers->nState = State_Continue;

			return Status();
		}
	}
	if (Mode_Body == m_nMode)
	{
		return m_blkflt.Filter(pBuffers);
	}

	return Status();
}

Status AES128Filter::Reset()
{
	Status status;

	m_bFirstBlock = true;

	if (Dir_Encrypt == m_nDir)
	{
		if (!(status = InitEncrypt()))
		{
			return status;
		}

		return m_blkflt.Reset();
	}
	else
	if (Dir_Decrypt == m_nDir)
	{
		if (!(status = InitDecrypt()))
		{
			return status;
		}

		return m_blkflt.Reset();
	}
	else
	{
		return Status_InvalidArg;
	}
}

}//namespace Crypt

}//namespace CX
