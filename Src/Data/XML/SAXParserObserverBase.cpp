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
#include "CX/Data/XML/SAXParserObserverBase.hpp"


namespace CX
{

namespace Data
{

namespace XML
{

SAXParserObserverBase::SAXParserObserverBase()
{
}

SAXParserObserverBase::~SAXParserObserverBase()
{
}

void SAXParserObserverBase::SetStatus(const Status &status)
{
	__m_status__ = status;
}

const Status &SAXParserObserverBase::GetStatus() const
{
	return __m_status__;
}

const Char *SAXParserObserverBase::AddToPath(String &sPath, const Char *szName)
{
	sPath += "/";
	sPath += szName;

	return sPath.c_str();
}

const Char *SAXParserObserverBase::RemoveFromPath(String &sPath)
{
	Size cPos;

	if ((Size) - 1 == (cPos = sPath.rfind('/')))
	{
		sPath.clear();
	}
	else
	{
		sPath.erase(cPos);
	}

	return sPath.c_str();
}

Bool SAXParserObserverBase::ComparePath(const Char *szXMLPath, const Char *szCrPath, Bool *pbFullCmp)
{
	const Char *pszXML = szXMLPath;
	const Char *pszCr  = szCrPath;

	*pbFullCmp = False;
	while ('\0' != *pszXML && '\0' != *pszCr)
	{
		if (cx_tolower(*pszXML) != cx_tolower(*pszCr))
		{
			break;
		}
		pszXML++;
		pszCr++;
	}
	if ('\0' != *pszCr)
	{
		return False;
	}
	if ('\0' == *pszXML)
	{
		*pbFullCmp = True;
	}

	return True;
}

void SAXParserObserverBase::GetAttrValues(__AttrValuesVector__ &attrs, const AttrsVector *pVectorAttrs, 
                                          const CX::Char *szArg1/* = NULL*/, const CX::Char *szArg2/* = NULL*/, 
                                          const CX::Char *szArg3/* = NULL*/, const CX::Char *szArg4/* = NULL*/,
                                          const CX::Char *szArg5/* = NULL*/, const CX::Char *szArg6/* = NULL*/, 
                                          const CX::Char *szArg7/* = NULL*/, const CX::Char *szArg8/* = NULL*/, 
                                          const CX::Char *szArg9/* = NULL*/, const CX::Char *szArg10/* = NULL*/,
                                          const CX::Char *szArg11/* = NULL*/, const CX::Char *szArg12/* = NULL*/, 
                                          const CX::Char *szArg13/* = NULL*/, const CX::Char *szArg14/* = NULL*/, 
                                          const CX::Char *szArg15/* = NULL*/, const CX::Char *szArg16/* = NULL*/)
{
	for (;;)
	{
		if (NULL != szArg1) attrs.push_back(NULL); else break;
		if (NULL != szArg2) attrs.push_back(NULL); else break;
		if (NULL != szArg3) attrs.push_back(NULL); else break;
		if (NULL != szArg4) attrs.push_back(NULL); else break;
		if (NULL != szArg5) attrs.push_back(NULL); else break;
		if (NULL != szArg6) attrs.push_back(NULL); else break;
		if (NULL != szArg7) attrs.push_back(NULL); else break;
		if (NULL != szArg8) attrs.push_back(NULL); else break;
		if (NULL != szArg9) attrs.push_back(NULL); else break;
		if (NULL != szArg10) attrs.push_back(NULL); else break;
		if (NULL != szArg11) attrs.push_back(NULL); else break;
		if (NULL != szArg12) attrs.push_back(NULL); else break;
		if (NULL != szArg13) attrs.push_back(NULL); else break;
		if (NULL != szArg14) attrs.push_back(NULL); else break;
		if (NULL != szArg15) attrs.push_back(NULL); else break;
		if (NULL != szArg16) attrs.push_back(NULL); else break;
		
		break;
	}

	if (NULL == pVectorAttrs)
	{
		return;
	}

	for (AttrsVector::const_iterator iter = pVectorAttrs->begin(); iter != pVectorAttrs->end(); ++iter)
	{
		for (;;)
		{
			if (NULL != szArg1) { if (0 == cx_stricmp(szArg1, iter->sName.c_str())) { attrs[0] = iter->sValue.c_str(); break; } } else break;
			if (NULL != szArg2) { if (0 == cx_stricmp(szArg2, iter->sName.c_str())) { attrs[1] = iter->sValue.c_str(); break; } } else break;
			if (NULL != szArg3) { if (0 == cx_stricmp(szArg3, iter->sName.c_str())) { attrs[2] = iter->sValue.c_str(); break; } } else break;
			if (NULL != szArg4) { if (0 == cx_stricmp(szArg4, iter->sName.c_str())) { attrs[3] = iter->sValue.c_str(); break; } } else break;
			if (NULL != szArg5) { if (0 == cx_stricmp(szArg5, iter->sName.c_str())) { attrs[4] = iter->sValue.c_str(); break; } } else break;
			if (NULL != szArg6) { if (0 == cx_stricmp(szArg6, iter->sName.c_str())) { attrs[5] = iter->sValue.c_str(); break; } } else break;
			if (NULL != szArg7) { if (0 == cx_stricmp(szArg7, iter->sName.c_str())) { attrs[6] = iter->sValue.c_str(); break; } } else break;
			if (NULL != szArg8) { if (0 == cx_stricmp(szArg8, iter->sName.c_str())) { attrs[7] = iter->sValue.c_str(); break; } } else break;
			if (NULL != szArg9) { if (0 == cx_stricmp(szArg9, iter->sName.c_str())) { attrs[8] = iter->sValue.c_str(); break; } } else break;
			if (NULL != szArg10) { if (0 == cx_stricmp(szArg10, iter->sName.c_str())) { attrs[9] = iter->sValue.c_str(); break; } } else break;
			if (NULL != szArg11) { if (0 == cx_stricmp(szArg11, iter->sName.c_str())) { attrs[10] = iter->sValue.c_str(); break; } } else break;
			if (NULL != szArg12) { if (0 == cx_stricmp(szArg12, iter->sName.c_str())) { attrs[11] = iter->sValue.c_str(); break; } } else break;
			if (NULL != szArg13) { if (0 == cx_stricmp(szArg13, iter->sName.c_str())) { attrs[12] = iter->sValue.c_str(); break; } } else break;
			if (NULL != szArg14) { if (0 == cx_stricmp(szArg14, iter->sName.c_str())) { attrs[13] = iter->sValue.c_str(); break; } } else break;
			if (NULL != szArg15) { if (0 == cx_stricmp(szArg15, iter->sName.c_str())) { attrs[14] = iter->sValue.c_str(); break; } } else break;
			if (NULL != szArg16) { if (0 == cx_stricmp(szArg16, iter->sName.c_str())) { attrs[15] = iter->sValue.c_str(); break; } } else break;

			break;
		}
	}
}

void SAXParserObserverBase::OnBeginParse()
{
	__m_xml_path__.clear();
	__m_status__.Clear();
}

void SAXParserObserverBase::OnEndParse()
{
}

void SAXParserObserverBase::OnStartElement(const Char *szName, const AttrsVector *pVectorAttrs)
{
	if (!__m_status__.IsOK())
	{
		return;
	}

	AddToPath(__m_xml_path__, szName);

	OnElement(__m_xml_path__.c_str(), True, pVectorAttrs, NULL);
}

void SAXParserObserverBase::OnEndElement(const Char *szName, const Char *szText)
{
	CX_UNUSED(szName);

	if (!__m_status__.IsOK())
	{
		return;
	}

	OnElement(__m_xml_path__.c_str(), False, NULL, szText);

	RemoveFromPath(__m_xml_path__);
}

}//namespace XML

}//namespace Data

}//namespace CX
