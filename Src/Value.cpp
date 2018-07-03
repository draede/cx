/* 
 * CX - C++ framework for general purpose development
 *
 * https://github.com/draede/cx
 * 
 * Copyright (C) 2014 - 2018 draede, draede [at] outlook [dot] com
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

#include "CX/Value.hpp"
#include "CX/IO/MemInputStream.hpp"
#include "CX/IO/MemOutputStream.hpp"


namespace CX
{


class VarJSONSAXParserObserver : public Data::JSON::ISAXParserObserver
{
public:

	Bool     m_bInit;
	Value   *m_pRoot;
	Value   *m_pCurrent;
	String   m_sKey;

	virtual Bool OnBeginParse()
	{
		return True;
	}

	virtual Bool OnEndParse()
	{
		return True;
	}

	virtual Bool OnBeginObject()
	{
		if (m_bInit)
		{
			if (m_pCurrent->SetAsObject().IsNOK())
			{
				return False;
			}
			m_bInit = False;
		}
		else
		{
			Value *pValue = new (std::nothrow) Value(Value::Type_Object);

			if (NULL == pValue)
			{
				return False;
			}
			if (m_pCurrent->IsObject())
			{
				m_pCurrent->AddMember(m_sKey, pValue);
				m_pCurrent = pValue;
			}
			else
			{
				m_pCurrent->AddItem(pValue);
				m_pCurrent = pValue;
			}
		}

		return True;
	}

	virtual Bool OnEndObject()
	{
		m_pCurrent = &m_pCurrent->GetParent();

		return True;
	}

	virtual Bool OnBeginArray()
	{
		if (m_bInit)
		{
			if (m_pCurrent->SetAsArray().IsNOK())
			{
				return False;
			}
			m_bInit = False;
		}
		else
		{
			Value *pValue = new (std::nothrow) Value(Value::Type_Array);

			if (NULL == pValue)
			{
				return False;
			}
			if (m_pCurrent->IsObject())
			{
				m_pCurrent->AddMember(m_sKey, pValue);
				m_pCurrent = pValue;
			}
			else
			{
				m_pCurrent->AddItem(pValue);
				m_pCurrent = pValue;
			}
		}

		return True;
	}

	virtual Bool OnEndArray()
	{
		m_pCurrent = &m_pCurrent->GetParent();

		return True;
	}

	virtual Bool OnKey(const Char *pBuffer, Size cLen)
	{
		m_sKey.assign(pBuffer, cLen);

		return True;
	}

	virtual Bool OnNullValue()
	{
		if (m_pCurrent->IsObject())
		{
			(*m_pCurrent)[m_sKey].SetNull();
		}
		else
		{
			(*m_pCurrent)[-1].SetNull();
		}

		return True;
	}

	virtual Bool OnBoolValue(Bool bBool)
	{
		if (m_pCurrent->IsObject())
		{
			(*m_pCurrent)[m_sKey].SetBool(bBool);
		}
		else
		{
			(*m_pCurrent)[-1].SetBool(bBool);
		}

		return True;
	}

	virtual Bool OnIntValue(Int64 nInt)
	{
		if (m_pCurrent->IsObject())
		{
			(*m_pCurrent)[m_sKey].SetInt(nInt);
		}
		else
		{
			(*m_pCurrent)[-1].SetInt(nInt);
		}

		return True;
	}

	virtual Bool OnUIntValue(UInt64 uInt)
	{
		if (m_pCurrent->IsObject())
		{
			(*m_pCurrent)[m_sKey].SetUInt(uInt);
		}
		else
		{
			(*m_pCurrent)[-1].SetUInt(uInt);
		}

		return True;
	}

	virtual Bool OnRealValue(Double lfReal)
	{
		if (m_pCurrent->IsObject())
		{
			(*m_pCurrent)[m_sKey].SetReal(lfReal);
		}
		else
		{
			(*m_pCurrent)[-1].SetReal(lfReal);
		}

		return True;
	}

	virtual Bool OnStringValue(const Char *pBuffer, Size cLen)
	{
		if (m_pCurrent->IsObject())
		{
			(*m_pCurrent)[m_sKey].SetString(String(pBuffer, cLen));
		}
		else
		{
			(*m_pCurrent)[-1].SetString(String(pBuffer, cLen));
		}

		return True;
	}

};

Value         Value::INVALID_VALUE(NULL, NULL, NULL);
const Double  Value::DEFAULT_REAL = 0.0;
const String  Value::DEFAULT_STRING;

Value::Value()
{
	m_nType   = Type_Null;
	m_pParent = NULL;
}

Value::Value(void *pDummy1, void *pDummy2, void *pDummy3)
{
	CX_UNUSED(pDummy1);
	CX_UNUSED(pDummy2);
	CX_UNUSED(pDummy3);

	m_nType   = Type_Invalid;
	m_pParent = NULL;
}

Value::Value(Value *pParent)
{
	m_nType   = Type_Null;
	m_pParent = pParent;
}

Value::Value(Type nType)
{
	m_nType   = Type_Null;
	m_pParent = NULL;

	switch (nType)
	{
		case Type_Bool:    SetBool(DEFAULT_BOOL); break;
		case Type_Int:     SetInt(DEFAULT_INT); break;
		case Type_UInt:    SetUInt(DEFAULT_UINT); break;
		case Type_Real:    SetReal(DEFAULT_REAL); break;
		case Type_String:  SetString(DEFAULT_STRING); break;
		case Type_Object:  SetAsObject(); break;
		case Type_Array:   SetAsArray(); break;
		default:
		{
		}
	}
}

Value::Value(Bool bValue)
{
	m_nType   = Type_Null;
	m_pParent = NULL;
	SetBool(bValue);
}

Value::Value(Int64 nValue)
{
	m_nType   = Type_Null;
	m_pParent = NULL;
	SetInt(nValue);
}

Value::Value(UInt64 uValue)
{
	m_nType   = Type_Null;
	m_pParent = NULL;
	SetUInt(uValue);
}

Value::Value(Double lfValue)
{
	m_nType   = Type_Null;
	m_pParent = NULL;
	SetReal(lfValue);
}

Value::Value(const String &sValue)
{
	m_nType   = Type_Null;
	m_pParent = NULL;
	SetString(sValue);
}

Value::Value(const Value &value)
{
	m_nType = Type_Null;
	Copy(value);
}

Value::~Value()
{
	if (!IsInvalid())
	{
		FreeMem();
		m_nType = Type_Null;
		m_pParent = NULL;
	}
}

void Value::FreeMem()
{
	if (IsInvalid())
	{
		return;
	}

	if (Type_String == m_nType)
	{
		delete m_psString;
	}
	else
	if (Type_Object == m_nType)
	{
		for (Object::iterator iter = m_pObject->begin(); iter != m_pObject->end(); ++iter)
		{
			delete iter->second;
		}
		delete m_pObject;
	}
	else
	if (Type_Array == m_nType)
	{
		for (Array::iterator iter = m_pArray->begin(); iter != m_pArray->end(); ++iter)
		{
			delete *iter;
		}
		delete m_pArray;
	}
}

Value &Value::operator=(const Value &value)
{
	if (IsInvalid())
	{
		return *this;
	}

	Copy(value);

	return *this;
}

Status Value::Copy(const Value &value)
{
	if (IsInvalid())
	{
		return Status_InvalidCall;
	}

	switch (value.m_nType)
	{
		case Type_Null:    return SetNull();
		case Type_Bool:    return SetBool(value.GetBool());
		case Type_Int:     return SetInt(value.GetInt());
		case Type_UInt:    return SetUInt(value.GetUInt());
		case Type_Real:    return SetReal(value.GetReal());
		case Type_String:  return SetString(value.GetString());
		case Type_Object:
		{
			Status status;

			if ((status = SetAsObject()).IsNOK())
			{
				return status;
			}
			for (Object::iterator iter = value.m_pObject->begin(); iter != value.m_pObject->end(); ++iter)
			{
				if ((status = AddMember(iter->first, iter->second)).IsNOK())
				{
					return status;
				}
			}

			return Status();
		}
		break;
		case Type_Array:
		{
			Status status;

			if ((status = SetAsArray()).IsNOK())
			{
				return status;
			}
			for (Array::iterator iter = value.m_pArray->begin(); iter != value.m_pArray->end(); ++iter)
			{
				if ((status = AddItem(*iter)).IsNOK())
				{
					return status;
				}
			}

			return Status();
		}
		break;
		default:
		{
			return Status_InvalidArg;
		}
	}
}

Bool Value::HasParent()
{
	return (NULL != m_pParent);
}

const Value &Value::GetParent() const
{
	if (NULL != m_pParent)
	{
		return *m_pParent;
	}
	else
	{
		return INVALID_VALUE;
	}
}

Value &Value::GetParent()
{
	if (NULL != m_pParent)
	{
		return *m_pParent;
	}
	else
	{
		return INVALID_VALUE;
	}
}

Value::Type Value::GetType() const
{
	return m_nType;
}

Bool Value::IsInvalid() const 
{ 
	return (Type_Invalid == GetType()); 
}

Bool Value::IsNull() const 
{ 
	return (Type_Null == GetType()); 
}

Bool Value::IsBool() const 
{ 
	return (Type_Bool == GetType()); 
}

Bool Value::IsInt() const 
{ 
	return (Type_Int == GetType()); 
}

Bool Value::IsUInt() const
{
	return (Type_UInt == GetType());
}

Bool Value::IsReal() const 
{ 
	return (Type_Real == GetType()); 
}

Bool Value::IsString() const 
{ 
	return (Type_String == GetType()); 
}

Bool Value::IsObject() const 
{ 
	return (Type_Object == GetType()); 
}

Bool Value::IsArray() const 
{ 
	return (Type_Array == GetType()); 
}

Bool Value::GetNull(Status *pStatus/* = NULL*/) const
{
	if (IsInvalid())
	{
		if (NULL != pStatus)
		{
			*pStatus = Status_InvalidCall;
		}

		return False;
	}

	if (IsNull())
	{
		if (NULL != pStatus)
		{
			*pStatus = Status_OK;
		}

		return True;
	}
	else
	{
		if (NULL != pStatus)
		{
			*pStatus = Status_InvalidArg;
		}

		return False;
	}
}

