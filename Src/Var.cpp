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

#include "CX/Var.hpp"
#include "CX/Status.hpp"
#include "CX/IO/MemInputStream.hpp"
#include "CX/IO/MemOutputStream.hpp"
#include "CX/Data/JSON/SAXParser.hpp"
#include "CX/Data/JSON/ISAXParserObserver.hpp"
#include "Data/JSON/RapidJSONStreams.hpp"


namespace CX
{

class VarJSONSAXParserObserver : public Data::JSON::ISAXParserObserver
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
			Var *pVar = new Var(Var::Type_Object);

			if (NULL == pVar)
			{
				return False;
			}
			if (m_pCurrent->IsObject())
			{
				pVar->SetName(m_sKey.c_str());
				pVar->SetParent(m_pCurrent);
				(m_pCurrent->m_pObject->mapVars)[m_sKey] = pVar;
				m_pCurrent->m_pObject->vectorVarExs.push_back(Var::VarEx(m_sKey, pVar));
				m_pCurrent = pVar;
			}
			else
			{
				pVar->SetParent(m_pCurrent);
				m_pCurrent->m_pArray->vectorVars.push_back(pVar);
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
			Var *pVar = new Var(Var::Type_Array);

			if (NULL == pVar)
			{
				return False;
			}
			if (m_pCurrent->IsObject())
			{
				pVar->SetName(m_sKey.c_str());
				pVar->SetParent(m_pCurrent);
				(m_pCurrent->m_pObject->mapVars)[m_sKey] = pVar;
				m_pCurrent->m_pObject->vectorVarExs.push_back(Var::VarEx(m_sKey, pVar));
				m_pCurrent = pVar;
			}
			else
			{
				pVar->SetParent(m_pCurrent);
				m_pCurrent->m_pArray->vectorVars.push_back(pVar);
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
		m_pObject->mapVars.clear();
		for (VarsMap::iterator iter = var.m_pObject->mapVars.begin(); 
		     iter != var.m_pObject->mapVars.end(); ++iter)
		{
			Var *pVar = new Var(*iter->second);

			if (NULL == pVar)
			{
				return Status(Status_MemAllocFailed, "Failed to allocate var");
			}
			pVar->SetParent(this);

			m_pObject->mapVars[iter->first] = pVar;
			m_pObject->vectorVarExs.push_back(VarEx(iter->first, pVar));
		}
		m_pObject->vectorVarExs.clear();
		for (VarExsVector::iterator iter = var.m_pObject->vectorVarExs.begin(); 
		     iter != var.m_pObject->vectorVarExs.end(); ++iter)
		{
			m_pObject->vectorVarExs.push_back(*iter);
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
		m_pArray->vectorVars.clear();
		for (VarsVector::iterator iter = var.m_pArray->vectorVars.begin(); 
		     iter != var.m_pArray->vectorVars.end(); ++iter)
		{
			Var *pVar = new Var(**iter);

			if (NULL == pVar)
			{
				return Status(Status_MemAllocFailed, "Failed to allocate var");
			}
			pVar->SetParent(this);

			m_pArray->vectorVars.push_back(pVar);
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
		if (m_pObject->mapVars.size() != var.m_pObject->mapVars.size())
		{
			return False;
		}
		for (VarsMap::iterator iter = m_pObject->mapVars.begin(); 
		     iter != m_pObject->mapVars.end(); ++iter)
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
		if (m_pArray->vectorVars.size() != var.m_pArray->vectorVars.size())
		{
			return False;
		}
		for (Size i = 0; i < m_pArray->vectorVars.size(); i++)
		{
			if (!(m_pArray->vectorVars)[i]->Equals(*(var.m_pArray->vectorVars)[i]))
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

Size Var::GetNameLen() const
{
	if (IsInvalid())
	{
		return 0;
	}

	return m_sName.size();
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
	VarsMap::iterator iter = m_pObject->mapVars.find(szOldName);

	m_pObject->mapVars.erase(iter);
	(m_pObject->mapVars)[szNewName] = pChild;

	for (VarExsVector::iterator iterEx = m_pObject->vectorVarExs.begin(); 
	     iterEx != m_pObject->vectorVarExs.end(); ++iterEx)
	{
		if (0 == cx_strcmp(iterEx->sName.c_str(), szOldName) && iterEx->pVar == pChild)
		{
			iterEx->sName = szNewName;
			break;
		}
	}
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
		delete m_psString;
	}
	else
	if (Type_Object == m_nType && Type_Object != nType)
	{
		for (VarsMap::iterator iter = m_pObject->mapVars.begin(); 
		     iter != m_pObject->mapVars.end(); ++iter)
		{
			delete iter->second;
		}
		delete m_pObject;
	}
	else
	if (Type_Array == m_nType && Type_Array != nType)
	{
		for (VarsVector::iterator iter = m_pArray->vectorVars.begin(); 
		     iter != m_pArray->vectorVars.end(); ++iter)
		{
			delete *iter;
		}
		delete m_pArray;
	}
	if (Type_String != m_nType && Type_String == nType)
	{
		if (NULL == (m_psString = new String()))
		{
			return Status(Status_MemAllocFailed, "Failed to alloc string");
		}
	}
	else
	if (Type_Object != m_nType && Type_Object == nType)
	{
		if (NULL == (m_pObject = new ObjectVar()))
		{
			return Status(Status_MemAllocFailed, "Failed to alloc object");
		}
	}
	else
	if (Type_Array != m_nType && Type_Array == nType)
	{
		if (NULL == (m_pArray = new ArrayVar()))
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

Size Var::GetStringLen() const
{
	if (IsInvalid())
	{
		return 0;
	}
	if (!IsString())
	{
		return 0;
	}
		
	return m_psString->size();
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

	return (m_pObject->mapVars.end() != m_pObject->mapVars.find(szName));
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

	return (m_pObject->mapVars.size() > cIndex);
}

Size Var::GetObjectMembersCount() const
{
	if (!IsObject())
	{
		return 0;
	}

	return m_pObject->mapVars.size();
}

Size Var::GetArrayItemsCount() const
{
	if (!IsArray())
	{
		return 0;
	}
	
	return m_pArray->vectorVars.size();
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

	VarsMap::iterator iter = m_pObject->mapVars.find(szName);

	if (m_pObject->mapVars.end() == iter)
	{
		Var *pVar = new Var();

		if (NULL == pVar)
		{
			return INVALID_VAR;
		}
		pVar->SetName(szName);
		pVar->SetParent(this);
		(m_pObject->mapVars)[szName] = pVar;
		m_pObject->vectorVarExs.push_back(VarEx(szName, pVar));

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

	VarsMap::iterator iter = m_pObject->mapVars.find(szName);

	if (m_pObject->mapVars.end() == iter)
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

	if (cIndex < 0 || cIndex >= (int)m_pArray->vectorVars.size())
	{
		Var *pVar = new Var();

		if (NULL == pVar)
		{
			return INVALID_VAR;
		}
		pVar->SetParent(this);
		m_pArray->vectorVars.push_back(pVar);

		return *pVar;
	}
	else
	{
		return *(m_pArray->vectorVars)[cIndex];
	}
}

const Var &Var::GetArrayItem(int cIndex/* = -1*/) const
{
	if (!IsArray())
	{
		return INVALID_VAR;
	}

	if (cIndex < 0 || cIndex >= (int)m_pArray->vectorVars.size())
	{
		return INVALID_VAR;
	}

	return *(m_pArray->vectorVars)[cIndex];
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

Var::ObjectIterator::ObjectIterator()
{
	m_pVar = NULL;
}

Var::ObjectIterator::ObjectIterator(ObjectVar *pVar)
{
	m_pVar = pVar;
	m_iter = m_pVar->vectorVarExs.begin();
}

Bool Var::ObjectIterator::IsValid() const
{
	if (NULL == m_pVar)
	{
		return False;
	}

	return (m_pVar->vectorVarExs.end() != m_iter);
}

Status Var::ObjectIterator::Reset()
{
	if (NULL == m_pVar)
	{
		return Status(Status_InvalidCall, "Iterator is invalid");
	}

	m_iter = m_pVar->vectorVarExs.begin();

	return Status();
}

Status Var::ObjectIterator::Next()
{
	if (NULL == m_pVar)
	{
		return Status(Status_InvalidCall, "Iterator is invalid");
	}
	if (m_pVar->vectorVarExs.end() == m_iter)
	{
		return Status(Status_InvalidCall, "Iterator is invalid");
	}
	m_iter++;
	if (m_pVar->vectorVarExs.end() == m_iter)
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
	if (m_pVar->vectorVarExs.end() == m_iter)
	{
		return INVALID_VAR;
	}

	return *m_iter->pVar;
}

Var::ObjectConstIterator::ObjectConstIterator()
{
	m_pVar = NULL;
}

Var::ObjectConstIterator::ObjectConstIterator(const ObjectVar *pVar)
{
	m_pVar = pVar;
	m_iter = m_pVar->vectorVarExs.begin();
}

Bool Var::ObjectConstIterator::IsValid() const
{
	if (NULL == m_pVar)
	{
		return False;
	}

	return (m_pVar->vectorVarExs.end() != m_iter);
}

Status Var::ObjectConstIterator::Reset()
{
	if (NULL == m_pVar)
	{
		return Status(Status_InvalidCall, "Iterator is invalid");
	}

	m_iter = m_pVar->vectorVarExs.begin();

	return Status();
}

Status Var::ObjectConstIterator::Next()
{
	if (NULL == m_pVar)
	{
		return Status(Status_InvalidCall, "Iterator is invalid");
	}
	if (m_pVar->vectorVarExs.end() == m_iter)
	{
		return Status(Status_InvalidCall, "Iterator is invalid");
	}
	m_iter++;
	if (m_pVar->vectorVarExs.end() == m_iter)
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
	if (m_pVar->vectorVarExs.end() == m_iter)
	{
		return INVALID_VAR;
	}

	return *m_iter->pVar;
}

Var::ArrayIterator::ArrayIterator()
{
	m_pVar = NULL;
}

Var::ArrayIterator::ArrayIterator(ArrayVar *pVar)
{
	m_pVar = pVar;
	m_iter = m_pVar->vectorVars.begin();
}

Bool Var::ArrayIterator::IsValid() const
{
	if (NULL == m_pVar)
	{
		return False;
	}

	return (m_pVar->vectorVars.end() != m_iter);
}

Status Var::ArrayIterator::Reset()
{
	if (NULL == m_pVar)
	{
		return Status(Status_InvalidCall, "Iterator is invalid");
	}

	m_iter = m_pVar->vectorVars.begin();

	return Status();
}

Status Var::ArrayIterator::Next()
{
	if (NULL == m_pVar)
	{
		return Status(Status_InvalidCall, "Iterator is invalid");
	}
	if (m_pVar->vectorVars.end() == m_iter)
	{
		return Status(Status_InvalidCall, "Iterator is invalid");
	}
	m_iter++;
	if (m_pVar->vectorVars.end() == m_iter)
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
	if (m_pVar->vectorVars.end() == m_iter)
	{
		return INVALID_VAR;
	}

	return **m_iter;
}

Var::ArrayConstIterator::ArrayConstIterator(const ArrayVar *pVar)
{
	m_pVar = pVar;
	m_iter = m_pVar->vectorVars.begin();
}

Var::ArrayConstIterator::ArrayConstIterator()
{
	m_pVar = NULL;
}

Bool Var::ArrayConstIterator::IsValid() const
{
	if (NULL == m_pVar)
	{
		return False;
	}

	return (m_pVar->vectorVars.end() != m_iter);
}

Status Var::ArrayConstIterator::Reset()
{
	if (NULL == m_pVar)
	{
		return Status(Status_InvalidCall, "Iterator is invalid");
	}

	m_iter = m_pVar->vectorVars.begin();

	return Status_OK;
}

Status Var::ArrayConstIterator::Next()
{
	if (NULL == m_pVar)
	{
		return Status(Status_InvalidCall, "Iterator is invalid");
	}
	if (m_pVar->vectorVars.end() == m_iter)
	{
		return Status(Status_InvalidCall, "Iterator is invalid");
	}
	m_iter++;
	if (m_pVar->vectorVars.end() == m_iter)
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
	if (m_pVar->vectorVars.end() == m_iter)
	{
		return INVALID_VAR;
	}

	return **m_iter;
}

Status Var::Read(IO::IInputStream *pInputStream)
{
	VarJSONSAXParserObserver   obs;
	Data::JSON::SAXParser  parser;

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
	return WriteNoRec(this, pOutputStream);
}

Status Var::Write(String &sStr)
{
	IO::MemOutputStream mos(&sStr);

	return Write(&mos);
}

Status Var::WriteNoRecName(const Var *pVar, IO::IOutputStream *pOutputStream)
{
	Size   cbSize;
	Status status;

	//name
	if (NULL != pVar->GetParent() && !pVar->GetParent()->IsArray())
	{
		status = pOutputStream->Write("\"", 1, &cbSize);
		if (status.IsNOK())
		{
			return status;
		}
		status = WriteString(pOutputStream, pVar->GetName(), pVar->GetNameLen());
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

	return Status();
}

Status Var::WriteNoRecScalar(const Var *pVar, IO::IOutputStream *pOutputStream, Bool bLast)
{
	Size   cbSize;
	Size   cbAckSize;
	Char   szOutput[64];
	Status status;

	if (pVar->IsNull())
	{
		status = pOutputStream->Write("null", 4, &cbSize);
		if (status.IsNOK())
		{
			return status;
		}
	}
	else
	if (pVar->IsBool())
	{
		if (!pVar->GetBool())
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
	if (pVar->IsInt())
	{
		status = ToString(pVar->GetInt(), 0, szOutput, sizeof(szOutput) / sizeof(szOutput[0]), 
								&cbAckSize, 0);
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
	if (pVar->IsReal())
	{
		status = ToString(pVar->GetReal(), 0, szOutput, sizeof(szOutput) / sizeof(szOutput[0]), 
								&cbAckSize, 6);
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
	if (pVar->IsString())
	{
		status = pOutputStream->Write("\"", 1, &cbSize);
		if (status.IsNOK())
		{
			return status;
		}
		status = WriteString(pOutputStream, pVar->GetString(), pVar->GetStringLen());
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
	{
		return Status(Status_InvalidArg, "Var is not a scalar");
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
		status = pOutputStream->Write(",\n", 2, &cbSize);
		if (status.IsNOK())
		{
			return status;
		}
	}

	return Status();
}

Status Var::WriteNoRec(const Var *pVar, IO::IOutputStream *pOutputStream)
{
	static const Size     MAX_INDENT = 1024;
	Size                  cbSize;
	Char                  indent[MAX_INDENT];
	WriteNoRecDataStack   stack;
	Size                  cIndent;
	Status                status;

	if (!pVar->IsValid())
	{
		return Status(Status_InvalidCall, "Var is invalid");
	}
	
	memset(indent, '\t', MAX_INDENT);
	
	status = WriteNoRecName(pVar, pOutputStream);
	if (status.IsNOK())
	{
		return status;
	}
	if (!pVar->IsObject() && !pVar->IsArray())
	{
		return WriteNoRecScalar(pVar, pOutputStream, True);
	}

	WriteNoRecData data;

	data.pVar = pVar;
	if (pVar->IsObject())
	{
		data.iterObject = pVar->GetObjectConstIterator();
		status          = pOutputStream->Write("{\n", 2, &cbSize);
	}
	else
	{
		data.iterArray = pVar->GetArrayConstIterator();
		status          = pOutputStream->Write("[\n", 2, &cbSize);
	}
	if (status.IsNOK())
	{
		return status;
	}

	stack.push(data);

	cIndent = 1;
	while (!stack.empty())
	{
		const Var *pTmpVar = NULL;

		//get next child
		if (stack.top().pVar->IsObject())
		{
			if (stack.top().iterObject.IsValid())
			{
				pTmpVar = &stack.top().iterObject.Get();
				stack.top().iterObject.Next();
			}
		}
		else
		{
			if (stack.top().iterArray.IsValid())
			{
				pTmpVar = &stack.top().iterArray.Get();
				stack.top().iterArray.Next();
			}
		}

		if (NULL != pTmpVar) //has next child
		{
			if (!pTmpVar->IsValid())
			{
				return Status(Status_InvalidCall, "Var is invalid");
			}

			status = pOutputStream->Write(indent, MAX_INDENT > cIndent ? cIndent : MAX_INDENT, &cbSize);
			if (status.IsNOK())
			{
				return status;
			}

			//write the name of an object child
			if (stack.top().pVar->IsObject())
			{
				status = WriteNoRecName(pTmpVar, pOutputStream);
				if (status.IsNOK())
				{
					return status;
				}
			}
			if (pTmpVar->IsObject()) //child is an object
			{
				if (stack.top().pVar->IsObject())
				{
					status = pOutputStream->Write("\n", 1, &cbSize);
					if (status.IsNOK())
					{
						return status;
					}
					status = pOutputStream->Write(indent, MAX_INDENT > cIndent ? cIndent : MAX_INDENT, &cbSize);
					if (status.IsNOK())
					{
						return status;
					}
				}
				status = pOutputStream->Write("{\n", 2, &cbSize);
				if (status.IsNOK())
				{
					return status;
				}

				cIndent++;

				if (pTmpVar->GetObjectConstIterator().IsValid())
				{
					WriteNoRecData d;

					d.pVar       = pTmpVar;
					d.iterObject = pTmpVar->GetObjectConstIterator();

					stack.push(d);

					pTmpVar = &data.iterObject.Get();
				}
				else
				{
					cIndent--;
					status = pOutputStream->Write(indent, MAX_INDENT > cIndent ? cIndent : MAX_INDENT, &cbSize);
					if (status.IsNOK())
					{
						return status;
					}
					status = pOutputStream->Write("}", 1, &cbSize);
					if (status.IsNOK())
					{
						return status;
					}
					if (stack.empty() ||
						(stack.top().pVar->IsObject() && !stack.top().iterObject.IsValid()) ||
						(stack.top().pVar->IsArray() && !stack.top().iterArray.IsValid()))
					{
						status = pOutputStream->Write("\n", 1, &cbSize);
						if (status.IsNOK())
						{
							return status;
						}
					}
					else
					{
						status = pOutputStream->Write(",\n", 2, &cbSize);
						if (status.IsNOK())
						{
							return status;
						}
					}
				}
			}
			else
			if (pTmpVar->IsArray()) //child is an array
			{
				status = pOutputStream->Write("\n", 1, &cbSize);
				if (status.IsNOK())
				{
					return status;
				}
				status = pOutputStream->Write(indent, MAX_INDENT > cIndent ? cIndent : MAX_INDENT, &cbSize);
				if (status.IsNOK())
				{
					return status;
				}
				status = pOutputStream->Write("[\n", 2, &cbSize);
				if (status.IsNOK())
				{
					return status;
				}

				cIndent++;

				if (pTmpVar->GetArrayConstIterator().IsValid())
				{
					WriteNoRecData d;

					d.pVar      = pTmpVar;
					d.iterArray = pTmpVar->GetArrayConstIterator();

					stack.push(d);

					pTmpVar = &data.iterArray.Get();
				}
				else
				{
					cIndent--;
					status = pOutputStream->Write(indent, MAX_INDENT > cIndent ? cIndent : MAX_INDENT, &cbSize);
					if (status.IsNOK())
					{
						return status;
					}
					status = pOutputStream->Write("]", 1, &cbSize);
					if (status.IsNOK())
					{
						return status;
					}
					if (stack.empty() ||
						(stack.top().pVar->IsObject() && !stack.top().iterObject.IsValid()) ||
						(stack.top().pVar->IsArray() && !stack.top().iterArray.IsValid()))
					{
						status = pOutputStream->Write("\n", 1, &cbSize);
						if (status.IsNOK())
						{
							return status;
						}
					}
					else
					{
						status = pOutputStream->Write(",\n", 2, &cbSize);
						if (status.IsNOK())
						{
							return status;
						}
					}
				}
			}
			else  //child is a scalar
			{
				status = WriteNoRecScalar(pTmpVar, pOutputStream, 
				                 (stack.top().pVar->IsObject() && !stack.top().iterObject.IsValid()) || 
				                 (stack.top().pVar->IsArray() && !stack.top().iterArray.IsValid()));
				if (status.IsNOK())
				{
					return status;
				}
			}
		}
		else //end of the object/array
		{
			cIndent--;
			status = pOutputStream->Write(indent, MAX_INDENT > cIndent ? cIndent : MAX_INDENT, &cbSize);
			if (status.IsNOK())
			{
				return status;
			}
			if (stack.top().pVar->IsObject())
			{
				status = pOutputStream->Write("}", 1, &cbSize);
				if (status.IsNOK())
				{
					return status;
				}
			}
			else
			{
				status = pOutputStream->Write("]", 1, &cbSize);
				if (status.IsNOK())
				{
					return status;
				}
			}
			stack.pop();
			if (stack.empty() || 
			    (stack.top().pVar->IsObject() && !stack.top().iterObject.IsValid()) ||
			    (stack.top().pVar->IsArray() && !stack.top().iterArray.IsValid()))
			{
				status = pOutputStream->Write("\n", 1, &cbSize);
				if (status.IsNOK())
				{
					return status;
				}
			}
			else
			{
				status = pOutputStream->Write(",\n", 2, &cbSize);
				if (status.IsNOK())
				{
					return status;
				}
			}
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

