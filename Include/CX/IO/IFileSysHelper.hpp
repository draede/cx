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


namespace CX
{

namespace IO
{

class CX_API IFileSysHelper
{
public:

	enum FileSizeType
	{
		FileSizeType_Bytes,     // < 1024
		FileSizeType_KiloBytes, // < 1048576
		FileSizeType_MegaBytes, // < 1073741824
		FileSizeType_GigaBytes, // < 1099511627776
		FileSizeType_TeraBytes, // < 1125899906842624
		FileSizeType_PetaBytes,
	};

	virtual ~IFileSysHelper() { }

	virtual const Char *GetPathSep() = 0;

	virtual Status AppendPath(String &sPath, const Char *szStr) = 0;

	virtual bool IsAbsPath(const Char *szPath) = 0;

#pragma push_macro("CopyFile")
#undef CopyFile
	virtual Status CopyFile(const Char *szOldPath, const Char *szNewPath) = 0;
#pragma pop_macro("CopyFile")

	virtual Status RenameFile(const Char *szOldPath, const Char *szNewPath) = 0;

	virtual Status RemoveFile(const Char *szPath) = 0;

	virtual Status CreateFolder(const Char *szPath) = 0;

	virtual Status RenameFolder(const Char *szOldPath, const Char *szNewPath) = 0;

	virtual Status RemoveFolder(const Char *szPath) = 0;

	virtual Double GetNiceSize(UInt64 cbSize, FileSizeType *pnFileSizeType)
	{
		if (1024 > cbSize)
		{
			*pnFileSizeType = FileSizeType_Bytes;

			return (Double)cbSize;
		}
		else
		if (1048576 > cbSize)
		{
			*pnFileSizeType = FileSizeType_KiloBytes;

			return (Double)cbSize / 1024.0;
		}
		else
		if (1073741824 > cbSize)
		{
			*pnFileSizeType = FileSizeType_MegaBytes;

			return (Double)cbSize / 1048576.0;
		}
		else
		if (1099511627776 > cbSize)
		{
			*pnFileSizeType = FileSizeType_GigaBytes;

			return (Double)cbSize / 1073741824.0;
		}
		else
		if (1125899906842624 > cbSize)
		{
			*pnFileSizeType = FileSizeType_TeraBytes;

			return (Double)cbSize / 1099511627776.0;
		}
		else
		{
			*pnFileSizeType = FileSizeType_PetaBytes;

			return (Double)cbSize / 1125899906842624.0;
		}
	}

};

}//namespace IO

}//namespace CX

