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
#include "CX/SB/IDataWriter.hpp"
#include "CX/SB/Comparators.hpp"
#include "CX/SB/Hashers.hpp"


namespace CX
{

namespace SB
{

template <typename T> inline Status Write(const T p, IDataWriter *pDataWriter);

template <> inline Status Write<Bool>(Bool p, IDataWriter *pDataWriter)
{
	return pDataWriter->WriteBool(p);
}

template <> inline Status Write<BoolForVector>(BoolForVector p, IDataWriter *pDataWriter)
{
	return pDataWriter->WriteBool(p);
}

template <> inline Status Write<Int8>(Int8 p, IDataWriter *pDataWriter)
{
	return pDataWriter->WriteInt8(p);
}

template <> inline Status Write<UInt8>(UInt8 p, IDataWriter *pDataWriter)
{
	return pDataWriter->WriteUInt8(p);
}

template <> inline Status Write<Int16>(Int16 p, IDataWriter *pDataWriter)
{
	return pDataWriter->WriteInt16(p);
}

template <> inline Status Write<UInt16>(UInt16 p, IDataWriter *pDataWriter)
{
	return pDataWriter->WriteUInt16(p);
}

template <> inline Status Write<Int32>(Int32 p, IDataWriter *pDataWriter)
{
	return pDataWriter->WriteInt32(p);
}

template <> inline Status Write<UInt32>(UInt32 p, IDataWriter *pDataWriter)
{
	return pDataWriter->WriteUInt32(p);
}

template <> inline Status Write<Int64>(Int64 p, IDataWriter *pDataWriter)
{
	return pDataWriter->WriteInt64(p);
}

template <> inline Status Write<UInt64>(UInt64 p, IDataWriter *pDataWriter)
{
	return pDataWriter->WriteUInt64(p);
}

template <> inline Status Write<Float>(Float p, IDataWriter *pDataWriter)
{
	return pDataWriter->WriteFloat(p);
}

template <> inline Status Write<Double>(Double p, IDataWriter *pDataWriter)
{
	return pDataWriter->WriteDouble(p);
}

template <> inline Status Write<String>(String p, IDataWriter *pDataWriter)
{
	return pDataWriter->WriteString(p.c_str());
}

template <> inline Status Write<WString>(WString p, IDataWriter *pDataWriter)
{
	return pDataWriter->WriteWString(p.c_str());
}

template <> inline Status Write<const Char *>(const Char *p, IDataWriter *pDataWriter)
{
	return pDataWriter->WriteString(p);
}

template <> inline Status Write<const WChar *>(const WChar *p, IDataWriter *pDataWriter)
{
	return pDataWriter->WriteWString(p);
}

template <typename T> inline Status Write(std::vector<T, STLAlloc<T> > p, IDataWriter *pDataWriter)
{
	Status status;

	if ((status = pDataWriter->BeginArray(p.size())).IsNOK())
	{
		return status;
	}
	for (typename std::vector<T, STLAlloc<T> >::iterator iter = p.begin(); iter != p.end(); ++iter)
	{
		if ((status = pDataWriter->WriteItem(*iter)).IsNOK())
		{
			return status;
		}
	}
	if ((status = pDataWriter->EndArray()).IsNOK())
	{
		return status;
	}

	return Status();
}

template <typename K> 
inline Status Write(std::set<K, Less<K>, STLAlloc<K> > p, IDataWriter *pDataWriter)
{
	Status status;

	if ((status = pDataWriter->BeginArray(p.size())).IsNOK())
	{
		return status;
	}
	for (typename std::set<K, Less<K>, STLAlloc<K> >::iterator iter = p.begin(); iter != p.end(); ++iter)
	{
		if ((status = pDataWriter->WriteItem(*iter)).IsNOK())
		{
			return status;
		}
	}
	if ((status = pDataWriter->EndArray()).IsNOK())
	{
		return status;
	}

	return Status();
}

template <typename K, typename V> 
inline Status Write(std::map<K, V, Less<K>, STLAlloc<std::pair<const K, V> > > p, IDataWriter *pDataWriter)
{
	Status status;

	if ((status = pDataWriter->BeginArray(p.size())).IsNOK())
	{
		return status;
	}
	for (typename std::map<K, V, Less<K>, STLAlloc<std::pair<const K, V> > >::iterator iter = p.begin(); iter != p.end(); ++iter)
	{
		if ((status = pDataWriter->BeginItem()).IsNOK())
		{
			return status;
		}
		if ((status = pDataWriter->BeginArray(2)).IsNOK())
		{
			return status;
		}
		if ((status = pDataWriter->WriteItem(iter->first)).IsNOK())
		{
			return status;
		}
		if ((status = pDataWriter->WriteItem(iter->second)).IsNOK())
		{
			return status;
		}
		if ((status = pDataWriter->EndArray()).IsNOK())
		{
			return status;
		}
		if ((status = pDataWriter->EndItem()).IsNOK())
		{
			return status;
		}
	}
	if ((status = pDataWriter->EndArray()).IsNOK())
	{
		return status;
	}

	return Status();
}

template <typename K> 
inline Status Write(google::sparse_hash_set<K, Hasher<K>, Comparator<K>, SparseHashAllocator<K> > p, IDataWriter *pDataWriter)
{
	Status status;

	if ((status = pDataWriter->BeginArray(p.size())).IsNOK())
	{
		return status;
	}
	for (typename google::sparse_hash_set<K, Hasher<K>, Comparator<K>, SparseHashAllocator<K> >::iterator iter = p.begin(); 
	     iter != p.end(); ++iter)
	{
		if ((status = pDataWriter->WriteItem(*iter)).IsNOK())
		{
			return status;
		}
	}
	if ((status = pDataWriter->EndArray()).IsNOK())
	{
		return status;
	}

	return Status();
}

template <typename K, typename V> 
inline Status Write(google::sparse_hash_map<K, V, Hasher<K>, Comparator<K>, SparseHashAllocator<std::pair<const K, V> > > p, 
                    IDataWriter *pDataWriter)
{
	Status status;

	if ((status = pDataWriter->BeginArray(p.size())).IsNOK())
	{
		return status;
	}
	for (typename google::sparse_hash_map<K, V, Hasher<K>, Comparator<K>, SparseHashAllocator<std::pair<const K, V> > >::iterator 
	     iter = p.begin(); iter != p.end(); ++iter)
	{
		if ((status = pDataWriter->BeginItem()).IsNOK())
		{
			return status;
		}
		if ((status = pDataWriter->BeginArray(2)).IsNOK())
		{
			return status;
		}
		if ((status = pDataWriter->WriteItem(iter->first)).IsNOK())
		{
			return status;
		}
		if ((status = pDataWriter->WriteItem(iter->second)).IsNOK())
		{
			return status;
		}
		if ((status = pDataWriter->EndArray()).IsNOK())
		{
			return status;
		}
		if ((status = pDataWriter->EndItem()).IsNOK())
		{
			return status;
		}
	}
	if ((status = pDataWriter->EndArray()).IsNOK())
	{
		return status;
	}

	return Status();
}

}//namespace SB

}//namespace CX

