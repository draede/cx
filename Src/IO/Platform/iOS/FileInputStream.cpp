/* 
 * CX - C++ framework for general purpose development
 *
 * https://github.com/draede/cx
 * 
 * Copyright (C) 2014 - 2022 draede - draede [at] outlook [dot] com
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

#include "CX/precomp.hpp"
#include "CX/Platform.hpp"


#if defined(CX_OS_IOS)


#include "CX/IO/FileInputStream.hpp"
#include "CX/Str/UTF8.hpp"
#include "CX/Status.hpp"
#include "CX/C/stdio.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>


namespace CX
{

namespace IO
{

FileInputStream::FileInputStream(const Char *szPath)
{
	if (NULL == (m_pFile = cx_fopen(szPath, "rb")))
#pragma warning(pop)
	{
		return;
	}

	m_sPath = szPath;
}

FileInputStream::FileInputStream(const WChar *wszPath)
{
	String sPath;

	if (Str::UTF8::FromWChar(wszPath, &sPath).IsOK())
	{
#pragma warning(push)
#pragma warning(disable: 4996)
		if (NULL == (m_pFile = cx_fopen(sPath.c_str(), "rb")))
#pragma warning(pop)
		{
			return;
		}

		m_sPath = sPath;
	}
	else
	{
		m_pFile = NULL;
	}	
}

FileInputStream::~FileInputStream()
{
	if (NULL != m_pFile)
	{
		fclose(m_pFile);
		m_pFile = NULL;
	}
}

Status FileInputStream::Read(void *pBuffer, Size cbReqSize, Size *pcbAckSize)
{
	if (NULL == m_pFile)
	{
		return Status(Status_NotInitialized, "File not opened");
	}
	*pcbAckSize = fread(pBuffer, 1, cbReqSize, m_pFile);
	if (ferror(m_pFile))
	{
		return Status(Status_ReadFailed, "fread failed with error {1}", errno);
	}
	if (0 == *pcbAckSize)
	{
		return Status(Status_EOF, "End of stream reached");
	}

	return Status();
}

Status FileInputStream::SetPos(UInt64 cbPos)
{
	if (NULL == m_pFile)
	{
		return Status(Status_NotInitialized, "File not opened");
	}

	cx_fseek64(m_pFile, cbPos, SEEK_SET);
	if (ferror(m_pFile))
	{
		return Status(Status_GetPos, "cx_fseek64 failed with error {1}", errno);
	}

	return Status();
}

Status FileInputStream::GetPos(UInt64 *pcbPos) const
{
	if (NULL == m_pFile)
	{
		return Status(Status_NotInitialized, "File not opened");
	}

	*pcbPos = cx_ftell64(m_pFile);
	if (ferror(m_pFile))
	{
		return Status(Status_GetPos, "cx_ftell64 failed with error {1}", errno);
	}

	return Status();
}

Status FileInputStream::GetSize(UInt64 *pcbSize) const
{
	if (NULL == m_pFile)
	{
		return Status(Status_NotInitialized, "File not opened");
	}

	cx_stat64struct buf;

	if (0 != cx_stat64(cx_fileno(m_pFile), &buf))
	{
		return Status(Status_GetSize, "fstat failed with error {1}", errno);
	}
	*pcbSize = buf.st_size;

	return Status();
}

Bool FileInputStream::IsEOF() const
{
	if (NULL == m_pFile)
	{
		return True;
	}

	return (0 != feof(m_pFile));
}

Bool FileInputStream::IsOK() const
{
	return (NULL != m_pFile);
}

const Char *FileInputStream::GetPath() const
{
	if (NULL == m_pFile)
	{
		return "";
	}

	return m_sPath.c_str();
}

}//namespace IO

}//namespace CX


#endif

