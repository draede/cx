/* 
 * CX - C++ framework for general purpose development
 *
 * https://github.com/draede/cx
 * 
 * Copyright (C) 2014 - 2022 draede - draede [at] outlook [dot] com
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


#include "CX/Platform.hpp"


#if defined(CX_OS_WINDOWS)


#include "CX/IO/IInputStream.hpp"
#include "CX/C/stdio.h"
#include "CX/String.hpp"
#include "CX/APIDefs.hpp"


namespace CX
{

namespace IO
{

class CX_API MemoryMappedFile
{
public:

	enum Flag
	{
		Flag_AttrNormal         = 1,
		Flag_AttrSequentialScan = 2,
		Flag_AttrRandomAccess   = 4,
	};

	MemoryMappedFile();

	MemoryMappedFile(const Char *szPath, UInt64 cbMapOffset = 0, UInt64 cbMapSize = 0, UInt32 nFlags = Flag_AttrNormal);

	MemoryMappedFile(const WChar *wszPath, UInt64 cbMapOffset = 0, UInt64 cbMapSize = 0, 
	                 UInt32 nFlags = Flag_AttrNormal);

	~MemoryMappedFile();

	Status Open(const Char *szPath, UInt64 cbMapOffset = 0, UInt64 cbMapSize = 0, UInt32 nFlags = Flag_AttrNormal);

	Status Open(const WChar *wszPath, UInt64 cbMapOffset = 0, UInt64 cbMapSize = 0, UInt32 nFlags = Flag_AttrNormal);

	Status Close();

	Bool IsOK() const;

	UInt64 GetFileSize() const;

	const void *GetMapData() const;

	UInt64 GetMapOffset() const;

	UInt64 GetMapSize() const;

	const Char *GetPath() const;

	void *GetHandle();

	Status Remap(UInt64 cbNewMapOffset, UInt64 cbNewMapSize = 0);

private:

	UInt64    m_cbFileSize;
	void      *m_pMapData;
	UInt64    m_cbMapOffset;
	UInt64    m_cbMapSize;

#pragma warning(push)
#pragma warning(disable: 4251)
	String   m_sPath;
#pragma warning(pop)

	void     *m_pFile;
	void     *m_pFileMap;

	Status OpenInternal(const WChar *wszPath, UInt64 cbMapOffset = 0, UInt64 cbMapSize = 0, 
	                    UInt32 nFlags = Flag_AttrNormal);

};

}//namespace IO

}//namespace CX


#endif

