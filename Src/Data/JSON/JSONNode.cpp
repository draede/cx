/* 
 * CX - C++ framework for general purpose development
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

#include "CX/Data/JSON/Node.hpp"
#include "CX/IO/FileInputStream.hpp"
#include "CX/IO/FileOutputStream.hpp"
#include "CX/IO/MemInputStream.hpp"
#include "CX/Set.hpp"
#include "CX/Print.hpp"


namespace CX
{

namespace Data
{

namespace JSON
{

Node::Node(const Node &node)
{
	m_pParent = NULL;
	m_nType   = Type_Null;
	Copy(node);
}

Node::Node(Type nType/* = Type_Null*/)
{
	m_pParent = NULL;
	m_nType   = Type_Null;
	SetType(nType);
}

Node::Node(BoolType bValue)
{
	m_pParent = NULL;
	m_nType   = Type_Null;
	SetBool(bValue);
}

Node::Node(IntType nValue)
{
	m_pParent = NULL;
	m_nType   = Type_Null;
	SetInt(nValue);
}

Node::Node(DoubleType lfValue)
{
	m_pParent = NULL;
	m_nType   = Type_Null;
	SetDouble(lfValue);
}

Node::Node(const StringType &sValue)
{
	m_pParent = NULL;
	m_nType   = Type_Null;
	SetString(sValue);
}

Node::~Node()
{
	FreeMem();
}

Node &Node::INVALID_NODE()
{
	static Node node;

	node.m_nType = Type_Invalid;

	return node;
}

void Node::FreeMem()
{
	if (Type_String == m_nType)
	{
		delete m_psValue;
	}
	else
	if (Type_Object == m_nType)
	{
		for (NodesMap::iterator iter = m_pMapNodes->begin(); iter != m_pMapNodes->end(); ++iter)
		{
			delete iter->second;
		}
		delete m_pMapNodes;
	}
	else
	if (Type_Array == m_nType)
	{
		for (NodesVector::iterator iter = m_pVectorNodes->begin(); iter != m_pVectorNodes->end(); ++iter)
		{
			delete *iter;
		}
		delete m_pVectorNodes;
	}
	m_nType = Type_Null;
}

Status Node::Copy(const Node &node)
{
	Status status;

	if (Type_Null == node.GetType())
	{
		return SetNull();
	}
	else
	if (Type_Bool == node.GetType())
	{
		return SetBool(node.GetBool());
	}
	else
	if (Type_Int == node.GetType())
	{
		return SetInt(node.GetInt());
	}
	else
	if (Type_Double == node.GetType())
	{
		return SetDouble(node.GetDouble());
	}
	else
	if (Type_String == node.GetType())
	{
		return SetString(node.GetString());
	}
	else
	if (Type_Object == node.GetType())
	{
		if ((status = SetObject()).IsNOK())
		{
			return status;
		}
		for (NodesMap::const_iterator iter = node.m_pMapNodes->begin(); iter != node.m_pMapNodes->end(); ++iter)
		{
			Node *pNode;

			if (NULL == (pNode = AddMember(iter->first)))
			{
				return Status_MemAllocFailed;
			}
			*pNode = *iter->second;
		}
		
		return Status();
	}
	else
	if (Type_Array == node.GetType())
	{
		if ((status = SetArray()).IsNOK())
		{
			return status;
		}
		for (NodesVector::const_iterator iter = node.m_pVectorNodes->begin(); iter != node.m_pVectorNodes->end(); ++iter)
		{
			Node *pNode;

			if (NULL == (pNode = AddItem()))
			{
				return Status_MemAllocFailed;
			}
			*pNode = **iter;
		}

		return Status();
	}
	else
	{
		return Status_NotSupported;
	}
}

Node::Type Node::GetType() const
{
	return m_nType;
}

Node &Node::GetParent()
{
	if (NULL != m_pParent)
	{
		return *m_pParent;
	}
	else
	{
		return INVALID_NODE();
	}
}

