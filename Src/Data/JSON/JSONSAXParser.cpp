
#include "CX/Data/JSON/JSONSAXParser.h"
#include "CX/Status.h"
#include "../../../Contrib/RapidJSON/Include/rapidjson.h"
#include "../../../Contrib/RapidJSON/Include/reader.h"
#include "RapidJSONStreams.h"


struct CX_Data_JSON_SAX_Handler
{
	typedef CX::Vector<CX::Data::JSON::IJSONSAXParserObserver *>::Type   ObserversVector;

	ObserversVector *m_pVectorObservers;

	bool Null()
	{
		if (!Notify(&CX::Data::JSON::IJSONSAXParserObserver::OnNullValue))
		{
			return false;
		}

		return true;
	}

	bool Bool(bool b)
	{
		if (!Notify(&CX::Data::JSON::IJSONSAXParserObserver::OnBoolValue, b))
		{
			return false;
		}

		return true;
	}

	bool Int(int i)
	{
		if (!Notify(&CX::Data::JSON::IJSONSAXParserObserver::OnIntValue, (CX::Int64)i))
		{
			return false;
		}

		return true;
	}

	bool Uint(unsigned i)
	{
		if (!Notify(&CX::Data::JSON::IJSONSAXParserObserver::OnIntValue, (CX::Int64)i))
		{
			return false;
		}

		return true;
	}

	bool Int64(int64_t i)
	{
		if (!Notify(&CX::Data::JSON::IJSONSAXParserObserver::OnIntValue, (CX::Int64)i))
		{
			return false;
		}

		return true;
	}

	bool Uint64(uint64_t i)
	{
		if (!Notify(&CX::Data::JSON::IJSONSAXParserObserver::OnIntValue, (CX::Int64)i))
		{
			return false;
		}

		return true;
	}

	bool Double(double d)
	{
		if (!Notify(&CX::Data::JSON::IJSONSAXParserObserver::OnRealValue, d))
		{
			return false;
		}

		return true;
	}

	bool String(const char *str, rapidjson::SizeType length, bool copy)
	{
		if (!Notify(&CX::Data::JSON::IJSONSAXParserObserver::OnStringValue, str, (CX::Size)length))
		{
			return false;
		}

		return true;
	}

	bool StartObject()
	{
		if (!Notify(&CX::Data::JSON::IJSONSAXParserObserver::OnBeginObject))
		{
			return false;
		}

		return true;
	}

	bool Key(const char *str, rapidjson::SizeType length, bool copy)
	{
		if (!Notify(&CX::Data::JSON::IJSONSAXParserObserver::OnKey, str, (CX::Size)length))
		{
			return false;
		}

		return true;
	}

	bool EndObject(rapidjson::SizeType memberCount)
	{
		if (!Notify(&CX::Data::JSON::IJSONSAXParserObserver::OnEndObject))
		{
			return false;
		}

		return true;
	}

	bool StartArray()
	{
		if (!Notify(&CX::Data::JSON::IJSONSAXParserObserver::OnBeginArray))
		{
			return false;
		}

		return true;
	}

	bool EndArray(rapidjson::SizeType elementCount)
	{
		if (!Notify(&CX::Data::JSON::IJSONSAXParserObserver::OnEndArray))
		{
			return false;
		}

		return true;
	}

	bool StartDoc()
	{
		if (!Notify(&CX::Data::JSON::IJSONSAXParserObserver::OnBeginParse))
		{
			return false;
		}

		return true;
	}

	bool EndDoc()
	{
		if (!Notify(&CX::Data::JSON::IJSONSAXParserObserver::OnEndParse))
		{
			return false;
		}

		return true;
	}

	bool Notify(bool (CX::Data::JSON::IJSONSAXParserObserver::*pfnHandler)())
	{
		for (ObserversVector::iterator iter = m_pVectorObservers->begin();
		     iter != m_pVectorObservers->end(); ++iter)
		{
			if (!((*iter)->*pfnHandler)())
			{
				return false;
			}
		}

		return true;
	}

	template <typename T1>
	bool Notify(bool (CX::Data::JSON::IJSONSAXParserObserver::*pfnHandler)(T1), T1 p1)
	{
		for (ObserversVector::iterator iter = m_pVectorObservers->begin();
		     iter != m_pVectorObservers->end(); ++iter)
		{
			if (!((*iter)->*pfnHandler)(p1))
			{
				return false;
			}
		}

		return true;
	}

