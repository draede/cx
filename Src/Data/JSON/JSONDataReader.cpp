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

#include "CX/Data/JSON/DataReader.hpp"
#include "CX/Str/UTF8.hpp"
#include "CX/C/ctype.h"


namespace CX
{

namespace Data
{

namespace JSON
{

DataReader::DataReader(IO::IInputStream *pInputStream)
{
	m_pInputStream    = pInputStream;
	m_pBuffer         = NULL;
	m_cbBufSize       = 0;
	m_cbBufUsedSize   = 0;
	m_cbBufOffset     = 0;
	m_bIsEOF          = false;
}

DataReader::~DataReader()
{
	if (NULL != m_pBuffer)
	{
		Free(m_pBuffer);
	}
}

Status DataReader::Read()
{
	Size   cbReqSize;
	Size   cbAckSize;
	Status status;

	if (NULL == m_pBuffer)
	{
		if (NULL == (m_pBuffer = (Byte *)Alloc(READ_BUFFER_SIZE)))
		{
			return Status(Status_MemAllocFailed, "Failed to allocate internal buffer of {1} bytes", READ_BUFFER_SIZE);
		}
		m_cbBufSize     = READ_BUFFER_SIZE;
		m_cbBufOffset   = 0;
		m_cbBufUsedSize = 0;
	}
	if (m_cbBufOffset >= m_cbBufUsedSize)
	{
		m_cbBufUsedSize = 0;
		m_cbBufOffset   = 0;
	}
	if (m_cbBufUsedSize < m_cbBufSize)
	{
		cbReqSize = m_cbBufSize - m_cbBufUsedSize;
		if ((status = m_pInputStream->Read(m_pBuffer + m_cbBufUsedSize, cbReqSize, &cbAckSize)).IsNOK())
		{
			if (status.GetCode() == Status_EOF)
			{
				m_bIsEOF = true;
			}

			return status;
		}
		m_cbBufUsedSize += cbAckSize;
	}

	return Status();
}

bool DataReader::IsEOF()
{
	return m_bIsEOF;
}

bool DataReader::IsValid()
{
	if (m_bIsEOF)
	{
		return false;
	}
	if (m_cbBufOffset >= m_cbBufUsedSize)
	{
		Status status;

		if ((status = Read()).IsNOK())
		{
			return false;
		}
	}

	return (m_cbBufOffset < m_cbBufUsedSize);
}

Byte DataReader::Get()
{
	return m_pBuffer[m_cbBufOffset];
}

Status DataReader::Next()
{
	if (m_cbBufOffset >= m_cbBufUsedSize)
	{
		return Status(Status_InvalidCall, "Out of order call");
	}
	m_cbBufOffset++;

	return Status();
}

Status DataReader::SkipWhiteSpaces()
{
	while (IsValid() && cx_isspace(Get()))
	{
		Next();
	}

	return Status();
}

Status DataReader::SkipChar(Char ch)
{
	Status status;

	if ((status = SkipWhiteSpaces()).IsNOK())
	{
		return status;
	}
	if (!IsValid())
	{
		return Status(Status_ReadFailed, "Expected '{1}'", ch);
	}
	if ((Byte)ch != Get())
	{
		return Status(Status_ReadFailed, "Expected '{1}'", ch);
	}
	Next();

	return Status();
}

bool DataReader::CheckArrayEnd()
{
	Status status;

	if ((status = SkipWhiteSpaces()).IsNOK())
	{
		return false;
	}
	if (!IsValid())
	{
		return false;
	}
	if ((Byte)']' != Get())
	{
		return false;
	}

	return true;
}

Status DataReader::ReadBool(Bool *pbValue)
{
	String sToken;
	Status status;

	if ((status = SkipWhiteSpaces()).IsNOK())
	{
		return status;
	}
	while (IsValid() && cx_isalpha(Get()))
	{
		sToken += (Char )Get();
		Next();
	}
	if (0 == cx_stricmp(sToken.c_str(), "false"))
	{
		*pbValue = false;

		return Status();
	}
	else
	if (0 == cx_stricmp(sToken.c_str(), "true"))
	{
		*pbValue = true;

		return Status();
	}
	else
	{
		return Status(Status_ParseFailed, "Expected true or false");
	}
}

Status DataReader::ReadInt(Int64 *pnValue)
{
	Size   cDigits;
	Bool   bNeg;
	Status status;

	if ((status = SkipWhiteSpaces()).IsNOK())
	{
		return status;
	}
	cDigits  = 0;
	bNeg     = false;
	*pnValue = 0;
	if (!IsValid())
	{
		return Status(Status_ParseFailed, "Expected int value");
	}
	if ((Byte)'-' == Get())
	{
		bNeg = true;
		Next();
	}
	else
	if ((Byte)'+' == Get())
	{
		Next();
	}
	while (IsValid() && cx_isdigit(Get()))
	{
		(*pnValue) *= 10;
		(*pnValue) += Get() - '0';
		cDigits++;
		Next();
	}
	if (0 == cDigits)
	{
		return Status(Status_ParseFailed, "Expected int value");
	}
	if (bNeg)
	{
		(*pnValue) = -(*pnValue);
	}

	return Status();
}

Status DataReader::ReadReal(Double *plfValue)
{
	Size   cDigits;
	Bool   bNeg;
	Bool   bPoint;
	Double lfFactor;
	Status status;

	if ((status = SkipWhiteSpaces()).IsNOK())
	{
		return status;
	}
	cDigits   = 0;
	bNeg      = false;
	*plfValue = 0.0;
	lfFactor  = 1.0;
	bPoint    = false;
	if (!IsValid())
	{
		return Status(Status_ParseFailed, "Expected real value");
	}
	if ((Byte)'-' == Get())
	{
		bNeg = true;
		Next();
	}
	else
	if ((Byte)'+' == Get())
	{
		Next();
	}
	while (IsValid())
	{
		if ((Byte)'.' == Get())
		{
			if (bPoint)
			{
				return Status(Status_ParseFailed, "Invalid real value");
			}
			bPoint = true;
			Next();
		}
		else
		if (cx_isdigit(Get()))
		{
			(*plfValue) *= 10.0;
			(*plfValue) += Get() - '0';
			if (bPoint)
			{
				lfFactor *= 10.0;
			}
			cDigits++;
			Next();
		}
		else
		{
			break;
		}
	}
	if (0 == cDigits)
	{
		return Status(Status_ParseFailed, "Expected real value");
	}
	if (bPoint)
	{
		(*plfValue) /= lfFactor;
	}
	if (bNeg)
	{
		(*plfValue) = -(*plfValue);
	}

	return Status();
}

Status DataReader::ReadString(String *psValue)
{
	Status status;

	if ((status = SkipWhiteSpaces()).IsNOK())
	{
		return status;
	}
	psValue->clear();
	if ((status = SkipChar('"')).IsNOK())
	{
		return status;
	}
	for (;;)
	{
		if (!IsValid())
		{
			return Status(Status_ParseFailed, "Expected string value");
		}
		if ((Byte)'\\' == Get())
		{
			Next();
			if (!IsValid())
			{
				return Status(Status_ParseFailed, "Expected string value");
			}
			if ((Byte)'\\' == Get() || (Byte)'"' == Get() || (Byte)'/' == Get())
			{
				*psValue += (Char)Get();
				Next();
			}
			else
			if ((Byte)'b' == Get())
			{
				*psValue += "\b";
				Next();
			}
			else
			if ((Byte)'f' == Get())
			{
				*psValue += "\f";
				Next();
			}
			else
			if ((Byte)'n' == Get())
			{
				*psValue += "\n";
				Next();
			}
			else
			if ((Byte)'r' == Get())
			{
				*psValue += "\r";
				Next();
			}
			else
			if ((Byte)'t' == Get())
			{
				*psValue += "\t";
				Next();
			}
			else
			{
				*psValue += "\\";
				*psValue += (Char)Get();
				Next();
			}
		}
		else
		{
			if ((Byte)'"' == Get())
			{
				Next();

				break;
			}
			else
			{
				*psValue += (Char)Get();
				Next();
			}
		}
	}

	return Status();
}

Status DataReader::Begin()
{
	Status status;

	if (NULL == m_pInputStream || !m_pInputStream->IsOK())
	{
		return Status(Status_NotInitialized, "Input stream is not valid");
	}
	if (!m_stackStates.empty())
	{
		return Status(Status_InvalidCall, "Out of order call");
	}
	if ((status = SkipChar('{')).IsNOK())
	{
		return status;
	}
	m_stackStates.push(StateData(State_Object));

	return Status();
}

Status DataReader::End()
{
	Status status;

	if (NULL == m_pInputStream || !m_pInputStream->IsOK())
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
	if ((status = SkipChar('}')).IsNOK())
	{
		return status;
	}
	m_stackStates.pop();

	return Status();
}

Status DataReader::BeginObjectObject(const Char *szName)
{
	String sName;
	Status status;

	if (NULL == m_pInputStream || !m_pInputStream->IsOK())
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
	if (0 < m_stackStates.top().cCount)
	{
		if ((status = SkipChar(',')).IsNOK())
		{
			return status;
		}
	}
	if ((status = ReadString(&sName)).IsNOK())
	{
		return status;
	}
	if (0 != cx_strcmp(sName.c_str(), szName))
	{
		return Status(Status_ParseFailed, "Expected '{1}'", szName);
	}
	if ((status = SkipChar(':')).IsNOK())
	{
		return status;
	}
	m_stackStates.top().cCount++;
	if ((status = SkipChar('{')).IsNOK())
	{
		return status;
	}
	m_stackStates.push(StateData(State_Object));

	return Status();
}

Status DataReader::EndObjectObject()
{
	Status status;

	if (NULL == m_pInputStream || !m_pInputStream->IsOK())
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
	if ((status = SkipChar('}')).IsNOK())
	{
		return status;
	}
	m_stackStates.pop();

	return Status();
}

Status DataReader::BeginObjectArray(const Char *szName)
{
	String sName;
	Status status;

	if (NULL == m_pInputStream || !m_pInputStream->IsOK())
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
	if (0 < m_stackStates.top().cCount)
	{
		if ((status = SkipChar(',')).IsNOK())
		{
			return status;
		}
	}
	if ((status = ReadString(&sName)).IsNOK())
	{
		return status;
	}
	if (0 != cx_strcmp(sName.c_str(), szName))
	{
		return Status(Status_ParseFailed, "Expected '{1}'", szName);
	}
	if ((status = SkipChar(':')).IsNOK())
	{
		return status;
	}
	m_stackStates.top().cCount++;
	if ((status = SkipChar('[')).IsNOK())
	{
		return status;
	}
	m_stackStates.push(StateData(State_Array));

	return Status();
}

Status DataReader::EndObjectArray()
{
	Status status;

	if (NULL == m_pInputStream || !m_pInputStream->IsOK())
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
	if ((status = SkipChar(']')).IsNOK())
	{
		return status;
	}
	m_stackStates.pop();

	return Status();
}

Status DataReader::ReadObjectBool(const Char *szName, Bool *pbValue)
{
	String sName;
	Status status;

	if (NULL == m_pInputStream || !m_pInputStream->IsOK())
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
	if (0 < m_stackStates.top().cCount)
	{
		if ((status = SkipChar(',')).IsNOK())
		{
			return status;
		}
	}
	if ((status = ReadString(&sName)).IsNOK())
	{
		return status;
	}
	if (0 != cx_strcmp(sName.c_str(), szName))
	{
		return Status(Status_ParseFailed, "Expected '{1}'", szName);
	}
	if ((status = SkipChar(':')).IsNOK())
	{
		return status;
	}
	if ((status = ReadBool(pbValue)).IsNOK())
	{
		return status;
	}
	m_stackStates.top().cCount++;

	return Status();
}

Status DataReader::ReadObjectInt(const Char *szName, Int64 *pnValue)
{
	String sName;
	Status status;

	if (NULL == m_pInputStream || !m_pInputStream->IsOK())
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
	if (0 < m_stackStates.top().cCount)
	{
		if ((status = SkipChar(',')).IsNOK())
		{
			return status;
		}
	}
	if ((status = ReadString(&sName)).IsNOK())
	{
		return status;
	}
	if (0 != cx_strcmp(sName.c_str(), szName))
	{
		return Status(Status_ParseFailed, "Expected '{1}'", szName);
	}
	if ((status = SkipChar(':')).IsNOK())
	{
		return status;
	}
	if ((status = ReadInt(pnValue)).IsNOK())
	{
		return status;
	}
	m_stackStates.top().cCount++;

	return Status();
}

Status DataReader::ReadObjectReal(const Char *szName, Double *plfValue)
{
	String sName;
	Status status;

	if (NULL == m_pInputStream || !m_pInputStream->IsOK())
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
	if (0 < m_stackStates.top().cCount)
	{
		if ((status = SkipChar(',')).IsNOK())
		{
			return status;
		}
	}
	if ((status = ReadString(&sName)).IsNOK())
	{
		return status;
	}
	if (0 != cx_strcmp(sName.c_str(), szName))
	{
		return Status(Status_ParseFailed, "Expected '{1}'", szName);
	}
	if ((status = SkipChar(':')).IsNOK())
	{
		return status;
	}
	if ((status = ReadReal(plfValue)).IsNOK())
	{
		return status;
	}
	m_stackStates.top().cCount++;

	return Status();
}

Status DataReader::ReadObjectString(const Char *szName, String *psValue)
{
	String sName;
	Status status;

	if (NULL == m_pInputStream || !m_pInputStream->IsOK())
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
	if (0 < m_stackStates.top().cCount)
	{
		if ((status = SkipChar(',')).IsNOK())
		{
			return status;
		}
	}
	if ((status = ReadString(&sName)).IsNOK())
	{
		return status;
	}
	if (0 != cx_strcmp(sName.c_str(), szName))
	{
		return Status(Status_ParseFailed, "Expected '{1}'", szName);
	}
	if ((status = SkipChar(':')).IsNOK())
	{
		return status;
	}
	if ((status = ReadString(psValue)).IsNOK())
	{
		return status;
	}
	m_stackStates.top().cCount++;

	return Status();
}

Status DataReader::ReadObjectWString(const Char *szName, WString *pwsValue)
{
	String sName;
	String sValue;
	Status status;

	if (NULL == m_pInputStream || !m_pInputStream->IsOK())
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
	if (0 < m_stackStates.top().cCount)
	{
		if ((status = SkipChar(',')).IsNOK())
		{
			return status;
		}
	}
	if ((status = ReadString(&sName)).IsNOK())
	{
		return status;
	}
	if (0 != cx_strcmp(sName.c_str(), szName))
	{
		return Status(Status_ParseFailed, "Expected '{1}'", szName);
	}
	if ((status = SkipChar(':')).IsNOK())
	{
		return status;
	}
	if ((status = ReadString(&sValue)).IsNOK())
	{
		return status;
	}
	if ((status = Str::UTF8::ToUTF16(sValue.c_str(), pwsValue)).IsNOK())
	{
		return status;
	}
	m_stackStates.top().cCount++;

	return Status();
}

Status DataReader::BeginArrayObject()
{
	Status status;

	if (NULL == m_pInputStream || !m_pInputStream->IsOK())
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
	if (CheckArrayEnd())
	{
		return Status_NoMoreItems;
	}
	if (0 < m_stackStates.top().cCount)
	{
		if ((status = SkipChar(',')).IsNOK())
		{
			return status;
		}
	}
	m_stackStates.top().cCount++;
	if ((status = SkipChar('{')).IsNOK())
	{
		return status;
	}
	m_stackStates.push(StateData(State_Object));

	return Status();
}

Status DataReader::EndArrayObject()
{
	Status status;

	if (NULL == m_pInputStream || !m_pInputStream->IsOK())
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
	if ((status = SkipChar('}')).IsNOK())
	{
		return status;
	}
	m_stackStates.pop();

	return Status();
}

Status DataReader::BeginArrayArray()
{
	Status status;

	if (NULL == m_pInputStream || !m_pInputStream->IsOK())
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
	if (CheckArrayEnd())
	{
		return Status_NoMoreItems;
	}
	if (0 < m_stackStates.top().cCount)
	{
		if ((status = SkipChar(',')).IsNOK())
		{
			return status;
		}
	}
	m_stackStates.top().cCount++;
	if ((status = SkipChar('[')).IsNOK())
	{
		return status;
	}
	m_stackStates.push(StateData(State_Array));

	return Status();
}

Status DataReader::EndArrayArray()
{
	Status status;

	if (NULL == m_pInputStream || !m_pInputStream->IsOK())
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
	if ((status = SkipChar(']')).IsNOK())
	{
		return status;
	}
	m_stackStates.pop();

	return Status();
}

//will return Status_NoMoreItems at the end of the array
Status DataReader::ReadArrayBool(Bool *pbValue)
{
	Status status;

	if (NULL == m_pInputStream || !m_pInputStream->IsOK())
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
	if (CheckArrayEnd())
	{
		return Status_NoMoreItems;
	}
	if (0 < m_stackStates.top().cCount)
	{
		if ((status = SkipChar(',')).IsNOK())
		{
			return status;
		}
	}
	if ((status = ReadBool(pbValue)).IsNOK())
	{
		return status;
	}
	m_stackStates.top().cCount++;

	return Status();
}

//will return Status_NoMoreItems at the end of the array
Status DataReader::ReadArrayInt(Int64 *pnValue)
{
	Status status;

	if (NULL == m_pInputStream || !m_pInputStream->IsOK())
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
	if (CheckArrayEnd())
	{
		return Status_NoMoreItems;
	}
	if (0 < m_stackStates.top().cCount)
	{
		if ((status = SkipChar(',')).IsNOK())
		{
			return status;
		}
	}
	if ((status = ReadInt(pnValue)).IsNOK())
	{
		return status;
	}
	m_stackStates.top().cCount++;

	return Status();
}

//will return Status_NoMoreItems at the end of the array
Status DataReader::ReadArrayReal(Double *plfValue)
{
	Status status;

	if (NULL == m_pInputStream || !m_pInputStream->IsOK())
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
	if (CheckArrayEnd())
	{
		return Status_NoMoreItems;
	}
	if (0 < m_stackStates.top().cCount)
	{
		if ((status = SkipChar(',')).IsNOK())
		{
			return status;
		}
	}
	if ((status = ReadReal(plfValue)).IsNOK())
	{
		return status;
	}
	m_stackStates.top().cCount++;

	return Status();
}

//will return Status_NoMoreItems at the end of the array
Status DataReader::ReadArrayString(String *psValue)
{
	Status status;

	if (NULL == m_pInputStream || !m_pInputStream->IsOK())
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
	if (CheckArrayEnd())
	{
		return Status_NoMoreItems;
	}
	if (0 < m_stackStates.top().cCount)
	{
		if ((status = SkipChar(',')).IsNOK())
		{
			return status;
		}
	}
	if ((status = ReadString(psValue)).IsNOK())
	{
		return status;
	}
	m_stackStates.top().cCount++;

	return Status();
}

//will return Status_NoMoreItems at the end of the array
Status DataReader::ReadArrayWString(WString *pwsValue)
{
	String sValue;
	Status status;

	if (NULL == m_pInputStream || !m_pInputStream->IsOK())
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
	if (CheckArrayEnd())
	{
		return Status_NoMoreItems;
	}
	if (0 < m_stackStates.top().cCount)
	{
		if ((status = SkipChar(',')).IsNOK())
		{
			return status;
		}
	}
	if ((status = ReadString(&sValue)).IsNOK())
	{
		return status;
	}
	if ((status = Str::UTF8::ToUTF16(sValue.c_str(), pwsValue)).IsNOK())
	{
		return status;
	}
	m_stackStates.top().cCount++;

	return Status();
}

}//namespace JSON

}//namespace Data

}//namespace CX

