/* 
 * CX - C++ framework for general purpose development
 *
 * https://github.com/draede/cx
 * 
 * Copyright (C) 2014 - 2019 draede - draede [at] outlook [dot] com
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
#include "CX/Data/MO/Language.hpp"
#include "CX/IO/FileInputStream.hpp"


namespace CX
{

namespace Data
{

namespace MO
{

Language::Language()
{
}

Language::Language(const String &sID)
{
	m_sID = sID;
}

Language::Language(const Language &lang)
{
	Copy(lang);
}

Language::~Language()
{
}

Status Language::Copy(const Language &lang)
{
	m_sID            = lang.m_sID;
	m_mapStrings     = lang.m_mapStrings;

	return Status();
}

Status Language::SetID(const String &sID)
{
	m_sID = sID;

	return Status();
}

const String &Language::GetID() const
{
	return m_sID;
}


Size Language::GetStringsCount() const
{
	return m_mapStrings.size();
}

const String &Language::GetString(Size cIndex) const
{
	static const String sTmp;

	if (m_mapStrings.size() <= cIndex)
	{
		return sTmp;
	}

	StringsMap::const_iterator iter = m_mapStrings.begin();

	while (0 < cIndex)
	{
		++iter;
		cIndex--;
	}

	return iter->first;
}

const String &Language::GetTranslation(Size cIndex) const
{
	static const String sTmp;

	if (m_mapStrings.size() <= cIndex)
	{
		return sTmp;
	}

	StringsMap::const_iterator iter = m_mapStrings.begin();

	while (0 < cIndex)
	{
		++iter;
		cIndex--;
	}

	return iter->second;
}

const String &Language::GetTranslation(const String &sString) const
{
	StringsMap::const_iterator iter = m_mapStrings.find(sString);

	if (m_mapStrings.end() != iter)
	{
		return iter->second;
	}
	else
	{
		return sString;
	}
}

Status Language::LoadFromStream(const Char *szPath)
{
	IO::FileInputStream fis(szPath);

	if (!fis.IsOK())
	{
		return Status(Status_OpenFailed, "Failed to open '{1}'", szPath);
	}

	return LoadFromStream(&fis);
}

Status Language::LoadFromStream(IO::IInputStream *pInputStream)
{
	SAXParser   parser;
	SAXObserver obs;
	Status      status;

	m_mapStrings.clear();
	obs.m_pMapStrings = &m_mapStrings;
	if ((status = parser.AddObserver(&obs)).IsNOK())
	{
		return status;
	}
	if ((status = parser.BeginParse()).IsNOK())
	{
		return status;
	}
	if ((status = parser.ParseStream(pInputStream)).IsNOK())
	{
		return status;
	}
	if ((status = parser.EndParse()).IsNOK())
	{
		return status;
	}

	return Status();
}

void Language::SAXObserver::OnBeginParse()
{
}

void Language::SAXObserver::OnEndParse()
{
}

void Language::SAXObserver::OnString(const Char *szOrig, const Char *szTranslated)
{
	(*m_pMapStrings)[szOrig] = szTranslated;
}

}//namespace MO

}//namespace Data

}//namespace CX

