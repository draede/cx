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
#include "CX/Str/Z85BinStr.hpp"
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
	m_stackStates.push(State_None);
	m_cIndent       = 0;
	m_bFirst        = True;
}

DataWriter::~DataWriter()
{
}

void DataWriter::AdjustIndent()
{
	Size cActLen   = m_sIndent.size();
	Size cFinalLen = m_cIndent * 3;

	if (cActLen < cFinalLen)
	{
		for (Size i = cActLen; i < cFinalLen; i++)
		{
			m_sIndent += " ";
		}
	}
	else
	if (cActLen > cFinalLen)
	{
		m_sIndent = m_sIndent.substr(0, cFinalLen);
	}
}

Status DataWriter::BeginRootObject()
{
	Status status;

	if (NULL == m_pOutputStream)
	{
		return Status(Status_NotInitialized, "No valid output stream");
	}
	if (m_stackStates.empty() || State_None != m_stackStates.top())
	{
		return Status(Status_InvalidCall, "Out of order call (must be called at the beginning)");
	}
	if ((status = Print(m_pOutputStream, "{{")).IsNOK())
	{
		return status;
	}
	m_stackStates.push(State_RootObject);
	m_bFirst = True;
	m_cIndent++;
	AdjustIndent();

	return Status();
}

Status DataWriter::EndRootObject()
{
	Status status;

	if (NULL == m_pOutputStream)
	{
		return Status(Status_NotInitialized, "No valid output stream");
	}
	if (m_stackStates.empty() || State_RootObject != m_stackStates.top())
	{
		return Status(Status_InvalidCall, "Out of order call (must be in the root object)");
	}
	m_cIndent--;
	AdjustIndent();
	if ((status = Print(m_pOutputStream, "\n}")).IsNOK())
	{
		return status;
	}
	m_stackStates.pop();
	m_bFirst = false;

	return Status();
}

Status DataWriter::BeginRootArray()
{
	Status status;

	if (NULL == m_pOutputStream)
	{
		return Status(Status_NotInitialized, "No valid output stream");
	}
	if (m_stackStates.empty() || State_None != m_stackStates.top())
	{
		return Status(Status_InvalidCall, "Out of order call (must be called at the beginning)");
	}
	if ((status = Print(m_pOutputStream, "[")).IsNOK())
	{
		return status;
	}
	m_stackStates.push(State_RootArray);
	m_bFirst = True;
	m_cIndent++;
	AdjustIndent();

	return Status();
}

Status DataWriter::EndRootArray()
{
	Status status;

	if (NULL == m_pOutputStream)
	{
		return Status(Status_NotInitialized, "No valid output stream");
	}
	if (m_stackStates.empty() || State_RootArray != m_stackStates.top())
	{
		return Status(Status_InvalidCall, "Out of order call (must be in the root array)");
	}
	m_cIndent--;
	AdjustIndent();
	if ((status = Print(m_pOutputStream, "\n]")).IsNOK())
	{
		return status;
	}
	m_stackStates.pop();
	m_bFirst = false;

	return Status();
}

//object member
Status DataWriter::WriteNull(const Char *szName)
{
	String sName;
	Status status;

	if (NULL == m_pOutputStream)
	{
		return Status(Status_NotInitialized, "No valid output stream");
	}
	if (m_stackStates.empty() || 
	    (State_Object != m_stackStates.top() && State_RootObject != m_stackStates.top()))
	{
		return Status(Status_InvalidCall, "Out of order call (must be called from an object)");
	}
	if ((status = SAXParser::EscapeString(szName, &sName)).IsNOK())
	{
		return status;
	}
	if (m_bFirst)
	{
		if ((status = Print(m_pOutputStream, "\n{1}\"{2}\": null", m_sIndent, sName)).IsNOK())
		{
			return status;
		}
	}
	else
	{
		if ((status = Print(m_pOutputStream, ",\n{1}\"{2}\": null", m_sIndent, sName)).IsNOK())
		{
			return status;
		}
	}
	m_bFirst = false;

	return Status();
}

//array item
Status DataWriter::WriteNull()
{
	Status status;

	if (NULL == m_pOutputStream)
	{
		return Status(Status_NotInitialized, "No valid output stream");
	}
	if (m_stackStates.empty() ||
		(State_Array != m_stackStates.top() && State_RootArray != m_stackStates.top()))
	{
		return Status(Status_InvalidCall, "Out of order call (must be called from an array)");
	}
	if (m_bFirst)
	{
		if ((status = Print(m_pOutputStream, "\n{1}null", m_sIndent)).IsNOK())
		{
			return status;
		}
	}
	else
	{
		if ((status = Print(m_pOutputStream, ",\n{1}null", m_sIndent)).IsNOK())
		{
			return status;
		}
	}
	m_bFirst = false;

	return Status();
}

