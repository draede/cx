
#include "CX/Hash/SHA384Hash.h"
#include "CX/Status.h"
#include "../../Contrib/SHA/Include/sha2.h"


namespace CX
{

namespace Hash
{

const Char SHA384Hash::NAME[] = "SHA384";

SHA384Hash::SHA384Hash()
{
	Status::Clear();

	if (NULL == (m_pCTX = New<SHA384_CTX>()))
	{
		Status::Set(Status_MemAllocFailed, "Failed to allocate context");
		return;
	}

	Init();
}

SHA384Hash::~SHA384Hash()
{
	Status::Clear();

	Delete((SHA384_CTX *)m_pCTX);
}

const Char *SHA384Hash::GetName()
{
	Status::Clear();

	return NAME;
}

Size SHA384Hash::GetSize()
{
	Status::Clear();

	return SIZE;
}

StatusCode SHA384Hash::Init(const void *pHash/* = NULL*/)
{
	(void)(pHash);

	Status::Clear();

	if (NULL == m_pCTX)
	{
		return Status::Set(Status_NotInitialized, "Context not initialized");
	}

	SHA384_Init((SHA384_CTX *)m_pCTX);

	return Status_OK;
}

StatusCode SHA384Hash::Update(const void *pBuffer, Size cbSize)
{
	Status::Clear();

	if (NULL == m_pCTX)
	{
		return Status::Set(Status_NotInitialized, "Context not initialized");
	}

	SHA384_Update((SHA384_CTX *)m_pCTX, (const u_int8_t *)pBuffer, cbSize);

	return Status_OK;
}

StatusCode SHA384Hash::Done(void *pHash)
{
	Status::Clear();

	if (NULL == m_pCTX)
	{
		return Status::Set(Status_NotInitialized, "Context not initialized");
	}

	SHA384_Final((u_int8_t *)pHash, (SHA384_CTX *)m_pCTX);

	return Status_OK;
}

}//namespace Hash

}//namespace CX

