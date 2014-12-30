
#include "CX/Platform.h"


#if defined(CX_OS_WINDOWS)


#include "CX/Sys/DynLib.h"
#include "CX/Status.h"


namespace CX
{

namespace Sys
{

DynLib::DynLib()
{
	Status::Clear();

	m_hHandle = NULL;
}

DynLib::~DynLib()
{
	Status::Clear();

	Unload();
}

bool DynLib::IsOK()
{
	Status::Clear();

	return (NULL != m_hHandle);
}

StatusCode DynLib::Load(const Char *szPath)
{
	Unload();
	Status::Clear();

	if (NULL == (m_hHandle = LoadLibraryA(szPath)))
	{
		return Status::Set(Status_FileNotFound, "Failed to load library");
	}

	return Status_OK;
}

StatusCode DynLib::Unload()
{
	Status::Clear();

	if (NULL != m_hHandle)
	{
		FreeLibrary(m_hHandle);
	}
	
	return Status_OK;
}

void *DynLib::GetFunc(const Char *szName)
{
	Status::Clear();

	if (NULL == m_hHandle)
	{
		Status::Set(Status_NotInitialized, "");

		return NULL;
	}

	return GetProcAddress(m_hHandle, szName);
}


}//namespace Sys

}//namespace CX


#endif

