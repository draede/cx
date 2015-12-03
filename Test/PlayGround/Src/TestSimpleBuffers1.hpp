
#pragma once


#include "CX/Types.hpp"
#include "CX/Status.hpp"
#include "CX/String.hpp"
#include "CX/Vector.hpp"
#include "CX/Set.hpp"
#include "CX/Map.hpp"
#include "CX/BLOB.hpp"
#include "CX/BitSet.hpp"
#include "CX/C/string.h"
#include "CX/SimpleBuffers/MemberType.hpp"
#include "CX/SimpleBuffers/DataWriter.hpp"
#include "CX/SimpleBuffers/DataReader.hpp"
#include "CX/SimpleBuffers/DataIniter.hpp"
#include "CX/SimpleBuffers/ISimpleBuffer.hpp"


class TestSimpleBuffers1 : public CX::SimpleBuffers::ISimpleBuffer
{
public:

	TestSimpleBuffers1()
	{
		Init();
	}

	virtual void Init()
	{
		CX::SimpleBuffers::DataIniter<CX::SimpleBuffers::MemberType_Scalar, CX::Bool>::Init(this->m_value_bool);
		CX::SimpleBuffers::DataIniter<CX::SimpleBuffers::MemberType_Scalar, CX::Int8>::Init(this->m_value_int8);
		CX::SimpleBuffers::DataIniter<CX::SimpleBuffers::MemberType_Scalar, CX::UInt8>::Init(this->m_value_uint8);
		CX::SimpleBuffers::DataIniter<CX::SimpleBuffers::MemberType_Scalar, CX::Int16>::Init(this->m_value_int16);
		CX::SimpleBuffers::DataIniter<CX::SimpleBuffers::MemberType_Scalar, CX::UInt16>::Init(this->m_value_uint16);
		CX::SimpleBuffers::DataIniter<CX::SimpleBuffers::MemberType_Scalar, CX::Int32>::Init(this->m_value_int32);
		CX::SimpleBuffers::DataIniter<CX::SimpleBuffers::MemberType_Scalar, CX::UInt32>::Init(this->m_value_uint32);
		CX::SimpleBuffers::DataIniter<CX::SimpleBuffers::MemberType_Scalar, CX::Int64>::Init(this->m_value_int64);
		CX::SimpleBuffers::DataIniter<CX::SimpleBuffers::MemberType_Scalar, CX::UInt64>::Init(this->m_value_uint64);
		CX::SimpleBuffers::DataIniter<CX::SimpleBuffers::MemberType_Scalar, CX::Float>::Init(this->m_value_float);
		CX::SimpleBuffers::DataIniter<CX::SimpleBuffers::MemberType_Scalar, CX::Double>::Init(this->m_value_double);
		CX::SimpleBuffers::DataIniter<CX::SimpleBuffers::MemberType_Scalar, CX::String>::Init(this->m_value_string);
		CX::SimpleBuffers::DataIniter<CX::SimpleBuffers::MemberType_Scalar, CX::BLOB>::Init(this->m_value_blob);
	}

	const CX::Bool &Getvalue_bool() const
	{
		return this->m_value_bool;
	}

	CX::Bool &Getvalue_bool()
	{
		return this->m_value_bool;
	}

	void Setvalue_bool(const CX::Bool &p)
	{
		this->m_value_bool = p;
	}

	const CX::Int8 &Getvalue_int8() const
	{
		return this->m_value_int8;
	}

	CX::Int8 &Getvalue_int8()
	{
		return this->m_value_int8;
	}

	void Setvalue_int8(const CX::Int8 &p)
	{
		this->m_value_int8 = p;
	}

	const CX::UInt8 &Getvalue_uint8() const
	{
		return this->m_value_uint8;
	}

	CX::UInt8 &Getvalue_uint8()
	{
		return this->m_value_uint8;
	}

	void Setvalue_uint8(const CX::UInt8 &p)
	{
		this->m_value_uint8 = p;
	}

	const CX::Int16 &Getvalue_int16() const
	{
		return this->m_value_int16;
	}

