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
#include "CX/Data/XML/Node.hpp"
#include "CX/IO/FileInputStream.hpp"
#include "CX/IO/FileOutputStream.hpp"
#include "CX/IO/MemInputStream.hpp"
#include "CX/Set.hpp"
#include "CX/Print.hpp"
#include <stdarg.h>


namespace CX
{

namespace Data
{

namespace XML
{

Node::Node()
{
	m_pParent = NULL;
}

Node::Node(const Node &node)
{
	m_pParent = NULL;
	Copy(node);
}

Node::Node(const String &sName, const String &sValue, Size cAttrs/* = 0*/, ...)
{
	va_list ap;

	m_pParent = NULL;
	m_sName   = sName;
	m_sValue  = sValue;
	va_start(ap, cAttrs);
	for (Size i = 0; i < cAttrs; i++) 
	{
		const Char *szName  =  va_arg(ap, const Char *);
		const Char *szValue = va_arg(ap, const Char *);

		m_mapAttrs[szName] = szValue;
	}
	va_end(ap);
}

Node::~Node()
{
	FreeMem();
}

void Node::FreeMem()
{
	m_mapAttrs.clear();
	for (NodesVector::const_iterator iter = m_vectorNodes.begin(); iter != m_vectorNodes.begin(); ++iter)
	{
		delete *iter;
	}
	m_vectorNodes.clear();
}

Status Node::Copy(const Node &node)
{
	FreeMem();
	m_mapAttrs.clear();
	m_vectorNodes.clear();
	m_mapAttrs = node.m_mapAttrs;
	for (NodesVector::const_iterator iter = node.m_vectorNodes.begin(); iter != node.m_vectorNodes.begin(); ++iter)
	{
		Node *pNode;

		if (NULL == (pNode = new (std::nothrow) Node()))
		{
			return Status_MemAllocFailed;
		}
		*pNode = **iter;
		m_vectorNodes.push_back(pNode);
	}

	return SetName(node.m_sName);
}

Node *Node::GetParent()
{
	return m_pParent;
}

const Node *Node::GetParent() const
{
	return m_pParent;
}

Status Node::SetName(const String &sName)
{
	m_sName = sName;

	return Status();
}

const String &Node::GetName() const
{
	return m_sName;
}

Status Node::SetValue(const String &sValue)
{
	m_sValue = sValue;

	return Status();
}

const String &Node::GetValue() const
{
	return m_sValue;
}

Size Node::GetChildrenCount() const
{
	return m_vectorNodes.size();
}

Node *Node::GetChild(Size cIndex)
{
	if (cIndex >= m_vectorNodes.size())
	{
		return NULL;
	}

	NodesVector::iterator iter = m_vectorNodes.begin();

	iter += cIndex;

	return *iter;
}

const Node *Node::GetChild(Size cIndex) const
{
	if (cIndex >= m_vectorNodes.size())
	{
		return NULL;
	}

	NodesVector::const_iterator iter = m_vectorNodes.begin();

	iter += cIndex;

	return *iter;
}

Node *Node::AddChild(const String &sName, const String &sValue, Size cAttrs/* = 0*/, ...)
{
	Node *pNode;

	if (NULL == (pNode = new (std::nothrow) Node(sName, sValue)))
	{
		return NULL;
	}
	pNode->m_pParent = this;
	m_vectorNodes.push_back(pNode);

	va_list ap;

	va_start(ap, cAttrs);
	for (Size i = 0; i < cAttrs; i++)
	{
		const Char *szName  = va_arg(ap, const Char *);
		const Char *szValue = va_arg(ap, const Char *);

		pNode->m_mapAttrs[szName] = szValue;
	}
	va_end(ap);

	return pNode;
}

Node *Node::InsertChild(Size cAtIndex, const String &sName, const String &sValue, Size cAttrs/* = 0*/, ...)
{
	Node *pNode;

	if (NULL == (pNode = new (std::nothrow) Node(sName, sValue)))
	{
		return NULL;
	}
	pNode->m_pParent = this;
	if (cAtIndex >= m_vectorNodes.size())
	{
		m_vectorNodes.push_back(pNode);
	}
	else
	{
		NodesVector::iterator iter = m_vectorNodes.begin();

		iter += cAtIndex;
		m_vectorNodes.insert(iter, pNode);
	}

	va_list ap;

	va_start(ap, cAttrs);
	for (Size i = 0; i < cAttrs; i++)
	{
		const Char *szName  = va_arg(ap, const Char *);
		const Char *szValue = va_arg(ap, const Char *);

		pNode->m_mapAttrs[szName] = szValue;
	}
	va_end(ap);

	return pNode;
}

Status Node::RemoveAllChildren()
{
	for (NodesVector::const_iterator iter = m_vectorNodes.begin(); iter != m_vectorNodes.begin(); ++iter)
	{
		delete *iter;
	}
	m_vectorNodes.clear();

	return Status();
}

Size Node::GetAttributesCount() const
{
	return m_mapAttrs.size();
}

const String &Node::GetAttrName(Size cIndex) const
{
	static const String sName;

	if (cIndex >= m_mapAttrs.size())
	{
		return sName;
	}

	AttrsMap::const_iterator iter = m_mapAttrs.begin();

	while(0 < cIndex)
	{
		++iter;
		cIndex--;
	}

	return iter->first;
}

const String &Node::GetAttrValue(Size cIndex) const
{
	static const String sValue;

	if (cIndex >= m_mapAttrs.size())
	{
		return sValue;
	}

	AttrsMap::const_iterator iter = m_mapAttrs.begin();

	while (0 < cIndex)
	{
		++iter;
		cIndex--;
	}

	return iter->second;
}

const String &Node::GetAttrValue(const String &sName) const
{
	static const String sValue;

	AttrsMap::const_iterator iter = m_mapAttrs.find(sName);

	if (m_mapAttrs.end() != iter)
	{
		return iter->second;
	}
	else
	{
		return sValue;
	}
}

Status Node::AddAttribute(const String &sName, const String &sValue)
{
	m_mapAttrs[sName] = sValue;

	return Status();
}

Status Node::RemoveAttribute(Size cIndex)
{
	if (cIndex >= m_mapAttrs.size())
	{
		return Status_NotFound;
	}

	AttrsMap::iterator iter = m_mapAttrs.begin();

	while (0 < cIndex)
	{
		++iter;
		cIndex--;
	}
	m_mapAttrs.erase(iter);

	return Status();
}

Status Node::RemoveAttribute(const String &sName)
{
	AttrsMap::iterator iter = m_mapAttrs.find(sName);

	if (m_mapAttrs.end() != iter)
	{
		m_mapAttrs.erase(iter);

		return Status();
	}
	else
	{
		return Status_NotFound;
	}
}

Status Node::RemoveAllAttributes()
{
	m_mapAttrs.clear();

	return Status();
}

Status Node::LoadFromStream(const Char *szPath, bool bTrimValueWhiteSpace/* = true*/)
{
	IO::FileInputStream fis(szPath);

	if (!fis.IsOK())
	{
		return Status(Status_OpenFailed, "Failed to open '{1}'", szPath);
	}

	return LoadFromStream(&fis, bTrimValueWhiteSpace);
}

Status Node::LoadFromStream(IO::IInputStream *pInputStream, bool bTrimValueWhiteSpace/* = true*/)
{
	SAXParser     parser;
	SAXObserver   obs;
	Status        status;

	obs.m_pRootNode            = this;
	obs.m_bTrimValueWhiteSpace = bTrimValueWhiteSpace;
	if ((status = parser.AddObserver(&obs)).IsNOK())
	{
		return status;
	}
	if ((status = parser.BeginParse()).IsNOK())
	{
		return status;
	}
	if ((status = parser.ParseStream(pInputStream)).IsNOK())
	{
		return status;
	}
	if ((status = parser.EndParse()).IsNOK())
	{
		return status;
	}

	return obs.m_status;
}

Status Node::LoadFromString(const String &sString, bool bTrimValueWhiteSpace/* = true*/)
{
	return LoadFromString(sString.c_str(), sString.size(), bTrimValueWhiteSpace);
}

Status Node::LoadFromString(const Char *pString, Size cLen/* = (Size)-1*/, bool bTrimValueWhiteSpace/* = true*/)
{
	IO::MemInputStream mis(pString, cLen);
	SAXParser          parser;
	SAXObserver        obs;
	Status             status;

	obs.m_pRootNode            = this;
	obs.m_bTrimValueWhiteSpace = bTrimValueWhiteSpace;
	if ((status = parser.AddObserver(&obs)).IsNOK())
	{
		return status;
	}
	if ((status = parser.BeginParse()).IsNOK())
	{
		return status;
	}
	if ((status = parser.ParseStream(&mis)).IsNOK())
	{
		return status;
	}
	if ((status = parser.EndParse()).IsNOK())
	{
		return status;
	}

	return obs.m_status;
}

Status Node::SaveToStream(const Char *szPath) const
{
	IO::FileOutputStream fos(szPath);

	if (!fos.IsOK())
	{
		return Status(Status_CreateFailed, "Failed to create file '{1}'", szPath);
	}

	return SaveToStream(&fos);
}

Status Node::SaveToStream(IO::IOutputStream *pOutputStream) const
{
	return Write(pOutputStream);
}

Status Node::SaveToString(String &sString) const
{
	return Write(&sString);
}

Node &Node::operator = (const Node &node)
{
	Copy(node);

	return *this;
}

bool Node::operator == (const Node &node)
{
	if (m_mapAttrs.size() != node.m_mapAttrs.size())
	{
		return false;
	}
	if (m_vectorNodes.size() != node.m_vectorNodes.size())
	{
		return false;
	}
	if (m_sName != node.m_sName)
	{
		return false;
	}
	if (m_sValue != node.m_sValue)
	{
		return false;
	}
	for (AttrsMap::const_iterator iter = m_mapAttrs.begin(); iter != m_mapAttrs.end(); ++iter)
	{
		AttrsMap::const_iterator iterEx = node.m_mapAttrs.find(iter->first);

		if (node.m_mapAttrs.end() == iterEx)
		{
			return false;
		}
		if (iter->second != iterEx->second)
		{
			return false;
		}
	}

	Set<Size>::Type   setItems;

	Size cCount = m_vectorNodes.size();

	for (Size i = 0; i < cCount; i++)
	{
		bool bFound = false;

		for (Size j = 0; j < cCount; j++)
		{
			if (setItems.end() == setItems.find(j))
			{
				if (*m_vectorNodes[i] == *node.m_vectorNodes[j])
				{
					bFound = true;
					setItems.insert(j);

					break;
				}
			}
		}
		if (!bFound)
		{
			return false;
		}
	}

	return true;
}

bool Node::operator != (const Node &node)
{
	return !(*this == node);
}

Node &Node::operator [] (Size cIndex)
{
	Node *pNode = GetChild(cIndex);

	if (NULL != pNode)
	{
		return *pNode;
	}
	else
	{
		return *AddChild("", "");
	}
}

const Node &Node::operator [] (Size cIndex) const
{
	static const Node node;

	const Node *pNode = GetChild(cIndex);

	if (NULL != pNode)
	{
		return *pNode;
	}

	return node;
}

const String &Node::operator () (const String &sName) const
{
	return GetAttrValue(sName);
}

String &Node::operator () (const String &sName)
{
	AttrsMap::iterator iter = m_mapAttrs.find(sName);

	if (m_mapAttrs.end() != iter)
	{
		return iter->second;
	}
	else
	{
		m_mapAttrs[sName] = "";

		return m_mapAttrs[sName];
	}
}

void Node::SAXObserver::OnBeginParse()
{
	m_pCrNode = NULL;
	m_status.Clear();
}

void Node::SAXObserver::OnEndParse()
{
}

void Node::SAXObserver::OnStartElement(const Char *szName, const AttrsVector *pVectorAttrs)
{
	Node *pNode;

	if (NULL == m_pCrNode)
	{
		m_pCrNode = m_pRootNode;
		pNode     = m_pCrNode;
	}
	else
	{
		pNode     = m_pCrNode->AddChild(szName, "");
		m_pCrNode = pNode;
	}
	for (AttrsVector::const_iterator iter = pVectorAttrs->begin(); iter != pVectorAttrs->end(); ++iter)
	{
		pNode->m_mapAttrs[iter->sName] = iter->sValue;
	}
	pNode->SetName(szName);
}

void Node::SAXObserver::OnEndElement(const Char *szName, const Char *szText)
{
	CX_UNUSED(szName);

	if (NULL == m_pCrNode || NULL == szText)
	{
		return;
	}
	if (m_bTrimValueWhiteSpace)
	{
		const Char *pszStart;

		pszStart = szText;
		while (cx_isspace(*pszStart))
		{
			pszStart++;
		}
		if (0 != *pszStart)
		{
			const Char *pszStop;
			const Char *pszLast;

			pszLast = NULL;
			pszStop = pszStart;
			while (0 != *pszStop)
			{
				if (cx_isspace(*pszStop))
				{
					if (NULL == pszLast)
					{
						pszLast = pszStop;
					}
				}
				else
				{
					if (NULL != pszLast)
					{
						pszLast = NULL;
					}
				}
				pszStop++;
			}
			if (NULL == pszLast)
			{
				m_pCrNode->SetValue(pszStart);
			}
			else
			{
				String sValue(pszStart, pszLast - pszStart);

				m_pCrNode->SetValue(sValue);
			}
		}
	}
	else
	{
		m_pCrNode->SetValue(szText);
	}
	m_pCrNode = m_pCrNode->m_pParent;
}

}//namespace XML

}//namespace Data

}//namespace CX

