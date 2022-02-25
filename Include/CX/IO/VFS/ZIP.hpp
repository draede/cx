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



#include "CX/Types.hpp"
#include "CX/Status.hpp"
#include "CX/APIDefs.hpp"
#include "CX/IO/VFS/IVFS.hpp"


namespace CX
{

namespace IO
{

namespace VFS
{

class CX_API ZIP : IVFS
{
public:

	static const Char *NAME() { return "zip"; }

	static Status ForceRegister();

	ZIP();

	~ZIP();
	
	virtual const Char *GetName() const;

	virtual Status Open(const Char *szPath);

	virtual Status OpenW(const WChar *wszPath);

	virtual Status Close();

	virtual Bool IsOK() const;

	virtual Bool IsReadable() const;

	virtual Bool IsWriteable() const;

	virtual Status CreateFolder(const Char *szPath);

	virtual Status CreateFolderW(const WChar *wszPath);

	virtual IFolder *OpenFolder(const Char *szPath = NULL);

	virtual IFolder *OpenFolderW(const WChar *wszPath = NULL);

	virtual Status CloseFolder(IFolder *pFolder);

	virtual IInputStream *OpenInputStream(const Char *szPath);

	virtual IInputStream *OpenInputStreamW(const WChar *wszPath);

	virtual Status CloseInputStream(IInputStream *pInputStream);

	virtual IOutputStream *CreateOutputStream(const Char *szPath);

	virtual IOutputStream *CreateOutputStreamW(const WChar *wszPath);

	virtual Status CloseOutputStream(IOutputStream *pOutputStream);

private:

	static Status   REGISTER_STATUS;

#pragma warning(push)
#pragma warning(disable: 4251)
	String   m_sPath;
#pragma warning(pop)
	void      *m_pZIP;

	static IVFS *Create();

	static void Destroy(IVFS *pVFS);

};

}//namespace VFS

}//namespace IO

}//namespace CX 
