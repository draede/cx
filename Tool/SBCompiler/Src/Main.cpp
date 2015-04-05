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

#include "CX/SB/Parser.hpp"
#include "CX/SB/Generator.hpp"
#include "CX/IO/FileInputStream.hpp"
#include "CX/Print.hpp"
#include "CX/C/Platform/Windows/windows.h"


using namespace CX;


void Usage()
{
	Print(stdout, "USAGE: sbc <input_proto_file(s)> [<output_path>]\n");
}

int HandleFile(const Char *szPath, const Char *szOutPath)
{
	IO::FileInputStream    fis(szPath);
	SB::Parser             parser;
	SB::Parser::Data       data;
	const Char             *pPos;
	Status                 status;

	if (!fis.IsOK())
	{
		Print(stdout, "Failed to open '{1}'\n", szPath);
	}
	if ((status = parser.Parse(&fis, data)).IsNOK())
	{
		Print(stdout, "Parse ('{1}') : error {2}, message \"{3}\"\n", szPath, status.GetCode(), status.GetMsg());

		return 2;
	}

	for (SB::ObjectsMap::const_iterator iter = data.mapObjects.begin(); iter != data.mapObjects.end(); ++iter)
	{
		String sOutPath;

		sOutPath = szOutPath;
		if (!sOutPath.empty() && '\\' != *sOutPath.rbegin() && '/' != *sOutPath.rbegin())
		{
			sOutPath += "\\";
		}

		pPos = cx_strrchr(iter->first.c_str(), '.');

		if (NULL == pPos)
		{
			sOutPath += iter->first;
		}
		else
		{
			sOutPath += pPos + 1;
		}

		sOutPath += ".hpp";

		if ((status = SB::Generator::GenerateWithPath(iter->second, sOutPath.c_str())).IsNOK())
		{
			Print(stdout, "Generate ('{1}') : error {2}, message \"{3}\"\n", iter->first, status.GetCode(), status.GetMsg());

			return 3;
		}
	}
	
	return 0;
}

void SearchFiles(const Char *szMask, const Char *szOutPath)
{
	WIN32_FIND_DATA   data;
	HANDLE            hFind;
	String            sBasePath;
	const Char        *pPos;

	pPos = cx_strrchr(szMask, '\\');
	if (NULL == pPos)
	{
		pPos = cx_strrchr(szMask, '/');
	}
	if (NULL != pPos)
	{
		sBasePath.assign(szMask, pPos - szMask);
		sBasePath += "\\";
	}
	if (INVALID_HANDLE_VALUE != (hFind = FindFirstFile(szMask, &data)))
	{
		for (;;)
		{
			String sPath = sBasePath;

			sPath += data.cFileName;
			HandleFile(sPath.c_str(), szOutPath);
			if (!FindNextFile(hFind, &data))
			{
				break;
			}
		}
		FindClose(hFind);
	}
}


int main(int argc, char *argv[])
{
	if (2 > argc)
	{
		Usage();

		return 1;
	}
	SearchFiles(argv[1], 3 <= argc ? argv[2] : "");

	return 0;
}

