/* 
 * CX - C++ framework for general purpose developement
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


#include "CX/Types.hpp"
#include "CX/Status.hpp"
#include "CX/String.hpp"
#include "CX/Limits.hpp"
#include "CX/APIDefs.hpp"
#include "CX/IObject.hpp"


namespace CX
{

namespace Str
{

class CX_API UTF8 : public IObject
{
public:

	//TYPE_SIZE_MAX == cUTF8Len => null terminated
	//*pcUTF16Len contains the max size of wszUTF16
	//if NULL == wszUTF16 => *pcUTF16Len will contain the needed len
	static Status ToUTF16(const Char *szUTF8, Size cUTF8Len, WChar *wszUTF16, Size *pcUTF16Len);

	//TYPE_SIZE_MAX == cUTF16Len => null terminated
	//*pcUTF8Len contains the max size of szUTF8
	//if NULL == szUTF8 => *pcUTF8Len will contain the needed len
	static Status FromUTF16(const WChar *wszUTF16, Size cUTF16Len, Char *szUTF8, Size *pcUTF8Len);

	//TYPE_SIZE_MAX == cUTF8Len => null terminated
	static Status ToUTF16(const Char *szUTF8, WString *psUTF16, Size cUTF8Len = TYPE_SIZE_MAX);

	//TYPE_SIZE_MAX == cWCharLen => null terminated
	static Status FromUTF16(const WChar *wsUTF16, String *psUTF8, Size cUTF16Len = TYPE_SIZE_MAX);

private:

	UTF8();

	~UTF8();

};

}//namespace Str

}//namespace CX


#endif

