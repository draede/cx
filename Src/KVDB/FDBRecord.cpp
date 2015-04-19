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

#include "CX/KVDB/FDBRecord.hpp"
#include "libforestdb/forestdb.h"


namespace CX
{

namespace KVDB
{

FDBRecord::FDBRecord(IIterator *pIterator, void *pDoc)
{
	m_pIterator = pIterator;
	m_pDoc      = pDoc;
}

FDBRecord::~FDBRecord()
{
}

IIterator *FDBRecord::GetIterator()
{
	return m_pIterator;
}

void *FDBRecord::GetDoc()
{
	return m_pDoc;
}

const void *FDBRecord::GetKey()
{
	return ((fdb_doc *)m_pDoc)->key;
}

Size FDBRecord::GetKeySize()
{
	return ((fdb_doc *)m_pDoc)->keylen;
}

const void *FDBRecord::GetValue()
{
	return ((fdb_doc *)m_pDoc)->body;
}

Size FDBRecord::GetValueSize()
{
	return ((fdb_doc *)m_pDoc)->bodylen;
}

}//namespace KVDB

}//namespace CX

