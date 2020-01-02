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

#include "CX/precomp.hpp"
#include "CX/Util/DynMember.hpp"


namespace CX
{

namespace Util
{

DynMember::DynMember()
{
	nType = Type_Invalid;
}

DynMember::DynMember(const DynMember &member)
{
	nType = Type_Invalid;
	Copy(member);
}

DynMember::~DynMember()
{
	Reset();
}

DynMember &DynMember::operator=(const DynMember &member)
{
	Copy(member);

	return *this;
}

Status DynMember::Copy(const DynMember &member)
{
	switch (member.nType)
	{
		case Type_Bool:       return Set(member.v.bBool); 
		case Type_Int8:       return Set(member.v.nInt8); 
		case Type_UInt8:      return Set(member.v.uUInt8); 
		case Type_Int16:      return Set(member.v.nInt16); 
		case Type_UInt16:     return Set(member.v.uUInt16); 
		case Type_Int32:      return Set(member.v.nInt32); 
		case Type_UInt32:     return Set(member.v.uUInt32); 
		case Type_Int64:      return Set(member.v.nInt64); 
		case Type_UInt64:     return Set(member.v.uUInt64); 
		case Type_Float:      return Set(member.v.fFloat); 
		case Type_Double:     return Set(member.v.lfDouble); 
		case Type_Char:       return Set(member.v.chChar); 
		case Type_WChar:      return Set(member.v.wchWChar); 
		case Type_String:     return Set(*member.v.psString); 
		case Type_WString:    return Set(*member.v.psString); 
		case Type_BoolSet:    return Set(*member.v.pSetBool); 
		case Type_Int8Set:    return Set(*member.v.pSetInt8); 
		case Type_UInt8Set:   return Set(*member.v.pSetUInt8); 
		case Type_Int16Set:   return Set(*member.v.pSetInt16); 
		case Type_UInt16Set:  return Set(*member.v.pSetUInt16); 
		case Type_Int32Set:   return Set(*member.v.pSetInt32); 
		case Type_UInt32Set:  return Set(*member.v.pSetUInt32); 
		case Type_Int64Set:   return Set(*member.v.pSetInt64); 
		case Type_UInt64Set:  return Set(*member.v.pSetUInt64); 
		case Type_CharSet:    return Set(*member.v.pSetChar); 
		case Type_WCharSet:   return Set(*member.v.pSetWChar); 
		case Type_FloatSet:   return Set(*member.v.pSetFloat); 
		case Type_DoubleSet:  return Set(*member.v.pSetDouble); 
		case Type_StringSet:  return Set(*member.v.pSetString); 
		case Type_WStringSet: return Set(*member.v.pSetWString); 
		default:              return Status_InvalidArg;
	}
}

Status DynMember::Reset()
{
	if (Type_String == nType) delete v.psString;
	else if (Type_WString == nType) delete v.pwsString;
	else if (Type_BoolSet == nType) delete v.pSetBool;
	else if (Type_Int8Set == nType) delete v.pSetInt8;
	else if (Type_UInt8Set == nType) delete v.pSetUInt8;
	else if (Type_Int16Set == nType) delete v.pSetInt16;
	else if (Type_UInt16Set == nType) delete v.pSetUInt16;
	else if (Type_Int32Set == nType) delete v.pSetInt32;
	else if (Type_UInt32Set == nType) delete v.pSetUInt32;
	else if (Type_Int64Set == nType) delete v.pSetInt64;
	else if (Type_UInt64Set == nType) delete v.pSetUInt64;
	else if (Type_FloatSet == nType) delete v.pSetFloat;
	else if (Type_DoubleSet == nType) delete v.pSetDouble;
	else if (Type_CharSet == nType) delete v.pSetChar;
	else if (Type_WCharSet == nType) delete v.pSetWChar;
	else if (Type_StringSet == nType) delete v.pSetString;
	else if (Type_WStringSet == nType) delete v.pSetWString;

	nType = Type_Invalid;

	return Status();
}

}//namespace Util

}//namespace CX

