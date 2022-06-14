/* 
 * CX - C++ framework for general purpose development
 *
 * https://github.com/draede/cx
 * 
 * Copyright (C) 2014 - 2021 draede
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
#include "CX/IO/Platform/Windows/FileEnumerator2.hpp"
#include "CX/Print.hpp"


using namespace CX;


class MyEnumHandler : public IO::FileEnumerator2::IHandler
{
public:

	virtual Bool OnBegin()
	{
		Print(stdout, "OnBegin\n");

		return True;
	}

	virtual Bool OnEnd(const IO::FileEnumerator2::Stats *pStats)
	{
		Print(stdout, "OnEnd\n");

		return True;
	}

	//called from multiple threads
	virtual Bool OnFile(IO::FileEnumerator2::IFile *pFile, void **ppResult, const IO::FileEnumerator2::Stats *pStats)
	{
		Print(stdout, "OnFile : {1}\n", pFile->GetPath());

		return True;
	}

	//called from a single thread
	virtual Bool OnBeginResults(void **ppContext)
	{
		Print(stdout, "OnBeginResults\n");

		return True;
	}

	//called from a single thread
	virtual Bool OnResult(void *pResult, void *pContext)
	{
		Print(stdout, "OnResult\n");

		return True;
	}

	//called from a single thread
	virtual Bool OnEndResults(void *pContext)
	{
		Print(stdout, "OnEndResults\n");

		return True;
	}

	virtual Bool OnError(const Status &status)
	{
		Print(stdout, "OnError\n");

		return True;
	}

	//new 
	virtual Bool OnFolder(const WChar *wszPath)
	{
		Print(stdout, "OnFolder : {1}\n", wszPath);

		return True;
	}

	//new 
	virtual Bool OnBeginPath(const WChar *wszPath, Size cIndex)
	{
		Print(stdout, "OnBeginPath : {1}\n", wszPath);

		return True;
	}

	//new 
	virtual Bool OnEndPath(const WChar *wszPath, Size cIndex)
	{
		Print(stdout, "OnEndPath : {1}\n", wszPath);

		return True;
	}

	//new 
	virtual Bool OnListLineError(const WChar *wszLine, Size cPathIndex, const Status &status)
	{
		Print(stdout, "OnListLineError : {1}\n", wszLine);

		return True;
	}

};

int main(int argc, char *argv[])
{
	CX_UNUSED(argc);
	CX_UNUSED(argv);

	//Tester::Run();

	/*MyEnumHandler                 handler;
	IO::FileEnumerator2::Config   config = IO::FileEnumerator2::Config::GetDefault();

	config.nEnumFlags = IO::FileEnumerator2::Config::Flag_EnumNoReparsePoints | 
	                    IO::FileEnumerator2::Config::Flag_EnumNoOffline | 
	                    IO::FileEnumerator2::Config::Flag_EnumNoRecallOnDataAccess | 
	                    IO::FileEnumerator2::Config::Flag_EnumNoRecallOnOpen;
	IO::FileEnumerator2::Run(L"J:", &handler, config);
	*/

	return 0;
}