	CX::Int16 &Getvalue_int16()
	{
		return this->m_value_int16;
	}

	void Setvalue_int16(const CX::Int16 &p)
	{
		this->m_value_int16 = p;
	}

	const CX::UInt16 &Getvalue_uint16() const
	{
		return this->m_value_uint16;
	}

	CX::UInt16 &Getvalue_uint16()
	{
		return this->m_value_uint16;
	}

	void Setvalue_uint16(const CX::UInt16 &p)
	{
		this->m_value_uint16 = p;
	}

	const CX::Int32 &Getvalue_int32() const
	{
		return this->m_value_int32;
	}

	CX::Int32 &Getvalue_int32()
	{
		return this->m_value_int32;
	}

	void Setvalue_int32(const CX::Int32 &p)
	{
		this->m_value_int32 = p;
	}

	const CX::UInt32 &Getvalue_uint32() const
	{
		return this->m_value_uint32;
	}

	CX::UInt32 &Getvalue_uint32()
	{
		return this->m_value_uint32;
	}

	void Setvalue_uint32(const CX::UInt32 &p)
	{
		this->m_value_uint32 = p;
	}

	const CX::Int64 &Getvalue_int64() const
	{
		return this->m_value_int64;
	}

	CX::Int64 &Getvalue_int64()
	{
		return this->m_value_int64;
	}

	void Setvalue_int64(const CX::Int64 &p)
	{
		this->m_value_int64 = p;
	}

	const CX::UInt64 &Getvalue_uint64() const
	{
		return this->m_value_uint64;
	}

	CX::UInt64 &Getvalue_uint64()
	{
		return this->m_value_uint64;
	}

	void Setvalue_uint64(const CX::UInt64 &p)
	{
		this->m_value_uint64 = p;
	}

	const CX::Float &Getvalue_float() const
	{
		return this->m_value_float;
	}

	CX::Float &Getvalue_float()
	{
		return this->m_value_float;
	}

	void Setvalue_float(const CX::Float &p)
	{
		this->m_value_float = p;
	}

	const CX::Double &Getvalue_double() const
	{
		return this->m_value_double;
	}

	CX::Double &Getvalue_double()
	{
		return this->m_value_double;
	}

	void Setvalue_double(const CX::Double &p)
	{
		this->m_value_double = p;
	}

	const CX::String &Getvalue_string() const
	{
		return this->m_value_string;
	}

	CX::String &Getvalue_string()
	{
		return this->m_value_string;
	}

	void Setvalue_string(const CX::String &p)
	{
		this->m_value_string = p;
	}

	const CX::BLOB &Getvalue_blob() const
	{
		return this->m_value_blob;
	}

	CX::BLOB &Getvalue_blob()
	{
		return this->m_value_blob;
	}

	void Setvalue_blob(const CX::BLOB &p)
	{
		this->m_value_blob = p;
	}

	virtual CX::Size GetMembersCount()
	{
		return 13;
	}

	virtual CX::Bool GetMemberLoaded(CX::Size cIndex)
	{
		if (m_bitsetLoadedMembers.size() <= cIndex)
		{
			return CX::False;
		}

		return m_bitsetLoadedMembers[cIndex];
	}

