
#include "CX/Archive/Object.hpp"


namespace CX
{

namespace Archive
{

Object::Object()
{
}

Object::Object(const Object &object)
{
	Copy(object);
}

Object::~Object()
{
}

Object &Object::operator=(const Object &object)
{
	Copy(object);

	return *this;
}

void Object::Copy(const Object &object)
{
	m_sName         = object.m_sName;
	m_vectorMembers = object.m_vectorMembers;
	m_mapPragmas    = object.m_mapPragmas;
}

void Object::SetName(const String &sName)
{
	m_sName = sName;
}

const String &Object::GetName() const
{
	return m_sName;
}

const Object::MembersVector &Object::GetMembers() const
{
	return m_vectorMembers;
}

Object::MembersVector &Object::GetMembers()
{
	return m_vectorMembers;
}

const Object::PragmasVector &Object::GetPragmasByLocation(const Char *szPragmaLocation) const
{
	static const PragmasVector vectorPragmas;
	PragmasMap::const_iterator iter;

	if (m_mapPragmas.end() != (iter = m_mapPragmas.find(szPragmaLocation)))
	{
		return iter->second;
	}
	else
	{
		return vectorPragmas;
	}
}

Status Object::AddPragma(const String &sLocation, const String &sID, const String &sValue)
{
	Pragma p;

	p.sID    = sID;
	p.sValue = sValue;
	m_mapPragmas[sLocation].push_back(p);

	return Status();
}

}//namespace Archive

}//namespace CX
