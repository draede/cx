/* 
 * CX - C++ framework for general purpose development
 *
 * https://github.com/draede/cx
 * 
 * Copyright (C) 2014 - 2022 draede
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


#if defined(CX_OS_ANDROID)


#include "CX/IO/IOutputStream.hpp"
#include "CX/C/stdio.h"
#include "CX/String.hpp"
#include "CX/APIDefs.hpp"


namespace CX
{

namespace IO
{

class CX_API FileOutputStream : public IOutputStream
{
public:

	FileOutputStream(const Char *szPath, bool bAppend = false);

	FileOutputStream(const WChar *wszPath, bool bAppend = false);

	~FileOutputStream();

	virtual Status Write(const void *pBuffer, Size cbReqSize, Size *pcbAckSize);

	virtual Status GetSize(UInt64 *pcbSize) const;

	virtual Bool IsOK() const;

	virtual const Char *GetPath() const;

	virtual Status Flush();

private:

	FILE     *m_pFile;

#pragma warning(push)
#pragma warning(disable: 4251)
	String   m_sPath;
#pragma warning(pop)

};

}//namespace IO

}//namespace CX


#endif