const Node &Node::GetParent() const
{
	if (NULL != m_pParent)
	{
		return *m_pParent;
	}
	else
	{
		return INVALID_NODE();
	}
}

bool Node::HasParent() const
{
	return (NULL != m_pParent);
}

bool Node::IsInvalid() const
{
	return (Type_Invalid == m_nType);
}

bool Node::IsNull() const
{
	return (Type_Null == m_nType);
}

bool Node::IsBool() const
{
	return (Type_Bool == m_nType);
}

bool Node::IsInt() const
{
	return (Type_Int == m_nType);
}

bool Node::IsDouble() const
{
	return (Type_Double == m_nType);
}

bool Node::IsObject() const
{
	return (Type_Object == m_nType);
}

bool Node::IsArray() const
{
	return (Type_Array == m_nType);
}

Status Node::SetType(Type nType)
{
	if (Type_Null == nType)
	{
		return SetNull();
	}
	else
	if (Type_Bool == nType)
	{
		return SetBool(BOOL_DEFAULT);
	}
	else
	if (Type_Int == nType)
	{
		return SetInt(INT_DEFAULT);
	}
	else
	if (Type_Double == nType)
	{
		return SetDouble(DOUBLE_DEFAULT());
	}
	else
	if (Type_String == nType)
	{
		return SetString(STRING_DEFAULT());
	}
	else
	if (Type_Object == nType)
	{
		return SetObject();
	}
	else
	if (Type_Array == nType)
	{
		return SetArray();
	}
	else
	{
		return Status_NotSupported;
	}
}

Status Node::SetNull()
{
	if (Type_Invalid == m_nType)
	{
		return Status_InvalidCall;
	}
	FreeMem();
	m_nType = Type_Null;

	return Status();
}

Status Node::SetBool(BoolType bValue)
{
	if (Type_Invalid == m_nType)
	{
		return Status_InvalidCall;
	}
	FreeMem();
	m_nType  = Type_Bool;
	m_bValue = bValue;

	return Status();
}

Node::BoolType Node::GetBool(Status *pStatus/* = NULL*/) const
{
	if (Type_Bool == m_nType)
	{
		return m_bValue;
	}
	else
	{
		if (NULL != pStatus)
		{
			pStatus->Set(Status_InvalidCall, "");
		}

		return BOOL_DEFAULT;
	}
}

Status Node::SetInt(IntType nValue)
{
	if (Type_Invalid == m_nType)
	{
		return Status_InvalidCall;
	}
	FreeMem();
	m_nType  = Type_Int;
	m_nValue = nValue;

	return Status();
}

Node::IntType Node::GetInt(Status *pStatus/* = NULL*/) const
{
	if (Type_Int == m_nType)
	{
		return m_nValue;
	}
	else
	{
		if (NULL != pStatus)
		{
			pStatus->Set(Status_InvalidCall, "");
		}

		return INT_DEFAULT;
	}
}

Status Node::SetDouble(DoubleType lfValue)
{
	if (Type_Invalid == m_nType)
	{
		return Status_InvalidCall;
	}
	FreeMem();
	m_nType   = Type_Double;
	m_lfValue = lfValue;

	return Status();
}

Node::DoubleType Node::GetDouble(Status *pStatus/* = NULL*/) const
{
	if (Type_Double == m_nType)
	{
		return m_lfValue;
	}
	else
	{
		if (NULL != pStatus)
		{
			pStatus->Set(Status_InvalidCall, "");
		}

		return DOUBLE_DEFAULT();
	}
}

Status Node::SetString(const StringType &sValue)
{
	if (Type_Invalid == m_nType)
	{
		return Status_InvalidCall;
	}
	if (Type_String == m_nType)
	{
		*m_psValue = sValue;
	}
	else
	{
		String *psValue;

		if (NULL == (psValue = new (std::nothrow) StringType(sValue)))
		{
			return Status_MemAllocFailed;
		}
		FreeMem();
		m_psValue = psValue;
		m_nType   = Type_String;
	}

	return Status();
}

