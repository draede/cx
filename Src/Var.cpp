/* 
 * CX - C++ framework for general purpose developement
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

#include "CX/Var.h"
#include "CX/Status.h"
#include "CX/IO/MemInputStream.h"
#include "CX/IO/MemOutputStream.h"
#include "CX/Data/JSON/JSONSAXParser.h"
#include "CX/Data/JSON/IJSONSAXParserObserver.h"
#include "Data/JSON/RapidJSONStreams.h"


namespace CX
{

class VarJSONSAXParserObserver : public Data::JSON::IJSONSAXParserObserver
{
public:

	Bool     m_bInit;
	Var      *m_pRoot;
	Var      *m_pCurrent;
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
			if (m_pCurrent->SetObject().IsNOK())
			{
				return False;
			}
			m_bInit = False;
		}
		else
		{
			Var *pVar = New<Var>(Var::Type_Object);

			if (NULL == pVar)
			{
				return False;
			}
			if (m_pCurrent->IsObject())
			{
				pVar->SetName(m_sKey.c_str());
				pVar->SetParent(m_pCurrent);
				(*m_pCurrent->m_pObject)[m_sKey] = pVar;
				m_pCurrent = pVar;
			}
			else
			{
				pVar->SetParent(m_pCurrent);
				m_pCurrent->m_pArray->push_back(pVar);
				m_pCurrent = pVar;
			}
		}

		return True;
	}

	virtual Bool OnEndObject()
	{
		m_pCurrent = m_pCurrent->GetParent();

		return True;
	}

	virtual Bool OnBeginArray()
	{
		if (m_bInit)
		{
			if (m_pCurrent->SetArray().IsNOK())
			{
				return False;
			}
			m_bInit = False;
		}
		else
		{
			Var *pVar = New<Var>(Var::Type_Array);

			if (NULL == pVar)
			{
				return False;
			}
			if (m_pCurrent->IsObject())
			{
				pVar->SetName(m_sKey.c_str());
				pVar->SetParent(m_pCurrent);
				(*m_pCurrent->m_pObject)[m_sKey] = pVar;
				m_pCurrent = pVar;
			}
			else
			{
				pVar->SetParent(m_pCurrent);
				m_pCurrent->m_pArray->push_back(pVar);
				m_pCurrent = pVar;
			}
		}

		return True;
	}

	virtual Bool OnEndArray()
	{
		m_pCurrent = m_pCurrent->GetParent();

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

Var            Var::INVALID_VAR(True, True, True, True, True);
const Double   Var::DEFAULT_REAL    = 0.0;
const Char     *Var::DEFAULT_STRING = "";

Var::Var(Bool a1, Bool a2, Bool a3, Bool a4, Bool a5)
{
	a1; a2; a3; a4; a5;
	m_nType = Type_Invalid;
}

Var::Var(Type nType/* = Type_Null*/)
{
	m_pParent = NULL;
	m_nType   = Type_Null;
	SetType(nType);
}

Var::Var(Bool bBool)
{
	m_pParent = NULL;
	m_nType   = Type_Null;
	SetBool(bBool);
}

Var::Var(Int64 nInt)
{
	m_pParent = NULL;
	m_nType   = Type_Null;
	SetInt(nInt);
}

Var::Var(Double lfReal)
{
	m_pParent = NULL;
	m_nType   = Type_Null;
	SetReal(lfReal);
}

Var::Var(const Char *szString)
{
	m_pParent = NULL;
	m_nType   = Type_Null;
	SetString(szString);
}

Var::Var(const String &sString)
{
	m_pParent = NULL;
	m_nType   = Type_Null;
	SetString(sString);
}

Var::Var(const Var &var)
{
	Copy(var);
}

Var::~Var()
{
	SetType(Type_Null);
}

Var &Var::operator=(const Var &var)
{
	Copy(var);

	return *this;
}

