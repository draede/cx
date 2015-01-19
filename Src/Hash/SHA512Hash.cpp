
#include "CX/Hash/SHA512Hash.h"
#include "CX/Status.h"
#include "../../Contrib/SHA/Include/sha2.h"


namespace CX
{

namespace Hash
{

const Char SHA512Hash::NAME[] = "SHA512";

SHA512Hash::SHA512Hash()
{
	Status::Clear();

	if (NULL == (m_pCTX = New<SHA512_CTX>()))
	{
		Status::Set(Status_MemAllocFailed, "Failed to allocate context");
		return;
	}

	Init();
}

SHA512Hash::~SHA512Hash()
{
	Status::Clear();

	Delete((SHA512_CTX *)m_pCTX);
}

const Char *SHA512Hash::GetName()
{
	Status::Clear();

	return NAME;
}

Size SHA512Hash::GetSize()
{
	Status::Clear();

	return SIZE;
}

StatusCode SHA512Hash::Init(const void *pHash/* = NULL*/)
{
	(void)(pHash);

	Status::Clear();

	if (NULL == m_pCTX)
	{
		return Status::Set(Status_NotInitialized, "Context not initialized");
	}

	SHA512_Init((SHA512_CTX *)m_pCTX);

	return Status_OK;
}

StatusCode SHA512Hash::Update(const void *pBuffer, Size cbSize)
{
	Status::Clear();

	if (NULL == m_pCTX)
	{
		return Status::Set(Status_NotInitialized, "Context not initialized");
	}

	SHA512_Update((SHA512_CTX *)m_pCTX, (const u_int8_t *)pBuffer, cbSize);

	return Status_OK;
}

StatusCode SHA512Hash::Done(void *pHash)
{
	Status::Clear();

	if (NULL == m_pCTX)
	{
		return Status::Set(Status_NotInitialized, "Context not initialized");
	}

	SHA512_Final((u_int8_t *)pHash, (SHA512_CTX *)m_pCTX);

	return Status_OK;
}

}//namespace Hash

}//namespace CX

