/* 
 * CX - C++ framework for general purpose development
 *
 * https://github.com/draede/cx
 * 
 * Copyright (C) 2014-2015 draede - draede [at] outlook [dot] com
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

#include "CX/Data/JSON/SAXParser.hpp"
#include "CX/Status.hpp"
#include "../../../Contrib/RapidJSON/Include/rapidjson.h"
#include "../../../Contrib/RapidJSON/Include/reader.h"
#include "RapidJSONStreams.hpp"


struct CX_Data_JSON_SAX_Handler
{
	typedef CX::Vector<CX::Data::JSON::ISAXParserObserver *>::Type   ObserversVector;

	ObserversVector *m_pVectorObservers;

	CX::Bool Null()
	{
		if (!Notify(&CX::Data::JSON::ISAXParserObserver::OnNullValue))
		{
			return CX::False;
		}

		return CX::True;
	}

	CX::Bool Bool(CX::Bool b)
	{
		if (!Notify(&CX::Data::JSON::ISAXParserObserver::OnBoolValue, b))
		{
			return CX::False;
		}

		return CX::True;
	}

	CX::Bool Int(int i)
	{
		if (!Notify(&CX::Data::JSON::ISAXParserObserver::OnIntValue, (CX::Int64)i))
		{
			return CX::False;
		}

		return CX::True;
	}

	CX::Bool Uint(unsigned i)
	{
		if (!Notify(&CX::Data::JSON::ISAXParserObserver::OnIntValue, (CX::Int64)i))
		{
			return CX::False;
		}

		return CX::True;
	}

	CX::Bool Int64(int64_t i)
	{
		if (!Notify(&CX::Data::JSON::ISAXParserObserver::OnIntValue, (CX::Int64)i))
		{
			return CX::False;
		}

		return CX::True;
	}

	CX::Bool Uint64(uint64_t i)
	{
		if (!Notify(&CX::Data::JSON::ISAXParserObserver::OnIntValue, (CX::Int64)i))
		{
			return CX::False;
		}

		return CX::True;
	}

	CX::Bool Double(double d)
	{
		if (!Notify(&CX::Data::JSON::ISAXParserObserver::OnRealValue, d))
		{
			return CX::False;
		}

		return CX::True;
	}

	CX::Bool String(const char *str, rapidjson::SizeType length, CX::Bool copy)
	{
		copy;

		if (!Notify(&CX::Data::JSON::ISAXParserObserver::OnStringValue, str, (CX::Size)length))
		{
			return CX::False;
		}

		return CX::True;
	}

	CX::Bool StartObject()
	{
		if (!Notify(&CX::Data::JSON::ISAXParserObserver::OnBeginObject))
		{
			return CX::False;
		}

		return CX::True;
	}

	CX::Bool Key(const char *str, rapidjson::SizeType length, CX::Bool copy)
	{
		copy;

		if (!Notify(&CX::Data::JSON::ISAXParserObserver::OnKey, str, (CX::Size)length))
		{
			return CX::False;
		}

		return CX::True;
	}

	CX::Bool EndObject(rapidjson::SizeType memberCount)
	{
		memberCount;

		if (!Notify(&CX::Data::JSON::ISAXParserObserver::OnEndObject))
		{
			return CX::False;
		}

		return CX::True;
	}

	CX::Bool StartArray()
	{
		if (!Notify(&CX::Data::JSON::ISAXParserObserver::OnBeginArray))
		{
			return CX::False;
		}

		return CX::True;
	}

	CX::Bool EndArray(rapidjson::SizeType elementCount)
	{
		elementCount;

		if (!Notify(&CX::Data::JSON::ISAXParserObserver::OnEndArray))
		{
			return CX::False;
		}

		return CX::True;
	}

	CX::Bool StartDoc()
	{
		if (!Notify(&CX::Data::JSON::ISAXParserObserver::OnBeginParse))
		{
			return CX::False;
		}

		return CX::True;
	}

	CX::Bool EndDoc()
	{
		if (!Notify(&CX::Data::JSON::ISAXParserObserver::OnEndParse))
		{
			return CX::False;
		}

		return CX::True;
	}

	CX::Bool Notify(CX::Bool (CX::Data::JSON::ISAXParserObserver::*pfnHandler)())
	{
		for (ObserversVector::iterator iter = m_pVectorObservers->begin();
		     iter != m_pVectorObservers->end(); ++iter)
		{
			if (!((*iter)->*pfnHandler)())
			{
				return CX::False;
			}
		}

		return CX::True;
	}

	template <typename T1>
	CX::Bool Notify(CX::Bool (CX::Data::JSON::ISAXParserObserver::*pfnHandler)(T1), T1 p1)
	{
		for (ObserversVector::iterator iter = m_pVectorObservers->begin();
		     iter != m_pVectorObservers->end(); ++iter)
		{
			if (!((*iter)->*pfnHandler)(p1))
			{
				return CX::False;
			}
		}

		return CX::True;
	}

	template <typename T1, typename T2>
	CX::Bool Notify(CX::Bool (CX::Data::JSON::ISAXParserObserver::*pfnHandler)(T1, T2), T1 p1, 
	                T2 p2)
	{
		for (ObserversVector::iterator iter = m_pVectorObservers->begin();
		     iter != m_pVectorObservers->end(); ++iter)
		{
			if (!((*iter)->*pfnHandler)(p1, p2))
			{
				return CX::False;
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
	if (NULL != (m_pHandler = new CX_Data_JSON_SAX_Handler()))
	{
		m_pHandler->m_pVectorObservers = &m_vectorObservers;
	}
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

	RapidJSONInputStream str(pInputStream);

	if (!m_pHandler->StartDoc())
	{
		return Status(Status_Cancelled, "Parsing was cancelled");
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
		return Status(Status_Cancelled, "Parsing was cancelled");
	}

	return Status();
}

Status SAXParser::ParseBuffer(const void *pBuffer, Size cbSize)
{
	if (NULL == m_pHandler)
	{
		return Status(Status_MemAllocFailed, "Failed to allocate sax handler");
	}

	RapidJSONBufferInputStream str((const char *)pBuffer, cbSize);

	if (!m_pHandler->StartDoc())
	{
		return Status(Status_Cancelled, "Parsing was cancelled");
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
		return Status(Status_Cancelled, "Parsing was cancelled");
	}

	return Status();
}

Status SAXParser::ParseString(const Char *szString)
{
	if (NULL == m_pHandler)
	{
		return Status(Status_MemAllocFailed, "Failed to allocate sax handler");
	}

	RapidJSONStringInputStream str(szString);

	if (!m_pHandler->StartDoc())
	{
		return Status(Status_Cancelled, "Parsing was cancelled");
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
		return Status(Status_Cancelled, "Parsing was cancelled");
	}

	return Status();
}

Status SAXParser::ParseString(const String &sString)
{
	if (NULL == m_pHandler)
	{
		return Status(Status_MemAllocFailed, "Failed to allocate sax handler");
	}

	RapidJSONStringInputStream str(sString.c_str());

	if (!m_pHandler->StartDoc())
	{
		return Status(Status_Cancelled, "Parsing was cancelled");
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
		return Status(Status_Cancelled, "Parsing was cancelled");
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