Status Var::Copy(const Var &var)
{
	if (var.IsInvalid() || IsInvalid())
	{
		return Status(Status_InvalidCall, "Var is invalid");
	}
	
	if (Type_Null == var.GetType())
	{
		return SetNull();
	}
	else
	if (Type_Bool == var.GetType())
	{
		return SetBool(var.GetBool());
	}
	else
	if (Type_Int == var.GetType())
	{
		return SetInt(var.GetInt());
	}
	else
	if (Type_Real == var.GetType())
	{
		return SetReal(var.GetReal());
	}
	else
	if (Type_String == var.GetType())
	{
		return SetString(var.GetString());
	}
	else
	if (Type_Object == var.GetType())
	{
		Status status;

		status = SetObject();
		if (status.IsNOK())
		{
			return status;
		}
		for (ObjectVar::iterator iter = var.m_pObject->begin(); iter != var.m_pObject->end(); ++iter)
		{
			Var *pVar = New<Var>(*iter->second);

			if (NULL == pVar)
			{
				return Status(Status_MemAllocFailed, "Failed to allocate var");
			}
			pVar->SetParent(this);

			(*m_pObject)[iter->first] = pVar;
		}

		return Status();
	}
	else
	if (Type_Array == var.GetType())
	{
		Status status;

		status = SetArray();
		if (status.IsNOK())
		{
			return status;
		}
		for (ArrayVar::iterator iter = var.m_pArray->begin(); iter != var.m_pArray->end(); ++iter)
		{
			Var *pVar = New<Var>(**iter);

			if (NULL == pVar)
			{
				return Status(Status_MemAllocFailed, "Failed to allocate var");
			}
			pVar->SetParent(this);

			m_pArray->push_back(pVar);
		}

		return Status();
	}

	return Status(Status_NotSupported, "Not supported");
}

Bool Var::operator==(const Var &var) const
{
	return Equals(var);
}

Bool Var::operator!=(const Var &var) const
{
	return !Equals(var);
}

Bool Var::Equals(const Var &var, Bool bIgnoreCase/* = True*/) const
{
	if (var.GetType() != GetType())
	{
		return False;
	}
	if (0 != cx_stricmp(var.GetName(), GetName()))
	{
		return False;
	}
	if (IsInvalid() || IsNull())
	{
		return True;
	}
	if (IsBool())
	{
		return (var.GetBool() == GetBool());
	}
	else
	if (IsInt())
	{
		return (var.GetInt() == GetInt());
	}
	else
	if (IsReal())
	{
		return (var.GetReal() == GetReal());
	}
	else
	if (IsString())
	{
		if (bIgnoreCase)
		{
			return (0 == cx_stricmp(var.GetString(), GetString()));
		}
		else
		{
			return (0 != cx_strcmp(var.GetString(), GetString()));
		}
	}
	else
	if (IsObject())
	{
		if (m_pObject->size() != var.m_pObject->size())
		{
			return False;
		}
		for (ObjectVar::iterator iter = m_pObject->begin(); iter != m_pObject->end(); ++iter)
		{
			if (!var.IsObjectMember(iter->first))
			{
				return False;
			}
			if (!iter->second->Equals(var.GetObjectMember(iter->first)))
			{
				return False;
			}
		}

		return True;
	}
	else
	if (IsArray())
	{
		if (m_pArray->size() != var.m_pArray->size())
		{
			return False;
		}
		for (Size i = 0; i < m_pArray->size(); i++)
		{
			if (!(*m_pArray)[i]->Equals(*(*var.m_pArray)[i]))
			{
				return False;
			}
		}

		return True;
	}
	else
	{
		return False;
	}
}

const Char *Var::GetName() const
{
	if (IsInvalid())
	{
		return "";
	}

	return m_sName.c_str();
}

Status Var::SetName(const Char *szName)
{
	if (IsInvalid())
	{
		return Status(Status_InvalidCall, "Var is invalid");
	}
	if (NULL != GetParent() && GetParent()->IsObject())
	{
		GetParent()->HandleChildNameChange(this, m_sName.c_str(), szName);
	}
	m_sName = szName;

	return Status();
}

void Var::HandleChildNameChange(Var *pChild, const Char *szOldName, const Char *szNewName)
{
	ObjectVar::iterator iter = m_pObject->find(szOldName);

	m_pObject->erase(iter);
	(*m_pObject)[szNewName] = pChild;
}

Var::Type Var::GetType() const
{
	return m_nType;
}

Status Var::SetType(Type nType)
{
	if (IsInvalid())
	{
		return Status(Status_InvalidCall, "Var is invalid");
	}
	if (Type_String == m_nType && Type_String != nType)
	{
		Delete(m_psString);
	}
	else
	if (Type_Object == m_nType && Type_Object != nType)
	{
		for (ObjectVar::iterator iter = m_pObject->begin(); iter != m_pObject->end(); ++iter)
		{
			Delete(iter->second);
		}
		Delete(m_pObject);
	}
	else
	if (Type_Array == m_nType && Type_Array != nType)
	{
		for (ArrayVar::iterator iter = m_pArray->begin(); iter != m_pArray->end(); ++iter)
		{
			Delete(*iter);
		}
		Delete(m_pArray);
	}
	if (Type_String != m_nType && Type_String == nType)
	{
		if (NULL == (m_psString = New<String>()))
		{
			return Status(Status_MemAllocFailed, "Failed to alloc string");
		}
	}
	else
	if (Type_Object != m_nType && Type_Object == nType)
	{
		if (NULL == (m_pObject = New<ObjectVar>()))
		{
			return Status(Status_MemAllocFailed, "Failed to alloc object");
		}
	}
	else
	if (Type_Array != m_nType && Type_Array == nType)
	{
		if (NULL == (m_pArray = New<ArrayVar>()))
		{
			return Status(Status_MemAllocFailed, "Failed to alloc array");
		}
	}
	m_nType = nType;

	return Status();
}

