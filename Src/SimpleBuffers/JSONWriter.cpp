/* 
 * CX - C++ framework for general purpose development
 *
 * https://github.com/draede/cx
 * 
 * Copyright (C) 2014 - 2022 draede
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
#include "CX/SimpleBuffers/JSONWriter.hpp"


namespace CX
{

namespace SimpleBuffers
{

JSONWriter::JSONWriter()
{
	m_pOutputStream = NULL;
	m_cIndent       = 0;
}

JSONWriter::~JSONWriter()
{
}

Status JSONWriter::PrintFieldsSeparator(bool bEnd)
{
	if (0 < m_stackStates.top().cElems && !bEnd)
	{
		return Print(m_pOutputStream, ",\n");
	}
	else
	{
		return Print(m_pOutputStream, "\n");
	}
}

Status JSONWriter::PrintIndent()
{
	Size cIndent = m_cIndent * INDENT_TAB_SIZE;

	if (cIndent > m_sIndent.size())
	{
		String sTab(INDENT_TAB_SIZE, ' ');

		while (cIndent > m_sIndent.size())
		{
			m_sIndent += sTab;
		}
	}

	if (0 == cIndent)
	{
		return Status();
	}

	Size   cbAckSize;
	Status status;

	if ((status = m_pOutputStream->Write(m_sIndent.c_str(), cIndent, &cbAckSize)).IsNOK())
	{
		return status;
	}
	if (cIndent != cbAckSize)
	{
		return Status(Status_WriteFailed);
	}

	return Status();
}

Status JSONWriter::Begin(IO::IOutputStream *pOutputStream)
{
	if (NULL == pOutputStream)
	{
		return Status(Status_InvalidArg, "Invalid output stream");
	}
	m_pOutputStream = pOutputStream;
	while (!m_stackStates.empty())
	{
		m_stackStates.pop();
	}
	m_stackStates.push(StateData(State_Root));
	m_sIndent.clear();
	m_cIndent = 0;

	return Status();
}

Status JSONWriter::End()
{
	m_pOutputStream = NULL;
	while (!m_stackStates.empty())
	{
		m_stackStates.pop();
	}
	m_sIndent.clear();
	m_cIndent = 0;

	return Status();
}

Status JSONWriter::WriteBool(Bool v, const Char *szName/* = NULL*/)
{
	return Write(&JSONWriter::WriteFunc<Bool>, v, szName);
}

Status JSONWriter::WriteInt8(Int8 v, const Char *szName/* = NULL*/)
{
	return Write(&JSONWriter::WriteFunc<Int8>, v, szName);
}

Status JSONWriter::WriteUInt8(UInt8 v, const Char *szName/* = NULL*/)
{
	return Write(&JSONWriter::WriteFunc<UInt8>, v, szName);
}

Status JSONWriter::WriteInt16(Int16 v, const Char *szName/* = NULL*/)
{
	return Write(&JSONWriter::WriteFunc<Int16>, v, szName);
}

Status JSONWriter::WriteUInt16(UInt16 v, const Char *szName/* = NULL*/)
{
	return Write(&JSONWriter::WriteFunc<UInt16>, v, szName);
}

Status JSONWriter::WriteInt32(Int32 v, const Char *szName/* = NULL*/)
{
	return Write(&JSONWriter::WriteFunc<Int32>, v, szName);
}

Status JSONWriter::WriteUInt32(UInt32 v, const Char *szName/* = NULL*/)
{
	return Write(&JSONWriter::WriteFunc<UInt32>, v, szName);
}

Status JSONWriter::WriteInt64(Int64 v, const Char *szName/* = NULL*/)
{
	return Write(&JSONWriter::WriteFunc<Int64>, v, szName);
}

Status JSONWriter::WriteUInt64(UInt64 v, const Char *szName/* = NULL*/)
{
	return Write(&JSONWriter::WriteFunc<UInt64>, v, szName);
}

Status JSONWriter::WriteFloat(Float v, const Char *szName/* = NULL*/)
{
	return Write(&JSONWriter::WriteFunc<Float>, v, szName);
}

Status JSONWriter::WriteDouble(Double v, const Char *szName/* = NULL*/)
{
	return Write(&JSONWriter::WriteFunc<Double>, v, szName);
}

Status JSONWriter::WriteString(const String &v, const Char *szName/* = NULL*/)
{
	return Write(&JSONWriter::WriteFunc<String>, v, szName);
}

Status JSONWriter::WriteBLOB(const void *pData, Size cbSize, const Char *szName/* = NULL*/)
{
	Status         status;

	if ((status = PrintFieldsSeparator(false)).IsNOK())
	{
		return status;
	}
	if ((status = PrintIndent()).IsNOK())
	{
		return status;
	}

	if (0 == cbSize)
	{
		if (NULL != szName)
		{
			return Print(m_pOutputStream, "\"{1}\": \"\"", szName);
		}
		else
		{
			return Print(m_pOutputStream, "\"\"");
		}
	}

	Str::Z85BinStr z85;
	Size           cLen;
	Char           *szBuffer;

	cLen = z85.GetStrLenFromBinSize(pData, cbSize);
	szBuffer = NULL;
	for (;;)
	{
		if (NULL == (szBuffer = (Char *)Mem::Alloc(cLen + 1)))
		{
			status = Status(Status_MemAllocFailed, "Failed to allocate temp string");

			break;
		}
		if ((status = z85.ToString(pData, cbSize, szBuffer, cLen)).IsNOK())
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
			status = Print(m_pOutputStream, "\"{1}\": \"{2}\"", szName, szBuffer);
		}
		else
		{
			status = Print(m_pOutputStream, "\"{1}\"", szBuffer);
		}
	}
	if (NULL != szBuffer)
	{
		Mem::Free(szBuffer);
	}

	return status;
}

