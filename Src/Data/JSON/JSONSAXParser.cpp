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
#include "CX/Data/JSON/SAXParser.hpp"
#include "CX/IO/FileInputStream.hpp"
#include "CX/Status.hpp"
#include "rapidjson.h"
#include "reader.h"
#include "CX/Data/JSON/RapidJSONStreams.hpp"


static const CX::Char ERROR_MAX_DEPTH_EXCEEDED[]       = "Max include depth exceeded";
static const CX::Char ERROR_INCLUDE_ENTRY_NOT_STRING[] = "Include entry must be a string";
static const CX::Char ERROR_INCLUDE_PATH_NOT_FOUND[]   = "Include path not found";


struct CX_Data_JSON_SAX_Handler
{
	typedef CX::Vector<CX::Data::JSON::ISAXParserObserver *>::Type   ObserversVector;

	CX::Data::JSON::SAXParser   *m_pParser;
	CX_Data_JSON_SAX_Handler    *m_pRelay;
	CX::Bool                    m_bInclude;
	CX::Size                    m_cDepth;

	CX_Data_JSON_SAX_Handler()
	{
		m_pParser  = NULL;
		m_bInclude = CX::False;
		m_pRelay   = NULL;
		m_cDepth   = 0;
	}

	CX::Bool Null()
	{
		if (m_bInclude)
		{
			m_bInclude = false;
			if (NULL != m_pRelay)
			{
				if (!m_pRelay->String(ERROR_INCLUDE_ENTRY_NOT_STRING, 
				                      sizeof(ERROR_INCLUDE_ENTRY_NOT_STRING) - sizeof(CX::Char), CX::True))
				{
					return CX::False;
				}
			}
			else
			{
				if (!Notify(&CX::Data::JSON::ISAXParserObserver::OnStringValue, ERROR_INCLUDE_ENTRY_NOT_STRING, 
				            sizeof(ERROR_INCLUDE_ENTRY_NOT_STRING) - sizeof(CX::Char)))
				{
					return CX::False;
				}
			}
		}
		else
		{
			if (NULL != m_pRelay)
			{
				if (!m_pRelay->Null())
				{
					return CX::False;
				}
			}
			else
			{
				if (!Notify(&CX::Data::JSON::ISAXParserObserver::OnNullValue))
				{
					return CX::False;
				}
			}
		}

		return CX::True;
	}

	CX::Bool Bool(CX::Bool b)
	{
		if (m_bInclude)
		{
			m_bInclude = false;
			if (NULL != m_pRelay)
			{
				if (!m_pRelay->String(ERROR_INCLUDE_ENTRY_NOT_STRING, 
				                      sizeof(ERROR_INCLUDE_ENTRY_NOT_STRING) - sizeof(CX::Char), CX::True))
				{
					return CX::False;
				}
			}
			else
			{
				if (!Notify(&CX::Data::JSON::ISAXParserObserver::OnStringValue, ERROR_INCLUDE_ENTRY_NOT_STRING, 
				            sizeof(ERROR_INCLUDE_ENTRY_NOT_STRING) - sizeof(CX::Char)))
				{
					return CX::False;
				}
			}
		}
		else
		{
			if (NULL != m_pRelay)
			{
				if (!m_pRelay->Bool(b))
				{
					return CX::False;
				}
			}
			else
			{
				if (!Notify(&CX::Data::JSON::ISAXParserObserver::OnBoolValue, b))
				{
					return CX::False;
				}
			}
		}

		return CX::True;
	}

	CX::Bool Int(int i)
	{
		if (m_bInclude)
		{
			m_bInclude = false;
			if (NULL != m_pRelay)
			{
				if (!m_pRelay->String(ERROR_INCLUDE_ENTRY_NOT_STRING, 
				                      sizeof(ERROR_INCLUDE_ENTRY_NOT_STRING) - sizeof(CX::Char), CX::True))
				{
					return CX::False;
				}
			}
			else
			{
				if (!Notify(&CX::Data::JSON::ISAXParserObserver::OnStringValue, ERROR_INCLUDE_ENTRY_NOT_STRING, 
				            sizeof(ERROR_INCLUDE_ENTRY_NOT_STRING) - sizeof(CX::Char)))
				{
					return CX::False;
				}
			}
		}
		else
		{
			if (NULL != m_pRelay)
			{
				if (!m_pRelay->Int(i))
				{
					return CX::False;
				}
			}
			else
			{
				if (!Notify(&CX::Data::JSON::ISAXParserObserver::OnIntValue, (CX::Int64)i))
				{
					return CX::False;
				}
			}
		}

		return CX::True;
	}