Status Value::SetNull()
{
	if (IsInvalid())
	{
		return Status_InvalidCall;
	}

	FreeMem();
	m_nType = Type_Null;

	return Status();
}

Bool Value::GetBool(Bool bDefault/* = DEFAULT_BOOL*/, Status *pStatus/* = NULL*/) const
{
	if (IsInvalid())
	{
		if (NULL != pStatus)
		{
			*pStatus = Status_InvalidCall;
		}

		return bDefault;
	}

	if (IsBool())
	{
		if (NULL != pStatus)
		{
			*pStatus = Status_OK;
		}

		return m_bBool;
	}
	else
	{
		if (NULL != pStatus)
		{
			*pStatus = Status_InvalidArg;
		}

		return bDefault;
	}
}

Status Value::SetBool(Bool bValue)
{
	if (IsInvalid())
	{
		return Status_InvalidCall;
	}
	
	FreeMem();
	m_nType = Type_Bool;
	m_bBool = bValue;

	return Status();
}

Int64 Value::GetInt(Int64 nDefault/* = DEFAULT_INT*/, Status *pStatus/* = NULL*/) const
{
	if (IsInvalid())
	{
		if (NULL != pStatus)
		{
			*pStatus = Status_InvalidCall;
		}

		return nDefault;
	}

	if (IsInt())
	{
		if (NULL != pStatus)
		{
			*pStatus = Status_OK;
		}

		return m_nInt;
	}
	else
	{
		if (NULL != pStatus)
		{
			*pStatus = Status_InvalidArg;
		}

		return nDefault;
	}
}

