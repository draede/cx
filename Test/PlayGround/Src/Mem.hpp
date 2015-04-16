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


#include "CX/Mem.hpp"
#include "CX/Print.hpp"
#include "CX/Platform.hpp"


inline void Mem_DumpAllocs()
{
	CX::Mem::AllocsVector vectorAllocs;

	CX::Mem::GetCurrentAllocs(vectorAllocs);
	CX::Mem::PrintAllocs(stdout, vectorAllocs);
	CX::Print(stdout, "\n");
}

class MemTest: public CX::IObject
{
};

inline void Mem_Test1()
{
	CX::Mem::SetFlags(CX::Mem::Flag_SourceMemTrack);

	MemTest *pTest = new MemTest();
	char    *x     = (char *)CX::Mem::Alloc(sizeof(char) * 100);

	memcpy(x, "this is a test", 14);

	CX::Print(stdout, "\nIntermediate memory dump (leaks should be detected)\n");
	Mem_DumpAllocs();

	CX::Mem::Free(x);
	delete pTest;

	CX::Print(stdout, "\nFinal memory dump (no leaks should be detected)\n");
	Mem_DumpAllocs();
}