	CX::Bool Uint(unsigned i)
	{
		if (m_bInclude)
		{
			m_bInclude = false;
			if (NULL != m_pRelay)
			{
				if (!m_pRelay->String(ERROR_INCLUDE_ENTRY_NOT_STRING, 
				                     sizeof(ERROR_INCLUDE_ENTRY_NOT_STRING) - sizeof(CX::Char), CX::True))
				{
					return CX::False;
				}
			}
			else
			{
				if (!Notify(&CX::Data::JSON::ISAXParserObserver::OnStringValue, ERROR_INCLUDE_ENTRY_NOT_STRING, 
				            sizeof(ERROR_INCLUDE_ENTRY_NOT_STRING) - sizeof(CX::Char)))
				{
					return CX::False;
				}
			}
		}
		else
		{
			if (NULL != m_pRelay)
			{
				if (!m_pRelay->Uint(i))
				{
					return CX::False;
				}
			}
			else
			{
				if (!Notify(&CX::Data::JSON::ISAXParserObserver::OnUIntValue, (CX::UInt64)i))
				{
					return CX::False;
				}
			}
		}

		return CX::True;
	}

	CX::Bool Int64(int64_t i)
	{
		if (m_bInclude)
		{
			m_bInclude = false;
			if (NULL != m_pRelay)
			{
				if (!m_pRelay->String(ERROR_INCLUDE_ENTRY_NOT_STRING, 
				                     sizeof(ERROR_INCLUDE_ENTRY_NOT_STRING) - sizeof(CX::Char), CX::True))
				{
					return CX::False;
				}
			}
			else
			{
				if (!Notify(&CX::Data::JSON::ISAXParserObserver::OnStringValue, ERROR_INCLUDE_ENTRY_NOT_STRING, 
				            sizeof(ERROR_INCLUDE_ENTRY_NOT_STRING) - sizeof(CX::Char)))
				{
					return CX::False;
				}
			}
		}
		else
		{
			if (NULL != m_pRelay)
			{
				if (!m_pRelay->Int64(i))
				{
					return CX::False;
				}
			}
			else
			{
				if (!Notify(&CX::Data::JSON::ISAXParserObserver::OnIntValue, (CX::Int64)i))
				{
					return CX::False;
				}
			}
		}

		return CX::True;
	}

	CX::Bool Uint64(uint64_t i)
	{
		if (m_bInclude)
		{
			m_bInclude = false;
			if (NULL != m_pRelay)
			{
				if (!m_pRelay->String(ERROR_INCLUDE_ENTRY_NOT_STRING, 
				                      sizeof(ERROR_INCLUDE_ENTRY_NOT_STRING) - sizeof(CX::Char), CX::True))
				{
					return CX::False;
				}
			}
			else
			{
				if (!Notify(&CX::Data::JSON::ISAXParserObserver::OnStringValue, ERROR_INCLUDE_ENTRY_NOT_STRING, 
				            sizeof(ERROR_INCLUDE_ENTRY_NOT_STRING) - sizeof(CX::Char)))
				{
					return CX::False;
				}
			}
		}
		else
		{
			if (NULL != m_pRelay)
			{
				if (!m_pRelay->Uint64(i))
				{
					return CX::False;
				}
			}
			else
			{
				if (!Notify(&CX::Data::JSON::ISAXParserObserver::OnUIntValue, (CX::UInt64)i))
				{
					return CX::False;
				}
			}
		}

		return CX::True;
	}

	bool RawNumber(const char* str, rapidjson::SizeType length, bool copy) 
	{
		str;
		length;
		copy;

		return false;
	}

	CX::Bool Double(double d)
	{
		if (m_bInclude)
		{
			m_bInclude = false;
			if (NULL != m_pRelay)
			{
				if (!m_pRelay->String(ERROR_INCLUDE_ENTRY_NOT_STRING, 
				                      sizeof(ERROR_INCLUDE_ENTRY_NOT_STRING) - sizeof(CX::Char), CX::True))
				{
					return CX::False;
				}
			}
			else
			{
				if (!Notify(&CX::Data::JSON::ISAXParserObserver::OnStringValue, ERROR_INCLUDE_ENTRY_NOT_STRING, 
				            sizeof(ERROR_INCLUDE_ENTRY_NOT_STRING) - sizeof(CX::Char)))
				{
					return CX::False;
				}
			}
		}
		else
		{
			if (NULL != m_pRelay)
			{
				if (!m_pRelay->Double(d))
				{
					return CX::False;
				}
			}
			else
			{
				if (!Notify(&CX::Data::JSON::ISAXParserObserver::OnRealValue, d))
				{
					return CX::False;
				}
			}
		}

		return CX::True;
	}

