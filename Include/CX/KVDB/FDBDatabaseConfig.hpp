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


#include "CX/KVDB/IDatabaseConfig.hpp"


namespace CX
{

namespace KVDB
{

class CX_API FDBDatabaseConfig : public IDatabaseConfig
{
public:
	
	enum Durability
	{
		Durability_None,          //Synchronous commit through OS page cache
		Durability_Direct,        //Synchronous commit through the direct IO option to bypass the OS page cache
		Durability_Async,         //Asynchronous commit through OS page cache
		Durability_DirectAsync,   //Asynchronous commit through the direct IO option to bypass the OS page cache
	};

	static const bool         DEFAULT_READONLY = false;
	static const UInt64       DEFAULT_BUFFER_CACHE_SIZE = 4194304;
	static const Durability   DEFAULT_DURABILITY = Durability_Async;

	FDBDatabaseConfig()
	{
		m_bReadOnly         = DEFAULT_READONLY;
		m_cbBufferCacheSize = DEFAULT_BUFFER_CACHE_SIZE;
		m_nDurability       = DEFAULT_DURABILITY;
	}

	bool         m_bReadOnly;
	UInt64       m_cbBufferCacheSize;
	Durability   m_nDurability;

};

}//namespace KVDB

}//namespace CX

