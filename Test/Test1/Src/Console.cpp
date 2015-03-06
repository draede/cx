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

#include "CX/Sys/DynLib.hpp"
#include "CX/Print.hpp"
#include <stdio.h>
#include "DLL.h"

class Test
{
public:

	Test()
	{
		printf("Test()\n");
	}

	~Test()
	{
		printf("~Test()\n");
	}

};

typedef void (* TestVectorFunc)(CX::Vector<int>::Type *);

typedef void * (* TestMemFunc)(CX::Size);


#include "CX/Str/UTF8.hpp"

int main(int argc, char *argv[])
{
	argc;
	argv;

	//test UTF8<->UTF16
	{
		CX::Char    szUTF8In[]   = {0xE4, 0xBD, 0xA0, 0xE5, 0xA5, 0xBD, 0xE4, 0xB8, 0x96, 0xE7, 
		                            0x95, 0x8C, 0x0};
		CX::WChar   wszUTF16In[] = {0x4F60, 0x597D, 0x4E16, 0x754C, 0x0};
		CX::String  sUTF8Out;
		CX::WString wsUTF16Out;
		CX::Char    *szUTF8Out;
		CX::WChar   *wszUTF16Out;
		CX_Size     cLen;
		CX::Status  status;

		status = CX::Str::UTF8::ToUTF16(szUTF8In, &wsUTF16Out);
		status = CX::Str::UTF8::FromUTF16(wszUTF16In, &sUTF8Out);

		status = CX::Str::UTF8::ToUTF16(szUTF8In, CX::SIZET_MAX, NULL, &cLen);
		wszUTF16Out = CX::NewArr<CX::WChar>(cLen);
		status = CX::Str::UTF8::ToUTF16(szUTF8In, CX::SIZET_MAX, wszUTF16Out, &cLen);

		status = CX::Str::UTF8::FromUTF16(wszUTF16In, CX::SIZET_MAX, NULL, &cLen);
		szUTF8Out = CX::NewArr<CX::Char>(cLen);
		status = CX::Str::UTF8::FromUTF16(wszUTF16In, CX::SIZET_MAX, szUTF8Out, &cLen);

		CX::DeleteArr(wszUTF16Out);
		CX::DeleteArr(szUTF8Out);
	}

	CX::Print(stdout, "{1} => '{2}'", 100, "a123b");

	char *x = (char *)CX::Alloc(100);

	CX::Free(x);

	Test *pTest = CX::New<Test>();

	CX::Delete(pTest);

	CX::Sys::DynLib	dynlib;
	TestVectorFunc		pfnTestVector;
	TestMemFunc			pfnTestMem;

	if (dynlib.Load("dll.dll").IsOK())
	{
		if (NULL != (pfnTestVector = (TestVectorFunc)dynlib.GetFunc("TestVector")) &&
				NULL != (pfnTestMem = (TestMemFunc)dynlib.GetFunc("TestMem")))
		{
			CX::Vector<int>::Type vectorInts;

			pfnTestVector(&vectorInts);
			printf("%d\n", vectorInts[0]);
			printf("%d\n", vectorInts[1]);
			printf("%d\n", vectorInts[2]);
			vectorInts.clear();

			void *a  = pfnTestMem(100);
			CX::Free(a);
		}
		dynlib.Unload();
	}

	return 0;
}

