
#pragma once


#include "CX/IO/IInputStream.hpp"
#include "CX/IO/IFilter.hpp"


namespace CX
{

namespace IO
{

class FilteredInputStream : public IInputStream
{
public:

	FilteredInputStream(IFilter *pFilter, IInputStream *pInputStream);

	~FilteredInputStream();

	virtual Status Read(void *pBuffer, Size cbReqSize, Size *pcbAckSize);

	virtual Status SetPos(UInt64 cbPos);

	virtual Status GetPos(UInt64 *pcbPos) const;

	virtual Status GetSize(UInt64 *pcbSize) const;

	virtual Bool IsEOF() const;

	virtual Bool IsOK() const;

	virtual const Char *GetPath() const;

private:

	static const Size INPUT_BUFFER_SIZE = 65536;

	IFilter          *m_pFilter;
	IInputStream     *m_pInputStream;
	void             *m_pInBuffer;
	UInt32           m_cbBufferSize;
	UInt32           m_cbInBufferTotalSize;
	IFilter::Buffers m_buffers;

};

}//namespace IO

}//namespace CX
