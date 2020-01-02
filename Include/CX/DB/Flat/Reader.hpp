/* 
 * CX - C++ framework for general purpose development
 *
 * https://github.com/draede/cx
 * 
 * Copyright (C) 2014 - 2020 draede - draede [at] outlook [dot] com
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
#include "CX/IO/MemoryMappedFile.hpp"
#include "CX/DB/Flat/Header.hpp"
#include "CX/DB/Flat/Footer.hpp"


namespace CX
{

namespace DB
{

namespace Flat
{

class Reader
{
public:

	Reader();

	~Reader();

	Status Open(const Char *szPath);

	Status Open(const WChar *wszPath);

	Status Close();

	Bool IsOK() const;

	UInt32 GetRecordSize() const;

	UInt32 GetRecordsCount() const;

	UInt32 GetHeaderSize() const;

	const void *GetHeader() const;

	UInt32 GetFooterSize() const;

	const void *GetFooter() const;

	const void *GetRecords() const;

	const void *GetRecord(UInt32 cIndex) const;

private:

	IO::MemoryMappedFile   m_mmf;
	UInt32                 m_cRecords;
	UInt32                 m_cbRecordSize;
	const void             *m_pRecords;
	UInt32                 m_cbHeaderSize;
	const void             *m_pHeader;
	UInt32                 m_cbFooterSize;
	const void             *m_pFooter;

	Status Open();

};

}//namespace Flat

}//namespace DB

}//namespace CX
