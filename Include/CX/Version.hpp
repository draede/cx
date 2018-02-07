/* 
 * CX - C++ framework for general purpose development
 *
 * https://github.com/draede/cx
 * 
 * Copyright (C) 2014 - 2018 draede - draede [at] outlook [dot] com
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


#include "CX/Platform.hpp"
#include "CX/APIDefs.hpp"
#include "CX/Types.hpp"
#include "CX/String.hpp"
#include "CX/C/Version.h"


namespace CX
{

class CX_API Version
{
public:

	Version();

	Version(int nMajor, int nMinor, int nRevision, int nBuild, 
	        const Char *szName, const Char *szCopyright, const Char *szCompany, 
	        const Char *szDescription);

	~Version();

	void SetMajor(int nMajor);

	int GetMajor() const;

	void SetMinor(int nMinor);

	int GetMinor() const;

	void SetRevision(int nRevision);

	int GetRevision() const;

	void SetBuild(int nBuild);

	int GetBuild() const;

	void SetName(const Char *szName);

	const Char *GetName() const;

	void SetCopyright(const Char *szCopyright);

	const Char *GetCopyright() const;

	void SetCompany(const Char *szCompany);

	const Char *GetCompany() const;

	void SetDescription(const Char *szDescription);

	const Char *GetDescription() const;

private:

	int      m_nMajor;
	int      m_nMinor;
	int      m_nRevision;
	int      m_nBuild;

#pragma warning(push)
#pragma warning(disable: 4251)
	String   m_sName;
	String   m_sCopyright;
	String   m_sCompany;
	String   m_sDescription;
#pragma warning(pop)

};

class CX_API CXVersion
{
public:

	static const Version &Get();

private:

	CXVersion();

	~CXVersion();

};

}//namespace CX

