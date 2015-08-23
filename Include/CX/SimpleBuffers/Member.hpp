
#pragma once


#include "CX/SimpleBuffers/MemberType.hpp"
#include "CX/String.hpp"
#include "CX/Status.hpp"


namespace CX
{

namespace SimpleBuffers
{

class Member
{
public:

	Member();

	Member(const String &sName, MemberType nMemberType, const String &sKeyType, const String &sValType = "");

	Member(const Member &member);

	~Member();

	Member &operator=(const Member &member);

	void Copy(const Member &member);

	void SetName(const String &sName);

	const String &GetName() const;

	void SetMemberType(MemberType nMemberType);

	MemberType GetMemberType() const;

	void SetKeyType(const String &sKeyType);

	const String &GetKeyType() const;

	void SetValType(const String &sValType);

	const String &GetValType() const;

private:

	String       m_sName;
	MemberType   m_nMemberType;
	String       m_sKeyType;
	String       m_sValType;

};

}//namespace SimpleBuffers

}//namespace CX
