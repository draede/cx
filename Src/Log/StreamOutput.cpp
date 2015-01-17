
#include "CX/Log/StreamOutput.h"
#include "CX/Alloc.h"
#include "CX/Status.h"


namespace CX
{

namespace Log
{

StreamOutput::StreamOutput(IO::IOutputStream *pOutputStream)
{
	Status::Clear();

	m_pOutputStream = pOutputStream;
}

StreamOutput::~StreamOutput()
{
	Status::Clear();

	if (NULL != m_pOutputStream)
	{
		Delete(m_pOutputStream);
	}
}

StatusCode StreamOutput::Write(Level nLevel, const Char *szTag, const Char *pBuffer, Size cLen)
{
	Status::Clear();

	if (NULL == m_pOutputStream)
	{
		return Status::Set(Status_NotInitialized, "No valid stream set");
	}

	Size cbAckSize;

	if (CXNOK(m_pOutputStream->Write(pBuffer, cLen, &cbAckSize)))
	{
		return Status::GetCode();
	}

	return Status_OK;
}

}//namespace Log

}//namespace CX

