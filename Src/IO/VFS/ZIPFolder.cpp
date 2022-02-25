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
#include "ZIPFolder.hpp"
#include "unzip.h"


namespace CX
{

namespace IO
{

namespace VFS
{

ZIPFolder::ZIPFolder(const Char *szPath)
{
	m_nEntryType = EntryType_Invalid;
	if (NULL != (m_pZIP = unzOpen64(szPath)))
	{
		GoToFirstEntry();
	}
}

ZIPFolder::~ZIPFolder()
{
	if (NULL != m_pZIP)
	{
		unzClose(m_pZIP);
		m_pZIP = NULL;
	}
}

Status ZIPFolder::GoToFirstEntry()
{
	if (NULL == m_pZIP)
	{
		return Status_NotInitialized;
	}

	int      nRet;
	Status   status;

	if (UNZ_OK != (nRet = unzGoToFirstFile(m_pZIP)))
	{
		m_nEntryType = EntryType_Invalid;

		return Status_NoMoreItems;
	}
	if ((status = GetCurrentFileInfo()))
	{
		return Status();
	}
	m_nEntryType = EntryType_File;

	return GoToNextEntry();
}

Status ZIPFolder::GoToNextEntry()
{
	int      nRet;
	Status   status;

	if (NULL == m_pZIP)
	{
		return Status_NotInitialized;
	}
	if (EntryType_Invalid == m_nEntryType)
	{
		return Status_NoMoreItems;
	}
	for (;;)
	{
		if (UNZ_OK != (nRet = unzGoToNextFile(m_pZIP)))
		{
			m_nEntryType = EntryType_Invalid;

			return Status_NoMoreItems;
		}
		if (!(status = GetCurrentFileInfo()))
		{
			m_nEntryType = EntryType_Invalid;

			return Status_NoMoreItems;
		}

		return Status();
	}
}

ZIPFolder::EntryType ZIPFolder::GetEntryType()
{
	if (NULL == m_pZIP)
	{
		return EntryType_Invalid;
	}

	return m_nEntryType;
}

const Char *ZIPFolder::GetEntryPath()
{
	if (NULL == m_pZIP)
	{
		return NULL;
	}

	return m_sEntryPath.c_str();
}

const WChar *ZIPFolder::GetEntryPathW()
{
	if (NULL == m_pZIP)
	{
		return NULL;
	}

	return m_wsEntryPath.c_str();
}

Status ZIPFolder::GetCurrentFileInfo()
{
	unz_file_info64   info;
	int               nRet;

	if (UNZ_OK != (nRet = unzGetCurrentFileInfo64(m_pZIP, &info, NULL, 0, NULL, 0, NULL, 0)))
	{
		return Status_OperationFailed;
	}
	if (0 < info.size_filename)
	{
		m_sEntryPath.resize(info.size_filename);
		if (UNZ_OK != (nRet = unzGetCurrentFileInfo64(m_pZIP, &info, &m_sEntryPath[0], info.size_filename, NULL, 0, 
		                                              NULL, 0)))
		{
			return Status_OperationFailed;
		}
		Str::UTF8::ToWChar(m_sEntryPath.c_str(), &m_wsEntryPath);
		m_nEntryType = EntryType_File;

		return Status();
	}
	else
	{
		return Status_OperationFailed;
	}
}

}//namespace VFS

}//namespace IO

}//namespace CX
