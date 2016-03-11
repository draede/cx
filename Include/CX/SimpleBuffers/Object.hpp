/* 
 * CX - C++ framework for general purpose development
 *
 * https://github.com/draede/cx
 * 
 * Copyright (C) 2014 - 2016 draede - draede [at] outlook [dot] com
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


#include "CX/SimpleBuffers/Member.hpp"
#include "CX/Vector.hpp"
#include "CX/Map.hpp"
#include "CX/APIDefs.hpp"


namespace CX
{

namespace SimpleBuffers
{

class CX_API Object :public IObject
{
public:

	typedef Vector<Member>::Type      MembersVector;

	struct Pragma
	{
		String sID;
		String sValue;
	};

	typedef Vector<Pragma>::Type   PragmasVector;

	struct Const
	{
		String sType;
		String sName;
		String sValue;
	};

	typedef Vector<Const>::Type   ConstsVector;

	static const Char *PRAGMA_LOCATION_PROLOG() { return "prolog"; }
	static const Char *PRAGMA_LOCATION_EPILOG() { return "epilog"; }
	static const Char *PRAGMA_CPP() { return "cpp"; }

	Object();

	Object(const Object &object);

	~Object();

	Object &operator=(const Object &object);

	void Copy(const Object &object);

	void SetName(const String &sName);

	const String &GetName() const;

	const MembersVector &GetMembers() const;

	MembersVector &GetMembers();

	const ConstsVector &GetConsts() const;

	ConstsVector &GetConsts();

	const PragmasVector &GetPragmasByLocation(const Char *szPragmaLocation) const;

	Status AddPragma(const String &sLocation, const String &sID, const String &sValue);

private:

	typedef Map<String, PragmasVector, CaseInsensitiveOrderPolicy>::Type   PragmasMap;

#pragma warning(push)
#pragma warning(disable: 4251)
	String          m_sName;
	ConstsVector    m_vectorConsts;
	MembersVector   m_vectorMembers;
	PragmasMap      m_mapPragmas;
#pragma warning(pop)

};

}//namespace SimpleBuffers

}//namespace CX
