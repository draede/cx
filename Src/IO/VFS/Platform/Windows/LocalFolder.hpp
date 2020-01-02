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


#include "CX/Platform.hpp"


#if defined(CX_OS_WINDOWS)


#include "CX/IO/VFS/Platform/Windows/Local.hpp"
#include "CX/Str/UTF8.hpp"
#include "CX/C/Platform/Windows/windows.h"


namespace CX
{

namespace IO
{

namespace VFS
{

class LocalFolder : public IFolder
{
public:

	LocalFolder(const WChar *wszBasePath = NULL);

	~LocalFolder();

	virtual Status GoToFirstEntry();

	virtual Status GoToNextEntry();

	virtual EntryType GetEntryType();

	virtual const Char *GetEntryPath();

	virtual const WChar *GetEntryPathW();

private:

	WString            m_wsBasePath;
	HANDLE             m_hFind;
	WIN32_FIND_DATAW   m_data;
	
	Char               m_drives[26][3];
	WChar              m_drivesW[26][7];
	Size               m_cDrivesCount;
	Size               m_cDrivesIndex;

	String             m_sEntryPath;
	WString            m_wsEntryPath;

};

}//namespace VFS

}//namespace IO

}//namespace CX


#endif