Bool Var::IsInvalid() const
{
	return (Type_Invalid == m_nType);
}

Bool Var::IsValid() const
{
	return (Type_Invalid != m_nType);
}

Bool Var::IsNull() const
{
	return (Type_Null == m_nType);
}

Bool Var::IsBool() const
{
	return (Type_Bool == m_nType);
}

Bool Var::IsInt() const
{
	return (Type_Int == m_nType);
}

Bool Var::IsReal() const
{
	return (Type_Real == m_nType);
}

Bool Var::IsString() const
{
	return (Type_String == m_nType);
}

Bool Var::IsObject() const
{
	return (Type_Object == m_nType);
}

Bool Var::IsArray() const
{
	return (Type_Array == m_nType);
}

Status Var::SetNull()
{
	if (IsInvalid())
	{
		return Status(Status_InvalidCall, "Var is invalid");
	}
	
	return SetType(Type_Null);
}

Status Var::SetBool(Bool bBool)
{
	Status status;

	if (IsInvalid())
	{
		return Status(Status_InvalidCall, "Var is invalid");
	}
	status = SetType(Type_Bool);
	if (status.IsNOK())
	{
		return status;
	}
	m_bBool = bBool;

	return Status();
}

Bool Var::GetBool(Bool bBoolDefault/* = DEFAULT_BOOL*/) const
{
	if (IsInvalid())
	{
		return bBoolDefault;
	}
	if (IsBool())
	{
		return m_bBool;
	}

	return bBoolDefault;
}

Status Var::SetInt(Int64 nInt)
{
	Status status;

	if (IsInvalid())
	{
		return Status(Status_InvalidCall, "Var is invalid");
	}
	status = SetType(Type_Int);
	if (status.IsNOK())
	{
		return status;
	}
	m_nInt = nInt;

	return Status();
}

Int64 Var::GetInt(Int64 nIntDefault/* = DEFAULT_INT*/) const
{
	if (IsInvalid())
	{
		return nIntDefault;
	}
	if (IsInt())
	{
		return m_nInt;
	}

	return nIntDefault;
}

Status Var::SetReal(Double lfReal)
{
	Status status;

	if (IsInvalid())
	{
		return Status(Status_InvalidCall, "Var is invalid");
	}
	status = SetType(Type_Real);
	if (status.IsNOK())
	{
		return status;
	}
	m_lfReal = lfReal;

	return Status();
}

Double Var::GetReal(Double lfRealDefault/* = DEFAULT_REAL*/) const
{
	if (IsInvalid())
	{
		return lfRealDefault;
	}
	if (IsReal())
	{
		return m_lfReal;
	}

	return lfRealDefault;
}

Status Var::SetString(const Char *szString)
{
	Status status;

	if (IsInvalid())
	{
		return Status(Status_InvalidCall, "Var is invalid");
	}
	status = SetType(Type_String);
	if (status.IsNOK())
	{
		return status;
	}
	*m_psString = szString;

	return Status();
}

Status Var::SetString(const String &sString)
{
	Status status;

	if (IsInvalid())
	{
		return Status(Status_InvalidCall, "Var is invalid");
	}
	status = SetType(Type_String);
	if (status.IsNOK())
	{
		return status;
	}
	*m_psString = sString;

	return Status();
}

const Char *Var::GetString(const Char *szStringDefault/* = DEFAULT_STRING*/) const
{
	if (IsInvalid())
	{
		return szStringDefault;
	}
	if (IsString())
	{
		return m_psString->c_str();
	}

	return szStringDefault;
}

Status Var::SetObject()
{
	if (IsInvalid())
	{
		return Status(Status_InvalidCall, "Var is invalid");
	}

	return SetType(Type_Object);
}

Status Var::SetArray()
{
	if (IsInvalid())
	{
		return Status(Status_InvalidCall, "Var is invalid");
	}

	return SetType(Type_Array);
}