	virtual CX::Bool GetMemberLoaded(const CX::Char *szName)
	{
#pragma warning(push)
#pragma warning(disable: 4996)
		if (0 == cx_strcmp("value_bool", szName))
#pragma warning(pop)
		{
			return m_bitsetLoadedMembers[0];
		}
#pragma warning(push)
#pragma warning(disable: 4996)
		if (0 == cx_strcmp("value_int8", szName))
#pragma warning(pop)
		{
			return m_bitsetLoadedMembers[1];
		}
#pragma warning(push)
#pragma warning(disable: 4996)
		if (0 == cx_strcmp("value_uint8", szName))
#pragma warning(pop)
		{
			return m_bitsetLoadedMembers[2];
		}
#pragma warning(push)
#pragma warning(disable: 4996)
		if (0 == cx_strcmp("value_int16", szName))
#pragma warning(pop)
		{
			return m_bitsetLoadedMembers[3];
		}
#pragma warning(push)
#pragma warning(disable: 4996)
		if (0 == cx_strcmp("value_uint16", szName))
#pragma warning(pop)
		{
			return m_bitsetLoadedMembers[4];
		}
#pragma warning(push)
#pragma warning(disable: 4996)
		if (0 == cx_strcmp("value_int32", szName))
#pragma warning(pop)
		{
			return m_bitsetLoadedMembers[5];
		}
#pragma warning(push)
#pragma warning(disable: 4996)
		if (0 == cx_strcmp("value_uint32", szName))
#pragma warning(pop)
		{
			return m_bitsetLoadedMembers[6];
		}
#pragma warning(push)
#pragma warning(disable: 4996)
		if (0 == cx_strcmp("value_int64", szName))
#pragma warning(pop)
		{
			return m_bitsetLoadedMembers[7];
		}
#pragma warning(push)
#pragma warning(disable: 4996)
		if (0 == cx_strcmp("value_uint64", szName))
#pragma warning(pop)
		{
			return m_bitsetLoadedMembers[8];
		}
#pragma warning(push)
#pragma warning(disable: 4996)
		if (0 == cx_strcmp("value_float", szName))
#pragma warning(pop)
		{
			return m_bitsetLoadedMembers[9];
		}
#pragma warning(push)
#pragma warning(disable: 4996)
		if (0 == cx_strcmp("value_double", szName))
#pragma warning(pop)
		{
			return m_bitsetLoadedMembers[10];
		}
#pragma warning(push)
#pragma warning(disable: 4996)
		if (0 == cx_strcmp("value_string", szName))
#pragma warning(pop)
		{
			return m_bitsetLoadedMembers[11];
		}
#pragma warning(push)
#pragma warning(disable: 4996)
		if (0 == cx_strcmp("value_blob", szName))
#pragma warning(pop)
		{
			return m_bitsetLoadedMembers[12];
		}

		return CX::False;
	}

	virtual CX::Status SetMemberLoaded(CX::Size cIndex, CX::Bool bLoaded)
	{
		if (m_bitsetLoadedMembers.size() <= cIndex)
		{
			return CX::Status_InvalidArg;
		}
		m_bitsetLoadedMembers[cIndex] = bLoaded;

		return CX::Status();
	}

	virtual CX::Status SetMemberLoaded(const CX::Char *szName, CX::Bool bLoaded)
	{
#pragma warning(push)
#pragma warning(disable: 4996)
		if (0 == cx_strcmp("value_bool", szName))
#pragma warning(pop)
		{
			m_bitsetLoadedMembers[0] = bLoaded;

			return CX::Status();
		}
#pragma warning(push)
#pragma warning(disable: 4996)
		if (0 == cx_strcmp("value_int8", szName))
#pragma warning(pop)
		{
			m_bitsetLoadedMembers[1] = bLoaded;

			return CX::Status();
		}
#pragma warning(push)
#pragma warning(disable: 4996)
		if (0 == cx_strcmp("value_uint8", szName))
#pragma warning(pop)
		{
			m_bitsetLoadedMembers[2] = bLoaded;

			return CX::Status();
		}
#pragma warning(push)
#pragma warning(disable: 4996)
		if (0 == cx_strcmp("value_int16", szName))
#pragma warning(pop)
		{
			m_bitsetLoadedMembers[3] = bLoaded;

			return CX::Status();
		}
#pragma warning(push)
#pragma warning(disable: 4996)
		if (0 == cx_strcmp("value_uint16", szName))
#pragma warning(pop)
		{
			m_bitsetLoadedMembers[4] = bLoaded;

			return CX::Status();
		}
#pragma warning(push)
#pragma warning(disable: 4996)
		if (0 == cx_strcmp("value_int32", szName))
#pragma warning(pop)
		{
			m_bitsetLoadedMembers[5] = bLoaded;

			return CX::Status();
		}
#pragma warning(push)
#pragma warning(disable: 4996)
		if (0 == cx_strcmp("value_uint32", szName))
#pragma warning(pop)
		{
			m_bitsetLoadedMembers[6] = bLoaded;

			return CX::Status();
		}
#pragma warning(push)
#pragma warning(disable: 4996)
		if (0 == cx_strcmp("value_int64", szName))
#pragma warning(pop)
		{
			m_bitsetLoadedMembers[7] = bLoaded;

			return CX::Status();
		}
#pragma warning(push)
#pragma warning(disable: 4996)
		if (0 == cx_strcmp("value_uint64", szName))
#pragma warning(pop)
		{
			m_bitsetLoadedMembers[8] = bLoaded;

			return CX::Status();
		}
#pragma warning(push)
#pragma warning(disable: 4996)
		if (0 == cx_strcmp("value_float", szName))
#pragma warning(pop)
		{
			m_bitsetLoadedMembers[9] = bLoaded;

			return CX::Status();
		}
#pragma warning(push)
#pragma warning(disable: 4996)
		if (0 == cx_strcmp("value_double", szName))
#pragma warning(pop)
		{
			m_bitsetLoadedMembers[10] = bLoaded;

			return CX::Status();
		}
#pragma warning(push)
#pragma warning(disable: 4996)
		if (0 == cx_strcmp("value_string", szName))
#pragma warning(pop)
		{
			m_bitsetLoadedMembers[11] = bLoaded;

			return CX::Status();
		}
#pragma warning(push)
#pragma warning(disable: 4996)
		if (0 == cx_strcmp("value_blob", szName))
#pragma warning(pop)
		{
			m_bitsetLoadedMembers[12] = bLoaded;

			return CX::Status();
		}

		return CX::Status_InvalidArg;
	}

