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
 
#include "CX/precomp.hpp"
#include "CX/SimpleBuffers/Object.hpp"


namespace CX
{

namespace SimpleBuffers
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
	m_vectorConsts  = object.m_vectorConsts;
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

const Object::ConstsVector &Object::GetConsts() const
{
	return m_vectorConsts;
}

Object::ConstsVector &Object::GetConsts()
{
	return m_vectorConsts;
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

}//namespace SimpleBuffers

}//namespace CX
