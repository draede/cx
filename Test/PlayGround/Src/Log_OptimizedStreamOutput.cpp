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

#include "CX/Log/Logger.hpp"
#include "CX/Log/OptimizedStreamOutput.hpp"
#include "CX/Print.hpp"
#include "Tester.hpp"


using namespace CX;


void Log_OptimizedStreamOutput_Test()
{
	Print(stdout, "Begin Log OptimizedStreamOutput test\n");

	Log::Logger::GetDefaultLogger().AddOutput(new (std::nothrow) Log::OptimizedStreamOutput("test.log"));
	Log::Logger::GetDefaultLogger().SetLevel(Log::Level_Debug);

	for (Size i = 0; i < 1000000; i++)
	{
		CXINFO("Some tag", "this is test #{1}", i + 1);
	}
}

REGISTER_TEST(Log_OptimizedStreamOutput_Test);