const Node::StringType &Node::GetString(Status *pStatus/* = NULL*/) const
{
	if (Type_String == m_nType)
	{
		return *m_psValue;
	}
	else
	{
		if (NULL != pStatus)
		{
			pStatus->Set(Status_InvalidCall, "");
		}

		return STRING_DEFAULT();
	}
}

Status Node::SetObject()
{
	if (Type_Invalid == m_nType)
	{
		return Status_InvalidCall;
	}
	if (Type_Object == m_nType)
	{
		m_pMapNodes->clear();
	}
	else
	{
		NodesMap *pMapNodes;

		if (NULL == (pMapNodes = new (std::nothrow) NodesMap()))
		{
			return Status_MemAllocFailed;
		}
		FreeMem();
		m_pMapNodes = pMapNodes;
		m_nType     = Type_Object;
	}

	return Status();
}

Status Node::SetArray()
{
	if (Type_Invalid == m_nType)
	{
		return Status_InvalidCall;
	}
	if (Type_Array == m_nType)
	{
		m_pVectorNodes->clear();
	}
	else
	{
		NodesVector *pVectorNodes;

		if (NULL == (pVectorNodes = new (std::nothrow) NodesVector()))
		{
			return Status_MemAllocFailed;
		}
		FreeMem();
		m_pVectorNodes = pVectorNodes;
		m_nType        = Type_Array;
	}

	return Status();
}

Size Node::GetMembersCount() const
{
	if (Type_Object == m_nType)
	{
		return m_pMapNodes->size();
	}
	else
	{
		return 0;
	}
}

Node &Node::GetMember(const String &sName)
{
	if (Type_Object == m_nType)
	{
		NodesMap::iterator iter = m_pMapNodes->find(sName);

		if (m_pMapNodes->end() != iter)
		{
			return *iter->second;
		}
	}

	return INVALID_NODE();
}

const Node &Node::GetMember(const String &sName) const
{
	if (Type_Object == m_nType)
	{
		NodesMap::iterator iter = m_pMapNodes->find(sName);

		if (m_pMapNodes->end() != iter)
		{
			return *iter->second;
		}
	}

	return INVALID_NODE();
}

Node &Node::GetMember(Size cIndex)
{
	if (Type_Object == m_nType)
	{
		if (cIndex < m_pMapNodes->size())
		{
			NodesMap::iterator iter = m_pMapNodes->begin();

			while (0 < cIndex)
			{
				cIndex--;
				iter++;
			}

			return *iter->second;
		}
	}

	return INVALID_NODE();
}

const Node &Node::GetMember(Size cIndex) const
{
	if (Type_Object == m_nType)
	{
		if (cIndex < m_pMapNodes->size())
		{
			NodesMap::iterator iter = m_pMapNodes->begin();

			while (0 < cIndex)
			{
				cIndex--;
				iter++;
			}

			return *iter->second;
		}
	}

	return INVALID_NODE();
}

Node *Node::AddMember(const String &sName)
{
	Status status;

	if (Type_Object == m_nType)
	{
		NodesMap::iterator iter = m_pMapNodes->find(sName);

		if (m_pMapNodes->end() != iter)
		{
			return iter->second;
		}
		else
		{
			Node *pNode;

			if (NULL == (pNode = new (std::nothrow) Node()))
			{
				return NULL;
			}
			(*m_pMapNodes)[sName] = pNode;
			pNode->m_pParent = this;

			return pNode;
		}
	}
	else
	{
		return NULL;
	}
}

Status Node::RemoveMember(const String &sName)
{
	Status status;

	if (Type_Object == m_nType)
	{
		NodesMap::iterator iter = m_pMapNodes->find(sName);

		if (m_pMapNodes->end() != iter)
		{
			delete iter->second;
			m_pMapNodes->erase(iter);

			return Status();
		}
		else
		{
			return Status_NotFound;
		}
	}
	else
	{
		return Status_InvalidCall;
	}
}

