/* 
 * CX - C++ framework for general purpose development
 *
 * https://github.com/draede/cx
 * 
 * Copyright (C) 2014 - 2021 draede - draede [at] outlook [dot] com
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
#include "CX/IO/VFS/ZIP.hpp"
#include "ZIPFolder.hpp"
#include "CX/Str/UTF8.hpp"
#include "CX/IO/VFS/VFSRegistry.hpp"
#include "ZIPInputStream.hpp"
#include "ZIPFolder.hpp"
#include "unzip.h"
#include <new>


namespace CX
{

namespace IO
{

namespace VFS
{

Status ZIP::REGISTER_STATUS = ZIP::ForceRegister();

IVFS *ZIP::Create()
{
	return new (std::nothrow) ZIP();
}

void ZIP::Destroy(IVFS *pVFS)
{
	ZIP   *pActVFS = dynamic_cast<ZIP *>(pVFS);

	if (NULL != pActVFS)
	{
		delete pActVFS;
	}
}

Status ZIP::ForceRegister()
{
	return (REGISTER_STATUS = VFSRegistry::Get().Register(ZIP::NAME(), &ZIP::Create, &ZIP::Destroy));
}

ZIP::ZIP()
{
	m_pZIP = NULL;
}

ZIP::~ZIP()
{
	Close();
}

const Char *ZIP::GetName() const
{
	return NAME();
}

Status ZIP::Open(const Char *szPath)
{
	Close();

	if (NULL == (m_pZIP = unzOpen64(szPath)))
	{
		return Status_OpenFailed;
	}
	m_sPath = szPath;

	return Status();
}

Status ZIP::OpenW(const WChar *wszPath)
{
	String   sPath;
	Status   status;

	if (!(status = Str::UTF8::FromWChar(wszPath, &sPath)))
	{
		return status;
	}

	return Open(sPath.c_str());
}

Status ZIP::Close()
{
	if (NULL != m_pZIP)
	{
		unzClose(m_pZIP);
		m_pZIP = NULL;
	}
	m_sPath.clear();

	return Status();
}

Bool ZIP::IsOK() const
{
	return (NULL != m_pZIP);
}

Bool ZIP::IsReadable() const
{
	return True;
}

Bool ZIP::IsWriteable() const
{
	return False;
}

Status ZIP::CreateFolder(const Char *szPath)
{
	CX_UNUSED(szPath);

	return Status_NotSupported;
}

Status ZIP::CreateFolderW(const WChar *wszPath)
{
	CX_UNUSED(wszPath);

	return Status_NotSupported;
}

IFolder *ZIP::OpenFolder(const Char *szPath/* = NULL*/)
{
	if (NULL == m_pZIP)
	{
		return NULL;
	}
	if (NULL != szPath && 0 != *szPath)
	{
		return NULL;
	}

	ZIPFolder   *pFolder = new (std::nothrow) ZIPFolder(m_sPath.c_str());

	if (NULL == pFolder)
	{
		return NULL;
	}

	return pFolder;
}

IFolder *ZIP::OpenFolderW(const WChar *wszPath/* = NULL*/)
{
	if (NULL == m_pZIP)
	{
		return NULL;
	}
	if (NULL == wszPath)
	{
		return OpenFolder(NULL);
	}

	String   sPath;
	Status   status;

	if (!(status = Str::UTF8::FromWChar(wszPath, &sPath)))
	{
		return NULL;
	}

	return OpenFolder(sPath.c_str());
}

Status ZIP::CloseFolder(IFolder *pFolder)
{
	if (NULL == m_pZIP)
	{
		return Status_NotInitialized;
	}

	ZIPFolder   *pActFolder = dynamic_cast<ZIPFolder *>(pFolder);

	if (NULL == pActFolder)
	{
		return Status_InvalidCall;
	}

	delete pActFolder;

	return Status();
}

IInputStream *ZIP::OpenInputStream(const Char *szPath)
{
	if (NULL == m_pZIP)
	{
		return NULL;
	}

	ZIPInputStream   *pZIF = new (std::nothrow) ZIPInputStream(m_sPath.c_str(), szPath);

	if (NULL == pZIF)
	{
		return NULL;
	}
	if (!pZIF->IsOK())
	{
		delete pZIF;

		return NULL;
	}

	return pZIF;
}

IInputStream *ZIP::OpenInputStreamW(const WChar *wszPath)
{
	if (NULL == m_pZIP)
	{
		return NULL;
	}

	String   sPath;
	Status   status;

	if (!(status = Str::UTF8::FromWChar(wszPath, &sPath)))
	{
		return NULL;
	}

	return OpenInputStream(sPath.c_str());
}

Status ZIP::CloseInputStream(IInputStream *pInputStream)
{
	if (NULL == m_pZIP)
	{
		return Status_NotInitialized;
	}

	ZIPInputStream   *pFIS = dynamic_cast<ZIPInputStream *>(pInputStream);

	if (NULL == pFIS)
	{
		return Status_InvalidCall;
	}

	delete pFIS;

	return Status();
}

IOutputStream *ZIP::CreateOutputStream(const Char *szPath)
{
	CX_UNUSED(szPath);

	return NULL;
}

IOutputStream *ZIP::CreateOutputStreamW(const WChar *wszPath)
{
	CX_UNUSED(wszPath);

	return NULL;
}

Status ZIP::CloseOutputStream(IOutputStream *pOutputStream)
{
	CX_UNUSED(pOutputStream);

	return NULL;
}

}//namespace VFS

}//namespace IO

}//namespace CX
