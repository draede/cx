
#include "CX/IO/FileInputStream.h"
#include "CX/Status.h"
#include <sys/types.h>
#include <sys/stat.h>


namespace CX
{

namespace IO
{

FileInputStream::FileInputStream(const Char *szPath)
{
	Status::Clear();

#pragma warning(push)
#pragma warning(disable: 4996)
	if (NULL == (m_pFile = cx_fopen(szPath, CX_FILE_RFLAGS)))
#pragma warning(pop)
	{
		Status::Set(Status_OpenFailed, "fopen failed with error {1}", errno);

		return;
	}
	m_sPath = szPath;
}

FileInputStream::~FileInputStream()
{
	Status::Clear();

	if (NULL != m_pFile)
	{
		fclose(m_pFile);
		m_pFile = NULL;
	}
}

StatusCode FileInputStream::Read(void *pBuffer, Size cbReqSize, Size *pcbAckSize)
{
	Status::Clear();

	if (NULL == m_pFile)
	{
		return Status::Set(Status_NotInitialized, "File not opened");
	}
	*pcbAckSize = fread(pBuffer, 1, cbReqSize, m_pFile);
	if (ferror(m_pFile))
	{
		return Status::Set(Status_ReadFailed, "fread failed with error {1}", errno);
	}
	if (0 == *pcbAckSize)
	{
		return Status_EOF;
	}

	return Status_OK;
}

StatusCode FileInputStream::SetPos(UInt64 cbPos)
{
	Status::Clear();

	if (NULL == m_pFile)
	{
		return Status::Set(Status_NotInitialized, "File not opened");
	}
	if (0 != cx_fseek(m_pFile, cbPos, CX_SEEK_SET))
	{
		return Status::Set(Status_SetPos, "fseek failed with error {1}", errno);
	}

	return Status_OK;
}

StatusCode FileInputStream::GetPos(UInt64 *pcbPos) const
{
	Status::Clear();

	if (NULL == m_pFile)
	{
		return Status::Set(Status_NotInitialized, "File not opened");
	}
	*pcbPos = cx_ftell(m_pFile);
	if (ferror(m_pFile))
	{
		return Status::Set(Status_GetPos, "ftell failed with error {1}", errno);
	}

	return Status_OK;
}

StatusCode FileInputStream::GetSize(UInt64 *pcbSize) const
{
	Status::Clear();

	if (NULL == m_pFile)
	{
		return Status::Set(Status_NotInitialized, "File not opened");
	}

	cx_statstruct buf;

	if (0 != cx_fstat(cx_fileno(m_pFile), &buf))
	{
		return Status::Set(Status_GetSize, "fstat failed with error {1}", errno);
	}
	*pcbSize = buf.st_size;

	return Status_OK;
}

bool FileInputStream::IsEOF() const
{
	Status::Clear();

	if (NULL == m_pFile)
	{
		Status::Set(Status_NotInitialized, "File not opened");

		return true;
	}

	return (0 != feof(m_pFile));
}

bool FileInputStream::IsOK() const
{
	Status::Clear();

	return (NULL != m_pFile);
}

const Char *FileInputStream::GetPath() const
{
	Status::Clear();

	if (NULL == m_pFile)
	{
		Status::Set(Status_NotInitialized, "File not opened");

		return "";
	}

	return m_sPath.c_str();
}

}//namespace IO

}//namespace CX

