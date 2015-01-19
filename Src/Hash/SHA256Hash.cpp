
#include "CX/Hash/SHA256Hash.h"
#include "CX/Status.h"
#include "../../Contrib/SHA/Include/sha2.h"


namespace CX
{

namespace Hash
{

const Char SHA256Hash::NAME[] = "SHA256";

SHA256Hash::SHA256Hash()
{
	Status::Clear();

	if (NULL == (m_pCTX = New<SHA256_CTX>()))
	{
		Status::Set(Status_MemAllocFailed, "Failed to allocate context");
		return;
	}

	Init();
}

SHA256Hash::~SHA256Hash()
{
	Status::Clear();

	Delete((SHA256_CTX *)m_pCTX);
}

const Char *SHA256Hash::GetName()
{
	Status::Clear();

	return NAME;
}

Size SHA256Hash::GetSize()
{
	Status::Clear();

	return SIZE;
}

StatusCode SHA256Hash::Init(const void *pHash/* = NULL*/)
{
	(void)(pHash);

	Status::Clear();

	if (NULL == m_pCTX)
	{
		return Status::Set(Status_NotInitialized, "Context not initialized");
	}

	SHA256_Init((SHA256_CTX *)m_pCTX);

	return Status_OK;
}

StatusCode SHA256Hash::Update(const void *pBuffer, Size cbSize)
{
	Status::Clear();

	if (NULL == m_pCTX)
	{
		return Status::Set(Status_NotInitialized, "Context not initialized");
	}

	SHA256_Update((SHA256_CTX *)m_pCTX, (const u_int8_t *)pBuffer, cbSize);

	return Status_OK;
}

StatusCode SHA256Hash::Done(void *pHash)
{
	Status::Clear();

	if (NULL == m_pCTX)
	{
		return Status::Set(Status_NotInitialized, "Context not initialized");
	}

	SHA256_Final((u_int8_t *)pHash, (SHA256_CTX *)m_pCTX);

	return Status_OK;
}

}//namespace Hash

}//namespace CX

