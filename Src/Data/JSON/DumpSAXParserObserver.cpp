
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

