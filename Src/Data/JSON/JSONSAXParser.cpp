/* 
 * CX - C++ framework for general purpose developement
 *
 * https://github.com/draede/cx
 * 
 * Copyright (C) 2014 draede - draede [at] outlook [dot] com
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

#include "CX/Data/JSON/JSONSAXParser.h"
#include "CX/Status.h"
#include "../../../Contrib/RapidJSON/Include/rapidjson.h"
#include "../../../Contrib/RapidJSON/Include/reader.h"
#include "RapidJSONStreams.h"


struct CX_Data_JSON_SAX_Handler
{
	typedef CX::Vector<CX::Data::JSON::IJSONSAXParserObserver *>::Type   ObserversVector;

	ObserversVector *m_pVectorObservers;

	CX::Bool Null()
	{
		if (!Notify(&CX::Data::JSON::IJSONSAXParserObserver::OnNullValue))
		{
			return CX::False;
		}

		return CX::True;
	}

	CX::Bool Bool(CX::Bool b)
	{
		if (!Notify(&CX::Data::JSON::IJSONSAXParserObserver::OnBoolValue, b))
		{
			return CX::False;
		}

		return CX::True;
	}

	CX::Bool Int(int i)
	{
		if (!Notify(&CX::Data::JSON::IJSONSAXParserObserver::OnIntValue, (CX::Int64)i))
		{
			return CX::False;
		}

		return CX::True;
	}

	CX::Bool Uint(unsigned i)
	{
		if (!Notify(&CX::Data::JSON::IJSONSAXParserObserver::OnIntValue, (CX::Int64)i))
		{
			return CX::False;
		}

		return CX::True;
	}

	CX::Bool Int64(int64_t i)
	{
		if (!Notify(&CX::Data::JSON::IJSONSAXParserObserver::OnIntValue, (CX::Int64)i))
		{
			return CX::False;
		}

		return CX::True;
	}

	CX::Bool Uint64(uint64_t i)
	{
		if (!Notify(&CX::Data::JSON::IJSONSAXParserObserver::OnIntValue, (CX::Int64)i))
		{
			return CX::False;
		}

		return CX::True;
	}

	CX::Bool Double(double d)
	{
		if (!Notify(&CX::Data::JSON::IJSONSAXParserObserver::OnRealValue, d))
		{
			return CX::False;
		}

		return CX::True;
	}

	CX::Bool String(const char *str, rapidjson::SizeType length, CX::Bool copy)
	{
		copy;

		if (!Notify(&CX::Data::JSON::IJSONSAXParserObserver::OnStringValue, str, (CX::Size)length))
		{
			return CX::False;
		}

		return CX::True;
	}

	CX::Bool StartObject()
	{
		if (!Notify(&CX::Data::JSON::IJSONSAXParserObserver::OnBeginObject))
		{
			return CX::False;
		}

		return CX::True;
	}

	CX::Bool Key(const char *str, rapidjson::SizeType length, CX::Bool copy)
	{
		copy;

		if (!Notify(&CX::Data::JSON::IJSONSAXParserObserver::OnKey, str, (CX::Size)length))
		{
			return CX::False;
		}

		return CX::True;
	}

	CX::Bool EndObject(rapidjson::SizeType memberCount)
	{
		memberCount;

		if (!Notify(&CX::Data::JSON::IJSONSAXParserObserver::OnEndObject))
		{
			return CX::False;
		}

		return CX::True;
	}

	CX::Bool StartArray()
	{
		if (!Notify(&CX::Data::JSON::IJSONSAXParserObserver::OnBeginArray))
		{
			return CX::False;
		}

		return CX::True;
	}

	CX::Bool EndArray(rapidjson::SizeType elementCount)
	{
		elementCount;

		if (!Notify(&CX::Data::JSON::IJSONSAXParserObserver::OnEndArray))
		{
			return CX::False;
		}

		return CX::True;
	}

	CX::Bool StartDoc()
	{
		if (!Notify(&CX::Data::JSON::IJSONSAXParserObserver::OnBeginParse))
		{
			return CX::False;
		}

		return CX::True;
	}

	CX::Bool EndDoc()
	{
		if (!Notify(&CX::Data::JSON::IJSONSAXParserObserver::OnEndParse))
		{
			return CX::False;
		}

		return CX::True;
	}

	CX::Bool Notify(CX::Bool (CX::Data::JSON::IJSONSAXParserObserver::*pfnHandler)())
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
	CX::Bool Notify(CX::Bool (CX::Data::JSON::IJSONSAXParserObserver::*pfnHandler)(T1), T1 p1)
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
	CX::Bool Notify(CX::Bool (CX::Data::JSON::IJSONSAXParserObserver::*pfnHandler)(T1, T2), T1 p1, 
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

JSONSAXParser::JSONSAXParser()
{
	if (NULL != (m_pHandler = New<CX_Data_JSON_SAX_Handler>()))
	{
		m_pHandler->m_pVectorObservers = &m_vectorObservers;
	}
}

JSONSAXParser::~JSONSAXParser()
{
	if (NULL != m_pHandler)
	{
		Delete(m_pHandler);
	}
}

Status JSONSAXParser::ParseStream(IO::IInputStream *pInputStream)
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

Status JSONSAXParser::ParseBuffer(const void *pBuffer, Size cbSize)
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

Status JSONSAXParser::ParseString(const Char *szString)
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

Status JSONSAXParser::ParseString(const String &sString)
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

Status JSONSAXParser::AddObserver(IJSONSAXParserObserver *pObserver)
{
	m_vectorObservers.push_back(pObserver);

	return Status();
}

Status JSONSAXParser::RemoveObservers()
{
	m_vectorObservers.clear();

	return Status();
}

}//namespace JSON

}//namespace Data

}//namespace CX