	CX::Bool String(const char *str, rapidjson::SizeType length, CX::Bool copy)
	{
		if (m_bInclude)
		{
			m_bInclude = false;
			if (m_pParser->m_cIncludeCurrentDepth >= m_pParser->m_cIncludeMaxDepth || 0 == length)
			{
				if (!Notify(&CX::Data::JSON::ISAXParserObserver::OnKey, ERROR_MAX_DEPTH_EXCEEDED, 
				            sizeof(ERROR_MAX_DEPTH_EXCEEDED) - sizeof(CX::Char)))
				{
					return CX::False;
				}
			}
			else
			{
				CX::Data::JSON::SAXParser   parser;
				CX::String                  sPath;
				CX::Status                  status;

				if ('/' == str[0] || '\\' == str[0] || (2 <= length && cx_isalpha(str[0]) && ':' == str[1]))
				{
					sPath.assign(str, length);
				}
				else
				{
					if (m_pParser->m_sFilePath.empty())
					{
						sPath.assign(str, length);
					}
					else
					{
						const CX::Char *pszStart = m_pParser->m_sFilePath.c_str();
						const CX::Char *pszPos;

						pszPos = pszStart + m_pParser->m_sFilePath.size() - 1;
						while ('/' != *pszPos && '\\' != *pszPos && pszPos > pszStart)
						{
							pszPos--;
						}
						if ('/' == *pszPos || '\\' == *pszPos)
						{
							sPath.assign(pszStart, pszPos - pszStart + 1);
							sPath.append(str, length);
						}
						else
						{
							sPath.assign(str, length);
						}
					}
				}

				parser.SetIncludeEntryName(m_pParser->m_sIncludeEntryName.c_str());
				parser.SetIncludeMaxDepth(m_pParser->m_cIncludeMaxDepth);
				parser.SetCurrentIncludeDepth(m_pParser->m_cIncludeCurrentDepth + 1);
				parser.m_pHandler->m_pRelay = this;

				if (!(status = parser.ParseStream(sPath.c_str())))
				{
					if (CX::Status_OpenFailed == status.GetCode())
					{
						if (!Notify(&CX::Data::JSON::ISAXParserObserver::OnKey, ERROR_INCLUDE_PATH_NOT_FOUND, 
						            sizeof(ERROR_INCLUDE_PATH_NOT_FOUND) - sizeof(CX::Char)))
						{
							return CX::False;
						}
					}

					return CX::False;
				}
			}
		}
		else
		{
			if (NULL != m_pRelay)
			{
				if (!m_pRelay->String(str, length, copy))
				{
					return CX::False;
				}
			}
			else
			{
				if (!Notify(&CX::Data::JSON::ISAXParserObserver::OnStringValue, str, (CX::Size)length))
				{
					return CX::False;
				}
			}
		}

		return CX::True;
	}

	CX::Bool StartObject()
	{
		if (m_bInclude)
		{
			m_bInclude = false;
			if (NULL != m_pRelay)
			{
				if (!m_pRelay->String(ERROR_INCLUDE_ENTRY_NOT_STRING, 
				                      sizeof(ERROR_INCLUDE_ENTRY_NOT_STRING) - sizeof(CX::Char), CX::True))
				{
					return CX::False;
				}
			}
			else
			{
				if (!Notify(&CX::Data::JSON::ISAXParserObserver::OnStringValue, ERROR_INCLUDE_ENTRY_NOT_STRING, 
				            sizeof(ERROR_INCLUDE_ENTRY_NOT_STRING) - sizeof(CX::Char)))
				{
					return CX::False;
				}
			}
		}
		else
		{
			if (NULL == m_pRelay || 0 < m_cDepth)
			{
				if (NULL != m_pRelay)
				{
					if (!m_pRelay->StartObject())
					{
						return CX::False;
					}
				}
				else
				{
					if (!Notify(&CX::Data::JSON::ISAXParserObserver::OnBeginObject))
					{
						return CX::False;
					}
				}
			}
		}

		m_cDepth++;

		return CX::True;
	}