	virtual CX::Status SetAllMembersLoaded(CX::Bool bLoaded)
	{
		if (bLoaded)
		{
			m_bitsetLoadedMembers.set();
		}
		else
		{
			m_bitsetLoadedMembers.reset();
		}

		return CX::Status();
	}

	virtual CX::Status Read(CX::SimpleBuffers::IReader *pReader, const CX::Char *szName = NULL)
	{
		CX::Status status;

		Init();
		m_bitsetLoadedMembers.reset();
		if ((status = pReader->BeginObject(szName)).IsNOK())
		{
			return status;
		}
		if ((status = CX::SimpleBuffers::DataReader<CX::SimpleBuffers::MemberType_Scalar, CX::Bool>::Read(pReader, this->m_value_bool, "value_bool")).IsNOK())
		{
			return status;
		}
		else
		{
			m_bitsetLoadedMembers[0] = true;
		}
		if ((status = CX::SimpleBuffers::DataReader<CX::SimpleBuffers::MemberType_Scalar, CX::Int8>::Read(pReader, this->m_value_int8, "value_int8")).IsNOK())
		{
			return status;
		}
		else
		{
			m_bitsetLoadedMembers[1] = true;
		}
		if ((status = CX::SimpleBuffers::DataReader<CX::SimpleBuffers::MemberType_Scalar, CX::UInt8>::Read(pReader, this->m_value_uint8, "value_uint8")).IsNOK())
		{
			return status;
		}
		else
		{
			m_bitsetLoadedMembers[2] = true;
		}
		if ((status = CX::SimpleBuffers::DataReader<CX::SimpleBuffers::MemberType_Scalar, CX::Int16>::Read(pReader, this->m_value_int16, "value_int16")).IsNOK())
		{
			return status;
		}
		else
		{
			m_bitsetLoadedMembers[3] = true;
		}
		if ((status = CX::SimpleBuffers::DataReader<CX::SimpleBuffers::MemberType_Scalar, CX::UInt16>::Read(pReader, this->m_value_uint16, "value_uint16")).IsNOK())
		{
			return status;
		}
		else
		{
			m_bitsetLoadedMembers[4] = true;
		}
		if ((status = CX::SimpleBuffers::DataReader<CX::SimpleBuffers::MemberType_Scalar, CX::Int32>::Read(pReader, this->m_value_int32, "value_int32")).IsNOK())
		{
			return status;
		}
		else
		{
			m_bitsetLoadedMembers[5] = true;
		}
		if ((status = CX::SimpleBuffers::DataReader<CX::SimpleBuffers::MemberType_Scalar, CX::UInt32>::Read(pReader, this->m_value_uint32, "value_uint32")).IsNOK())
		{
			return status;
		}
		else
		{
			m_bitsetLoadedMembers[6] = true;
		}
		if ((status = CX::SimpleBuffers::DataReader<CX::SimpleBuffers::MemberType_Scalar, CX::Int64>::Read(pReader, this->m_value_int64, "value_int64")).IsNOK())
		{
			return status;
		}
		else
		{
			m_bitsetLoadedMembers[7] = true;
		}
		if ((status = CX::SimpleBuffers::DataReader<CX::SimpleBuffers::MemberType_Scalar, CX::UInt64>::Read(pReader, this->m_value_uint64, "value_uint64")).IsNOK())
		{
			return status;
		}
		else
		{
			m_bitsetLoadedMembers[8] = true;
		}
		if ((status = CX::SimpleBuffers::DataReader<CX::SimpleBuffers::MemberType_Scalar, CX::Float>::Read(pReader, this->m_value_float, "value_float")).IsNOK())
		{
			return status;
		}
		else
		{
			m_bitsetLoadedMembers[9] = true;
		}
		if ((status = CX::SimpleBuffers::DataReader<CX::SimpleBuffers::MemberType_Scalar, CX::Double>::Read(pReader, this->m_value_double, "value_double")).IsNOK())
		{
			return status;
		}
		else
		{
			m_bitsetLoadedMembers[10] = true;
		}
		if ((status = CX::SimpleBuffers::DataReader<CX::SimpleBuffers::MemberType_Scalar, CX::String>::Read(pReader, this->m_value_string, "value_string")).IsNOK())
		{
			return status;
		}
		else
		{
			m_bitsetLoadedMembers[11] = true;
		}
		if ((status = CX::SimpleBuffers::DataReader<CX::SimpleBuffers::MemberType_Scalar, CX::BLOB>::Read(pReader, this->m_value_blob, "value_blob")).IsNOK())
		{
			return status;
		}
		else
		{
			m_bitsetLoadedMembers[12] = true;
		}
		if ((status = pReader->EndObject()).IsNOK())
		{
			return status;
		}

		return CX::Status();
	}

