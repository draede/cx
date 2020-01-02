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

#include "CX/Types.hpp"
#include "CX/Status.hpp"
#include "CX/IO/VFS/VFSRegistry.hpp"
#include "CX/IO/VFS/Local.hpp"
#include "CX/Print.hpp"
#include "Tester.hpp"


using namespace CX;


void IO_VFS_Local_Test_Drives()
{
	IO::VFS::IVFS      *pVFS;
	IO::VFS::IFolder   *pFolder;
	Status             status;

	if (NULL != (pVFS = IO::VFS::VFSRegistry::Get().Create("")))
	{
		if (NULL != (pFolder = pVFS->OpenFolder()))
		{
			for (;;)
			{
				Print(stdout, "{1}\n", pFolder->GetEntryPathW());

				if (!pFolder->GoToNextEntry())
				{
					break;
				}
			}

			pVFS->CloseFolder(pFolder);
		}

		IO::VFS::VFSRegistry::Get().Destroy(pVFS);
	}
	if (NULL != (pVFS = IO::VFS::VFSRegistry::Get().CreateW(L"")))
	{
		if (NULL != (pFolder = pVFS->OpenFolder()))
		{
			for (;;)
			{
				Print(stdout, "{1}\n", pFolder->GetEntryPath());

				if (!pFolder->GoToNextEntry())
				{
					break;
				}
			}

			pVFS->CloseFolder(pFolder);
		}

		IO::VFS::VFSRegistry::Get().Destroy(pVFS);
	}
}

void IO_VFS_Local_Test_Folders()
{
	IO::VFS::IVFS      *pVFS;
	IO::VFS::IFolder   *pFolder;
	Status             status;

	if (NULL != (pVFS = IO::VFS::VFSRegistry::Get().Create("")))
	{
		if (NULL != (pFolder = pVFS->OpenFolder("C:")))
		{
			for (;;)
			{
				Print(stdout, "{1}\n", pFolder->GetEntryPathW());

				if (!pFolder->GoToNextEntry())
				{
					break;
				}
			}

			pVFS->CloseFolder(pFolder);
		}

		IO::VFS::VFSRegistry::Get().Destroy(pVFS);
	}
	if (NULL != (pVFS = IO::VFS::VFSRegistry::Get().CreateW(L"")))
	{
		if (NULL != (pFolder = pVFS->OpenFolderW(L"C:")))
		{
			for (;;)
			{
				Print(stdout, "{1}\n", pFolder->GetEntryPath());

				if (!pFolder->GoToNextEntry())
				{
					break;
				}
			}

			pVFS->CloseFolder(pFolder);
		}

		IO::VFS::VFSRegistry::Get().Destroy(pVFS);
	}
}

REGISTER_TEST(IO_VFS_Local_Test_Drives);
REGISTER_TEST(IO_VFS_Local_Test_Folders);
