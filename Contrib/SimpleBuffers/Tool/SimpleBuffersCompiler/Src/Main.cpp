
#include "CX/String.hpp"
#include "Parser.hpp"
#include "Generator.hpp"


void Usage()
{
	CX::Print(stdout, "USAGE: sbc <sb_file> <output_path>\n");
}

int main(int argc, char *argv[])
{
	if (3 > argc)
	{
		Usage();

		return 1;
	}

	CX::SB::Parser   parser;
	CX::Status       status;

	if ((status = parser.BeginParse()).IsNOK())
	{
		CX::Print(stdout, "Begin parse failed with code '{1}' and msg '{2}'\n", status.GetCode(), status.GetMsg());

		return 2;
	}
	if ((status = parser.Parse(argv[1])).IsNOK())
	{
		CX::Print(stdout, "Parse failed with code '{1}' and msg '{2}'\n", status.GetCode(), status.GetMsg());

		return 3;
	}
	if ((status = parser.EndParse()).IsNOK())
	{
		CX::Print(stdout, "End parse failed with code '{1}' and msg '{2}'\n", status.GetCode(), status.GetMsg());

		return 4;
	}

	const CX::SB::StructsMap &mapStructs = parser.GetStructs();

	for (CX::SB::StructsMap::const_iterator iter = mapStructs.begin(); iter != mapStructs.end(); ++iter)
	{
		{
			CX::String          sPath;
			CX::SB::Generator   generator;

			CX::Print(&sPath, "{1}\\{2}.h", iter->second.m_sName);

			if ((status = generator.GenerateHeaderWithPath(sPath.c_str(), &iter->second)).IsNOK())
			{
				CX::Print(stdout, "Generate '{1}' failed with code '{2}' and msg '{3}'\n", sPath, status.GetCode(), status.GetMsg());
			}
		}
		{
			CX::String          sPath;
			CX::SB::Generator   generator;

			CX::Print(&sPath, "{1}\\{2}.c", iter->second.m_sName);

			if ((status = generator.GenerateSourceWithPath(sPath.c_str(), &iter->second)).IsNOK())
			{
				CX::Print(stdout, "Generate '{1}' failed with code '{2}' and msg '{3}'\n", sPath, status.GetCode(), status.GetMsg());
			}
		}
	}

	return 0;
}