	CX::Bool Key(const char *str, rapidjson::SizeType length, CX::Bool copy)
	{
		CX_UNUSED(copy);

		if (!m_pParser->m_sIncludeEntryName.empty() && length == m_pParser->m_sIncludeEntryName.size() && 
		    0 == memcmp(str, m_pParser->m_sIncludeEntryName.c_str(), length))
		{
			m_bInclude = CX::True;
		}
		else
		{
			if (NULL != m_pRelay)
			{
				if (!m_pRelay->Key(str, length, copy))
				{
					return CX::False;
				}
			}
			else
			{
				if (!Notify(&CX::Data::JSON::ISAXParserObserver::OnKey, str, (CX::Size)length))
				{
					return CX::False;
				}
			}
		}

		return CX::True;
	}

	CX::Bool EndObject(rapidjson::SizeType memberCount)
	{
		m_cDepth--;

		if (NULL == m_pRelay || 0 < m_cDepth)
		{
			if (NULL != m_pRelay)
			{
				if (!m_pRelay->EndObject(memberCount))
				{
					return CX::False;
				}
			}
			else
			{
				if (!Notify(&CX::Data::JSON::ISAXParserObserver::OnEndObject))
				{
					return CX::False;
				}
			}
		}

		return CX::True;
	}

	CX::Bool StartArray()
	{
		if (m_bInclude)
		{
			m_bInclude = false;
			if (NULL != m_pRelay)
			{
				if (!m_pRelay->String(ERROR_INCLUDE_ENTRY_NOT_STRING, 
				                      sizeof(ERROR_INCLUDE_ENTRY_NOT_STRING) - sizeof(CX::Char), CX::True))
				{
					return CX::False;
				}
			}
			else
			{
				if (!Notify(&CX::Data::JSON::ISAXParserObserver::OnStringValue, ERROR_INCLUDE_ENTRY_NOT_STRING, 
				            sizeof(ERROR_INCLUDE_ENTRY_NOT_STRING) - sizeof(CX::Char)))
				{
					return CX::False;
				}
			}
		}
		else
		{
			if (NULL == m_pRelay || 0 < m_cDepth)
			{
				if (NULL != m_pRelay)
				{
					if (!m_pRelay->StartArray())
					{
						return CX::False;
					}
				}
				else
				{
					if (!Notify(&CX::Data::JSON::ISAXParserObserver::OnBeginArray))
					{
						return CX::False;
					}
				}
			}
		}

		m_cDepth++;

		return CX::True;
	}

	CX::Bool EndArray(rapidjson::SizeType elementCount)
	{
		m_cDepth--;

		if (NULL == m_pRelay || 0 < m_cDepth)
		{
			if (NULL != m_pRelay)
			{
				if (!m_pRelay->EndArray(elementCount))
				{
					return CX::False;
				}
			}
			else
			{
				if (!Notify(&CX::Data::JSON::ISAXParserObserver::OnEndArray))
				{
					return CX::False;
				}
			}
		}

		return CX::True;
	}

	CX::Bool StartDoc()
	{
		if (m_bInclude)
		{
			m_bInclude = false;
			if (!Notify(&CX::Data::JSON::ISAXParserObserver::OnKey, m_pParser->m_sIncludeEntryName.c_str(), 
			            m_pParser->m_sIncludeEntryName.size()))
			{
				return CX::False;
			}
		}
		else
		{
			if (NULL == m_pRelay || 0 < m_cDepth)
			{
				if (NULL != m_pRelay)
				{
					if (!m_pRelay->StartDoc())
					{
						return CX::False;
					}
				}
				else
				{
					if (!Notify(&CX::Data::JSON::ISAXParserObserver::OnBeginParse))
					{
						return CX::False;
					}
				}
			}
		}

		return CX::True;
	}

	CX::Bool EndDoc()
	{
		if (NULL == m_pRelay || 0 < m_cDepth)
		{
			if (NULL != m_pRelay)
			{
				if (!m_pRelay->EndDoc())
				{
					return CX::False;
				}
			}
			else
			{
				if (!Notify(&CX::Data::JSON::ISAXParserObserver::OnEndParse))
				{
					return CX::False;
				}
			}
		}

		return CX::True;
	}

