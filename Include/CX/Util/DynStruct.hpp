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

#pragma once


#include "CX/Util/DynMember.hpp"
#include "CX/Map.hpp"


namespace CX
{

namespace Util
{

class DynStruct
{
public:

	DynStruct();

	DynStruct(const DynStruct &ds);

	~DynStruct();

	DynStruct &operator=(const DynStruct &ds);

	Status Copy(const DynStruct &ds);

	template <typename T>
	Status Set(const String &sName, const T &value)
	{
		if (sName.empty())
		{
			return Status_InvalidArg;
		}

		MembersMap::iterator iter;

		if (m_mapMembers.end() == (iter = m_mapMembers.find(sName)))
		{
			DynMember *pMember;
			Status    status;

			if (NULL == (pMember = new (std::nothrow) DynMember()))
			{
				return Status_MemAllocFailed;
			}
			if ((status = pMember->Set(value)).IsNOK())
			{
				delete pMember;

				return status;
			}
			m_mapMembers[sName] = pMember;

			return Status();
		}
		else
		{
			return m_mapMembers[sName]->Set(value);
		}
	}

	template <typename T>
	Status Get(const String &sName, T &value) const
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

		return iter->second->Get(value);
	}

	Status Remove(const String &sName);

	Status RemoveAll();

	bool Exists(const String &sName) const;

	Size GetCount() const;

	const String &GetName(Size cIndex);

	DynMember::Type GetType(const String &sName) const;

	Status SetBool(const String &sName, Bool value);

	Status SetInt8(const String &sName, Int8 value);

	Status SetUInt8(const String &sName, UInt8 value);

	Status SetInt16(const String &sName, Int16 value);

	Status SetUInt16(const String &sName, UInt16 value);

	Status SetInt32(const String &sName, Int32 value);

	Status SetUInt32(const String &sName, UInt32 value);

	Status SetInt64(const String &sName, Int64 value);

	Status SetUInt64(const String &sName, UInt64 value);

	Status SetFloat(const String &sName, Float value);

	Status SetDouble(const String &sName, Double value);

	Status SetChar(const String &sName, Char value);

	Status SetWChar(const String &sName, WChar value);

	Status SetString(const String &sName, const String &value);

	Status SetWString(const String &sName, const WString &value);

	Status SetBoolSet(const String &sName, const DynMember::BoolSet value);

	Status SetInt8Set(const String &sName, const DynMember::Int8Set value);

	Status SetUInt8Set(const String &sName, const DynMember::UInt8Set value);

	Status SetInt16Set(const String &sName, const DynMember::Int16Set value);

	Status SetUInt16Set(const String &sName, const DynMember::UInt16Set value);

	Status SetInt32Set(const String &sName, const DynMember::Int8Set value);

	Status SetUInt32Set(const String &sName, const DynMember::UInt8Set value);

	Status SetInt64Set(const String &sName, const DynMember::Int8Set value);

	Status SetUInt64Set(const String &sName, const DynMember::UInt8Set value);

	Status SetFloatSet(const String &sName, const DynMember::FloatSet value);

	Status SetDoubleSet(const String &sName, const DynMember::DoubleSet value);

	Status SetCharSet(const String &sName, const DynMember::CharSet value);

	Status SetWCharSet(const String &sName, const DynMember::WCharSet value);

	Status SetStringSet(const String &sName, const DynMember::StringSet &value);

	Status SetWStringSet(const String &sName, const DynMember::WStringSet &value);

	Bool GetBool(const String &sName, Bool defaultValue = False, Status *pStatus = NULL) const;

	Int8 GetInt8(const String &sName, Int8 defaultValue = 0, Status *pStatus = NULL) const;

	UInt8 GetUInt8(const String &sName, UInt8 defaultValue = 0, Status *pStatus = NULL) const;

	Int16 GetInt16(const String &sName, Int16 defaultValue = 0, Status *pStatus = NULL) const;

	UInt16 GetUInt16(const String &sName, UInt16 defaultValue = 0, Status *pStatus = NULL) const;

	Int32 GetInt32(const String &sName, Int32 defaultValue = 0, Status *pStatus = NULL) const;

	UInt32 GetUInt32(const String &sName, UInt32 defaultValue = 0, Status *pStatus = NULL) const;

	Int64 GetInt64(const String &sName, Int64 defaultValue = 0, Status *pStatus = NULL) const;

	UInt64 GetUInt64(const String &sName, UInt64 defaultValue = 0, Status *pStatus = NULL) const;

	Float GetFloat(const String &sName, Float defaultValue = 0.0f, Status *pStatus = NULL) const;

	Double GetDouble(const String &sName, Double defaultValue = 0.0, Status *pStatus = NULL) const;

	Char GetChar(const String &sName, Char defaultValue = '\0', Status *pStatus = NULL) const;

	WChar GetWChar(const String &sName, WChar defaultValue = L'\0', Status *pStatus = NULL) const;

	const String &GetString(const String &sName, const String &defaultValue = "", Status *pStatus = NULL) const;

	const WString &GetWString(const String &sName, const WString &defaultValue = L"", Status *pStatus = NULL) const;

	const DynMember::BoolSet &GetBoolSet(const String &sName, Status *pStatus = NULL) const;

	const DynMember::Int8Set &GetInt8Set(const String &sName, Status *pStatus = NULL) const;

	const DynMember::UInt8Set &GetUInt8Set(const String &sName, Status *pStatus = NULL) const;

	const DynMember::Int16Set &GetInt16Set(const String &sName, Status *pStatus = NULL) const;

	const DynMember::UInt16Set &GetUInt16Set(const String &sName, Status *pStatus = NULL) const;

	const DynMember::Int32Set &GetInt32Set(const String &sName, Status *pStatus = NULL) const;

	const DynMember::UInt32Set &GetUInt32Set(const String &sName, Status *pStatus = NULL) const;

	const DynMember::Int64Set &GetInt64Set(const String &sName, Status *pStatus = NULL) const;

	const DynMember::UInt64Set &GetUInt64Set(const String &sName, Status *pStatus = NULL) const;

	const DynMember::FloatSet &GetFloatSet(const String &sName, Status *pStatus = NULL) const;

	const DynMember::DoubleSet &GetDoubleSet(const String &sName, Status *pStatus = NULL) const;

	const DynMember::CharSet &GetCharSet(const String &sName, Status *pStatus = NULL) const;

	const DynMember::WCharSet &GetWCharSet(const String &sName, Status *pStatus = NULL) const;

	const DynMember::StringSet &GetStringSet(const String &sName, Status *pStatus = NULL) const;

	const DynMember::WStringSet &GetWStringSet(const String &sName, Status *pStatus = NULL) const;

private:

	typedef Map<String, DynMember *>::Type    MembersMap;

	MembersMap m_mapMembers;

};

}//namespace Util

}//namespace CX
