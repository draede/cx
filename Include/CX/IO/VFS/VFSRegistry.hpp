/* 
 * CX - C++ framework for general purpose development
 *
 * https://github.com/draede/cx
 * 
 * Copyright (C) 2014 - 2022 draede
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
 
#pragma once


#include "CX/Types.hpp"
#include "CX/Status.hpp"
#include "CX/String.hpp"
#include "CX/Map.hpp"
#include "CX/APIDefs.hpp"
#include "CX/IO/VFS/IVFS.hpp"


namespace CX
{

namespace IO
{

namespace VFS
{

class CX_API VFSRegistry
{
public:

	static VFSRegistry &Get();

	//<vfs_name>://vfs_specific_path
	IVFS *Create(const Char *szPath);

	//<vfs_name>://vfs_specific_path
	IVFS *CreateW(const WChar *wszPath);

	Status Destroy(IVFS *pVFS);

	Status Register(const Char *szName, IVFS::CreateProc pfnCreate, IVFS::DestroyProc pfnDestroy);

private:

	struct VFS
	{
		IVFS::CreateProc    pfnCreate;
		IVFS::DestroyProc   pfnDestroy;
	};

	typedef Map<String, VFS>::Type   VFSsMap; 

#pragma warning(push)
#pragma warning(disable: 4251)
	VFSsMap   m_mapVFSs;
#pragma warning(pop) 

	VFSRegistry();

	~VFSRegistry();

};

}//namespace VFS

}//namespace IO

}//namespace CX 
