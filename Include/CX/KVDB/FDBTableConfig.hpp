/* 
 * CX - C++ framework for general purpose developement
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


#include "CX/KVDB/ITableConfig.hpp"


namespace CX
{

namespace KVDB
{

class CX_API FDBTableConfig : public ITableConfig
{
public:

	typedef int(*KeyComparatorFunc)(void *pDataA, Size cbSizeA, void *pDataB, Size cbSizeB);

	static const bool                DEFAULT_CREATE_IF_MISSING = true;

	static const KeyComparatorFunc   DEFAULT_KEY_COMPARATOR() { return NULL; }

	FDBTableConfig()
	{
		m_bCreateIfMissing = DEFAULT_CREATE_IF_MISSING;
		m_pfnKeyComparator = DEFAULT_KEY_COMPARATOR();
	}

	bool                m_bCreateIfMissing;
	KeyComparatorFunc   m_pfnKeyComparator;

};

}//namespace KVDB

}//namespace CX

