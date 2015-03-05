/* 
 * CX - C++ framework for general purpose developement
 *
 * https://github.com/draede/cx
 * 
 * Copyright (C) 2014-2015 draede - draede [at] outlook [dot] com
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

#include "CX/IO/FileOutputStream.hpp"
#include "CX/Str/UTF8.hpp"
#include "CX/Status.hpp"


namespace CX
{

namespace IO
{

FileOutputStream::FileOutputStream(const Char *szPath)
{
	WString wsPath;

	if (Str::UTF8::ToWChar(szPath, &wsPath).IsOK())
	{
#pragma warning(push)
#pragma warning(disable: 4996)
		if (NULL == (m_pFile = cxw_fopen(wsPath.c_str(), CX_WFILE_WFLAGS)))
#pragma warning(pop)
		{
			return;
		}

		m_sPath = szPath;
	}
	else
	{
		m_pFile = NULL;
	}
}

FileOutputStream::FileOutputStream(const WChar *wszPath)
{
#pragma warning(push)
#pragma warning(disable: 4996)
	if (NULL == (m_pFile = cxw_fopen(wszPath, CX_WFILE_WFLAGS)))
#pragma warning(pop)
	{
		return;
	}

	Str::UTF8::FromWChar(wszPath, &m_sPath);
}

FileOutputStream::~FileOutputStream()
{
	if (NULL != m_pFile)
	{
		fclose(m_pFile);
		m_pFile = NULL;
	}
}

Status FileOutputStream::Write(const void *pBuffer, Size cbReqSize, Size *pcbAckSize)
{
	if (NULL == m_pFile)
	{
		return Status(Status_NotInitialized, "File not opened");
	}
	*pcbAckSize = fwrite(pBuffer, 1, cbReqSize, m_pFile);
	if (ferror(m_pFile))
	{
		return Status(Status_WriteFailed, "fread failed with error {1}", errno);
	}
	if (0 == *pcbAckSize)
	{
		return Status(Status_EOF, "End of stream reached");
	}

	return Status();
}

Status FileOutputStream::GetSize(UInt64 *pcbSize) const
{
	if (NULL == m_pFile)
	{
		return Status(Status_NotInitialized, "File not opened");
	}

	cx_statstruct buf;

	if (0 != cx_fstat(cx_fileno(m_pFile), &buf))
	{
		return Status(Status_GetSize, "fstat failed with error {1}", errno);
	}
	*pcbSize = buf.st_size;

	return Status();
}

Bool FileOutputStream::IsOK() const
{
	return (NULL != m_pFile);
}

const Char *FileOutputStream::GetPath() const
{
	if (NULL == m_pFile)
	{
		return "";
	}

	return m_sPath.c_str();
}

}//namespace IO

}//namespace CX

