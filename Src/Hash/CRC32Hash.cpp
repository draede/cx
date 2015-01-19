
#include "CX/Hash/CRC32Hash.h"
#include "CX/Status.h"
#include "../../Contrib/CRC32Slice8/Include/crc32.h"


namespace CX
{

namespace Hash
{

const Char CRC32Hash::NAME[] = "CRC32";

CRC32Hash::CRC32Hash()
{
	Status::Clear();

	Init();
}

CRC32Hash::~CRC32Hash()
{
	Status::Clear();
}

const Char *CRC32Hash::GetName()
{
	Status::Clear();

	return NAME;
}

Size CRC32Hash::GetSize()
{
	Status::Clear();

	return SIZE;
}

StatusCode CRC32Hash::Init(const void *pHash/* = NULL*/)
{
	Status::Clear();

	if (NULL != pHash)
	{
		m_nCRC32 = *((UInt32 *)pHash);
	}
	else
	{
		m_nCRC32 = 0;
	}

	return Status_OK;
}

StatusCode CRC32Hash::Update(const void *pBuffer, Size cbSize)
{
	Status::Clear();

	m_nCRC32 = crc32_8bytes(pBuffer, cbSize, m_nCRC32);

	return Status_OK;
}

StatusCode CRC32Hash::Done(void *pHash)
{
	Status::Clear();

	*((UInt32 *)pHash) = m_nCRC32;

	return Status_OK;
}

}//namespace Hash

}//namespace CX

