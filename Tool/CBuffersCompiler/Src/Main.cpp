/* 
 * CX - C++ framework for general purpose development
 *
 * https://github.com/draede/cx
 * 
 * Copyright (C) 2014 - 2017 draede, draede [at] outlook [dot] com
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
 
#include "CX/CBuffers/Tools/Parser.hpp"
#include "CX/CBuffers/Tools/ParserData.hpp"
#include "CX/CBuffers/Tools/C_Generator.hpp"
#include "CX/CBuffers/Buffer.h"
#include "CX/CBuffers/CLibEnvironment.h"
#include "CX/Print.hpp"


using namespace CX;


typedef Map<String, CBuffers::Tools::IGenerator *>::Type   GeneratorsMap;


void Usage(const GeneratorsMap &mapGenerators)
{
	Print(stdout, "USAGE: cbc <language> <input_proto_file> <output_path>\n");
	Print(stdout, "Languages:\n");
	for (auto iter = mapGenerators.begin(); iter != mapGenerators.end(); ++iter)
	{
		Print(stdout, "   -{1} = {2}\n", iter->second->GetID(), iter->second->GetName());
	}
}

int main(int argc, char *argv[])
{
	CBuffers::Tools::C_Generator   gen_c;
	GeneratorsMap                  mapGenerators;
	GeneratorsMap::iterator        iterGenerators;
	CBuffers::Tools::StructsMap    mapStructs;
	Status                         status;

	mapGenerators[gen_c.GetID()] = &gen_c;

	if (4 != argc)
	{
		Usage(mapGenerators);

		return 1;
	}

	if ('-' != *argv[1])
	{
		Print(stdout, "Invalid language '{1}'\n", argv[1]);

		return 1;
	}
	if (mapGenerators.end() == (iterGenerators = mapGenerators.find(argv[1] + 1)))
	{
		Print(stdout, "Unknown language '{1}'\n", argv[1]);

		return 2;
	}
	if (!(status = CBuffers::Tools::Parser::Parse(argv[2], mapStructs)))
	{
		Print(stdout, "Failed to parse '{1}' : {2}\n", argv[2], status.GetMsg());

		return 3;
	}
	if (!(status =  CBuffers::Tools::IGenerator::PrepareStructs(mapStructs)))
	{
		Print(stdout, "Failed to prepare structs\n");

		return 4;
	}
	if (!(status = iterGenerators->second->Generate(mapStructs, argv[3])))
	{
		Print(stdout, "Failed to generate structs : {1}\n", status.GetMsg());

		return 4;
	}

	return 0;
}

