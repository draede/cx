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


#include "CX/Str/UTF8.hpp"


inline void Str_UTF8_Test1()
{
	CX::Byte    szUTF8In[] = { 0xE4, 0xBD, 0xA0, 0xE5, 0xA5, 0xBD, 0xE4, 0xB8, 0x96, 0xE7, 0x95, 0x8C, 0x00 };
	CX::WChar   wszUTF16In[] = { 0x4F60, 0x597D, 0x4E16, 0x754C, 0x0 };
	CX::String  sUTF8Out;
	CX::WString wsUTF16Out;
	CX::Char    *szUTF8Out;
	CX::WChar   *wszUTF16Out;
	CX_Size     cLen;
	CX::Status  status;

	status = CX::Str::UTF8::ToUTF16((const CX::Char *)szUTF8In, &wsUTF16Out);
	status = CX::Str::UTF8::FromUTF16(wszUTF16In, &sUTF8Out);

	status = CX::Str::UTF8::ToUTF16((const CX::Char *)szUTF8In, CX::TYPE_SIZE_MAX, NULL, &cLen);
	wszUTF16Out = new CX::WChar[cLen];
	status = CX::Str::UTF8::ToUTF16((const CX::Char *)szUTF8In, CX::TYPE_SIZE_MAX, wszUTF16Out, &cLen);

	status = CX::Str::UTF8::FromUTF16(wszUTF16In, CX::TYPE_SIZE_MAX, NULL, &cLen);
	szUTF8Out = new CX::Char[cLen];
	status = CX::Str::UTF8::FromUTF16(wszUTF16In, CX::TYPE_SIZE_MAX, szUTF8Out, &cLen);

	delete [] wszUTF16Out;
	delete [] szUTF8Out;
}