Status Value::SetInt(Int64 nValue)
{
	if (IsInvalid())
	{
		return Status_InvalidCall;
	}

	FreeMem();
	m_nType  = Type_Int;
	m_nInt   = nValue;

	return Status();
}

UInt64 Value::GetUInt(UInt64 uDefault/* = DEFAULT_UINT*/, Status *pStatus/* = NULL*/) const
{
	if (IsInvalid())
	{
		if (NULL != pStatus)
		{
			*pStatus = Status_InvalidCall;
		}

		return uDefault;
	}

	if (IsUInt())
	{
		if (NULL != pStatus)
		{
			*pStatus = Status_OK;
		}

		return m_uInt;
	}
	else
	{
		if (NULL != pStatus)
		{
			*pStatus = Status_InvalidArg;
		}

		return uDefault;
	}
}

Status Value::SetUInt(UInt64 uValue)
{
	if (IsInvalid())
	{
		return Status_InvalidCall;
	}

	FreeMem();
	m_nType = Type_UInt;
	m_uInt  = uValue;

	return Status();
}

Double Value::GetReal(Double lfDefault/* = DEFAULT_DOUBLE*/, Status *pStatus/* = NULL*/) const
{
	if (IsInvalid())
	{
		if (NULL != pStatus)
		{
			*pStatus = Status_InvalidCall;
		}

		return lfDefault;
	}

	if (IsReal())
	{
		if (NULL != pStatus)
		{
			*pStatus = Status_OK;
		}

		return m_lfReal;
	}
	else
	{
		if (NULL != pStatus)
		{
			*pStatus = Status_InvalidArg;
		}

		return lfDefault;
	}
}

