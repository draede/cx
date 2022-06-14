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
#include "CX/Data/JSON/DumpSAXParserObserver.hpp"
#include "CX/Print.hpp"


namespace CX
{

namespace Data
{

namespace JSON
{

DumpSAXParserObserver::DumpSAXParserObserver(IO::IOutputStream *pOutputStream)
{
	m_pOutputStream = pOutputStream;
}

DumpSAXParserObserver::~DumpSAXParserObserver()
{
}

Bool DumpSAXParserObserver::OnBeginParse()
{
	while (!m_stackNodes.empty())
	{
		m_stackNodes.pop();
	}
	m_sKey.clear();

	return True;
}

Bool DumpSAXParserObserver::OnEndParse()
{
	if (!m_stackNodes.empty())
	{
		return False;
	}
	m_sKey.clear();

	return True;
}

Bool DumpSAXParserObserver::OnBeginObject()
{
	if (m_stackNodes.empty())
	{
		if (!m_sKey.empty())
		{
			return False;
		}
	}
	else
	{
		if ((NodeType_Object == m_stackNodes.top().nType && m_sKey.empty()) || 
		    (NodeType_Array == m_stackNodes.top().nType && !m_sKey.empty()))
		{
			return False;
		}
	}

	if (m_stackNodes.empty())
	{
		Print(m_pOutputStream, "{{");
	}
	else
	{
		String   sIndent(m_stackNodes.size(), '\t');

		if (NodeType_Object == m_stackNodes.top().nType)
		{
			if (0 == m_stackNodes.top().cChildrenCount)
			{
				Print(m_pOutputStream, "\n{1}\"{2}\": \n{1}{{", sIndent, m_sKey);
			}
			else
			{
				Print(m_pOutputStream, ",\n{1}\"{2}\": \n{1}{{", sIndent, m_sKey);
			}
		}
		else
		{
			if (0 == m_stackNodes.top().cChildrenCount)
			{
				Print(m_pOutputStream, "\n{1}{{", sIndent);
			}
			else
			{
				Print(m_pOutputStream, ",\n{1}{{", sIndent);
			}
		}
		m_stackNodes.top().cChildrenCount++;
	}

	m_stackNodes.push(Node(NodeType_Object));

	m_sKey.clear();

	return True;
}

Bool DumpSAXParserObserver::OnEndObject()
{
	if (m_stackNodes.empty())
	{
		return False;
	}
	m_stackNodes.pop();

	String   sIndent(m_stackNodes.size(), '\t');

	Print(m_pOutputStream, "\n{1}}", sIndent);

	m_sKey.clear();

	return True;
}

Bool DumpSAXParserObserver::OnBeginArray()
{
	if (m_stackNodes.empty())
	{
		if (!m_sKey.empty())
		{
			return False;
		}
	}
	else
	{
		if ((NodeType_Object == m_stackNodes.top().nType && m_sKey.empty()) || 
		    (NodeType_Array == m_stackNodes.top().nType && !m_sKey.empty()))
		{
			return False;
		}
	}

	if (m_stackNodes.empty())
	{
		Print(m_pOutputStream, "[");
	}
	else
	{
		String   sIndent(m_stackNodes.size(), '\t');

		if (NodeType_Object == m_stackNodes.top().nType)
		{
			if (0 == m_stackNodes.top().cChildrenCount)
			{
				Print(m_pOutputStream, "\n{1}\"{2}\": \n{1}[", sIndent, m_sKey);
			}
			else
			{
				Print(m_pOutputStream, ",\n{1}\"{2}\": \n{1}[", sIndent, m_sKey);
			}
		}
		else
		{
			if (0 == m_stackNodes.top().cChildrenCount)
			{
				Print(m_pOutputStream, "\n{1}[", sIndent);
			}
			else
			{
				Print(m_pOutputStream, ",\n{1}[", sIndent);
			}
		}
		m_stackNodes.top().cChildrenCount++;
	}

	m_stackNodes.push(Node(NodeType_Array));

	m_sKey.clear();

	return True;
}

Bool DumpSAXParserObserver::OnEndArray()
{
	if (m_stackNodes.empty())
	{
		return False;
	}
	m_stackNodes.pop();

	String   sIndent(m_stackNodes.size(), '\t');

	Print(m_pOutputStream, "\n{1}]", sIndent);

	m_sKey.clear();

	return True;
}

Bool DumpSAXParserObserver::OnKey(const Char *pBuffer, Size cLen)
{
	if (m_stackNodes.empty())
	{
		return False;
	}

	m_sKey.assign(pBuffer, cLen);

	return True;
}

Bool DumpSAXParserObserver::OnNullValue()
{
	return DumpValue("null");
}

Bool DumpSAXParserObserver::OnBoolValue(Bool bBool)
{
	return DumpValue(bBool);
}

Bool DumpSAXParserObserver::OnIntValue(Int64 nInt)
{
	return DumpValue(nInt);
}

Bool DumpSAXParserObserver::OnUIntValue(UInt64 uUInt)
{
	return DumpValue(uUInt);
}

Bool DumpSAXParserObserver::OnRealValue(Double lfReal)
{
	return DumpValue(lfReal);
}

Bool DumpSAXParserObserver::OnStringValue(const Char *pBuffer, Size cLen)
{
	String sString(pBuffer, cLen);

	return DumpStringValue(sString);
}

}//namespace JSON

}//namespace Data

}//namespace CX

