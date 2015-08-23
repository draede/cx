
#pragma once


#include "CX/IO/BlockFilter.hpp"


namespace CX
{

namespace Crypt
{

class AES128Filter : public IO::IFilter, private IO::IBlockFilterHandler
{
public:

	static const Size BLOCK_SIZE      = 16;
	static const Size KEY_SIZE        = 16;

	enum Dir
	{
		Dir_Encrypt,
		Dir_Decrypt,
	};

	AES128Filter(Dir nDir, const void *pKey, Size cbKeySize = 0);

	~AES128Filter();

	virtual Status Filter(Buffers *pBuffers);

private:

	enum Mode
	{
		Mode_Header,
		Mode_Body,
	};

	Mode              m_nMode;
	Dir               m_nDir;
	Byte              m_key[KEY_SIZE];
	Byte              m_iv[BLOCK_SIZE];
	Byte              m_buffer[BLOCK_SIZE];
	Size              m_cbOffset;
	IO::BlockFilter   m_blkflt;
	bool              m_bFirstBlock;

	Status InitEncrypt();
	
	Status InitDecrypt();

	Status FilterEncrypt(Buffers *pBuffers);

	Status FilterDecrypt(Buffers *pBuffers);

	virtual Size GetBlockSize();

	virtual Status OnBlocks(Size cbSize, const void *pIn, void *pOut);

};

}//namespace Crypt

}//namespace CX
