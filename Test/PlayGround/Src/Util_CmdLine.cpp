/* 
 * CX - C++ framework for general purpose development
 *
 * https://github.com/draede/cx
 * 
 * Copyright (C) 2014 - 2019 draede - draede [at] outlook [dot] com
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

#include "CX/CLI/CmdLine.hpp"
#include "Tester.hpp"
#include "CX/Print.hpp"


using namespace CX;


void Util_CmdLine_Test1()
{
	CLI::CmdLine   cmdline;
	Status         status;

	cmdline.SetBanner("USAGE: a.exe")
	       .SetMinPositionalParams(1)
	       .AddOption('a', "aaa", "a option")
	       .AddOption('b', "bbb", "b option")
	       .AddOption('c', "ccc", "c option")
	       .AddOption('d', "ddd", "d option")
	       .AddParam(cmdline.ParamType_String, "path", "path param", True);

	cmdline.ShowUsage();

	status = cmdline.Parse("a.exe -abc --ddd --path \"xxx\\\"yyy\\\"zzz\" arg1 arg2", True);
}

REGISTER_TEST(Util_CmdLine_Test1);
