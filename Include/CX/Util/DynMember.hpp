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

#pragma once


#include "CX/Types.hpp"
#include "CX/Status.hpp"
#include "CX/Set.hpp"


namespace CX
{

namespace Util
{

struct DynMember
{
	enum Type
	{
		Type_Invalid,
		Type_Bool,
		Type_Int8,
		Type_UInt8,
		Type_Int16,
		Type_UInt16,
		Type_Int32,
		Type_UInt32,
		Type_Int64,
		Type_UInt64,
		Type_Float,
		Type_Double,
		Type_Char,
		Type_WChar,
		Type_String,
		Type_WString,
		Type_BoolSet,
		Type_Int8Set,
		Type_UInt8Set,
		Type_Int16Set,
		Type_UInt16Set,
		Type_Int32Set,
		Type_UInt32Set,
		Type_Int64Set,
		Type_UInt64Set,
		Type_CharSet,
		Type_WCharSet,
		Type_FloatSet,
		Type_DoubleSet,
		Type_StringSet,
		Type_WStringSet,
	};

	typedef Set<Bool>::Type      BoolSet;
	typedef Set<Int8>::Type      Int8Set;
	typedef Set<UInt8>::Type     UInt8Set;
	typedef Set<Int16>::Type     Int16Set;
	typedef Set<UInt16>::Type    UInt16Set;
	typedef Set<Int32>::Type     Int32Set;
	typedef Set<UInt32>::Type    UInt32Set;
	typedef Set<Int64>::Type     Int64Set;
	typedef Set<UInt64>::Type    UInt64Set;
	typedef Set<Float>::Type     FloatSet;
	typedef Set<Double>::Type    DoubleSet;
	typedef Set<Char>::Type      CharSet;
	typedef Set<WChar>::Type     WCharSet;
	typedef Set<String>::Type    StringSet;
	typedef Set<WString>::Type   WStringSet;

	Type nType;

	union
	{
		Bool       bBool;
		Int8       nInt8;
		UInt8      uUInt8;
		Int16      nInt16;
		UInt16     uUInt16;
		Int32      nInt32;
		UInt32     uUInt32;
		Int64      nInt64;
		UInt64     uUInt64;
		Float      fFloat;
		Double     lfDouble;
		Char       chChar;
		WChar      wchWChar;
		String     *psString;
		WString    *pwsString;
		BoolSet    *pSetBool;
		Int8Set    *pSetInt8;
		UInt8Set   *pSetUInt8;
		Int16Set   *pSetInt16;
		UInt16Set  *pSetUInt16;
		Int32Set   *pSetInt32;
		UInt32Set  *pSetUInt32;
		Int64Set   *pSetInt64;
		UInt64Set  *pSetUInt64;
		FloatSet   *pSetFloat;
		DoubleSet  *pSetDouble;
		CharSet    *pSetChar;
		WCharSet   *pSetWChar;
		StringSet  *pSetString;
		WStringSet *pSetWString;
	}v;

	DynMember();

	DynMember(const DynMember &member);

	~DynMember();

	DynMember &operator=(const DynMember &member);

	Status Copy(const DynMember &member);

	template <typename T> Status Set(const T &value);
#if defined(CX_OS_WINDOWS)
	template <> Status Set<Bool>(const Bool &value);
	template <> Status Set<Int8>(const Int8 &value);
	template <> Status Set<UInt8>(const UInt8 &value);
	template <> Status Set<Int16>(const Int16 &value);
	template <> Status Set<UInt16>(const UInt16 &value);
	template <> Status Set<Int32>(const Int32 &value);
	template <> Status Set<UInt32>(const UInt32 &value);
	template <> Status Set<Int64>(const Int64 &value);
	template <> Status Set<UInt64>(const UInt64 &value);
	template <> Status Set<Float>(const Float &value);
	template <> Status Set<Double>(const Double &value);
	template <> Status Set<Char>(const Char &value);
	template <> Status Set<WChar>(const WChar &value);
	template <> Status Set<String>(const String &value);
	template <> Status Set<WString>(const WString &value);
	template <> Status Set<BoolSet>(const BoolSet &value);
	template <> Status Set<Int8Set>(const Int8Set &value);
	template <> Status Set<UInt8Set>(const UInt8Set &value);
	template <> Status Set<Int16Set>(const Int16Set &value);
	template <> Status Set<UInt16Set>(const UInt16Set &value);
	template <> Status Set<Int32Set>(const Int32Set &value);
	template <> Status Set<UInt32Set>(const UInt32Set &value);
	template <> Status Set<Int64Set>(const Int64Set &value);
	template <> Status Set<UInt64Set>(const UInt64Set &value);
	template <> Status Set<FloatSet>(const FloatSet &value);
	template <> Status Set<DoubleSet>(const DoubleSet &value);
	template <> Status Set<CharSet>(const CharSet &value);
	template <> Status Set<WCharSet>(const WCharSet &value);
	template <> Status Set<StringSet>(const StringSet &value);
	template <> Status Set<WStringSet>(const WStringSet &value);
#endif