Status Value::SetReal(Double lfValue)
{
	if (IsInvalid())
	{
		return Status_InvalidCall;
	}

	FreeMem();
	m_nType    = Type_Real;
	m_lfReal   = lfValue;

	return Status();
}

const String &Value::GetString(const String &sDefault/* = DEFAULT_STRING*/, Status *pStatus/* = NULL*/) const
{
	if (IsInvalid())
	{
		if (NULL != pStatus)
		{
			*pStatus = Status_InvalidCall;
		}

		return sDefault;
	}

	if (IsString())
	{
		if (NULL != pStatus)
		{
			*pStatus = Status_OK;
		}

		return *m_psString;
	}
	else
	{
		if (NULL != pStatus)
		{
			*pStatus = Status_InvalidArg;
		}

		return sDefault;
	}
}

Status Value::SetString(const String &sValue)
{
	if (IsInvalid())
	{
		return Status_InvalidCall;
	}

	String *psValue;

	if (NULL == (psValue = new (std::nothrow) String(sValue)))
	{
		return Status_MemAllocFailed;
	}
	FreeMem();
	m_nType    = Type_String;
	m_psString = psValue;

	return Status();
}

Status Value::SetAsArray()
{
	if (IsInvalid())
	{
		return Status_InvalidCall;
	}

	Array *pArray;

	if (NULL == (pArray = new (std::nothrow) Array()))
	{
		return Status_MemAllocFailed;
	}
	FreeMem();
	m_nType  = Type_Array;
	m_pArray = pArray;

	return Status();
}

Status Value::SetAsObject()
{
	if (IsInvalid())
	{
		return Status_InvalidCall;
	}

	Object *pObject;

	if (NULL == (pObject = new (std::nothrow) Object()))
	{
		return Status_MemAllocFailed;
	}
	FreeMem();
	m_nType   = Type_Object;
	m_pObject = pObject;

	return Status();
}

Size Value::GetItemsCount(Status *pStatus/* = NULL*/) const
{
	if (IsInvalid())
	{
		if (NULL != pStatus)
		{
			*pStatus = Status_InvalidCall;
		}

		return 0;
	}

	if (IsArray())
	{
		if (NULL != pStatus)
		{
			*pStatus = Status_OK;
		}

		return m_pArray->size();
	}
	else
	{
		if (NULL != pStatus)
		{
			*pStatus = Status_InvalidArg;
		}

		return 0;
	}
}

Value &Value::AddItem(Status *pStatus/* = NULL*/)
{
	if (IsInvalid())
	{
		if (NULL != pStatus)
		{
			*pStatus = Status_InvalidCall;
		}

		return INVALID_VALUE;
	}

	if (!IsArray())
	{
		if (NULL != pStatus)
		{
			*pStatus = Status_InvalidArg;
		}

		return INVALID_VALUE;
	}

	Value *pValue;

	if (NULL == (pValue = new (std::nothrow) Value(this)))
	{
		return INVALID_VALUE;
	}

	m_pArray->push_back(pValue);

	return *pValue;
}

Status Value::AddItem(Value *pValue)
{
	if (IsInvalid())
	{
		return Status_InvalidCall;
	}

	if (!IsArray())
	{
		return Status_InvalidArg;
	}

	if (NULL != pValue->m_pParent)
	{
		return Status_InvalidArg;
	}

	pValue->m_pParent = this;
	m_pArray->push_back(pValue);

	return Status();
}

Value &Value::InsertItem(int cIndex, Status *pStatus/* = NULL*/)
{
	if (IsInvalid())
	{
		if (NULL != pStatus)
		{
			*pStatus = Status_InvalidCall;
		}

		return INVALID_VALUE;
	}

	if (!IsArray())
	{
		if (NULL != pStatus)
		{
			*pStatus = Status_InvalidArg;
		}

		return INVALID_VALUE;
	}

	if (0 > cIndex)
	{
		if (NULL != pStatus)
		{
			*pStatus = Status_InvalidArg;
		}

		return INVALID_VALUE;
	}

	if (cIndex >= (int)m_pArray->size())
	{
		return AddItem(pStatus);
	}

	Value *pValue;

	if (NULL == (pValue = new (std::nothrow) Value(this)))
	{
		return INVALID_VALUE;
	}

	Array::iterator iter = m_pArray->begin() + cIndex;

	m_pArray->insert(iter, pValue);

	return *pValue;
}

