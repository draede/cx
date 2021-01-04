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


#include "CX/SimpleBuffers/MemberType.hpp"
#include "CX/String.hpp"
#include "CX/Status.hpp"
#include "CX/APIDefs.hpp"


namespace CX
{

namespace SimpleBuffers
{

class CX_API Member
{
public:

	Member();

	Member(const String &sName, const String &sMemberName, const String &sGetterName, const String &sSetterName, 
	       const String &sDefault, bool bOptional, MemberType nMemberType, const String &sKeyType, const String &sValType = "");

	Member(const Member &member);

	~Member();

	Member &operator=(const Member &member);

	void Copy(const Member &member);

	void SetName(const String &sName);

	const String &GetName() const;

	void SetMemberName(const String &sMemberName);

	const String &GetMemberName() const;

	void SetGetterName(const String &sGetterName);

	const String &GetGetterName() const;

	void SetSetterName(const String &sSetterName);

	const String &GetSetterName() const;

	void SetDefault(const String &sDefault);

	const String &GetDefault() const;

	void SetMemberType(MemberType nMemberType);

	MemberType GetMemberType() const;

	void SetOptional(bool nOptional);

	bool GetOptional() const;

	void SetKeyType(const String &sKeyType);

	const String &GetKeyType() const;

	void SetValType(const String &sValType);

	const String &GetValType() const;

private:

#pragma warning(push)
#pragma warning(disable: 4251)
	String       m_sName;
	String       m_sMemberName;
	String       m_sGetterName;
	String       m_sSetterName;
	String       m_sDefault;
	String       m_sKeyType;
	String       m_sValType;
#pragma warning(pop)
	bool         m_bOptional;
	MemberType   m_nMemberType;

};

}//namespace SimpleBuffers

}//namespace CX
