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
#include "CX/Util/MemPool.hpp"
#include "CX/Platform.hpp"


#if defined(CX_OS_WINDOWS)


#include "CX/C/Platform/Windows/windows.h"


namespace CX
{

namespace DB
{

namespace Flat
{

class Writer
{
public:

	static const Size   BUFFER_SIZE     = 65536;
	static const Size   MIN_BUFFER_SIZE = 4096;
	static const Size   MAX_BUFFER_SIZE = 1073741824;

	Writer();

	~Writer();

	//0 == cbRecordSize => variable size for records
	Status Create(const Char *szPath, UInt32 nMagic, UInt32 cbHeaderSize, UInt32 cbRecordSize, 
	              Size cbBufferSize = BUFFER_SIZE);

	//0 == cbRecordSize => variable size for records
	Status Create(const WChar *wszPath, UInt32 nMagic, UInt32 cbHeaderSize, UInt32 cbRecordSize, 
	              Size cbBufferSize = BUFFER_SIZE);

	Status Open(const Char *szPath, Size cbBufferSize = BUFFER_SIZE);

	Status Open(const WChar *wszPath, Size cbBufferSize = BUFFER_SIZE);

	//0 == cbRecordSize => variable size for records
	Status CreateOrOpen(const Char *szPath, UInt32 nMagic, UInt32 cbHeaderSize, UInt32 cbRecordSize, 
	                    Size cbBufferSize = BUFFER_SIZE);

	//0 == cbRecordSize => variable size for records
	Status CreateOrOpen(const WChar *wszPath, UInt32 nMagic, UInt32 cbHeaderSize, UInt32 cbRecordSize, 
	                    Size cbBufferSize = BUFFER_SIZE);

	Status Close(const void *pCustomHeader = NULL, UInt32 cbCustomHeaderSize = 0);

	Status Flush();

	Bool IsOK() const;

	UInt32 GetMagic() const;

	UInt32 GetHeaderSize() const;

	UInt32 GetRecordSize() const;

	UInt32 GetRecordCount() const;

	Size GetBufferSize() const;

	Status GetHeader(void *pHeader, UInt32 cbSize);

	Status AddRecord(const void *pData, UInt32 cbSize);

private:

	HANDLE             m_hFile;
	UInt32             m_nMagic;
	UInt32             m_cbHeaderSize;
	UInt32             m_cbRecordSize;
	UInt32             m_cRecordCount;
	Util::DynMemPool   m_buffer;
	Size               m_cbBufferSize;

	template <typename T>
	Status Write(const T &v)
	{
		return Write(&v, sizeof(T));
	}

	Status Write(const void *pData, Size cbSize);

	template <typename T>
	Status Read(T &v)
	{
		return Read(&v, sizeof(T));
	}

	Status Read(void *pData, Size cbSize);

	Status CreateInternal(UInt32 nMagic, UInt32 cbHeaderSize, UInt32 cbRecordSize, Size cbBufferSize = BUFFER_SIZE);

	Status OpenInternal(Size cbBufferSize = BUFFER_SIZE);

	Status BufferedWrite(const void *pData, Size cbSize);

};

}//namespace Flat

}//namespace DB

}//namespace CX


#endif
