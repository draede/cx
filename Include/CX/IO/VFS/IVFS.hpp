/* 
 * CX - C++ framework for general purpose development
 *
 * https://github.com/draede/cx
 * 
 * Copyright (C) 2014 - 2019 draede - draede [at] outlook [dot] com
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
#include "CX/APIDefs.hpp"
#include "CX/IO/VFS/IFolder.hpp"
#include "CX/IO/IInputStream.hpp"
#include "CX/IO/IOutputStream.hpp"


namespace CX
{

namespace IO
{

namespace VFS
{

class CX_API IVFS
{
public:

	typedef IVFS * (* CreateProc)();
	typedef void (* DestroyProc)(IVFS *pVFS);

	virtual const Char *GetName() const = 0;

	virtual Status Open(const Char *szPath) = 0;

	virtual Status OpenW(const WChar *wszPath) = 0;

	virtual Status Close() = 0;

	virtual Bool IsOK() const = 0;

	virtual Bool IsReadable() const = 0;

	virtual Bool IsWriteable() const = 0;

	virtual Status CreateFolder(const Char *szPath) = 0;

	virtual Status CreateFolderW(const WChar *wszPath) = 0;

	virtual IFolder *OpenFolder(const Char *szPath = NULL) = 0;

	virtual IFolder *OpenFolderW(const WChar *wszPath = NULL) = 0;

	virtual Status CloseFolder(IFolder *pFolder) = 0;

	virtual IInputStream *OpenInputStream(const Char *szPath) = 0;

	virtual IInputStream *OpenInputStreamW(const WChar *wszPath) = 0;

	virtual Status CloseInputStream(IInputStream *pInputStream) = 0;

	virtual IOutputStream *CreateOutputStream(const Char *szPath) = 0;

	virtual IOutputStream *CreateOutputStreamW(const WChar *wszPath) = 0;

	virtual Status CloseOutputStream(IOutputStream *pOutputStream) = 0;

protected:

	virtual ~IVFS() { }

};

}//namespace VFS

}//namespace IO

}//namespace CX 
