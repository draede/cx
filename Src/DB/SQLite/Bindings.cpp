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
#include "CX/DB/SQLite/Bindings.hpp"
#include "CX/C/stdarg.h"


namespace CX
{

namespace DB
{

namespace SQLite
{

Bindings::Bindings()
{
}

Bindings::~Bindings()
{
	Clear();
}

Status Bindings::AddNull()
{
	Arg arg;

	arg.nType           = ArgType_Null;
	arg.nStoreType      = ArgStore_Static;
	arg.pfnFreeArgStore = NULL;

	m_vectorArgs.push_back(arg);

	return Status();
}

Status Bindings::AddInt(Int64 nValue)
{
	Arg arg;

	arg.nType           = ArgType_Int;
	arg.nStoreType      = ArgStore_Static;
	arg.pfnFreeArgStore = NULL;
	arg.nIntValue       = nValue;

	m_vectorArgs.push_back(arg);

	return Status();
}

Status Bindings::AddReal(Double lfValue)
{
	Arg arg;

	arg.nType           = ArgType_Real;
	arg.nStoreType      = ArgStore_Static;
	arg.pfnFreeArgStore = NULL;
	arg.lfRealValue     = lfValue;

	m_vectorArgs.push_back(arg);

	return Status();
}

Status Bindings::AddString(const Char *szString, Size cLen/* = (Size)-1*/, 
                           ArgStoreType nArgStoreType/* = ArgStore_Transient*/, 
                           FreeArgStoreProc pfnFreeArgStore/* = NULL*/)
{
	Arg arg;

	if ((Size)-1 == cLen)
	{
		cLen = cx_strlen(szString);
	}
	if (ArgStore_Static == nArgStoreType)
	{
		arg.nType           = ArgType_String;
		arg.nStoreType      = ArgStore_Static;
		arg.pfnFreeArgStore = NULL;
		arg.str.szString    = (Char *)szString;
		arg.str.cLen        = cLen;
	}
	else
	if (ArgStore_Transient == nArgStoreType)
	{
		Char *szTmp;

		if (NULL == (szTmp = (Char *)Mem::Alloc(cLen + 1)))
		{
			return Status_MemAllocFailed;
		}
		memcpy(szTmp, szString, cLen);
		szTmp[cLen]         = 0;
		arg.nType           = ArgType_String;
		arg.nStoreType      = ArgStore_Custom;
		arg.pfnFreeArgStore = &Bindings::FreeArgStore;
		arg.str.szString    = szTmp;
		arg.str.cLen        = cLen;
	}
	else
	if (ArgStore_Custom == nArgStoreType)
	{
		arg.nType           = ArgType_String;
		arg.nStoreType      = ArgStore_Custom;
		arg.pfnFreeArgStore = pfnFreeArgStore;
		arg.str.szString    = (Char *)szString;
		arg.str.cLen        = cLen;
	}
	else
	{
		return Status_InvalidArg;
	}

	m_vectorArgs.push_back(arg);

	return Status();
}

Status Bindings::AddWString(const WChar *wszString, Size cLen/* = (Size)-1*/, 
                            ArgStoreType nArgStoreType/* = ArgStore_Transient*/, 
                            FreeArgStoreProc pfnFreeArgStore/* = NULL*/)
{
	Arg arg;

	if ((Size)-1 == cLen)
	{
		cLen = cxw_strlen(wszString);
	}
	if (ArgStore_Static == nArgStoreType)
	{
		arg.nType           = ArgType_WString;
		arg.nStoreType      = ArgStore_Static;
		arg.pfnFreeArgStore = NULL;
		arg.wstr.wszString  = (WChar *)wszString;
		arg.wstr.cLen       = cLen;
	}
	else
	if (ArgStore_Transient == nArgStoreType)
	{
		WChar *wszTmp;

		if (NULL == (wszTmp = (WChar *)Mem::Alloc(sizeof(WChar) * (cLen + 1))))
		{
			return Status_MemAllocFailed;
		}
		memcpy(wszTmp, wszString, sizeof(WChar) * cLen);
		wszTmp[cLen]        = 0;
		arg.nType           = ArgType_WString;
		arg.nStoreType      = ArgStore_Custom;
		arg.pfnFreeArgStore = &Bindings::FreeArgStore;
		arg.wstr.wszString  = wszTmp;
		arg.wstr.cLen       = cLen;
	}
	else
	if (ArgStore_Custom == nArgStoreType)
	{
		arg.nType           = ArgType_WString;
		arg.nStoreType      = ArgStore_Custom;
		arg.pfnFreeArgStore = pfnFreeArgStore;
		arg.wstr.wszString   = (WChar *)wszString;
		arg.wstr.cLen       = cLen;
	}
	else
	{
		return Status_InvalidArg;
	}

	m_vectorArgs.push_back(arg);

	return Status();
}

Status Bindings::AddBLOB(const void *pData, Size cbSize, ArgStoreType nArgStoreType/* = ArgStore_Default*/, 
                         FreeArgStoreProc pfnFreeArgStore/* = NULL*/)
{
	Arg arg;

	if (ArgStore_Static == nArgStoreType)
	{
		arg.nType           = ArgType_BLOB;
		arg.nStoreType      = ArgStore_Static;
		arg.pfnFreeArgStore = NULL;
		arg.blob.pBLOB      = (void *)pData;
		arg.blob.cbSize     = cbSize;
	}
	else
	if (ArgStore_Transient == nArgStoreType)
	{
		void *pTmp;

		if (NULL == (pTmp = (Char *)Mem::Alloc(cbSize)))
		{
			return Status_MemAllocFailed;
		}
		memcpy(pTmp, pData, cbSize);
		arg.nType           = ArgType_BLOB;
		arg.nStoreType      = ArgStore_Custom;
		arg.pfnFreeArgStore = &Bindings::FreeArgStore;
		arg.blob.pBLOB      = pTmp;
		arg.blob.cbSize     = cbSize;
	}
	else
	if (ArgStore_Custom == nArgStoreType)
	{
		arg.nType           = ArgType_BLOB;
		arg.nStoreType      = ArgStore_Custom;
		arg.pfnFreeArgStore = pfnFreeArgStore;
		arg.blob.pBLOB      = (void *)pData;
		arg.blob.cbSize     = cbSize;
	}
	else
	{
		return Status_InvalidArg;
	}

	m_vectorArgs.push_back(arg);

	return Status();

}

Status Bindings::AddZeroBLOB(Size cbSize)
{
	Arg arg;

	arg.nType           = ArgType_ZeroBLOB;
	arg.nStoreType      = ArgStore_Static;
	arg.pfnFreeArgStore = NULL;
	arg.cbZeroBLOB      = cbSize;


	m_vectorArgs.push_back(arg);

	return Status();
}

Status Bindings::Clear()
{
	for (ArgsVector::iterator iter = m_vectorArgs.begin(); iter != m_vectorArgs.end(); ++iter)
	{
		if (ArgType_String == iter->nType)
		{
			if (NULL != iter->str.szString && ArgStore_Custom == iter->nStoreType && NULL != iter->pfnFreeArgStore)
			{
				iter->pfnFreeArgStore(iter->str.szString);
				iter->str.szString = NULL;
				iter->str.cLen     = 0;
			}
		}
		else
		if (ArgType_WString == iter->nType)
		{
			if (NULL != iter->wstr.wszString && ArgStore_Custom == iter->nStoreType && NULL != iter->pfnFreeArgStore)
			{
				iter->pfnFreeArgStore(iter->wstr.wszString);
				iter->wstr.wszString = NULL;
				iter->wstr.cLen      = 0;
			}
		}
		else
		if (ArgType_BLOB == iter->nType)
		{
			if (NULL != iter->blob.pBLOB && ArgStore_Custom == iter->nStoreType && NULL != iter->pfnFreeArgStore)
			{
				iter->pfnFreeArgStore(iter->blob.pBLOB);
				iter->blob.pBLOB  = NULL;
				iter->blob.cbSize = 0;
			}
		}
	}
	m_vectorArgs.clear();

	return Status();
}

Size Bindings::GetArgsCount() const
{
	return m_vectorArgs.size();
}

Arg *Bindings::GetArg(Size cIndex)
{
	if (cIndex >= m_vectorArgs.size())
	{
		return NULL;
	}

	return &m_vectorArgs[cIndex];
}

void Bindings::FreeArgStore(void *pData)
{
	Mem::Free(pData);
}

}//namespace SQLite

}//namespace DB

}//namespace CX

