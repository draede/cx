
#pragma once


#include "CX/IO/IFilter.hpp"


namespace CX
{

namespace IO
{

class IBlockFilterHandler
{
public:

	virtual ~IBlockFilterHandler() { }

	virtual Size GetBlockSize() = 0;

	//pIn & pOut have cbSize size, cbSize is multiple of GetBlockSize()
	virtual Status OnBlocks(Size cbSize, const void *pIn, void *pOut) = 0;

};

class BlockFilter : public IFilter
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

};

}//namespace IO

}//namespace CX
