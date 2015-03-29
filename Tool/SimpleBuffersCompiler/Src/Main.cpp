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

#include "CX/IO/SimpleBuffers/ProtoParser.hpp"
#include "CX/IO/SimpleBuffers/Generator.hpp"
#include "CX/Print.hpp"


using namespace CX;


void Usage()
{
	Print(stdout, "USAGE: sbc <input_proto_file> [<output_dir>]\n");
}


int main(int argc, char *argv[])
{
	if (2 > argc)
	{
		Usage();

		return 1;
	}

	IO::SimpleBuffers::ProtoParser parser;
	Status                         status;

	if ((status = parser.BeginParse()).IsNOK())
	{
		Print(stdout, "Failed to begin parsing : error {1}, message \"{2}\"\n", status.GetCode(), status.GetMsg());

		return 2;
	}
	if ((status = parser.Parse(argv[1])).IsNOK())
	{
		Print(stdout, "Parse failed : error {1}, message \"{2}\"\n", status.GetCode(), status.GetMsg());

		return 3;
	}
	if ((status = parser.EndParse()).IsNOK())
	{
		Print(stdout, "Failed to end parsing : error {1}, message \"{2}\"\n", status.GetCode(), status.GetMsg());

		return 4;
	}

	for (IO::SimpleBuffers::ObjectsMap::const_iterator iter = parser.GetObjects().begin(); 
	     iter != parser.GetObjects().end(); ++iter)
	{
		String sPath;

		if (3 <= argc)
		{
			sPath += argv[2];
			if (!sPath.empty() && '\\' != *sPath.rbegin() && '/' != *sPath.rbegin())
			{
				sPath += "\\";
			}
		}

		const Char *pPos = cx_strrchr(iter->first.c_str(), '.');

		if (NULL == pPos)
		{
			sPath += iter->first;
		}
		else
		{
			sPath += pPos + 1;
		}

		sPath += ".hpp";

		if ((status = IO::SimpleBuffers::Generator::GenerateCPPWithPath(&iter->second, sPath.c_str())).IsNOK())
		{
			Print(stdout, "Generate({3}) : error {1}, message \"{2}\"\n", status.GetCode(), status.GetMsg(), iter->first);

			return 5;
		}
	}

	return 0;
}

