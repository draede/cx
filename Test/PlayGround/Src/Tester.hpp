/*
* CX - C++ framework for general purpose development
*
* https://github.com/draede/cx
*
* Copyright (C) 2014 - 2017 draede - draede [at] outlook [dot] com
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
#include "CX/Vector.hpp"
#include "CX/Set.hpp"
#include "CX/String.hpp"
#include <functional>


#define TESTFILES_ROOT_PATH   L"\\\\?\\Q:\\__TESTFILES__"


class Tester
{
public:

	typedef void (* HandleFileProc)(const CX::WChar *, void *);

	static CX::Status RegisterTest(const CX::Char *szName, const std::function<void()> &func);

	static void Run();

	static void AddInclusion(const CX::Char *szPattern);

	static void RemoveInclusion(const CX::Char *szPattern);

	static void RemoveAllInclusions();

	static void EnumFiles(const CX::WChar *wszPath, HandleFileProc pfnHandleFile, void *pUsrCtx);

private:

	struct TestData
	{
		CX::String           sName;
		std::function<void()> func;
	};

	typedef CX::Set<CX::String, CX::CaseInsensitiveOrderPolicy>::Type   InclusionsSet;

	typedef CX::Vector<TestData>::Type   TestsVector;

	Tester();

	~Tester();

	static TestsVector &GetTests();

	static InclusionsSet &GetInclusions();

	static bool IsAllowedToRun(const CX::String &sTest);

};

class TestHelper
{
public:

	TestHelper(const CX::Char *szName, const std::function<void()> &func);

};

#define REGISTER_TEST(name)   TestHelper   g_ ## name (#name, std::bind(&name));