Status Value::InsertItem(int cIndex, Value *pValue)
{
	if (IsInvalid())
	{
		return Status_InvalidCall;
	}

	if (!IsArray())
	{
		return Status_InvalidArg;
	}

	if (NULL == pValue->m_pParent)
	{
		return Status_InvalidArg;
	}

	if (0 > cIndex)
	{
		return Status_InvalidArg;
	}

	if (cIndex >= (int)m_pArray->size())
	{
		return AddItem(pValue);
	}

	pValue->m_pParent = this;

	Array::iterator iter = m_pArray->begin() + cIndex;

	m_pArray->insert(iter, pValue);

	return Status();
}

Status Value::RemoveItem(int cIndex)
{
	if (IsInvalid())
	{
		return Status_InvalidCall;
	}

	if (!IsArray())
	{
		return Status_InvalidArg;
	}

	if (cIndex >= (int)m_pArray->size())
	{
		return Status_InvalidArg;
	}

	Array::iterator iter = m_pArray->begin() + cIndex;

	delete *iter;
	m_pArray->erase(iter);

	return Status();
}

Status Value::RemoveAllItems()
{
	if (IsInvalid())
	{
		return Status_InvalidCall;
	}

	if (!IsArray())
	{
		return Status_InvalidArg;
	}

	for (Array::iterator iter = m_pArray->begin(); iter != m_pArray->end(); ++iter)
	{
		delete *iter;
	}
	m_pArray->clear();

	return Status();
}

const Value &Value::GetItem(int cIndex, Status *pStatus/* = NULL*/) const
{
	if (IsInvalid())
	{
		if (NULL != pStatus)
		{
			*pStatus = Status_InvalidCall;
		}

		return INVALID_VALUE;
	}

	if (!IsArray())
	{
		if (NULL != pStatus)
		{
			*pStatus = Status_InvalidArg;
		}

		return INVALID_VALUE;
	}

	if (0 > cIndex || cIndex >= (int)m_pArray->size())
	{
		if (NULL != pStatus)
		{
			*pStatus = Status_InvalidArg;
		}

		return INVALID_VALUE;
	}

	return *(*m_pArray)[cIndex];
}

Value &Value::GetItem(int cIndex, Status *pStatus/* = NULL*/) //-1 will create a new value at the end
{
	if (IsInvalid())
	{
		if (NULL != pStatus)
		{
			*pStatus = Status_InvalidCall;
		}

		return INVALID_VALUE;
	}

	if (!IsArray())
	{
		if (NULL != pStatus)
		{
			*pStatus = Status_InvalidArg;
		}

		return INVALID_VALUE;
	}

	if (-1 == cIndex)
	{
		return AddItem(pStatus);
	}

	if (0 > cIndex || cIndex >= (int)m_pArray->size())
	{
		if (NULL != pStatus)
		{
			*pStatus = Status_InvalidArg;
		}

		return INVALID_VALUE;
	}

	return *(*m_pArray)[cIndex];
}

const Value &Value::operator[](int cIndex) const
{
	return GetItem(cIndex);
}

Value &Value::operator[](int cIndex) //a non existing member will be created
{
	return GetItem(cIndex);
}

Size Value::GetMembersCount(Status *pStatus/* = NULL*/) const
{
	if (IsInvalid())
	{
		if (NULL != pStatus)
		{
			*pStatus = Status_InvalidCall;
		}

		return 0;
	}

	if (!IsObject())
	{
		if (NULL != pStatus)
		{
			*pStatus = Status_InvalidArg;
		}

		return 0;
	}

	return m_pObject->size();
}

Bool Value::Exists(const String &sName, Status *pStatus/* = NULL*/) const
{
	if (IsInvalid())
	{
		if (NULL != pStatus)
		{
			*pStatus = Status_InvalidCall;
		}

		return False;
	}

	if (!IsObject())
	{
		if (NULL != pStatus)
		{
			*pStatus = Status_InvalidArg;
		}

		return False;
	}

	return (m_pObject->end() != m_pObject->find(sName));
}

