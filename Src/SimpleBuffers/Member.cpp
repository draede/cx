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
#include "CX/SimpleBuffers/Member.hpp"


namespace CX
{

namespace SimpleBuffers
{

Member::Member()
{
	m_bOptional = false;
}

Member::Member(const String &sName, const String &sMemberName, const String &sGetterName, const String &sSetterName, 
               const String &sDefault, bool bOptional, MemberType nMemberType, const String &sKeyType, 
               const String &sValType/* = ""*/)
{
	m_sName       = sName;
	m_sMemberName = sMemberName;
	m_sGetterName = sGetterName;
	m_sSetterName = sSetterName;
	m_sDefault    = sDefault;
	m_bOptional   = bOptional;
	m_nMemberType = nMemberType;
	m_sKeyType    = sKeyType;
	m_sValType    = sValType;
}

Member::Member(const Member &member)
{
	Copy(member);
}

Member::~Member()
{
}

Member &Member::operator=(const Member &member)
{
	Copy(member);

	return *this;
}

void Member::Copy(const Member &member)
{
	m_sName       = member.m_sName;
	m_sMemberName = member.m_sMemberName;
	m_sGetterName = member.m_sGetterName;
	m_sSetterName = member.m_sSetterName;
	m_sDefault    = member.m_sDefault;
	m_nMemberType = member.m_nMemberType;
	m_bOptional   = member.m_bOptional;
	m_sKeyType    = member.m_sKeyType;
	m_sValType    = member.m_sValType;
}

void Member::SetName(const String &sName)
{
	m_sName = sName;
}

const String &Member::GetName() const
{
	return m_sName;
}

void Member::SetMemberName(const String &sMemberName)
{
	m_sMemberName = sMemberName;
}

const String &Member::GetMemberName() const
{
	return m_sMemberName;
}

void Member::SetGetterName(const String &sGetterName)
{
	m_sGetterName = sGetterName;
}

const String &Member::GetGetterName() const
{
	return m_sGetterName;
}

void Member::SetSetterName(const String &sSetterName)
{
	m_sSetterName = sSetterName;
}

const String &Member::GetSetterName() const
{
	return m_sSetterName;
}

void Member::SetDefault(const String &sDefault)
{
	m_sDefault = sDefault;
}

const String &Member::GetDefault() const
{
	return m_sDefault;
}

void Member::SetMemberType(MemberType nMemberType)
{
	m_nMemberType = nMemberType;
}

MemberType Member::GetMemberType() const
{
	return m_nMemberType;
}

void Member::SetOptional(bool bOptional)
{
	m_bOptional = bOptional;
}

bool Member::GetOptional() const
{
	return m_bOptional;
}

void Member::SetKeyType(const String &sKeyType)
{
	m_sKeyType = sKeyType;
}

const String &Member::GetKeyType() const
{
	return m_sKeyType;
}

void Member::SetValType(const String &sValType)
{
	m_sValType = sValType;
}

const String &Member::GetValType() const
{
	return m_sValType;
}

}//namespace SimpleBuffers

}//namespace CX
