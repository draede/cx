/* 
 * CX - C++ framework for general purpose development
 *
 * https://github.com/draede/cx
 * 
 * Copyright (C) 2014 - 2021 draede - draede [at] outlook [dot] com
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
#include "CX/IO/Platform/Windows/MemoryMappedFile.hpp"
#include "CX/Data/NPY/Format.hpp"
#include "CX/Data/NPY/ByteOrder.hpp"
#include "CX/Data/NPY/Type.hpp"
#include "CX/Data/NPY/Version.hpp"
#include "CX/Data/NPY/Header.hpp"
#include "CX/Data/NPY/HeaderSAXDictionaryObserver.hpp"


namespace CX
{

namespace Data
{

namespace NPY
{

class CX_API Reader
{
public:

	Reader();

	~Reader();

	Status Open(const Char *szPath);

	Status Open(const WChar *wszPath);

	Status Close();

	const void *GetRows(Size cStartRowIndex, Size cReqRowsCount, Size *pcAckRowsCount);

	Bool IsOK() const;

	const Char *GetPathA() const;

	const WChar *GetPathW() const;

	Size GetRowsCount() const;

	Size GetColumnsCount() const;

	Type GetType() const;

	ByteOrder GetByteOrder() const;

	Size GetRowSize() const;

	Format GetFormat() const;

	Version GetVersion() const;

private:

	static const UInt64    INITIAL_MAP_SIZE = 4 * 1024 * 1024;

#pragma warning(push)
#pragma warning(disable: 4251)
	String                 m_sPath;
	WString                m_wsPath;
#pragma warning(pop)

	IO::MemoryMappedFile   m_mmf;
	Header                 m_header;
	UInt32                 m_cbAllocationGranularity;

	Status Open();

};

}//namespace NPY

}//namespace Data

}//namespace CX