	virtual CX::Status Write(CX::SimpleBuffers::IWriter *pWriter, const CX::Char *szName = NULL) const
	{
		CX::Status status;

		if ((status = pWriter->BeginObject(szName)).IsNOK())
		{
			return status;
		}
		if ((status = CX::SimpleBuffers::DataWriter<CX::SimpleBuffers::MemberType_Scalar, CX::Bool>::Write(pWriter, this->m_value_bool, "value_bool")).IsNOK())
		{
			return status;
		}
		if ((status = CX::SimpleBuffers::DataWriter<CX::SimpleBuffers::MemberType_Scalar, CX::Int8>::Write(pWriter, this->m_value_int8, "value_int8")).IsNOK())
		{
			return status;
		}
		if ((status = CX::SimpleBuffers::DataWriter<CX::SimpleBuffers::MemberType_Scalar, CX::UInt8>::Write(pWriter, this->m_value_uint8, "value_uint8")).IsNOK())
		{
			return status;
		}
		if ((status = CX::SimpleBuffers::DataWriter<CX::SimpleBuffers::MemberType_Scalar, CX::Int16>::Write(pWriter, this->m_value_int16, "value_int16")).IsNOK())
		{
			return status;
		}
		if ((status = CX::SimpleBuffers::DataWriter<CX::SimpleBuffers::MemberType_Scalar, CX::UInt16>::Write(pWriter, this->m_value_uint16, "value_uint16")).IsNOK())
		{
			return status;
		}
		if ((status = CX::SimpleBuffers::DataWriter<CX::SimpleBuffers::MemberType_Scalar, CX::Int32>::Write(pWriter, this->m_value_int32, "value_int32")).IsNOK())
		{
			return status;
		}
		if ((status = CX::SimpleBuffers::DataWriter<CX::SimpleBuffers::MemberType_Scalar, CX::UInt32>::Write(pWriter, this->m_value_uint32, "value_uint32")).IsNOK())
		{
			return status;
		}
		if ((status = CX::SimpleBuffers::DataWriter<CX::SimpleBuffers::MemberType_Scalar, CX::Int64>::Write(pWriter, this->m_value_int64, "value_int64")).IsNOK())
		{
			return status;
		}
		if ((status = CX::SimpleBuffers::DataWriter<CX::SimpleBuffers::MemberType_Scalar, CX::UInt64>::Write(pWriter, this->m_value_uint64, "value_uint64")).IsNOK())
		{
			return status;
		}
		if ((status = CX::SimpleBuffers::DataWriter<CX::SimpleBuffers::MemberType_Scalar, CX::Float>::Write(pWriter, this->m_value_float, "value_float")).IsNOK())
		{
			return status;
		}
		if ((status = CX::SimpleBuffers::DataWriter<CX::SimpleBuffers::MemberType_Scalar, CX::Double>::Write(pWriter, this->m_value_double, "value_double")).IsNOK())
		{
			return status;
		}
		if ((status = CX::SimpleBuffers::DataWriter<CX::SimpleBuffers::MemberType_Scalar, CX::String>::Write(pWriter, this->m_value_string, "value_string")).IsNOK())
		{
			return status;
		}
		if ((status = CX::SimpleBuffers::DataWriter<CX::SimpleBuffers::MemberType_Scalar, CX::BLOB>::Write(pWriter, this->m_value_blob, "value_blob")).IsNOK())
		{
			return status;
		}
		if ((status = pWriter->EndObject()).IsNOK())
		{
			return status;
		}

		return CX::Status();
	}

