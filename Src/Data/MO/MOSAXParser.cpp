/* 
 * CX - C++ framework for general purpose development
 *
 * https://github.com/draede/cx
 * 
 * Copyright (C) 2014 - 2022 draede - draede [at] outlook [dot] com
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
#include "CX/Data/MO/SAXParser.hpp"
#include "CX/Status.hpp"
#include "CX/IO/FileInputStream.hpp"


namespace CX
{

namespace Data
{

namespace MO
{

SAXParser::SAXParser()
{
}

SAXParser::~SAXParser()
{
}

Status SAXParser::BeginParse()
{
	for (ObserversVector::iterator iter = m_vectorObservers.begin(); iter != m_vectorObservers.end(); ++iter)
	{
		(*iter)->OnBeginParse();
	}

	return Status_OK;
}

Status SAXParser::EndParse()
{
	for (ObserversVector::iterator iter = m_vectorObservers.begin(); iter != m_vectorObservers.end(); ++iter)
	{
		(*iter)->OnEndParse();
	}

	return Status_OK;
}

Status SAXParser::ParseStream(const Char *szPath)
{
	IO::FileInputStream fis(szPath);

	if (!fis.IsOK())
	{
		return Status(Status_OpenFailed, "Failed to open '{1}'", szPath);
	}

	return ParseStream(&fis);
}

Status SAXParser::ParseStream(IO::IInputStream *pInputStream)
{
	UInt32   header[7];
	Size     cbAckSize;
	UInt32   cbLenOrig;
	UInt32   cbLenTr;
	Char     szOrig[MAX_STRING_LEN + 1];
	Char     szTr[MAX_STRING_LEN + 1];
	Status   status;

	if ((status = pInputStream->Read(header, sizeof(header), &cbAckSize)).IsNOK())
	{
		return status;
	}
	if (sizeof(header) != cbAckSize)
	{
		return Status(Status_InvalidArg, "Invalid header size : {1}", cbAckSize);
	}
	if (MAGIC1 != header[MAGIC] && MAGIC2 != header[MAGIC])
	{
		return Status(Status_InvalidArg, "Invalid magic values : M={1}", header[MAGIC]);
	}
	if (MAX_STRINGS < header[COUNT])
	{
		return Status(Status_TooManyItems, "Too many strings : {1}", header[COUNT]);
	}
	for (Size i = 0; i < header[COUNT]; i++)
	{
		if ((status = ReadString(pInputStream, (UInt32)(header[ORIGOFFSET] + i * sizeof(UInt32) * 2), szOrig, 
		                         &cbLenOrig)).IsNOK())
		{
			return status;
		}
		if ((status = ReadString(pInputStream, (UInt32)(header[TROFFSET] + i * sizeof(UInt32) * 2), szTr, 
		                         &cbLenTr)).IsNOK())
		{
			return status;
		}
		for (ObserversVector::iterator iter = m_vectorObservers.begin(); iter != m_vectorObservers.end(); ++iter)
		{
			(*iter)->OnString(szOrig, szTr);
		}
	}

	return Status_OK;
}

Status SAXParser::ReadString(IO::IInputStream *pInputStream, UInt32 cbOffset, Char *pStr, UInt32 *pcbLen)
{
	UInt32   header[2];
	Size     cbAckSize;
	Status   status;

	if ((status = pInputStream->SetPos(cbOffset)).IsNOK())
	{
		return status;
	}
	if ((status = pInputStream->Read(header, sizeof(header), &cbAckSize)).IsNOK())
	{
		return status;
	}
	if (sizeof(header) != cbAckSize)
	{
		return Status(Status_InvalidArg, "Invalid string header size : {1}", cbAckSize);
	}
	if (MAX_STRING_LEN < header[LEN])
	{
		return Status(Status_TooBig, "String to large : {1}", header[LEN]);
	}
	if (0 < header[LEN])
	{
		if ((status = pInputStream->SetPos(header[OFFSET])).IsNOK())
		{
			return status;
		}
		if ((status = pInputStream->Read(pStr, header[LEN], &cbAckSize)).IsNOK())
		{
			return status;
		}
		if (header[LEN] != cbAckSize)
		{
			return Status(Status_InvalidArg, "Failed to read string : {1} (expected [2})", cbAckSize, header[LEN]);
		}
	}
	pStr[header[LEN]] = 0;
	*pcbLen           = header[LEN];

	return Status_OK;
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

}//namespace CSV

}//namespace Data

}//namespace CX
