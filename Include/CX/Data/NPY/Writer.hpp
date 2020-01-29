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
#include "CX/String.hpp"
#include "CX/APIDefs.hpp"
#include "CX/Data/NPY/Format.hpp"
#include "CX/Data/NPY/ByteOrder.hpp"
#include "CX/Data/NPY/Type.hpp"
#include "CX/Data/NPY/Version.hpp"
#include "CX/Data/NPY/Consts.hpp"
#include "CX/Data/NPY/Header.hpp"


namespace CX
{

namespace Data
{

namespace NPY
{

class CX_API Writer
{
public:

	static const Size     MIN_BUFFER_SIZE     = 1024;
	static const Size     MAX_BUFFER_SIZE     = 1024 * 1024 * 1024;
	static const Size     DEFAULT_BUFFER_SIZE = 8 * 1024 * 1024;

	Writer();

	~Writer();

	Status Create(const Char *szPath, Size cColumnsCount, Type nType, ByteOrder nByteOrder = ByteOrder_LittleEndian, 
	             Format nFormat = Format_C, Version nVersion = Version_1_0, Size cbBufferSize = DEFAULT_BUFFER_SIZE);

	Status Create(const WChar *wszPath, Size cColumnsCount, Type nType, ByteOrder nByteOrder = ByteOrder_LittleEndian, 
	             Format nFormat = Format_C, Version nVersion = Version_1_0, Size cbBufferSize = DEFAULT_BUFFER_SIZE);

	Status Close();

	Status Write(const void *pBuffer, Size cbSize);

	Bool IsOK() const;

	const Char *GetPathA() const;

	const WChar *GetPathW() const;

	Size GetColumnsCount() const;

	Type GetType() const;

	ByteOrder GetByteOrder() const;

	Size GetRowSize() const;

	Format GetFormat() const;

	Version GetVersion() const;

	Size GetBufferSize() const;

	UInt64 GetReceivedBytes() const;

	UInt64 GetWrittenBytes() const;

private:

	static const Size   MIN_EXTRA_PADDING = 20;//we want to be able to update the rows count in place

#pragma warning(push)
#pragma warning(disable: 4251)
	String             m_sPath;
	WString            m_wsPath;
#pragma warning(pop)

	void               *m_pFile;
	Header             m_header;
	Byte               *m_pBuffer;
	Size               m_cbBufferSize;
	Size               m_cbBufferUsedSize;
	UInt64             m_cbReceived;
	UInt64             m_cbWritten;

	Status CheckCreateArgs(Size cColumnsCount, Type nType, ByteOrder nByteOrder, Format nFormat, Version nVersion, 
	                       Size cbBufferSize);

	Status Create(Size cColumnsCount, Type nType, ByteOrder nByteOrder, Format nFormat, Version nVersion, 
	              Size cbBufferSize);

	Status Flush();

};

}//namespace NPY

}//namespace Data

}//namespace CX
