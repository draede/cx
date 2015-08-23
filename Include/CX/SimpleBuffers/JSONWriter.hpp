
#pragma once


#include "CX/SimpleBuffers/IWriter.hpp"
#include "CX/Stack.hpp"
#include "CX/Print.hpp"
#include "CX/Str/Z85BinStr.hpp"


namespace CX
{

namespace SimpleBuffers
{

class JSONWriter : public IWriter
{
public:

	JSONWriter();

	~JSONWriter();

	virtual Status Begin(IO::IOutputStream *pOutputStream);

	virtual Status End();

	virtual Status WriteBool(Bool v, const Char *szName = NULL);

	virtual Status WriteInt8(Int8 v, const Char *szName = NULL);

	virtual Status WriteUInt8(UInt8 v, const Char *szName = NULL);

	virtual Status WriteInt16(Int16 v, const Char *szName = NULL);

	virtual Status WriteUInt16(UInt16 v, const Char *szName = NULL);

	virtual Status WriteInt32(Int32 v, const Char *szName = NULL);

	virtual Status WriteUInt32(UInt32 v, const Char *szName = NULL);

	virtual Status WriteInt64(Int64 v, const Char *szName = NULL);

	virtual Status WriteUInt64(UInt64 v, const Char *szName = NULL);

	virtual Status WriteFloat(Float v, const Char *szName = NULL);

	virtual Status WriteDouble(Double v, const Char *szName = NULL);

	virtual Status WriteString(const String &v, const Char *szName = NULL);

	virtual Status WriteBLOB(const BLOB &v, const Char *szName = NULL);

	virtual Status BeginObject(const Char *szName = NULL);

	virtual Status EndObject();

	virtual Status BeginArray(const Char *szName = NULL);

	virtual Status EndArray();

private:

	static const Size INDENT_TAB_SIZE = 3;

	enum State
	{
		State_Root,
		State_Object,
		State_Array,
	};

	struct StateData
	{
		StateData()
		{
		}

		StateData(State nState)
		{
			this->nState = nState;
			this->cElems = 0;
		}

		State nState;
		Size  cElems;

	};

	typedef Stack<StateData>::Type  StatesStack;

	StatesStack         m_stackStates;
	IO::IOutputStream   *m_pOutputStream;
	String              m_sIndent;
	Size                m_cIndent;

	template <typename T>
	static Status WriteFunc(IO::IOutputStream *pOutputStream, const T &v, const Char *szName)
	{
		if (NULL != szName)
		{
			return Print(pOutputStream, "\"{1}\": {2}", szName, v);
		}
		else
		{
			return Print(pOutputStream, "{1}", v);
		}
	}

	template <typename T>
	Status Write(Status (* WriteFuncProc)(IO::IOutputStream *, const T &, const Char *szName), const T &v, 
	             const Char *szName = NULL)
	{
		if (m_stackStates.empty())
		{
			return Status(Status_InvalidCall, "Begin was not called");
		}

		Status status;

		if (State_Object == m_stackStates.top().nState)
		{
			if (NULL == szName)
			{
				return Status(Status_InvalidCall, "Member must have a name");
			}
		}
		else
		if (State_Array == m_stackStates.top().nState)
		{
			if (NULL != szName)
			{
				return Status(Status_InvalidCall, "Item must not have a name");
			}
		}
		else
		{
			return Status(Status_InvalidCall, "Data must be placed inside objects or arrays");
		}
		if ((status = PrintFieldsSeparator(false)).IsNOK())
		{
			return status;
		}
		if ((status = PrintIndent()).IsNOK())
		{
			return status;
		}
		if ((status = WriteFuncProc(m_pOutputStream, v, szName)).IsNOK())
		{
			return status;
		}
		m_stackStates.top().cElems++;

		return Status();
	}

	Status PrintFieldsSeparator(bool bEnd);

	Status PrintIndent();

};

template <>
Status JSONWriter::WriteFunc<Bool>(IO::IOutputStream *pOutputStream, const Bool &v, const Char *szName)
{
	if (NULL != szName)
	{
		return Print(pOutputStream, "\"{1}\": {2}", szName, v ? "true" : "false");
	}
	else
	{
		return Print(pOutputStream, "{1}", v ? "true" : "false");
	}
}

template <>
Status JSONWriter::WriteFunc<String>(IO::IOutputStream *pOutputStream, const String &v, const Char *szName)
{
	if (NULL != szName)
	{
		return Print(pOutputStream, "\"{1}\": \"{2}\"", szName, v);
	}
	else
	{
		return Print(pOutputStream, "\"{1}\"", v);
	}
}

template <>
Status JSONWriter::WriteFunc<BLOB>(IO::IOutputStream *pOutputStream, const BLOB &v, const Char *szName)
{
	if (0 == v.size())
	{
		if (NULL != szName)
		{
			return Print(pOutputStream, "\"{1}\": \"\"", szName);
		}
		else
		{
			return Print(pOutputStream, "\"\"");
		}
	}

	Str::Z85BinStr z85;
	Size           cLen;
	Char           *szBuffer;
	Status         status;

	cLen     = z85.GetStrLenFromBinSize(&v[0], v.size());
	szBuffer = NULL;
	for (;;)
	{
		if (NULL == (szBuffer = (Char *)Mem::Alloc(cLen + 1)))
		{
			status = Status(Status_MemAllocFailed, "Failed to allocate temp string");

			break;
		}
		if ((status = z85.ToString(&v[0], v.size(), szBuffer, cLen)).IsNOK())
		{
			break;
		}

		break;
	}
	if (status.IsOK() && NULL != szBuffer)
	{
		szBuffer[cLen] = 0;
		if (NULL != szName)
		{
			status = Print(pOutputStream, "\"{1}\": \"{2}\"", szName, szBuffer);
		}
		else
		{
			status = Print(pOutputStream, "\"{1}\"", szBuffer);
		}
	}
	if (NULL != szBuffer)
	{
		Mem::Free(szBuffer);
	}
		
	return status;
}

}//namespace SimpleBuffers

}//namespace CX