//object member
Status DataWriter::WriteBool(const Char *szName, Bool bValue)
{
	String sName;
	Status status;

	if (NULL == m_pOutputStream)
	{
		return Status(Status_NotInitialized, "No valid output stream");
	}
	if (m_stackStates.empty() || 
	    (State_Object != m_stackStates.top() && State_RootObject != m_stackStates.top()))
	{
		return Status(Status_InvalidCall, "Out of order call (must be called from an object)");
	}
	if ((status = SAXParser::EscapeString(szName, &sName)).IsNOK())
	{
		return status;
	}
	if (m_bFirst)
	{
		if ((status = 
		     Print(m_pOutputStream, "\n{1}\"{2}\": {3}", m_sIndent, sName, bValue)).IsNOK())
		{
			return status;
		}
	}
	else
	{
		if ((status = 
		     Print(m_pOutputStream, ",\n{1}\"{2}\": {3}", m_sIndent, sName, bValue)).IsNOK())
		{
			return status;
		}
	}
	m_bFirst = false;

	return Status();
}

//array item
Status DataWriter::WriteBool(Bool bValue)
{
	Status status;

	if (NULL == m_pOutputStream)
	{
		return Status(Status_NotInitialized, "No valid output stream");
	}
	if (m_stackStates.empty() ||
		(State_Array != m_stackStates.top() && State_RootArray != m_stackStates.top()))
	{
		return Status(Status_InvalidCall, "Out of order call (must be called from an array)");
	}
	if (m_bFirst)
	{
		if ((status = Print(m_pOutputStream, "\n{1}{2}", m_sIndent, bValue)).IsNOK())
		{
			return status;
		}
	}
	else
	{
		if ((status = Print(m_pOutputStream, ",\n{1}{2}", m_sIndent, bValue)).IsNOK())
		{
			return status;
		}
	}
	m_bFirst = false;

	return Status();
}

//object member
Status DataWriter::WriteInt(const Char *szName, Int64 nValue)
{
	String sName;
	Status status;

	if (NULL == m_pOutputStream)
	{
		return Status(Status_NotInitialized, "No valid output stream");
	}
	if (m_stackStates.empty() || 
	    (State_Object != m_stackStates.top() && State_RootObject != m_stackStates.top()))
	{
		return Status(Status_InvalidCall, "Out of order call (must be called from an object)");
	}
	if ((status = SAXParser::EscapeString(szName, &sName)).IsNOK())
	{
		return status;
	}
	if (m_bFirst)
	{
		if ((status = 
		     Print(m_pOutputStream, "\n{1}\"{2}\": {3}", m_sIndent, sName, nValue)).IsNOK())
		{
			return status;
		}
	}
	else
	{
		if ((status = 
		     Print(m_pOutputStream, ",\n{1}\"{2}\": {3}", m_sIndent, sName, nValue)).IsNOK())
		{
			return status;
		}
	}
	m_bFirst = false;

	return Status();
}

//array item
Status DataWriter::WriteInt(Int64 nValue)
{
	Status status;

	if (NULL == m_pOutputStream)
	{
		return Status(Status_NotInitialized, "No valid output stream");
	}
	if (m_stackStates.empty() ||
		(State_Array != m_stackStates.top() && State_RootArray != m_stackStates.top()))
	{
		return Status(Status_InvalidCall, "Out of order call (must be called from an array)");
	}
	if (m_bFirst)
	{
		if ((status = Print(m_pOutputStream, "\n{1}{2}", m_sIndent, nValue)).IsNOK())
		{
			return status;
		}
	}
	else
	{
		if ((status = Print(m_pOutputStream, ",\n{1}{2}", m_sIndent, nValue)).IsNOK())
		{
			return status;
		}
	}
	m_bFirst = false;

	return Status();
}

//object member
Status DataWriter::WriteReal(const Char *szName, Double lfValue)
{
	String sName;
	Status status;

	if (NULL == m_pOutputStream)
	{
		return Status(Status_NotInitialized, "No valid output stream");
	}
	if (m_stackStates.empty() || 
	    (State_Object != m_stackStates.top() && State_RootObject != m_stackStates.top()))
	{
		return Status(Status_InvalidCall, "Out of order call (must be called from an object)");
	}
	if ((status = SAXParser::EscapeString(szName, &sName)).IsNOK())
	{
		return status;
	}
	if (m_bFirst)
	{
		if ((status = 
		     Print(m_pOutputStream, "\n{1}\"{2}\": {3}", m_sIndent, sName, lfValue)).IsNOK())
		{
			return status;
		}
	}
	else
	{
		if ((status = 
		     Print(m_pOutputStream, ",\n{1}\"{2}\": {3}", m_sIndent, sName, lfValue)).IsNOK())
		{
			return status;
		}
	}
	m_bFirst = false;

	return Status();
}

