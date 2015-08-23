
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
	
	m_bFirstBlock = true;

	if (Dir_Encrypt == m_nDir)
	{
		InitEncrypt();
	}
	else
	{
		InitDecrypt();
	}
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
		pBuffers->cbOutSize -= (UInt32)cbSize;
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
		pBuffers->cbInSize -= (UInt32)cbSize;
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

}//namespace Crypt

}//namespace CX
