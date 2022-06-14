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
 
#include "CX/Data/JSON/SAXParser.hpp"
#include "CX/Data/JSON/DumpSAXParserObserver.hpp"
#include "CX/IO/FileOutputStream.hpp"
#include "CX/Print.hpp"
#include "CX/C/stdio.h"


using namespace CX;



void Usage()
{
	Print(stdout, "USAGE: JSONDump <input_json> <output_json>\n");
}

int main(int argc, char *argv[])
{
	if (3 != argc)
	{
		Usage();

		return 1;
	}

	Data::JSON::SAXParser               parser;
	IO::FileOutputStream                fos(argv[2]);

	if (!fos.IsOK())
	{
		Print(stdout, "Failed to create '{1}'\n", argv[2]);

		return 2;
	}

	Data::JSON::DumpSAXParserObserver   obs(&fos);
	Status                              status;

	if (!(status = parser.SetIncludeEntryName()))
	{
		Print(stdout, "Failed to set include entry name : {1} ({2})\n", status.GetCode(), status.GetMsg());
#pragma warning(push)
#pragma warning(disable: 4996)
		unlink(argv[2]);
#pragma warning(pop)

		return 3;
	}
	if (!(status = parser.AddObserver(&obs)))
	{
		Print(stdout, "Failed to add observer : {1} ({2})\n", status.GetCode(), status.GetMsg());
#pragma warning(push)
#pragma warning(disable: 4996)
		unlink(argv[2]);
#pragma warning(pop)

		return 4;
	}
	if (!(status = parser.ParseStream(argv[1])))
	{
		Print(stdout, "Failed to parse '{1}' : {2} ({3})\n", argv[1], status.GetCode(), status.GetMsg());
#pragma warning(push)
#pragma warning(disable: 4996)
		unlink(argv[2]);
#pragma warning(pop)

		return 5;
	}

	return 0;
}

