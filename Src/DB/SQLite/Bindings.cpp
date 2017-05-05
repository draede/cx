/* 
 * CX - C++ framework for general purpose development
 *
 * https://github.com/draede/cx
 * 
 * Copyright (C) 2014 - 2017 draede - draede [at] outlook [dot] com
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

Bindings::Bindings(const Char *szArgsType, ...)
{
	va_list vl;

	va_start(vl, szArgsType);
	Init(szArgsType, vl);
	va_end(vl);
}

Bindings::Bindings(const Char *szArgsType, va_list vl)
{
	Init(szArgsType, vl);
}

Bindings::Bindings(const Bindings &bindings)
{
	Copy(bindings);
}

Bindings::~Bindings()
{
	Clear();
}

Bindings &Bindings::operator=(const Bindings &bindings)
{
	Copy(bindings);

	return *this;
}

Status Bindings::Copy(const Bindings &bindings)
{
	Status status;

	Clear();
	for (ArgsVector::const_iterator iter = bindings.m_vectorArgs.begin(); iter != bindings.m_vectorArgs.end(); ++iter)
	{
		if (ArgType_Null == iter->nType)
		{
			if ((status = AddNull()).IsNOK())
			{
				return status;
			}
		}
		else
		if (ArgType_Int == iter->nType)
		{
			if ((status = AddInt(iter->nIntValue)).IsNOK())
			{
				return status;
			}
		}
		else
		if (ArgType_Real == iter->nType)
		{
			if ((status = AddReal(iter->lfRealValue)).IsNOK())
			{
				return status;
			}
		}
		else
		if (ArgType_String == iter->nType)
		{
			if ((status = AddString(iter->psString->c_str())).IsNOK())
			{
				return status;
			}
		}
		else
		if (ArgType_WString == iter->nType)
		{
			if ((status = AddWString(iter->pwsString->c_str())).IsNOK())
			{
				return status;
			}
		}
		else
		if (ArgType_BLOB == iter->nType)
		{
			if ((status = AddBLOB(iter->pBLOB->pData, iter->pBLOB->cbSize)).IsNOK())
			{
				return status;
			}
		}
		else
		if (ArgType_ZeroBLOB == iter->nType)
		{
			if ((status = AddZeroBLOB(iter->cbZeroBLOB)).IsNOK())
			{
				return status;
			}
		}
		else
		{
			return Status_InvalidArg;
		}
	}

	return Status();
}

Status Bindings::Init(const Char *szArgsType, ...)
{
	va_list vl;
	Status  status;

	va_start(vl, szArgsType);
	status = Init(szArgsType, vl);
	va_end(vl);

	return status;
}

Status Bindings::Init(const Char *szArgsType, va_list vl)
{
	const Char *pszPos;
	Status     status;

	pszPos = szArgsType;
	while (0 != *pszPos)
	{
		if ('n' == *pszPos)
		{
			if ((status = AddNull()).IsNOK())
			{
				break;
			}
		}
		else
		if ('i' == *pszPos)
		{
			Int64 nValue = va_arg(vl, Int64);

			if ((status = AddInt(nValue)).IsNOK())
			{
				break;
			}
		}
		else
		if ('r' == *pszPos)
		{
			Double lfValue = va_arg(vl, Double);

			if ((status = AddReal(lfValue)).IsNOK())
			{
				break;
			}
		}
		else
		if ('s' == *pszPos)
		{
			const Char *szValue = va_arg(vl, const Char *);

			if ((status = AddString(szValue)).IsNOK())
			{
				break;
			}
		}
		else
		if ('w' == *pszPos)
		{
			const WChar *wszValue = va_arg(vl, const WChar *);

			if ((status = AddWString(wszValue)).IsNOK())
			{
				break;
			}
		}
		else
		if ('b' == *pszPos)
		{
			const void *pData = va_arg(vl, const void *);
			Size       cbSize = va_arg(vl, Size);

			if ((status = AddBLOB(pData, cbSize)).IsNOK())
			{
				break;
			}
		}
		else
		if ('z' == *pszPos)
		{
			Size cbSize = va_arg(vl, Size);

			if ((status = AddZeroBLOB(cbSize)).IsNOK())
			{
				break;
			}
		}
		else
		{
			status = Status_InvalidArg;

			break;
		}
		pszPos++;
	}
	if (status.IsNOK())
	{
		Clear();
	}

	return Status();
}

Status Bindings::AddNull()
{
	Arg arg;

	arg.nType = ArgType_Null;

	m_vectorArgs.push_back(arg);

	return Status();
}

Status Bindings::AddInt(Int64 nValue)
{
	Arg arg;

	arg.nType     = ArgType_Int;
	arg.nIntValue = nValue;

	m_vectorArgs.push_back(arg);

	return Status();
}

Status Bindings::AddReal(Double lfValue)
{
	Arg arg;

	arg.nType       = ArgType_Real;
	arg.lfRealValue = lfValue;

	m_vectorArgs.push_back(arg);

	return Status();
}

Status Bindings::AddString(const Char *szString)
{
	Arg arg;

	arg.nType = ArgType_String;
	if (NULL == (arg.psString = new (std::nothrow) String(szString)))
	{
		return Status_MemAllocFailed;
	}

	m_vectorArgs.push_back(arg);

	return Status();
}

Status Bindings::AddWString(const WChar *wszString)
{
	Arg arg;

	arg.nType = ArgType_WString;
	if (NULL == (arg.pwsString = new (std::nothrow) WString(wszString)))
	{
		return Status_MemAllocFailed;
	}

	m_vectorArgs.push_back(arg);

	return Status();
}

Status Bindings::AddBLOB(const void *pData, Size cbSize)
{
	Arg arg;

	arg.nType = ArgType_BLOB;
	if (NULL == (arg.pBLOB = new (std::nothrow) BLOB()))
	{
		return Status_MemAllocFailed;
	}
	if (NULL == (arg.pBLOB->pData = malloc(cbSize)))
	{
		delete arg.pBLOB;
		return Status_MemAllocFailed;
	}
	memcpy(arg.pBLOB->pData, pData, cbSize);
	arg.pBLOB->cbSize = cbSize;

	m_vectorArgs.push_back(arg);

	return Status();
}

Status Bindings::AddZeroBLOB(Size cbSize)
{
	Arg arg;

	arg.nType      = ArgType_ZeroBLOB;
	arg.cbZeroBLOB = cbSize;

	m_vectorArgs.push_back(arg);

	return Status();
}

Status Bindings::Clear()
{
	for (ArgsVector::iterator iter = m_vectorArgs.begin(); iter != m_vectorArgs.end(); ++iter)
	{
		if (ArgType_String == iter->nType)
		{
			delete iter->psString;
		}
		else
		if (ArgType_WString == iter->nType)
		{
			delete iter->pwsString;
		}
		else
		if (ArgType_BLOB == iter->nType)
		{
			free(iter->pBLOB->pData);
			delete iter->pBLOB;
		}
	}
	m_vectorArgs.clear();

	return Status();
}

Size Bindings::GetArgsCount() const
{
	return m_vectorArgs.size();
}

Bindings::ArgType Bindings::GetArgType(Size cIndex) const
{
	if (cIndex >= m_vectorArgs.size())
	{
		return ArgType_Null;
	}

	return m_vectorArgs[cIndex].nType;
}

Int64 Bindings::GetInt(Size cIndex) const
{
	if (cIndex >= m_vectorArgs.size())
	{
		return 0;
	}
	if (ArgType_Int != m_vectorArgs[cIndex].nType)
	{
		return 0;
	}

	return m_vectorArgs[cIndex].nIntValue;
}

Double Bindings::GetReal(Size cIndex) const
{
	if (cIndex >= m_vectorArgs.size())
	{
		return 0.0;
	}
	if (ArgType_Real != m_vectorArgs[cIndex].nType)
	{
		return 0.0;
	}

	return m_vectorArgs[cIndex].lfRealValue;
}

const Char *Bindings::GetString(Size cIndex) const
{
	if (cIndex >= m_vectorArgs.size())
	{
		return "";
	}
	if (ArgType_String != m_vectorArgs[cIndex].nType)
	{
		return "";
	}

	return m_vectorArgs[cIndex].psString->c_str();
}

Size Bindings::GetStringLen(Size cIndex) const
{
	if (cIndex >= m_vectorArgs.size())
	{
		return 0;
	}
	if (ArgType_String != m_vectorArgs[cIndex].nType)
	{
		return 0;
	}

	return m_vectorArgs[cIndex].psString->size();
}

const WChar *Bindings::GetWString(Size cIndex) const
{
	if (cIndex >= m_vectorArgs.size())
	{
		return L"";
	}
	if (ArgType_WString != m_vectorArgs[cIndex].nType)
	{
		return L"";
	}

	return m_vectorArgs[cIndex].pwsString->c_str();
}

Size Bindings::GetWStringLen(Size cIndex) const
{
	if (cIndex >= m_vectorArgs.size())
	{
		return 0;
	}
	if (ArgType_WString != m_vectorArgs[cIndex].nType)
	{
		return 0;
	}

	return m_vectorArgs[cIndex].pwsString->size();
}

const void *Bindings::GetBLOB(Size cIndex) const
{
	if (cIndex >= m_vectorArgs.size())
	{
		return NULL;
	}
	if (ArgType_BLOB != m_vectorArgs[cIndex].nType)
	{
		return NULL;
	}

	return m_vectorArgs[cIndex].pBLOB->pData;
}

Size Bindings::GetBLOBSize(Size cIndex) const
{
	if (cIndex >= m_vectorArgs.size())
	{
		return 0;
	}
	if (ArgType_BLOB == m_vectorArgs[cIndex].nType)
	{
		return m_vectorArgs[cIndex].pBLOB->cbSize;
	}
	else
	if (ArgType_ZeroBLOB == m_vectorArgs[cIndex].nType)
	{
		return m_vectorArgs[cIndex].cbZeroBLOB;
	}
	else
	{
		return 0;
	}
}

}//namespace SQLite

}//namespace DB

}//namespace CX

