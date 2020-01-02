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
#include "CX/Version.hpp"


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
	m_sCopyright     = szCopyright;
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
    
    CX_UNUSED(SIG);

	return version;
}

}//namespace CX

