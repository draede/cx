/*
* CX - C++ framework for general purpose development
*
* https://github.com/draede/cx
*
* Copyright (C) 2014 - 2020 draede - draede [at] outlook [dot] com
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

#include "Tester.hpp"
#include "CX/Print.hpp"
#include "CX/Str/WildMat.hpp"
#include <windows.h>


using namespace CX;


Tester::Tester()
{
}

Tester::~Tester()
{
}

Tester::TestsVector &Tester::GetTests()
{
	static TestsVector vectorTests;

	return vectorTests;
}

Tester::InclusionsSet &Tester::GetInclusions()
{
	static InclusionsSet setInclusions;

	return setInclusions;
}

void Tester::AddInclusion(const CX::Char *szPattern)
{
	InclusionsSet &setInclusions = GetInclusions();

	setInclusions.insert(szPattern);
}

void Tester::RemoveInclusion(const CX::Char *szPattern)
{
	InclusionsSet           &setInclusions = GetInclusions();
	InclusionsSet::iterator iter           = setInclusions.find(szPattern);

	if (setInclusions.end() != iter)
	{
		setInclusions.erase(iter);
	}
}

void Tester::RemoveAllInclusions()
{
	InclusionsSet &setInclusions = GetInclusions();

	setInclusions.clear();
}

bool Tester::IsAllowedToRun(const CX::String &sTest)
{
	InclusionsSet &setInclusions = GetInclusions();

	if (setInclusions.empty())
	{
		return true;
	}

	for (InclusionsSet::iterator iter = setInclusions.begin(); iter != setInclusions.end(); ++iter)
	{
		if (CX::Str::WildMat::Get().MatchA(sTest.c_str(), iter->c_str(), true))
		{
			return true;
		}
	}

	return false;
}

Status Tester::RegisterTest(const CX::Char *szName, const std::function<void()> &func)
{
	TestData test;

	test.sName = szName;
	test.func  = func;
	GetTests().push_back(test);

	return Status();
}

void Tester::Run()
{
	for (TestsVector::const_iterator iter = GetTests().begin(); iter != GetTests().end(); ++iter)
	{
		if (!IsAllowedToRun(iter->sName))
		{
			continue;
		}

		Print(stdout, "Running '{1}'...\n", iter->sName);
		iter->func();
		Print(stdout, "\n");
	}
}

void Tester::EnumFiles(const CX::WChar *wszPath, HandleFileProc pfnHandleFile, void *pUsrCtx)
{
	WIN32_FIND_DATAW   data;
	HANDLE             hFind;
	WString            wsMask;
	WString            wsTmpPath;
	Status             status;

	wsMask = wszPath;
	wsMask += L"\\*.*";
	if (INVALID_HANDLE_VALUE == (hFind = FindFirstFileW(wsMask.c_str(), &data)))
	{
		return;
	}
	for (;;)
	{
		if (!(FILE_ATTRIBUTE_REPARSE_POINT & data.dwFileAttributes))
		{
			wsTmpPath = wszPath;
			wsTmpPath += L"\\";
			wsTmpPath += data.cFileName;
			if (!(FILE_ATTRIBUTE_DIRECTORY & data.dwFileAttributes))
			{
				pfnHandleFile(wsTmpPath.c_str(), pUsrCtx);
			}
			else
			if (0 != wcscmp(data.cFileName, L".") && 0 != wcscmp(data.cFileName, L".."))
			{
				EnumFiles(wsTmpPath.c_str(), pfnHandleFile, pUsrCtx);
			}
		}
		if (!FindNextFileW(hFind, &data))
		{
			break;
		}
	}
	FindClose(hFind);
}

TestHelper::TestHelper(const Char *szName, const std::function<void()> &func)
{
	Tester::RegisterTest(szName, func);
}