Value &Value::AddMember(const String &sName, Status *pStatus/* = NULL*/)
{
	if (IsInvalid())
	{
		if (NULL != pStatus)
		{
			*pStatus = Status_InvalidCall;
		}

		return INVALID_VALUE;
	}

	if (!IsObject())
	{
		if (NULL != pStatus)
		{
			*pStatus = Status_InvalidArg;
		}

		return INVALID_VALUE;
	}

	Object::iterator iter = m_pObject->find(sName);

	if (m_pObject->end() != iter)
	{
		return *iter->second;
	}

	Value *pValue;

	if (NULL == (pValue = new (std::nothrow) Value(this)))
	{
		return INVALID_VALUE;
	}

	(*m_pObject)[sName] = pValue;

	return *pValue;
}

Status Value::AddMember(const String &sName, Value *pValue)
{
	if (IsInvalid())
	{
		return Status_InvalidCall;
	}

	if (!IsObject())
	{
		return Status_InvalidArg;
	}

	Object::iterator iter = m_pObject->find(sName);

	if (m_pObject->end() != iter)
	{
		return Status_InvalidArg;
	}

	if (NULL != pValue->m_pParent)
	{
		return Status_InvalidArg;
	}

	pValue->m_pParent = this;
	(*m_pObject)[sName] = pValue;

	return Status();
}

Status Value::RemoveMember(const String &sName)
{
	if (IsInvalid())
	{
		return Status_InvalidCall;
	}

	if (!IsObject())
	{
		return Status_InvalidArg;
	}

	Object::iterator iter = m_pObject->find(sName);

	if (m_pObject->end() == iter)
	{
		return Status_NotFound;
	}
	m_pObject->erase(iter);

	return Status();
}

Status Value::RemoveAllMembers()
{
	if (IsInvalid())
	{
		return Status_InvalidCall;
	}

	if (!IsObject())
	{
		return Status_InvalidArg;
	}

	for (Object::iterator iter = m_pObject->begin(); iter != m_pObject->end(); ++iter)
	{
		delete iter->second;
	}
	m_pObject->clear();

	return Status();
}

Value &Value::GetMemberByIndex(Size cIndex, String *psName/* = NULL*/, Status *pStatus/* = NULL*/)
{
	if (IsInvalid())
	{
		if (NULL != pStatus)
		{
			*pStatus = Status_InvalidCall;
		}

		return INVALID_VALUE;
	}

	if (!IsObject())
	{
		if (NULL != pStatus)
		{
			*pStatus = Status_InvalidArg;
		}

		return INVALID_VALUE;
	}

	if (cIndex >= m_pObject->size())
	{
		if (NULL != pStatus)
		{
			*pStatus = Status_InvalidArg;
		}

		return INVALID_VALUE;
	}

	Object::iterator iter = m_pObject->begin();

	while (0 < cIndex)
	{
		++iter;
		cIndex--;
	}
	if (NULL != psName)
	{
		*psName = iter->first;
	}
	
	return *iter->second;
}

const Value &Value::GetMemberByIndex(Size cIndex, String *psName/* = NULL*/, Status *pStatus/* = NULL*/) const
{
	if (IsInvalid())
	{
		if (NULL != pStatus)
		{
			*pStatus = Status_InvalidCall;
		}

		return INVALID_VALUE;
	}

	if (!IsObject())
	{
		if (NULL != pStatus)
		{
			*pStatus = Status_InvalidArg;
		}

		return INVALID_VALUE;
	}

	if (cIndex >= m_pObject->size())
	{
		if (NULL != pStatus)
		{
			*pStatus = Status_InvalidArg;
		}

		return INVALID_VALUE;
	}

	Object::const_iterator iter = m_pObject->begin();

	while (0 < cIndex)
	{
		++iter;
		cIndex--;
	}
	if (NULL != psName)
	{
		*psName = iter->first;
	}

	return *iter->second;
}

const Value &Value::GetMember(const String &sName, Status *pStatus/* = NULL*/) const
{
	if (IsInvalid())
	{
		if (NULL != pStatus)
		{
			*pStatus = Status_InvalidCall;
		}

		return INVALID_VALUE;
	}

	if (!IsObject())
	{
		if (NULL != pStatus)
		{
			*pStatus = Status_InvalidArg;
		}

		return INVALID_VALUE;
	}

	Object::const_iterator iter = m_pObject->find(sName);

	if (m_pObject->end() == iter)
	{
		if (NULL != pStatus)
		{
			*pStatus = Status_NotFound;
		}

		return INVALID_VALUE;
	}
	
	return *iter->second;
}

