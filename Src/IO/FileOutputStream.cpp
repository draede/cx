
#pragma once


#include "CX/IO/FileOutputStream.h"
#include "CX/Status.h"


namespace CX
{

namespace IO
{

FileOutputStream::FileOutputStream(const Char *szPath)
{
	Status::Clear();

#pragma warning(push)
#pragma warning(disable: 4996)
	if (NULL == (m_pFile = cx_fopen(szPath, CX_FILE_WFLAGS)))
#pragma warning(pop)
	{
		Status::Set(Status_OpenFailed, "fopen failed with error {1}", errno);

		return;
	}
	m_sPath = szPath;
}

FileOutputStream::~FileOutputStream()
{
	Status::Clear();

	if (NULL != m_pFile)
	{
		fclose(m_pFile);
		m_pFile = NULL;
	}
}

StatusCode FileOutputStream::Write(const void *pBuffer, Size cbReqSize, Size *pcbAckSize)
{
	Status::Clear();

	if (NULL == m_pFile)
	{
		return Status::Set(Status_NotInitialized, "File not opened");
	}
	*pcbAckSize = fwrite(pBuffer, 1, cbReqSize, m_pFile);
	if (ferror(m_pFile))
	{
		return Status::Set(Status_WriteFailed, "fread failed with error {1}", errno);
	}
	if (0 == *pcbAckSize)
	{
		return Status_EOF;
	}

	return Status_OK;
}

StatusCode FileOutputStream::GetSize(UInt64 *pcbSize) const
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

bool FileOutputStream::IsOK() const
{
	Status::Clear();

	return (NULL != m_pFile);
}

const Char *FileOutputStream::GetPath() const
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

