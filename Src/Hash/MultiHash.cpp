/* 
 * CX - C++ framework for general purpose development
 *
 * https://github.com/draede/cx
 * 
 * Copyright (C) 2014 - 2017 draede, draede [at] outlook [dot] com
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
 
#include "CX/Hash/MultiHash.hpp"
#include "CX/Mem.hpp"


namespace CX
{

namespace Hash
{

MultiHash::MultiHash()
{
}

MultiHash::~MultiHash()
{
	for (HashesMap::iterator iter = m_mapHashes.begin(); iter != m_mapHashes.end(); ++iter)
	{
		if (NULL != iter->second.pData)
		{
			Mem::Free(iter->second.pData);
			iter->second.pData = NULL;
			iter->second.cbSize = 0;
		}
	}
	m_mapHashes.clear();
}

Size MultiHash::GetHashesCount() const
{
	return m_mapHashes.size();
}

const Char *MultiHash::GetHashName(Size cIndex) const
{
	if (m_mapHashes.size() <= cIndex)
	{
		return "";
	}

	HashesMap::const_iterator iter = m_mapHashes.begin();

	while (0 < cIndex)
	{
		cIndex--;
		iter++;
	}

	return iter->first.c_str();
}

const void *MultiHash::GetHashResultData(const Char *szName) const
{
	HashesMap::const_iterator iter;
	
	if (m_mapHashes.end() == (iter = m_mapHashes.find(szName)))
	{
		return NULL;
	}

	return iter->second.pData;
}

Size MultiHash::GetHashResultSize(const Char *szName) const
{
	HashesMap::const_iterator iter;

	if (m_mapHashes.end() == (iter = m_mapHashes.find(szName)))
	{
		return 0;
	}

	return iter->second.cbSize;
}

Status MultiHash::Init()
{
	if (m_mapHashes.empty())
	{
		return Status_NotInitialized;
	}

	Status status;

	for (HashesMap::iterator iter = m_mapHashes.begin(); iter != m_mapHashes.end(); ++iter)
	{
		if ((status = iter->second.pHash->Init(NULL)).IsNOK())
		{
			return status;
		}
		if (NULL != iter->second.pData)
		{
			Mem::Free(iter->second.pData);
			iter->second.pData  = NULL;
			iter->second.cbSize = 0;
		}
	}

	return Status();
}

Status MultiHash::Update(const void *pBuffer, Size cbSize)
{
	if (m_mapHashes.empty())
	{
		return Status_NotInitialized;
	}

	Status status;

	for (HashesMap::iterator iter = m_mapHashes.begin(); iter != m_mapHashes.end(); ++iter)
	{
		if ((status = iter->second.pHash->Update(pBuffer, cbSize)).IsNOK())
		{
			return status;
		}
	}

	return Status();
}

Status MultiHash::Done()
{
	if (m_mapHashes.empty())
	{
		return Status_NotInitialized;
	}

	Status status;

	for (HashesMap::iterator iter = m_mapHashes.begin(); iter != m_mapHashes.end(); ++iter)
	{
		if (NULL != iter->second.pData && iter->second.pHash->GetSize() != iter->second.cbSize)
		{
			Mem::Free(iter->second.pData);
			iter->second.pData  = NULL;
			iter->second.cbSize = 0;
		}
		if (NULL == iter->second.pData)
		{
			if (NULL == (iter->second.pData = Mem::Alloc(iter->second.pHash->GetSize())))
			{
				return Status_MemAllocFailed;
			}
			iter->second.cbSize = iter->second.pHash->GetSize();
		}
		if ((status = iter->second.pHash->Done(iter->second.pData)).IsNOK())
		{
			return status;
		}
	}

	return Status();
}

}//namespace Hash

}//namespace CX
