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
#include "CX/Data/CSV/SAXParser.hpp"
#include "CX/IO/FileInputStream.hpp"


namespace CX
{

namespace Data
{

namespace CSV
{

SAXParser::SAXParser()
{
	m_nState      = State_None;
	m_chDelimiter = ',';
	m_bInQuote    = False;
	m_cRowIndex   = 0;
	m_uFlags      = 0;
}

SAXParser::~SAXParser()
{
}

Status SAXParser::BeginParse(Char chDelimiter/* = ','*/)
{
	if ('\r' == chDelimiter || '"' == chDelimiter || '\0' == chDelimiter)
	{
		return Status_InvalidArg;
	}
	m_nState      = State_Row;
	m_chDelimiter = chDelimiter;
	m_bInQuote    = False;
	m_cRowIndex   = 0;
	m_vectorColumns.clear();
	m_sColumn.clear();
	for (auto iter = m_vectorObservers.begin(); iter != m_vectorObservers.end(); ++iter)
	{
		(*iter)->OnBeginParse();
	}

	return Status();
}

Status SAXParser::EndParse()
{
	if (!m_sColumn.empty())
	{
		m_vectorColumns.push_back(m_sColumn);
		m_sColumn.clear();
	}
	if (!m_vectorColumns.empty())
	{
		for (auto iter = m_vectorObservers.begin(); iter != m_vectorObservers.end(); ++iter)
		{
			if (!(*iter)->OnRow(m_cRowIndex, m_vectorColumns))
			{
				break;
			}
		}
		m_cRowIndex++;
		m_vectorColumns.clear();
	}
	for (auto iter = m_vectorObservers.begin(); iter != m_vectorObservers.end(); ++iter)
	{
		(*iter)->OnEndParse();
	}
	m_nState      = State_None;
	m_chDelimiter = ',';
	m_bInQuote    = False;
	m_cRowIndex   = 0;
	m_vectorColumns.clear();
	m_sColumn.clear();

	return Status();
}

Status SAXParser::ParseStream(const Char *szPath)
{
	if (State_None == m_nState)
	{
		return Status_NotInitialized;
	}

	IO::FileInputStream   is(szPath);

	if (!is.IsOK())
	{
		return Status_OpenFailed;
	}

	return ParseStream(&is);
}

Status SAXParser::ParseStream(const WChar *wszPath)
{
	if (State_None == m_nState)
	{
		return Status_NotInitialized;
	}

	IO::FileInputStream   is(wszPath);

	if (!is.IsOK())
	{
		return Status_OpenFailed;
	}

	return ParseStream(&is);
}

Status SAXParser::ParseStream(IO::IInputStream *pInputStream)
{
	if (State_None == m_nState)
	{
		return Status_NotInitialized;
	}

	Byte     buffer[BUFFER_SIZE];
	Size     cbAckSize;
	Status   status;

	while (!pInputStream->IsEOF())
	{
		if ((status = pInputStream->Read(buffer, sizeof(buffer), &cbAckSize)))
		{
			if (!(status = ParseBuffer(buffer, cbAckSize)))
			{
				break;
			}
		}
		else
		{
			if (Status_EOF == status.GetCode())
			{
				status.Clear();
			}

			break;
		}
	}

	return status;
}

Status SAXParser::ParseBuffer(const void *pBuffer, Size cbSize)
{
	if (State_None == m_nState)
	{
		return Status_NotInitialized;
	}

	const UChar   *pPos = (const UChar *)pBuffer;
	const UChar   *pEnd = (const UChar *)pBuffer + cbSize;

	while (pPos < pEnd)
	{
		if (State_Row == m_nState)
		{
			if (m_chDelimiter == *pPos)
			{
				if (!m_bInQuote)
				{
					m_vectorColumns.push_back(m_sColumn);
					m_sColumn.clear();
				}
				else
				{
					m_sColumn += m_chDelimiter;
				}

				pPos++;
			}
			else
			if ('"' == *pPos)
			{
				if (!m_bInQuote)
				{
					m_bInQuote = True;
				}
				else
				{
					m_nState = State_Quote;
				}

				pPos++;
			}
			else
			if ('\r' == *pPos || '\n' == *pPos)
			{
				if (m_bInQuote && (Flag_Enable_Multi_Line_Strings == (Flag_Enable_Multi_Line_Strings & m_uFlags)))
				{
					m_sColumn += (Char)*pPos;

					pPos++;
				}
				else
				{
					m_nState = State_EOL;

					m_vectorColumns.push_back(m_sColumn);
					m_sColumn.clear();
					if (!m_vectorColumns.empty())
					{
						for (auto iter = m_vectorObservers.begin(); iter != m_vectorObservers.end(); ++iter)
						{
							if (!(*iter)->OnRow(m_cRowIndex, m_vectorColumns))
							{
								return Status_Cancelled;
							}
						}
						m_cRowIndex++;
						m_vectorColumns.clear();
					}
					pPos++;
				}
			}
			else
			{
				m_sColumn += (Char)*pPos;

				pPos++;
			}
		}
		else
		if (State_Quote == m_nState)
		{
			if ('"' == *pPos)
			{
				m_sColumn += '"';
				m_nState = State_Row;

				pPos++;
			}
			else
			{
				m_bInQuote = False;
				
				m_nState = State_Row;
			}
		}
		else
		if (State_EOL == m_nState)
		{
			if ('\r' == *pPos || '\n' == *pPos)
			{
				pPos++;
			}
			else
			{
				m_nState = State_Row;
			}
		}
	}

	return Status();
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

Status SAXParser::SetFlags(UInt64 uFlags)
{
	if (State_None != m_nState)
	{
		return Status_InvalidCall;
	}

	m_uFlags = uFlags;

	return Status();
}

UInt64 SAXParser::GetFlags() const
{
	return m_uFlags;
}

}//namespace CSV

}//namespace Data

}//namespace CX
