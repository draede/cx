/* 
 * CX - C++ framework for general purpose development
 *
 * https://github.com/draede/cx
 * 
 * Copyright (C) 2014 - 2017 draede - draede [at] outlook [dot] com
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
#include "CX/Data/JSON/SAXParser.hpp"
#include "CX/Print.hpp"


namespace CX
{

namespace Data
{

namespace JSON
{

class Node
{
public:

	enum Type
	{
		Type_Invalid,
		Type_Null,
		Type_Bool,
		Type_Int,
		Type_Double,
		Type_String,
		Type_Object,
		Type_Array,
	};

	typedef bool     BoolType;
	typedef Int64    IntType;
	typedef double   DoubleType;
	typedef String   StringType;
	typedef Char     CharType;

	static const BoolType     BOOL_DEFAULT   = false;
	static const IntType      INT_DEFAULT    = 0;
	static const DoubleType   DOUBLE_DEFAULT() { return 0.0; };
	static const StringType   &STRING_DEFAULT() { static String tmp; return tmp; }

	Node(const Node &node);

	Node(Type nType = Type_Null);

	Node(BoolType bValue);

	Node(IntType nValue);

	Node(DoubleType lfValue);

	Node(const StringType &sValue);

	~Node();

	Status Copy(const Node &node);

	Type GetType() const;

	Node &GetParent();

	const Node &GetParent() const;

	bool HasParent() const;

	bool IsInvalid() const;

	bool IsNull() const;

	bool IsBool() const;

	bool IsInt() const;

	bool IsDouble() const;

	bool IsObject() const;

	bool IsArray() const;

	Status SetType(Type nType);

	Status SetNull();

	Status SetBool(BoolType bValue);

	BoolType GetBool(Status *pStatus = NULL) const;

	Status SetInt(IntType nValue);

	IntType GetInt(Status *pStatus = NULL) const;

	Status SetDouble(DoubleType lfValue);

	DoubleType GetDouble(Status *pStatus = NULL) const;

	Status SetString(const StringType &sValue);

	const StringType &GetString(Status *pStatus = NULL) const;

	Status SetObject();

	Status SetArray();

	Size GetMembersCount() const;

	Node &GetMember(Size cIndex);
	
	const Node &GetMember(Size cIndex) const;

	Node &GetMember(const String &sName);

	const Node &GetMember(const String &sName) const;

	Node *AddMember(const String &sName);

	Status RemoveMember(const String &sName);

	Status RemoveAllMembers();

	Size GetItemsCount() const;

	Node &GetItem(Size cIndex);

	const Node &GetItem(Size cIndex) const;

	Node *AddItem();

	Node *InsertItem(Size cAtIndex);

	Status RemoveItem(Size cIndex);

	Status RemoveAllItems();

	Status LoadFromStream(const Char *szPath);

	Status LoadFromStream(IO::IInputStream *pInputStream);

	Status LoadFromString(const String &sString);

	Status LoadFromString(const Char *pString, Size cLen = (Size)-1);

	Status SaveToStream(const Char *szPath) const;

	Status SaveToStream(IO::IOutputStream *pOutputStream) const;

	Status SaveToString(String &sString) const;

	Node &operator = (const Node &node);

	Node &operator = (BoolType bValue);

	Node &operator = (IntType nValue);

	Node &operator = (DoubleType lfValue);

	Node &operator = (const StringType &sValue);

	Node &operator = (const CharType *szValue);

	bool operator == (const Node &node);

	bool operator == (BoolType bValue);

	bool operator == (IntType nValue);

	bool operator == (DoubleType lfValue);

	bool operator == (const StringType &sValue);

	bool operator == (const CharType *szValue);

	bool operator != (const Node &node);

	bool operator != (BoolType bValue);

	bool operator != (IntType nValue);

	bool operator != (DoubleType lfValue);

	bool operator != (const StringType &sValue);

	bool operator != (const CharType *szValue);

	operator BoolType ();

	operator IntType ();

	operator DoubleType ();

	Node &operator [] (const String &sName);

	const Node &operator [] (const String &sName) const;

	Node &operator [] (const CharType *szName);

	const Node &operator [] (const CharType *szName) const;

	Node &operator [] (int cIndex);

	const Node &operator [] (int cIndex) const;

	template <typename OUTPUT>
	Status Write(OUTPUT out) const
	{
		return WriteHelper(*this, out);
	}

private:

	typedef Map<String, Node *>::Type   NodesMap;
	typedef Vector<Node *>::Type        NodesVector;

	Node *m_pParent;
	Type m_nType;

	union
	{
		BoolType      m_bValue;
		IntType       m_nValue;
		DoubleType    m_lfValue;
		StringType    *m_psValue;
		NodesMap      *m_pMapNodes;
		NodesVector   *m_pVectorNodes;
	};

	void FreeMem();

	static Node &INVALID_NODE();

	template <typename OUTPUT>
	static inline Status WriteHelper(const Node &node, OUTPUT out, Size cIndent = 0)
	{
		Status status;

		if (Type_Null == node.m_nType)
		{
			return Print(out, "null");
		}
		else
		if (Type_Bool == node.m_nType)
		{
			return Print(out, "{1}", node.m_bValue ? "true" : "false");
		}
		else
		if (Type_Int == node.m_nType)
		{
			return Print(out, "{1}", node.m_nValue);
		}
		else
		if (Type_Double == node.m_nType)
		{
			return Print(out, "{1}", node.m_lfValue);
		}
		else
		if (Type_String == node.m_nType)
		{
			String sValue;

			if ((status = SAXParser::EscapeString(node.m_psValue->c_str(), &sValue)).IsNOK())
			{
				return status;
			}

			return Print(out, "\"{1}\"", sValue);
		}
		else
		if (Type_Object == node.m_nType)
		{
			String sIndent(cIndent, '\t');

			if ((status = Print(out, "{1}{{\n", sIndent)).IsNOK())
			{
				return status;
			}
			for (NodesMap::iterator iter = node.m_pMapNodes->begin(); iter != node.m_pMapNodes->end(); ++iter)
			{
				String sName;

				if ((status = SAXParser::EscapeString(iter->first.c_str(), &sName)).IsNOK())
				{
					return status;
				}
				if (Type_Object == iter->second->m_nType || Type_Array == iter->second->m_nType)
				{
					if ((status = Print(out, "{1}\t\"{2}\":\n", sIndent, sName)).IsNOK())
					{
						return status;
					}
				}
				else
				{
					if ((status = Print(out, "{1}\t\"{2}\": ", sIndent, sName)).IsNOK())
					{
						return status;
					}
				}
				if ((status = WriteHelper(*iter->second, out, cIndent + 1)).IsNOK())
				{
					return status;
				}

				NodesMap::iterator iter2 = iter;

				iter2++;

				if ((status = Print(out, "{1}\n", node.m_pMapNodes->end() == iter2 ? "" : ",")).IsNOK())
				{
					return status;
				}
			}
			if ((status = Print(out, "{1}}", sIndent)).IsNOK())
			{
				return status;
			}

			return Status();
		}
		else
		if (Type_Array == node.m_nType)
		{
			String sIndent(cIndent, '\t');

			if ((status = Print(out, "{1}[\n", sIndent)).IsNOK())
			{
				return status;
			}
			for (NodesVector::iterator iter = node.m_pVectorNodes->begin(); iter != node.m_pVectorNodes->end(); ++iter)
			{
				if (Type_Object != (*iter)->m_nType && Type_Array != (*iter)->m_nType)
				{
					if ((status = Print(out, "{1}\t", sIndent)).IsNOK())
					{
						return status;
					}
				}
				if ((status = WriteHelper(**iter, out, cIndent + 1)).IsNOK())
				{
					return status;
				}

				NodesVector::iterator iter2 = iter;

				iter2++;

				if ((status = Print(out, "{1}\n", node.m_pVectorNodes->end() == iter2 ? "" : ",")).IsNOK())
				{
					return status;
				}
			}
			if ((status = Print(out, "{1}]", sIndent)).IsNOK())
			{
				return status;
			}

			return Status();
		}
		else
		{
			return Status_InvalidArg;
		}
	}

	class SAXObserver : public Data::JSON::ISAXParserObserver
	{
	public:

		String m_sKey;
		Node   *m_pRootNode;
		Node   *m_pCrNode;
		Status m_status;

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

		virtual Bool OnRealValue(Double lfReal);

		virtual Bool OnStringValue(const Char *pBuffer, Size cLen);

	};

};

}//namespace JSON

}//namespace Data

}//namespace CX