Value &Value::GetMember(const String &sName, Status *pStatus/* = NULL*/) //a non existing member will be created
{
	if (IsInvalid())
	{
		if (NULL != pStatus)
		{
			*pStatus = Status_InvalidCall;
		}

		return INVALID_VALUE;
	}

	if (!IsObject())
	{
		if (NULL != pStatus)
		{
			*pStatus = Status_InvalidArg;
		}

		return INVALID_VALUE;
	}

	Object::iterator iter = m_pObject->find(sName);

	if (m_pObject->end() == iter)
	{
		return AddMember(sName, pStatus);
	}

	return *iter->second;
}

const Value &Value::operator[](const String &sName) const
{
	return GetMember(sName);
}

Value &Value::operator[](const String &sName) //a non existing member will be created
{
	return GetMember(sName);
}

const Value &Value::operator[](const Char *szName) const
{
	return GetMember(szName);
}

Value &Value::operator[](const Char *szName) //a non existing member will be created
{
	return GetMember(szName);
}

Value &Value::operator=(Bool bBool)
{
	SetBool(bBool);

	return *this;
}

Value &Value::operator=(Int64 nInt)
{
	SetInt(nInt);

	return *this;
}

Value &Value::operator=(UInt64 uInt)
{
	SetUInt(uInt);

	return *this;
}

Value &Value::operator=(Double lfReal)
{
	SetReal(lfReal);

	return *this;
}

Value &Value::operator=(const String &sString)
{
	SetString(sString);

	return *this;
}

Value &Value::operator=(const Char *szString)
{
	SetString(szString);

	return *this;
}

Value::operator Bool () const
{
	return GetBool();
}

Value::operator Int64 () const
{
	return GetInt();
}

Value::operator UInt64 () const
{
	return GetUInt();
}

Value::operator Double () const
{
	return GetReal();
}

Value::operator const String & () const
{
	return GetString();
}

Value::operator const Char * () const
{
	return GetString().c_str();
}

Status Value::Read(IO::IInputStream *pInputStream)
{
	Data::JSON::SAXParser      parser;
	VarJSONSAXParserObserver   observer;
	Status                     status;

	observer.m_pRoot = observer.m_pCurrent = this;
	observer.m_bInit = True;
	if ((status = parser.AddObserver(&observer)).IsNOK())
	{
		return status;
	}

	return parser.ParseStream(pInputStream);
}

Status Value::Read(const void *pData, Size cbSize)
{
	IO::MemInputStream mis(pData, cbSize);

	return Read(&mis);
}

Status Value::Read(const String &sData)
{
	return Read(sData.c_str(), sData.size());
}

Status Value::Read(const Char *szData)
{
	return Read(String(szData));
}

