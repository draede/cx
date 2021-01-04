/* 
 * CX - C++ framework for general purpose development
 *
 * https://github.com/draede/cx
 * 
 * Copyright (C) 2014 - 2021 draede - draede [at] outlook [dot] com
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

#pragma once


#include "CX/Types.hpp"
#include "CX/Status.hpp"
#include "CX/Map.hpp"
#include "CX/Vector.hpp"
#include "CX/IO/IInputStream.hpp"
#include "CX/IO/IOutputStream.hpp"
#include "CX/Data/XML/SAXParser.hpp"
#include "CX/Print.hpp"


namespace CX
{

namespace Data
{

namespace XML
{

class Node
{
public:

	Node();

	Node(const Node &node);

	Node(const String &sName, const String &sValue, Size cAttrs = 0, ...);

	~Node();

	Status Copy(const Node &node);

	Node *GetParent();

	const Node *GetParent() const;

	Status SetName(const String &sName);

	const String &GetName() const;

	Status SetValue(const String &sValue);

	const String &GetValue() const;

	Size GetChildrenCount() const;

	Node *GetChild(Size cIndex);
	
	const Node *GetChild(Size cIndex) const;

	Node *GetChild(const String &sName);

	const Node *GetChild(const String &sName) const;

	Node *AddChild(const String &sName, const String &sValue, Size cAttrs = 0, ...);

	Node *InsertChild(Size cAtIndex, const String &sName, const String &sValue, Size cAttrs = 0, ...);

	Status RemoveChild(const Size cIndex);

	Status RemoveAllChildren();

	Size GetAttributesCount() const;

	const String &GetAttrName(Size cIndex) const;

	const String &GetAttrValue(Size cIndex) const;

	const String &GetAttrValue(const String &sName) const;

	Status AddAttribute(const String &sName, const String &sValue);

	Status RemoveAttribute(Size cIndex);

	Status RemoveAttribute(const String &sName);

	Status RemoveAllAttributes();

	Status LoadFromStream(const Char *szPath, bool bTrimValueWhiteSpace = true);

	Status LoadFromStream(IO::IInputStream *pInputStream, bool bTrimValueWhiteSpace = true);

	Status LoadFromString(const String &sString, bool bTrimValueWhiteSpace = true);

	Status LoadFromString(const Char *pString, Size cLen = (Size)-1, bool bTrimValueWhiteSpace = true);

	Status SaveToStream(const Char *szPath) const;

	Status SaveToStream(IO::IOutputStream *pOutputStream) const;

	Status SaveToString(String &sString) const;

	Node &operator = (const Node &node);

	bool operator == (const Node &node);

	bool operator != (const Node &node);

	const Node &operator [] (Size cIndex) const;

	Node &operator [] (Size cIndex);

	const String &operator () (const String &sName) const;

	String &operator () (const String &sName);

	template <typename OUTPUT>
	Status Write(OUTPUT out) const
	{
		return WriteHelper(*this, out);
	}

private:

	typedef Vector<Node *>::Type        NodesVector;
	typedef Map<String, String>::Type   AttrsMap;

	Node          *m_pParent;
	String        m_sName;
	String        m_sValue;
	AttrsMap      m_mapAttrs;
	NodesVector   m_vectorNodes;

	void FreeMem();

	template <typename OUTPUT>
	static inline Status WriteHelper(const Node &node, OUTPUT out, Size cIndent = 0)
	{
		String sIndent(cIndent, '\t');
		String sName;
		Status status;

		if ((status = SAXParser::EscapeString(node.m_sName.c_str(), &sName)).IsNOK())
		{
			return status;
		}
		if ((status = Print(out, "{1}<{2}", sIndent, sName)).IsNOK())
		{
			return status;
		}
		for (AttrsMap::const_iterator iter = node.m_mapAttrs.begin(); iter != node.m_mapAttrs.end(); ++iter)
		{
			String sAttrName;
			String sAttrValue;

			if ((status = SAXParser::EscapeString(iter->first.c_str(), &sAttrName)).IsNOK())
			{
				return status;
			}
			if ((status = SAXParser::EscapeString(iter->second.c_str(), &sAttrValue)).IsNOK())
			{
				return status;
			}
			if ((status = Print(out, " {1}=\"{2}\"", sAttrName, sAttrValue)).IsNOK())
			{
				return status;
			}
		}
		if (!node.m_vectorNodes.empty() || !node.m_sValue.empty())
		{
			String sValue;

			if ((status = SAXParser::EscapeString(node.m_sValue.c_str(), &sValue)).IsNOK())
			{
				return status;
			}
			if (node.m_vectorNodes.empty())
			{
				if ((status = Print(out, ">{1}</{2}>\n", sValue, sName)).IsNOK())
				{
					return status;
				}
			}
			else
			{
				if ((status = Print(out, ">\n")).IsNOK())
				{
					return status;
				}
				for (NodesVector::const_iterator iter = node.m_vectorNodes.begin(); 
				     iter != node.m_vectorNodes.end(); ++iter)
				{
					if ((status = WriteHelper(**iter, out, cIndent + 1)).IsNOK())
					{
						return status;
					}
				}
				if (sValue.empty())
				{
					if ((status = Print(out, "{1}</{2}>\n", sIndent, sName)).IsNOK())
					{
						return status;
					}
				}
				else
				{
					if ((status = Print(out, "{1}\n{2}</{3}>\n", sValue, sIndent, sName)).IsNOK())
					{
						return status;
					}
				}
			}
		}
		else
		{
			if ((status = Print(out, " />\n")).IsNOK())
			{
				return status;
			}
		}

		return Status();
	}

	class SAXObserver : public Data::XML::ISAXParserObserver
	{
	public:

		Node   *m_pRootNode;
		Node   *m_pCrNode;
		bool   m_bTrimValueWhiteSpace;
		Status m_status;

		virtual void OnBeginParse();

		virtual void OnEndParse();

		virtual void OnStartElement(const Char *szName, const AttrsVector *pVectorAttrs);

		virtual void OnEndElement(const Char *szName, const Char *szText);

	};

};

}//namespace XML

}//namespace Data

}//namespace CX

