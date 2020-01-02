/* 
 * CX - C++ framework for general purpose development
 *
 * https://github.com/draede/cx
 * 
 * Copyright (C) 2014 - 2020 draede - draede [at] outlook [dot] com
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


#include "CX/IO/VFS/Platform/Windows/Local.hpp"
#include "LocalFolder.hpp"
#include "CX/Str/UTF8.hpp"
#include "CX/IO/FileInputStream.hpp"
#include "CX/IO/FileOutputStream.hpp"
#include "CX/IO/VFS/VFSRegistry.hpp"
#include <new>


namespace CX
{

namespace IO
{

namespace VFS
{

Status Local::REGISTER_STATUS = Local::ForceRegister();

IVFS *Local::Create()
{
	return new (std::nothrow) Local();
}

void Local::Destroy(IVFS *pVFS)
{
	Local   *pActVFS = dynamic_cast<Local *>(pVFS);

	if (NULL != pActVFS)
	{
		delete pActVFS;
	}
}

Status Local::ForceRegister()
{
	return (REGISTER_STATUS = VFSRegistry::Get().Register(Local::NAME(), &Local::Create, &Local::Destroy));
}

Local::Local()
{
	m_bOpened = False;
}

Local::~Local()
{
	Close();
}

const Char *Local::GetName() const
{
	return NAME();
}

Status Local::Open(const Char *szPath)
{
	if (NULL == szPath)
	{
		return OpenW(NULL);
	}

	WString   wsBasePath;
	Status    status;

	if (!(status = Str::UTF8::ToWChar(szPath, &wsBasePath)))
	{
		return status;
	}

	return OpenW(wsBasePath.c_str());
}

Status Local::OpenW(const WChar *wszPath)
{
	Close();

	if (NULL == wszPath || 0 == *wszPath)
	{
		m_wsBasePath.clear();
		m_bOpened = True;
	}
	else
	{
		m_wsBasePath = wszPath;
		m_bOpened    = True;
	}

	return Status();
}

Status Local::Close()
{
	m_wsBasePath.clear();
	m_bOpened = False;

	return Status();
}

Bool Local::IsOK() const
{
	return m_bOpened;
}

Bool Local::IsReadable() const
{
	return True;
}

Bool Local::IsWriteable() const
{
	return True;
}

Status Local::CreateFolder(const Char *szPath)
{
	if (!m_bOpened)
	{
		return Status_NotInitialized;
	}

	WString   wsPath;
	Status    status;

	if (!(status = Str::UTF8::ToWChar(szPath, &wsPath)))
	{
		return status;
	}

	return CreateFolderW(wsPath.c_str());
}

Status Local::CreateFolderW(const WChar *wszPath)
{
	if (!m_bOpened)
	{
		return Status_NotInitialized;
	}

	if (!::CreateDirectoryW(wszPath, NULL))
	{
		return Status_OperationFailed;
	}

	return Status();
}

IFolder *Local::OpenFolder(const Char *szPath/* = NULL*/)
{
	if (!m_bOpened)
	{
		return NULL;
	}

	if (NULL == szPath)
	{
		return OpenFolderW(NULL);
	}

	WString   wsPath;
	Status    status;

	if (!(status = Str::UTF8::ToWChar(szPath, &wsPath)))
	{
		return NULL;
	}

	return OpenFolderW(wsPath.c_str());
}

IFolder *Local::OpenFolderW(const WChar *wszPath/* = NULL*/)
{
	if (!m_bOpened)
	{
		return NULL;
	}

	if (NULL == wszPath)
	{
		if (m_wsBasePath.empty())
		{
			LocalFolder   *pFolder = new (std::nothrow) LocalFolder(NULL);

			return pFolder;
		}
		else
		{
			LocalFolder   *pFolder = new (std::nothrow) LocalFolder(m_wsBasePath.c_str());

			return pFolder;
		}
	}
	else
	{
		LocalFolder   *pFolder = new (std::nothrow) LocalFolder(wszPath);

		return pFolder;
	}
}

Status Local::CloseFolder(IFolder *pFolder)
{
	if (!m_bOpened)
	{
		return Status_NotInitialized;
	}

	LocalFolder   *pActFolder = dynamic_cast<LocalFolder *>(pFolder);

	if (NULL == pActFolder)
	{
		return Status_InvalidCall;
	}

	delete pActFolder;

	return Status();
}

IInputStream *Local::OpenInputStream(const Char *szPath)
{
	if (!m_bOpened)
	{
		return NULL;
	}

	WString   wsPath;
	Status    status;

	if (!(status = Str::UTF8::ToWChar(szPath, &wsPath)))
	{
		return NULL;
	}

	return OpenInputStreamW(wsPath.c_str());
}

IInputStream *Local::OpenInputStreamW(const WChar *wszPath)
{
	if (!m_bOpened)
	{
		return NULL;
	}

	IO::FileInputStream   *pFIS = new (std::nothrow) IO::FileInputStream(wszPath);

	if (NULL == pFIS)
	{
		return NULL;
	}
	if (!pFIS->IsOK())
	{
		delete pFIS;

		return NULL;
	}

	return pFIS;
}

Status Local::CloseInputStream(IInputStream *pInputStream)
{
	if (!m_bOpened)
	{
		return Status_NotInitialized;
	}

	IO::FileInputStream   *pFIS = dynamic_cast<IO::FileInputStream *>(pInputStream);

	if (NULL == pFIS)
	{
		return Status_InvalidCall;
	}

	delete pFIS;

	return Status();
}

IOutputStream *Local::CreateOutputStream(const Char *szPath)
{
	if (!m_bOpened)
	{
		return NULL;
	}

	WString   wsPath;
	Status    status;

	if (!(status = Str::UTF8::ToWChar(szPath, &wsPath)))
	{
		return NULL;
	}

	return CreateOutputStreamW(wsPath.c_str());
}

IOutputStream *Local::CreateOutputStreamW(const WChar *wszPath)
{
	if (!m_bOpened)
	{
		return NULL;
	}

	IO::FileOutputStream   *pFOS = new (std::nothrow) IO::FileOutputStream(wszPath);

	if (NULL == pFOS)
	{
		return NULL;
	}
	if (!pFOS->IsOK())
	{
		delete pFOS;

		return NULL;
	}

	return pFOS;
}

Status Local::CloseOutputStream(IOutputStream *pOutputStream)
{
	if (!m_bOpened)
	{
		return Status_NotInitialized;
	}

	IO::FileOutputStream   *pFOS = dynamic_cast<IO::FileOutputStream *>(pOutputStream);

	if (NULL == pFOS)
	{
		return Status_InvalidCall;
	}

	delete pFOS;

	return Status();
}

}//namespace VFS

}//namespace IO

}//namespace CX


#endif
