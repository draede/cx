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


#include "CX/Types.hpp"
#include "CX/Status.hpp"
#include "CX/String.hpp"
#include "CX/Limits.hpp"
#include "CX/APIDefs.hpp"


namespace CX
{

namespace Str
{

class CX_API UTF8
{
public:

	//TYPE_SIZE_MAX == cUTF8Len => null terminated and the output will have an 0 added
	//*pcWCharLen contains the max size of wszWChar
	//if NULL == wszWChar => *pcWCharLen will contain the needed len
	static Status ToWChar(const Char *szUTF8, Size cUTF8Len, WChar *wszWChar, Size *pcWCharLen);

	//TYPE_SIZE_MAX == cWCharLen => null terminated and the output will have an 0 added
	//*pcUTF8Len contains the max size of szUTF8
	//if NULL == szUTF8 => *pcUTF8Len will contain the needed len
	static Status FromWChar(const WChar *wszWChar, Size cWCharLen, Char *szUTF8, Size *pcUTF8Len);

	//TYPE_SIZE_MAX == cUTF8Len => null terminated
	static Status ToWChar(const Char *szUTF8, WString *psWChar, Size cUTF8Len = TYPE_SIZE_MAX);

	//TYPE_SIZE_MAX == cWCharLen => null terminated
	static Status FromWChar(const WChar *wsWChar, String *psUTF8, Size cWCharLen = TYPE_SIZE_MAX);

private:

	UTF8();

	~UTF8();

};

}//namespace Str

}//namespace CX


#endif

