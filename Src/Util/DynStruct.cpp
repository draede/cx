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
#include "CX/Util/DynStruct.hpp"


namespace CX
{

namespace Util
{

DynStruct::DynStruct()
{
}

DynStruct::DynStruct(const DynStruct &ds)
{
	Copy(ds);
}

DynStruct::~DynStruct()
{
	RemoveAll();
}

DynStruct &DynStruct::operator=(const DynStruct &ds)
{
	Copy(ds);

	return *this;
}

Status DynStruct::Copy(const DynStruct &ds)
{
	Status status;

	RemoveAll();

	for (MembersMap::const_iterator iter = ds.m_mapMembers.begin(); iter != ds.m_mapMembers.end(); ++iter)
	{
		DynMember *pMember;

		if (NULL == (pMember = new (std::nothrow) DynMember()))
		{
			return Status_MemAllocFailed;
		}
		if ((status = pMember->Copy(*iter->second)).IsNOK())
		{
			delete pMember;

			return Status();
		}
		m_mapMembers[iter->first] = pMember;
	}

	return Status();
}

Status DynStruct::Remove(const String &sName)
{
	if (sName.empty())
	{
		return Status_InvalidArg;
	}

	MembersMap::const_iterator iter;

	if (m_mapMembers.end() == (iter = m_mapMembers.find(sName)))
	{
		return Status_NotFound;
	}

	iter->second->Reset();
	m_mapMembers.erase(iter);

	return Status();
}

Status DynStruct::RemoveAll()
{
	for (MembersMap::iterator iter = m_mapMembers.begin(); iter != m_mapMembers.end(); ++iter)
	{
		iter->second->Reset();
	}
	m_mapMembers.clear();

	return Status();
}

bool DynStruct::Exists(const String &sName) const
{
	return (m_mapMembers.end() != m_mapMembers.find(sName));
}

Size DynStruct::GetCount() const
{
	return m_mapMembers.size();
}

const String &DynStruct::GetName(Size cIndex)
{
	static const String empty;

	if (m_mapMembers.size() <= cIndex)
	{
		return empty;
	}

	MembersMap::const_iterator iter;

	iter = m_mapMembers.begin();
	while (0 < cIndex)
	{
		cIndex--;
		++iter;
	}

	return iter->first;
}

DynMember::Type DynStruct::GetType(const String &sName) const
{
	if (sName.empty())
	{
		return DynMember::Type_Invalid;
	}

	MembersMap::const_iterator iter;

	if (m_mapMembers.end() == (iter = m_mapMembers.find(sName)))
	{
		return DynMember::Type_Invalid;;
	}

	return iter->second->nType;
}

Status DynStruct::SetBool(const String &sName, Bool value)
{
	return Set(sName, value);
}

Status DynStruct::SetInt8(const String &sName, Int8 value)
{
	return Set(sName, value);
}

Status DynStruct::SetUInt8(const String &sName, UInt8 value)
{
	return Set(sName, value);
}

Status DynStruct::SetInt16(const String &sName, Int16 value)
{
	return Set(sName, value);
}

Status DynStruct::SetUInt16(const String &sName, UInt16 value)
{
	return Set(sName, value);
}

Status DynStruct::SetInt32(const String &sName, Int32 value)
{
	return Set(sName, value);
}

Status DynStruct::SetUInt32(const String &sName, UInt32 value)
{
	return Set(sName, value);
}

Status DynStruct::SetInt64(const String &sName, Int64 value)
{
	return Set(sName, value);
}

Status DynStruct::SetUInt64(const String &sName, UInt64 value)
{
	return Set(sName, value);
}

Status DynStruct::SetFloat(const String &sName, Float value)
{
	return Set(sName, value);
}

Status DynStruct::SetDouble(const String &sName, Double value)
{
	return Set(sName, value);
}

Status DynStruct::SetChar(const String &sName, Char value)
{
	return Set(sName, value);
}

Status DynStruct::SetWChar(const String &sName, WChar value)
{
	return Set(sName, value);
}

Status DynStruct::SetString(const String &sName, const String &value)
{
	return Set(sName, value);
}

Status DynStruct::SetWString(const String &sName, const WString &value)
{
	return Set(sName, value);
}

Status DynStruct::SetBoolSet(const String &sName, const DynMember::BoolSet value)
{
	return Set(sName, value);
}

Status DynStruct::SetInt8Set(const String &sName, const DynMember::Int8Set value)
{
	return Set(sName, value);
}

Status DynStruct::SetUInt8Set(const String &sName, const DynMember::UInt8Set value)
{
	return Set(sName, value);
}

Status DynStruct::SetInt16Set(const String &sName, const DynMember::Int16Set value)
{
	return Set(sName, value);
}

Status DynStruct::SetUInt16Set(const String &sName, const DynMember::UInt16Set value)
{
	return Set(sName, value);
}

Status DynStruct::SetInt32Set(const String &sName, const DynMember::Int8Set value)
{
	return Set(sName, value);
}

Status DynStruct::SetUInt32Set(const String &sName, const DynMember::UInt8Set value)
{
	return Set(sName, value);
}

Status DynStruct::SetInt64Set(const String &sName, const DynMember::Int8Set value)
{
	return Set(sName, value);
}

Status DynStruct::SetUInt64Set(const String &sName, const DynMember::UInt8Set value)
{
	return Set(sName, value);
}

Status DynStruct::SetFloatSet(const String &sName, const DynMember::FloatSet value)
{
	return Set(sName, value);
}

Status DynStruct::SetDoubleSet(const String &sName, const DynMember::DoubleSet value)
{
	return Set(sName, value);
}

Status DynStruct::SetCharSet(const String &sName, const DynMember::CharSet value)
{
	return Set(sName, value);
}

Status DynStruct::SetWCharSet(const String &sName, const DynMember::WCharSet value)
{
	return Set(sName, value);
}

Status DynStruct::SetStringSet(const String &sName, const DynMember::StringSet &value)
{
	return Set(sName, value);
}

Status DynStruct::SetWStringSet(const String &sName, const DynMember::WStringSet &value)
{
	return Set(sName, value);
}

Bool DynStruct::GetBool(const String &sName, Bool defaultValue/* = False*/, Status *pStatus/* = NULL*/) const
{
	Bool   value;
	Status status;

	if ((status = Get(sName, value)).IsNOK())
	{
		if (NULL != pStatus)
		{
			*pStatus = status;
		}

		return defaultValue;
	}
	if (NULL != pStatus)
	{
		pStatus->Clear();
	}

	return value;
}

Int8 DynStruct::GetInt8(const String &sName, Int8 defaultValue/* = 0*/, Status *pStatus/* = NULL*/) const
{
	Int8   value;
	Status status;

	if ((status = Get(sName, value)).IsNOK())
	{
		if (NULL != pStatus)
		{
			*pStatus = status;
		}

		return defaultValue;
	}
	if (NULL != pStatus)
	{
		pStatus->Clear();
	}

	return value;
}

UInt8 DynStruct::GetUInt8(const String &sName, UInt8 defaultValue/* = 0*/, Status *pStatus/* = NULL*/) const
{
	UInt8  value;
	Status status;

	if ((status = Get(sName, value)).IsNOK())
	{
		if (NULL != pStatus)
		{
			*pStatus = status;
		}

		return defaultValue;
	}
	if (NULL != pStatus)
	{
		pStatus->Clear();
	}

	return value;
}


Int16 DynStruct::GetInt16(const String &sName, Int16 defaultValue/* = 0*/, Status *pStatus/* = NULL*/) const
{
	Int16  value;
	Status status;

	if ((status = Get(sName, value)).IsNOK())
	{
		if (NULL != pStatus)
		{
			*pStatus = status;
		}

		return defaultValue;
	}
	if (NULL != pStatus)
	{
		pStatus->Clear();
	}

	return value;
}

UInt16 DynStruct::GetUInt16(const String &sName, UInt16 defaultValue/* = 0*/, Status *pStatus/* = NULL*/) const
{
	UInt16 value;
	Status status;

	if ((status = Get(sName, value)).IsNOK())
	{
		if (NULL != pStatus)
		{
			*pStatus = status;
		}

		return defaultValue;
	}
	if (NULL != pStatus)
	{
		pStatus->Clear();
	}

	return value;
}

Int32 DynStruct::GetInt32(const String &sName, Int32 defaultValue/* = 0*/, Status *pStatus/* = NULL*/) const
{
	Int32  value;
	Status status;

	if ((status = Get(sName, value)).IsNOK())
	{
		if (NULL != pStatus)
		{
			*pStatus = status;
		}

		return defaultValue;
	}
	if (NULL != pStatus)
	{
		pStatus->Clear();
	}

	return value;
}

UInt32 DynStruct::GetUInt32(const String &sName, UInt32 defaultValue/* = 0*/, Status *pStatus/* = NULL*/) const
{
	UInt32 value;
	Status status;

	if ((status = Get(sName, value)).IsNOK())
	{
		if (NULL != pStatus)
		{
			*pStatus = status;
		}

		return defaultValue;
	}
	if (NULL != pStatus)
	{
		pStatus->Clear();
	}

	return value;
}

Int64 DynStruct::GetInt64(const String &sName, Int64 defaultValue/* = 0*/, Status *pStatus/* = NULL*/) const
{
	Int64  value;
	Status status;

	if ((status = Get(sName, value)).IsNOK())
	{
		if (NULL != pStatus)
		{
			*pStatus = status;
		}

		return defaultValue;
	}
	if (NULL != pStatus)
	{
		pStatus->Clear();
	}

	return value;
}

UInt64 DynStruct::GetUInt64(const String &sName, UInt64 defaultValue/* = 0*/, Status *pStatus/* = NULL*/) const
{
	UInt64 value;
	Status status;

	if ((status = Get(sName, value)).IsNOK())
	{
		if (NULL != pStatus)
		{
			*pStatus = status;
		}

		return defaultValue;
	}
	if (NULL != pStatus)
	{
		pStatus->Clear();
	}

	return value;
}

Float DynStruct::GetFloat(const String &sName, Float defaultValue/* = 0.0f*/, Status *pStatus/* = NULL*/) const
{
	Float  value;
	Status status;

	if ((status = Get(sName, value)).IsNOK())
	{
		if (NULL != pStatus)
		{
			*pStatus = status;
		}

		return defaultValue;
	}
	if (NULL != pStatus)
	{
		pStatus->Clear();
	}

	return value;
}

Double DynStruct::GetDouble(const String &sName, Double defaultValue/* = 0.0*/, Status *pStatus/* = NULL*/) const
{
	Double value;
	Status status;

	if ((status = Get(sName, value)).IsNOK())
	{
		if (NULL != pStatus)
		{
			*pStatus = status;
		}

		return defaultValue;
	}
	if (NULL != pStatus)
	{
		pStatus->Clear();
	}

	return value;
}

Char DynStruct::GetChar(const String &sName, Char defaultValue/* = '\0'*/, Status *pStatus/* = NULL*/) const
{
	Char   value;
	Status status;

	if ((status = Get(sName, value)).IsNOK())
	{
		if (NULL != pStatus)
		{
			*pStatus = status;
		}

		return defaultValue;
	}
	if (NULL != pStatus)
	{
		pStatus->Clear();
	}

	return value;
}

WChar DynStruct::GetWChar(const String &sName, WChar defaultValue/* = L'\0'*/, Status *pStatus/* = NULL*/) const
{
	WChar  value;
	Status status;

	if ((status = Get(sName, value)).IsNOK())
	{
		if (NULL != pStatus)
		{
			*pStatus = status;
		}

		return defaultValue;
	}
	if (NULL != pStatus)
	{
		pStatus->Clear();
	}

	return value;
}

const String &DynStruct::GetString(const String &sName, const String &defaultValue/* = ""*/, 
                                   Status *pStatus/* = NULL*/) const
{
	static String value;
	Status        status;

	value.clear();
	if ((status = Get(sName, value)).IsNOK())
	{
		if (NULL != pStatus)
		{
			*pStatus = status;
		}
		value = defaultValue;

		return value;
	}
	if (NULL != pStatus)
	{
		pStatus->Clear();
	}

	return value;
}

const WString &DynStruct::GetWString(const String &sName, const WString &defaultValue/* = L""*/, 
                                     Status *pStatus/* = NULL*/) const
{
	static WString value;
	Status         status;

	value.clear();
	if ((status = Get(sName, value)).IsNOK())
	{
		if (NULL != pStatus)
		{
			*pStatus = status;
		}
		value = defaultValue;

		return value;
	}
	if (NULL != pStatus)
	{
		pStatus->Clear();
	}

	return value;
}

const DynMember::BoolSet &DynStruct::GetBoolSet(const String &sName, Status *pStatus/* = NULL*/) const
{
	static DynMember::BoolSet value;
	Status                    status;

	value.clear();
	if ((status = Get(sName, value)).IsNOK())
	{
		if (NULL != pStatus)
		{
			*pStatus = status;
		}
		value.clear();

		return value;
	}
	if (NULL != pStatus)
	{
		pStatus->Clear();
	}

	return value;
}

const DynMember::Int8Set &DynStruct::GetInt8Set(const String &sName, Status *pStatus/* = NULL*/) const
{
	static DynMember::Int8Set value;
	Status                    status;

	value.clear();
	if ((status = Get(sName, value)).IsNOK())
	{
		if (NULL != pStatus)
		{
			*pStatus = status;
		}
		value.clear();

		return value;
	}
	if (NULL != pStatus)
	{
		pStatus->Clear();
	}

	return value;
}

const DynMember::UInt8Set &DynStruct::GetUInt8Set(const String &sName, Status *pStatus/* = NULL*/) const
{
	static DynMember::UInt8Set value;
	Status                     status;

	value.clear();
	if ((status = Get(sName, value)).IsNOK())
	{
		if (NULL != pStatus)
		{
			*pStatus = status;
		}
		value.clear();

		return value;
	}
	if (NULL != pStatus)
	{
		pStatus->Clear();
	}

	return value;
}

const DynMember::Int16Set &DynStruct::GetInt16Set(const String &sName, Status *pStatus/* = NULL*/) const
{
	static DynMember::Int16Set value;
	Status                     status;

	value.clear();
	if ((status = Get(sName, value)).IsNOK())
	{
		if (NULL != pStatus)
		{
			*pStatus = status;
		}
		value.clear();

		return value;
	}
	if (NULL != pStatus)
	{
		pStatus->Clear();
	}

	return value;
}

const DynMember::UInt16Set &DynStruct::GetUInt16Set(const String &sName, Status *pStatus/* = NULL*/) const
{
	static DynMember::UInt16Set value;
	Status                      status;

	value.clear();
	if ((status = Get(sName, value)).IsNOK())
	{
		if (NULL != pStatus)
		{
			*pStatus = status;
		}
		value.clear();

		return value;
	}
	if (NULL != pStatus)
	{
		pStatus->Clear();
	}

	return value;
}

const DynMember::Int32Set &DynStruct::GetInt32Set(const String &sName, Status *pStatus/* = NULL*/) const
{
	static DynMember::Int32Set value;
	Status                     status;

	value.clear();
	if ((status = Get(sName, value)).IsNOK())
	{
		if (NULL != pStatus)
		{
			*pStatus = status;
		}
		value.clear();

		return value;
	}
	if (NULL != pStatus)
	{
		pStatus->Clear();
	}

	return value;
}

const DynMember::UInt32Set &DynStruct::GetUInt32Set(const String &sName, Status *pStatus/* = NULL*/) const
{
	static DynMember::UInt32Set value;
	Status                      status;

	value.clear();
	if ((status = Get(sName, value)).IsNOK())
	{
		if (NULL != pStatus)
		{
			*pStatus = status;
		}
		value.clear();

		return value;
	}
	if (NULL != pStatus)
	{
		pStatus->Clear();
	}

	return value;
}

const DynMember::Int64Set &DynStruct::GetInt64Set(const String &sName, Status *pStatus/* = NULL*/) const
{
	static DynMember::Int64Set value;
	Status                     status;

	value.clear();
	if ((status = Get(sName, value)).IsNOK())
	{
		if (NULL != pStatus)
		{
			*pStatus = status;
		}
		value.clear();

		return value;
	}
	if (NULL != pStatus)
	{
		pStatus->Clear();
	}

	return value;
}

const DynMember::UInt64Set &DynStruct::GetUInt64Set(const String &sName, Status *pStatus/* = NULL*/) const
{
	static DynMember::UInt64Set value;
	Status                      status;

	value.clear();
	if ((status = Get(sName, value)).IsNOK())
	{
		if (NULL != pStatus)
		{
			*pStatus = status;
		}
		value.clear();

		return value;
	}
	if (NULL != pStatus)
	{
		pStatus->Clear();
	}

	return value;
}

const DynMember::FloatSet &DynStruct::GetFloatSet(const String &sName, Status *pStatus/* = NULL*/) const
{
	static DynMember::FloatSet value;
	Status                     status;

	value.clear();
	if ((status = Get(sName, value)).IsNOK())
	{
		if (NULL != pStatus)
		{
			*pStatus = status;
		}
		value.clear();

		return value;
	}
	if (NULL != pStatus)
	{
		pStatus->Clear();
	}

	return value;
}

const DynMember::DoubleSet &DynStruct::GetDoubleSet(const String &sName, Status *pStatus/* = NULL*/) const
{
	static DynMember::DoubleSet value;
	Status                     status;

	value.clear();
	if ((status = Get(sName, value)).IsNOK())
	{
		if (NULL != pStatus)
		{
			*pStatus = status;
		}
		value.clear();

		return value;
	}
	if (NULL != pStatus)
	{
		pStatus->Clear();
	}

	return value;
}

const DynMember::CharSet &DynStruct::GetCharSet(const String &sName, Status *pStatus/* = NULL*/) const
{
	static DynMember::CharSet value;
	Status                    status;

	value.clear();
	if ((status = Get(sName, value)).IsNOK())
	{
		if (NULL != pStatus)
		{
			*pStatus = status;
		}
		value.clear();

		return value;
	}
	if (NULL != pStatus)
	{
		pStatus->Clear();
	}

	return value;
}

const DynMember::WCharSet &DynStruct::GetWCharSet(const String &sName, Status *pStatus/* = NULL*/) const
{
	static DynMember::WCharSet value;
	Status                     status;

	value.clear();
	if ((status = Get(sName, value)).IsNOK())
	{
		if (NULL != pStatus)
		{
			*pStatus = status;
		}
		value.clear();

		return value;
	}
	if (NULL != pStatus)
	{
		pStatus->Clear();
	}

	return value;
}

const DynMember::StringSet &DynStruct::GetStringSet(const String &sName, Status *pStatus/* = NULL*/) const
{
	static DynMember::StringSet value;
	Status                      status;

	value.clear();
	if ((status = Get(sName, value)).IsNOK())
	{
		if (NULL != pStatus)
		{
			*pStatus = status;
		}
		value.clear();

		return value;
	}
	if (NULL != pStatus)
	{
		pStatus->Clear();
	}

	return value;
}

const DynMember::WStringSet &DynStruct::GetWStringSet(const String &sName, Status *pStatus/* = NULL*/) const
{
	static DynMember::WStringSet value;
	Status                       status;

	value.clear();
	if ((status = Get(sName, value)).IsNOK())
	{
		if (NULL != pStatus)
		{
			*pStatus = status;
		}
		value.clear();

		return value;
	}
	if (NULL != pStatus)
	{
		pStatus->Clear();
	}

	return value;
}

}//namespace Util

}//namespace CX
