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


#if defined(CX_OS_WINDOWS)


#include "LocalFolder.hpp"
#include "CX/Str/UTF8.hpp"


namespace CX
{

namespace IO
{

namespace VFS
{

LocalFolder::LocalFolder(const WChar *wszBasePath/* = NULL*/)
{
	m_hFind        = NULL;
	memset(&m_data, 0, sizeof(m_data));

	m_cDrivesCount = 0;
	m_cDrivesIndex = 0;
	memset(m_drives, 0, sizeof(m_drives));
	memset(m_drivesW, 0, sizeof(m_drivesW));

	if (NULL == wszBasePath || 0 == *wszBasePath)
	{
		DWORD  dwDrives = GetLogicalDrives();
		DWORD  dwPow    = 1;

		m_cDrivesCount = 0;
		m_cDrivesIndex = 0;
		dwPow          = 1;
		for (WChar i = L'A'; i <= L'Z'; i++)
		{
			if (0 != (dwDrives & dwPow))
			{
				m_drives[m_cDrivesCount][0]  = (Char)i;
				m_drives[m_cDrivesCount][1]  = L':';
				m_drives[m_cDrivesCount][2]  = 0;

				m_drivesW[m_cDrivesCount][0] = L'\\';
				m_drivesW[m_cDrivesCount][1] = L'\\';
				m_drivesW[m_cDrivesCount][2] = L'?';
				m_drivesW[m_cDrivesCount][3] = L'\\';
				m_drivesW[m_cDrivesCount][4] = i;
				m_drivesW[m_cDrivesCount][5] = L':';
				m_drivesW[m_cDrivesCount][6] = 0;

				m_cDrivesCount++;
			}
			dwPow *= 2;
		}
	}
	else
	{
		m_wsBasePath = wszBasePath;

		GoToFirstEntry();
	}
}

LocalFolder::~LocalFolder()
{
	if (NULL != m_hFind)
	{
		::FindClose(m_hFind);
		m_hFind = NULL;
	}
}

Status LocalFolder::GoToFirstEntry()
{
	if (NULL != m_hFind)
	{
		::FindClose(m_hFind);
		m_hFind = NULL;
	}

	if (m_wsBasePath.empty())
	{
		m_cDrivesIndex = 0;

		return Status();
	}
	else
	{
		WString   wsMask;

		wsMask = m_wsBasePath;
		wsMask += L"\\*.*";
		if (INVALID_HANDLE_VALUE == (m_hFind = ::FindFirstFileW(wsMask.c_str(), &m_data)))
		{
			m_hFind = NULL;

			return Status_NoMoreItems;
		}

		if ((0 != (FILE_ATTRIBUTE_REPARSE_POINT & m_data.dwFileAttributes)) || 
			   (0 != (FILE_ATTRIBUTE_DIRECTORY & m_data.dwFileAttributes) && 
			   (0 == cxw_strcmp(L".", m_data.cFileName) || 0 == cxw_strcmp(L"..", m_data.cFileName))))
		{
			return GoToNextEntry();
		}

		return Status();
	}
}

Status LocalFolder::GoToNextEntry()
{
	if (m_wsBasePath.empty())
	{
		if (m_cDrivesIndex >= m_cDrivesCount)
		{
			return Status_NoMoreItems;
		}
		m_cDrivesIndex++;
		if (m_cDrivesIndex >= m_cDrivesCount)
		{
			return Status_NoMoreItems;
		}

		return Status();
	}
	else
	{
		if (NULL == m_hFind)
		{
			return Status_NoMoreItems;
		}
		for (;;)
		{
			if (!::FindNextFileW(m_hFind, &m_data))
			{
				::FindClose(m_hFind);
				m_hFind = NULL;

				return Status_NoMoreItems;
			}
			if ((0 != (FILE_ATTRIBUTE_REPARSE_POINT & m_data.dwFileAttributes)) || 
				   (0 != (FILE_ATTRIBUTE_DIRECTORY & m_data.dwFileAttributes) && 
				   (0 == cxw_strcmp(L".", m_data.cFileName) || 0 == cxw_strcmp(L"..", m_data.cFileName))))
			{
				continue;
			}

			break;
		}

		return Status();
	}
}

LocalFolder::EntryType LocalFolder::GetEntryType()
{
	if (m_wsBasePath.empty())
	{
		if (m_cDrivesIndex >= m_cDrivesCount)
		{
			return EntryType_Invalid;
		}

		return EntryType_Folder;
	}
	else
	{
		if (NULL == m_hFind)
		{
			return EntryType_Invalid;
		}

		if (0 != (FILE_ATTRIBUTE_DIRECTORY & m_data.dwFileAttributes))
		{
			return EntryType_Folder;
		}
		else
		{
			return EntryType_File;
		}
	}
}

const Char *LocalFolder::GetEntryPath()
{
	if (m_wsBasePath.empty())
	{
		if (m_cDrivesIndex >= m_cDrivesCount)
		{
			return NULL;
		}

		return m_drives[m_cDrivesIndex];
	}
	else
	{
		const WChar   *wszPath = GetEntryPathW();
		Status        status;

		if (NULL == wszPath)
		{
			return NULL;
		}

		if (!(status = Str::UTF8::FromWChar(wszPath, &m_sEntryPath)))
		{
			return NULL;
		}

		return m_sEntryPath.c_str();
	}
}

const WChar *LocalFolder::GetEntryPathW()
{
	if (m_wsBasePath.empty())
	{
		if (m_cDrivesIndex >= m_cDrivesCount)
		{
			return NULL;
		}

		return m_drivesW[m_cDrivesIndex];
	}
	else
	{
		if (NULL == m_hFind)
		{
			return NULL;
		}

		m_wsEntryPath = m_wsBasePath;
		m_wsEntryPath += L"\\";
		m_wsEntryPath += m_data.cFileName;

		return m_wsEntryPath.c_str();
	}
}

}//namespace VFS

}//namespace IO

}//namespace CX


#endif
