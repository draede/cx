/* 
 * CX - C++ framework for general purpose development
 *
 * https://github.com/draede/cx
 * 
 * Copyright (C) 2014-2015 draede - draede [at] outlook [dot] com
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


#include "CX/IO/IFileSysHelper.hpp"


namespace CX
{

namespace IO
{

class LocalFileSysHelper : public IFileSysHelper
{
public:

	static LocalFileSysHelper &Get();

	virtual const Char *GetPathSep();

	virtual Status AppendPath(String &sPath, const Char *szStr);

	virtual bool IsAbsPath(const Char *szPath);

	virtual Status CopyFile(const Char *szOldPath, const Char *szNewPath);

	virtual Status RenameFile(const Char *szOldPath, const Char *szNewPath);

	virtual Status RemoveFile(const Char *szPath);

	virtual Status CreateFolder(const Char *szPath);

	virtual Status RenameFolder(const Char *szOldPath, const Char *szNewPath);

	virtual Status RemoveFolder(const Char *szPath);

private:

	LocalFileSysHelper();

	~LocalFileSysHelper();

};

}//namespace IO

}//namespace CX


#endif

