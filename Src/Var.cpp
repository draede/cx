
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

	bool     m_bInit;
	Var      *m_pRoot;
	Var      *m_pCurrent;
	String   m_sKey;

	virtual bool OnBeginParse()
	{
		return true;
	}

	virtual bool OnEndParse()
	{
		return true;
	}

	virtual bool OnBeginObject()
	{
		if (m_bInit)
		{
			if (CXNOK(m_pCurrent->SetObject()))
			{
				return false;
			}
			m_bInit = false;
		}
		else
		{
			Var *pVar = New<Var>(Var::Type_Object);

			if (NULL == pVar)
			{
				return false;
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

		return true;
	}

	virtual bool OnEndObject()
	{
		m_pCurrent = m_pCurrent->GetParent();

		return true;
	}

	virtual bool OnBeginArray()
	{
		if (m_bInit)
		{
			if (CXNOK(m_pCurrent->SetArray()))
			{
				return false;
			}
			m_bInit = false;
		}
		else
		{
			Var *pVar = New<Var>(Var::Type_Array);

			if (NULL == pVar)
			{
				return false;
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

		return true;
	}

	virtual bool OnEndArray()
	{
		m_pCurrent = m_pCurrent->GetParent();

		return true;
	}

	virtual bool OnKey(const Char *pBuffer, Size cLen)
	{
		m_sKey.assign(pBuffer, cLen);

		return true;
	}

	virtual bool OnNullValue()
	{
		if (m_pCurrent->IsObject())
		{
			(*m_pCurrent)[m_sKey].SetNull();
		}
		else
		{
			(*m_pCurrent)[-1].SetNull();
		}
		
		return true;
	}

	virtual bool OnBoolValue(bool bBool)
	{
		if (m_pCurrent->IsObject())
		{
			(*m_pCurrent)[m_sKey].SetBool(bBool);
		}
		else
		{
			(*m_pCurrent)[-1].SetBool(bBool);
		}

		return true;
	}

	virtual bool OnIntValue(Int64 nInt)
	{
		if (m_pCurrent->IsObject())
		{
			(*m_pCurrent)[m_sKey].SetInt(nInt);
		}
		else
		{
			(*m_pCurrent)[-1].SetInt(nInt);
		}

		return true;
	}

	virtual bool OnRealValue(Double lfReal)
	{
		if (m_pCurrent->IsObject())
		{
			(*m_pCurrent)[m_sKey].SetReal(lfReal);
		}
		else
		{
			(*m_pCurrent)[-1].SetReal(lfReal);
		}

		return true;
	}

	virtual bool OnStringValue(const Char *pBuffer, Size cLen)
	{
		if (m_pCurrent->IsObject())
		{
			(*m_pCurrent)[m_sKey].SetString(String(pBuffer, cLen));
		}
		else
		{
			(*m_pCurrent)[-1].SetString(String(pBuffer, cLen));
		}

		return true;
	}

};

Var            Var::INVALID_VAR(true, true, true, true, true);
const Double   Var::DEFAULT_REAL    = 0.0;
const Char     *Var::DEFAULT_STRING = "";

Var::Var(bool a1, bool a2, bool a3, bool a4, bool a5)
{
	a1; a2; a3; a4; a5;
	m_nType = Type_Invalid;
}

Var::Var(Type nType/* = Type_Null*/)
{
	Status::Clear();

	m_pParent = NULL;
	m_nType   = Type_Null;
	SetType(nType);
}

Var::Var(bool bBool)
{
	Status::Clear();

	m_pParent = NULL;
	m_nType   = Type_Null;
	SetBool(bBool);
}

Var::Var(Int64 nInt)
{
	Status::Clear();

	m_pParent = NULL;
	m_nType   = Type_Null;
	SetInt(nInt);
}

Var::Var(Double lfReal)
{
	Status::Clear();

	m_pParent = NULL;
	m_nType   = Type_Null;
	SetReal(lfReal);
}

Var::Var(const Char *szString)
{
	Status::Clear();

	m_pParent = NULL;
	m_nType   = Type_Null;
	SetString(szString);
}

Var::Var(const String &sString)
{
	Status::Clear();

	m_pParent = NULL;
	m_nType   = Type_Null;
	SetString(sString);
}

Var::Var(const Var &var)
{
	Status::Clear();

	Copy(var);
}

Var::~Var()
{
	SetType(Type_Null);
}

Var &Var::operator=(const Var &var)
{
	Status::Clear();

	Copy(var);

	return *this;
}

StatusCode Var::Copy(const Var &var)
{
	Status::Clear();

	if (var.IsInvalid() || IsInvalid())
	{
		return Status::Set(Status_InvalidCall, "Var is invalid");
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
		if (CXNOK(SetObject()))
		{
			return Status::GetCode();
		}
		for (ObjectVar::iterator iter = var.m_pObject->begin(); iter != var.m_pObject->end(); ++iter)
		{
			Var *pVar = New<Var>(*iter->second);

			if (NULL == pVar)
			{
				return Status::Set(Status_MemAllocFailed, "Failed to allocate var");
			}
			pVar->SetParent(this);

			(*m_pObject)[iter->first] = pVar;
		}

		return Status_OK;
	}
	else
	if (Type_Array == var.GetType())
	{
		if (CXNOK(SetArray()))
		{
			return Status::GetCode();
		}
		for (ArrayVar::iterator iter = var.m_pArray->begin(); iter != var.m_pArray->end(); ++iter)
		{
			Var *pVar = New<Var>(**iter);

			if (NULL == pVar)
			{
				return Status::Set(Status_MemAllocFailed, "Failed to allocate var");
			}
			pVar->SetParent(this);

			m_pArray->push_back(pVar);
		}

		return Status_OK;
	}

	return Status::Set(Status_NotSupported, "Not supported");
}

bool Var::operator==(const Var &var) const
{
	return Equals(var);
}

bool Var::operator!=(const Var &var) const
{
	return !Equals(var);
}

bool Var::Equals(const Var &var, bool bIgnoreCase/* = true*/) const
{
	if (var.GetType() != GetType())
	{
		return false;
	}
	if (0 != cx_stricmp(var.GetName(), GetName()))
	{
		return false;
	}
	if (IsInvalid() || IsNull())
	{
		return true;
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
			return false;
		}
		for (ObjectVar::iterator iter = m_pObject->begin(); iter != m_pObject->end(); ++iter)
		{
			if (!var.IsObjectMember(iter->first))
			{
				return false;
			}
			if (!iter->second->Equals(var.GetObjectMember(iter->first)))
			{
				return false;
			}
		}

		return true;
	}
	else
	if (IsArray())
	{
		if (m_pArray->size() != var.m_pArray->size())
		{
			return false;
		}
		for (Size i = 0; i < m_pArray->size(); i++)
		{
			if (!(*m_pArray)[i]->Equals(*(*var.m_pArray)[i]))
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

const Char *Var::GetName() const
{
	Status::Clear();

	if (IsInvalid())
	{
		Status::Set(Status_InvalidCall, "Var is invalid");

		return "";
	}

	return m_sName.c_str();
}

StatusCode Var::SetName(const Char *szName)
{
	Status::Clear();

	if (IsInvalid())
	{
		return Status::Set(Status_InvalidCall, "Var is invalid");
	}
	if (NULL != GetParent() && GetParent()->IsObject())
	{
		GetParent()->HandleChildNameChange(this, m_sName.c_str(), szName);
	}
	m_sName = szName;

	return Status_OK;
}

void Var::HandleChildNameChange(Var *pChild, const Char *szOldName, const Char *szNewName)
{
	ObjectVar::iterator iter = m_pObject->find(szOldName);

	m_pObject->erase(iter);
	(*m_pObject)[szNewName] = pChild;
}

Var::Type Var::GetType() const
{
	Status::Clear();

	return m_nType;
}

StatusCode Var::SetType(Type nType)
{
	Status::Clear();

	if (IsInvalid())
	{
		return Status::Set(Status_InvalidCall, "Var is invalid");
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
			return Status::Set(Status_MemAllocFailed, "Failed to alloc string");
		}
	}
	else
	if (Type_Object != m_nType && Type_Object == nType)
	{
		if (NULL == (m_pObject = New<ObjectVar>()))
		{
			return Status::Set(Status_MemAllocFailed, "Failed to alloc object");
		}
	}
	else
	if (Type_Array != m_nType && Type_Array == nType)
	{
		if (NULL == (m_pArray = New<ArrayVar>()))
		{
			return Status::Set(Status_MemAllocFailed, "Failed to alloc array");
		}
	}
	m_nType = nType;

	return Status_OK;
}

bool Var::IsInvalid() const
{
	Status::Clear();

	return (Type_Invalid == m_nType);
}

bool Var::IsValid() const
{
	Status::Clear();

	return (Type_Invalid != m_nType);
}

bool Var::IsNull() const
{
	Status::Clear();

	return (Type_Null == m_nType);
}

bool Var::IsBool() const
{
	Status::Clear();

	return (Type_Bool == m_nType);
}

bool Var::IsInt() const
{
	Status::Clear();

	return (Type_Int == m_nType);
}

bool Var::IsReal() const
{
	Status::Clear();

	return (Type_Real == m_nType);
}

bool Var::IsString() const
{
	Status::Clear();

	return (Type_String == m_nType);
}

bool Var::IsObject() const
{
	Status::Clear();

	return (Type_Object == m_nType);
}

bool Var::IsArray() const
{
	Status::Clear();

	return (Type_Array == m_nType);
}

StatusCode Var::SetNull()
{
	Status::Clear();

	if (IsInvalid())
	{
		return Status::Set(Status_InvalidCall, "Var is invalid");
	}
	
	return SetType(Type_Null);
}

StatusCode Var::SetBool(bool bBool)
{
	Status::Clear();

	if (IsInvalid())
	{
		return Status::Set(Status_InvalidCall, "Var is invalid");
	}
	if (CXNOK(SetType(Type_Bool)))
	{
		return Status::GetCode();
	}
	m_bBool = bBool;

	return Status_OK;
}

bool Var::GetBool(bool bBoolDefault/* = DEFAULT_BOOL*/) const
{
	Status::Clear();

	if (IsInvalid())
	{
		Status::Set(Status_InvalidCall, "Var is invalid");

		return bBoolDefault;
	}
	if (IsBool())
	{
		return m_bBool;
	}

	return bBoolDefault;
}

StatusCode Var::SetInt(Int64 nInt)
{
	Status::Clear();

	if (IsInvalid())
	{
		return Status::Set(Status_InvalidCall, "Var is invalid");
	}
	if (CXNOK(SetType(Type_Int)))
	{
		return Status::GetCode();
	}
	m_nInt = nInt;

	return Status_OK;
}

Int64 Var::GetInt(Int64 nIntDefault/* = DEFAULT_INT*/) const
{
	Status::Clear();

	if (IsInvalid())
	{
		Status::Set(Status_InvalidCall, "Var is invalid");

		return nIntDefault;
	}
	if (IsInt())
	{
		return m_nInt;
	}

	return nIntDefault;
}

StatusCode Var::SetReal(Double lfReal)
{
	Status::Clear();

	if (IsInvalid())
	{
		return Status::Set(Status_InvalidCall, "Var is invalid");
	}
	if (CXNOK(SetType(Type_Real)))
	{
		return Status::GetCode();
	}
	m_lfReal = lfReal;

	return Status_OK;
}

Double Var::GetReal(Double lfRealDefault/* = DEFAULT_REAL*/) const
{
	Status::Clear();

	if (IsInvalid())
	{
		Status::Set(Status_InvalidCall, "Var is invalid");

		return lfRealDefault;
	}
	if (IsReal())
	{
		return m_lfReal;
	}

	return lfRealDefault;
}

StatusCode Var::SetString(const Char *szString)
{
	Status::Clear();

	if (IsInvalid())
	{
		return Status::Set(Status_InvalidCall, "Var is invalid");
	}
	if (CXNOK(SetType(Type_String)))
	{
		return Status::GetCode();
	}
	*m_psString = szString;

	return Status_OK;
}

StatusCode Var::SetString(const String &sString)
{
	Status::Clear();

	if (IsInvalid())
	{
		return Status::Set(Status_InvalidCall, "Var is invalid");
	}
	if (CXNOK(SetType(Type_String)))
	{
		return Status::GetCode();
	}
	*m_psString = sString;

	return Status_OK;
}

const Char *Var::GetString(const Char *szStringDefault/* = DEFAULT_STRING*/) const
{
	Status::Clear();

	if (IsInvalid())
	{
		Status::Set(Status_InvalidCall, "Var is invalid");

		return szStringDefault;
	}
	if (IsString())
	{
		return m_psString->c_str();
	}

	return szStringDefault;
}

StatusCode Var::SetObject()
{
	Status::Clear();

	if (IsInvalid())
	{
		return Status::Set(Status_InvalidCall, "Var is invalid");
	}

	return SetType(Type_Object);
}

StatusCode Var::SetArray()
{
	Status::Clear();

	if (IsInvalid())
	{
		return Status::Set(Status_InvalidCall, "Var is invalid");
	}

	return SetType(Type_Array);
}

bool Var::IsObjectMember(const char *szName) const
{
	Status::Clear();

	if (!IsObject())
	{
		Status::Set(Status_InvalidCall, "Var is not an object");

		return false;
	}

	return (m_pObject->end() != m_pObject->find(szName));
}

bool Var::IsObjectMember(const String &sName) const
{
	return IsObjectMember(sName.c_str());
}

bool Var::IsArrayItem(Size cIndex) const
{
	Status::Clear();

	if (!IsArray())
	{
		Status::Set(Status_InvalidCall, "Var is not an array");

		return false;
	}

	return (m_pObject->size() > cIndex);
}

Size Var::GetObjectMembersCount() const
{
	Status::Clear();

	if (!IsObject())
	{
		Status::Set(Status_InvalidCall, "Var is not an object");

		return 0;
	}

	return m_pObject->size();
}

Size Var::GetArrayItemsCount() const
{
	Status::Clear();

	if (!IsArray())
	{
		Status::Set(Status_InvalidCall, "Var is not an array");

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
	Status::Clear();

	if (!IsObject())
	{
		Status::Set(Status_InvalidCall, "Var is not an object");

		return ObjectIterator(NULL);
	}

	return ObjectIterator(m_pObject);
}

Var::ObjectConstIterator Var::GetObjectConstIterator() const
{
	Status::Clear();

	if (!IsObject())
	{
		Status::Set(Status_InvalidCall, "Var is not an object");

		return ObjectConstIterator(NULL);
	}

	return ObjectConstIterator(m_pObject);
}

Var::ArrayIterator Var::GetArrayIterator()
{
	Status::Clear();

	if (!IsArray())
	{
		Status::Set(Status_InvalidCall, "Var is not an array");

		return ArrayIterator(NULL);
	}

	return ArrayIterator(m_pArray);
}

Var::ArrayConstIterator Var::GetArrayConstIterator() const
{
	Status::Clear();

	if (!IsArray())
	{
		Status::Set(Status_InvalidCall, "Var is not an array");

		return ArrayConstIterator(NULL);
	}

	return ArrayConstIterator(m_pArray);
}

Var &Var::GetObjectMember(const char *szName)
{
	Status::Clear();

	if (CXNOK(SetObject()))
	{
		return INVALID_VAR;
	}

	ObjectVar::iterator iter = m_pObject->find(szName);

	if (m_pObject->end() == iter)
	{
		Var *pVar = New<Var>();

		if (NULL == pVar)
		{
			Status::Set(Status_MemAllocFailed, "Failed to allocate var");

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
	Status::Clear();

	if (!IsObject())
	{
		Status::Set(Status_NotFound, "Member not found");

		return INVALID_VAR;
	}

	ObjectVar::iterator iter = m_pObject->find(szName);

	if (m_pObject->end() == iter)
	{
		Status::Set(Status_NotFound, "Member not found");

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
	Status::Clear();

	if (CXNOK(SetArray()))
	{
		return INVALID_VAR;
	}

	if (cIndex < 0 || cIndex >= (int)m_pArray->size())
	{
		Var *pVar = New<Var>();

		if (NULL == pVar)
		{
			Status::Set(Status_MemAllocFailed, "Failed to allocate var");

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
	Status::Clear();

	if (!IsArray())
	{
		Status::Set(Status_NotFound, "Item not found");

		return INVALID_VAR;
	}

	if (cIndex < 0 || cIndex >= (int)m_pArray->size())
	{
		Status::Set(Status_NotFound, "Item not found");

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

Var &Var::operator=(bool bBool)
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

Var::operator bool() const
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
	Status::Clear();

	m_pVar = pVar;
	m_iter = m_pVar->begin();
}

bool Var::ObjectIterator::IsValid() const
{
	Status::Clear();

	if (NULL == m_pVar)
	{
		return false;
	}

	return (m_pVar->end() != m_iter);
}

StatusCode Var::ObjectIterator::Reset()
{
	Status::Clear();

	if (NULL == m_pVar)
	{
		return Status::Set(Status_InvalidCall, "Iterator is invalid");
	}

	m_iter = m_pVar->begin();

	return Status_OK;
}

StatusCode Var::ObjectIterator::Next()
{
	Status::Clear();

	if (NULL == m_pVar)
	{
		return Status::Set(Status_InvalidCall, "Iterator is invalid");
	}
	if (m_pVar->end() == m_iter)
	{
		return Status::Set(Status_InvalidCall, "Iterator is invalid");
	}
	m_iter++;
	if (m_pVar->end() == m_iter)
	{
		return Status::Set(Status_InvalidCall, "Iterator is invalid");
	}

	return Status_OK;
}

Var &Var::ObjectIterator::Get()
{
	Status::Clear();

	if (NULL == m_pVar)
	{
		Status::Set(Status_InvalidCall, "Iterator is invalid");

		return INVALID_VAR;
	}
	if (m_pVar->end() == m_iter)
	{
		Status::Set(Status_InvalidCall, "Iterator is invalid");

		return INVALID_VAR;
	}

	return *m_iter->second;
}

Var::ObjectConstIterator::ObjectConstIterator(const ObjectVar *pVar)
{
	Status::Clear();

	m_pVar = pVar;
	m_iter = m_pVar->begin();
}

bool Var::ObjectConstIterator::IsValid() const
{
	Status::Clear();

	if (NULL == m_pVar)
	{
		return false;
	}

	return (m_pVar->end() != m_iter);
}

StatusCode Var::ObjectConstIterator::Reset()
{
	Status::Clear();

	if (NULL == m_pVar)
	{
		return Status::Set(Status_InvalidCall, "Iterator is invalid");
	}

	m_iter = m_pVar->begin();

	return Status_OK;
}

StatusCode Var::ObjectConstIterator::Next()
{
	Status::Clear();

	if (NULL == m_pVar)
	{
		return Status::Set(Status_InvalidCall, "Iterator is invalid");
	}
	if (m_pVar->end() == m_iter)
	{
		return Status::Set(Status_InvalidCall, "Iterator is invalid");
	}
	m_iter++;
	if (m_pVar->end() == m_iter)
	{
		return Status::Set(Status_InvalidCall, "Iterator is invalid");
	}

	return Status_OK;
}

const Var &Var::ObjectConstIterator::Get() const
{
	Status::Clear();

	if (NULL == m_pVar)
	{
		Status::Set(Status_InvalidCall, "Iterator is invalid");

		return INVALID_VAR;
	}
	if (m_pVar->end() == m_iter)
	{
		Status::Set(Status_InvalidCall, "Iterator is invalid");

		return INVALID_VAR;
	}

	return *m_iter->second;
}

Var::ArrayIterator::ArrayIterator(ArrayVar *pVar)
{
	Status::Clear();

	m_pVar = pVar;
	m_iter = m_pVar->begin();
}

bool Var::ArrayIterator::IsValid() const
{
	Status::Clear();

	if (NULL == m_pVar)
	{
		return false;
	}

	return (m_pVar->end() != m_iter);
}

StatusCode Var::ArrayIterator::Reset()
{
	Status::Clear();

	if (NULL == m_pVar)
	{
		return Status::Set(Status_InvalidCall, "Iterator is invalid");
	}

	m_iter = m_pVar->begin();

	return Status_OK;
}

StatusCode Var::ArrayIterator::Next()
{
	Status::Clear();

	if (NULL == m_pVar)
	{
		return Status::Set(Status_InvalidCall, "Iterator is invalid");
	}
	if (m_pVar->end() == m_iter)
	{
		return Status::Set(Status_InvalidCall, "Iterator is invalid");
	}
	m_iter++;
	if (m_pVar->end() == m_iter)
	{
		return Status::Set(Status_InvalidCall, "Iterator is invalid");
	}

	return Status_OK;
}

Var &Var::ArrayIterator::Get()
{
	Status::Clear();

	if (NULL == m_pVar)
	{
		Status::Set(Status_InvalidCall, "Iterator is invalid");

		return INVALID_VAR;
	}
	if (m_pVar->end() == m_iter)
	{
		Status::Set(Status_InvalidCall, "Iterator is invalid");

		return INVALID_VAR;
	}

	return **m_iter;
}

Var::ArrayConstIterator::ArrayConstIterator(const ArrayVar *pVar)
{
	Status::Clear();

	m_pVar = pVar;
	m_iter = m_pVar->begin();
}

bool Var::ArrayConstIterator::IsValid() const
{
	Status::Clear();

	if (NULL == m_pVar)
	{
		return false;
	}

	return (m_pVar->end() != m_iter);
}

StatusCode Var::ArrayConstIterator::Reset()
{
	Status::Clear();

	if (NULL == m_pVar)
	{
		return Status::Set(Status_InvalidCall, "Iterator is invalid");
	}

	m_iter = m_pVar->begin();

	return Status_OK;
}

StatusCode Var::ArrayConstIterator::Next()
{
	Status::Clear();

	if (NULL == m_pVar)
	{
		return Status::Set(Status_InvalidCall, "Iterator is invalid");
	}
	if (m_pVar->end() == m_iter)
	{
		return Status::Set(Status_InvalidCall, "Iterator is invalid");
	}
	m_iter++;
	if (m_pVar->end() == m_iter)
	{
		return Status::Set(Status_InvalidCall, "Iterator is invalid");
	}

	return Status_OK;
}

const Var &Var::ArrayConstIterator::Get() const
{
	Status::Clear();

	if (NULL == m_pVar)
	{
		Status::Set(Status_InvalidCall, "Iterator is invalid");

		return INVALID_VAR;
	}
	if (m_pVar->end() == m_iter)
	{
		Status::Set(Status_InvalidCall, "Iterator is invalid");

		return INVALID_VAR;
	}

	return **m_iter;
}

StatusCode Var::Read(IO::IInputStream *pInputStream)
{
	VarJSONSAXParserObserver   obs;
	Data::JSON::JSONSAXParser  parser;

	Status::Clear();

	obs.m_pRoot = obs.m_pCurrent = this;
	obs.m_bInit = true;
	parser.AddObserver(&obs);
	if (CXNOK(parser.ParseStream(pInputStream)))
	{
		return Status::GetCode();
	}

	return Status_OK;
}

StatusCode Var::Read(const String &sStr)
{
	Status::Clear();

	IO::MemInputStream mis(&sStr);

	return Read(&mis);
}

StatusCode Var::Write(IO::IOutputStream *pOutputStream)
{
	return Write(pOutputStream, 0, true);
}

StatusCode Var::Write(String &sStr)
{
	Status::Clear();

	IO::MemOutputStream mos(&sStr);

	return Write(&mos);
}

StatusCode Var::Write(IO::IOutputStream *pOutputStream, Size cIndent, bool bLast)
{
	Size cbSize;
	Size cbAckSize;
	Char szOutput[64];
	Char pIndent[256];
	Status::Clear();

	if (!IsValid())
	{
		return Status::Set(Status_InvalidCall, "Var is invalid");
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
	if (CXNOK(pOutputStream->Write(pIndent, cIndent, &cbSize)))
	{
		return Status::GetCode();
	}
	//name
	if (NULL != GetParent() && !GetParent()->IsArray())
	{
		if (CXNOK(pOutputStream->Write("\"", 1, &cbSize)))
		{
			return Status::GetCode();
		}
		if (CXNOK(WriteString(pOutputStream, m_sName.c_str(), m_sName.size())))
		{
			return Status::GetCode();
		}
		if (CXNOK(pOutputStream->Write("\": ", 3, &cbSize)))
		{
			return Status::GetCode();
		}
	}
	if (IsNull())
	{
		if (CXNOK(pOutputStream->Write("null", 4, &cbSize)))
		{
			return Status::GetCode();
		}
	}
	else
	if (IsBool())
	{
		if (!GetBool())
		{
			if (CXNOK(pOutputStream->Write("false", 5, &cbSize)))
			{
				return Status::GetCode();
			}
		}
		else
		{
			if (CXNOK(pOutputStream->Write("true", 4, &cbSize)))
			{
				return Status::GetCode();
			}
		}
	}
	else
	if (IsInt())
	{
		if (CXNOK(ToString(GetInt(), szOutput, sizeof(szOutput) / sizeof(szOutput[0]), &cbAckSize, 0)))
		{
			return Status::GetCode();
		}
		if (CXNOK(pOutputStream->Write(szOutput, cbAckSize, &cbSize)))
		{
			return Status::GetCode();
		}
	}
	else
	if (IsReal())
	{
		if (CXNOK(ToString(GetReal(), szOutput, sizeof(szOutput) / sizeof(szOutput[0]), &cbAckSize, 6)))
		{
			return Status::GetCode();
		}
		if (CXNOK(pOutputStream->Write(szOutput, cbAckSize, &cbSize)))
		{
			return Status::GetCode();
		}
	}
	else
	if (IsString())
	{
		if (CXNOK(pOutputStream->Write("\"", 1, &cbSize)))
		{
			return Status::GetCode();
		}
		if (CXNOK(WriteString(pOutputStream, m_psString->c_str(), m_psString->size())))
		{
			return Status::GetCode();
		}
		if (CXNOK(pOutputStream->Write("\"", 1, &cbSize)))
		{
			return Status::GetCode();
		}
	}
	else 
	if (IsObject())
	{
		if (CXNOK(pOutputStream->Write("\n", 1, &cbSize)))
		{
			return Status::GetCode();
		}
		if (CXNOK(pOutputStream->Write(pIndent, cIndent, &cbSize)))
		{
			return Status::GetCode();
		}
		if (CXNOK(pOutputStream->Write("{\n", 2, &cbSize)))
		{
			return Status::GetCode();
		}
		for (ObjectVar::iterator iter = m_pObject->begin(); iter != m_pObject->end(); ++iter)
		{
			ObjectVar::iterator iterEx = iter;

			iterEx++;
			if (CXNOK(iter->second->Write(pOutputStream, cIndent + 1, (m_pObject->end() == iterEx))))
			{
				return Status::GetCode();
			}
		}
		if (CXNOK(pOutputStream->Write(pIndent, cIndent, &cbSize)))
		{
			return Status::GetCode();
		}
		if (CXNOK(pOutputStream->Write("}", 1, &cbSize)))
		{
			return Status::GetCode();
		}
	}
	else 
	if (IsArray())
	{
		if (CXNOK(pOutputStream->Write("\n", 1, &cbSize)))
		{
			return Status::GetCode();
		}
		if (CXNOK(pOutputStream->Write(pIndent, cIndent, &cbSize)))
		{
			return Status::GetCode();
		}
		if (CXNOK(pOutputStream->Write("[\n", 2, &cbSize)))
		{
			return Status::GetCode();
		}
		for (ArrayVar::iterator iter = m_pArray->begin(); iter != m_pArray->end(); ++iter)
		{
			ArrayVar::iterator iterEx = iter;

			iterEx++;
			if (CXNOK((*iter)->Write(pOutputStream, cIndent + 1, (m_pArray->end() == iterEx))))
			{
				return Status::GetCode();
			}
		}
		if (CXNOK(pOutputStream->Write(pIndent, cIndent, &cbSize)))
		{
			return Status::GetCode();
		}
		if (CXNOK(pOutputStream->Write("]", 1, &cbSize)))
		{
			return Status::GetCode();
		}
	}
	if (bLast)
	{
		if (CXNOK(pOutputStream->Write("\n", 1, &cbSize)))
		{
			return Status::GetCode();
		}
	}
	else
	{
		if (CXNOK(pOutputStream->Write(",\n", 1, &cbSize)))
		{
			return Status::GetCode();
		}
	}

	return Status_OK;
}

unsigned read_code_point_from_utf8()
{
	int code_unit1, code_unit2, code_unit3, code_unit4;

	code_unit1 = getchar();
	if (code_unit1 < 0x80) {
		return code_unit1;
	}
	else if (code_unit1 < 0xC2) {
		/* continuation or overlong 2-byte sequence */
		goto ERROR1;
	}
	else if (code_unit1 < 0xE0) {
		/* 2-byte sequence */
		code_unit2 = getchar();
		if ((code_unit2 & 0xC0) != 0x80) goto ERROR2;
		return (code_unit1 << 6) + code_unit2 - 0x3080;
	}
	else if (code_unit1 < 0xF0) {
		/* 3-byte sequence */
		code_unit2 = getchar();
		if ((code_unit2 & 0xC0) != 0x80) goto ERROR2;
		if (code_unit1 == 0xE0 && code_unit2 < 0xA0) goto ERROR2; /* overlong */
		code_unit3 = getchar();
		if ((code_unit3 & 0xC0) != 0x80) goto ERROR3;
		return (code_unit1 << 12) + (code_unit2 << 6) + code_unit3 - 0xE2080;
	}
	else if (code_unit1 < 0xF5) {
		/* 4-byte sequence */
		code_unit2 = getchar();
		if ((code_unit2 & 0xC0) != 0x80) goto ERROR2;
		if (code_unit1 == 0xF0 && code_unit2 < 0x90) goto ERROR2; /* overlong */
		if (code_unit1 == 0xF4 && code_unit2 >= 0x90) goto ERROR2; /* > U+10FFFF */
		code_unit3 = getchar();
		if ((code_unit3 & 0xC0) != 0x80) goto ERROR3;
		code_unit4 = getchar();
		if ((code_unit4 & 0xC0) != 0x80) goto ERROR4;
		return (code_unit1 << 18) + (code_unit2 << 12) + (code_unit3 << 6) + code_unit4 - 0x3C82080;
	}
	else {
		/* > U+10FFFF */
		goto ERROR1;
	}

ERROR4:
	ungetc(code_unit4, stdin);
ERROR3:
	ungetc(code_unit3, stdin);
ERROR2:
	ungetc(code_unit2, stdin);
ERROR1:
	return code_unit1 + 0xDC00;
}

StatusCode Var::WriteString(IO::IOutputStream *pOutputStream, const Char *pBuffer, Size cLen)
{
	static Char hexdigits[] = 
	{ 
		'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F' 
	};
	const Byte *pPos;
	Char       escape[6];
	Size       cbAckSize;

	Status::Clear();

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
			if (CXNOK(pOutputStream->Write(escape, 6, &cbAckSize)))
			{
				return Status::GetCode();
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
				if (CXNOK(pOutputStream->Write(escape, 2, &cbAckSize)))
				{
					return Status::GetCode();
				}
				cLen--;
				pPos++;
			}
			else
			{
				if (CXNOK(pOutputStream->Write(pPos, 1, &cbAckSize)))
				{
					return Status::GetCode();
				}
				cLen--;
				pPos++;
			}
		}
		else
		if (0xC2 > *pPos)
		{
			return Status::Set(Status_InvalidArg, "Invalid UTF8 string");
		}
		else
		{
			unsigned cp = 0;

			if (0xE0 > *pPos) //2 bytes
			{
				if (2 > cLen)
				{
					return Status::Set(Status_InvalidArg, "Invalid UTF8 string");
				}
				if ((pPos[1] & 0xC0) != 0x80)
				{
					return Status::Set(Status_InvalidArg, "Invalid UTF8 string");
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
					return Status::Set(Status_InvalidArg, "Invalid UTF8 string");
				}
				if ((pPos[1] & 0xC0) != 0x80)
				{
					return Status::Set(Status_InvalidArg, "Invalid UTF8 string");
				}
				if (pPos[0] == 0xE0 && pPos[1] < 0xA0)
				{
					return Status::Set(Status_InvalidArg, "Invalid UTF8 string");
				}
				if ((pPos[2] & 0xC0) != 0x80)
				{
					return Status::Set(Status_InvalidArg, "Invalid UTF8 string");
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
					return Status::Set(Status_InvalidArg, "Invalid UTF8 string");
				}
				if ((pPos[1] & 0xC0) != 0x80)
				{
					return Status::Set(Status_InvalidArg, "Invalid UTF8 string");
				}
				if (pPos[0] == 0xF0 && pPos[1] < 0x90)
				{
					return Status::Set(Status_InvalidArg, "Invalid UTF8 string");
				}
				if (pPos[0] == 0xF4 && pPos[1] >= 0x90)
				{
					return Status::Set(Status_InvalidArg, "Invalid UTF8 string");
				}
				if ((pPos[2] & 0xC0) != 0x80)
				{
					return Status::Set(Status_InvalidArg, "Invalid UTF8 string");
				}
				if ((pPos[3] & 0xC0) != 0x80)
				{
					return Status::Set(Status_InvalidArg, "Invalid UTF8 string");
				}

				cp = (pPos[0] << 18) + (pPos[1] << 12) + (pPos[2] << 6) + pPos[3] - 0x3C82080;

				pPos += 4;
				cLen -= 4;
			}
			else
			{
				return Status::Set(Status_InvalidArg, "Invalid UTF8 string");
			}

			if (cp <= 0xD7FF || (cp >= 0xE000 && cp <= 0xFFFF)) 
			{
				escape[0] = '\\';
				escape[1] = 'u';
				escape[2] = hexdigits[((Byte *)&cp)[1] / 16];
				escape[3] = hexdigits[((Byte *)&cp)[1] % 16];
				escape[4] = hexdigits[((Byte *)&cp)[0] / 16];
				escape[5] = hexdigits[((Byte *)&cp)[0] % 16];
				if (CXNOK(pOutputStream->Write(escape, 6, &cbAckSize)))
				{
					return Status::GetCode();
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
				if (CXNOK(pOutputStream->Write(escape, 6, &cbAckSize)))
				{
					return Status::GetCode();
				}

				escape[0] = '\\';
				escape[1] = 'u';
				escape[2] = hexdigits[((Byte *)&trail)[1] / 16];
				escape[3] = hexdigits[((Byte *)&trail)[1] % 16];
				escape[4] = hexdigits[((Byte *)&trail)[0] / 16];
				escape[5] = hexdigits[((Byte *)&trail)[0] % 16];
				if (CXNOK(pOutputStream->Write(escape, 6, &cbAckSize)))
				{
					return Status::GetCode();
				}
			}
			else
			{
				return Status::Set(Status_InvalidArg, "Invalid UTF8 string");
			}
		}
	}

	return Status_OK;
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

