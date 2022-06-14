/* 
 * CX - C++ framework for general purpose development
 *
 * https://github.com/draede/cx
 * 
 * Copyright (C) 2014 - 2022 draede
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


#include "CX/Hash/IHash.hpp"
#include "CX/Types.hpp"
#include "CX/Map.hpp"
#include <new>


namespace CX
{

namespace Hash
{

class MultiHash
{
public:

	MultiHash();

	~MultiHash();

	template <typename HASH>
	Status AddHash()
	{
		HASH                *pHash;
		Hash                hash;
		HashesMap::iterator iter;

		if (NULL == (pHash = new (std::nothrow) HASH()))
		{
			return Status_MemAllocFailed;
		}
		if (NULL == (hash.pHash = dynamic_cast<IHash *>(pHash)))
		{
			delete pHash;

			return Status_InvalidArg;
		}
		if (m_mapHashes.end() != (iter = m_mapHashes.find(hash.pHash->GetName())))
		{
			delete pHash;

			return Status_Duplicate;
		}
		hash.pData  = NULL;
		hash.cbSize = 0;
		m_mapHashes[hash.pHash->GetName()] = hash;

		return Status();
	}

	Size GetHashesCount() const;

	const Char *GetHashName(Size cIndex) const;

	const void *GetHashResultData(const Char *szName) const;

	Size GetHashResultSize(const Char *szName) const;

	Status Init();

	Status Update(const void *pBuffer, Size cbSize);

	Status Done();

private:

	struct Hash
	{
		IHash *pHash;
		void  *pData;
		Size  cbSize;
	};

	typedef Map<String, Hash>::Type   HashesMap;

	HashesMap   m_mapHashes;

};

}//namespace Hash

}//namespace CX
