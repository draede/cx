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


#include "CX/STLAlloc.hpp"
#include "CX/C/string.h"
#include "CX/Platform.hpp"
#include <string>


namespace CX
{

typedef std::basic_string<Char, std::char_traits<Char>, STLAlloc<Char> >         String;

typedef std::basic_string<WChar, std::char_traits<WChar>, STLAlloc<WChar> >      WString;

struct CaseInsensitiveOrderPolicy
{
	Bool operator()(const String &s1, const String &s2) const
	{
#pragma warning(push)
#pragma warning(disable: 4996)
		return (0 > cx_stricmp(s1.c_str(), s2.c_str()));
#pragma warning(pop)
	}
};

struct CaseSensitiveOrderPolicy
{
	Bool operator()(const String &s1, const String &s2) const
	{
		return (0 > cx_strcmp(s1.c_str(), s2.c_str()));
	}
};

#ifdef CX_OS_WINDOWS

struct WCaseInsensitiveOrderPolicy
{
	Bool operator()(const WString &s1, const WString &s2) const
	{
#pragma warning(push)
#pragma warning(disable: 4996)
		return (0 > cxw_stricmp(s1.c_str(), s2.c_str()));
#pragma warning(pop)
	}
};

struct WCaseSensitiveOrderPolicy
{
	Bool operator()(const WString &s1, const WString &s2) const
	{
		return (0 > cxw_strcmp(s1.c_str(), s2.c_str()));
	}
};

#endif

}//namespace CX