Bool Var::IsObjectMember(const char *szName) const
{
	if (!IsObject())
	{
		Status(Status_InvalidCall, "Var is not an object");

		return False;
	}

	return (m_pObject->end() != m_pObject->find(szName));
}

Bool Var::IsObjectMember(const String &sName) const
{
	return IsObjectMember(sName.c_str());
}

Bool Var::IsArrayItem(Size cIndex) const
{
	if (!IsArray())
	{
		return False;
	}

	return (m_pObject->size() > cIndex);
}

Size Var::GetObjectMembersCount() const
{
	if (!IsObject())
	{
		return 0;
	}

	return m_pObject->size();
}

Size Var::GetArrayItemsCount() const
{
	if (!IsArray())
	{
		return 0;
	}
	
	return m_pArray->size();
}

void Var::SetParent(Var *pParent)
{
	m_pParent = pParent;
}

Var *Var::GetParent() const 
{
	return m_pParent;
}

Var::ObjectIterator Var::GetObjectIterator()
{
	if (!IsObject())
	{
		return ObjectIterator(NULL);
	}

	return ObjectIterator(m_pObject);
}

Var::ObjectConstIterator Var::GetObjectConstIterator() const
{
	if (!IsObject())
	{
		return ObjectConstIterator(NULL);
	}

	return ObjectConstIterator(m_pObject);
}

Var::ArrayIterator Var::GetArrayIterator()
{
	if (!IsArray())
	{
		return ArrayIterator(NULL);
	}

	return ArrayIterator(m_pArray);
}

Var::ArrayConstIterator Var::GetArrayConstIterator() const
{
	if (!IsArray())
	{
		return ArrayConstIterator(NULL);
	}

	return ArrayConstIterator(m_pArray);
}

Var &Var::GetObjectMember(const char *szName)
{
	if (SetObject().IsNOK())
	{
		return INVALID_VAR;
	}

	ObjectVar::iterator iter = m_pObject->find(szName);

	if (m_pObject->end() == iter)
	{
		Var *pVar = New<Var>();

		if (NULL == pVar)
		{
			return INVALID_VAR;
		}
		pVar->SetName(szName);
		pVar->SetParent(this);
		(*m_pObject)[szName] = pVar;

		return *pVar;
	}
	else
	{
		return *iter->second;
	}
}

const Var &Var::GetObjectMember(const char *szName) const
{
	if (!IsObject())
	{
		return INVALID_VAR;
	}

	ObjectVar::iterator iter = m_pObject->find(szName);

	if (m_pObject->end() == iter)
	{
		return INVALID_VAR;
	}

	return *iter->second;
}

Var &Var::GetObjectMember(const String &sName)
{
	return GetObjectMember(sName.c_str());
}

const Var &Var::GetObjectMember(const String &sName) const
{
	return GetObjectMember(sName.c_str());
}

Var &Var::GetArrayItem(int cIndex/* = -1*/)
{
	if (SetArray().IsNOK())
	{
		return INVALID_VAR;
	}

	if (cIndex < 0 || cIndex >= (int)m_pArray->size())
	{
		Var *pVar = New<Var>();

		if (NULL == pVar)
		{
			return INVALID_VAR;
		}
		pVar->SetParent(this);
		m_pArray->push_back(pVar);

		return *pVar;
	}
	else
	{
		return *(*m_pArray)[cIndex];
	}
}

const Var &Var::GetArrayItem(int cIndex/* = -1*/) const
{
	if (!IsArray())
	{
		return INVALID_VAR;
	}

	if (cIndex < 0 || cIndex >= (int)m_pArray->size())
	{
		return INVALID_VAR;
	}

	return *(*m_pArray)[cIndex];
}

Var &Var::operator[](const char *szName)
{
	return GetObjectMember(szName);
}

Var &Var::operator[](const String &sName)
{
	return GetObjectMember(sName);
}

Var &Var::operator[](int cIndex)
{
	return GetArrayItem(cIndex);
}

Var &Var::operator=(Bool bBool)
{
	SetBool(bBool);

	return *this;
}

Var &Var::operator=(Int64 nInt)
{
	SetInt(nInt);

	return *this;
}

Var &Var::operator=(Double lfReal)
{
	SetReal(lfReal);

	return *this;
}

Var &Var::operator=(const char *szString)
{
	SetString(szString);

	return *this;
}

Var &Var::operator=(const String &sString)
{
	SetString(sString);

	return *this;
}

Var::operator Bool() const
{
	return GetBool();
}

Var::operator Int64 () const
{
	return GetInt();
}