	CX::Bool Notify(CX::Bool (CX::Data::JSON::ISAXParserObserver::*pfnHandler)())
	{
		if (NULL != m_pRelay)
		{
			if (!m_pRelay->Notify(pfnHandler))
			{
				return CX::False;
			}
		}
		else
		{
			for (ObserversVector::iterator iter = m_pParser->m_vectorObservers.begin();
			     iter != m_pParser->m_vectorObservers.end(); ++iter)
			{
				if (!((*iter)->*pfnHandler)())
				{
					return CX::False;
				}
			}
		}

		return CX::True;
	}

	template <typename T1>
	CX::Bool Notify(CX::Bool (CX::Data::JSON::ISAXParserObserver::*pfnHandler)(T1), T1 p1)
	{
		if (NULL != m_pRelay)
		{
			if (!m_pRelay->Notify(pfnHandler, p1))
			{
				return CX::False;
			}
		}
		else
		{
			for (ObserversVector::iterator iter = m_pParser->m_vectorObservers.begin();
			     iter != m_pParser->m_vectorObservers.end(); ++iter)
			{
				if (!((*iter)->*pfnHandler)(p1))
				{
					return CX::False;
				}
			}
		}

		return CX::True;
	}

	template <typename T1, typename T2>
	CX::Bool Notify(CX::Bool (CX::Data::JSON::ISAXParserObserver::*pfnHandler)(T1, T2), T1 p1, T2 p2)
	{
		if (NULL != m_pRelay)
		{
			if (!m_pRelay->Notify(pfnHandler, p1, p2))
			{
				return CX::False;
			}
		}
		else
		{
			for (ObserversVector::iterator iter = m_pParser->m_vectorObservers.begin();
			     iter != m_pParser->m_vectorObservers.end(); ++iter)
			{
				if (!((*iter)->*pfnHandler)(p1, p2))
				{
					return CX::False;
				}
			}
		}

		return CX::True;
	}

};