Status JSONWriter::WriteCustom(ICustom::Type nType, const void *pData, const Char *szName/* = NULL*/)
{
	CX_UNUSED(nType);
	CX_UNUSED(pData);
	CX_UNUSED(szName);

	return Status_NotSupported;
}

Status JSONWriter::BeginObject(const Char *szName/* = NULL*/)
{
	if (m_stackStates.empty())
	{
		return Status(Status_InvalidCall, "Begin was not called");
	}

	Status status;

	if (State_Root == m_stackStates.top().nState)
	{
		if (NULL != szName)
		{
			return Status(Status_InvalidCall, "Root object must have no name");
		}
		if (0 < m_stackStates.top().cElems)
		{
			return Status(Status_InvalidCall, "Only one root object allowed");
		}
		if ((status = Print(m_pOutputStream, "{{", szName)).IsNOK())
		{
			return status;
		}
	}
	else
	if (State_Object == m_stackStates.top().nState)
	{
		if (NULL == szName)
		{
			return Status(Status_InvalidCall, "Member must have a name");
		}
		if ((status = PrintFieldsSeparator(false)).IsNOK())
		{
			return status;
		}
		if ((status = PrintIndent()).IsNOK())
		{
			return status;
		}
		if ((status = Print(m_pOutputStream, "\"{1}\":\n", szName)).IsNOK())
		{
			return status;
		}
		if ((status = PrintIndent()).IsNOK())
		{
			return status;
		}
		if ((status = Print(m_pOutputStream, "{{", szName)).IsNOK())
		{
			return status;
		}
	}
	else
	if (State_Array == m_stackStates.top().nState)
	{
		if (NULL != szName)
		{
			return Status(Status_InvalidCall, "Item must not have a name");
		}
		if ((status = PrintFieldsSeparator(false)).IsNOK())
		{
			return status;
		}
		if ((status = PrintIndent()).IsNOK())
		{
			return status;
		}
		if ((status = Print(m_pOutputStream, "{{")).IsNOK())
		{
			return status;
		}
	}
	else
	{
		return Status(Status_InvalidCall, "Objects must be placed as root or inside objects or arrays");
	}
	m_stackStates.push(StateData(State_Object));
	m_cIndent++;

	return Status();
}

Status JSONWriter::EndObject()
{
	if (m_stackStates.empty())
	{
		return Status(Status_InvalidCall, "Begin was not called");
	}

	Status status;

	if (State_Object != m_stackStates.top().nState)
	{
		return Status(Status_InvalidCall, "Must be in an object");
	}
	m_stackStates.pop();
	m_cIndent--;
	if ((status = PrintFieldsSeparator(true)).IsNOK())
	{
		return status;
	}
	if ((status = PrintIndent()).IsNOK())
	{
		return status;
	}
	if ((status = Print(m_pOutputStream, "}")).IsNOK())
	{
		return status;
	}
	if (m_stackStates.empty())
	{
		return Status(Status_InvalidCall, "Begin was not called");
	}
	m_stackStates.top().cElems++;

	return Status();
}

Status JSONWriter::BeginArray(const Char *szName/* = NULL*/)
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
		if ((status = PrintFieldsSeparator(false)).IsNOK())
		{
			return status;
		}
		if ((status = PrintIndent()).IsNOK())
		{
			return status;
		}
		if ((status = Print(m_pOutputStream, "\"{1}\":\n", szName)).IsNOK())
		{
			return status;
		}
		if ((status = PrintIndent()).IsNOK())
		{
			return status;
		}
		if ((status = Print(m_pOutputStream, "[", szName)).IsNOK())
		{
			return status;
		}
	}
	else
	if (State_Array == m_stackStates.top().nState)
	{
		if (NULL != szName)
		{
			return Status(Status_InvalidCall, "Item must not have a name");
		}
		if ((status = PrintFieldsSeparator(false)).IsNOK())
		{
			return status;
		}
		if ((status = PrintIndent()).IsNOK())
		{
			return status;
		}
		if ((status = Print(m_pOutputStream, "[")).IsNOK())
		{
			return status;
		}
	}
	else
	{
		return Status(Status_InvalidCall, "Arrays must be placed inside objects or arrays");
	}
	m_stackStates.push(StateData(State_Array));
	m_cIndent++;

	return Status();
}

Status JSONWriter::EndArray()
{
	if (m_stackStates.empty())
	{
		return Status(Status_InvalidCall, "Begin was not called");
	}

	Status status;

	if (State_Array != m_stackStates.top().nState)
	{
		return Status(Status_InvalidCall, "Must be in an array");
	}
	m_stackStates.pop();
	m_cIndent--;
	if ((status = PrintFieldsSeparator(true)).IsNOK())
	{
		return status;
	}
	if ((status = PrintIndent()).IsNOK())
	{
		return status;
	}
	if ((status = Print(m_pOutputStream, "]")).IsNOK())
	{
		return status;
	}
	if (m_stackStates.empty())
	{
		return Status(Status_InvalidCall, "Begin was not called");
	}
	m_stackStates.top().cElems++;

	return Status();
}

}//namespace SimpleBuffers

}//namespace CX
