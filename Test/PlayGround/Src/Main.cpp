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

#include "Str_UTF8.hpp"
#include "Util_StackTrace.hpp"
#include "Mem.hpp"
#include "MultiHashSetAndHashMap.h"
#include "CX/Str/UTF8.hpp"


using namespace CX;


int main(int argc, char *argv[])
{
	const Char    szUTF8In[]   = "this is a test";
	const WChar   wszUTF16In[] = L"this is a test";
	Char          szUTF8Out[256];
	WChar         wszUTF16Out[256];
	String        sOut;
	WString       wsOut;
	Size          cWLen;
	Size          cLen;
	Status        status;

	status = Str::UTF8::ToWChar(szUTF8In, TYPE_SIZE_MAX, wszUTF16Out, &cWLen);
	status = Str::UTF8::FromWChar(wszUTF16In, TYPE_SIZE_MAX, szUTF8Out, &cLen);
	status = Str::UTF8::ToWChar(szUTF8In, 15, wszUTF16Out, &cWLen);
	status = Str::UTF8::FromWChar(wszUTF16In, 15, szUTF8Out, &cLen);

	status = Str::UTF8::ToWChar(szUTF8In, &wsOut, TYPE_SIZE_MAX);
	cWLen = wsOut.size();
	status = Str::UTF8::FromWChar(wszUTF16In, &sOut, TYPE_SIZE_MAX);
	cLen = sOut.size();
	status = Str::UTF8::ToWChar(szUTF8In, &wsOut, 15);
	cWLen = wsOut.size();
	status = Str::UTF8::FromWChar(wszUTF16In, &sOut, 15);
	cLen = sOut.size();


	argc;
	argv;

	//Str_UTF8_Test1();
	//Util_StackTrace_Test1();

	//Mem_Test1();

	MultiHashSetAndHashMap_Test1();

	return 0;
}

