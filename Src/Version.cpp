
#include "CX/Version.h"


namespace CX
{

Version::Version()
{
	m_nMajor      = 0;
	m_nMinor      = 0;
	m_nRevision   = 0;
	m_nBuild      = 0;
}

Version::Version(int nMajor, int nMinor, int nRevision, int nBuild, 
	      const Char *szName, const Char *szCopyright, const Char *szCompany, 
	      const Char *szDescription)
{
	m_nMajor         = nMajor;
	m_nMinor         = nMinor;
	m_nRevision      = nRevision;
	m_nBuild         = nBuild;
	m_sName          = szName;
	m_sCopyright     = szCompany;
	m_sCompany       = szCompany;
	m_sDescription   = szDescription;
}

Version::~Version()
{
}

void Version::SetMajor(int nMajor)
{
	m_nMajor = nMajor;
}

int Version::GetMajor() const
{
	return m_nMajor;
}

void Version::SetMinor(int nMinor)
{
	m_nMinor = nMinor;
}

int Version::GetMinor() const
{
	return m_nMinor;
}

void Version::SetRevision(int nRevision)
{
	m_nRevision = nRevision;
}

int Version::GetRevision() const
{
	return m_nRevision;
}

void Version::SetBuild(int nBuild)
{
	m_nBuild = nBuild;
}

int Version::GetBuild() const
{
	return m_nBuild;
}

void Version::SetName(const Char *szName)
{
	m_sName = szName;
}

const Char *Version::GetName() const
{
	return m_sName.c_str();
}

void Version::SetCopyright(const Char *szCopyright)
{
	m_sCopyright = szCopyright;
}

const Char *Version::GetCopyright() const
{
	return m_sCopyright.c_str();
}

void Version::SetCompany(const Char *szCompany)
{
	m_sCompany = szCompany;
}

const Char *Version::GetCompany() const
{
	return m_sCompany.c_str();
}

void Version::SetDescription(const Char *szDescription)
{
	m_sDescription = szDescription;
}

const Char *Version::GetDescription() const
{
	return m_sDescription.c_str();
}

CXVersion::CXVersion()
{
}

CXVersion::~CXVersion()
{
}

const Version &CXVersion::Get()
{
	static Version version(CX_VER_MAJOR, CX_VER_MINOR, CX_VER_REVISION, CX_VER_BUILD, 
	                       CX_VER_NAME, CX_VER_COPYRIGHT, CX_VER_COMPANY, CX_VER_DESCRIPTION);

	static const Char *SIG = "*** " CX_VER_HLP_SIGNATURE(CX_VER_MAJOR, CX_VER_MINOR, 
	                                                     CX_VER_REVISION, CX_VER_BUILD, CX_VER_NAME, 
	                                                     CX_VER_COPYRIGHT, CX_VER_COMPANY, 
	                                                     CX_VER_DESCRIPTION) " ***";

	return version;
}

}//namespace CX

