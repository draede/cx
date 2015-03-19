/* 
 * CX - C++ framework for general purpose developement
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

#include "CX/Data/JSON/DataWriter.hpp"
#include "CX/Data/JSON/SAXParser.hpp"
#include "CX/Str/UTF8.hpp"
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
}

DataWriter::~DataWriter()
{
}

Status DataWriter::WriteIndent()
{
	if (m_sIndent.size() < m_stackStates.size())
	{
		for (Size i = m_sIndent.size(); i < m_stackStates.size(); i++)
		{
			m_sIndent += "\t";
		}
	}

	Size    cbAckSize;
	Status  status;

	if ((status = m_pOutputStream->Write(m_sIndent.c_str(), m_stackStates.size(), &cbAckSize)).IsNOK())
	{
		return status;
	}
	if (m_stackStates.size() != cbAckSize)
	{
		return Status_WriteFailed;
	}

	return Status();
}

Status DataWriter::WriteItemSeparator(bool bForceNoComma/* = false*/)
{
	Status status;

	if (0 == m_stackStates.top().cCount || bForceNoComma)
	{
		if ((status = Print(m_pOutputStream, "\n")).IsNOK())
		{
			return status;
		}
	}
	else
	{
		if ((status = Print(m_pOutputStream, ",\n")).IsNOK())
		{
			return status;
		}
	}
	if ((status = WriteIndent()).IsNOK())
	{
		return status;
	}

	return Status();
}

Status DataWriter::Begin()
{
	Status status;

	if (NULL == m_pOutputStream || !m_pOutputStream->IsOK())
	{
		return Status(Status_NotInitialized, "Input stream is not valid");
	}
	if (!m_stackStates.empty())
	{
		return Status(Status_InvalidCall, "Out of order call");
	}
	m_stackStates.push(StateData(State_Object));
	if ((status = Print(m_pOutputStream, "{{")).IsNOK())
	{
		return status;
	}

	return Status();
}

Status DataWriter::End()
{
	Status status;

	if (NULL == m_pOutputStream || !m_pOutputStream->IsOK())
	{
		return Status(Status_NotInitialized, "Input stream is not valid");
	}
	if (m_stackStates.empty())
	{
		return Status(Status_InvalidCall, "Out of order call");
	}
	if (State_Object != m_stackStates.top().nState)
	{
		return Status(Status_InvalidCall, "Out of order call");
	}
	if ((status = Print(m_pOutputStream, "\n}")).IsNOK())
	{
		return status;
	}
	m_stackStates.pop();
	if (!m_stackStates.empty())
	{
		return Status(Status_InvalidCall, "Out of order call");
	}

	return Status();
}

Status DataWriter::BeginObjectObject(const Char *szName)
{
	Status status;

	if (NULL == m_pOutputStream || !m_pOutputStream->IsOK())
	{
		return Status(Status_NotInitialized, "Input stream is not valid");
	}
	if (m_stackStates.empty())
	{
		return Status(Status_InvalidCall, "Out of order call");
	}
	if (State_Object != m_stackStates.top().nState)
	{
		return Status(Status_InvalidCall, "Out of order call");
	}
	if ((status = WriteItemSeparator()).IsNOK())
	{
		return status;
	}
	if ((status = Print(m_pOutputStream, "\"{1}\":", szName)).IsNOK())
	{
		return status;
	}
	if ((status = WriteItemSeparator(true)).IsNOK())
	{
		return status;
	}
	if ((status = Print(m_pOutputStream, "{{", szName)).IsNOK())
	{
		return status;
	}
	m_stackStates.top().cCount++;
	m_stackStates.push(StateData(State_Object));

	return Status();
}

Status DataWriter::EndObjectObject()
{
	Status status;

	if (NULL == m_pOutputStream || !m_pOutputStream->IsOK())
	{
		return Status(Status_NotInitialized, "Input stream is not valid");
	}
	if (m_stackStates.empty())
	{
		return Status(Status_InvalidCall, "Out of order call");
	}
	if (State_Object != m_stackStates.top().nState)
	{
		return Status(Status_InvalidCall, "Out of order call");
	}
	m_stackStates.pop();
	if ((status = WriteItemSeparator(true)).IsNOK())
	{
		return status;
	}
	if ((status = Print(m_pOutputStream, "}")).IsNOK())
	{
		return status;
	}

	return Status();
}