Var::operator Double () const
{
	return GetReal();
}

Var::operator const Char * () const
{
	return GetString();
}

//===

Var::ObjectIterator::ObjectIterator(ObjectVar *pVar)
{
	m_pVar = pVar;
	m_iter = m_pVar->begin();
}

Bool Var::ObjectIterator::IsValid() const
{
	if (NULL == m_pVar)
	{
		return False;
	}

	return (m_pVar->end() != m_iter);
}

Status Var::ObjectIterator::Reset()
{
	if (NULL == m_pVar)
	{
		return Status(Status_InvalidCall, "Iterator is invalid");
	}

	m_iter = m_pVar->begin();

	return Status();
}

Status Var::ObjectIterator::Next()
{
	if (NULL == m_pVar)
	{
		return Status(Status_InvalidCall, "Iterator is invalid");
	}
	if (m_pVar->end() == m_iter)
	{
		return Status(Status_InvalidCall, "Iterator is invalid");
	}
	m_iter++;
	if (m_pVar->end() == m_iter)
	{
		return Status(Status_InvalidCall, "Iterator is invalid");
	}

	return Status();
}

Var &Var::ObjectIterator::Get()
{
	if (NULL == m_pVar)
	{
		return INVALID_VAR;
	}
	if (m_pVar->end() == m_iter)
	{
		return INVALID_VAR;
	}

	return *m_iter->second;
}

Var::ObjectConstIterator::ObjectConstIterator(const ObjectVar *pVar)
{
	m_pVar = pVar;
	m_iter = m_pVar->begin();
}

Bool Var::ObjectConstIterator::IsValid() const
{
	if (NULL == m_pVar)
	{
		return False;
	}

	return (m_pVar->end() != m_iter);
}

Status Var::ObjectConstIterator::Reset()
{
	if (NULL == m_pVar)
	{
		return Status(Status_InvalidCall, "Iterator is invalid");
	}

	m_iter = m_pVar->begin();

	return Status();
}

Status Var::ObjectConstIterator::Next()
{
	if (NULL == m_pVar)
	{
		return Status(Status_InvalidCall, "Iterator is invalid");
	}
	if (m_pVar->end() == m_iter)
	{
		return Status(Status_InvalidCall, "Iterator is invalid");
	}
	m_iter++;
	if (m_pVar->end() == m_iter)
	{
		return Status(Status_InvalidCall, "Iterator is invalid");
	}

	return Status();
}

const Var &Var::ObjectConstIterator::Get() const
{
	if (NULL == m_pVar)
	{
		return INVALID_VAR;
	}
	if (m_pVar->end() == m_iter)
	{
		return INVALID_VAR;
	}

	return *m_iter->second;
}

Var::ArrayIterator::ArrayIterator(ArrayVar *pVar)
{
	m_pVar = pVar;
	m_iter = m_pVar->begin();
}

Bool Var::ArrayIterator::IsValid() const
{
	if (NULL == m_pVar)
	{
		return False;
	}

	return (m_pVar->end() != m_iter);
}

Status Var::ArrayIterator::Reset()
{
	if (NULL == m_pVar)
	{
		return Status(Status_InvalidCall, "Iterator is invalid");
	}

	m_iter = m_pVar->begin();

	return Status();
}

Status Var::ArrayIterator::Next()
{
	if (NULL == m_pVar)
	{
		return Status(Status_InvalidCall, "Iterator is invalid");
	}
	if (m_pVar->end() == m_iter)
	{
		return Status(Status_InvalidCall, "Iterator is invalid");
	}
	m_iter++;
	if (m_pVar->end() == m_iter)
	{
		return Status(Status_InvalidCall, "Iterator is invalid");
	}

	return Status();
}

Var &Var::ArrayIterator::Get()
{
	if (NULL == m_pVar)
	{
		return INVALID_VAR;
	}
	if (m_pVar->end() == m_iter)
	{
		return INVALID_VAR;
	}

	return **m_iter;
}

Var::ArrayConstIterator::ArrayConstIterator(const ArrayVar *pVar)
{
	m_pVar = pVar;
	m_iter = m_pVar->begin();
}

Bool Var::ArrayConstIterator::IsValid() const
{
	if (NULL == m_pVar)
	{
		return False;
	}

	return (m_pVar->end() != m_iter);
}

Status Var::ArrayConstIterator::Reset()
{
	if (NULL == m_pVar)
	{
		return Status(Status_InvalidCall, "Iterator is invalid");
	}

	m_iter = m_pVar->begin();

	return Status_OK;
}

