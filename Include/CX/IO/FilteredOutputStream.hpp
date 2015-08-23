
#pragma once


#include "CX/IO/IOutputStream.hpp"
#include "CX/IO/IFilter.hpp"


namespace CX
{

namespace IO
{

class FilteredOutputStream : public IOutputStream
{
public:

	FilteredOutputStream(IFilter *pFilter, IOutputStream *pOutputStream);

	~FilteredOutputStream();

	virtual Status Write(const void *pBuffer, Size cbReqSize, Size *pcbAckSize);

	virtual Status GetSize(UInt64 *pcbSize) const;

	virtual Bool IsOK() const;

	virtual const Char *GetPath() const;

	Status Flush();

private:

	static const Size OUTPUT_BUFFER_SIZE = 65536;

	IFilter          *m_pFilter;
	IOutputStream    *m_pOutputStream;
	void             *m_pOutBuffer;
	UInt32           m_cbOutBufferSize;
	IFilter::Buffers m_buffers;

};

}//namespace IO

}//namespace CX
