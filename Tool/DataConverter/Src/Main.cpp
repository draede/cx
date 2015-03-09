
#include "CX/Data/JSON/DataReader.hpp"
#include "CX/Data/JSON/DataWriter.hpp"
#include "CX/Data/BINJSON/DataReader.hpp"
#include "CX/Data/BINJSON/DataWriter.hpp"
#include "CX/IO/FileInputStream.hpp"
#include "CX/IO/FileOutputStream.hpp"
#include "CX/IO/Helper.hpp"
#include "CX/Print.hpp"
#include "CX/C/string.h"


typedef CX::IO::IDataReader * (* DataReaderFactory)(CX::IO::IInputStream *pInputStream);

typedef CX::IO::IDataWriter * (*DataWriterFactory)(CX::IO::IOutputStream *pOutputStream);


typedef struct _DataHandler
{
	const CX::Char      *szName;
	const CX::Char      *szDesc;
	DataReaderFactory   pfnDataReaderFactory;
	DataWriterFactory   pfnDataWriterFactory;
}DataHandler;


CX::IO::IDataReader *BINJSON_DataReader(CX::IO::IInputStream *pInputStream)
{
	return CX::New<CX::Data::BINJSON::DataReader>(pInputStream);
}

CX::IO::IDataWriter *BINJSON_DataWriter(CX::IO::IOutputStream *pOutputStream)
{
	return CX::New<CX::Data::BINJSON::DataWriter>(pOutputStream);
}

CX::IO::IDataReader *JSON_DataReader(CX::IO::IInputStream *pInputStream)
{
	return CX::New<CX::Data::JSON::DataReader>(pInputStream);
}

CX::IO::IDataWriter *JSON_DataWriter(CX::IO::IOutputStream *pOutputStream)
{
	return CX::New<CX::Data::JSON::DataWriter>(pOutputStream);
}


static const DataHandler HANDLERS[] = 
{
	{ "json", "text json", &JSON_DataReader, &JSON_DataWriter },
	{ "binjson", "binary json", &BINJSON_DataReader, &BINJSON_DataWriter },
};

static const CX_Size HANDLERS_COUNT = sizeof(HANDLERS) / sizeof(HANDLERS[0]);


void Usage()
{
	CX::Print(stdout, "USAGE: dataconv <input_type> <input_file> <output_type> <output_file>\n");
	CX::Print(stdout, "TYPES:\n");
	for (CX::Size i = 0; i < HANDLERS_COUNT; i++)
	{
		CX::Print(stdout, "      - '{1}'- {2}\n", HANDLERS[i].szName, HANDLERS[i].szDesc);
	}
}

const DataHandler *GetTypeData(const CX::Char *szName)
{
	for (CX::Size i = 0; i < HANDLERS_COUNT; i++)
	{
		if (0 == cx_stricmp(HANDLERS[i].szName, szName))
		{
			return &HANDLERS[i];
		}
	}

	return NULL;
}

int main(int argc, char *argv[])
{
	if (5 > argc)
	{
		Usage();

		return 1;
	}

	const DataHandler *pInp = GetTypeData(argv[1]);
	const DataHandler *pOut = GetTypeData(argv[3]);

	if (NULL == pInp)
	{
		CX::Print(stdout, "Invalid input type '{1}'\n", argv[1]);

		return 2;
	}
	if (NULL == pOut)
	{
		CX::Print(stdout, "Invalid output type '{1}'\n", argv[3]);

		return 3;
	}

	CX::IO::FileInputStream  inpstr(argv[2]);
	CX::IO::FileOutputStream outstr(argv[4]);

	if (!inpstr.IsOK())
	{
		CX::Print(stdout, "Failed to open input file '{1}'\n", argv[2]);

		return 4;
	}
	if (!outstr.IsOK())
	{
		CX::Print(stdout, "Failed to create output file '{1}'\n", argv[4]);

		return 5;
	}

	CX::IO::IDataReader *pReader = pInp->pfnDataReaderFactory(&inpstr);

	if (NULL == pReader)
	{
		CX::Print(stdout, "Failed to create data reader with type '{1}' and file '{2}'\n", 
		          argv[1], argv[2]);

		return 6;
	}

	CX::IO::IDataWriter *pWriter = pOut->pfnDataWriterFactory(&outstr);

	if (NULL == pWriter)
	{
		CX::Print(stdout, "Failed to create data writer with type '{1}' and file '{2}'\n", 
		          argv[3], argv[4]);

		return 7;
	}

	CX::Status status;

	if ((status = CX::IO::Helper::CopyData(pReader, pWriter)).IsNOK())
	{
		CX::Print(stdout, "Failed to copy data with code '{1}' and msg '{2}'\n", status.GetCode(), status.GetMsg());
		CX::Delete(pWriter);
		CX::Delete(pReader);

		return 8;
	}

	CX::Delete(pWriter);
	CX::Delete(pReader);

	return 0;
}