//array item
Status DataWriter::WriteReal(Double lfValue)
{
	Status status;

	if (NULL == m_pOutputStream)
	{
		return Status(Status_NotInitialized, "No valid output stream");
	}
	if (m_stackStates.empty() ||
		(State_Array != m_stackStates.top() && State_RootArray != m_stackStates.top()))
	{
		return Status(Status_InvalidCall, "Out of order call (must be called from an array)");
	}
	if (m_bFirst)
	{
		if ((status = Print(m_pOutputStream, "\n{1}{2}", m_sIndent, lfValue)).IsNOK())
		{
			return status;
		}
	}
	else
	{
		if ((status = Print(m_pOutputStream, ",\n{1}{2}", m_sIndent, lfValue)).IsNOK())
		{
			return status;
		}
	}
	m_bFirst = false;

	return Status();
}

//object member
Status DataWriter::WriteString(const Char *szName, const Char *szValue)
{
	String sName;
	String sValue;
	Status status;

	if (NULL == m_pOutputStream)
	{
		return Status(Status_NotInitialized, "No valid output stream");
	}
	if (m_stackStates.empty() || 
	    (State_Object != m_stackStates.top() && State_RootObject != m_stackStates.top()))
	{
		return Status(Status_InvalidCall, "Out of order call (must be called from an object)");
	}
	if ((status = SAXParser::EscapeString(szName, &sName)).IsNOK())
	{
		return status;
	}
	if ((status = SAXParser::EscapeString(szValue, &sValue)).IsNOK())
	{
		return status;
	}
	if (m_bFirst)
	{
		if ((status = 
		     Print(m_pOutputStream, "\n{1}\"{2}\": \"{3}\"", m_sIndent, sName, sValue)).IsNOK())
		{
			return status;
		}
	}
	else
	{
		if ((status = 
		     Print(m_pOutputStream, ",\n{1}\"{2}\": \"{3}\"", m_sIndent, sName, sValue)).IsNOK())
		{
			return status;
		}
	}
	m_bFirst = false;

	return Status();
}

//array item
Status DataWriter::WriteString(const Char *szValue)
{
	String sValue;
	Status status;

	if (NULL == m_pOutputStream)
	{
		return Status(Status_NotInitialized, "No valid output stream");
	}
	if (m_stackStates.empty() ||
		(State_Array != m_stackStates.top() && State_RootArray != m_stackStates.top()))
	{
		return Status(Status_InvalidCall, "Out of order call (must be called from an array)");
	}
	if ((status = SAXParser::EscapeString(szValue, &sValue)).IsNOK())
	{
		return status;
	}
	if (m_bFirst)
	{
		if ((status = Print(m_pOutputStream, "\n{1}\"{2}\"", m_sIndent, sValue)).IsNOK())
		{
			return status;
		}
	}
	else
	{
		if ((status = Print(m_pOutputStream, ",\n{1}\"{2}\"", m_sIndent, sValue)).IsNOK())
		{
			return status;
		}
	}
	m_bFirst = false;

	return Status();
}

//object member
Status DataWriter::WriteWString(const Char *szName, const WChar *wszValue)
{
	String sName;
	String sTmpValue;
	String sValue;
	Status status;

	if (NULL == m_pOutputStream)
	{
		return Status(Status_NotInitialized, "No valid output stream");
	}
	if (m_stackStates.empty() ||
		(State_Object != m_stackStates.top() && State_RootObject != m_stackStates.top()))
	{
		return Status(Status_InvalidCall, "Out of order call (must be called from an object)");
	}
	if ((status = SAXParser::EscapeString(szName, &sName)).IsNOK())
	{
		return status;
	}
	if ((status = Str::UTF8::FromUTF16(wszValue, &sTmpValue)).IsNOK())
	{
		return status;
	}
	if ((status = SAXParser::EscapeString(sTmpValue.c_str(), &sValue)).IsNOK())
	{
		return status;
	}
	if (m_bFirst)
	{
		if ((status =
			Print(m_pOutputStream, "\n{1}\"{2}\": \"wstr://{3}\"", m_sIndent, sName, sValue)).IsNOK())
		{
			return status;
		}
	}
	else
	{
		if ((status =
			Print(m_pOutputStream, ",\n{1}\"{2}\": \"wstr://{3}\"", m_sIndent, sName, sValue)).IsNOK())
		{
			return status;
		}
	}
	m_bFirst = false;

	return Status();
}