Status Var::ArrayConstIterator::Next()
{
	if (NULL == m_pVar)
	{
		return Status(Status_InvalidCall, "Iterator is invalid");
	}
	if (m_pVar->end() == m_iter)
	{
		return Status(Status_InvalidCall, "Iterator is invalid");
	}
	m_iter++;
	if (m_pVar->end() == m_iter)
	{
		return Status(Status_InvalidCall, "Iterator is invalid");
	}

	return Status();
}

const Var &Var::ArrayConstIterator::Get() const
{
	if (NULL == m_pVar)
	{
		return INVALID_VAR;
	}
	if (m_pVar->end() == m_iter)
	{
		return INVALID_VAR;
	}

	return **m_iter;
}

Status Var::Read(IO::IInputStream *pInputStream)
{
	VarJSONSAXParserObserver   obs;
	Data::JSON::JSONSAXParser  parser;

	obs.m_pRoot = obs.m_pCurrent = this;
	obs.m_bInit = True;
	parser.AddObserver(&obs);

	return parser.ParseStream(pInputStream);
}

Status Var::Read(const String &sStr)
{
	IO::MemInputStream mis(&sStr);

	return Read(&mis);
}

Status Var::Write(IO::IOutputStream *pOutputStream)
{
	return Write(pOutputStream, 0, True);
}

Status Var::Write(String &sStr)
{
	IO::MemOutputStream mos(&sStr);

	return Write(&mos);
}

Status Var::Write(IO::IOutputStream *pOutputStream, Size cIndent, Bool bLast)
{
	Size   cbSize;
	Size   cbAckSize;
	Char   szOutput[64];
	Char   pIndent[256];
	Status status;

	if (!IsValid())
	{
		return Status(Status_InvalidCall, "Var is invalid");
	}
	//indent
	if (cIndent > 256)
	{
		cIndent = 256;
	}
	for (Size i = 0; i < cIndent; i++)
	{
		pIndent[i] = '\t';
	}
	status = pOutputStream->Write(pIndent, cIndent, &cbSize);
	if (status.IsNOK())
	{
		return status;
	}
	//name
	if (NULL != GetParent() && !GetParent()->IsArray())
	{
		status = pOutputStream->Write("\"", 1, &cbSize);
		if (status.IsNOK())
		{
			return status;
		}
		status = WriteString(pOutputStream, m_sName.c_str(), m_sName.size());
		if (status.IsNOK())
		{
			return status;
		}
		status = pOutputStream->Write("\": ", 3, &cbSize);
		if (status.IsNOK())
		{
			return status;
		}
	}
	if (IsNull())
	{
		status = pOutputStream->Write("null", 4, &cbSize);
		if (status.IsNOK())
		{
			return status;
		}
	}
	else
	if (IsBool())
	{
		if (!GetBool())
		{
			status = pOutputStream->Write("false", 5, &cbSize);
			if (status.IsNOK())
			{
				return status;
			}
		}
		else
		{
			status = pOutputStream->Write("true", 4, &cbSize);
			if (status.IsNOK())
			{
				return status;
			}
		}
	}
	else
	if (IsInt())
	{
		status = ToString(GetInt(), szOutput, sizeof(szOutput) / sizeof(szOutput[0]), &cbAckSize, 0);
		if (status.IsNOK())
		{
			return status;
		}
		status = pOutputStream->Write(szOutput, cbAckSize, &cbSize);
		if (status.IsNOK())
		{
			return status;
		}
	}
	else
	if (IsReal())
	{
		status = ToString(GetReal(), szOutput, sizeof(szOutput) / sizeof(szOutput[0]), &cbAckSize, 6);
		if (status.IsNOK())
		{
			return status;
		}
		status = pOutputStream->Write(szOutput, cbAckSize, &cbSize);
		if (status.IsNOK())
		{
			return status;
		}
	}
	else
	if (IsString())
	{
		status = pOutputStream->Write("\"", 1, &cbSize);
		if (status.IsNOK())
		{
			return status;
		}
		status = WriteString(pOutputStream, m_psString->c_str(), m_psString->size());
		if (status.IsNOK())
		{
			return status;
		}
		status = pOutputStream->Write("\"", 1, &cbSize);
		if (status.IsNOK())
		{
			return status;
		}
	}
	else 
	if (IsObject())
	{
		status = pOutputStream->Write("\n", 1, &cbSize);
		if (status.IsNOK())
		{
			return status;
		}
		status = pOutputStream->Write(pIndent, cIndent, &cbSize);
		if (status.IsNOK())
		{
			return status;
		}
		status = pOutputStream->Write("{\n", 2, &cbSize);
		if (status.IsNOK())
		{
			return status;
		}
		for (ObjectVar::iterator iter = m_pObject->begin(); iter != m_pObject->end(); ++iter)
		{
			ObjectVar::iterator iterEx = iter;

			iterEx++;
			status = iter->second->Write(pOutputStream, cIndent + 1, (m_pObject->end() == iterEx));
			if (status.IsNOK())
			{
				return status;
			}
		}
		status = pOutputStream->Write(pIndent, cIndent, &cbSize);
		if (status.IsNOK())
		{
			return status;
		}
		status = pOutputStream->Write("}", 1, &cbSize);
		if (status.IsNOK())
		{
			return status;
		}
	}
	else 
	if (IsArray())
	{
		status = pOutputStream->Write("\n", 1, &cbSize);
		if (status.IsNOK())
		{
			return status;
		}
		status = pOutputStream->Write(pIndent, cIndent, &cbSize);
		if (status.IsNOK())
		{
			return status;
		}
		status = pOutputStream->Write("[\n", 2, &cbSize);
		if (status.IsNOK())
		{
			return status;
		}
		for (ArrayVar::iterator iter = m_pArray->begin(); iter != m_pArray->end(); ++iter)
		{
			ArrayVar::iterator iterEx = iter;

			iterEx++;
			status = (*iter)->Write(pOutputStream, cIndent + 1, (m_pArray->end() == iterEx));
			if (status.IsNOK())
			{
				return status;
			}
		}
		status = pOutputStream->Write(pIndent, cIndent, &cbSize);
		if (status.IsNOK())
		{
			return status;
		}
		status = pOutputStream->Write("]", 1, &cbSize);
		if (status.IsNOK())
		{
			return status;
		}
	}
	if (bLast)
	{
		status = pOutputStream->Write("\n", 1, &cbSize);
		if (status.IsNOK())
		{
			return status;
		}
	}
	else
	{
		status = pOutputStream->Write(",\n", 1, &cbSize);
		if (status.IsNOK())
		{
			return status;
		}
	}

	return Status();
}

