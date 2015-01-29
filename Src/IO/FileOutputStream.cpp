/* 
 * CX - C++ framework for general purpose developement
 *
 * https://github.com/draede/cx
 * 
 * Copyright (C) 2014 draede - draede [at] outlook [dot] com
 *
 * Released under the MIT License.
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */ 

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

