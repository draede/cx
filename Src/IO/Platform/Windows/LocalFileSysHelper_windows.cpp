/* 
 * CX - C++ framework for general purpose development
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

#include "CX/Platform.hpp"


#if defined(CX_OS_WINDOWS)


#include "CX/IO/LocalFileSysHelper.hpp"
#include "CX/C/Platform/Windows/windows.h"
#include "CX/Str/UTF8.hpp"


#undef CopyFile


namespace CX
{

namespace IO
{

LocalFileSysHelper::LocalFileSysHelper()
{
}

LocalFileSysHelper::~LocalFileSysHelper()
{
}

LocalFileSysHelper &LocalFileSysHelper::Get()
{
	static LocalFileSysHelper helper;

	return helper;
}

const Char *LocalFileSysHelper::GetPathSep()
{
	return "\\";
}

Status LocalFileSysHelper::AppendPath(String &sPath, const Char *szStr)
{
	if (!sPath.empty() && '\\' != *sPath.rbegin() && '/' != *sPath.rbegin() &&'\\' != *szStr && '/' != *szStr)
	{
		sPath += "\\";
	}
	sPath += szStr;

	return Status();
}

bool LocalFileSysHelper::IsAbsPath(const Char *szPath)
{
	if (0 == *szPath)
	{
		return false;
	}
	if ('\\' == *szPath)
	{
		return true;
	}
	if ('/' == *szPath)
	{
		return true;
	}
	if (NULL != (cx_strchr(szPath, ':')))
	{
		return true;
	}

	return false;
}

Status LocalFileSysHelper::CopyFile(const Char *szOldPath, const Char *szNewPath)
{
	WString wsOldPath;
	WString wsNewPath;
	Status  status;

	if ((status = Str::UTF8::ToUTF16(szOldPath, &wsOldPath)).IsNOK())
	{
		return status;
	}
	if ((status = Str::UTF8::ToUTF16(szNewPath, &wsNewPath)).IsNOK())
	{
		return status;
	}
	if (!CopyFileW(wsOldPath.c_str(), wsNewPath.c_str(), FALSE))
	{
		return Status(Status_OperationFailed, "CopyFileW failed with error {1}", (int)GetLastError());
	}

	return Status();
}

Status LocalFileSysHelper::RenameFile(const Char *szOldPath, const Char *szNewPath)
{
	WString wsOldPath;
	WString wsNewPath;
	Status  status;

	if ((status = Str::UTF8::ToUTF16(szOldPath, &wsOldPath)).IsNOK())
	{
		return status;
	}
	if ((status = Str::UTF8::ToUTF16(szNewPath, &wsNewPath)).IsNOK())
	{
		return status;
	}
	if (!MoveFileExW(wsOldPath.c_str(), wsNewPath.c_str(), MOVEFILE_COPY_ALLOWED | MOVEFILE_REPLACE_EXISTING))
	{
		return Status(Status_OperationFailed, "MoveFileExW failed with error {1}", (int)GetLastError());
	}

	return Status();
}

Status LocalFileSysHelper::RemoveFile(const Char *szPath)
{
	WString wsPath;
	Status  status;

	if ((status = Str::UTF8::ToUTF16(szPath, &wsPath)).IsNOK())
	{
		return status;
	}
	if (!DeleteFileW(wsPath.c_str()))
	{
		return Status(Status_OperationFailed, "DeleteFileW failed with error {1}", (int)GetLastError());
	}

	return Status();
}

Status LocalFileSysHelper::CreateFolder(const Char *szPath)
{
	WString wsPath;
	Status  status;

	if ((status = Str::UTF8::ToUTF16(szPath, &wsPath)).IsNOK())
	{
		return status;
	}
	if (!CreateDirectoryW(wsPath.c_str(), NULL))
	{
		return Status(Status_OperationFailed, "CreateDirectoryW failed with error {1}", (int)GetLastError());
	}

	return Status();
}

Status LocalFileSysHelper::RenameFolder(const Char *szOldPath, const Char *szNewPath)
{
	WString wsOldPath;
	WString wsNewPath;
	Status  status;

	if ((status = Str::UTF8::ToUTF16(szOldPath, &wsOldPath)).IsNOK())
	{
		return status;
	}
	if ((status = Str::UTF8::ToUTF16(szNewPath, &wsNewPath)).IsNOK())
	{
		return status;
	}
	if (!MoveFileExW(wsOldPath.c_str(), wsNewPath.c_str(), MOVEFILE_COPY_ALLOWED | MOVEFILE_REPLACE_EXISTING))
	{
		return Status(Status_OperationFailed, "MoveFileExW failed with error {1}", (int)GetLastError());
	}

	return Status();
}

Status LocalFileSysHelper::RemoveFolder(const Char *szPath)
{
	WString wsPath;
	Status  status;

	if ((status = Str::UTF8::ToUTF16(szPath, &wsPath)).IsNOK())
	{
		return status;
	}
	if (!RemoveDirectoryW(wsPath.c_str()))
	{
		return Status(Status_OperationFailed, "RemoveDirectoryW failed with error {1}", (int)GetLastError());
	}

	return Status();
}

}//namespace IO

}//namespace CX


#endif

