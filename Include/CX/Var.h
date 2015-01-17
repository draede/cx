
#pragma once


#include "CX/Alloc.h"
#include "CX/Types.h"
#include "CX/String.h"
#include "CX/Map.h"
#include "CX/Vector.h"
#include "CX/IO/IInputStream.h"
#include "CX/IO/IOutputStream.h"
#include "CX/APIDefs.h"


namespace CX
{

class CX_API Var
{
private:

	typedef Vector<Var *>::Type                                    ArrayVar;
	typedef Map<String, Var *, CaseInsensitiveOrderPolicy>::Type   ObjectVar;

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

	Var(bool bBool);

	Var(Int64 nInt);

	Var(Double lfReal);

	Var(const Char *szString);

	Var(const String &sString);

	Var(const Var &var);

	Var &operator=(const Var &var);

	StatusCode Copy(const Var &var);

	bool operator==(const Var &var) const;

	bool operator!=(const Var &var) const;

	bool Equals(const Var &var, bool bIgnoreCase = true) const;

	const Char *GetName() const;

	StatusCode SetName(const Char *szName);

	Type GetType() const;

	StatusCode SetType(Type nType);

	bool IsInvalid() const;

	bool IsValid() const;

	bool IsNull() const;

	bool IsBool() const;

	bool IsInt() const;

	bool IsReal() const;

	bool IsString() const;

	bool IsObject() const;

	bool IsArray() const;

	StatusCode SetNull();

	StatusCode SetBool(bool bBool);

	bool GetBool(bool bBoolDefault = DEFAULT_BOOL) const;

	StatusCode SetInt(Int64 nInt);

	Int64 GetInt(Int64 nIntDefault = DEFAULT_INT) const;

	StatusCode SetReal(Double lfReal);

	Double GetReal(Double lfRealDefault = DEFAULT_REAL) const;

	StatusCode SetString(const Char *szString);

	StatusCode SetString(const String &sString);

	const Char *GetString(const Char *szStringDefault = DEFAULT_STRING) const;

	StatusCode SetObject();

	StatusCode SetArray();

	bool IsObjectMember(const char *szName) const;

	bool IsObjectMember(const String &sName) const;

	bool IsArrayItem(Size cIndex) const;

	Size GetObjectMembersCount() const;

	Size GetArrayItemsCount() const;

	Var *GetParent() const;

	class ObjectIterator
	{
	public:

		bool IsValid() const;

		StatusCode Reset();

		StatusCode Next();

		Var &Get();

	protected:

		friend class Var;

		ObjectIterator(ObjectVar *pVar);

	private:

		ObjectVar             *m_pVar;
		ObjectVar::iterator   m_iter;

	};

	class ObjectConstIterator
	{
	public:

		bool IsValid() const;

		StatusCode Reset();

		StatusCode Next();

		const Var &Get() const;

	protected:

		friend class Var;

		ObjectConstIterator(const ObjectVar *pVar);

	private:

		const ObjectVar             *m_pVar;
		ObjectVar::const_iterator   m_iter;

	};

	ObjectIterator GetObjectIterator();

	ObjectConstIterator GetObjectConstIterator() const;

	class ArrayIterator
	{
	public:

		bool IsValid() const;

		StatusCode Reset();

		StatusCode Next();

		Var &Get();

	protected:

		friend class Var;

		ArrayIterator(ArrayVar *pVar);

	private:

		ArrayVar             *m_pVar;
		ArrayVar::iterator   m_iter;

	};

	class ArrayConstIterator
	{
	public:

		bool IsValid() const;

		StatusCode Reset();

		StatusCode Next();

		const Var &Get() const;

	protected:

		friend class Var;

		ArrayConstIterator(const ArrayVar *pVar);

	private:

		const ArrayVar             *m_pVar;
		ArrayVar::const_iterator   m_iter;

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

	Var &operator=(bool bBool);

	Var &operator=(Int64 nInt);

	Var &operator=(Double lfReal);

	Var &operator=(const char *szString);

	Var &operator=(const String &sString);

	operator bool () const;

	operator Int64 () const;

	operator Double () const;

	operator const Char * () const;

	StatusCode Read(IO::IInputStream *pInputStream);

	StatusCode Read(const String &sStr);

	StatusCode Write(IO::IOutputStream *pOutputStream);

	StatusCode Write(String &sStr);

protected:

	friend class VarJSONSAXParserObserver;

	void SetParent(Var *pParent);

	void HandleChildNameChange(Var *pChild, const Char *szOldName, const Char *szNewName);

private:

	typedef Vector<Var *>::Type                                    ArrayVar;
	typedef Map<String, Var *, CaseInsensitiveOrderPolicy>::Type   ObjectVar;

	static const bool         DEFAULT_BOOL = false;
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
		bool        m_bBool;
		Int64       m_nInt;
		Double      m_lfReal;
		String      *m_psString;
		ObjectVar   *m_pObject;
		ArrayVar    *m_pArray;
	};

	Var *m_pParent;

	Var(bool a1, bool a2, bool a3, bool a4, bool a5);

	StatusCode Write(IO::IOutputStream *pOutputStream, Size cIndent, bool bLast);

	StatusCode WriteString(IO::IOutputStream *pOutputStream, const Char *pBuffer, Size cLen);

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

