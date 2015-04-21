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


#include "CX/KVDB/IRecord.hpp"


namespace CX
{

namespace KVDB
{

class CX_API MemRecord : public IRecord
{
public:

	virtual const void *GetKey();

	virtual Size GetKeySize();

	virtual const void *GetValue();

	virtual Size GetValueSize();

	virtual IIterator *GetIterator();

protected:

	friend class MemIterator;

	MemRecord(IIterator *pIterator, void *pKey, Size cbKeySize, void *pValue, Size cbValueSize);

	~MemRecord();

private:

	IIterator   *m_pIterator;
	void        *m_pKey;
	Size        m_cbKeySize;
	void        *m_pValue;
	Size        m_cbValueSize;

};

}//namespace KVDB

}//namespace CX

