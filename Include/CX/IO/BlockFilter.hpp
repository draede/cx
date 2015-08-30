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


#include "CX/IO/IFilter.hpp"
#include "CX/APIDefs.hpp"


namespace CX
{

namespace IO
{

class CX_API IBlockFilterHandler
{
public:

	virtual ~IBlockFilterHandler() { }

	virtual Size GetBlockSize() = 0;

	//pIn & pOut have cbSize size, cbSize is multiple of GetBlockSize()
	virtual Status OnBlocks(Size cbSize, const void *pIn, void *pOut) = 0;

};

class CX_API BlockFilter : public IFilter
{
public:

	enum Dir
	{
		Dir_Encode,
		Dir_Decode,
	};

	BlockFilter(Dir nDir, IBlockFilterHandler *pHandler);

	~BlockFilter();

	virtual Status Filter(Buffers *pBuffers);

	virtual Status Reset();

private:

	enum Mode
	{
		Mode_In,
		Mode_Out,
		Mode_InOut,
		Mode_Error,
		Mode_Finished,
	};

	Dir                   m_nDir;
	Mode                  m_nMode;
	IBlockFilterHandler   *m_pHandler;
	Size                  m_cbBlockSize;

	Byte                  *m_pInBuffer;
	Size                  m_cbInBufferSize;

	Byte                  *m_pOutBuffer;
	Size                  m_cbOutBufferSize;
	Size                  m_cbOutBufferOffset;

	Size                  m_cbReceivedBytes;
	Size                  m_cbSentBytes;

	void End();

};

}//namespace IO

}//namespace CX
