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
 
#pragma once


#include "CX/IO/BlockFilter.hpp"
#include "CX/APIDefs.hpp"


namespace CX
{

namespace Crypt
{

class CX_API AES128Filter : public IObject, public IO::IFilter, private IO::IBlockFilterHandler
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

	virtual Status Reset();

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