Status DataWriter::BeginObjectArray(const Char *szName)
{
	Status status;

	if (NULL == m_pOutputStream || !m_pOutputStream->IsOK())
	{
		return Status(Status_NotInitialized, "Input stream is not valid");
	}
	if (m_stackStates.empty())
	{
		return Status(Status_InvalidCall, "Out of order call");
	}
	if (State_Object != m_stackStates.top().nState)
	{
		return Status(Status_InvalidCall, "Out of order call");
	}
	if ((status = WriteItemSeparator()).IsNOK())
	{
		return status;
	}
	if ((status = Print(m_pOutputStream, "\"{1}\":", szName)).IsNOK())
	{
		return status;
	}
	if ((status = WriteItemSeparator(true)).IsNOK())
	{
		return status;
	}
	if ((status = Print(m_pOutputStream, "[", szName)).IsNOK())
	{
		return status;
	}
	m_stackStates.top().cCount++;
	m_stackStates.push(StateData(State_Array));

	return Status();
}

Status DataWriter::EndObjectArray()
{
	Status status;

	if (NULL == m_pOutputStream || !m_pOutputStream->IsOK())
	{
		return Status(Status_NotInitialized, "Input stream is not valid");
	}
	if (m_stackStates.empty())
	{
		return Status(Status_InvalidCall, "Out of order call");
	}
	if (State_Array != m_stackStates.top().nState)
	{
		return Status(Status_InvalidCall, "Out of order call");
	}
	m_stackStates.pop();
	if ((status = WriteItemSeparator(true)).IsNOK())
	{
		return status;
	}
	if ((status = Print(m_pOutputStream, "]")).IsNOK())
	{
		return status;
	}

	return Status();
}

Status DataWriter::WriteObjectBool(const Char *szName, Bool bValue)
{
	Status status;

	if (NULL == m_pOutputStream || !m_pOutputStream->IsOK())
	{
		return Status(Status_NotInitialized, "Input stream is not valid");
	}
	if (m_stackStates.empty())
	{
		return Status(Status_InvalidCall, "Out of order call");
	}
	if (State_Object != m_stackStates.top().nState)
	{
		return Status(Status_InvalidCall, "Out of order call");
	}
	if ((status = WriteItemSeparator()).IsNOK())
	{
		return status;
	}
	if ((status = Print(m_pOutputStream, "\"{1}\": {2}", szName, bValue ? "true" : "false")).IsNOK())
	{
		return status;
	}
	m_stackStates.top().cCount++;

	return Status();
}

Status DataWriter::WriteObjectInt(const Char *szName, Int64 nValue)
{
	Status status;

	if (NULL == m_pOutputStream || !m_pOutputStream->IsOK())
	{
		return Status(Status_NotInitialized, "Input stream is not valid");
	}
	if (m_stackStates.empty())
	{
		return Status(Status_InvalidCall, "Out of order call");
	}
	if (State_Object != m_stackStates.top().nState)
	{
		return Status(Status_InvalidCall, "Out of order call");
	}
	if ((status = WriteItemSeparator()).IsNOK())
	{
		return status;
	}
	if ((status = Print(m_pOutputStream, "\"{1}\": {2}", szName, nValue)).IsNOK())
	{
		return status;
	}
	m_stackStates.top().cCount++;

	return Status();
}

Status DataWriter::WriteObjectReal(const Char *szName, Double lfValue)
{
	Status status;

	if (NULL == m_pOutputStream || !m_pOutputStream->IsOK())
	{
		return Status(Status_NotInitialized, "Input stream is not valid");
	}
	if (m_stackStates.empty())
	{
		return Status(Status_InvalidCall, "Out of order call");
	}
	if (State_Object != m_stackStates.top().nState)
	{
		return Status(Status_InvalidCall, "Out of order call");
	}
	if ((status = WriteItemSeparator()).IsNOK())
	{
		return status;
	}
	if ((status = Print(m_pOutputStream, "\"{1}\": {2}", szName, lfValue)).IsNOK())
	{
		return status;
	}
	m_stackStates.top().cCount++;

	return Status();
}