Status Node::RemoveAllMembers()
{
	Status status;

	if (Type_Object == m_nType)
	{
		for (NodesMap::iterator iter = m_pMapNodes->begin(); iter != m_pMapNodes->end(); ++iter)
		{
			delete iter->second;
		}
		m_pMapNodes->clear();

		return Status();
	}
	else
	{
		return Status_InvalidCall;
	}
}

Size Node::GetItemsCount() const
{
	if (Type_Array == m_nType)
	{
		return m_pVectorNodes->size();
	}
	else
	{
		return 0;
	}
}

Node &Node::GetItem(Size cIndex)
{
	if (Type_Array == m_nType)
	{
		if (m_pVectorNodes->size() > cIndex)
		{
			return *(*m_pVectorNodes)[cIndex];
		}
	}

	return INVALID_NODE();
}

const Node &Node::GetItem(Size cIndex) const
{
	if (Type_Array == m_nType)
	{
		if (m_pVectorNodes->size() > cIndex)
		{
			return *(*m_pVectorNodes)[cIndex];
		}
	}

	return INVALID_NODE();
}

Node *Node::AddItem()
{
	Status status;

	if (Type_Array == m_nType)
	{
		Node *pNode;

		if (NULL == (pNode = new (std::nothrow) Node()))
		{
			return NULL;
		}
		m_pVectorNodes->push_back(pNode);
		pNode->m_pParent = this;

		return pNode;
	}
	else
	{
		return NULL;
	}
}

Node *Node::InsertItem(Size cAtIndex)
{
	Status status;

	if (Type_Array == m_nType)
	{
		if (m_pVectorNodes->size() <= cAtIndex)
		{
			return AddItem();
		}
		else
		{
			Node *pNode;

			if (NULL == (pNode = new (std::nothrow) Node()))
			{
				return NULL;
			}

			NodesVector::iterator iter = m_pVectorNodes->begin();

			iter += cAtIndex;

			m_pVectorNodes->insert(iter, pNode);
			pNode->m_pParent = this;

			return pNode;
		}
	}
	else
	{
		return NULL;
	}
}

Status Node::RemoveItem(Size cIndex)
{
	Status status;

	if (Type_Array == m_nType)
	{
		if (m_pVectorNodes->size() > cIndex)
		{
			NodesVector::iterator iter = m_pVectorNodes->begin();

			iter += cIndex;
			delete *iter;
			m_pVectorNodes->erase(iter);

			return Status();
		}
		else
		{
			return Status_NotFound;
		}
	}
	else
	{
		return Status_InvalidCall;
	}
}

Status Node::RemoveAllItems()
{
	Status status;

	if (Type_Array == m_nType)
	{
		for (NodesVector::iterator iter = m_pVectorNodes->begin(); iter != m_pVectorNodes->end(); ++iter)
		{
			delete *iter;
		}
		m_pVectorNodes->clear();

		return Status();
	}
	else
	{
		return Status_InvalidCall;
	}
}

Status Node::LoadFromStream(const Char *szPath)
{
	IO::FileInputStream fis(szPath);

	if (!fis.IsOK())
	{
		return Status(Status_OpenFailed, "Failed to open '{1}'", szPath);
	}

	return LoadFromStream(&fis);
}

Status Node::LoadFromStream(IO::IInputStream *pInputStream)
{
	SAXParser     parser;
	SAXObserver   obs;
	Status        status;

	obs.m_pRootNode = this;
	if ((status = parser.AddObserver(&obs)).IsNOK())
	{
		return status;
	}
	if ((status = parser.ParseStream(pInputStream)).IsNOK())
	{
		return status;
	}

	return obs.m_status;
}

Status Node::LoadFromString(const String &sString)
{
	return LoadFromString(sString.c_str(), sString.size());
}