	template <typename T> Status Get(T &value) const;
#if defined(CX_OS_WINDOWS)
	template <> Status Get<Bool>(Bool &value) const;
	template <> Status Get<Int8>(Int8 &value) const;
	template <> Status Get<UInt8>(UInt8 &value) const;
	template <> Status Get<Int16>(Int16 &value) const;
	template <> Status Get<UInt16>(UInt16 &value) const;
	template <> Status Get<Int32>(Int32 &value) const;
	template <> Status Get<UInt32>(UInt32 &value) const;
	template <> Status Get<Int64>(Int64 &value) const;
	template <> Status Get<UInt64>(UInt64 &value) const;
	template <> Status Get<Float>(Float &value) const;
	template <> Status Get<Double>(Double &value) const;
	template <> Status Get<Char>(Char &value) const;
	template <> Status Get<WChar>(WChar &value) const;
	template <> Status Get<String>(String &value) const;
	template <> Status Get<WString>(WString &value) const;
	template <> Status Get<BoolSet>(BoolSet &value) const;
	template <> Status Get<Int8Set>(Int8Set &value) const;
	template <> Status Get<UInt8Set>(UInt8Set &value) const;
	template <> Status Get<Int16Set>(Int16Set &value) const;
	template <> Status Get<UInt16Set>(UInt16Set &value) const;
	template <> Status Get<Int32Set>(Int32Set &value) const;
	template <> Status Get<UInt32Set>(UInt32Set &value) const;
	template <> Status Get<Int64Set>(Int64Set &value) const;
	template <> Status Get<UInt64Set>(UInt64Set &value) const;
	template <> Status Get<FloatSet>(FloatSet &value) const;
	template <> Status Get<DoubleSet>(DoubleSet &value) const;
	template <> Status Get<CharSet>(CharSet &value) const;
	template <> Status Get<WCharSet>(WCharSet &value) const;
	template <> Status Get<StringSet>(StringSet &value) const;
	template <> Status Get<WStringSet>(WStringSet &value) const;
#endif

