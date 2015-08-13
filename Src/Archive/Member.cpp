
#include "CX/Archive/Member.hpp"


namespace CX
{

namespace Archive
{

Member::Member()
{
}

Member::Member(const String &sName, MemberType nMemberType, const String &sKeyType, const String &sValType/* = ""*/)
{
	m_sName       = sName;
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
	m_nMemberType = member.m_nMemberType;
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

void Member::SetMemberType(MemberType nMemberType)
{
	m_nMemberType = nMemberType;
}

MemberType Member::GetMemberType() const
{
	return m_nMemberType;
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

}//namespace Archive

}//namespace CX
