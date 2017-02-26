/* 
 * CX - C++ framework for general purpose development
 *
 * https://github.com/draede/cx
 * 
 * Copyright (C) 2014 - 2017 draede, draede [at] outlook [dot] com
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
#include "CX/String.hpp"
#include "CX/Vector.hpp"
#include "CX/Map.hpp"
#include "CX/Stack.hpp"
#include "CX/Print.hpp"
#include "CX/Data/JSON/SAXParser.hpp"
#include "CX/IO/IInputStream.hpp"
#include "CX/IO/IOutputStream.hpp"


namespace CX
{

class Value
{
public:

	static const Bool    DEFAULT_BOOL   = False;
	static const Int64   DEFAULT_INT    = 0;
	static const Double  DEFAULT_REAL;
	static const String  DEFAULT_STRING;

	enum Type
	{
		Type_Invalid,
		Type_Null,
		Type_Bool,
		Type_Int,
		Type_Real,
		Type_String,
		Type_Object,
		Type_Array,
	};

	Value();

	Value(Type nType);

	Value(Bool bValue);

	Value(Int64 nValue);

	Value(Double lfValue);

	Value(const String &sValue);

	Value(const Value &value);

	~Value();

	Value &operator=(const Value &value);

	Status Copy(const Value &value);

	Bool HasParent();

	const Value &GetParent() const;

	Value &GetParent();

	Type GetType() const;

	Bool IsInvalid() const;

	Bool IsNull() const;

	Bool IsBool() const;

	Bool IsInt() const;

	Bool IsReal() const;

	Bool IsString() const;

	Bool IsObject() const;

	Bool IsArray() const;

	Bool GetNull(Status *pStatus = NULL) const;

	Status SetNull();

	Bool GetBool(Bool bDefault = DEFAULT_BOOL, Status *pStatus = NULL) const;

	Status SetBool(Bool bValue);

	Int64 GetInt(Int64 nDefault = DEFAULT_INT, Status *pStatus = NULL) const;

	Status SetInt(Int64 nValue);

	Double GetReal(Double lfDefault = DEFAULT_REAL, Status *pStatus = NULL) const;

	Status SetReal(Double lfValue);

	const String &GetString(const String &sDefault = DEFAULT_STRING, Status *pStatus = NULL) const;

	Status SetString(const String &sValue);

	Status SetAsArray();

	Status SetAsObject();

	Size GetItemsCount(Status *pStatus = NULL) const;

	Value &AddItem(Status *pStatus = NULL);

	Status AddItem(Value *pValue);

	Value &InsertItem(int cIndex, Status *pStatus = NULL);

	Status InsertItem(int cIndex, Value *pValue);

	Status RemoveItem(int cIndex);

	Status RemoveAllItems();

	const Value &GetItem(int cIndex, Status *pStatus = NULL) const;

	Value &GetItem(int cIndex, Status *pStatus = NULL); //-1 will create a new value at the end

	const Value &operator[](int cIndex) const;

	Value &operator[](int cIndex); //a non existing member will be created

	Size GetMembersCount(Status *pStatus = NULL) const;

	Bool Exists(const String &sName, Status *pStatus = NULL) const;

	Value &AddMember(const String &sName, Status *pStatus = NULL);

	Status AddMember(const String &sName, Value *pValue);

	Status RemoveMember(const String &sName);

	Status RemoveAllMembers();

	Value &GetMemberByIndex(Size cIndex, String *psName = NULL, Status *pStatus = NULL);

	const Value &GetMemberByIndex(Size cIndex, String *psName = NULL, Status *pStatus = NULL) const;

	const Value &GetMember(const String &sName, Status *pStatus = NULL) const;

	Value &GetMember(const String &sName, Status *pStatus = NULL); //a non existing member will be created

	const Value &operator[](const String &sName) const;

	Value &operator[](const String &sName); //a non existing member will be created

	const Value &operator[](const Char *szName) const;

	Value &operator[](const Char *szName); //a non existing member will be created

	Value &operator=(Bool bBool);

	Value &operator=(Int64 nInt);

	Value &operator=(Double lfReal);

	Value &operator=(const String &sString);

	Value &operator=(const Char *szString);

	operator Bool () const;

	operator Int64 () const;

	operator Double () const;

	operator const String & () const;

	operator const Char * () const;

	template <typename OUTPUT>
	void Write(OUTPUT output, Size cIndent = 0) const
	{
		WriteNodesStack   stack;

		if (Type_Null == m_nType)
		{
			Print(output, "null");

			return;
		}
		else
		if (Type_Bool == m_nType)
		{
			Print(output, "{1}", m_bBool ? "true" : "false");

			return;
		}
		else
		if (Type_Int == m_nType)
		{
			Print(output, "{1}", m_nInt);

			return;
		}
		else
		if (Type_Real == m_nType)
		{
			Print(output, "{1:.10}", m_lfReal);

			return;
		}
		else
		if (Type_String == m_nType)
		{
			String sValue;

			Data::JSON::SAXParser::EscapeString(m_psString->c_str(), &sValue);

			Print(output, "\"{1}\"", sValue);
			
			return;
		}
		else
		if (Type_Object == m_nType)
		{
			WriteNode node;

			node.pValue     = this;
			node.iterObject = m_pObject->begin();
			node.bFirst     = true;
			stack.push(node);
		}
		else
		if (Type_Array == m_nType)
		{
			WriteNode node;

			node.pValue     = this;
			node.iterArray  = m_pArray->begin();
			node.bFirst     = true;
			stack.push(node);
		}
		else
		{
			return;
		}

		while (!stack.empty())
		{
			WriteNode   &node   = stack.top();

			if (node.pValue->IsObject())
			{
				String sIndent(cIndent, '\t');

				if (node.bFirst)
				{
					Print(output, "{1}{{\n", sIndent);
					node.bFirst = false;
					cIndent++;
					sIndent += '\t';
				}
				else
				{
					if (node.pValue->m_pObject->empty())
					{
						Print(output, "\n");
					}
					else
					{
						if (node.iterObject != node.pValue->m_pObject->end())
						{
							Print(output, ",\n");
						}
						else
						{
							Print(output, "\n");
						}
					}
				}
				if (node.iterObject != node.pValue->m_pObject->end())
				{
					String sName;

					Data::JSON::SAXParser::EscapeString(node.iterObject->first.c_str(), &sName);
					if (Type_Object == node.iterObject->second->m_nType || Type_Array == node.iterObject->second->m_nType)
					{
						Print(output, "{1}\"{2}\": \n", sIndent, sName);
					}
					else
					{
						Print(output, "{1}\"{2}\": ", sIndent, sName);
					}

					if (Type_Null == node.iterObject->second->m_nType)
					{
						Print(output, "null");
						node.iterObject++;
					}
					else
					if (Type_Bool == node.iterObject->second->m_nType)
					{
						Print(output, "{1}", node.iterObject->second->m_bBool ? "true" : "false");
						node.iterObject++;
					}
					else
					if (Type_Int == node.iterObject->second->m_nType)
					{
						Print(output, "{1}", node.iterObject->second->m_nInt);
						node.iterObject++;
					}
					else
					if (Type_Real == node.iterObject->second->m_nType)
					{
						Print(output, "{1:.10}", node.iterObject->second->m_lfReal);
						node.iterObject++;
					}
					else
					if (Type_String == node.iterObject->second->m_nType)
					{
						String sValue;

						Data::JSON::SAXParser::EscapeString(node.iterObject->second->m_psString->c_str(), &sValue);

						Print(output, "\"{1}\"", sValue);
						node.iterObject++;
					}
					else
					if (Type_Object == node.iterObject->second->m_nType)
					{
						WriteNode new_node;

						new_node.pValue     = node.iterObject->second;
						new_node.iterObject = node.iterObject->second->m_pObject->begin();
						new_node.bFirst     = true;
						stack.push(new_node);
						node.iterObject++;
					}
					else
					if (Type_Array == node.iterObject->second->m_nType)
					{
						WriteNode new_node;

						new_node.pValue    = node.iterObject->second;
						new_node.iterArray = node.iterObject->second->m_pArray->begin();
						new_node.bFirst    = true;
						stack.push(new_node);
						node.iterObject++;
					}
					else
					{
						return;
					}
				}
				else //end
				{
					cIndent--;

					String sIndent(cIndent, '\t');

					Print(stdout, "{1}}", sIndent);
					stack.pop();
				}
			}
			else //array
			{
				String sIndent(cIndent, '\t');

				if (node.bFirst)
				{
					Print(output, "{1}[\n", sIndent);
					node.bFirst = false;
					cIndent++;
					sIndent += '\t';
				}
				else
				{
					if (node.pValue->m_pArray->empty())
					{
						Print(output, "\n");
					}
					else
					{
						if (node.iterArray != node.pValue->m_pArray->end())
						{
							Print(output, ",\n");
						}
						else
						{
							Print(output, "\n");
						}
					}
				}
				if (node.iterArray != node.pValue->m_pArray->end())
				{
					if (Type_Object != (*node.iterArray)->m_nType && Type_Array != (*node.iterArray)->m_nType)
					{
						Print(output, "{1}", sIndent);
					}

					if (Type_Null == (*node.iterArray)->m_nType)
					{
						Print(output, "null");
						node.iterArray++;
					}
					else
					if (Type_Bool == (*node.iterArray)->m_nType)
					{
						Print(output, "{1}", (*node.iterArray)->m_bBool ? "true" : "false");
						node.iterArray++;
					}
					else
					if (Type_Int == (*node.iterArray)->m_nType)
					{
						Print(output, "{1}", (*node.iterArray)->m_nInt);
						node.iterArray++;
					}
					else
					if (Type_Real == (*node.iterArray)->m_nType)
					{
						Print(output, "{1:.10}", (*node.iterArray)->m_lfReal);
						node.iterArray++;
					}
					else
					if (Type_String == (*node.iterArray)->m_nType)
					{
						String sValue;

						Data::JSON::SAXParser::EscapeString((*node.iterArray)->m_psString->c_str(), &sValue);

						Print(output, "\"{1}\"", sValue);
						node.iterArray++;
					}
					else
					if (Type_Object == (*node.iterArray)->m_nType)
					{
						WriteNode new_node;

						new_node.pValue     = (*node.iterArray);
						new_node.iterObject = (*node.iterArray)->m_pObject->begin();
						new_node.bFirst     = true;
						stack.push(new_node);
						node.iterArray++;
					}
					else
					if (Type_Array == (*node.iterArray)->m_nType)
					{
						WriteNode new_node;

						new_node.pValue    = (*node.iterArray);
						new_node.iterArray = (*node.iterArray)->m_pArray->begin();
						new_node.bFirst    = true;
						stack.push(new_node);
						node.iterArray++;
					}
					else
					{
						return;
					}
				}
				else //end
				{
					cIndent--;

					String sIndent(cIndent, '\t');

					Print(stdout, "{1}]", sIndent);
					stack.pop();
				}
			}
		}
	}

	Status Read(IO::IInputStream *pInputStream);

	Status Read(const void *pData, Size cbSize);

	Status Read(const String &sData);

	Status Read(const Char *szData);

protected:

	friend class VarJSONSAXParserObserver;

private:

	typedef Map<String, Value *>::Type   Object;
	typedef Vector<Value *>::Type        Array;

	struct WriteNode
	{
		const Value            *pValue;
		bool                   bFirst;
		Object::const_iterator iterObject;
		Array::const_iterator  iterArray;
	};

	typedef Stack<WriteNode>::Type   WriteNodesStack;

	static Value INVALID_VALUE;

	Value(void *pDummy1, void *pDummy2, void *pDummy3);

	Value(Value *pParent);

	Type  m_nType;
	Value *m_pParent;

	union
	{
		Bool           m_bBool;
		Int64          m_nInt;
		Double         m_lfReal;
		String         *m_psString;
		Object         *m_pObject;
		Array          *m_pArray;
	};

	void FreeMem();

};

}//namespace CX
