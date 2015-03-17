
#include "CX/String.hpp"
#include "CX/IO/FileInputStream.hpp"
#include "CX/IO/FileOutputStream.hpp"
#include "Struct.hpp"
#include "CX/C/stdlib.h"
#include <time.h>



CX::Status GenerateStructName(CX::String *psName)
{
	static CX::Size   cIndex = 0;

	cIndex++;
	CX::Print(psName, "Struct{1}", cIndex);

	return CX::Status();
}

CX::Status GenerateFieldName(CX::SB::Struct *pStruct, CX::String *psName)
{
	CX::Print(psName, "field{1}", pStruct->m_vectorFields.size());

	return CX::Status();
}

CX::Status GenerateSimpleStruct(CX::SB::Struct *pStruct, CX::Size cMinFieldsCount, CX::Size cMaxFieldsCount)
{
	CX::Size cFieldsCount = cMinFieldsCount + rand() % (cMaxFieldsCount - cMinFieldsCount + 1);

	GenerateStructName(&pStruct->m_sName);
	pStruct->m_vectorFields.clear();
	for (CX::Size i = 0; i < cFieldsCount; i++)
	{
		CX::SB::Field field;

		GenerateFieldName(pStruct, &field.m_sName);
	}

	return CX::Status();
}

CX::Status WriteStruct(CX::SB::Struct *pStruct, const CX::Char *szPath)
{


	return CX::Status();
}




int main(int argc, char *argv[])
{
	srand(time(NULL));

	return 0;
}

