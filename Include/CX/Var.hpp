/* 
 * CX - C++ framework for general purpose development
 *
 * https://github.com/draede/cx
 * 
 * Copyright (C) 2014 - 2016 draede - draede [at] outlook [dot] com
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


#include "CX/Mem.hpp"
#include "CX/Types.hpp"
#include "CX/String.hpp"
#include "CX/Map.hpp"
#include "CX/Vector.hpp"
#include "CX/Stack.hpp"
#include "CX/IO/IInputStream.hpp"
#include "CX/IO/IOutputStream.hpp"
#include "CX/APIDefs.hpp"
#include "CX/IObject.hpp"


namespace CX
{

class CX_API Var : public IObject
{
private:

	typedef Vector<Var *>::Type                                    VarsVector;
	typedef Map<String, Var *, CaseInsensitiveOrderPolicy>::Type   VarsMap;
	
	typedef struct _VarEx
	{
		_VarEx()
		{
		}

		_VarEx(const String &sName, Var *pVar)
		{
			this->sName = sName;
			this->pVar  = pVar;
		}

		_VarEx(const Char *szName, Var *pVar)
		{
			this->sName = szName;
			this->pVar  = pVar;
		}

		String sName;
		Var    *pVar;
	}VarEx;

	typedef Vector<VarEx>::Type                                     VarExsVector;

	typedef struct _ArrayVar
	{
		VarsVector    vectorVars;
	}ArrayVar;

	typedef struct _ObjectVar
	{
		VarsMap       mapVars;
		VarExsVector  vectorVarExs;
	}ObjectVar;

public:

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

	Var(Type nType = Type_Null);

	~Var();

	Var(Bool bBool);

	Var(Int64 nInt);

	Var(Double lfReal);

	Var(const Char *szString);

	Var(const String &sString);

	Var(const Var &var);

	Var &operator=(const Var &var);

	Status Copy(const Var &var);

	Bool operator==(const Var &var) const;

	Bool operator!=(const Var &var) const;

	Bool Equals(const Var &var, Bool bIgnoreCase = True) const;

	const Char *GetName() const;

	Size GetNameLen() const;

	Status SetName(const Char *szName);

	Type GetType() const;

	Status SetType(Type nType);

	Bool IsInvalid() const;

	Bool IsValid() const;

	Bool IsNull() const;

	Bool IsBool() const;

	Bool IsInt() const;

	Bool IsReal() const;

	Bool IsString() const;

	Bool IsObject() const;

	Bool IsArray() const;

	Status SetNull();

	Status SetBool(Bool bBool);

	Bool GetBool(Bool bBoolDefault = DEFAULT_BOOL) const;

	Status SetInt(Int64 nInt);

	Int64 GetInt(Int64 nIntDefault = DEFAULT_INT) const;

	Status SetReal(Double lfReal);

	Double GetReal(Double lfRealDefault = DEFAULT_REAL) const;

	Status SetString(const Char *szString);

	Status SetString(const String &sString);

	const Char *GetString(const Char *szStringDefault = DEFAULT_STRING) const;

	Size GetStringLen() const;

	Status SetObject();

	Status SetArray();

	Bool IsObjectMember(const char *szName) const;

	Bool IsObjectMember(const String &sName) const;

	Bool IsArrayItem(Size cIndex) const;

	Size GetObjectMembersCount() const;

	Size GetArrayItemsCount() const;

	Var *GetParent() const;

	class ObjectIterator : public IObject
	{
	public:

		ObjectIterator();

		Bool IsValid() const;

		Status Reset();

		Status Next();

		Var &Get();

	protected:

		friend class Var;

		ObjectIterator(ObjectVar *pVar);

	private:

		ObjectVar                *m_pVar;
		VarExsVector::iterator   m_iter;

	};

	class ObjectConstIterator : public IObject
	{
	public:

		ObjectConstIterator();

		Bool IsValid() const;

		Status Reset();

		Status Next();

		const Var &Get() const;

	protected:

		friend class Var;

		ObjectConstIterator(const ObjectVar *pVar);

	private:

		const ObjectVar                *m_pVar;
		VarExsVector::const_iterator   m_iter;

	};

	ObjectIterator GetObjectIterator();

	ObjectConstIterator GetObjectConstIterator() const;

	class ArrayIterator : public IObject
	{
	public:

		ArrayIterator();

		Bool IsValid() const;

		Status Reset();

		Status Next();

		Var &Get();

	protected:

		friend class Var;

		ArrayIterator(ArrayVar *pVar);

	private:

		ArrayVar               *m_pVar;
		VarsVector::iterator   m_iter;

	};

	class ArrayConstIterator : public IObject
	{
	public:

		ArrayConstIterator();

		Bool IsValid() const;

		Status Reset();

		Status Next();

		const Var &Get() const;

	protected:

		friend class Var;

		ArrayConstIterator(const ArrayVar *pVar);

	private:

		const ArrayVar              *m_pVar;
		VarsVector::const_iterator   m_iter;

	};

	ArrayIterator GetArrayIterator();

	ArrayConstIterator GetArrayConstIterator() const;

	Var &GetObjectMember(const char *szName);

	const Var &GetObjectMember(const char *szName) const;

	Var &GetObjectMember(const String &sName);

	const Var &GetObjectMember(const String &sName) const;

	Var &GetArrayItem(int cIndex = -1);

	const Var &GetArrayItem(int cIndex) const;

	Var &operator[](const char *szName);

	Var &operator[](const String &sName);

	Var &operator[](int cIndex);

	Var &operator=(Bool bBool);

	Var &operator=(Int64 nInt);

	Var &operator=(Double lfReal);

	Var &operator=(const char *szString);

	Var &operator=(const String &sString);

	operator Bool () const;

	operator Int64 () const;

	operator Double () const;

	operator const Char * () const;

	Status Read(IO::IInputStream *pInputStream);

	Status Read(const String &sStr);

	Status Write(IO::IOutputStream *pOutputStream);

	Status Write(String &sStr);

protected:

	friend class VarJSONSAXParserObserver;

	void SetParent(Var *pParent);

	void HandleChildNameChange(Var *pChild, const Char *szOldName, const Char *szNewName);

private:

	static const Bool         DEFAULT_BOOL = False;
	static const Int64        DEFAULT_INT  = 0;
	static const Double       DEFAULT_REAL;
	static const Char         *DEFAULT_STRING;

	static Var INVALID_VAR;

	Type     m_nType;

#pragma warning(push)
#pragma warning(disable: 4251)
	String   m_sName;
#pragma warning(pop)

	union
	{
		Bool        m_bBool;
		Int64       m_nInt;
		Double      m_lfReal;
		String      *m_psString;
		ObjectVar   *m_pObject;
		ArrayVar    *m_pArray;
	};

	Var *m_pParent;

	Var(Bool a1, Bool a2, Bool a3, Bool a4, Bool a5);

	typedef struct _WriteNoRecData
	{
		const Var            *pVar;
		ObjectConstIterator  iterObject;
		ArrayConstIterator   iterArray;
	}WriteNoRecData;

	typedef Stack<WriteNoRecData>::Type   WriteNoRecDataStack;

	static Status WriteNoRec(const Var *pVar, IO::IOutputStream *pOutputStream);

	static Status WriteNoRecName(const Var *pVar, IO::IOutputStream *pOutputStream);

	static Status WriteNoRecScalar(const Var *pVar, IO::IOutputStream *pOutputStream, Bool bLast);

	static Status WriteString(IO::IOutputStream *pOutputStream, const Char *pBuffer, Size cLen);

};

void operator>>(Var &var, String &sStr);

void operator<<(String &sStr, Var &var);

void operator>>(Var &var, IO::IOutputStream &os);

void operator<<(IO::IOutputStream &os, Var &var);

void operator<<(Var &var, const String &sStr);

void operator>>(const String &sStr, Var &var);

void operator<<(Var &var, IO::IInputStream &is);

void operator>>(IO::IInputStream &is, Var &var);

}//namespace CX