	bool operator<(const TestSimpleBuffers1 &p) const
	{
		if (this->m_value_bool < p.m_value_bool)
		{
			return true;
		}
		else
		if (this->m_value_bool > p.m_value_bool)
		{
			return false;
		}
		if (this->m_value_int8 < p.m_value_int8)
		{
			return true;
		}
		else
		if (this->m_value_int8 > p.m_value_int8)
		{
			return false;
		}
		if (this->m_value_uint8 < p.m_value_uint8)
		{
			return true;
		}
		else
		if (this->m_value_uint8 > p.m_value_uint8)
		{
			return false;
		}
		if (this->m_value_int16 < p.m_value_int16)
		{
			return true;
		}
		else
		if (this->m_value_int16 > p.m_value_int16)
		{
			return false;
		}
		if (this->m_value_uint16 < p.m_value_uint16)
		{
			return true;
		}
		else
		if (this->m_value_uint16 > p.m_value_uint16)
		{
			return false;
		}
		if (this->m_value_int32 < p.m_value_int32)
		{
			return true;
		}
		else
		if (this->m_value_int32 > p.m_value_int32)
		{
			return false;
		}
		if (this->m_value_uint32 < p.m_value_uint32)
		{
			return true;
		}
		else
		if (this->m_value_uint32 > p.m_value_uint32)
		{
			return false;
		}
		if (this->m_value_int64 < p.m_value_int64)
		{
			return true;
		}
		else
		if (this->m_value_int64 > p.m_value_int64)
		{
			return false;
		}
		if (this->m_value_uint64 < p.m_value_uint64)
		{
			return true;
		}
		else
		if (this->m_value_uint64 > p.m_value_uint64)
		{
			return false;
		}
		if (this->m_value_float < p.m_value_float)
		{
			return true;
		}
		else
		if (this->m_value_float > p.m_value_float)
		{
			return false;
		}
		if (this->m_value_double < p.m_value_double)
		{
			return true;
		}
		else
		if (this->m_value_double > p.m_value_double)
		{
			return false;
		}
		if (this->m_value_string < p.m_value_string)
		{
			return true;
		}
		else
		if (this->m_value_string > p.m_value_string)
		{
			return false;
		}
		if (this->m_value_blob < p.m_value_blob)
		{
			return true;
		}
		else
		if (this->m_value_blob > p.m_value_blob)
		{
			return false;
		}

		return false;
	}