	Status Reset();

};

template <> Status DynMember::Set<Bool>(const Bool &value) { if (nType != Type_Bool) { Status s = Reset(); if (s.IsNOK()) return s; } v.bBool = value; nType = Type_Bool; return Status(); }
template <> Status DynMember::Set<Int8>(const Int8 &value) { if (nType != Type_Int8) { Status s = Reset(); if (s.IsNOK()) return s; } v.nInt8 = value; nType = Type_Int8; return Status(); }
template <> Status DynMember::Set<UInt8>(const UInt8 &value) { if (nType != Type_UInt8) { Status s = Reset(); if (s.IsNOK()) return s; } v.uUInt8 = value; nType = Type_UInt8; return Status(); }
template <> Status DynMember::Set<Int16>(const Int16 &value) { if (nType != Type_Int16) { Status s = Reset(); if (s.IsNOK()) return s; } v.nInt16 = value; nType = Type_Int16; return Status(); }
template <> Status DynMember::Set<UInt16>(const UInt16 &value) { if (nType != Type_UInt16) { Status s = Reset(); if (s.IsNOK()) return s; } v.uUInt16 = value; nType = Type_UInt16; return Status(); }
template <> Status DynMember::Set<Int32>(const Int32 &value) { if (nType != Type_Int32) { Status s = Reset(); if (s.IsNOK()) return s; } v.nInt32 = value; nType = Type_Int32; return Status(); }
template <> Status DynMember::Set<UInt32>(const UInt32 &value) { if (nType != Type_UInt32) { Status s = Reset(); if (s.IsNOK()) return s; } v.uUInt32 = value; nType = Type_UInt32; return Status(); }
template <> Status DynMember::Set<Int64>(const Int64 &value) { if (nType != Type_Int64) { Status s = Reset(); if (s.IsNOK()) return s; } v.nInt64 = value; nType = Type_Int64; return Status(); }
template <> Status DynMember::Set<UInt64>(const UInt64 &value) { if (nType != Type_UInt64) { Status s = Reset(); if (s.IsNOK()) return s; } v.uUInt64 = value; nType = Type_UInt64; return Status(); }
template <> Status DynMember::Set<Float>(const Float &value) { if (nType != Type_Float) { Status s = Reset(); if (s.IsNOK()) return s; } v.fFloat = value; nType = Type_Float; return Status(); }
template <> Status DynMember::Set<Double>(const Double &value) { if (nType != Type_Double) { Status s = Reset(); if (s.IsNOK()) return s; } v.lfDouble = value; nType = Type_Double; return Status(); }
template <> Status DynMember::Set<Char>(const Char &value) { if (nType != Type_Char) { Status s = Reset(); if (s.IsNOK()) return s; } v.chChar = value; nType = Type_Char; return Status(); }
template <> Status DynMember::Set<WChar>(const WChar &value) { if (nType != Type_WChar) { Status s = Reset(); if (s.IsNOK()) return s; } v.wchWChar = value; nType = Type_WChar; return Status(); }
template <> Status DynMember::Set<String>(const String &value) { if (nType != Type_String) { Status s = Reset(); if (s.IsNOK()) return s; } if (NULL == (v.psString = new (std::nothrow) String(value))) return Status_MemAllocFailed; nType = Type_String; return Status(); }
template <> Status DynMember::Set<WString>(const WString &value) { if (nType != Type_WString) { Status s = Reset(); if (s.IsNOK()) return s; } if (NULL == (v.pwsString = new (std::nothrow) WString(value))) return Status_MemAllocFailed; nType = Type_WString; return Status(); }
template <> Status DynMember::Set<DynMember::BoolSet>(const DynMember::BoolSet &value) { if (nType != Type_BoolSet) { Status s = Reset(); if (s.IsNOK()) return s; } if (NULL == (v.pSetBool = new (std::nothrow) BoolSet(value))) return Status_MemAllocFailed; nType = Type_BoolSet; return Status(); }
template <> Status DynMember::Set<DynMember::Int8Set>(const DynMember::Int8Set &value) { if (nType != Type_Int8Set) { Status s = Reset(); if (s.IsNOK()) return s; } if (NULL == (v.pSetInt8 = new (std::nothrow) Int8Set(value))) return Status_MemAllocFailed; nType = Type_Int8Set; return Status(); }
template <> Status DynMember::Set<DynMember::UInt8Set>(const DynMember::UInt8Set &value) { if (nType != Type_UInt8Set) { Status s = Reset(); if (s.IsNOK()) return s; } if (NULL == (v.pSetUInt8 = new (std::nothrow) UInt8Set(value))) return Status_MemAllocFailed; nType = Type_UInt8Set; return Status(); }
template <> Status DynMember::Set<DynMember::Int16Set>(const DynMember::Int16Set &value) { if (nType != Type_Int16Set) { Status s = Reset(); if (s.IsNOK()) return s; } if (NULL == (v.pSetInt16 = new (std::nothrow) Int16Set(value))) return Status_MemAllocFailed; nType = Type_Int16Set; return Status(); }
template <> Status DynMember::Set<DynMember::UInt16Set>(const DynMember::UInt16Set &value) { if (nType != Type_UInt16Set) { Status s = Reset(); if (s.IsNOK()) return s; } if (NULL == (v.pSetUInt16 = new (std::nothrow) UInt16Set(value))) return Status_MemAllocFailed; nType = Type_UInt16Set; return Status(); }
template <> Status DynMember::Set<DynMember::Int32Set>(const DynMember::Int32Set &value) { if (nType != Type_Int32Set) { Status s = Reset(); if (s.IsNOK()) return s; } if (NULL == (v.pSetInt32 = new (std::nothrow) Int32Set(value))) return Status_MemAllocFailed; nType = Type_Int32Set; return Status(); }
template <> Status DynMember::Set<DynMember::UInt32Set>(const DynMember::UInt32Set &value) { if (nType != Type_UInt32Set) { Status s = Reset(); if (s.IsNOK()) return s; } if (NULL == (v.pSetUInt32 = new (std::nothrow) UInt32Set(value))) return Status_MemAllocFailed; nType = Type_UInt32Set; return Status(); }
template <> Status DynMember::Set<DynMember::Int64Set>(const DynMember::Int64Set &value) { if (nType != Type_Int64Set) { Status s = Reset(); if (s.IsNOK()) return s; } if (NULL == (v.pSetInt64 = new (std::nothrow) Int64Set(value))) return Status_MemAllocFailed; nType = Type_Int64Set; return Status(); }
template <> Status DynMember::Set<DynMember::UInt64Set>(const DynMember::UInt64Set &value) { if (nType != Type_UInt64Set) { Status s = Reset(); if (s.IsNOK()) return s; } if (NULL == (v.pSetUInt64 = new (std::nothrow) UInt64Set(value))) return Status_MemAllocFailed; nType = Type_UInt64Set; return Status(); }
template <> Status DynMember::Set<DynMember::FloatSet>(const DynMember::FloatSet &value) { if (nType != Type_FloatSet) { Status s = Reset(); if (s.IsNOK()) return s; } if (NULL == (v.pSetFloat = new (std::nothrow) FloatSet(value))) return Status_MemAllocFailed; nType = Type_FloatSet; return Status(); }
template <> Status DynMember::Set<DynMember::DoubleSet>(const DynMember::DoubleSet &value) { if (nType != Type_DoubleSet) { Status s = Reset(); if (s.IsNOK()) return s; } if (NULL == (v.pSetDouble = new (std::nothrow) DoubleSet(value))) return Status_MemAllocFailed; nType = Type_DoubleSet; return Status(); }
template <> Status DynMember::Set<DynMember::CharSet>(const DynMember::CharSet &value) { if (nType != Type_CharSet) { Status s = Reset(); if (s.IsNOK()) return s; } if (NULL == (v.pSetChar = new (std::nothrow) CharSet(value))) return Status_MemAllocFailed; nType = Type_CharSet; return Status(); }
template <> Status DynMember::Set<DynMember::WCharSet>(const DynMember::WCharSet &value) { if (nType != Type_WCharSet) { Status s = Reset(); if (s.IsNOK()) return s; } if (NULL == (v.pSetWChar = new (std::nothrow) WCharSet(value))) return Status_MemAllocFailed; nType = Type_WCharSet; return Status(); }
template <> Status DynMember::Set<DynMember::StringSet>(const DynMember::StringSet &value) { if (nType != Type_StringSet) { Status s = Reset(); if (s.IsNOK()) return s; } if (NULL == (v.pSetString = new (std::nothrow) StringSet(value))) return Status_MemAllocFailed; nType = Type_StringSet; return Status(); }
template <> Status DynMember::Set<DynMember::WStringSet>(const DynMember::WStringSet &value) { if (nType != Type_WStringSet) { Status s = Reset(); if (s.IsNOK()) return s; } if (NULL == (v.pSetWString = new (std::nothrow) WStringSet(value))) return Status_MemAllocFailed; nType = Type_WStringSet; return Status(); }

template <> Status DynMember::Get<Bool>(Bool &value) const { if (nType != Type_Bool) return Status_InvalidArg; value = v.bBool; return Status(); }
template <> Status DynMember::Get<Int8>(Int8 &value) const { if (nType != Type_Int8) return Status_InvalidArg; value = v.nInt8; return Status(); }
template <> Status DynMember::Get<UInt8>(UInt8 &value) const { if (nType != Type_UInt8) return Status_InvalidArg; value = v.uUInt8; return Status(); }
template <> Status DynMember::Get<Int16>(Int16 &value) const { if (nType != Type_Int16) return Status_InvalidArg; value = v.nInt16; return Status(); }
template <> Status DynMember::Get<UInt16>(UInt16 &value) const { if (nType != Type_UInt16) return Status_InvalidArg; value = v.uUInt16; return Status(); }
template <> Status DynMember::Get<Int32>(Int32 &value) const { if (nType != Type_Int32) return Status_InvalidArg; value = v.nInt32; return Status(); }
template <> Status DynMember::Get<UInt32>(UInt32 &value) const { if (nType != Type_UInt32) return Status_InvalidArg; value = v.uUInt32; return Status(); }
template <> Status DynMember::Get<Int64>(Int64 &value) const { if (nType != Type_Int64) return Status_InvalidArg; value = v.nInt64; return Status(); }
template <> Status DynMember::Get<UInt64>(UInt64 &value) const { if (nType != Type_UInt64) return Status_InvalidArg; value = v.uUInt64; return Status(); }
template <> Status DynMember::Get<Float>(Float &value) const { if (nType != Type_Float) return Status_InvalidArg; value = v.fFloat; return Status(); }
template <> Status DynMember::Get<Double>(Double &value) const { if (nType != Type_Double) return Status_InvalidArg; value = v.lfDouble; return Status(); }
template <> Status DynMember::Get<Char>(Char &value) const { if (nType != Type_Char) return Status_InvalidArg; value = v.chChar; return Status(); }
template <> Status DynMember::Get<WChar>(WChar &value) const { if (nType != Type_WChar) return Status_InvalidArg; value = v.wchWChar; return Status(); }
template <> Status DynMember::Get<String>(String &value) const { if (nType != Type_String) return Status_InvalidArg; value = *v.psString; return Status(); }
template <> Status DynMember::Get<WString>(WString &value) const { if (nType != Type_WString) return Status_InvalidArg; value = *v.pwsString; return Status(); }
template <> Status DynMember::Get<DynMember::BoolSet>(DynMember::BoolSet &value) const { if (nType != Type_BoolSet) return Status_InvalidArg; value = *v.pSetBool; return Status(); }
template <> Status DynMember::Get<DynMember::Int8Set>(DynMember::Int8Set &value) const { if (nType != Type_Int8Set) return Status_InvalidArg; value = *v.pSetInt8; return Status(); }
template <> Status DynMember::Get<DynMember::UInt8Set>(DynMember::UInt8Set &value) const { if (nType != Type_UInt8Set) return Status_InvalidArg; value = *v.pSetUInt8; return Status(); }
template <> Status DynMember::Get<DynMember::Int16Set>(DynMember::Int16Set &value) const { if (nType != Type_Int16Set) return Status_InvalidArg; value = *v.pSetInt16; return Status(); }
template <> Status DynMember::Get<DynMember::UInt16Set>(DynMember::UInt16Set &value) const { if (nType != Type_UInt16Set) return Status_InvalidArg; value = *v.pSetUInt16; return Status(); }
template <> Status DynMember::Get<DynMember::Int32Set>(DynMember::Int32Set &value) const { if (nType != Type_Int32Set) return Status_InvalidArg; value = *v.pSetInt32; return Status(); }
template <> Status DynMember::Get<DynMember::UInt32Set>(DynMember::UInt32Set &value) const { if (nType != Type_UInt32Set) return Status_InvalidArg; value = *v.pSetUInt32; return Status(); }
template <> Status DynMember::Get<DynMember::Int64Set>(DynMember::Int64Set &value) const { if (nType != Type_Int64Set) return Status_InvalidArg; value = *v.pSetInt64; return Status(); }
template <> Status DynMember::Get<DynMember::UInt64Set>(DynMember::UInt64Set &value) const { if (nType != Type_UInt64Set) return Status_InvalidArg; value = *v.pSetUInt64; return Status(); }
template <> Status DynMember::Get<DynMember::FloatSet>(DynMember::FloatSet &value) const { if (nType != Type_FloatSet) return Status_InvalidArg; value = *v.pSetFloat; return Status(); }
template <> Status DynMember::Get<DynMember::DoubleSet>(DynMember::DoubleSet &value) const { if (nType != Type_DoubleSet) return Status_InvalidArg; value = *v.pSetDouble; return Status(); }
template <> Status DynMember::Get<DynMember::CharSet>(DynMember::CharSet &value) const { if (nType != Type_CharSet) return Status_InvalidArg; value = *v.pSetChar; return Status(); }
template <> Status DynMember::Get<DynMember::WCharSet>(DynMember::WCharSet &value) const { if (nType != Type_WCharSet) return Status_InvalidArg; value = *v.pSetWChar; return Status(); }
template <> Status DynMember::Get<DynMember::StringSet>(DynMember::StringSet &value) const { if (nType != Type_StringSet) return Status_InvalidArg; value = *v.pSetString; return Status(); }
template <> Status DynMember::Get<DynMember::WStringSet>(DynMember::WStringSet &value) const { if (nType != Type_WStringSet) return Status_InvalidArg; value = *v.pSetWString; return Status(); }

}//namespace Util

}//namespace CX

