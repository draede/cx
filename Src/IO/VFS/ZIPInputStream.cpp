/* 
 * CX - C++ framework for general purpose development
 *
 * https://github.com/draede/cx
 * 
 * Copyright (C) 2014 - 2019 draede - draede [at] outlook [dot] com
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
#include "ZIPInputStream.hpp"


namespace CX
{

namespace IO
{

namespace VFS
{

ZIPInputStream::ZIPInputStream(const Char *szZIPPath, const Char *szFileInZIPPath)
{
	m_pZIP     = NULL;
	m_cbSize   = 0;
	m_cbOffset = 0;

	if (NULL == szZIPPath || NULL == szFileInZIPPath)
	{
		return;
	}

	const Char   *pszPos;
	const Char   *pszTmpPos;
	String       sPassword;
	int          nRet;
	Status       status;

	pszPos    = NULL;
	pszTmpPos = szFileInZIPPath;
	while (0 != *pszTmpPos)
	{
		if ('?' == *pszTmpPos)
		{
			pszPos = pszTmpPos;
		}
		pszTmpPos++;
	}

	if (NULL != pszPos)
	{
		m_sPath.assign(szFileInZIPPath, pszPos - szFileInZIPPath);
		sPassword = pszPos + 1;
	}
	else
	{
		m_sPath = szFileInZIPPath;
	}

	for (;;)
	{
		if (NULL == (m_pZIP = unzOpen64(szZIPPath)))
		{
			status  = Status_NotFound;

			break;
		}
		if (UNZ_OK != (nRet = unzLocateFile(m_pZIP, m_sPath.c_str(), 1)))
		{
			status  = Status_NotFound;

			break;
		}

		unz_file_info64   info;

		if (UNZ_OK != (nRet = unzGetCurrentFileInfo64(m_pZIP, &info, NULL, 0, NULL, 0, NULL, 0)))
		{
			status  = Status_OperationFailed;

			break;
		}

		if (sPassword.empty())
		{
			if (UNZ_OK != (nRet = unzOpenCurrentFile(m_pZIP)))
			{
				status = Status_OpenFailed;

				break;
			}
		}
		else
		{
			if (UNZ_OK != (nRet = unzOpenCurrentFilePassword(m_pZIP, sPassword.c_str())))
			{
				status = Status_OpenFailed;

				break;
			}
		}

		m_cbSize = info.uncompressed_size;

		break;
	}
	if (!status)
	{
		Close();
	}
}

ZIPInputStream::~ZIPInputStream()
{
	Close();
}

void ZIPInputStream::Close()
{
	if (NULL != m_pZIP)
	{
		unzCloseCurrentFile(m_pZIP);
		unzClose(m_pZIP);
		m_pZIP = NULL;
	}
	m_sPath.clear();
	m_cbSize   = 0;
	m_cbOffset = 0;
}

Status ZIPInputStream::Read(void *pBuffer, Size cbReqSize, Size *pcbAckSize)
{
	int   nRet;

	if (NULL == m_pZIP)
	{
		return Status_NotInitialized;
	}
	nRet = unzReadCurrentFile(m_pZIP, pBuffer, (unsigned int)cbReqSize);
	if (0 == nRet)
	{
		return Status_EOF;
	}
	if (0 > nRet)
	{
		return Status_ReadFailed;
	}
	*pcbAckSize = nRet;
	m_cbOffset += *pcbAckSize;

	return Status();
}

Status ZIPInputStream::SetPos(UInt64 cbPos)
{
	if (NULL == m_pZIP)
	{
		return Status_NotInitialized;
	}

	CX_UNUSED(cbPos);

	return Status_NotSupported;
}

Status ZIPInputStream::GetPos(UInt64 *pcbPos) const
{
	if (NULL == m_pZIP)
	{
		return Status_NotInitialized;
	}

	*pcbPos = m_cbOffset;

	return Status();
}

Status ZIPInputStream::GetSize(UInt64 *pcbSize) const
{
	if (NULL == m_pZIP)
	{
		return Status_NotInitialized;
	}

	*pcbSize = m_cbSize;

	return Status();
}

Bool ZIPInputStream::IsEOF() const
{
	return (m_cbOffset >= m_cbSize);
}

Bool ZIPInputStream::IsOK() const
{
	return (NULL != m_pZIP);
}

const Char *ZIPInputStream::GetPath() const
{
	return m_sPath.c_str();
}

}//namespace VFS

}//namespace IO

}//namespace CX
