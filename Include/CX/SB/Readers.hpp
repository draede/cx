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

#pragma once


#include "CX/Types.hpp"
#include "CX/Status.hpp"
#include "CX/SB/IDataReader.hpp"
#include "CX/SB/Comparators.hpp"
#include "CX/SB/Hashers.hpp"


namespace CX
{

namespace SB
{

template <typename T> inline Status Read(T &p, IDataReader *pDataReader);

template <> inline Status Read<Bool>(Bool &p, IDataReader *pDataReader)
{
	return pDataReader->ReadBool(&p);
}

template <> inline Status Read<BoolForVector>(BoolForVector &p, IDataReader *pDataReader)
{
	return pDataReader->ReadBool(&p);
}

template <> inline Status Read<Int8>(Int8 &p, IDataReader *pDataReader)
{
	return pDataReader->ReadInt8(&p);
}

template <> inline Status Read<UInt8>(UInt8 &p, IDataReader *pDataReader)
{
	return pDataReader->ReadUInt8(&p);
}

template <> inline Status Read<Int16>(Int16 &p, IDataReader *pDataReader)
{
	return pDataReader->ReadInt16(&p);
}

template <> inline Status Read<UInt16>(UInt16 &p, IDataReader *pDataReader)
{
	return pDataReader->ReadUInt16(&p);
}

template <> inline Status Read<Int32>(Int32 &p, IDataReader *pDataReader)
{
	return pDataReader->ReadInt32(&p);
}

template <> inline Status Read<UInt32>(UInt32 &p, IDataReader *pDataReader)
{
	return pDataReader->ReadUInt32(&p);
}

template <> inline Status Read<Int64>(Int64 &p, IDataReader *pDataReader)
{
	return pDataReader->ReadInt64(&p);
}

template <> inline Status Read<UInt64>(UInt64 &p, IDataReader *pDataReader)
{
	return pDataReader->ReadUInt64(&p);
}

template <> inline Status Read<Float>(Float &p, IDataReader *pDataReader)
{
	return pDataReader->ReadFloat(&p);
}

template <> inline Status Read<Double>(Double &p, IDataReader *pDataReader)
{
	return pDataReader->ReadDouble(&p);
}

template <> inline Status Read<String>(String &p, IDataReader *pDataReader)
{
	return pDataReader->ReadString(&p);
}

template <> inline Status Read<WString>(WString &p, IDataReader *pDataReader)
{
	return pDataReader->ReadWString(&p);
}

template <typename T> inline Status Read(std::vector<T, STLAlloc<T> > &p, IDataReader *pDataReader)
{
	Size   cCount;
	Status status;

	if ((status = pDataReader->BeginArray(&cCount)).IsNOK())
	{
		return status;
	}
	for (Size i = 0; i < cCount; i++)
	{
		T k;

		if ((status = pDataReader->ReadItem(k)).IsNOK())
		{
			return status;
		}
		p.push_back(k);
	}
	if ((status = pDataReader->EndArray()).IsNOK())
	{
		return status;
	}

	return Status();
}


template <typename K> 
inline Status Read(std::set<K, Less<K>, STLAlloc<K> > &p, IDataReader *pDataReader)
{
	Size   cCount;
	Status status;

	if ((status = pDataReader->BeginArray(&cCount)).IsNOK())
	{
		return status;
	}
	for (Size i = 0; i < cCount; i++)
	{
		K k;

		if ((status = pDataReader->ReadItem(k)).IsNOK())
		{
			return status;
		}
		p.insert(k);
	}
	if ((status = pDataReader->EndArray()).IsNOK())
	{
		return status;
	}

	return Status();
}

template <typename K, typename V> 
inline Status Read(std::map<K, V, Less<K>, STLAlloc<std::pair<const K, V> > > &p, IDataReader *pDataReader)
{
	Size   cCount;
	Size   cCount2;
	Status status;

	if ((status = pDataReader->BeginArray(&cCount)).IsNOK())
	{
		return status;
	}
	for (Size i = 0; i < cCount; i++)
	{
		K k;
		V v;

		if ((status = pDataReader->BeginItem()).IsNOK())
		{
			return status;
		}
		if ((status = pDataReader->BeginArray(&cCount2)).IsNOK())
		{
			return status;
		}
		if (2 != cCount2)
		{
			return Status(Status_InvalidArg, "Maps have 2 values per entry (key & value)");
		}
		if ((status = pDataReader->ReadItem(k)).IsNOK())
		{
			return status;
		}
		if ((status = pDataReader->ReadItem(v)).IsNOK())
		{
			return status;
		}
		if ((status = pDataReader->EndArray()).IsNOK())
		{
			return status;
		}
		if ((status = pDataReader->EndItem()).IsNOK())
		{
			return status;
		}
		p[k] = v;
	}
	if ((status = pDataReader->EndArray()).IsNOK())
	{
		return status;
	}

	return Status();
}

template <typename K> 
inline Status Read(google::sparse_hash_set<K, Hasher<K>, Comparator<K>, SparseHashAllocator<K> > &p, IDataReader *pDataReader)
{
	Size   cCount;
	Status status;

	if ((status = pDataReader->BeginArray(&cCount)).IsNOK())
	{
		return status;
	}
	for (Size i = 0; i < cCount; i++)
	{
		K k;

		if ((status = pDataReader->ReadItem(k)).IsNOK())
		{
			return status;
		}
		p.insert(k);
	}
	if ((status = pDataReader->EndArray()).IsNOK())
	{
		return status;
	}

	return Status();
}

template <typename K, typename V> 
inline Status Read(google::sparse_hash_map<K, V, Hasher<K>, Comparator<K>, SparseHashAllocator<std::pair<const K, V> > > &p, 
                   IDataReader *pDataReader)
{
	Size   cCount;
	Size   cCount2;
	Status status;

	if ((status = pDataReader->BeginArray(&cCount)).IsNOK())
	{
		return status;
	}
	for (Size i = 0; i < cCount; i++)
	{
		K k;
		V v;

		if ((status = pDataReader->BeginItem()).IsNOK())
		{
			return status;
		}
		if ((status = pDataReader->BeginArray(&cCount2)).IsNOK())
		{
			return status;
		}
		if (2 != cCount2)
		{
			return Status(Status_InvalidArg, "Maps have 2 values per entry (key & value)");
		}
		if ((status = pDataReader->ReadItem(k)).IsNOK())
		{
			return status;
		}
		if ((status = pDataReader->ReadItem(v)).IsNOK())
		{
			return status;
		}
		if ((status = pDataReader->EndArray()).IsNOK())
		{
			return status;
		}
		if ((status = pDataReader->EndItem()).IsNOK())
		{
			return status;
		}
		p[k] = v;
	}
	if ((status = pDataReader->EndArray()).IsNOK())
	{
		return status;
	}

	return Status();
}

}//namespace SB

}//namespace CX