	bool operator>(const TestSimpleBuffers1 &p) const
	{
		if (this->m_value_bool > p.m_value_bool)
		{
			return true;
		}
		else
		if (this->m_value_bool < p.m_value_bool)
		{
			return false;
		}
		if (this->m_value_int8 > p.m_value_int8)
		{
			return true;
		}
		else
		if (this->m_value_int8 < p.m_value_int8)
		{
			return false;
		}
		if (this->m_value_uint8 > p.m_value_uint8)
		{
			return true;
		}
		else
		if (this->m_value_uint8 < p.m_value_uint8)
		{
			return false;
		}
		if (this->m_value_int16 > p.m_value_int16)
		{
			return true;
		}
		else
		if (this->m_value_int16 < p.m_value_int16)
		{
			return false;
		}
		if (this->m_value_uint16 > p.m_value_uint16)
		{
			return true;
		}
		else
		if (this->m_value_uint16 < p.m_value_uint16)
		{
			return false;
		}
		if (this->m_value_int32 > p.m_value_int32)
		{
			return true;
		}
		else
		if (this->m_value_int32 < p.m_value_int32)
		{
			return false;
		}
		if (this->m_value_uint32 > p.m_value_uint32)
		{
			return true;
		}
		else
		if (this->m_value_uint32 < p.m_value_uint32)
		{
			return false;
		}
		if (this->m_value_int64 > p.m_value_int64)
		{
			return true;
		}
		else
		if (this->m_value_int64 < p.m_value_int64)
		{
			return false;
		}
		if (this->m_value_uint64 > p.m_value_uint64)
		{
			return true;
		}
		else
		if (this->m_value_uint64 < p.m_value_uint64)
		{
			return false;
		}
		if (this->m_value_float > p.m_value_float)
		{
			return true;
		}
		else
		if (this->m_value_float < p.m_value_float)
		{
			return false;
		}
		if (this->m_value_double > p.m_value_double)
		{
			return true;
		}
		else
		if (this->m_value_double < p.m_value_double)
		{
			return false;
		}
		if (this->m_value_string > p.m_value_string)
		{
			return true;
		}
		else
		if (this->m_value_string < p.m_value_string)
		{
			return false;
		}
		if (this->m_value_blob > p.m_value_blob)
		{
			return true;
		}
		else
		if (this->m_value_blob < p.m_value_blob)
		{
			return false;
		}

		return false;
	}

private:

	CX::Bool             m_value_bool;
	CX::Int8             m_value_int8;
	CX::UInt8            m_value_uint8;
	CX::Int16            m_value_int16;
	CX::UInt16           m_value_uint16;
	CX::Int32            m_value_int32;
	CX::UInt32           m_value_uint32;
	CX::Int64            m_value_int64;
	CX::UInt64           m_value_uint64;
	CX::Float            m_value_float;
	CX::Double           m_value_double;
	CX::String           m_value_string;
	CX::BLOB             m_value_blob;
	CX::BitSet<13>::Type m_bitsetLoadedMembers;

};


namespace CX
{

namespace SimpleBuffers
{

template <>
struct DataIniter<MemberType_Scalar, TestSimpleBuffers1>
{
	static void Init(TestSimpleBuffers1 &v)
	{
		v.Init();
	}
};

template <>
struct DataWriter<MemberType_Scalar, TestSimpleBuffers1>
{
	static Status Write(IWriter *pWriter, const TestSimpleBuffers1 &v, const CX::Char *szName = NULL)
	{
		return v.Write(pWriter, szName);
	}
};

template <>
struct DataReader<MemberType_Scalar, TestSimpleBuffers1>
{
	static Status Read(IReader *pReader, TestSimpleBuffers1 &v, const CX::Char *szName = NULL)
	{
		return v.Read(pReader, szName);
	}
};

}//namespace SimpleBuffers

}//namespace CX


