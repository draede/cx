
#include "CX/Archive/Parser.hpp"
#include "CX/Archive/CPPGenerator.hpp"
#include "CX/IO/FileOutputStream.hpp"
#include "CX/Print.hpp"


using namespace CX;


typedef Archive::IGenerator<IO::IOutputStream *> Generator;

typedef Generator * (* CreateGeneratorProc)();

struct GeneratorData
{
	String              sName;
	String              sDesc;
	CreateGeneratorProc pfnCreateGenerator;
};

Generator *CreateCPPGenerator()
{
	return new (std::nothrow) Archive::CPPGenerator<IO::IOutputStream *>();
}


GeneratorData GENERATOR_FACTORIES[] = 
{
	{ "cpp", "C++", &CreateCPPGenerator },
};

Size   GENERATOR_FACTORIES_COUNT = sizeof(GENERATOR_FACTORIES) / sizeof(GENERATOR_FACTORIES[0]);


void Usage()
{
	Print(stdout, "USAGE: ac <language> <input_proto_file> <output_lang_file>\n");
	Print(stdout, "Languages:\n");
	for (Size i = 0; i < GENERATOR_FACTORIES_COUNT; i++)
	{
		Print(stdout, "   {1} = {2}\n", GENERATOR_FACTORIES[i].sName, GENERATOR_FACTORIES[i].sDesc);
	}
}

int main(int argc, char *argv[])
{
	if (4 != argc)
	{
		Usage();

		return 1;
	}

	Archive::Object   object;
	GeneratorData     *pGeneratorData;
	Generator         *pGenerator;
	Status            status;

	pGeneratorData = NULL;
	for (Size i = 0; i < GENERATOR_FACTORIES_COUNT; i++)
	{
#pragma warning(push)
#pragma warning(disable: 4996)
		if (0 == cx_stricmp(GENERATOR_FACTORIES[i].sName.c_str(), argv[1]))
#pragma warning(pop)
		{
			pGeneratorData = &GENERATOR_FACTORIES[i];
			break;
		}
	}
	if (NULL == pGeneratorData)
	{
		Usage();

		return 2;
	}
	if (!(status = Archive::Parser::ParseFilePath(argv[2], object)))
	{
		Print(stdout, "Error {1} parsing {2} ({3})\n", status.GetCode(), argv[2], status.GetMsg());

		return 3;
	}
	if (NULL == (pGenerator = pGeneratorData->pfnCreateGenerator()))
	{
		Print(stdout, "Failed to create generator {1}\n", pGeneratorData->sName);

		return 4;
	}

	IO::FileOutputStream  fos(argv[3]);

	if (!fos.IsOK())
	{
		Print(stdout, "Failed to create output file {1}\n", argv[3]);

		return 5;
	}
	if (!(status = pGenerator->Generate(&fos, object)))
	{
		Print(stdout, "Error {1} generating output file ({2})\n", status.GetCode(), status.GetMsg());
		delete pGenerator;

		return 6;
	}
	delete pGenerator;

	return 0;
}