//array item
Status DataWriter::WriteWString(const WChar *wszValue)
{
	String sTmpValue;
	String sValue;
	Status status;

	if (NULL == m_pOutputStream)
	{
		return Status(Status_NotInitialized, "No valid output stream");
	}
	if (m_stackStates.empty() ||
		(State_Array != m_stackStates.top() && State_RootArray != m_stackStates.top()))
	{
		return Status(Status_InvalidCall, "Out of order call (must be called from an array)");
	}
	if ((status = Str::UTF8::FromUTF16(wszValue, &sTmpValue)).IsNOK())
	{
		return status;
	}
	if ((status = SAXParser::EscapeString(sTmpValue.c_str(), &sValue)).IsNOK())
	{
		return status;
	}
	if (m_bFirst)
	{
		if ((status = Print(m_pOutputStream, "\n{1}\"wstr://{2}\"", m_sIndent, sValue)).IsNOK())
		{
			return status;
		}
	}
	else
	{
		if ((status = Print(m_pOutputStream, ",\n{1}\"wstr://{2}\"", m_sIndent, sValue)).IsNOK())
		{
			return status;
		}
	}
	m_bFirst = false;

	return Status();
}

//object member
Status DataWriter::WriteBLOB(const Char *szName, const void *pData, Size cbSize)
{
	String sName;
	String sValue;
	Status status;

	if (NULL == m_pOutputStream)
	{
		return Status(Status_NotInitialized, "No valid output stream");
	}
	if (m_stackStates.empty() ||
		(State_Object != m_stackStates.top() && State_RootObject != m_stackStates.top()))
	{
		return Status(Status_InvalidCall, "Out of order call (must be called from an object)");
	}
	if ((status = SAXParser::EscapeString(szName, &sName)).IsNOK())
	{
		return status;
	}

	Str::Z85BinStr binstr;

	if ((status = binstr.ToStringEx(pData, cbSize, &sValue)).IsNOK())
	{
		return status;
	}

	if (m_bFirst)
	{
		if ((status =
			Print(m_pOutputStream, "\n{1}\"{2}\": \"blob://{3}\"", m_sIndent, sName, sValue)).IsNOK())
		{
			return status;
		}
	}
	else
	{
		if ((status =
			Print(m_pOutputStream, ",\n{1}\"{2}\": \"blob://{3}\"", m_sIndent, sName, sValue)).IsNOK())
		{
			return status;
		}
	}
	m_bFirst = false;

	return Status();
}

//array item
Status DataWriter::WriteBLOB(const void *pData, Size cbSize)
{
	String sValue;
	Status status;

	if (NULL == m_pOutputStream)
	{
		return Status(Status_NotInitialized, "No valid output stream");
	}
	if (m_stackStates.empty() ||
		(State_Array != m_stackStates.top() && State_RootArray != m_stackStates.top()))
	{
		return Status(Status_InvalidCall, "Out of order call (must be called from an array)");
	}

	Str::Z85BinStr binstr;

	if ((status = binstr.ToStringEx(pData, cbSize, &sValue)).IsNOK())
	{
		return status;
	}

	if (m_bFirst)
	{
		if ((status = Print(m_pOutputStream, "\n{1}\"blob://{2}\"", m_sIndent, sValue)).IsNOK())
		{
			return status;
		}
	}
	else
	{
		if ((status = Print(m_pOutputStream, ",\n{1}\"blob://{2}\"", m_sIndent, sValue)).IsNOK())
		{
			return status;
		}
	}
	m_bFirst = false;

	return Status();
}

//object member
Status DataWriter::BeginObject(const Char *szName)
{
	String sName;
	Status status;

	if (NULL == m_pOutputStream)
	{
		return Status(Status_NotInitialized, "No valid output stream");
	}
	if (m_stackStates.empty() || 
	    (State_Object != m_stackStates.top() && State_RootObject != m_stackStates.top()))
	{
		return Status(Status_InvalidCall, "Out of order call (must be called from an object)");
	}
	if ((status = SAXParser::EscapeString(szName, &sName)).IsNOK())
	{
		return status;
	}
	if (m_bFirst)
	{
		if ((status =
		     Print(m_pOutputStream, "\n{1}\"{2}\":\n{3}{{", m_sIndent, sName, m_sIndent)).IsNOK())
		{
			return status;
		}
	}
	else
	{
		if ((status =
		     Print(m_pOutputStream, ",\n{1}\"{2}\":\n{3}{{", m_sIndent, sName, m_sIndent)).IsNOK())
		{
			return status;
		}
	}
	m_stackStates.push(State_Object);
	m_bFirst = True;
	m_cIndent++;
	AdjustIndent();

	return Status();
}

