/* 
 * CX - C++ framework for general purpose development
 *
 * https://github.com/draede/cx
 * 
 * Copyright (C) 2014 - 2019 draede - draede [at] outlook [dot] com
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


#include "CX/SimpleBuffers/IReader.hpp"
#include "CX/SimpleBuffers/MemberType.hpp"
#include "CX/Vector.hpp"
#include "CX/Set.hpp"
#include "CX/Map.hpp"


namespace CX
{

namespace SimpleBuffers
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
			typename Vector<K>::Type::iterator iter = v.insert(v.end(), K());

			if ((status = DataReader<MemberType_Scalar, K>::Read(pReader, *iter)).IsNOK())
			{
				v.erase(iter);
				if (Status_NoMoreItems != status.GetCode())
				{
					return status;
				}
				break;
			}
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
			typename Set<K>::Type::iterator iter = v.insert(v.end(), K());

			if ((status = DataReader<MemberType_Scalar, K>::Read(pReader, *iter)).IsNOK())
			{
				v.erase(iter);

				break;
			}
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

			if ((status = pReader->BeginObject()).IsNOK())
			{
				break;
			}
			if ((status = DataReader<MemberType_Scalar, K>::Read(pReader, key, "key")).IsNOK())
			{
				break;
			}

			typename Map<K, V>::Type::iterator iter = v.insert(std::pair<K, V>(key, V()));

			if ((status = DataReader<MemberType_Scalar, V>::Read(pReader, iter->second, "val")).IsNOK())
			{
				v.erase(iter);

				break;
			}
			if ((status = pReader->EndObject()).IsNOK())
			{
				v.erase(iter);

				break;
			}
		}
		if ((status = pReader->EndArray()).IsNOK())
		{
			return status;
		}

		return Status();
	}
};

}//namespace SimpleBuffers

}//namespace CX