int Value::Compare(const Value &v) const
{
	struct Node
	{
		const Value *pV1;
		const Value *pV2;
		int         cIndex;
	};

	typedef Stack<Node>::Type   NodesStack;

	NodesStack  stackNodes;
	const Value *pV1 = this;
	const Value *pV2 = &v;
	bool         bReady;
	Status       status;

	for (;;)
	{
		if (!stackNodes.empty())
		{
			stackNodes.top().cIndex++;
			if (stackNodes.top().pV1->IsObject())
			{
				String sName;

				if ((Size)stackNodes.top().cIndex < stackNodes.top().pV1->GetMembersCount())
				{
					pV1 = &stackNodes.top().pV1->GetMemberByIndex((Size)stackNodes.top().cIndex, &sName, &status);
					if (!status)
					{
						return status;
					}
					pV2 = &stackNodes.top().pV2->GetMember(sName, &status);
					if (!status)
					{
						return status;
					}
				}
				else
				{
					stackNodes.pop();
					if (stackNodes.empty())
					{
						break;
					}
					else
					{
						continue;
					}
				}
			}
			else
			{
				if ((Size)stackNodes.top().cIndex < stackNodes.top().pV1->GetItemsCount())
				{
					pV1 = &stackNodes.top().pV1->GetItem((Size)stackNodes.top().cIndex, &status);
					if (!status)
					{
						return status;
					}
					pV2 = &stackNodes.top().pV2->GetItem((Size)stackNodes.top().cIndex, &status);
					if (!status)
					{
						return status;
					}
				}
				else
				{
					stackNodes.pop();
					if (stackNodes.empty())
					{
						break;
					}
					else
					{
						continue;
					}
				}
			}
		}

		bReady = false;
		if (pV1->IsInt())
		{
			if (pV2->IsUInt())
			{
				if (0 > pV1->GetInt())
				{
					return -1;
				}
				else
				{
					if ((UInt64)pV1->GetInt() < pV2->GetUInt())
					{
						return -1;
					}
					else
					if ((UInt64)pV1->GetInt() > pV2->GetUInt())
					{
						return 1;
					}
				}
				bReady = true;
			}
			else
			if (pV2->IsReal())
			{
				if ((double)pV1->GetInt() < pV2->GetReal())
				{
					return -1;
				}
				else
				if ((double)pV1->GetInt() > pV2->GetReal())
				{
					return 1;
				}
				bReady = true;
			}
		}
		else
		if (pV1->IsUInt())
		{
			if (pV2->IsInt())
			{
				if (0 > pV2->GetInt())
				{
					return 1;
				}
				else
				{
					if (pV1->GetUInt() < (UInt64)pV2->GetInt())
					{
						return -1;
					}
					else
					if (pV1->GetUInt() > (UInt64)pV2->GetInt())
					{
						return 1;
					}
				}
				bReady = true;
			}
			else
			if (pV2->IsReal())
			{
				if ((double)pV1->GetUInt() < pV2->GetReal())
				{
					return -1;
				}
				else
				if ((double)pV1->GetUInt() > pV2->GetReal())
				{
					return 1;
				}
				bReady = true;
			}
		}
		else
		if (pV1->IsReal())
		{
			if (pV2->IsInt())
			{
				if (pV1->GetReal() < (double)pV2->GetInt())
				{
					return -1;
				}
				else
				if (pV1->GetReal() > (double)pV2->GetInt())
				{
					return 1;
				}
				bReady = true;
			}
			else
			if (pV2->IsUInt())
			{
				if (pV1->GetReal() < (double)pV2->GetUInt())
				{
					return -1;
				}
				else
				if (pV1->GetReal() > (double)pV2->GetUInt())
				{
					return 1;
				}
				bReady = true;
			}
		}
		if (!bReady)
		{
			if (pV1->GetType() < pV2->GetType())
			{
				return -1;
			}
			else
			if (pV1->GetType() > pV2->GetType())
			{
				return 1;
			}
			else
			{
				if (pV1->IsNull())
				{
					//nothing to do
				}
				else
				if (pV1->IsBool())
				{
					if (pV1->GetBool() < pV2->GetBool())
					{
						return -1;
					}
					else
					if (pV1->GetBool() > pV2->GetBool())
					{
						return 1;
					}
				}
				else
				if (pV1->IsInt())
				{
					if (pV1->GetInt() < pV2->GetInt())
					{
						return -1;
					}
					if (pV1->GetInt() > pV2->GetInt())
					{
						return 1;
					}
				}
				else
				if (pV1->IsUInt())
				{
					if (pV1->GetUInt() < pV2->GetUInt())
					{
						return -1;
					}
					if (pV1->GetUInt() > pV2->GetUInt())
					{
						return 1;
					}
				}
				else
				if (pV1->IsReal())
				{
					if (pV1->GetReal() < pV2->GetReal())
					{
						return -1;
					}
					if (pV1->GetReal() > pV2->GetReal())
					{
						return 1;
					}
				}
				else
				if (pV1->IsString())
				{
					int nCmp = cx_strcmp(pV1->GetString().c_str(), pV2->GetString().c_str());

					if (0 != nCmp)
					{
						return nCmp;
					}
				}
				else
				if (pV1->IsObject())
				{
					if (pV1->GetMembersCount() < pV2->GetMembersCount())
					{
						return -1;
					}
					else
					if (pV1->GetMembersCount() > pV2->GetMembersCount())
					{
						return 1;
					}

					Node node;

					node.pV1    = pV1;
					node.pV2    = pV2;
					node.cIndex = -1;

					stackNodes.push(node);
				}
				else
				if (pV1->IsArray())
				{
					if (pV1->GetItemsCount() < pV2->GetItemsCount())
					{
						return -1;
					}
					else
					if (pV1->GetItemsCount() > pV2->GetItemsCount())
					{
						return 1;
					}

					Node node;

					node.pV1    = pV1;
					node.pV2    = pV2;
					node.cIndex = -1;

					stackNodes.push(node);
				}
				else
				{
					return -1;
				}
			}
		}

		if (stackNodes.empty())
		{
			break;
		}
	}

	return 0;
}

}//namespace CX