Status DataWriter::WriteObjectString(const Char *szName, const Char *szValue)
{
	Status status;

	if (NULL == m_pOutputStream || !m_pOutputStream->IsOK())
	{
		return Status(Status_NotInitialized, "Input stream is not valid");
	}
	if (m_stackStates.empty())
	{
		return Status(Status_InvalidCall, "Out of order call");
	}
	if (State_Object != m_stackStates.top().nState)
	{
		return Status(Status_InvalidCall, "Out of order call");
	}
	if ((status = WriteItemSeparator()).IsNOK())
	{
		return status;
	}

	String sValue;

	if ((status = SAXParser::EscapeString(szValue, &sValue)).IsNOK())
	{
		return status;
	}

	if ((status = Print(m_pOutputStream, "\"{1}\": \"{2}\"", szName, sValue.c_str())).IsNOK())
	{
		return status;
	}
	m_stackStates.top().cCount++;

	return Status();
}

Status DataWriter::WriteObjectWString(const Char *szName, const WChar *wszValue)
{
	Status status;

	if (NULL == m_pOutputStream || !m_pOutputStream->IsOK())
	{
		return Status(Status_NotInitialized, "Input stream is not valid");
	}
	if (m_stackStates.empty())
	{
		return Status(Status_InvalidCall, "Out of order call");
	}
	if (State_Object != m_stackStates.top().nState)
	{
		return Status(Status_InvalidCall, "Out of order call");
	}
	if ((status = WriteItemSeparator()).IsNOK())
	{
		return status;
	}

	String sTmpValue;

	if ((status = Str::UTF8::FromUTF16(wszValue, &sTmpValue)).IsNOK())
	{
		return status;
	}

	String sValue;

	if ((status = SAXParser::EscapeString(sTmpValue.c_str(), &sValue)).IsNOK())
	{
		return status;
	}

	if ((status = Print(m_pOutputStream, "\"{1}\": \"{2}\"", szName, sValue.c_str())).IsNOK())
	{
		return status;
	}
	m_stackStates.top().cCount++;

	return Status();
}

Status DataWriter::BeginArrayObject()
{
	Status status;

	if (NULL == m_pOutputStream || !m_pOutputStream->IsOK())
	{
		return Status(Status_NotInitialized, "Input stream is not valid");
	}
	if (m_stackStates.empty())
	{
		return Status(Status_InvalidCall, "Out of order call");
	}
	if (State_Array != m_stackStates.top().nState)
	{
		return Status(Status_InvalidCall, "Out of order call");
	}
	if ((status = WriteItemSeparator()).IsNOK())
	{
		return status;
	}
	if ((status = Print(m_pOutputStream, "{{")).IsNOK())
	{
		return status;
	}
	m_stackStates.top().cCount++;
	m_stackStates.push(StateData(State_Object));

	return Status();
}

Status DataWriter::EndArrayObject()
{
	Status status;

	if (NULL == m_pOutputStream || !m_pOutputStream->IsOK())
	{
		return Status(Status_NotInitialized, "Input stream is not valid");
	}
	if (m_stackStates.empty())
	{
		return Status(Status_InvalidCall, "Out of order call");
	}
	if (State_Object != m_stackStates.top().nState)
	{
		return Status(Status_InvalidCall, "Out of order call");
	}
	m_stackStates.pop();
	if ((status = WriteItemSeparator(true)).IsNOK())
	{
		return status;
	}
	if ((status = Print(m_pOutputStream, "}")).IsNOK())
	{
		return status;
	}

	return Status();
}

Status DataWriter::BeginArrayArray()
{
	Status status;

	if (NULL == m_pOutputStream || !m_pOutputStream->IsOK())
	{
		return Status(Status_NotInitialized, "Input stream is not valid");
	}
	if (m_stackStates.empty())
	{
		return Status(Status_InvalidCall, "Out of order call");
	}
	if (State_Array != m_stackStates.top().nState)
	{
		return Status(Status_InvalidCall, "Out of order call");
	}
	if ((status = WriteItemSeparator(true)).IsNOK())
	{
		return status;
	}
	if ((status = Print(m_pOutputStream, "]")).IsNOK())
	{
		return status;
	}
	m_stackStates.pop();

	return Status();
}

Status DataWriter::EndArrayArray()
{
	Status status;

	if (NULL == m_pOutputStream || !m_pOutputStream->IsOK())
	{
		return Status(Status_NotInitialized, "Input stream is not valid");
	}
	if (m_stackStates.empty())
	{
		return Status(Status_InvalidCall, "Out of order call");
	}
	if (State_Array != m_stackStates.top().nState)
	{
		return Status(Status_InvalidCall, "Out of order call");
	}
	m_stackStates.pop();
	if ((status = WriteItemSeparator(true)).IsNOK())
	{
		return status;
	}
	if ((status = Print(m_pOutputStream, "]")).IsNOK())
	{
		return status;
	}

	return Status();
}

