
#pragma once


#include "CX/SimpleBuffers/IWriter.hpp"
#include "CX/SimpleBuffers/MemberType.hpp"
#include "CX/Vector.hpp"
#include "CX/Set.hpp"
#include "CX/Map.hpp"


namespace CX
{

namespace SimpleBuffers
{

template <MemberType M, typename K, typename V = void>
struct DataWriter;

template <>
struct DataWriter<MemberType_Scalar, Bool>
{
	static Status Write(IWriter *pWriter, Bool v, const CX::Char *szName = NULL)
	{
		return pWriter->WriteBool(v, szName);
	}
};

template <>
struct DataWriter<MemberType_Scalar, Int8>
{
	static Status Write(IWriter *pWriter, Int8 v, const Char *szName = NULL)
	{
		return pWriter->WriteInt8(v, szName);
	}
};

template <>
struct DataWriter<MemberType_Scalar, UInt8>
{
	static Status Write(IWriter *pWriter, UInt8 v, const Char *szName = NULL)
	{
		return pWriter->WriteUInt8(v, szName);
	}
};

template <>
struct DataWriter<MemberType_Scalar, Int16>
{
	static Status Write(IWriter *pWriter, Int16 v, const Char *szName = NULL)
	{
		return pWriter->WriteInt16(v, szName);
	}
};

template <>
struct DataWriter<MemberType_Scalar, UInt16>
{
	static Status Write(IWriter *pWriter, UInt16 v, const Char *szName = NULL)
	{
		return pWriter->WriteUInt16(v, szName);
	}
};

template <>
struct DataWriter<MemberType_Scalar, Int32>
{
	static Status Write(IWriter *pWriter, Int32 v, const Char *szName = NULL)
	{
		return pWriter->WriteInt32(v, szName);
	}
};

template <>
struct DataWriter<MemberType_Scalar, UInt32>
{
	static Status Write(IWriter *pWriter, UInt32 v, const Char *szName = NULL)
	{
		return pWriter->WriteUInt32(v, szName);
	}
};

template <>
struct DataWriter<MemberType_Scalar, Int64>
{
	static Status Write(IWriter *pWriter, Int64 v, const Char *szName = NULL)
	{
		return pWriter->WriteInt64(v, szName);
	}
};

template <>
struct DataWriter<MemberType_Scalar, UInt64>
{
	static Status Write(IWriter *pWriter, UInt64 v, const Char *szName = NULL)
	{
		return pWriter->WriteUInt64(v, szName);
	}
};

template <>
struct DataWriter<MemberType_Scalar, Float>
{
	static Status Write(IWriter *pWriter, Float v, const Char *szName = NULL)
	{
		return pWriter->WriteFloat(v, szName);
	}
};

template <>
struct DataWriter<MemberType_Scalar, Double>
{
	static Status Write(IWriter *pWriter, Double v, const Char *szName = NULL)
	{
		return pWriter->WriteDouble(v, szName);
	}
};

template <>
struct DataWriter<MemberType_Scalar, String>
{
	static Status Write(IWriter *pWriter, const String &v, const Char *szName = NULL)
	{
		return pWriter->WriteString(v, szName);
	}
};

template <>
struct DataWriter<MemberType_Scalar, BLOB>
{
	static Status Write(IWriter *pWriter, const BLOB &v, const Char *szName = NULL)
	{
		return pWriter->WriteBLOB(v, szName);
	}
};

template <typename K>
struct DataWriter<MemberType_Vector, K>
{
	static Status Write(IWriter *pWriter, const typename Vector<K>::Type &v, const Char *szName = NULL)
	{
		Status status;

		if ((status = pWriter->BeginArray(szName)).IsNOK())
		{
			return status;
		}
		for (typename Vector<K>::Type::const_iterator iter = v.begin(); iter != v.end(); ++iter)
		{
			if ((status = DataWriter<MemberType_Scalar, K>::Write(pWriter, *iter)).IsNOK())
			{
				return status;
			}
		}
		if ((status = pWriter->EndArray()).IsNOK())
		{
			return status;
		}

		return Status();
	}
};

template <typename K>
struct DataWriter<MemberType_Set, K>
{
	static Status Write(IWriter *pWriter, const typename Set<K>::Type &v, const Char *szName = NULL)
	{
		Status status;

		if ((status = pWriter->BeginArray(szName)).IsNOK())
		{
			return status;
		}
		for (typename Set<K>::Type::const_iterator iter = v.begin(); iter != v.end(); ++iter)
		{
			if ((status = DataWriter<MemberType_Scalar, K>::Write(pWriter, *iter)).IsNOK())
			{
				return status;
			}
		}
		if ((status = pWriter->EndArray()).IsNOK())
		{
			return status;
		}

		return Status();
	}
};

template <typename K, typename V>
struct DataWriter<MemberType_Map, K, V>
{
	static Status Write(IWriter *pWriter, const typename Map<K, V>::Type &v, const Char *szName = NULL)
	{
		Status status;

		if ((status = pWriter->BeginArray(szName)).IsNOK())
		{
			return status;
		}
		for (typename Map<K, V>::Type::const_iterator iter = v.begin(); iter != v.end(); ++iter)
		{
			if ((status = pWriter->BeginObject()).IsNOK())
			{
				return status;
			}
			if ((status = DataWriter<MemberType_Scalar, K>::Write(pWriter, iter->first, "key")).IsNOK())
			{
				return status;
			}
			if ((status = DataWriter<MemberType_Scalar, V>::Write(pWriter, iter->second, "val")).IsNOK())
			{
				return status;
			}
			if ((status = pWriter->EndObject()).IsNOK())
			{
				return status;
			}
		}
		if ((status = pWriter->EndArray()).IsNOK())
		{
			return status;
		}

		return Status();
	}
};

}//namespace SimpleBuffers

}//namespace CX
