/* 
 * CX - C++ framework for general purpose development
 *
 * https://github.com/draede/cx
 * 
 * Copyright (C) 2014 - 2018 draede - draede [at] outlook [dot] com
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
#include "CX/DB/Flat/Header.hpp"
#include "CX/DB/Flat/Footer.hpp"


namespace CX
{

namespace DB
{

namespace Flat
{

class Writer
{
public:

	static const UInt32   BUFFER_SIZE = 1 * 1024 * 1024;

	Writer();

	~Writer();

	Status Create(const Char *szPath, UInt32 cbRecordSize, const void *pHeader = NULL, UInt32 cbHeaderSize = 0, 
	              UInt32 cbBufferSize = BUFFER_SIZE);

	Status Create(const WChar *wszPath, UInt32 cbRecordSize, const void *pHeader = NULL, UInt32 cbHeaderSize = 0,
	              UInt32 cbBufferSize = BUFFER_SIZE);

	Status Open(const Char *szPath, UInt32 *pcbRecordSize = NULL, void **ppHeader = NULL, UInt32 *pcbHeaderSize = NULL,
	            void **ppFooter = NULL, UInt32 *pcbFooterSize = NULL, UInt32 cbBufferSize = BUFFER_SIZE);

	Status Open(const WChar *wszPath, UInt32 *pcbRecordSize = NULL, void **ppHeader = NULL, 
	            UInt32 *pcbHeaderSize = NULL, void **ppFooter = NULL, UInt32 *pcbFooterSize = NULL, 
	            UInt32 cbBufferSize = BUFFER_SIZE);

	Status Close(const void *pFooter = NULL, UInt32 cbFooterSize = 0);

	Bool IsOK() const;

	UInt32 GetRecordSize() const;

	UInt32 GetRecordsCount() const;

	Status Append(const void *records, UInt32 cCount = 1);

	Status FreeHeader(void *pHeader);

	Status FreeFooter(void *pFooter);

private:

	void     *m_pFile;
	Byte     *m_buffer;
	UInt32   m_cbBufferSize;
	UInt32   m_cbUsedBufferSize;
	UInt32   m_cbRecordSize;
	UInt32   m_cRecords;

	Status Create(UInt32 cbRecordSize, const void *pHeader, UInt32 cbHeaderSize, UInt32 cbBufferSize);

	Status Open(UInt32 *pcbRecordSize = NULL, void **ppHeader = NULL, UInt32 *pcbHeaderSize = NULL,
	            void **ppFooter = NULL, UInt32 *pcbFooterSize = NULL, UInt32 cbBufferSize = BUFFER_SIZE);

	Status Close(Bool bWriteFooter, const void *pFooter, UInt32 cbFooterSize);

	Status Write(const void *pData, UInt32 cbSize);

	Status Flush();

	Status Read(void *pData, UInt32 cbSize);

};

}//namespace Flat

}//namespace DB

}//namespace CX

