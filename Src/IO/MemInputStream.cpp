
#include "CX/IO/MemInputStream.h"
#include "CX/Status.h"


namespace CX
{

namespace IO
{

MemInputStream::MemInputStream(const void *pMem, Size cbSize)
{
	Status::Clear();

	m_nType		= Type_Mem;
	m_pMem		= pMem;
	m_cbMemSize	= cbSize;
	m_cbOffset	= 0;
}

MemInputStream::MemInputStream(const String *pStr)
{
	Status::Clear();

	m_nType		= Type_String;
	m_pStr		= pStr;
	m_cbOffset	= 0;
}

MemInputStream::MemInputStream(const Util::IMemPool *pMemPool)
{
	Status::Clear();

	m_nType		= Type_MemPool;
	m_pMemPool	= pMemPool;
	m_cbOffset	= 0;
}

MemInputStream::~MemInputStream()
{
	Status::Clear();
}

Size MemInputStream::GetSizeImpl() const
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

const void *MemInputStream::GetMemImpl(Size cbOffset) const
{
	Status::Clear();

	switch (m_nType)
	{
		case Type_Mem:			return (const Byte *)m_pMem + cbOffset;
		case Type_String:		return m_pStr->c_str() + cbOffset;
		case Type_MemPool:	return (const Byte *)m_pMemPool->GetMem() + cbOffset;
	}

	return 0;
}

StatusCode MemInputStream::Read(void *pBuffer, Size cbReqSize, Size *pcbAckSize)
{
	Status::Clear();

	if (cbReqSize > GetSizeImpl() - m_cbOffset)
	{
		*pcbAckSize = GetSizeImpl() - m_cbOffset;
	}
	else
	{
		*pcbAckSize = cbReqSize;
	}
	if (0 < *pcbAckSize)
	{
		memcpy(pBuffer, GetMemImpl(m_cbOffset), *pcbAckSize);
		m_cbOffset += *pcbAckSize;
	}
	else
	{
		return Status::Set(Status_EOF, "End of stream reached");
	}

	return Status_OK;
}

StatusCode MemInputStream::SetPos(UInt64 cbPos)
{
	Status::Clear();

	if (cbPos >= (UInt64)GetSizeImpl())
	{
		return Status::Set(Status_OutOfBounds, "Invalid offset");
	}

	return Status_OK;
}

StatusCode MemInputStream::GetPos(UInt64 *pcbPos) const
{
	Status::Clear();

	*pcbPos = m_cbOffset;

	return Status_OK;
}

StatusCode MemInputStream::GetSize(UInt64 *pcbSize) const
{
	Status::Clear();

	*pcbSize = GetSizeImpl();

	return Status_OK;
}

bool MemInputStream::IsEOF() const
{
	Status::Clear();

	return (m_cbOffset >= GetSizeImpl());
}

bool MemInputStream::IsOK() const
{
	Status::Clear();

	return true;
}

const Char *MemInputStream::GetPath() const
{
	Status::Clear();

	return "";
}

}//namespace IO

}//namespace CX

