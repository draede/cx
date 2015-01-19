
#include "CX/Hash/SHA1Hash.h"
#include "CX/Status.h"
#include "../../Contrib/SHA/Include/sha.h"


namespace CX
{

namespace Hash
{

const Char SHA1Hash::NAME[] = "SHA1";

SHA1Hash::SHA1Hash()
{
	Status::Clear();

	if (NULL == (m_pCTX = New<SHA_CTX>()))
	{
		Status::Set(Status_MemAllocFailed, "Failed to allocate context");
		return;
	}

	Init();
}

SHA1Hash::~SHA1Hash()
{
	Status::Clear();

	Delete((SHA_CTX *)m_pCTX);
}

const Char *SHA1Hash::GetName()
{
	Status::Clear();

	return NAME;
}

Size SHA1Hash::GetSize()
{
	Status::Clear();

	return SIZE;
}

StatusCode SHA1Hash::Init(const void *pHash/* = NULL*/)
{
	(void)(pHash);

	Status::Clear();

	if (NULL == m_pCTX)
	{
		return Status::Set(Status_NotInitialized, "Context not initialized");
	}

	SHA1_Init((SHA_CTX *)m_pCTX);

	return Status_OK;
}

StatusCode SHA1Hash::Update(const void *pBuffer, Size cbSize)
{
	Status::Clear();

	if (NULL == m_pCTX)
	{
		return Status::Set(Status_NotInitialized, "Context not initialized");
	}

	SHA1_Update((SHA_CTX *)m_pCTX, (sha1_byte *)pBuffer, (unsigned int)cbSize);

	return Status_OK;
}

StatusCode SHA1Hash::Done(void *pHash)
{
	Status::Clear();

	if (NULL == m_pCTX)
	{
		return Status::Set(Status_NotInitialized, "Context not initialized");
	}

	SHA1_Final((sha1_byte *)pHash, (SHA_CTX *)m_pCTX);

	return Status_OK;
}

}//namespace Hash

}//namespace CX