	template <typename T1, typename T2>
	bool Notify(bool (CX::Data::JSON::IJSONSAXParserObserver::*pfnHandler)(T1, T2), T1 p1, T2 p2)
	{
		for (ObserversVector::iterator iter = m_pVectorObservers->begin();
		     iter != m_pVectorObservers->end(); ++iter)
		{
			if (!((*iter)->*pfnHandler)(p1, p2))
			{
				return false;
			}
		}

		return true;
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
	Status::Clear();

	if (NULL != (m_pHandler = New<CX_Data_JSON_SAX_Handler>()))
	{
		m_pHandler->m_pVectorObservers = &m_vectorObservers;
	}
	else
	{
		Status::Set(Status_MemAllocFailed, "Failed to allocate sax handler");
	}
}

JSONSAXParser::~JSONSAXParser()
{
	Status::Clear();
	if (NULL != m_pHandler)
	{
		Delete(m_pHandler);
	}
}

StatusCode JSONSAXParser::ParseStream(IO::IInputStream *pInputStream)
{
	Status::Clear();

	if (NULL == m_pHandler)
	{
		return Status::Set(Status_MemAllocFailed, "Failed to allocate sax handler");
	}

	RapidJSONInputStream str(pInputStream);

	if (!m_pHandler->StartDoc())
	{
		return Status::Set(Status_Cancelled, "Parsing was cancelled");
	}

	rapidjson::GenericReader<rapidjson::UTF8<>, rapidjson::UTF8<> > reader;
	rapidjson::ParseResult res;

	res = reader.Parse(str, *m_pHandler);

	if (res.IsError())
	{
		return Status::Set(Status_ParseFailed, "JSON parsing failed with error {1}", (int)res.Code());
	}

	if (!m_pHandler->EndDoc())
	{
		return Status::Set(Status_Cancelled, "Parsing was cancelled");
	}

	return Status_OK;
}

StatusCode JSONSAXParser::ParseBuffer(const void *pBuffer, Size cbSize)
{
	Status::Clear();
	
	if (NULL == m_pHandler)
	{
		return Status::Set(Status_MemAllocFailed, "Failed to allocate sax handler");
	}

	RapidJSONBufferInputStream str((const char *)pBuffer, cbSize);

	if (!m_pHandler->StartDoc())
	{
		return Status::Set(Status_Cancelled, "Parsing was cancelled");
	}

	rapidjson::GenericReader<rapidjson::UTF8<>, rapidjson::UTF8<> > reader;
	rapidjson::ParseResult res;

	res = reader.Parse(str, *m_pHandler);

	if (res.IsError())
	{
		return Status::Set(Status_ParseFailed, "JSON parsing failed with error {1}", (int)res.Code());
	}

	if (!m_pHandler->EndDoc())
	{
		return Status::Set(Status_Cancelled, "Parsing was cancelled");
	}

	return Status_OK;
}

StatusCode JSONSAXParser::ParseString(const Char *szString)
{
	Status::Clear();
	
	if (NULL == m_pHandler)
	{
		return Status::Set(Status_MemAllocFailed, "Failed to allocate sax handler");
	}

	RapidJSONStringInputStream str(szString);

	if (!m_pHandler->StartDoc())
	{
		return Status::Set(Status_Cancelled, "Parsing was cancelled");
	}

	rapidjson::GenericReader<rapidjson::UTF8<>, rapidjson::UTF8<> > reader;
	rapidjson::ParseResult res;

	res = reader.Parse(str, *m_pHandler);

	if (res.IsError())
	{
		return Status::Set(Status_ParseFailed, "JSON parsing failed with error {1}", (int)res.Code());
	}

	if (!m_pHandler->EndDoc())
	{
		return Status::Set(Status_Cancelled, "Parsing was cancelled");
	}

	return Status_OK;
}

StatusCode JSONSAXParser::ParseString(const String &sString)
{
	Status::Clear();

	if (NULL == m_pHandler)
	{
		return Status::Set(Status_MemAllocFailed, "Failed to allocate sax handler");
	}

	RapidJSONStringInputStream str(sString.c_str());

	if (!m_pHandler->StartDoc())
	{
		return Status::Set(Status_Cancelled, "Parsing was cancelled");
	}

	rapidjson::GenericReader<rapidjson::UTF8<>, rapidjson::UTF8<> > reader;
	rapidjson::ParseResult res;

	res = reader.Parse(str, *m_pHandler);

	if (res.IsError())
	{
		return Status::Set(Status_ParseFailed, "JSON parsing failed with error {1}", (int)res.Code());
	}

	if (!m_pHandler->EndDoc())
	{
		return Status::Set(Status_Cancelled, "Parsing was cancelled");
	}

	return Status_OK;
}

StatusCode JSONSAXParser::AddObserver(IJSONSAXParserObserver *pObserver)
{
	Status::Clear();

	m_vectorObservers.push_back(pObserver);

	return Status_OK;
}

StatusCode JSONSAXParser::RemoveObservers()
{
	Status::Clear();

	m_vectorObservers.clear();

	return Status_OK;
}

}//namespace JSON

}//namespace Data

}//namespace CX