Status Var::WriteString(IO::IOutputStream *pOutputStream, const Char *pBuffer, Size cLen)
{
	static Char hexdigits[] = 
	{ 
		'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F' 
	};
	const Byte *pPos;
	Char       escape[6];
	Size       cbAckSize;
	Status     status;

	pPos = (const Byte *)pBuffer;
	while (0 < cLen)
	{
		if (0x20 > *pPos) //control char
		{
			escape[0] = '\\';
			escape[1] = 'u';
			escape[2] = '0';
			escape[3] = '0';
			escape[4] = hexdigits[*pPos / 16];
			escape[5] = hexdigits[*pPos % 16];
			status = pOutputStream->Write(escape, 6, &cbAckSize);
			if (status.IsNOK())
			{
				return status;
			}
			cLen--;
			pPos++;
		}
		else
		if (0x80 > *pPos) //ASCII
		{
			if ('"' == *pPos || '\\' == *pPos || '/' == *pPos || '\b' == *pPos || '\f' == *pPos ||
				'\n' == *pPos || '\r' == *pPos || '\t' == *pPos)
			{
				escape[0] = '\\';
				escape[1] = *pPos;
				status    = pOutputStream->Write(escape, 2, &cbAckSize);
				if (status.IsNOK())
				{
					return status;
				}
				cLen--;
				pPos++;
			}
			else
			{
				status = pOutputStream->Write(pPos, 1, &cbAckSize);
				if (status.IsNOK())
				{
					return status;
				}
				cLen--;
				pPos++;
			}
		}
		else
		if (0xC2 > *pPos)
		{
			return Status(Status_InvalidArg, "Invalid UTF8 string");
		}
		else
		{
			unsigned cp = 0;

			if (0xE0 > *pPos) //2 bytes
			{
				if (2 > cLen)
				{
					return Status(Status_InvalidArg, "Invalid UTF8 string");
				}
				if ((pPos[1] & 0xC0) != 0x80)
				{
					return Status(Status_InvalidArg, "Invalid UTF8 string");
				}

				cp = (pPos[0] << 6) + pPos[1] - 0x3080;

				pPos += 2;
				cLen -= 2;
			}
			else
			if (0xF0 > *pPos) //3 bytes
			{
				if (3 > cLen)
				{
					return Status(Status_InvalidArg, "Invalid UTF8 string");
				}
				if ((pPos[1] & 0xC0) != 0x80)
				{
					return Status(Status_InvalidArg, "Invalid UTF8 string");
				}
				if (pPos[0] == 0xE0 && pPos[1] < 0xA0)
				{
					return Status(Status_InvalidArg, "Invalid UTF8 string");
				}
				if ((pPos[2] & 0xC0) != 0x80)
				{
					return Status(Status_InvalidArg, "Invalid UTF8 string");
				}

				cp = (pPos[0] << 12) + (pPos[1] << 6) + pPos[2] - 0xE2080;

				pPos += 3;
				cLen -= 3;
			}
			else
			if (0xF5 > *pPos) //4 bytes
			{
				if (4 > cLen)
				{
					return Status(Status_InvalidArg, "Invalid UTF8 string");
				}
				if ((pPos[1] & 0xC0) != 0x80)
				{
					return Status(Status_InvalidArg, "Invalid UTF8 string");
				}
				if (pPos[0] == 0xF0 && pPos[1] < 0x90)
				{
					return Status(Status_InvalidArg, "Invalid UTF8 string");
				}
				if (pPos[0] == 0xF4 && pPos[1] >= 0x90)
				{
					return Status(Status_InvalidArg, "Invalid UTF8 string");
				}
				if ((pPos[2] & 0xC0) != 0x80)
				{
					return Status(Status_InvalidArg, "Invalid UTF8 string");
				}
				if ((pPos[3] & 0xC0) != 0x80)
				{
					return Status(Status_InvalidArg, "Invalid UTF8 string");
				}

				cp = (pPos[0] << 18) + (pPos[1] << 12) + (pPos[2] << 6) + pPos[3] - 0x3C82080;

				pPos += 4;
				cLen -= 4;
			}
			else
			{
				return Status(Status_InvalidArg, "Invalid UTF8 string");
			}

			if (cp <= 0xD7FF || (cp >= 0xE000 && cp <= 0xFFFF)) 
			{
				escape[0] = '\\';
				escape[1] = 'u';
				escape[2] = hexdigits[((Byte *)&cp)[1] / 16];
				escape[3] = hexdigits[((Byte *)&cp)[1] % 16];
				escape[4] = hexdigits[((Byte *)&cp)[0] / 16];
				escape[5] = hexdigits[((Byte *)&cp)[0] % 16];
				status = pOutputStream->Write(escape, 6, &cbAckSize);
				if (status.IsNOK())
				{
					return status;
				}
			}
			else 
			if (cp >= 0x010000 && cp <= 0x10FFFF)
			{
				// Surrogate pair
				unsigned s     = cp - 0x010000;
				unsigned lead  = (s >> 10) + 0xD800;
				unsigned trail = (s & 0x3FF) + 0xDC00;

				escape[0] = '\\';
				escape[1] = 'u';
				escape[2] = hexdigits[((Byte *)&lead)[1] / 16];
				escape[3] = hexdigits[((Byte *)&lead)[1] % 16];
				escape[4] = hexdigits[((Byte *)&lead)[0] / 16];
				escape[5] = hexdigits[((Byte *)&lead)[0] % 16];
				status = pOutputStream->Write(escape, 6, &cbAckSize);
				if (status.IsNOK())
				{
					return status;
				}

				escape[0] = '\\';
				escape[1] = 'u';
				escape[2] = hexdigits[((Byte *)&trail)[1] / 16];
				escape[3] = hexdigits[((Byte *)&trail)[1] % 16];
				escape[4] = hexdigits[((Byte *)&trail)[0] / 16];
				escape[5] = hexdigits[((Byte *)&trail)[0] % 16];
				status = pOutputStream->Write(escape, 6, &cbAckSize);
				if (status.IsNOK())
				{
					return status;
				}
			}
			else
			{
				return Status(Status_InvalidArg, "Invalid UTF8 string");
			}
		}
	}

	return Status();
}

void operator>>(Var &var, String &sStr)
{
	var.Write(sStr);
}

void operator<<(String &sStr, Var &var)
{
	var.Write(sStr);
}

void operator>>(Var &var, IO::IOutputStream &os)
{
	var.Write(&os);
}

void operator<<(IO::IOutputStream &os, Var &var)
{
	var.Write(&os);
}

void operator<<(Var &var, const String &sStr)
{
	var.Read(sStr);
}

void operator>>(const String &sStr, Var &var)
{
	var.Read(sStr);
}

void operator<<(Var &var, IO::IInputStream &is)
{
	var.Read(&is);
}

void operator>>(IO::IInputStream &is, Var &var)
{
	var.Read(&is);
}

}//namespace CX

