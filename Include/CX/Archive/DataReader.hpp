
#pragma once


#include "CX/Archive/IReader.hpp"
#include "CX/Archive/MemberType.hpp"
#include "CX/Vector.hpp"
#include "CX/Set.hpp"
#include "CX/Map.hpp"


namespace CX
{

namespace Archive
{

template <MemberType M, typename K, typename V = void>
struct DataReader;

template <>
struct DataReader<MemberType_Scalar, Bool>
{
	static Status Read(IReader *pReader, Bool &v, const CX::Char *szName = NULL)
	{
		return pReader->ReadBool(v, szName);
	}
};

template <>
struct DataReader<MemberType_Scalar, Int8>
{
	static Status Read(IReader *pReader, Int8 &v, const Char *szName = NULL)
	{
		return pReader->ReadInt8(v, szName);
	}
};

template <>
struct DataReader<MemberType_Scalar, UInt8>
{
	static Status Read(IReader *pReader, UInt8 &v, const Char *szName = NULL)
	{
		return pReader->ReadUInt8(v, szName);
	}
};

template <>
struct DataReader<MemberType_Scalar, Int16>
{
	static Status Read(IReader *pReader, Int16 &v, const Char *szName = NULL)
	{
		return pReader->ReadInt16(v, szName);
	}
};

template <>
struct DataReader<MemberType_Scalar, UInt16>
{
	static Status Read(IReader *pReader, UInt16 &v, const Char *szName = NULL)
	{
		return pReader->ReadUInt16(v, szName);
	}
};

template <>
struct DataReader<MemberType_Scalar, Int32>
{
	static Status Read(IReader *pReader, Int32 &v, const Char *szName = NULL)
	{
		return pReader->ReadInt32(v, szName);
	}
};

template <>
struct DataReader<MemberType_Scalar, UInt32>
{
	static Status Read(IReader *pReader, UInt32 &v, const Char *szName = NULL)
	{
		return pReader->ReadUInt32(v, szName);
	}
};

template <>
struct DataReader<MemberType_Scalar, Int64>
{
	static Status Read(IReader *pReader, Int64 &v, const Char *szName = NULL)
	{
		return pReader->ReadInt64(v, szName);
	}
};

template <>
struct DataReader<MemberType_Scalar, UInt64>
{
	static Status Read(IReader *pReader, UInt64 &v, const Char *szName = NULL)
	{
		return pReader->ReadUInt64(v, szName);
	}
};

template <>
struct DataReader<MemberType_Scalar, Float>
{
	static Status Read(IReader *pReader, Float &v, const Char *szName = NULL)
	{
		return pReader->ReadFloat(v, szName);
	}
};

template <>
struct DataReader<MemberType_Scalar, Double>
{
	static Status Read(IReader *pReader, Double &v, const Char *szName = NULL)
	{
		return pReader->ReadDouble(v, szName);
	}
};

template <>
struct DataReader<MemberType_Scalar, String>
{
	static Status Read(IReader *pReader, String &v, const Char *szName = NULL)
	{
		return pReader->ReadString(v, szName);
	}
};

template <>
struct DataReader<MemberType_Scalar, BLOB>
{
	static Status Read(IReader *pReader, BLOB &v, const Char *szName = NULL)
	{
		return pReader->ReadBLOB(v, szName);
	}
};

template <typename K>
struct DataReader<MemberType_Vector, K>
{
	static Status Read(IReader *pReader, typename Vector<K>::Type &v, const Char *szName = NULL)
	{
		Status status;

		if ((status = pReader->BeginArray(szName)).IsNOK())
		{
			return status;
		}
		for (;;)
		{
			K k;

			if ((status = DataReader<MemberType_Scalar, K>::Read(pReader, k)).IsNOK())
			{
				break;
			}
			v.push_back(k);
		}
		if ((status = pReader->EndArray()).IsNOK())
		{
			return status;
		}

		return Status();
	}
};

template <typename K>
struct DataReader<MemberType_Set, K>
{
	static Status Read(IReader *pReader, typename Set<K>::Type &v, const Char *szName = NULL)
	{
		Status status;

		if ((status = pReader->BeginArray(szName)).IsNOK())
		{
			return status;
		}
		for (;;)
		{
			K k;

			if ((status = DataReader<MemberType_Scalar, K>::Read(pReader, k)).IsNOK())
			{
				break;
			}
			v.insert(k);
		}
		if ((status = pReader->EndArray()).IsNOK())
		{
			return status;
		}

		return Status();
	}
};

template <typename K, typename V>
struct DataReader<MemberType_Map, K, V>
{
	static Status Read(IReader *pReader, typename Map<K, V>::Type &v, const Char *szName = NULL)
	{
		Status status;

		if ((status = pReader->BeginArray(szName)).IsNOK())
		{
			return status;
		}
		for (;;)
		{
			K key;
			V val;

			if ((status = pReader->BeginObject()).IsNOK())
			{
				break;
			}
			if ((status = DataReader<MemberType_Scalar, K>::Read(pReader, key, "key")).IsNOK())
			{
				break;
			}
			if ((status = DataReader<MemberType_Scalar, V>::Read(pReader, val, "val")).IsNOK())
			{
				break;
			}
			if ((status = pReader->EndObject()).IsNOK())
			{
				break;
			}
			v[key] = val;
		}
		if ((status = pReader->EndArray()).IsNOK())
		{
			return status;
		}

		return Status();
	}
};

}//namespace Archive

}//namespace CX
