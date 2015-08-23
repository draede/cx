
#pragma once


#include "CX/Types.hpp"
#include "CX/Status.hpp"


namespace CX
{

namespace IO
{

class IFilter
{
public:

	enum State
	{
		State_Continue,
		State_Finish,
		State_Error,
	};

	struct Buffers
	{
		const void *pInBuffer;
		UInt32     cbInSize;
		UInt32     cbInTotalSize;
		void       *pOutBuffer;
		UInt32     cbOutSize;
		UInt32     cbOutTotalSize;
		bool       bHasMoreData;
		State      nState;
	};

	virtual ~IFilter() { }

	virtual Status Filter(Buffers *pBuffers) = 0;

};

}//namespace IO

}//namespace CX