//array item
Status DataWriter::BeginObject()
{
	Status status;

	if (NULL == m_pOutputStream)
	{
		return Status(Status_NotInitialized, "No valid output stream");
	}
	if (m_stackStates.empty() ||
		(State_Array != m_stackStates.top() && State_RootArray != m_stackStates.top()))
	{
		return Status(Status_InvalidCall, "Out of order call (must be called from an array)");
	}
	if (m_bFirst)
	{
		if ((status = Print(m_pOutputStream, "\n{1}{{", m_sIndent)).IsNOK())
		{
			return status;
		}
	}
	else
	{
		if ((status = Print(m_pOutputStream, ",\n{1}{{", m_sIndent)).IsNOK())
		{
			return status;
		}
	}
	m_stackStates.push(State_Object);
	m_bFirst = True;
	m_cIndent++;
	AdjustIndent();

	return Status();
}

//object member
Status DataWriter::BeginArray(const Char *szName)
{
	String sName;
	Status status;

	if (NULL == m_pOutputStream)
	{
		return Status(Status_NotInitialized, "No valid output stream");
	}
	if (m_stackStates.empty() ||
		(State_Object != m_stackStates.top() && State_RootObject != m_stackStates.top()))
	{
		return Status(Status_InvalidCall, "Out of order call (must be called from an object)");
	}
	if ((status = SAXParser::EscapeString(szName, &sName)).IsNOK())
	{
		return status;
	}
	if (m_bFirst)
	{
		if ((status =
			Print(m_pOutputStream, "\n{1}\"{2}\":\n{3}[", m_sIndent, sName, m_sIndent)).IsNOK())
		{
			return status;
		}
	}
	else
	{
		if ((status =
			Print(m_pOutputStream, ",\n{1}\"{2}\":\n{3}[", m_sIndent, sName, m_sIndent)).IsNOK())
		{
			return status;
		}
	}
	m_stackStates.push(State_Array);
	m_bFirst = True;
	m_cIndent++;
	AdjustIndent();

	return Status();
}

//array item
Status DataWriter::BeginArray()
{
	Status status;

	if (NULL == m_pOutputStream)
	{
		return Status(Status_NotInitialized, "No valid output stream");
	}
	if (m_stackStates.empty() ||
		(State_Array != m_stackStates.top() && State_RootArray != m_stackStates.top()))
	{
		return Status(Status_InvalidCall, "Out of order call (must be called from an array)");
	}
	if (m_bFirst)
	{
		if ((status = Print(m_pOutputStream, "\n{1}[", m_sIndent)).IsNOK())
		{
			return status;
		}
	}
	else
	{
		if ((status = Print(m_pOutputStream, ",\n{1}[", m_sIndent)).IsNOK())
		{
			return status;
		}
	}
	m_stackStates.push(State_Array);
	m_bFirst = True;
	m_cIndent++;
	AdjustIndent();

	return Status();
}

Status DataWriter::EndObject()
{
	Status status;

	if (NULL == m_pOutputStream)
	{
		return Status(Status_NotInitialized, "No valid output stream");
	}
	if (m_stackStates.empty() || State_Object != m_stackStates.top())
	{
		return Status(Status_InvalidCall, "Out of order call (must be called from an object)");
	}
	m_cIndent--;
	AdjustIndent();
	if ((status = Print(m_pOutputStream, "\n{1}}", m_sIndent)).IsNOK())
	{
		return status;
	}
	m_stackStates.pop();
	m_bFirst = False;

	return Status();
}

Status DataWriter::EndArray()
{
	Status status;

	if (NULL == m_pOutputStream)
	{
		return Status(Status_NotInitialized, "No valid output stream");
	}
	if (m_stackStates.empty() || State_Array != m_stackStates.top())
	{
		return Status(Status_InvalidCall, "Out of order call (must be called from an object)");
	}
	m_cIndent--;
	AdjustIndent();
	if ((status = Print(m_pOutputStream, "\n{1}]", m_sIndent)).IsNOK())
	{
		return status;
	}
	m_stackStates.pop();
	m_bFirst = False;

	return Status();
}

}//namespace JSON

}//namespace Data

}//namespace CX

