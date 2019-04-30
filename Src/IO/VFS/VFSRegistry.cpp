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
#include "CX/IO/VFS/VFSRegistry.hpp"
#include "CX/IO/VFS/Local.hpp"
#include "CX/Str/UTF8.hpp"


namespace CX
{

namespace IO
{

namespace VFS
{

VFSRegistry::VFSRegistry()
{
}

VFSRegistry::~VFSRegistry()
{
}

VFSRegistry &VFSRegistry::Get()
{
	static VFSRegistry   registry;

	return registry;
}

IVFS *VFSRegistry::Create(const Char *szName)
{
	const Char   *pszPos = cx_strstr(szName, "://");

	if (NULL == pszPos)
	{
		String   sLocalName;

		sLocalName = Local::NAME();
		sLocalName += "://";

		return Create(sLocalName.c_str());
	}

	String   sName(szName, pszPos - szName);
	IVFS     *pVFS;
	Status   status;

	auto iter = m_mapVFSs.find(sName.c_str());

	if (m_mapVFSs.end() == iter)
	{
		return NULL;
	}
	if (NULL == (pVFS = iter->second.pfnCreate()))
	{
		return NULL;
	}
	if (!(status = pVFS->Open(pszPos + 3)))
	{
		iter->second.pfnDestroy(pVFS);

		return NULL;
	}

	return pVFS;
}

IVFS *VFSRegistry::CreateW(const WChar *wszName)
{
	const WChar   *pwszPos = cxw_strstr(wszName, L"://");
	Status        status;

	if (NULL == pwszPos)
	{
		String    sLocalName;
		WString   wsLocalName;

		sLocalName = Local::NAME();
		sLocalName += "://";
		if (!(status = Str::UTF8::ToWChar(sLocalName.c_str(), &wsLocalName)))
		{
			return NULL;
		}

		return CreateW(wsLocalName.c_str());
	}

	WString   wsName(wszName, pwszPos - wszName);
	String    sName;
	IVFS     *pVFS;

	if (!(status = Str::UTF8::FromWChar(wsName.c_str(), &sName)))
	{
		return NULL;
	}

	auto iter = m_mapVFSs.find(sName.c_str());

	if (m_mapVFSs.end() == iter)
	{
		return NULL;
	}
	if (NULL == (pVFS = iter->second.pfnCreate()))
	{
		return NULL;
	}
	if (!(status = pVFS->OpenW(pwszPos + 3)))
	{
		iter->second.pfnDestroy(pVFS);

		return NULL;
	}

	return pVFS;
}

Status VFSRegistry::Destroy(IVFS *pVFS)
{
	auto iter = m_mapVFSs.find(pVFS->GetName());

	if (m_mapVFSs.end() == iter)
	{
		return Status_NotFound;
	}
	pVFS->Close();
	iter->second.pfnDestroy(pVFS);

	return Status();
}

Status VFSRegistry::Register(const Char *szName, IVFS::CreateProc pfnCreate, IVFS::DestroyProc pfnDestroy)
{
	VFS   vfs;

	vfs.pfnCreate     = pfnCreate;
	vfs.pfnDestroy    = pfnDestroy;
	m_mapVFSs[szName] = vfs;

	return Status();
}

}//namespace VFS

}//namespace IO

}//namespace CX
