
#include "CX/Data/JSON/DataWriter.hpp"
#include "CX/Print.hpp"


namespace CX
{

namespace Data
{

namespace JSON
{

DataWriter::DataWriter(IO::IOutputStream *pOutputStream)
{
	m_pOutputStream = pOutputStream;
	m_nState        = State_Begin;
}

DataWriter::~DataWriter()
{
}

Status DataWriter::BeginRootObject()
{
	Status status;

	if (NULL == m_pOutputStream)
	{
		return Status(Status_NotInitialized, "No valid output stream");
	}
	if (State_Begin != m_nState)
	{
		return Status(Status_InvalidCall, "Out of order call (must be called at the beginning)");
	}
	if ((status = Print(m_pOutputStream, "{\n")).IsNOK())
	{
		return status;
	}
	m_nState = State_RootObject;

	return Status();
}

Status DataWriter::EndRootObject()
{
	Status status;

	if (NULL == m_pOutputStream)
	{
		return Status(Status_NotInitialized, "No valid output stream");
	}
	if (State_RootObject != m_nState)
	{
		return Status(Status_InvalidCall, "Out of order call (must be in the root object)");
	}
	if ((status = Print(m_pOutputStream, "}\n")).IsNOK())
	{
		return status;
	}
	m_nState = State_End;

	return Status();
}

Status DataWriter::BeginRootArray()
{
}

Status DataWriter::EndRootArray()
{
}

Status DataWriter::ObjWriteNull(const Char *szName)
{
}

Status DataWriter::ObjWriteBool(const Char *szName, Bool bValue)
{
}

Status DataWriter::ObjWriteInt(const Char *szName, Int64 nValue)
{
}

Status DataWriter::ObjWriteReal(const Char *szName, Double lfValue)
{
}

Status DataWriter::ObjWriteString(const Char *szName, const Char *szValue)
{
}

Status DataWriter::ObjBeginObject(const Char *szName)
{
}

Status DataWriter::ObjEndObject()
{
}

Status DataWriter::ObjBeginArray(const Char *szName)
{
}

Status DataWriter::ObjEndArray()
{
}

Status DataWriter::ArrWriteNull()
{
}

Status DataWriter::ArrWriteBool(Bool bValue)
{
}

Status DataWriter::ArrWriteInt(Int64 nValue)
{
}

Status DataWriter::ArrWriteReal(Double lfValue)
{
}

Status DataWriter::ArrWriteBString(const Char *szValue)
{
}

Status DataWriter::ArrBeginObject()
{
}

Status DataWriter::ArrEndObject()
{
}

Status DataWriter::ArrBeginArray()
{
}

Status DataWriter::ArrEndArray()
{
}

}//namespace JSON

}//namespace Data

}//namespace CX

