
#pragma once


#include "CX/Data/JSON/ISAXParserObserver.hpp"
#include "CX/IO/IOutputStream.hpp"
#include "CX/String.hpp"
#include "CX/Stack.hpp"
#include "CX/Print.hpp"
#include "CX/APIDefs.hpp"


namespace CX
{

namespace Data
{

namespace JSON
{

class CX_API DumpSAXParserObserver : public ISAXParserObserver
{
public:

	DumpSAXParserObserver(IO::IOutputStream *pOutputStream);

	~DumpSAXParserObserver();

	virtual Bool OnBeginParse();

	virtual Bool OnEndParse();

	virtual Bool OnBeginObject();

	virtual Bool OnEndObject();

	virtual Bool OnBeginArray();

	virtual Bool OnEndArray();

	virtual Bool OnKey(const Char *pBuffer, Size cLen);

	virtual Bool OnNullValue();

	virtual Bool OnBoolValue(Bool bBool);

	virtual Bool OnIntValue(Int64 nInt);

	virtual Bool OnUIntValue(UInt64 uUInt);

	virtual Bool OnRealValue(Double lfReal);

	virtual Bool OnStringValue(const Char *pBuffer, Size cLen);

private:

	enum NodeType
	{
		NodeType_Array,
		NodeType_Object,
	};

	struct Node
	{
		Node()
		{
		}

		Node(NodeType nType, Size cChildrenCount = 0)
		{
			this->nType          = nType;
			this->cChildrenCount = cChildrenCount;
		}

		NodeType   nType;
		Size       cChildrenCount;
	};

	typedef Stack<Node>::Type   NodesStack;

	IO::IOutputStream   *m_pOutputStream;
#pragma warning(push)
#pragma warning(disable: 4251)
	NodesStack          m_stackNodes;
	String              m_sKey;
#pragma warning(pop)

	template <typename T>
	Bool DumpValue(const T &value)
	{
		if (m_stackNodes.empty())
		{
			return False;
		}
		if ((NodeType_Object == m_stackNodes.top().nType && m_sKey.empty()) || 
			 (NodeType_Array == m_stackNodes.top().nType && !m_sKey.empty()))
		{
			return False;
		}

		String   sIndent(m_stackNodes.size(), '\t');

		if (NodeType_Object == m_stackNodes.top().nType)
		{
			if (0 == m_stackNodes.top().cChildrenCount)
			{
				Print(m_pOutputStream, "\n{1}\"{2}\": {3}", sIndent, m_sKey, value);
			}
			else
			{
				Print(m_pOutputStream, ",\n{1}\"{2}\": {3}", sIndent, m_sKey, value);
			}
		}
		else
		{
			if (0 == m_stackNodes.top().cChildrenCount)
			{
				Print(m_pOutputStream, "\n{1}{2}", sIndent, value);
			}
			else
			{
				Print(m_pOutputStream, ",\n{1}{2}", sIndent, value);
			}
		}
		m_stackNodes.top().cChildrenCount++;

		m_sKey.clear();

		return True;
	}

	Bool DumpStringValue(const String &value)
	{
		if (m_stackNodes.empty())
		{
			return False;
		}
		if ((NodeType_Object == m_stackNodes.top().nType && m_sKey.empty()) || 
			 (NodeType_Array == m_stackNodes.top().nType && !m_sKey.empty()))
		{
			return False;
		}

		String   sIndent(m_stackNodes.size(), '\t');

		if (NodeType_Object == m_stackNodes.top().nType)
		{
			if (0 == m_stackNodes.top().cChildrenCount)
			{
				Print(m_pOutputStream, "\n{1}\"{2}\": \"{3}\"", sIndent, m_sKey, value);
			}
			else
			{
				Print(m_pOutputStream, ",\n{1}\"{2}\": \"{3}\"", sIndent, m_sKey, value);
			}
		}
		else
		{
			if (0 == m_stackNodes.top().cChildrenCount)
			{
				Print(m_pOutputStream, "\n{1}\"{2}\"", sIndent, value);
			}
			else
			{
				Print(m_pOutputStream, ",\n{1}\"{2}\"", sIndent, value);
			}
		}
		m_stackNodes.top().cChildrenCount++;

		m_sKey.clear();

		return True;
	}

};

}//namespace JSON

}//namespace Data

}//namespace CX

