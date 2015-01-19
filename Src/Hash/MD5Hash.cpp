
#include "CX/Hash/MD5Hash.h"
#include "CX/Status.h"
#include "../../Contrib/MD5/Include/md5.h"


namespace CX
{

namespace Hash
{

const Char MD5Hash::NAME[] = "MD5";

MD5Hash::MD5Hash()
{
	Status::Clear();

	if (NULL == (m_pCTX = New<MD5_CTX>()))
	{
		Status::Set(Status_MemAllocFailed, "Failed to allocate context");
		return;
	}

	Init();
}

MD5Hash::~MD5Hash()
{
	Status::Clear();

	Delete((MD5_CTX *)m_pCTX);
}

const Char *MD5Hash::GetName()
{
	Status::Clear();

	return NAME;
}

Size MD5Hash::GetSize()
{
	Status::Clear();

	return SIZE;
}

StatusCode MD5Hash::Init(const void *pHash/* = NULL*/)
{
	(void)(pHash);

	Status::Clear();

	if (NULL == m_pCTX)
	{
		return Status::Set(Status_NotInitialized, "Context not initialized");
	}

	MD5Init((MD5_CTX *)m_pCTX);

	return Status_OK;
}

StatusCode MD5Hash::Update(const void *pBuffer, Size cbSize)
{
	Status::Clear();

	if (NULL == m_pCTX)
	{
		return Status::Set(Status_NotInitialized, "Context not initialized");
	}

	MD5Update((MD5_CTX *)m_pCTX, (unsigned char *)pBuffer, (unsigned int)cbSize);

	return Status_OK;
}

StatusCode MD5Hash::Done(void *pHash)
{
	Status::Clear();

	if (NULL == m_pCTX)
	{
		return Status::Set(Status_NotInitialized, "Context not initialized");
	}

	MD5Final((MD5_CTX *)m_pCTX);

	memcpy(pHash, ((MD5_CTX *)m_pCTX)->digest, SIZE);

	return Status_OK;
}

}//namespace Hash

}//namespace CX

