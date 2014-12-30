
#pragma once


#include "CX/IO/MemOutputStream.h"
#include "CX/Status.h"


namespace CX
{

namespace IO
{

MemOutputStream::MemOutputStream(void *pMem, Size cbSize)
{
	Status::Clear();
}

MemOutputStream::MemOutputStream(String *pStr)
{
	Status::Clear();
}

MemOutputStream::MemOutputStream(Util::IMemPool *pMemPool)
{
	Status::Clear();

}

MemOutputStream::~MemOutputStream()
{
	Status::Clear();
}

Size MemOutputStream::GetSizeImpl() const
{
	Status::Clear();

	switch(m_nType)
	{
		case Type_Mem:			return m_cbMemSize;
		case Type_String:		return m_pStr->size();
		case Type_MemPool:	return m_pMemPool->GetSize();
	}

	return 0;
}

StatusCode MemOutputStream::WriteImpl(const void *pMem, Size cbReqSize, Size *pcbAckSize)
{
	Status::Clear();

	switch(m_nType)
	{
		case Type_Mem:
		{
			if (m_cbOffset + cbReqSize > m_cbMemSize)
			{
				*pcbAckSize = m_cbMemSize - m_cbOffset;
			}
			else
			{
				*pcbAckSize = cbReqSize;
			}
			if (0 < *pcbAckSize)
			{
				memcpy((Byte *)m_pMem + m_cbOffset, pMem, *pcbAckSize);
				m_cbOffset += *pcbAckSize;

				return Status_OK;
			}
			else
			{
				return Status::Set(Status_WriteFailed, "Max capacity reached");
			}
		}
		break;
		case Type_String:
		{
			m_pStr->append((const Char *)pMem, cbReqSize);
			*pcbAckSize = cbReqSize;

			return Status_OK;
		}
		break;
		case Type_MemPool:
		{
			if (CXOK(m_pMemPool->Add(pMem, cbReqSize)))
			{
				*pcbAckSize = cbReqSize;

				return Status_OK;
			}
			else
			{
				return Status::GetCode();
			}
		}
		break;
	}

	return Status::Set(Status_NotSupported, "Not supported");
}

StatusCode MemOutputStream::Write(const void *pBuffer, Size cbReqSize, Size *pcbAckSize)
{
	Status::Clear();

	return WriteImpl(pBuffer, cbReqSize, pcbAckSize);
}

StatusCode MemOutputStream::GetSize(UInt64 *pcbSize) const
{
	Status::Clear();

	*pcbSize = (UInt64)GetSizeImpl();

	return Status_OK;
}

bool MemOutputStream::IsOK() const
{
	Status::Clear();

	return true;
}

const Char *MemOutputStream::GetPath() const
{
	Status::Clear();

	return "";
}

}//namespace IO

}//namespace CX

