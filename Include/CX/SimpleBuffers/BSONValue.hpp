/* 
 * CX - C++ framework for general purpose development
 *
 * https://github.com/draede/cx
 * 
 * Copyright (C) 2014 - 2017 draede - draede [at] outlook [dot] com
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
#include "CX/SimpleBuffers/IReader.hpp"
#include "CX/SimpleBuffers/DataReader.hpp"
#include "CX/SimpleBuffers/DataWriter.hpp"
#include "CX/SimpleBuffers/DataIniter.hpp"


namespace CX
{

namespace SimpleBuffers
{

class BSONValue
{
public:

	BSONValue();

	BSONValue(const BSONValue &v);

	~BSONValue();

	const void *GetBuffer() const;

	void *GetBuffer();

	Size GetBufferSize() const ;

	const void *GetAllBuffer() const;

	void *GetAllBuffer();

	Size GetAllBufferSize() const;

	int Compare(const BSONValue &v) const;

	Status Copy(const BSONValue &v);

	BSONValue &operator=(const BSONValue &v);

	bool operator<(const BSONValue &v) const;

	bool operator>(const BSONValue &v) const;

	ICustom::Allocator *GetAllocator();

	void Init();

	Status Write(IWriter *pWriter, const CX::Char *szName = NULL) const;

	Status Read(IReader *pReader, const CX::Char *szName = NULL);

private:

	void                 *m_pData;
	Size                 m_cbSize;
	void                 *m_pAllData;
	Size                 m_cbAllSize;
	ICustom::Allocator   m_allocator;

};

}//namespace SimpleBuffers

}//namespace CX