namespace CX
{

namespace Data
{

namespace JSON
{

SAXParser::SAXParser()
{
	if (NULL != (m_pHandler = new (std::nothrow) CX_Data_JSON_SAX_Handler()))
	{
		m_pHandler->m_pParser = this;
	}
	m_cIncludeMaxDepth     = INCLUDE_MAX_DEPTH;
	m_cIncludeCurrentDepth = 0;
}

SAXParser::~SAXParser()
{
	if (NULL != m_pHandler)
	{
		delete(m_pHandler);
	}
}

#pragma warning(suppress: 6262)
Status SAXParser::ParseStream(IO::IInputStream *pInputStream)
{
	if (NULL == m_pHandler)
	{
		return Status(Status_MemAllocFailed, "Failed to allocate sax handler");
	}

	IO::FileInputStream *pFIS = dynamic_cast<IO::FileInputStream *>(pInputStream);
	
	if (NULL != pFIS)
	{
		m_sFilePath = pFIS->GetPath();
	}
	else
	{
		m_sFilePath.clear();
	}

	RapidJSONInputStream str(pInputStream);

	if (!m_pHandler->StartDoc())
	{
		return Status(Status_Cancelled, "Parsing was canceled");
	}

	rapidjson::GenericReader<rapidjson::UTF8<>, rapidjson::UTF8<> > reader;
	rapidjson::ParseResult res;

	res = reader.Parse(str, *m_pHandler);

	if (res.IsError())
	{
		return Status(Status_ParseFailed, "JSON parsing failed with error {1}", (int)res.Code());
	}

	if (!m_pHandler->EndDoc())
	{
		return Status(Status_Cancelled, "Parsing was canceled");
	}

	m_sFilePath.clear();

	return Status();
}

Status SAXParser::ParseStream(const Char *szPath)
{
	IO::FileInputStream fis(szPath);

	m_sFilePath = szPath;

	if (!fis.IsOK())
	{
		return Status(Status_OpenFailed, "Failed to open '{1}'", szPath);
	}

	return ParseStream(&fis);
}

Status SAXParser::ParseBuffer(const void *pBuffer, Size cbSize)
{
	if (NULL == m_pHandler)
	{
		return Status(Status_MemAllocFailed, "Failed to allocate sax handler");
	}

	m_sFilePath.clear();

	RapidJSONBufferInputStream str((const char *)pBuffer, cbSize);

	if (!m_pHandler->StartDoc())
	{
		return Status(Status_Cancelled, "Parsing was canceled");
	}

	rapidjson::GenericReader<rapidjson::UTF8<>, rapidjson::UTF8<> > reader;
	rapidjson::ParseResult res;

	res = reader.Parse(str, *m_pHandler);

	if (res.IsError())
	{
		return Status(Status_ParseFailed, "JSON parsing failed with error {1}", (int)res.Code());
	}

	if (!m_pHandler->EndDoc())
	{
		return Status(Status_Cancelled, "Parsing was canceled");
	}

	return Status();
}

Status SAXParser::ParseString(const Char *szString)
{
	if (NULL == m_pHandler)
	{
		return Status(Status_MemAllocFailed, "Failed to allocate sax handler");
	}

	m_sFilePath.clear();

	RapidJSONStringInputStream str(szString);

	if (!m_pHandler->StartDoc())
	{
		return Status(Status_Cancelled, "Parsing was canceled");
	}

	rapidjson::GenericReader<rapidjson::UTF8<>, rapidjson::UTF8<> > reader;
	rapidjson::ParseResult res;

	res = reader.Parse(str, *m_pHandler);

	if (res.IsError())
	{
		return Status(Status_ParseFailed, "JSON parsing failed with error {1}", (int)res.Code());
	}

	if (!m_pHandler->EndDoc())
	{
		return Status(Status_Cancelled, "Parsing was canceled");
	}

	return Status();
}

Status SAXParser::ParseString(const String &sString)
{
	if (NULL == m_pHandler)
	{
		return Status(Status_MemAllocFailed, "Failed to allocate sax handler");
	}

	m_sFilePath.clear();

	RapidJSONStringInputStream str(sString.c_str());

	if (!m_pHandler->StartDoc())
	{
		return Status(Status_Cancelled, "Parsing was canceled");
	}

	rapidjson::GenericReader<rapidjson::UTF8<>, rapidjson::UTF8<> > reader;
	rapidjson::ParseResult res;

	res = reader.Parse(str, *m_pHandler);

	if (res.IsError())
	{
		return Status(Status_ParseFailed, "JSON parsing failed with error {1}", (int)res.Code());
	}

	if (!m_pHandler->EndDoc())
	{
		return Status(Status_Cancelled, "Parsing was canceled");
	}

	return Status();
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

Status SAXParser::SetIncludeEntryName(const Char *szIncludeEntryName/* = INCLUDE_ENTRY_NAME()*/)
{
	m_sIncludeEntryName = szIncludeEntryName;

	return Status();
}

const Char *SAXParser::GetIncludeEntryName() const
{
	return m_sIncludeEntryName.c_str();
}

Status SAXParser::SetIncludeMaxDepth(Size cIncludeMaxDepth/* = INCLUDE_MAX_DEPTH*/)
{
	m_cIncludeMaxDepth = cIncludeMaxDepth;

	return Status();
}

Size SAXParser::GetIncludeMaxDepth() const
{
	return m_cIncludeMaxDepth;
}

void SAXParser::SetCurrentIncludeDepth(Size cCurrentIncludeDepth)
{
	m_cIncludeCurrentDepth = cCurrentIncludeDepth;
}

Status SAXParser::EscapeString(const Char *szStr, String *psStr)
{
	static Char hexdigits[] = 
	{ 
		'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F' 
	};
	const Byte *pPos;
	Char       escape[6];
	Status     status;

	pPos = (const Byte *)szStr;
	while (0 != *pPos)
	{
		if (0x20 > *pPos) //control char
		{
			escape[0] = '\\';
			escape[1] = 'u';
			escape[2] = '0';
			escape[3] = '0';
			escape[4] = hexdigits[*pPos / 16];
			escape[5] = hexdigits[*pPos % 16];
			psStr->append(escape, 6);
			pPos++;
		}
		else
		if (0x80 > *pPos) //ASCII
		{
			if ('"' == *pPos || '\\' == *pPos || '/' == *pPos || '\b' == *pPos || '\f' == *pPos ||
				'\n' == *pPos || '\r' == *pPos || '\t' == *pPos)
			{
				escape[0] = '\\';
				escape[1] = *pPos;
				psStr->append(escape, 2);
				pPos++;
			}
			else
			{
				*psStr += (Char)*pPos;
				pPos++;
			}
		}
		else
		if (0xC2 > *pPos)
		{
			return Status(Status_InvalidArg, "Invalid UTF8 string");
		}
		else
		{
			unsigned cp = 0;

			if (0xE0 > *pPos) //2 bytes
			{
				if (0 == *(pPos + 1))
				{
					return Status(Status_InvalidArg, "Invalid UTF8 string");
				}
				if ((pPos[1] & 0xC0) != 0x80)
				{
					return Status(Status_InvalidArg, "Invalid UTF8 string");
				}

				cp = (pPos[0] << 6) + pPos[1] - 0x3080;

				pPos += 2;
			}
			else
			if (0xF0 > *pPos) //3 bytes
			{
				if (0 == *(pPos + 1) || 0 == *(pPos + 2))
				{
					return Status(Status_InvalidArg, "Invalid UTF8 string");
				}
				if ((pPos[1] & 0xC0) != 0x80)
				{
					return Status(Status_InvalidArg, "Invalid UTF8 string");
				}
				if (pPos[0] == 0xE0 && pPos[1] < 0xA0)
				{
					return Status(Status_InvalidArg, "Invalid UTF8 string");
				}
				if ((pPos[2] & 0xC0) != 0x80)
				{
					return Status(Status_InvalidArg, "Invalid UTF8 string");
				}

				cp = (pPos[0] << 12) + (pPos[1] << 6) + pPos[2] - 0xE2080;

				pPos += 3;
			}
			else
			if (0xF5 > *pPos) //4 bytes
			{
				if (0 == *(pPos + 1) || 0 == *(pPos + 2) || 0 == *(pPos + 3))
				{
					return Status(Status_InvalidArg, "Invalid UTF8 string");
				}
				if ((pPos[1] & 0xC0) != 0x80)
				{
					return Status(Status_InvalidArg, "Invalid UTF8 string");
				}
				if (pPos[0] == 0xF0 && pPos[1] < 0x90)
				{
					return Status(Status_InvalidArg, "Invalid UTF8 string");
				}
				if (pPos[0] == 0xF4 && pPos[1] >= 0x90)
				{
					return Status(Status_InvalidArg, "Invalid UTF8 string");
				}
				if ((pPos[2] & 0xC0) != 0x80)
				{
					return Status(Status_InvalidArg, "Invalid UTF8 string");
				}
				if ((pPos[3] & 0xC0) != 0x80)
				{
					return Status(Status_InvalidArg, "Invalid UTF8 string");
				}

				cp = (pPos[0] << 18) + (pPos[1] << 12) + (pPos[2] << 6) + pPos[3] - 0x3C82080;

				pPos += 4;
			}
			else
			{
				return Status(Status_InvalidArg, "Invalid UTF8 string");
			}

			if (cp <= 0xD7FF || (cp >= 0xE000 && cp <= 0xFFFF)) 
			{
				escape[0] = '\\';
				escape[1] = 'u';
				escape[2] = hexdigits[((Byte *)&cp)[1] / 16];
				escape[3] = hexdigits[((Byte *)&cp)[1] % 16];
				escape[4] = hexdigits[((Byte *)&cp)[0] / 16];
				escape[5] = hexdigits[((Byte *)&cp)[0] % 16];
				psStr->append(escape, 6);
			}
			else 
			if (cp >= 0x010000 && cp <= 0x10FFFF)
			{
				// Surrogate pair
				unsigned s     = cp - 0x010000;
				unsigned lead  = (s >> 10) + 0xD800;
				unsigned trail = (s & 0x3FF) + 0xDC00;

				escape[0] = '\\';
				escape[1] = 'u';
				escape[2] = hexdigits[((Byte *)&lead)[1] / 16];
				escape[3] = hexdigits[((Byte *)&lead)[1] % 16];
				escape[4] = hexdigits[((Byte *)&lead)[0] / 16];
				escape[5] = hexdigits[((Byte *)&lead)[0] % 16];
				psStr->append(escape, 6);
				escape[0] = '\\';
				escape[1] = 'u';
				escape[2] = hexdigits[((Byte *)&trail)[1] / 16];
				escape[3] = hexdigits[((Byte *)&trail)[1] % 16];
				escape[4] = hexdigits[((Byte *)&trail)[0] / 16];
				escape[5] = hexdigits[((Byte *)&trail)[0] % 16];
				psStr->append(escape, 6);
			}
			else
			{
				return Status(Status_InvalidArg, "Invalid UTF8 string");
			}
		}
	}

	return Status();
}

}//namespace JSON

}//namespace Data

}//namespace CX
