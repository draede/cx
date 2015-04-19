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

#include "CX/SB/TextDataWriter.hpp"
#include "CX/Data/JSON/SAXParser.hpp"
#include "CX/Print.hpp"


namespace CX
{

namespace SB
{

TextDataWriter::TextDataWriter(IO::IOutputStream *pOutputStream)
{
	m_pOutputStream = pOutputStream;
	m_cIndent       = 0;
}

TextDataWriter::~TextDataWriter()
{
}

Status TextDataWriter::WriteIndent()
{
	Size   cbAckSize;
	Status status;

	for (Size i = m_sIndent.size(); i < m_cIndent; i++)
	{
		m_sIndent += "\t";
	}
	if ((status = m_pOutputStream->Write(m_sIndent.c_str(), m_cIndent, &cbAckSize)).IsNOK())
	{
		return status;
	}
	if (cbAckSize != m_cIndent)
	{
		return Status(Status_WriteFailed, "Failed to write {1} bytes", m_cIndent);
	}

	return Status();
}

Status TextDataWriter::BeginMember(const Char *szName)
{
	Status status;

	if (NULL == m_pOutputStream || !m_pOutputStream->IsOK())
	{
		return Status(Status_NotInitialized, "Invalid output stream");
	}
	if (m_stackStates.empty())
	{
		return Status(Status_InvalidCall, "Out of order");
	}
	if (State_Object != m_stackStates.top().nState)
	{
		return Status(Status_InvalidCall, "Out of order");
	}
	m_stackStates.top().cIndex++;
	if (m_stackStates.top().cIndex > m_stackStates.top().cCount)
	{
		return Status(Status_InvalidCall, "Too many members");
	}
	if ((status = WriteIndent()).IsNOK())
	{
		return status;
	}
	if ((status = Print(m_pOutputStream, "\"{1}\": ", szName)).IsNOK())
	{
		return status;
	}
	m_stackStates.push(StateData(State_Member));

	return Status();
}

Status TextDataWriter::EndMember()
{
	Status status;

	if (NULL == m_pOutputStream || !m_pOutputStream->IsOK())
	{
		return Status(Status_NotInitialized, "Invalid output stream");
	}
	if (m_stackStates.empty())
	{
		return Status(Status_InvalidCall, "Out of order");
	}
	if (State_Member != m_stackStates.top().nState)
	{
		return Status(Status_InvalidCall, "Out of order");
	}
	m_stackStates.pop();
	if (m_stackStates.empty())
	{
		return Status(Status_InvalidCall, "Out of order");
	}
	if (State_Object != m_stackStates.top().nState)
	{
		return Status(Status_InvalidCall, "Out of order");
	}
	if (m_stackStates.top().cIndex < m_stackStates.top().cCount)
	{
		if ((status = Print(m_pOutputStream, ",\n")).IsNOK())
		{
			return status;
		}
	}
	else
	{
		if ((status = Print(m_pOutputStream, "\n")).IsNOK())
		{
			return status;
		}
	}

	return Status();
}

Status TextDataWriter::BeginItem()
{
	Status status;

	if (NULL == m_pOutputStream || !m_pOutputStream->IsOK())
	{
		return Status(Status_NotInitialized, "Invalid output stream");
	}
	if (m_stackStates.empty())
	{
		return Status(Status_InvalidCall, "Out of order");
	}
	if (State_Array != m_stackStates.top().nState)
	{
		return Status(Status_InvalidCall, "Out of order");
	}
	m_stackStates.top().cIndex++;
	if (m_stackStates.top().cIndex > m_stackStates.top().cCount)
	{
		return Status(Status_InvalidCall, "Too many items");
	}
	if ((status = WriteIndent()).IsNOK())
	{
		return status;
	}
	m_stackStates.push(StateData(State_Item));

	return Status();
}

Status TextDataWriter::EndItem()
{
	Status status;

	if (NULL == m_pOutputStream || !m_pOutputStream->IsOK())
	{
		return Status(Status_NotInitialized, "Invalid output stream");
	}
	if (m_stackStates.empty())
	{
		return Status(Status_InvalidCall, "Out of order");
	}
	if (State_Item != m_stackStates.top().nState)
	{
		return Status(Status_InvalidCall, "Out of order");
	}
	m_stackStates.pop();
	if (m_stackStates.empty())
	{
		return Status(Status_InvalidCall, "Out of order");
	}
	if (State_Array!= m_stackStates.top().nState)
	{
		return Status(Status_InvalidCall, "Out of order");
	}
	if (m_stackStates.top().cIndex < m_stackStates.top().cCount)
	{
		if ((status = Print(m_pOutputStream, ",\n")).IsNOK())
		{
			return status;
		}
	}
	else
	{
		if ((status = Print(m_pOutputStream, "\n")).IsNOK())
		{
			return status;
		}
	}

	return Status();
}

Status TextDataWriter::BeginObject(Size cCount)
{
	Status status;

	if (NULL == m_pOutputStream || !m_pOutputStream->IsOK())
	{
		return Status(Status_NotInitialized, "Invalid output stream");
	}
	if (!m_stackStates.empty())
	{
		if ((status = Print(m_pOutputStream, "\n")).IsNOK())
		{
			return status;
		}
	}
	if ((status = WriteIndent()).IsNOK())
	{
		return status;
	}
	if ((status = Print(m_pOutputStream, "{{\n")).IsNOK())
	{
		return status;
	}
	m_stackStates.push(StateData(State_Object, cCount));
	m_cIndent++;

	return Status();
}

Status TextDataWriter::EndObject()
{
	Status status;

	if (NULL == m_pOutputStream || !m_pOutputStream->IsOK())
	{
		return Status(Status_NotInitialized, "Invalid output stream");
	}
	if (m_stackStates.empty())
	{
		return Status(Status_InvalidCall, "Out of order");
	}
	if (State_Object != m_stackStates.top().nState)
	{
		return Status(Status_InvalidCall, "Out of order");
	}
	if (m_stackStates.top().cIndex < m_stackStates.top().cCount)
	{
		return Status(Status_InvalidCall, "More members are expected");
	}
	m_stackStates.pop();
	m_cIndent--;
	if ((status = WriteIndent()).IsNOK())
	{
		return status;
	}
	if ((status = Print(m_pOutputStream, "}")).IsNOK())
	{
		return status;
	}

	return Status();
}

Status TextDataWriter::BeginArray(Size cCount)
{
	Status status;

	if (NULL == m_pOutputStream || !m_pOutputStream->IsOK())
	{
		return Status(Status_NotInitialized, "Invalid output stream");
	}
	if (!m_stackStates.empty())
	{
		if ((status = Print(m_pOutputStream, "\n")).IsNOK())
		{
			return status;
		}
	}
	if ((status = WriteIndent()).IsNOK())
	{
		return status;
	}
	if ((status = Print(m_pOutputStream, "[\n")).IsNOK())
	{
		return status;
	}
	m_stackStates.push(StateData(State_Array, cCount));
	m_cIndent++;

	return Status();
}

Status TextDataWriter::EndArray()
{
	Status status;

	if (NULL == m_pOutputStream || !m_pOutputStream->IsOK())
	{
		return Status(Status_NotInitialized, "Invalid output stream");
	}
	if (m_stackStates.empty())
	{
		return Status(Status_InvalidCall, "Out of order");
	}
	if (State_Array != m_stackStates.top().nState)
	{
		return Status(Status_InvalidCall, "Out of order");
	}
	if (m_stackStates.top().cIndex < m_stackStates.top().cCount)
	{
		return Status(Status_InvalidCall, "More members are expected");
	}
	m_stackStates.pop();
	m_cIndent--;
	if ((status = WriteIndent()).IsNOK())
	{
		return status;
	}
	if ((status = Print(m_pOutputStream, "]")).IsNOK())
	{
		return status;
	}

	return Status();
}

Status TextDataWriter::WriteBool(Bool bValue)
{
	return WriteValue(bValue);
}

Status TextDataWriter::WriteInt8(Int8 nValue)
{
	return WriteValue(nValue);
}

Status TextDataWriter::WriteUInt8(UInt8 uValue)
{
	return WriteValue(uValue);
}

Status TextDataWriter::WriteInt16(Int16 nValue)
{
	return WriteValue(nValue);
}

Status TextDataWriter::WriteUInt16(UInt16 uValue)
{
	return WriteValue(uValue);
}

Status TextDataWriter::WriteInt32(Int32 nValue)
{
	return WriteValue(nValue);
}

Status TextDataWriter::WriteUInt32(UInt32 uValue)
{
	return WriteValue(uValue);
}

Status TextDataWriter::WriteInt64(Int64 nValue)
{
	return WriteValue(nValue);
}

Status TextDataWriter::WriteUInt64(UInt64 uValue)
{
	return WriteValue(uValue);
}

Status TextDataWriter::WriteFloat(Float fValue)
{
	return WriteValue(fValue);
}

Status TextDataWriter::WriteDouble(Double lfValue)
{
	return WriteValue(lfValue);
}

Status TextDataWriter::WriteString(const Char *szValue)
{
	if (NULL == m_pOutputStream || !m_pOutputStream->IsOK())
	{
		return Status(Status_NotInitialized, "Invalid output stream");
	}
	if (m_stackStates.empty())
	{
		return Status(Status_InvalidCall, "Out of order");
	}
	if (State_Member != m_stackStates.top().nState && State_Item != m_stackStates.top().nState)
	{
		return Status(Status_InvalidCall, "Out of order");
	}

	String sEscaped;

	Data::JSON::SAXParser::EscapeString(szValue, &sEscaped);

	return Print(m_pOutputStream, "\"{1}\"", sEscaped.c_str());
}

Status TextDataWriter::WriteWString(const WChar *wszValue)
{
	if (NULL == m_pOutputStream || !m_pOutputStream->IsOK())
	{
		return Status(Status_NotInitialized, "Invalid output stream");
	}
	if (m_stackStates.empty())
	{
		return Status(Status_InvalidCall, "Out of order");
	}
	if (State_Member != m_stackStates.top().nState && State_Item != m_stackStates.top().nState)
	{
		return Status(Status_InvalidCall, "Out of order");
	}

	String sString;
	String sEscaped;

	Print(&sString, "{1}", wszValue);

	Data::JSON::SAXParser::EscapeString(sString.c_str(), &sEscaped);

	return Print(m_pOutputStream, "\"{1}\"", sEscaped);
}

}//namespace SB

}//namespace CX

