
#include "CX/Platform.hpp"


#if defined(CX_OS_WINDOWS)


#include "CX/IO/LocalFileSysEnumerator.hpp"
#include "CX/C/Platform/Windows/windows.h"
#include "CX/Str/UTF8.hpp"


namespace CX
{

namespace IO
{

LocalFileSysEnumerator::LocalFileSysEnumerator()
{
}

LocalFileSysEnumerator::~LocalFileSysEnumerator()
{
}

Status LocalFileSysEnumerator::EnumDrives(IItemHandler *pHandler, const WChar *wszMask)
{
	WChar       wszName[64];
	WChar       *wszPath;
	const WChar *pPos;
	DWORD       cPathLen;
	DWORD       cPathLenNeeded;
	HANDLE      hVolFind;
	bool        bContinue;
	Status      status;

	if (INVALID_HANDLE_VALUE == (hVolFind = FindFirstVolumeW(wszName, sizeof(wszName) / sizeof(wszName[0]))))
	{
		return Status(Status_OperationFailed, "FindFirstVolumeW failed with error {1}", GetLastError());
	}
	wszPath  = NULL;
	cPathLen = 0;
	status.Clear();
	for (;;)
	{
		if (!GetVolumePathNamesForVolumeNameW(wszName, wszPath, cPathLen, &cPathLenNeeded))
		{
			if (ERROR_MORE_DATA != GetLastError())
			{
				return Status(Status_OperationFailed, "GetVolumePathNamesForVolumeNameW failed with error {1}", GetLastError());

				break;
			}
			else
			{
				cPathLen = cPathLenNeeded;
				if (NULL == (wszPath = (WChar *)Mem::Realloc(wszPath, sizeof(WChar) * cPathLen)))
				{
					status = Status_MemAllocFailed;

					break;
				}
				if (!GetVolumePathNamesForVolumeNameW(wszName, wszPath, cPathLen, &cPathLenNeeded))
				{
					return Status(Status_OperationFailed, "GetVolumePathNamesForVolumeNameW failed with error {1}", GetLastError());

					break;
				}
			}
		}
		pPos      = wszPath;
		bContinue = true;
		if (NULL != pPos)
		{
			for (;;)
			{
				if (0 == *pPos)
				{
					if (wszPath < pPos)
					{
						if (!(status = Notify(pHandler, "", wszPath, IItemHandler::Type_Folder, 0, &bContinue)))
						{
							if (bContinue)
							{
								status.Clear();
							}
						}
						if (!(status = Enum(pHandler, wszMask, wszPath, &bContinue)))
						{
							if (bContinue)
							{
								status.Clear();
							}
						}
					}

					break;
				}
				else
				{
					pPos++;
				}
			}
		}
		if (!bContinue)
		{
			break;
		}
		if (!FindNextVolumeW(hVolFind, wszName, sizeof(wszName) / sizeof(wszName[0])))
		{
			break;
		}
	}
	if (NULL != wszPath)
	{
		Mem::Free(wszPath);
	}
	FindVolumeClose(hVolFind);

	return status;
}

Status LocalFileSysEnumerator::Notify(IItemHandler *pHandler, const Char *szPath, const WChar *wszName, 
                                      IItemHandler::Type nType, UInt64 cbSize, bool *pbContinue)
{
	String sName;
	Status status;

	*pbContinue = true;
	if (!(status = Str::UTF8::FromWChar(wszName, &sName)))
	{
		return status;
	}
	if (!(status = pHandler->OnItem(szPath, sName.c_str(), nType, cbSize)))
	{
		*pbContinue = false;
	}

	return status;
}

Status LocalFileSysEnumerator::Enum(IItemHandler *pHandler, const WChar *wszMask, const WChar *wszPath, bool *pbContinue)
{
	WIN32_FIND_DATAW data;
	WString          wsPath;
	String           sPath;
	WString          wsMask;
	HANDLE           hFind;
	ULARGE_INTEGER   uliSize;
	Status           status;

	if (0 != cxw_strncmp(wszPath, L"\\\\.\\", 4))
	{
		wsPath += L"\\\\.\\";
	}
	wsPath += wszPath;
	wsMask = wsPath;
	if (L'\\' != *wsPath.rbegin())
	{
		wsMask += L"\\";
	}
	wsMask += wszMask;

	if (!(status = Str::UTF8::FromWChar(wsPath.c_str(), &sPath)))
	{
		return status;
	}

	if (INVALID_HANDLE_VALUE == (hFind = FindFirstFileW(wsMask.c_str(), &data)))
	{
		return Status();
	}
	*pbContinue = true;
	for (;;)
	{
		if (0 != cxw_strcmp(data.cFileName, L".") && 0 != cxw_strcmp(data.cFileName, L".."))
		{
			if (!(FILE_ATTRIBUTE_REPARSE_POINT & data.dwFileAttributes))
			{
				if ((FILE_ATTRIBUTE_DIRECTORY & data.dwFileAttributes))
				{
					if (!(status = Notify(pHandler, sPath.c_str(), data.cFileName, IItemHandler::Type_Folder, 0, pbContinue)))
					{
						if ((*pbContinue))
						{
							status.Clear();
						}
						else
						{
							break;
						}
					}
					
					WString wsNextPath;

					wsNextPath = wsPath;
					if (L'\\' != *wsNextPath.rbegin())
					{
						wsNextPath += L"\\";
					}
					wsNextPath += data.cFileName;

					status = Enum(pHandler, wszMask, wsNextPath.c_str(), pbContinue);
					if ((*pbContinue))
					{
						status.Clear();
					}
					else
					{
						break;
					}
				}
				else
				{
					uliSize.LowPart  = data.nFileSizeLow;
					uliSize.HighPart = data.nFileSizeHigh;
					if (!(status = Notify(pHandler, sPath.c_str(), data.cFileName, IItemHandler::Type_File, uliSize.QuadPart, 
					                      pbContinue)))
					{
						if ((*pbContinue))
						{
							status.Clear();
						}
						else
						{
							break;
						}
					}
				}
			}
		}
		if (!FindNextFileW(hFind, &data))
		{
			break;
		}
	}
	FindClose(hFind);

	return Status();
}

Status LocalFileSysEnumerator::Enumerate(IItemHandler *pHandler, const Char *szMask/* = NULL*/, const Char *szPath/* = NULL*/)
{
	if (NULL == szMask)
	{
		szMask = "*.*";
	}

	WString wsMask;
	Status  status;

	if (!(status = Str::UTF8::ToWChar(szMask, &wsMask)))
	{
		return status;
	}
	if (NULL != szPath)
	{
		WString wsPath;
		bool    bContinue;

		if (!(status = Str::UTF8::ToWChar(szPath, &wsPath)))
		{
			return status;
		}

		return Enum(pHandler, wsMask.c_str(), wsPath.c_str(), &bContinue);
	}
	else
	{
		return EnumDrives(pHandler, wsMask.c_str());
	}
}

}//namespace IO

}//namespace CX


#endif
