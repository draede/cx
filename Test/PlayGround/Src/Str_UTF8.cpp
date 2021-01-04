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

#include "CX/Str/UTF8.hpp"
#include "Tester.hpp"


using namespace CX;


void Str_UTF8_Test1()
{
	Byte    szUTF8In[] = { 0xE4, 0xBD, 0xA0, 0xE5, 0xA5, 0xBD, 0xE4, 0xB8, 0x96, 0xE7, 0x95, 0x8C, 0x00 };
	WChar   wszUTF16In[] = { 0x4F60, 0x597D, 0x4E16, 0x754C, 0x0 };
	String  sUTF8Out;
	WString wsUTF16Out;
	Char    *szUTF8Out;
	WChar   *wszUTF16Out;
	CX_Size cLen;
	Status  status;

	status = Str::UTF8::ToWChar((const Char *)szUTF8In, &wsUTF16Out); status;
	status = Str::UTF8::FromWChar(wszUTF16In, &sUTF8Out); status;

	status = Str::UTF8::ToWChar((const Char *)szUTF8In, TYPE_SIZE_MAX, NULL, &cLen); status;
	wszUTF16Out = new (std::nothrow) WChar[cLen];
	status = Str::UTF8::ToWChar((const Char *)szUTF8In, TYPE_SIZE_MAX, wszUTF16Out, &cLen); status;

	status = Str::UTF8::FromWChar(wszUTF16In, TYPE_SIZE_MAX, NULL, &cLen); status;
	szUTF8Out = new (std::nothrow) Char[cLen];
	status = Str::UTF8::FromWChar(wszUTF16In, TYPE_SIZE_MAX, szUTF8Out, &cLen); status;

	delete[] wszUTF16Out;
	delete[] szUTF8Out;
}

void Str_UTF8_Test2()
{
	const Char    szUTF8In[] = "this is a test";
	const WChar   wszUTF16In[] = L"this is a test";
	Char          szUTF8Out[256];
	WChar         wszUTF16Out[256];
	String        sOut;
	WString       wsOut;
	Size          cWLen;
	Size          cLen;
	Status        status;

	status = Str::UTF8::ToWChar(szUTF8In, TYPE_SIZE_MAX, wszUTF16Out, &cWLen); status;
	status = Str::UTF8::FromWChar(wszUTF16In, TYPE_SIZE_MAX, szUTF8Out, &cLen); status;
	status = Str::UTF8::ToWChar(szUTF8In, 15, wszUTF16Out, &cWLen); status;
	status = Str::UTF8::FromWChar(wszUTF16In, 15, szUTF8Out, &cLen); status;

	status = Str::UTF8::ToWChar(szUTF8In, &wsOut, TYPE_SIZE_MAX); status;
	cWLen = wsOut.size();
	status = Str::UTF8::FromWChar(wszUTF16In, &sOut, TYPE_SIZE_MAX); status;
	cLen = sOut.size();
	status = Str::UTF8::ToWChar(szUTF8In, &wsOut, 15); status;
	cWLen = wsOut.size();
	status = Str::UTF8::FromWChar(wszUTF16In, &sOut, 15); status;
	cLen = sOut.size();
}

REGISTER_TEST(Str_UTF8_Test1);
REGISTER_TEST(Str_UTF8_Test2);
