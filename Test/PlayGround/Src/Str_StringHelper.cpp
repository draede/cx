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

#include "CX/Str/StringHelper.hpp"
#include "CX/Print.hpp"
#include "CX/Mem.hpp"
#include "Tester.hpp"


using namespace CX;


void Str_StringHelper_Test1()
{
	Mem::SetFlags(Mem::Flag_SourceMemTrack);

	{
		const Char *pszBegin;
		const Char *pszEnd;
		bool       bCmp;
		String     sStr1;
		String     sStr2;
		Status     status;

		status = Str::StringHelper::FindSubStr("this is just a test", 0, "just", 4, true, &pszBegin);
		status = Str::StringHelper::FindSubStr("this is just a test", 0, "Just", 4, false, &pszBegin);
		status = Str::StringHelper::FindWithMarkers("this is just a test", 0, " is ", 4, " a", 2, true, &pszBegin, &pszEnd);
		status = Str::StringHelper::FindWithMarkers("this is just a test", 0, " Is ", 4, " A", 2, false, &pszBegin, &pszEnd);
		bCmp   = Str::StringHelper::Compare("this is just a test", 0, "this", 4, false, 4);
		status = Str::StringHelper::Strip(" \ttest\n  ", 0, " \t\n", 3, true, &pszBegin, &pszEnd, Str::StringHelper::Strip_Both);
		status = Str::StringHelper::Replace("this is a test", 0, "is", 2, "ABC", 3, true, sStr1, true);
		status = Str::StringHelper::Replace("this is a test", 0, "is", 2, "ABC", 3, true, sStr2, false);
	}

	Mem::AllocsVector vectorAllocs;
	std::string       sOut;

	Mem::GetCurrentAllocs(vectorAllocs);
	Mem::PrintAllocs(sOut, vectorAllocs);
	Print(stdout, "{1}\n", sOut.c_str());
}

REGISTER_TEST(Str_StringHelper_Test1);