Status DataWriter::WriteArrayBool(Bool bValue)
{
	Status status;

	if (NULL == m_pOutputStream || !m_pOutputStream->IsOK())
	{
		return Status(Status_NotInitialized, "Input stream is not valid");
	}
	if (m_stackStates.empty())
	{
		return Status(Status_InvalidCall, "Out of order call");
	}
	if (State_Array != m_stackStates.top().nState)
	{
		return Status(Status_InvalidCall, "Out of order call");
	}
	if ((status = WriteItemSeparator()).IsNOK())
	{
		return status;
	}
	if ((status = Print(m_pOutputStream, "{1}", bValue ? "true" : "false")).IsNOK())
	{
		return status;
	}
	m_stackStates.top().cCount++;

	return Status();
}

Status DataWriter::WriteArrayInt(Int64 nValue)
{
	Status status;

	if (NULL == m_pOutputStream || !m_pOutputStream->IsOK())
	{
		return Status(Status_NotInitialized, "Input stream is not valid");
	}
	if (m_stackStates.empty())
	{
		return Status(Status_InvalidCall, "Out of order call");
	}
	if (m_stackStates.empty())
	{
		return Status(Status_InvalidCall, "Out of order call");
	}
	if ((status = WriteItemSeparator()).IsNOK())
	{
		return status;
	}
	if ((status = Print(m_pOutputStream, "{1}", nValue)).IsNOK())
	{
		return status;
	}
	m_stackStates.top().cCount++;

	return Status();
}

Status DataWriter::WriteArrayReal(Double lfValue)
{
	Status status;

	if (NULL == m_pOutputStream || !m_pOutputStream->IsOK())
	{
		return Status(Status_NotInitialized, "Input stream is not valid");
	}
	if (m_stackStates.empty())
	{
		return Status(Status_InvalidCall, "Out of order call");
	}
	if (State_Array != m_stackStates.top().nState)
	{
		return Status(Status_InvalidCall, "Out of order call");
	}
	if ((status = WriteItemSeparator()).IsNOK())
	{
		return status;
	}
	if ((status = Print(m_pOutputStream, "{1}", lfValue)).IsNOK())
	{
		return status;
	}
	m_stackStates.top().cCount++;

	return Status();
}

Status DataWriter::WriteArrayString(const Char *szValue)
{
	Status status;

	if (NULL == m_pOutputStream || !m_pOutputStream->IsOK())
	{
		return Status(Status_NotInitialized, "Input stream is not valid");
	}
	if (m_stackStates.empty())
	{
		return Status(Status_InvalidCall, "Out of order call");
	}
	if (State_Array != m_stackStates.top().nState)
	{
		return Status(Status_InvalidCall, "Out of order call");
	}
	if ((status = WriteItemSeparator()).IsNOK())
	{
		return status;
	}

	String sValue;

	if ((status = SAXParser::EscapeString(szValue, &sValue)).IsNOK())
	{
		return status;
	}

	if ((status = Print(m_pOutputStream, "\"{1}\"", sValue.c_str())).IsNOK())
	{
		return status;
	}
	m_stackStates.top().cCount++;

	return Status();
}

Status DataWriter::WriteArrayWString(const WChar *wszValue)
{
	Status status;

	if (NULL == m_pOutputStream || !m_pOutputStream->IsOK())
	{
		return Status(Status_NotInitialized, "Input stream is not valid");
	}
	if (m_stackStates.empty())
	{
		return Status(Status_InvalidCall, "Out of order call");
	}
	if (State_Array != m_stackStates.top().nState)
	{
		return Status(Status_InvalidCall, "Out of order call");
	}
	if ((status = WriteItemSeparator()).IsNOK())
	{
		return status;
	}

	String sTmpValue;

	if ((status = Str::UTF8::FromUTF16(wszValue, &sTmpValue)).IsNOK())
	{
		return status;
	}

	String sValue;

	if ((status = SAXParser::EscapeString(sTmpValue.c_str(), &sValue)).IsNOK())
	{
		return status;
	}

	if ((status = Print(m_pOutputStream, "\"{1}\"", sValue.c_str())).IsNOK())
	{
		return status;
	}
	m_stackStates.top().cCount++;


	return Status();
}

}//namespace JSON

}//namespace Data

}//namespace CX

