
#pragma once


#include "CX/Platform.h"


#if defined(CX_OS_WINDOWS)


#include "CX/Str/UTF8.h"
#include "CX/C/string.h"
#include "CX/Status.h"
#include <windows.h>


namespace CX
{

namespace Str
{

UTF8::UTF8()
{
}

UTF8::~UTF8()
{
}

StatusCode UTF8::ToWChar(const Char *szUTF8, WString *psWChar, Size cUTF8Len/* = SIZET_MAX*/)
{
	Status::Clear();

	int cSize;

	if (SIZET_MAX == cUTF8Len)
	{
		cUTF8Len = cx_strlen(szUTF8);
	}
	if (0 == cUTF8Len)
	{
		psWChar->clear();
	}
	else
	{
		if (0 < (cSize = ::MultiByteToWideChar(CP_UTF8, 0, szUTF8, (int)cUTF8Len, NULL, 0)))
		{
			psWChar->resize(cSize);
			if (0 >= (cSize = ::MultiByteToWideChar(CP_UTF8, 0, szUTF8, (int)cUTF8Len, &(*psWChar)[0], cSize)))
			{
				return Status::Set(Status_ConversionFailed, "MultiByteToWideChar failed with code {1}", GetLastError());
			}
		}
		else
		{
			return Status::Set(Status_ConversionFailed, "MultiByteToWideChar failed with code {1}", GetLastError());
		}
	}

	return Status_OK;
}

StatusCode UTF8::FromWChar(const WChar *wszWChar, String *psUTF8, Size cWCharLen/* = SIZET_MAX*/)
{
	Status::Clear();

	int cSize;

	if (SIZET_MAX == cWCharLen)
	{
		cWCharLen = wcslen(wszWChar);
	}
	if (0 == cWCharLen)
	{
		psUTF8->clear();
	}
	else
	{
		if (0 < (cSize = ::WideCharToMultiByte(CP_UTF8, 0, wszWChar, (int)cWCharLen, NULL, 0, NULL, NULL)))
		{
			psUTF8->resize(cSize);
			if (0 >= (cSize = ::WideCharToMultiByte(CP_UTF8, 0, wszWChar, (int)cWCharLen, &(*psUTF8)[0], cSize, NULL, NULL)))
			{
				return Status::Set(Status_ConversionFailed, "WideCharToMultiByte failed with code {1}", GetLastError());
			}
		}
		else
		{
			return Status::Set(Status_ConversionFailed, "WideCharToMultiByte failed with code {1}", GetLastError());
		}
	}

	return Status_OK;
}

}//namespace Str

}//namespace CX


#endif

