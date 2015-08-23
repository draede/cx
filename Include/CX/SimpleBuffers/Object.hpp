
#pragma once


#include "CX/SimpleBuffers/Member.hpp"
#include "CX/Vector.hpp"
#include "CX/Map.hpp"


namespace CX
{

namespace SimpleBuffers
{

class Object
{
public:

	typedef Vector<Member>::Type   MembersVector;

	struct Pragma
	{
		String sID;
		String sValue;
	};

	typedef Vector<Pragma>::Type   PragmasVector;

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

	const PragmasVector &GetPragmasByLocation(const Char *szPragmaLocation) const;

	Status AddPragma(const String &sLocation, const String &sID, const String &sValue);

private:

	typedef Map<String, PragmasVector, CaseInsensitiveOrderPolicy>::Type   PragmasMap;

	String          m_sName;
	MembersVector   m_vectorMembers;
	PragmasMap      m_mapPragmas;

};

}//namespace SimpleBuffers

}//namespace CX
