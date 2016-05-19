/* 
 * CX - C++ framework for general purpose development
 *
 * https://github.com/draede/cx
 * 
 * Copyright (C) 2014-2016 draede, draede [at] outlook [dot] com
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

#include "CX/Data/XML/SAXParser.hpp"
#include "CX/Status.hpp"
#include "CX/IO/FileInputStream.hpp"
#include "expat.h"


namespace CX
{

namespace Data
{

namespace XML
{

SAXParser::SAXParser()
{
	m_pParser = NULL;
}

SAXParser::~SAXParser()
{
	if (NULL != m_pParser)
	{
		XML_ParserFree((XML_Parser)m_pParser);
		m_pParser = NULL;
	}
}

Status SAXParser::BeginParse()
{
	if (NULL != m_pParser)
	{
		XML_ParserFree((XML_Parser)m_pParser);
		m_pParser = NULL;
	}
	if (NULL == (m_pParser = XML_ParserCreate(NULL)))
	{
		return Status_CreateFailed;
	}
	XML_UseForeignDTD((XML_Parser)m_pParser, XML_TRUE);
	XML_SetParamEntityParsing((XML_Parser)m_pParser, XML_PARAM_ENTITY_PARSING_ALWAYS);
	XML_SetUserData((XML_Parser)m_pParser, this);
	XML_SetStartElementHandler((XML_Parser)m_pParser, (XML_StartElementHandler)&SAXParser::StartElementHandler);
	XML_SetEndElementHandler((XML_Parser)m_pParser, (XML_EndElementHandler)&SAXParser::EndElementHandler);
	XML_SetCharacterDataHandler((XML_Parser)m_pParser, (XML_CharacterDataHandler)&SAXParser::CharacterDataHandler);
	for (ObserversVector::iterator iter = m_vectorObservers.begin(); iter != m_vectorObservers.end(); ++iter)
	{
		(*iter)->OnBeginParse();
	}

	return Status_OK;
}

Status SAXParser::EndParse()
{
	if (NULL == m_pParser)
	{
		return Status_NotInitialized;
	}
	XML_ParserFree((XML_Parser)m_pParser);
	m_pParser = NULL;
	for (ObserversVector::iterator iter = m_vectorObservers.begin(); iter != m_vectorObservers.end(); ++iter)
	{
		(*iter)->OnEndParse();
	}

	return Status_OK;
}

Status SAXParser::ParseStream(const Char *szPath)
{
	IO::FileInputStream fis(szPath);

	if (!fis.IsOK())
	{
		return Status(Status_OpenFailed, "Failed to open '{1}'", szPath);
	}

	return ParseStream(&fis);
}

Status SAXParser::ParseStream(IO::IInputStream *pInputStream)
{
	Byte   buffer[BUFFER_SIZE];
	Size   cbAckSize;
	bool   bFirst;
	Status status;

	bFirst = true;
	for (;;)
	{
		if ((status = pInputStream->Read(buffer, sizeof(buffer), &cbAckSize)).IsNOK())
		{
			if (Status_EOF == status.GetCode())
			{
				if (XML_STATUS_OK != XML_Parse((XML_Parser)m_pParser, "", 0, 1))
				{
					return Status_ParseFailed;
				}

				break;
			}
			else
			{
				return status;
			}
		}
		if (0 < cbAckSize)
		{
			if (XML_STATUS_OK != XML_Parse((XML_Parser)m_pParser, (const char *)buffer, (int)cbAckSize, 0))
			{
				return Status_ParseFailed;
			}
		}
	}

	return Status_OK;
}

Status SAXParser::AddObserver(ISAXParserObserver *pObserver)
{
	m_vectorObservers.push_back(pObserver);

	return Status();
}

Status SAXParser::RemoveObservers()
{
	m_vectorObservers.clear();

	return Status();
}

void SAXParser::StartElementHandler(void *pContext, const void *szName, const void **pszAttrs)
{
	SAXParser *pThis = (SAXParser *)pContext;

	pThis->m_sText.clear();

	CX::Data::XML::ISAXParserObserver::AttrsVector vectorAttrs;
	CX::Data::XML::ISAXParserObserver::Attr        attr;

	const XML_Char **pPos = (const XML_Char **)pszAttrs;

	while (pPos[0] != NULL)
	{
		attr.sName = (const CX::Char *)pPos[0];
		attr.sValue = (const CX::Char *)pPos[1];
		vectorAttrs.push_back(attr);
		pPos += 2;
	}
	for (ObserversVector::iterator iter = pThis->m_vectorObservers.begin();
	     iter != pThis->m_vectorObservers.end(); ++iter)
	{
		(*iter)->OnStartElement((CX::Char *)szName, &vectorAttrs);
	}
}

void SAXParser::EndElementHandler(void *pContext, const void *szName)
{
	SAXParser *pThis = (SAXParser *)pContext;

	for (ObserversVector::iterator iter = pThis->m_vectorObservers.begin();
	     iter != pThis->m_vectorObservers.end(); ++iter)
	{
		(*iter)->OnEndElement((CX::Char *)szName, (CX::Char *)pThis->m_sText.c_str());
	}
	pThis->m_sText.clear();
}

void SAXParser::CharacterDataHandler(void *pContext, const void *pBuffer, int cbSize)
{
	SAXParser *pThis = (SAXParser *)pContext;

	pThis->m_sText.append((const CX::Char *)pBuffer, cbSize);
}

Status SAXParser::EscapeString(const Char *szStr, String *psStr)
{
	const Char *pPos = szStr;

	while (0 != *pPos)
	{
		if ('"' == *pPos)
		{
			*psStr += "&quot;";
		}
		else
		if ('\'' == *pPos)
		{
			*psStr += "&apos;";
		}
		else
		if ('<' == *pPos)
		{
			*psStr += "&lt;";
		}
		else
		if ('>' == *pPos)
		{
			*psStr += "&gt;";
		}
		else
		if ('&' == *pPos)
		{
			*psStr += "&amp;";
		}
		else
		{
			*psStr += *pPos;
		}

		pPos++;
	}

	return Status();
}

}//namespace XML

}//namespace Data

}//namespace CX