Status Node::LoadFromString(const Char *pString, Size cLen/* = (Size)-1*/)
{
	IO::MemInputStream mis(pString, cLen);
	SAXParser          parser;
	SAXObserver        obs;
	Status             status;

	obs.m_pRootNode = this;
	if ((status = parser.AddObserver(&obs)).IsNOK())
	{
		return status;
	}
	if ((status = parser.ParseStream(&mis)).IsNOK())
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

Node &Node::operator = (BoolType bValue)
{
	SetBool(bValue);

	return *this;
}

Node &Node::operator = (IntType nValue)
{
	SetInt(nValue);

	return *this;
}

Node &Node::operator = (DoubleType lfValue)
{
	SetDouble(lfValue);

	return *this;
}

Node &Node::operator = (const StringType &sValue)
{
	SetString(sValue);

	return *this;
}

Node &Node::operator = (const Char *szValue)
{
	SetString(szValue);

	return *this;
}

bool Node::operator == (const Node &node)
{
	if (m_nType != node.m_nType)
	{
		return false;
	}
	if (Type_Null == m_nType)
	{
		return true;
	}
	else
	if (Type_Invalid == m_nType)
	{
		return true;
	}
	else
	if (Type_Bool == m_nType)
	{
		return (m_bValue == node.m_bValue);
	}
	else
	if (Type_Int == m_nType)
	{
		return (m_nValue == node.m_nValue);
	}
	else
	if (Type_Double == m_nType)
	{
		return (m_lfValue == node.m_lfValue);
	}
	else
	if (Type_String == m_nType)
	{
		return (*m_psValue == *node.m_psValue);
	}
	else
	if (Type_Object == m_nType)
	{
		if (m_pMapNodes->size() != node.m_pMapNodes->size())
		{
			return false;
		}
		for (NodesMap::const_iterator iter = m_pMapNodes->begin(); iter != m_pMapNodes->end(); iter++)
		{
			NodesMap::const_iterator iterEx = node.m_pMapNodes->find(iter->first);

			if (node.m_pMapNodes->end() == iterEx)
			{
				return false;
			}
			if (*iterEx->second != *iter->second)
			{
				return false;
			}
		}
		for (NodesMap::const_iterator iter = node.m_pMapNodes->begin(); iter != node.m_pMapNodes->end(); iter++)
		{
			NodesMap::const_iterator iterEx = m_pMapNodes->find(iter->first);

			if (m_pMapNodes->end() == iterEx)
			{
				return false;
			}
			if (*iterEx->second != *iter->second)
			{
				return false;
			}
		}

		return true;
	}
	else
	if (Type_Array == m_nType)
	{
		Set<Size>::Type   setItems;

		if (m_pVectorNodes->size() != node.m_pVectorNodes->size())
		{
			return false;
		}

		Size cCount = m_pVectorNodes->size();

		for (Size i = 0; i < cCount; i++)
		{
			bool bFound = false;

			for (Size j = 0; j < cCount; j++)
			{
				if (setItems.end() == setItems.find(j))
				{
					if (*(*m_pVectorNodes)[i] == *(*node.m_pVectorNodes)[j])
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
	else
	{
		return false;
	}
}

bool Node::operator == (BoolType bValue)
{
	if (Type_Bool != m_nType)
	{
		return false;
	}

	return (m_bValue == bValue);
}

bool Node::operator == (IntType nValue)
{
	if (Type_Int != m_nType)
	{
		return false;
	}

	return (m_nValue == nValue);
}

bool Node::operator == (DoubleType lfValue)
{
	if (Type_Double != m_nType)
	{
		return false;
	}

	return (m_lfValue == lfValue);
}

bool Node::operator == (const StringType &sValue)
{
	if (Type_String != m_nType)
	{
		return false;
	}

	return (*m_psValue == sValue);
}

bool Node::operator == (const Char *szValue)
{
	if (Type_String != m_nType)
	{
		return false;
	}

	return (*m_psValue == szValue);
}

bool Node::operator != (const Node &node)
{
	return (!(*this == node));
}

bool Node::operator != (BoolType bValue)
{
	return (!(*this == bValue));
}

bool Node::operator != (IntType nValue)
{
	return (!(*this == nValue));
}

bool Node::operator != (DoubleType lfValue)
{
	return (!(*this == lfValue));
}

bool Node::operator != (const StringType &sValue)
{
	return (!(*this == sValue));
}

bool Node::operator != (const CharType *szValue)
{
	return (!(*this == szValue));
}

Node::operator BoolType ()
{
	return GetBool();
}

Node::operator IntType ()
{
	return GetInt();
}

Node::operator DoubleType ()
{
	return GetDouble();
}

Node &Node::operator [] (const String &sName)
{
	if (Type_Object == m_nType)
	{
		Node &node = GetMember(sName);

		if (node.IsInvalid())
		{
			Node *pNode;

			if (NULL != (pNode = AddMember(sName)))
			{
				return *pNode;
			}
		}
		else
		{
			return node;
		}
	}

	return INVALID_NODE();
}

const Node &Node::operator [] (const String &sName) const
{
	return GetMember(sName);
}

Node &Node::operator [] (const CharType *szName)
{
	if (Type_Object == m_nType)
	{
		Node &node = GetMember(szName);

		if (node.IsInvalid())
		{
			Node *pNode;

			if (NULL != (pNode = AddMember(szName)))
			{
				return *pNode;
			}
		}
		else
		{
			return node;
		}

	}

	return INVALID_NODE();
}

const Node &Node::operator [] (const CharType *szName) const
{
	return GetMember(szName);
}

Node &Node::operator [] (int cIndex)
{
	if (Type_Array == m_nType && -1 <= cIndex)
	{
		Node &node = GetItem(cIndex);

		if (node.IsInvalid())
		{
			Node *pNode;

			if (NULL != (pNode = AddItem()))
			{
				return *pNode;
			}
		}
		else
		{
			return node;
		}
	}

	return INVALID_NODE();
}

const Node &Node::operator [] (int cIndex) const
{
	return GetItem(cIndex);
}

Bool Node::SAXObserver::OnBeginParse()
{
	m_pCrNode = NULL;
	m_status.Clear();

	return True;
}

Bool Node::SAXObserver::OnEndParse()
{
	return True;
}

Bool Node::SAXObserver::OnBeginObject()
{
	Status status;

	if (m_status.IsNOK())
	{
		return False;
	}
	if (NULL == m_pCrNode)
	{
		if ((status = m_pRootNode->SetObject()).IsNOK())
		{
			m_status = status;

			return False;
		}
		m_pCrNode = m_pRootNode;
	}
	else
	{
		Node *pNode;

		if (m_pCrNode->IsObject())
		{
			if (NULL == (pNode = m_pCrNode->AddMember(m_sKey)))
			{
				m_status = Status_MemAllocFailed;

				return False;
			}
		}
		else
		if (m_pCrNode->IsArray())
		{
			if (NULL == (pNode = m_pCrNode->AddItem()))
			{
				m_status = Status_MemAllocFailed;

				return False;
			}
		}
		else
		{
			m_status = Status(Status_InvalidArg, "Parent node must be an object or an array");
		
			return False;
		}
		if ((status = pNode->SetObject()).IsNOK())
		{
			m_status = status;

			return False;
		}
		m_pCrNode = pNode;
	}

	return True;
}

Bool Node::SAXObserver::OnEndObject()
{
	if (m_status.IsNOK())
	{
		return False;
	}
	if (NULL == m_pCrNode)
	{
		m_status = Status(Status_InvalidArg, "Unexpected end");

		return False;
	}
	else
	{
		if (!m_pCrNode->IsObject())
		{
			m_status = Status(Status_InvalidArg, "Unexpected end of not object");

			return False;
		}
		if (NULL != m_pCrNode->m_pParent)
		{
			m_pCrNode = m_pCrNode->m_pParent;
		}
		else
		{
			m_pCrNode = NULL;
		}
	}

	return True;
}

Bool Node::SAXObserver::OnBeginArray()
{
	Status status;

	if (m_status.IsNOK())
	{
		return False;
	}
	if (NULL == m_pCrNode)
	{
		if ((status = m_pRootNode->SetArray()).IsNOK())
		{
			m_status = status;

			return False;
		}
		m_pCrNode = m_pRootNode;
	}
	else
	{
		Node *pNode;

		if (m_pCrNode->IsObject())
		{
			if (NULL == (pNode = m_pCrNode->AddMember(m_sKey)))
			{
				m_status = Status_MemAllocFailed;

				return False;
			}
		}
		else
		if (m_pCrNode->IsArray())
		{
			if (NULL == (pNode = m_pCrNode->AddItem()))
			{
				m_status = Status_MemAllocFailed;

				return False;
			}
		}
		else
		{
			m_status = Status(Status_InvalidArg, "Parent node must be an object or an array");
		
			return False;
		}
		if ((status = pNode->SetArray()).IsNOK())
		{
			m_status = status;

			return False;
		}
		m_pCrNode = pNode;
	}

	return True;
}

Bool Node::SAXObserver::OnEndArray()
{
	if (m_status.IsNOK())
	{
		return False;
	}
	if (NULL == m_pCrNode)
	{
		m_status = Status(Status_InvalidArg, "Unexpected end");

		return False;
	}
	else
	{
		if (!m_pCrNode->IsArray())
		{
			m_status = Status(Status_InvalidArg, "Unexpected end of not array");

			return False;
		}
		if (NULL != m_pCrNode->m_pParent)
		{
			m_pCrNode = m_pCrNode->m_pParent;
		}
		else
		{
			m_pCrNode = NULL;
		}
	}

	return True;
}

Bool Node::SAXObserver::OnKey(const Char *pBuffer, Size cLen)
{
	if (m_status.IsNOK())
	{
		return False;
	}

	m_sKey.assign(pBuffer, cLen);

	return True;
}

Bool Node::SAXObserver::OnNullValue()
{
	Status status;

	if (m_status.IsNOK())
	{
		return False;
	}
	if (NULL == m_pCrNode)
	{
		m_status = Status(Status_InvalidArg, "Root node must be an object or an array");

		return False;
	}
	if (m_pCrNode->IsObject())
	{
		Node *pNode;

		if (NULL == (pNode = m_pCrNode->AddMember(m_sKey)))
		{
			m_status = Status_MemAllocFailed;

			return False;
		}
		if ((status = pNode->SetNull()).IsNOK())
		{
			m_status = status;
			
			return False;
		}

		return True;
	}
	else
	if (m_pCrNode->IsArray())
	{
		Node *pNode;

		if (NULL == (pNode = m_pCrNode->AddItem()))
		{
			m_status = Status_MemAllocFailed;

			return False;
		}
		if ((status = pNode->SetNull()).IsNOK())
		{
			m_status = status;

			return False;
		}

		return True;
	}
	else
	{
		m_status = Status(Status_InvalidArg, "Parent must be an object or an array");

		return False;
	}

	return True;

}

Bool Node::SAXObserver::OnBoolValue(Bool bBool)
{
	Status status;

	if (m_status.IsNOK())
	{
		return False;
	}
	if (NULL == m_pCrNode)
	{
		m_status = Status(Status_InvalidArg, "Root node must be an object or an array");

		return False;
	}
	if (m_pCrNode->IsObject())
	{
		Node *pNode;

		if (NULL == (pNode = m_pCrNode->AddMember(m_sKey)))
		{
			m_status = Status_MemAllocFailed;

			return False;
		}
		if ((status = pNode->SetBool(bBool)).IsNOK())
		{
			m_status = status;
			
			return False;
		}

		return True;
	}
	else
	if (m_pCrNode->IsArray())
	{
		Node *pNode;

		if (NULL == (pNode = m_pCrNode->AddItem()))
		{
			m_status = Status_MemAllocFailed;

			return False;
		}
		if ((status = pNode->SetBool(bBool)).IsNOK())
		{
			m_status = status;

			return False;
		}

		return True;
	}
	else
	{
		m_status = Status(Status_InvalidArg, "Parent must be an object or an array");

		return False;
	}

	return True;

}

Bool Node::SAXObserver::OnIntValue(Int64 nInt)
{
	Status status;

	if (m_status.IsNOK())
	{
		return False;
	}
	if (NULL == m_pCrNode)
	{
		m_status = Status(Status_InvalidArg, "Root node must be an object or an array");

		return False;
	}
	if (m_pCrNode->IsObject())
	{
		Node *pNode;

		if (NULL == (pNode = m_pCrNode->AddMember(m_sKey)))
		{
			m_status = Status_MemAllocFailed;

			return False;
		}
		if ((status = pNode->SetInt(nInt)).IsNOK())
		{
			m_status = status;
			
			return False;
		}

		return True;
	}
	else
	if (m_pCrNode->IsArray())
	{
		Node *pNode;

		if (NULL == (pNode = m_pCrNode->AddItem()))
		{
			m_status = Status_MemAllocFailed;

			return False;
		}
		if ((status = pNode->SetInt(nInt)).IsNOK())
		{
			m_status = status;

			return False;
		}

		return True;
	}
	else
	{
		m_status = Status(Status_InvalidArg, "Parent must be an object or an array");

		return False;
	}

	return True;
}

Bool Node::SAXObserver::OnRealValue(Double lfReal)
{
	Status status;

	if (m_status.IsNOK())
	{
		return False;
	}
	if (NULL == m_pCrNode)
	{
		m_status = Status(Status_InvalidArg, "Root node must be an object or an array");

		return False;
	}
	if (m_pCrNode->IsObject())
	{
		Node *pNode;

		if (NULL == (pNode = m_pCrNode->AddMember(m_sKey)))
		{
			m_status = Status_MemAllocFailed;

			return False;
		}
		if ((status = pNode->SetDouble(lfReal)).IsNOK())
		{
			m_status = status;
			
			return False;
		}

		return True;
	}
	else
	if (m_pCrNode->IsArray())
	{
		Node *pNode;

		if (NULL == (pNode = m_pCrNode->AddItem()))
		{
			m_status = Status_MemAllocFailed;

			return False;
		}
		if ((status = pNode->SetDouble(lfReal)).IsNOK())
		{
			m_status = status;

			return False;
		}

		return True;
	}
	else
	{
		m_status = Status(Status_InvalidArg, "Parent must be an object or an array");

		return False;
	}

	return True;
}

Bool Node::SAXObserver::OnStringValue(const Char *pBuffer, Size cLen)
{
	String sString(pBuffer, cLen);
	Status status;

	if (m_status.IsNOK())
	{
		return False;
	}
	if (NULL == m_pCrNode)
	{
		m_status = Status(Status_InvalidArg, "Root node must be an object or an array");

		return False;
	}
	if (m_pCrNode->IsObject())
	{
		Node *pNode;

		if (NULL == (pNode = m_pCrNode->AddMember(m_sKey)))
		{
			m_status = Status_MemAllocFailed;

			return False;
		}
		if ((status = pNode->SetString(sString)).IsNOK())
		{
			m_status = status;
			
			return False;
		}

		return True;
	}
	else
	if (m_pCrNode->IsArray())
	{
		Node *pNode;

		if (NULL == (pNode = m_pCrNode->AddItem()))
		{
			m_status = Status_MemAllocFailed;

			return False;
		}
		if ((status = pNode->SetString(sString)).IsNOK())
		{
			m_status = status;

			return False;
		}

		return True;
	}
	else
	{
		m_status = Status(Status_InvalidArg, "Parent must be an object or an array");

		return False;
	}

	return True;
}

}//namespace JSON

}//namespace Data

}//namespace CX



