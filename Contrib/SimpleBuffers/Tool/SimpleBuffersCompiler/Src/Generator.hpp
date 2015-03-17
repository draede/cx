
#pragma once


#include "Struct.hpp"
#include "CX/IO/FileOutputStream.hpp"
#include "CX/Set.hpp"
#include "CX/Print.hpp"


namespace CX
{

namespace SB
{


class Generator
{
public:

	Status GenerateHeaderWithPath(const Char *szPath, const Struct *pStruct)
	{
		IO::FileOutputStream fos(szPath);

		if (!fos.IsOK())
		{
			return Status(Status_CreateFailed, "Failed to create file '{1}'", szPath);
		}

		return GenerateHeader((IO::IOutputStream *)&fos, pStruct);
	}

	template <typename OUTPUT>
	Status GenerateHeader(OUTPUT out, const Struct *pStruct)
	{
		Print(out, "\n");
		Print(out, "#pragma once\n");
		Print(out, "\n");
		Print(out, "\n");
		Print(out, "#ifdef __cplusplus\n");
		Print(out, "extern \"C\" {{\n");
		Print(out, "#endif\n");
		Print(out, "\n");
		Print(out, "\n");
		Print(out, "#include \"SBAPI.h\"\n");

		Set<String>::Type  setHeaders;

		//includes from structs
		for (FieldsVector::const_iterator iter = pStruct->m_vectorFields.begin(); iter != pStruct->m_vectorFields.end(); ++iter)
		{
			if (Field::Type_Struct == iter->m_nType)
			{
				if (setHeaders.end() == setHeaders.find(iter->m_sStructName))
				{
					setHeaders.insert(iter->m_sStructName);
					Print(out, "#include \"{1}.h\"\n", iter->m_sStructName);
				}
			}
		}

		Print(out, "\n");
		Print(out, "\n");

		//struct def
		Print(out, "typedef struct _{1}\n", pStruct->m_sName);
		Print(out, "{{\n");
		for (FieldsVector::const_iterator iter = pStruct->m_vectorFields.begin(); iter != pStruct->m_vectorFields.end(); ++iter)
		{
			Print(out, "\t{1} {2};\n", Field::GetStringFromType(iter->m_nType, iter->m_bIsVector, iter->m_sStructName.c_str()), iter->m_sName);
		}
		Print(out, "}{1};\n", pStruct->m_sName);
		Print(out, "\n");

		//struct functions
		Print(out, "CX_StatusCode {1}_Init({1} *pStruct, CX_SB_Memory *pMem);   // init pointers and stuff...\n", pStruct->m_sName);
		Print(out, "\n");
		Print(out, "CX_StatusCode {1}_Uninit({1} *pStruct, CX_SB_Memory *pMem); // free pointers and stuff...\n", pStruct->m_sName);
		Print(out, "\n");
		Print(out, "CX_StatusCode {1}_Write({1} *pStruct, CX_SB_DataWriter *pWriter, CX_SB_Memory *pMem);\n", pStruct->m_sName);
		Print(out, "\n");
		Print(out, "CX_StatusCode {1}_Read({1} *pStruct, CX_SB_DataReader *pReader, CX_SB_Memory *pMem);\n", pStruct->m_sName);
		Print(out, "\n");
		Print(out, "typedef struct _{1}Array\n", pStruct->m_sName);
		Print(out, "{{\n");
		Print(out, "	{1}   *values;\n", pStruct->m_sName);
		Print(out, "	CX_Size         cUsedCount;\n");
		Print(out, "	CX_Size         cTotalCount;\n");
		Print(out, "}{1}Array;\n", pStruct->m_sName);
		Print(out, "\n");
		Print(out, "CX_StatusCode {1}Array_Init({1}Array *pArray, CX_SB_Memory *pMem);\n", pStruct->m_sName);
		Print(out, "\n");
		Print(out, "CX_StatusCode {1}Array_Uninit({1}Array *pArray, CX_SB_Memory *pMem);\n", pStruct->m_sName);
		Print(out, "\n");
		Print(out, "CX_Size {1}Array_GetCount({1}Array *pArray, CX_SB_Memory *pMem);\n", pStruct->m_sName);
		Print(out, "\n");
		Print(out, "{1} *{1}Array_Get({1}Array *pArray, CX_Size cIndex, CX_SB_Memory *pMem);\n", pStruct->m_sName);
		Print(out, "\n");
		Print(out, "{1} *{1}Array_Add({1}Array *pArray, CX_SB_Memory *pMem);\n", pStruct->m_sName);
		Print(out, "\n");
		Print(out, "CX_StatusCode {1}Array_Remove({1}Array *pArray, CX_Size cIndex, CX_Size cCount, CX_SB_Memory *pMem);\n", pStruct->m_sName);
		Print(out, "\n");
		Print(out, "CX_StatusCode {1}Array_FreeUnusedMem({1}Array *pArray, CX_SB_Memory *pMem);\n", pStruct->m_sName);
		Print(out, "\n");
		Print(out, "CX_StatusCode {1}Array_Reserve({1}Array *pArray, CX_Size cTotalCount, CX_SB_Memory *pMem);\n", pStruct->m_sName);
		Print(out, "\n");

		Print(out, "\n");
		Print(out, "\n");
		Print(out, "#ifdef __cplusplus\n");
		Print(out, "}\n");
		Print(out, "#endif\n");
		Print(out, "\n");

		return Status();
	}

	Status GenerateSourceWithPath(const Char *szPath, const Struct *pStruct)
	{
		IO::FileOutputStream fos(szPath);

		if (!fos.IsOK())
		{
			return Status(Status_CreateFailed, "Failed to create file '{1}'", szPath);
		}

		return GenerateSource((IO::IOutputStream *)&fos, pStruct);
	}

	template <typename OUTPUT>
	Status GenerateSource(OUTPUT out, const Struct *pStruct)
	{
		Print(out, "\n");
		Print(out, "#include \"{1}.h\"\n", pStruct->m_sName);
		Print(out, "\n");
		Print(out, "\n");

		Print(out, "CX_StatusCode {1}_Init({1} *pStruct, CX_SB_Memory *pMem)\n", pStruct->m_sName);
		Print(out, "{{\n");
		for (FieldsVector::const_iterator iter = pStruct->m_vectorFields.begin(); iter != pStruct->m_vectorFields.end(); ++iter)
		{
			if (!iter->m_bIsVector)
			{
				if (Field::Type_String == iter->m_nType)
				{
					Print(out, "\tCX_SB_String_Init(&pStruct->{1}, pMem);\n", iter->m_sName);
				}
				else
				if (Field::Type_WString == iter->m_nType)
				{
					Print(out, "\tCX_SB_WString_Init(&pStruct->{1}, pMem);\n", iter->m_sName);
				}
				else
				if (Field::Type_Struct == iter->m_nType)
				{
					Print(out, "\t{1}_Init(&pStruct->{2}, pMem);\n", iter->m_sStructName, iter->m_sName);
				}
			}
			else
			{
				Print(out, "\t{1}_Init(&pStruct->{2}, pMem);\n", Field::GetStringFromType(iter->m_nType, true, iter->m_sStructName.c_str()), iter->m_sName);
			}
		}
		Print(out, "\n");
		Print(out, "\treturn CX_Status_OK;\n");
		Print(out, "}\n");
		Print(out, "\n");

		Print(out, "CX_StatusCode {1}_Uninit({1} *pStruct, CX_SB_Memory *pMem)\n", pStruct->m_sName);
		Print(out, "{{\n");
		for (FieldsVector::const_iterator iter = pStruct->m_vectorFields.begin(); iter != pStruct->m_vectorFields.end(); ++iter)
		{
			if (!iter->m_bIsVector)
			{
				if (Field::Type_String == iter->m_nType)
				{
					Print(out, "\tCX_SB_String_Uninit(&pStruct->{1}, pMem);\n", iter->m_sName);
				}
				else
				if (Field::Type_WString == iter->m_nType)
				{
					Print(out, "\tCX_SB_WString_Uninit(&pStruct->{1}, pMem);\n", iter->m_sName);
				}
				else
				if (Field::Type_Struct == iter->m_nType)
				{
					Print(out, "\t{1}_Uninit(&pStruct->{2}, pMem);\n", iter->m_sStructName, iter->m_sName);
				}
			}
			else
			{
				Print(out, "\t{1}_Uninit(&pStruct->{2}, pMem);\n", Field::GetStringFromType(iter->m_nType, true, iter->m_sStructName.c_str()), iter->m_sName);
			}
		}
		Print(out, "\n");
		Print(out, "\treturn CX_Status_OK;\n");
		Print(out, "}\n");
		Print(out, "\n");

		Print(out, "CX_StatusCode {1}_Write({1} *pStruct, CX_SB_DataWriter *pWriter, CX_SB_Memory *pMem)\n", pStruct->m_sName);
		Print(out, "{{\n");
		for (FieldsVector::const_iterator iter = pStruct->m_vectorFields.begin(); iter != pStruct->m_vectorFields.end(); ++iter)
		{
			if (iter->m_bIsVector)
			{
				Print(out, "\tCX_Size cIndex;\n");

				break;
			}
		}
		Print(out, "\tCX_StatusCode nStatus;\n");
		Print(out, "\n");

		bool bNeedMem = false;
		for (FieldsVector::const_iterator iter = pStruct->m_vectorFields.begin(); iter != pStruct->m_vectorFields.end(); ++iter)
		{
			if (!iter->m_bIsVector)
			{
				if (Field::Type_Struct == iter->m_nType)
				{
					bNeedMem = true;
					break;
				}
			}
			else
			{
				bNeedMem = true;
				break;
			}
		}
		if (!bNeedMem)
		{
			Print(out, "\tpMem;\n");
			Print(out, "\n");
		}

		for (FieldsVector::const_iterator iter = pStruct->m_vectorFields.begin(); iter != pStruct->m_vectorFields.end(); ++iter)
		{
			if (!iter->m_bIsVector)
			{
				switch (iter->m_nType)
				{
					case Field::Type_Bool:
					{
						Print(out, "\tif (CXNOK(nStatus = pWriter->StructWriteBool(pWriter->pContext, \"{1}\", pStruct->{1})))\n", iter->m_sName);
						Print(out, "\t{{\n");
						Print(out, "\t\treturn nStatus;\n");
						Print(out, "\t}\n");
					}
					break;
					case Field::Type_Int8:
					case Field::Type_UInt8:
					case Field::Type_Int16:
					case Field::Type_UInt16:
					case Field::Type_Int32:
					case Field::Type_UInt32:
					case Field::Type_Int64:
					case Field::Type_UInt64:
					{
						Print(out, "\tif (CXNOK(nStatus = pWriter->StructWriteInt(pWriter->pContext, \"{1}\", (CX_Int64)pStruct->{1})))\n", iter->m_sName);
						Print(out, "\t{{\n");
						Print(out, "\t\treturn nStatus;\n");
						Print(out, "\t}\n");
					}
					break;
					case Field::Type_Float:
					case Field::Type_Double:
					{
						Print(out, "\tif (CXNOK(nStatus = pWriter->StructWriteReal(pWriter->pContext, \"{1}\", (CX_Double)pStruct->{1})))\n", iter->m_sName);
						Print(out, "\t{{\n");
						Print(out, "\t\treturn nStatus;\n");
						Print(out, "\t}\n");
					}
					break;
					case Field::Type_String:
					{
						Print(out, "\tif (CXNOK(nStatus = pWriter->StructWriteString(pWriter->pContext, \"{1}\", pStruct->{1})))\n", iter->m_sName);
						Print(out, "\t{{\n");
						Print(out, "\t\treturn nStatus;\n");
						Print(out, "\t}\n");
					}
					break;
					case Field::Type_WString:
					{
						Print(out, "\tif (CXNOK(nStatus = pWriter->StructWriteWString(pWriter->pContext, \"{1}\", pStruct->{1})))\n", iter->m_sName);
						Print(out, "\t{{\n");
						Print(out, "\t\treturn nStatus;\n");
						Print(out, "\t}\n");
					}
					break;
					case Field::Type_Struct:
					{
						Print(out, "\tif (CXNOK(nStatus = pWriter->StructBeginStruct(pWriter->pContext, \"{1}\")))\n", iter->m_sName);
						Print(out, "\t{{\n");
						Print(out, "\t\treturn nStatus;\n");
						Print(out, "\t}\n");

						Print(out, "\tif (CXNOK(nStatus = {2}_Write(&pStruct->{1}, pWriter, pMem)))\n", iter->m_sName, iter->m_sStructName);
						Print(out, "\t{{\n");
						Print(out, "\t\treturn nStatus;\n");
						Print(out, "\t}\n");

						Print(out, "\tif (CXNOK(nStatus = pWriter->StructEndStruct(pWriter->pContext)))\n");
						Print(out, "\t{{\n");
						Print(out, "\t\treturn nStatus;\n");
						Print(out, "\t}\n");
					}
					break;
				}
			}
			else
			{
				Print(out, "\tif (CXNOK(nStatus = pWriter->StructBeginArray(pWriter->pContext, \"{1}\")))\n", iter->m_sName);
				Print(out, "\t{{\n");
				Print(out, "\t\treturn nStatus;\n");
				Print(out, "\t}\n");
				Print(out, "\tfor (cIndex = 0; cIndex < pStruct->{1}.cUsedCount; cIndex++)\n", iter->m_sName);
				Print(out, "\t{{\n");
				switch (iter->m_nType)
				{
					case Field::Type_Bool:
					{
						Print(out, "\t\tif (CXNOK(nStatus = pWriter->ArrayWriteBool(pWriter->pContext, {2}_Get(&pStruct->{1}, cIndex, CX_False, pMem))))\n", iter->m_sName, Field::GetStringFromType(iter->m_nType, iter->m_bIsVector, iter->m_sStructName.c_str()));
						Print(out, "\t\t{{\n");
						Print(out, "\t\t\treturn nStatus;\n");
						Print(out, "\t\t}\n");
					}
					break;
					case Field::Type_Int8:
					case Field::Type_UInt8:
					case Field::Type_Int16:
					case Field::Type_UInt16:
					case Field::Type_Int32:
					case Field::Type_UInt32:
					case Field::Type_Int64:
					case Field::Type_UInt64:
					{
						Print(out, "\t\tif (CXNOK(nStatus = pWriter->ArrayWriteInt(pWriter->pContext, (CX_Int64){2}_Get(&pStruct->{1}, cIndex, 0, pMem))))\n", iter->m_sName, Field::GetStringFromType(iter->m_nType, iter->m_bIsVector, iter->m_sStructName.c_str()));
						Print(out, "\t\t{{\n");
						Print(out, "\t\t\treturn nStatus;\n");
						Print(out, "\t\t}\n");
					}
					break;
					case Field::Type_Float:
					case Field::Type_Double:
					{
						Print(out, "\t\tif (CXNOK(nStatus = pWriter->ArrayWriteReal(pWriter->pContext, (CX_Double){2}_Get(&pStruct->{1}, cIndex, 0, pMem))))\n", iter->m_sName, Field::GetStringFromType(iter->m_nType, iter->m_bIsVector, iter->m_sStructName.c_str()));
						Print(out, "\t\t{{\n");
						Print(out, "\t\t\treturn nStatus;\n");
						Print(out, "\t\t}\n");
					}
					break;
					case Field::Type_String:
					{
						Print(out, "\t\tif (CXNOK(nStatus = pWriter->ArrayWriteString(pWriter->pContext, {2}_Get(&pStruct->{1}, cIndex, \"\", pMem))))\n", iter->m_sName, Field::GetStringFromType(iter->m_nType, iter->m_bIsVector, iter->m_sStructName.c_str()));
						Print(out, "\t\t{{\n");
						Print(out, "\t\t\treturn nStatus;\n");
						Print(out, "\t\t}\n");
					}
					break;
					case Field::Type_WString:
					{
						Print(out, "\t\tif (CXNOK(nStatus = pWriter->ArrayWriteWString(pWriter->pContext, {2}_Get(&pStruct->{1}, cIndex, L\"\", pMem))))\n", iter->m_sName, Field::GetStringFromType(iter->m_nType, iter->m_bIsVector, iter->m_sStructName.c_str()));
						Print(out, "\t\t{{\n");
						Print(out, "\t\t\treturn nStatus;\n");
						Print(out, "\t\t}\n");
					}
					break;
					case Field::Type_Struct:
					{
						Print(out, "\t\tif (CXNOK(nStatus = pWriter->ArrayBeginStruct(pWriter->pContext)))\n");
						Print(out, "\t\t{{\n");
						Print(out, "\t\t\treturn nStatus;\n");
						Print(out, "\t\t}\n");
						Print(out, "\t\tif (CXNOK(nStatus = {3}_Write({2}_Get(&pStruct->{1}, cIndex, pMem), pWriter, pMem)))\n", iter->m_sName, Field::GetStringFromType(iter->m_nType, iter->m_bIsVector, iter->m_sStructName.c_str()), iter->m_sStructName);
						Print(out, "\t\t{{\n");
						Print(out, "\t\t\treturn nStatus;\n");
						Print(out, "\t\t}\n");
						Print(out, "\t\tif (CXNOK(nStatus = pWriter->ArrayEndStruct(pWriter->pContext)))\n");
						Print(out, "\t\t{{\n");
						Print(out, "\t\t\treturn nStatus;\n");
						Print(out, "\t\t}\n");
					}
					break;
				}
				Print(out, "\t}\n");
				Print(out, "\tif (CXNOK(nStatus = pWriter->StructEndArray(pWriter->pContext)))\n");
				Print(out, "\t{{\n");
				Print(out, "\t\treturn nStatus;\n");
				Print(out, "\t}\n");
			}
		}
		Print(out, "\n");
		Print(out, "\treturn CX_Status_OK;\n");
		Print(out, "}\n");
		Print(out, "\n");

		Print(out, "CX_StatusCode {1}_Read({1} *pStruct, CX_SB_DataReader *pReader, CX_SB_Memory *pMem)\n", pStruct->m_sName);
		Print(out, "{{\n");
		for (FieldsVector::const_iterator iter = pStruct->m_vectorFields.begin(); iter != pStruct->m_vectorFields.end(); ++iter)
		{
			if (iter->m_bIsVector && Field::Type_Bool == iter->m_nType)
			{
				Print(out, "\tCX_Bool bBoolVal;\n");

				break;
			}
		}
		for (FieldsVector::const_iterator iter = pStruct->m_vectorFields.begin(); iter != pStruct->m_vectorFields.end(); ++iter)
		{
			if (Field::Type_Int8 == iter->m_nType || 
			    Field::Type_UInt8 == iter->m_nType || 
			    Field::Type_Int16 == iter->m_nType || 
			    Field::Type_UInt16 == iter->m_nType || 
			    Field::Type_Int32 == iter->m_nType || 
			    Field::Type_UInt32 == iter->m_nType || 
			    Field::Type_Int64 == iter->m_nType || 
			    Field::Type_UInt64 == iter->m_nType)
			{
				Print(out, "\tCX_Int64 nIntVal;\n");

				break;
			}
		}
		for (FieldsVector::const_iterator iter = pStruct->m_vectorFields.begin(); iter != pStruct->m_vectorFields.end(); ++iter)
		{
			if (Field::Type_Float == iter->m_nType || 
			    Field::Type_Double == iter->m_nType)
			{
				Print(out, "\tCX_Double nRealVal;\n");

				break;
			}
		}
		for (FieldsVector::const_iterator iter = pStruct->m_vectorFields.begin(); iter != pStruct->m_vectorFields.end(); ++iter)
		{
			if (Field::Type_String == iter->m_nType)
			{
				Print(out, "\tCX_Char *szStrVal;\n");

				break;
			}
		}
		for (FieldsVector::const_iterator iter = pStruct->m_vectorFields.begin(); iter != pStruct->m_vectorFields.end(); ++iter)
		{
			if (Field::Type_WString == iter->m_nType)
			{
				Print(out, "\tCX_WChar *wszStrVal;\n");

				break;
			}
		}
		Print(out, "\tCX_StatusCode nStatus;\n");
		Print(out, "\n");

		bNeedMem = false;
		for (FieldsVector::const_iterator iter = pStruct->m_vectorFields.begin(); iter != pStruct->m_vectorFields.end(); ++iter)
		{
			if (!iter->m_bIsVector)
			{
				if (Field::Type_Struct == iter->m_nType)
				{
					bNeedMem = true;
					break;
				}
			}
			else
			{
				bNeedMem = true;
				break;
			}
		}
		if (!bNeedMem)
		{
			Print(out, "\tpMem;\n");
			Print(out, "\n");
		}

		for (FieldsVector::const_iterator iter = pStruct->m_vectorFields.begin(); iter != pStruct->m_vectorFields.end(); ++iter)
		{
			if (!iter->m_bIsVector)
			{
				switch (iter->m_nType)
				{
					case Field::Type_Bool:
					{
						Print(out, "\tif (CXNOK(nStatus = pReader->StructReadBool(pReader->pContext, \"{1}\", &pStruct->{1})))\n", iter->m_sName);
						Print(out, "\t{{\n");
						Print(out, "\t\treturn nStatus;\n");
						Print(out, "\t}\n");
					}
					break;
					case Field::Type_Int8:
					{
						Print(out, "\tif (CXNOK(nStatus = pReader->StructReadInt(pReader->pContext, \"{1}\", &nIntVal)))\n", iter->m_sName);
						Print(out, "\t{{\n");
						Print(out, "\t\treturn nStatus;\n");
						Print(out, "\t}\n");
						Print(out, "\tpStruct->{1} = (CX_Int8)nIntVal;\n", iter->m_sName);
					}
					break;
					case Field::Type_UInt8:
					{
						Print(out, "\tif (CXNOK(nStatus = pReader->StructReadInt(pReader->pContext, \"{1}\", &nIntVal)))\n", iter->m_sName);
						Print(out, "\t{{\n");
						Print(out, "\t\treturn nStatus;\n");
						Print(out, "\t}\n");
						Print(out, "\tpStruct->{1} = (CX_UInt8)nIntVal;\n", iter->m_sName);
					}
					break;
					case Field::Type_Int16:
					{
						Print(out, "\tif (CXNOK(nStatus = pReader->StructReadInt(pReader->pContext, \"{1}\", &nIntVal)))\n", iter->m_sName);
						Print(out, "\t{{\n");
						Print(out, "\t\treturn nStatus;\n");
						Print(out, "\t}\n");
						Print(out, "\tpStruct->{1} = (CX_Int16)nIntVal;\n", iter->m_sName);
					}
					break;
					case Field::Type_UInt16:
					{
						Print(out, "\tif (CXNOK(nStatus = pReader->StructReadInt(pReader->pContext, \"{1}\", &nIntVal)))\n", iter->m_sName);
						Print(out, "\t{{\n");
						Print(out, "\t\treturn nStatus;\n");
						Print(out, "\t}\n");
						Print(out, "\tpStruct->{1} = (CX_UInt16)nIntVal;\n", iter->m_sName);
					}
					break;
					case Field::Type_Int32:
					{
						Print(out, "\tif (CXNOK(nStatus = pReader->StructReadInt(pReader->pContext, \"{1}\", &nIntVal)))\n", iter->m_sName);
						Print(out, "\t{{\n");
						Print(out, "\t\treturn nStatus;\n");
						Print(out, "\t}\n");
						Print(out, "\tpStruct->{1} = (CX_Int32)nIntVal;\n", iter->m_sName);
					}
					break;
					case Field::Type_UInt32:
					{
						Print(out, "\tif (CXNOK(nStatus = pReader->StructReadInt(pReader->pContext, \"{1}\", &nIntVal)))\n", iter->m_sName);
						Print(out, "\t{{\n");
						Print(out, "\t\treturn nStatus;\n");
						Print(out, "\t}\n");
						Print(out, "\tpStruct->{1} = (CX_UInt32)nIntVal;\n", iter->m_sName);
					}
					break;
					case Field::Type_Int64:
					{
						Print(out, "\tif (CXNOK(nStatus = pReader->StructReadInt(pReader->pContext, \"{1}\", &nIntVal)))\n", iter->m_sName);
						Print(out, "\t{{\n");
						Print(out, "\t\treturn nStatus;\n");
						Print(out, "\t}\n");
						Print(out, "\tpStruct->{1} = (CX_Int64)nIntVal;\n", iter->m_sName);
					}
					break;
					case Field::Type_UInt64:
					{
						Print(out, "\tif (CXNOK(nStatus = pReader->StructReadInt(pReader->pContext, \"{1}\", &nIntVal)))\n", iter->m_sName);
						Print(out, "\t{{\n");
						Print(out, "\t\treturn nStatus;\n");
						Print(out, "\t}\n");
						Print(out, "\tpStruct->{1} = (CX_UInt64)nIntVal;\n", iter->m_sName);
					}
					break;
					case Field::Type_Float:
					{
						Print(out, "\tif (CXNOK(nStatus = pReader->StructReadReal(pReader->pContext, \"{1}\", &nRealVal)))\n", iter->m_sName);
						Print(out, "\t{{\n");
						Print(out, "\t\treturn nStatus;\n");
						Print(out, "\t}\n");
						Print(out, "\tpStruct->{1} = (CX_Float)nRealVal;\n", iter->m_sName);
					}
					break;
					case Field::Type_Double:
					{
						Print(out, "\tif (CXNOK(nStatus = pReader->StructReadReal(pReader->pContext, \"{1}\", &nRealVal)))\n", iter->m_sName);
						Print(out, "\t{{\n");
						Print(out, "\t\treturn nStatus;\n");
						Print(out, "\t}\n");
						Print(out, "\tpStruct->{1} = (CX_Double)nRealVal;\n", iter->m_sName);
					}
					break;
					case Field::Type_String:
					{
						Print(out, "\tif (CXNOK(nStatus = pReader->StructReadString(pReader->pContext, \"{1}\", &szStrVal)))\n", iter->m_sName);
						Print(out, "\t{{\n");
						Print(out, "\t\treturn nStatus;\n");
						Print(out, "\t}\n");
						Print(out, "\tif (CXNOK(nStatus = CX_SB_String_Init(&pStruct->{1}, pMem)))\n", iter->m_sName);
						Print(out, "\t{{\n");
						Print(out, "\t\tpReader->FreeString(pReader->pContext, szStrVal);\n");
						Print(out, "\n");
						Print(out, "\t\treturn nStatus;\n");
						Print(out, "\t}\n");
						Print(out, "\tif (CXNOK(nStatus = CX_SB_String_Set(&pStruct->{1}, szStrVal, pMem)))\n", iter->m_sName);
						Print(out, "\t{{\n");
						Print(out, "\t\tpReader->FreeString(pReader->pContext, szStrVal);\n");
						Print(out, "\t\tpMem->Free(pMem->pContext, pStruct->{1});\n", iter->m_sName);
						Print(out, "\t\tCX_SB_String_Uninit(&pStruct->{1}, pMem);\n", iter->m_sName);
						Print(out, "\t\treturn nStatus;\n");
						Print(out, "\t}\n");
						Print(out, "\tpReader->FreeString(pReader->pContext, szStrVal);\n");
					}
					break;
					case Field::Type_WString:
					{
						Print(out, "\tif (CXNOK(nStatus = pReader->StructReadWString(pReader->pContext, \"{1}\", &wszStrVal)))\n", iter->m_sName);
						Print(out, "\t{{\n");
						Print(out, "\t\treturn nStatus;\n");
						Print(out, "\t}\n");
						Print(out, "\tif (CXNOK(nStatus = CX_SB_WString_Init(&pStruct->{1}, pMem)))\n", iter->m_sName);
						Print(out, "\t{{\n");
						Print(out, "\t\tpReader->FreeWString(pReader->pContext, wszStrVal);\n");
						Print(out, "\n");
						Print(out, "\t\treturn nStatus;\n");
						Print(out, "\t}\n");
						Print(out, "\tif (CXNOK(nStatus = CX_SB_WString_Set(&pStruct->{1}, wszStrVal, pMem)))\n", iter->m_sName);
						Print(out, "\t{{\n");
						Print(out, "\t\tpReader->FreeWString(pReader->pContext, wszStrVal);\n");
						Print(out, "\t\tpMem->Free(pMem->pContext, pStruct->{1});\n", iter->m_sName);
						Print(out, "\t\tCX_SB_WString_Uninit(&pStruct->{1}, pMem);\n", iter->m_sName);
						Print(out, "\t\treturn nStatus;\n");
						Print(out, "\t}\n");
						Print(out, "\tpReader->FreeWString(pReader->pContext, wszStrVal);\n");
					}
					break;
					case Field::Type_Struct:
					{
						Print(out, "\tif (CXNOK(nStatus = pReader->StructBeginStruct(pReader->pContext, \"{1}\")))\n", iter->m_sName);
						Print(out, "\t{{\n");
						Print(out, "\t\treturn nStatus;\n");
						Print(out, "\t}\n");

						Print(out, "\tif (CXNOK(nStatus = {2}_Read(&pStruct->{1}, pReader, pMem)))\n", iter->m_sName, iter->m_sStructName);
						Print(out, "\t{{\n");
						Print(out, "\t\treturn nStatus;\n");
						Print(out, "\t}\n");

						Print(out, "\tif (CXNOK(nStatus = pReader->StructEndStruct(pReader->pContext)))\n");
						Print(out, "\t{{\n");
						Print(out, "\t\treturn nStatus;\n");
						Print(out, "\t}\n");
					}
					break;
				}
			}
			else
			{
				Print(out, "\tif (CXNOK(nStatus = pReader->StructBeginArray(pReader->pContext, \"{1}\")))\n", iter->m_sName);
				Print(out, "\t{{\n");
				Print(out, "\t\treturn nStatus;\n");
				Print(out, "\t}\n");
				Print(out, "\tfor (;;)\n", iter->m_sName);
				Print(out, "\t{{\n");
				switch (iter->m_nType)
				{
					case Field::Type_Bool:
					{
						Print(out, "\t\tif (CXNOK(nStatus = pReader->ArrayReadBool(pReader->pContext, &bBoolVal)))\n");
						Print(out, "\t\t{{\n");
						Print(out, "\t\t\tif (CX_Status_OutOfBounds == nStatus)\n");
						Print(out, "\t\t\t{{\n");
						Print(out, "\t\t\t\tbreak;\n");
						Print(out, "\t\t\t}\n");
						Print(out, "\t\t\treturn nStatus;\n");
						Print(out, "\t\t}\n");
						Print(out, "\t\tif (CXNOK(nStatus = CX_SB_BoolArray_Add(&pStruct->{1}, bBoolVal, pMem)))\n", iter->m_sName);
						Print(out, "\t\t{{\n");
						Print(out, "\t\t\treturn nStatus;\n");
						Print(out, "\t\t}\n");
					}
					break;
					case Field::Type_Int8:
					{
						Print(out, "\t\tif (CXNOK(nStatus = pReader->ArrayReadInt(pReader->pContext, &nIntVal)))\n");
						Print(out, "\t\t{{\n");
						Print(out, "\t\t\tif (CX_Status_OutOfBounds == nStatus)\n");
						Print(out, "\t\t\t{{\n");
						Print(out, "\t\t\t\tbreak;\n");
						Print(out, "\t\t\t}\n");
						Print(out, "\t\t\treturn nStatus;\n");
						Print(out, "\t\t}\n");
						Print(out, "\t\tif (CXNOK(nStatus = CX_SB_Int8Array_Add(&pStruct->{1}, (CX_Int8)nIntVal, pMem)))\n", iter->m_sName);
						Print(out, "\t\t{{\n");
						Print(out, "\t\t\treturn nStatus;\n");
						Print(out, "\t\t}\n");
					}
					break;
					case Field::Type_UInt8:
					{
						Print(out, "\t\tif (CXNOK(nStatus = pReader->ArrayReadInt(pReader->pContext, &nIntVal)))\n");
						Print(out, "\t\t{{\n");
						Print(out, "\t\t\tif (CX_Status_OutOfBounds == nStatus)\n");
						Print(out, "\t\t\t{{\n");
						Print(out, "\t\t\t\tbreak;\n");
						Print(out, "\t\t\t}\n");
						Print(out, "\t\t\treturn nStatus;\n");
						Print(out, "\t\t}\n");
						Print(out, "\t\tif (CXNOK(nStatus = CX_SB_UInt8Array_Add(&pStruct->{1}, (CX_UInt8)nIntVal, pMem)))\n", iter->m_sName);
						Print(out, "\t\t{{\n");
						Print(out, "\t\t\treturn nStatus;\n");
						Print(out, "\t\t}\n");
					}
					break;
					case Field::Type_Int16:
					{
						Print(out, "\t\tif (CXNOK(nStatus = pReader->ArrayReadInt(pReader->pContext, &nIntVal)))\n");
						Print(out, "\t\t{{\n");
						Print(out, "\t\t\tif (CX_Status_OutOfBounds == nStatus)\n");
						Print(out, "\t\t\t{{\n");
						Print(out, "\t\t\t\tbreak;\n");
						Print(out, "\t\t\t}\n");
						Print(out, "\t\t\treturn nStatus;\n");
						Print(out, "\t\t}\n");
						Print(out, "\t\tif (CXNOK(nStatus = CX_SB_Int16Array_Add(&pStruct->{1}, (CX_Int16)nIntVal, pMem)))\n", iter->m_sName);
						Print(out, "\t\t{{\n");
						Print(out, "\t\t\treturn nStatus;\n");
						Print(out, "\t\t}\n");
					}
					break;
					case Field::Type_UInt16:
					{
						Print(out, "\t\tif (CXNOK(nStatus = pReader->ArrayReadInt(pReader->pContext, &nIntVal)))\n");
						Print(out, "\t\t{{\n");
						Print(out, "\t\t\tif (CX_Status_OutOfBounds == nStatus)\n");
						Print(out, "\t\t\t{{\n");
						Print(out, "\t\t\t\tbreak;\n");
						Print(out, "\t\t\t}\n");
						Print(out, "\t\t\treturn nStatus;\n");
						Print(out, "\t\t}\n");
						Print(out, "\t\tif (CXNOK(nStatus = CX_SB_UInt16Array_Add(&pStruct->{1}, (CX_UInt16)nIntVal, pMem)))\n", iter->m_sName);
						Print(out, "\t\t{{\n");
						Print(out, "\t\t\treturn nStatus;\n");
						Print(out, "\t\t}\n");
					}
					break;
					case Field::Type_Int32:
					{
						Print(out, "\t\tif (CXNOK(nStatus = pReader->ArrayReadInt(pReader->pContext, &nIntVal)))\n");
						Print(out, "\t\t{{\n");
						Print(out, "\t\t\tif (CX_Status_OutOfBounds == nStatus)\n");
						Print(out, "\t\t\t{{\n");
						Print(out, "\t\t\t\tbreak;\n");
						Print(out, "\t\t\t}\n");
						Print(out, "\t\t\treturn nStatus;\n");
						Print(out, "\t\t}\n");
						Print(out, "\t\tif (CXNOK(nStatus = CX_SB_Int32Array_Add(&pStruct->{1}, (CX_Int32)nIntVal, pMem)))\n", iter->m_sName);
						Print(out, "\t\t{{\n");
						Print(out, "\t\t\treturn nStatus;\n");
						Print(out, "\t\t}\n");
					}
					break;
					case Field::Type_UInt32:
					{
						Print(out, "\t\tif (CXNOK(nStatus = pReader->ArrayReadInt(pReader->pContext, &nIntVal)))\n");
						Print(out, "\t\t{{\n");
						Print(out, "\t\t\tif (CX_Status_OutOfBounds == nStatus)\n");
						Print(out, "\t\t\t{{\n");
						Print(out, "\t\t\t\tbreak;\n");
						Print(out, "\t\t\t}\n");
						Print(out, "\t\t\treturn nStatus;\n");
						Print(out, "\t\t}\n");
						Print(out, "\t\tif (CXNOK(nStatus = CX_SB_UInt32Array_Add(&pStruct->{1}, (CX_UInt32)nIntVal, pMem)))\n", iter->m_sName);
						Print(out, "\t\t{{\n");
						Print(out, "\t\t\treturn nStatus;\n");
						Print(out, "\t\t}\n");
					}
					break;
					case Field::Type_Int64:
					{
						Print(out, "\t\tif (CXNOK(nStatus = pReader->ArrayReadInt(pReader->pContext, &nIntVal)))\n");
						Print(out, "\t\t{{\n");
						Print(out, "\t\t\tif (CX_Status_OutOfBounds == nStatus)\n");
						Print(out, "\t\t\t{{\n");
						Print(out, "\t\t\t\tbreak;\n");
						Print(out, "\t\t\t}\n");
						Print(out, "\t\t\treturn nStatus;\n");
						Print(out, "\t\t}\n");
						Print(out, "\t\tif (CXNOK(nStatus = CX_SB_Int64Array_Add(&pStruct->{1}, (CX_Int64)nIntVal, pMem)))\n", iter->m_sName);
						Print(out, "\t\t{{\n");
						Print(out, "\t\t\treturn nStatus;\n");
						Print(out, "\t\t}\n");
					}
					break;
					case Field::Type_UInt64:
					{
						Print(out, "\t\tif (CXNOK(nStatus = pReader->ArrayReadInt(pReader->pContext, &nIntVal)))\n");
						Print(out, "\t\t{{\n");
						Print(out, "\t\t\tif (CX_Status_OutOfBounds == nStatus)\n");
						Print(out, "\t\t\t{{\n");
						Print(out, "\t\t\t\tbreak;\n");
						Print(out, "\t\t\t}\n");
						Print(out, "\t\t\treturn nStatus;\n");
						Print(out, "\t\t}\n");
						Print(out, "\t\tif (CXNOK(nStatus = CX_SB_UInt64Array_Add(&pStruct->{1}, (CX_UInt64)nIntVal, pMem)))\n", iter->m_sName);
						Print(out, "\t\t{{\n");
						Print(out, "\t\t\treturn nStatus;\n");
						Print(out, "\t\t}\n");
					}
					break;
					case Field::Type_Float:
					{
						Print(out, "\t\tif (CXNOK(nStatus = pReader->ArrayReadReal(pReader->pContext, &nRealVal)))\n");
						Print(out, "\t\t{{\n");
						Print(out, "\t\t\tif (CX_Status_OutOfBounds == nStatus)\n");
						Print(out, "\t\t\t{{\n");
						Print(out, "\t\t\t\tbreak;\n");
						Print(out, "\t\t\t}\n");
						Print(out, "\t\t\treturn nStatus;\n");
						Print(out, "\t\t}\n");
						Print(out, "\t\tif (CXNOK(nStatus = CX_SB_FloatArray_Add(&pStruct->{1}, (CX_Float)nRealVal, pMem)))\n", iter->m_sName);
						Print(out, "\t\t{{\n");
						Print(out, "\t\t\treturn nStatus;\n");
						Print(out, "\t\t}\n");
					}
					break;
					case Field::Type_Double:
					{
						Print(out, "\t\tif (CXNOK(nStatus = pReader->ArrayReadReal(pReader->pContext, &nRealVal)))\n");
						Print(out, "\t\t{{\n");
						Print(out, "\t\t\tif (CX_Status_OutOfBounds == nStatus)\n");
						Print(out, "\t\t\t{{\n");
						Print(out, "\t\t\t\tbreak;\n");
						Print(out, "\t\t\t}\n");
						Print(out, "\t\t\treturn nStatus;\n");
						Print(out, "\t\t}\n");
						Print(out, "\t\tif (CXNOK(nStatus = CX_SB_DoubleArray_Add(&pStruct->{1}, (CX_Double)nRealVal, pMem)))\n", iter->m_sName);
						Print(out, "\t\t{{\n");
						Print(out, "\t\t\treturn nStatus;\n");
						Print(out, "\t\t}\n");
					}
					break;
					case Field::Type_String:
					{
						Print(out, "\t\tif (CXNOK(nStatus = pReader->ArrayReadString(pReader->pContext, &szStrVal)))\n", iter->m_sName);
						Print(out, "\t\t{{\n");
						Print(out, "\t\t\tif (CX_Status_OutOfBounds == nStatus)\n");
						Print(out, "\t\t\t{{\n");
						Print(out, "\t\t\t\tbreak;\n");
						Print(out, "\t\t\t}\n");
						Print(out, "\t\t\treturn nStatus;\n");
						Print(out, "\t\t}\n");
						Print(out, "\t\tif (CXNOK(nStatus = CX_SB_StringArray_Add(&pStruct->{1}, szStrVal, pMem)))\n", iter->m_sName);
						Print(out, "\t\t{{\n");
						Print(out, "\t\t\tpReader->FreeString(pReader->pContext, szStrVal);\n");
						Print(out, "\t\n");
						Print(out, "\t\t\treturn nStatus;\n");
						Print(out, "\t\t}\n");
						Print(out, "\t\t\tpReader->FreeString(pReader->pContext, szStrVal);\n");
					}
					break;
					case Field::Type_WString:
					{
						Print(out, "\t\tif (CXNOK(nStatus = pReader->ArrayReadWString(pReader->pContext, &wszStrVal)))\n", iter->m_sName);
						Print(out, "\t\t{{\n");
						Print(out, "\t\t\tif (CX_Status_OutOfBounds == nStatus)\n");
						Print(out, "\t\t\t{{\n");
						Print(out, "\t\t\t\tbreak;\n");
						Print(out, "\t\t\t}\n");
						Print(out, "\t\t\treturn nStatus;\n");
						Print(out, "\t\t}\n");
						Print(out, "\t\tif (CXNOK(nStatus = CX_SB_WStringArray_Add(&pStruct->{1}, wszStrVal, pMem)))\n", iter->m_sName);
						Print(out, "\t\t{{\n");
						Print(out, "\t\t\tpReader->FreeWString(pReader->pContext, wszStrVal);\n");
						Print(out, "\t\n");
						Print(out, "\t\t\treturn nStatus;\n");
						Print(out, "\t\t}\n");
						Print(out, "\t\t\tpReader->FreeWString(pReader->pContext, wszStrVal);\n");
					}
					break;
					case Field::Type_Struct:
					{
						Print(out, "\t\tif (CXNOK(nStatus = pReader->ArrayBeginStruct(pReader->pContext)))\n");
						Print(out, "\t\t{{\n");
						Print(out, "\t\t\tif (CX_Status_OutOfBounds == nStatus)\n");
						Print(out, "\t\t\t{{\n");
						Print(out, "\t\t\t\tbreak;\n");
						Print(out, "\t\t\t}\n");
						Print(out, "\t\t\treturn nStatus;\n");
						Print(out, "\t\t}\n");
						Print(out, "\t\tif (CXNOK(nStatus = {2}_Read({2}Array_Add(&pStruct->{1}, pMem), pReader, pMem)))\n", iter->m_sName, iter->m_sStructName);
						Print(out, "\t\t{{\n");
						Print(out, "\t\t\treturn nStatus;\n");
						Print(out, "\t\t}\n");
						Print(out, "\t\tif (CXNOK(nStatus = pReader->ArrayEndStruct(pReader->pContext)))\n");
						Print(out, "\t\t{{\n");
						Print(out, "\t\t\treturn nStatus;\n");
						Print(out, "\t\t}\n");
					}
					break;
				}
				Print(out, "\t}\n");
				Print(out, "\tif (CXNOK(nStatus = pReader->StructEndArray(pReader->pContext)))\n");
				Print(out, "\t{{\n");
				Print(out, "\t\treturn nStatus;\n");
				Print(out, "\t}\n");
			}
		}
		Print(out, "\n");
		Print(out, "\treturn CX_Status_OK;\n");
		Print(out, "}\n");
		Print(out, "\n");

		Print(out, "CX_StatusCode {1}Array_Init({1}Array *pArray, CX_SB_Memory *pMem)\n", pStruct->m_sName);
		Print(out, "{{\n");
		Print(out, "\tpMem;\n");
		Print(out, "\n");
		Print(out, "\tpArray->cUsedCount  = 0;\n");
		Print(out, "\tpArray->cTotalCount = 0;\n");
		Print(out, "\tpArray->values      = NULL;\n");
		Print(out, "\n");
		Print(out, "\treturn CX_Status_OK;\n");
		Print(out, "}\n");
		Print(out, "\n");

		Print(out, "CX_StatusCode {1}Array_Uninit({1}Array *pArray, CX_SB_Memory *pMem)\n", pStruct->m_sName);
		Print(out, "{{\n");
		Print(out, "\tCX_Size cIndex;\n");
		Print(out, "\n");
		Print(out, "\tif (NULL != pArray->values)\n");
		Print(out, "\t{{\n");
		Print(out, "\t\tfor (cIndex = 0; cIndex < pArray->cTotalCount; cIndex++)\n");
		Print(out, "\t\t{{\n");
		Print(out, "\t\t\t{1}_Uninit(&pArray->values[cIndex], pMem);\n", pStruct->m_sName);
		Print(out, "\t\t}\n");
		Print(out, "\n");
		Print(out, "\t\tpMem->Free(pMem->pContext, pArray->values);\n");
		Print(out, "\t}\n");
		Print(out, "\tpArray->cUsedCount  = 0;\n");
		Print(out, "\tpArray->cTotalCount = 0;\n");
		Print(out, "\tpArray->values      = NULL;\n");
		Print(out, "\n");
		Print(out, "\treturn CX_Status_OK;\n");
		Print(out, "}\n");
		Print(out, "\n");

		Print(out, "CX_Size {1}Array_GetCount({1}Array *pArray, CX_SB_Memory *pMem)\n", pStruct->m_sName);
		Print(out, "{{\n");
		Print(out, "\tpMem;\n");
		Print(out, "\n");
		Print(out, "\treturn pArray->cUsedCount;\n");
		Print(out, "}\n");
		Print(out, "\n");

		Print(out, "{1} *{1}Array_Get({1}Array *pArray, CX_Size cIndex, CX_SB_Memory *pMem)\n", pStruct->m_sName);
		Print(out, "{{\n");
		Print(out, "\tpMem;\n");
		Print(out, "\n");
		Print(out, "\tif (cIndex >= pArray->cUsedCount)\n");
		Print(out, "\t{{\n");
		Print(out, "\t\treturn NULL;\n");
		Print(out, "\t}\n");
		Print(out, "\n");
		Print(out, "\treturn &pArray->values[cIndex];\n");
		Print(out, "}\n");
		Print(out, "\n");

		Print(out, "{1} *{1}Array_Add({1}Array *pArray, CX_SB_Memory *pMem)\n", pStruct->m_sName);
		Print(out, "{{\n");
		Print(out, "\tCX_StatusCode nStatus;\n");
		Print(out, "\n");
		Print(out, "\tif (pArray->cUsedCount == pArray->cTotalCount)\n");
		Print(out, "\t{{\n");
		Print(out, "\t\tif (CXNOK(nStatus = {1}Array_Reserve(pArray, pArray->cUsedCount + 1, pMem)))\n", pStruct->m_sName);
		Print(out, "\t\t{{\n");
		Print(out, "\t\t\t\treturn NULL;\n");
		Print(out, "\t\t}\n");
		Print(out, "\t}\n");
		Print(out, "\tpArray->cUsedCount++;\n");
		Print(out, "\n");
		Print(out, "\treturn {1}Array_Get(pArray, pArray->cUsedCount - 1, pMem);\n", pStruct->m_sName);
		Print(out, "}\n");
		Print(out, "\n");

		Print(out, "CX_StatusCode {1}Array_Remove({1}Array *pArray, CX_Size cIndex, CX_Size cCount, CX_SB_Memory *pMem)\n", pStruct->m_sName);
		Print(out, "{{\n");
		Print(out, "\tCX_Size i;\n");
		Print(out, "\tCX_Size cSrcIndex;\n");
		Print(out, "\tCX_Size cSrcCount;\n");
		Print(out, "\n");
		Print(out, "\tif (cIndex >= pArray->cUsedCount)\n");
		Print(out, "\t{{\n");
		Print(out, "\t\treturn CX_Status_OutOfBounds;\n");
		Print(out, "\t}\n");
		Print(out, "\tif (cIndex + cCount > pArray->cUsedCount)\n");
		Print(out, "\t{{\n");
		Print(out, "\t\tcCount = pArray->cUsedCount - cIndex;\n");
		Print(out, "\t}\n");
		Print(out, "\tfor (i = 0; i < cCount; i++)\n");
		Print(out, "\t{{\n");
		Print(out, "\t\t{1}_Uninit(&pArray->values[cIndex + i], pMem);\n", pStruct->m_sName);
		Print(out, "\t\t{1}_Init(&pArray->values[cIndex + i], pMem);\n", pStruct->m_sName);
		Print(out, "\t}\n");
		Print(out, "\n");
		Print(out, "\tcSrcIndex = cIndex + cCount;\n");
		Print(out, "\tcSrcCount = cCount;\n");
		Print(out, "\tif (cSrcIndex + cSrcCount > pArray->cUsedCount)\n");
		Print(out, "\t{{\n");
		Print(out, "\t\tcSrcCount = pArray->cUsedCount - cSrcIndex;\n");
		Print(out, "\t}\n");
		Print(out, "\tfor (i = 0; i < cSrcCount; i++)\n");
		Print(out, "\t{{\n");
		Print(out, "\t\tpArray->values[cIndex + i] = pArray->values[cSrcIndex + i];\n");
		Print(out, "\t\t{1}_Init(&pArray->values[cSrcIndex + i], pMem);\n", pStruct->m_sName);
		Print(out, "\t}\n");
		Print(out, "\tpArray->cUsedCount -= cCount;\n");
		Print(out, "\n");
		Print(out, "\treturn CX_Status_OK;\n");
		Print(out, "}\n");
		Print(out, "\n");

		Print(out, "CX_StatusCode {1}Array_FreeUnusedMem({1}Array *pArray, CX_SB_Memory *pMem)\n", pStruct->m_sName);
		Print(out, "{{\n");
		Print(out, "\tCX_Size i;\n");
		Print(out, "\tvoid    *pPtr;\n");
		Print(out, "\n");
		Print(out, "\tif (pArray->cUsedCount == pArray->cTotalCount)\n");
		Print(out, "\t{{\n");
		Print(out, "\t\treturn CX_Status_OK;\n");
		Print(out, "\t}\n");
		Print(out, "\tfor (i = pArray->cUsedCount; i < pArray->cTotalCount; i++)\n");
		Print(out, "\t{{\n");
		Print(out, "\t\t{1}_Uninit(&pArray->values[i], pMem);\n", pStruct->m_sName);
		Print(out, "\t}\n");
		Print(out, "\tif (NULL == (pPtr = pMem->Realloc(pMem->pContext, pArray->values, sizeof({1}) * pArray->cUsedCount)))\n", pStruct->m_sName);
		Print(out, "\t{{\n");
		Print(out, "\t\treturn CX_Status_MemAllocFailed;\n");
		Print(out, "\t}\n");
		Print(out, "\tpArray->cTotalCount = pArray->cUsedCount;\n");
		Print(out, "\n");
		Print(out, "\treturn CX_Status_OK;\n");
		Print(out, "}\n");
		Print(out, "\n");

		Print(out, "CX_StatusCode {1}Array_Reserve({1}Array *pArray, CX_Size cTotalCount, CX_SB_Memory *pMem)\n", pStruct->m_sName);
		Print(out, "{{\n");
		Print(out, "	CX_Size i;\n");
		Print(out, "	void    *pPtr;\n");
		Print(out, "\n");
		Print(out, "	if (cTotalCount <= pArray->cTotalCount)\n");
		Print(out, "	{{\n");
		Print(out, "		return CX_Status_OK;\n");
		Print(out, "	}\n");
		Print(out, "	if (NULL == (pPtr = pMem->Realloc(pMem->pContext, pArray->values, sizeof({1}) * cTotalCount)))\n", pStruct->m_sName);
		Print(out, "	{{\n");
		Print(out, "		return CX_Status_MemAllocFailed;\n");
		Print(out, "	}\n");
		Print(out, "	pArray->values      = ({1} *)pPtr;\n", pStruct->m_sName);
		Print(out, "	for (i = pArray->cTotalCount; i < cTotalCount; i++)\n");
		Print(out, "	{{\n");
		Print(out, "		{1}_Init(&pArray->values[i], pMem);\n", pStruct->m_sName);
		Print(out, "	}\n");
		Print(out, "	pArray->cTotalCount = cTotalCount;\n");
		Print(out, "\n");
		Print(out, "	return CX_Status_OK;\n");
		Print(out, "}\n");
		Print(out, "\n");

		return Status();
	}

	//internal helpers

	Status SBAPI_GenerateHeaderWithPath(const Char *szPath)
	{
		IO::FileOutputStream fos(szPath);

		if (!fos.IsOK())
		{
			return Status(Status_CreateFailed, "Failed to create file '{1}'", szPath);
		}

		return SBAPI_GenerateHeader((IO::IOutputStream *)&fos);
	}

	template <typename OUTPUT>
	Status SBAPI_GenerateHeader(OUTPUT out)
	{
		const CX::Char *types[] =
		{
			"Bool",
			"Int8",
			"UInt8",
			"Int16",
			"UInt16",
			"Int32",
			"UInt32",
			"Int64",
			"UInt64",
			"Float",
			"Double",
		};
		const CX::Size cCount = sizeof(types) / sizeof(types[0]);

		for (CX::Size i = 0; i < cCount; i++)
		{
			CX::Print(out, "typedef struct _CX_SB_{1}Array\n", types[i]);
			CX::Print(out, "{{\n");
			CX::Print(out, "\tCX_{1}         *values;\n", types[i]);
			CX::Print(out, "\tCX_Size         cUsedCount;\n");
			CX::Print(out, "\tCX_Size         cTotalCount;\n");
			CX::Print(out, "}CX_SB_{1}Array;\n", types[i]);
			CX::Print(out, "\n");
			CX::Print(out, "CX_StatusCode CX_SB_{1}Array_Init(CX_SB_{1}Array *pArray, CX_SB_Memory *pMem);\n", types[i]);
			CX::Print(out, "\n");
			CX::Print(out, "CX_StatusCode CX_SB_{1}Array_Uninit(CX_SB_{1}Array *pArray, CX_SB_Memory *pMem);\n", types[i]);
			CX::Print(out, "\n");
			CX::Print(out, "CX_Size CX_SB_{1}Array_GetCount(CX_SB_{1}Array *pArray, CX_SB_Memory *pMem);\n", types[i]);
			CX::Print(out, "\n");
			CX::Print(out, "CX_StatusCode CX_SB_{1}Array_Set(CX_SB_{1}Array *pArray, CX_Size cIndex, CX_{1} val, CX_SB_Memory *pMem);\n", types[i]);
			CX::Print(out, "\n");
			CX::Print(out, "CX_{1} CX_SB_{1}Array_Get(CX_SB_{1}Array *pArray, CX_Size cIndex, CX_{1} defval, CX_SB_Memory *pMem);\n", types[i]);
			CX::Print(out, "\n");
			CX::Print(out, "CX_StatusCode CX_SB_{1}Array_Add(CX_SB_{1}Array *pArray, CX_{1} val, CX_SB_Memory *pMem);\n", types[i]);
			CX::Print(out, "\n");
			CX::Print(out, "CX_StatusCode CX_SB_{1}Array_Remove(CX_SB_{1}Array *pArray, CX_Size cIndex, CX_Size cCount, CX_SB_Memory *pMem);\n", types[i]);
			CX::Print(out, "\n");
			CX::Print(out, "CX_StatusCode CX_SB_{1}Array_FreeUnusedMem(CX_SB_{1}Array *pArray, CX_SB_Memory *pMem);\n", types[i]);
			CX::Print(out, "\n");
			CX::Print(out, "CX_StatusCode CX_SB_{1}Array_Reserve(CX_SB_{1}Array *pArray, CX_Size cTotalCount, CX_SB_Memory *pMem);\n", types[i]);
			CX::Print(out, "\n");
			CX::Print(out, "\n");
		}

		return Status();
	}

	Status SBAPI_GenerateSourceWithPath(const Char *szPath)
	{
		IO::FileOutputStream fos(szPath);

		if (!fos.IsOK())
		{
			return Status(Status_CreateFailed, "Failed to create file '{1}'", szPath);
		}

		return SBAPI_GenerateSource((IO::IOutputStream *)&fos);
	}

	template <typename OUTPUT>
	Status SBAPI_GenerateSource(OUTPUT out)
	{
		const CX::Char *types[] =
		{
			"Bool",
			"Int8",
			"UInt8",
			"Int16",
			"UInt16",
			"Int32",
			"UInt32",
			"Int64",
			"UInt64",
			"Float",
			"Double",
		};
		const CX::Size cCount = sizeof(types) / sizeof(types[0]);

		for (CX::Size i = 0; i < cCount; i++)
		{
			CX::Print(out, "CX_StatusCode CX_SB_{1}Array_Init(CX_SB_{1}Array *pArray, CX_SB_Memory *pMem)\n", types[i]);
			CX::Print(out, "{{\n");
			CX::Print(out, "\tpMem;\n");
			CX::Print(out, "\n");
			CX::Print(out, "\tpArray->cUsedCount  = 0;\n");
			CX::Print(out, "\tpArray->cTotalCount = 0;\n");
			CX::Print(out, "\tpArray->values      = NULL;\n");
			CX::Print(out, "\n");
			CX::Print(out, "\treturn CX_Status_OK;\n");
			CX::Print(out, "}\n");
			CX::Print(out, "\n");

			CX::Print(out, "CX_StatusCode CX_SB_{1}Array_Uninit(CX_SB_{1}Array *pArray, CX_SB_Memory *pMem)\n", types[i]);
			CX::Print(out, "{{\n");
			CX::Print(out, "\tif (NULL != pArray->values)\n");
			CX::Print(out, "\t{{\n");
			CX::Print(out, "\t\tpMem->Free(pMem->pContext, pArray->values);\n");
			CX::Print(out, "\t}\n");
			CX::Print(out, "\tpArray->cUsedCount  = 0;\n");
			CX::Print(out, "\tpArray->cTotalCount = 0;\n");
			CX::Print(out, "\tpArray->values      = NULL;\n");
			CX::Print(out, "\n");
			CX::Print(out, "\treturn CX_Status_OK;\n");
			CX::Print(out, "}\n");
			CX::Print(out, "\n");

			CX::Print(out, "CX_Size CX_SB_{1}Array_GetCount(CX_SB_{1}Array *pArray, CX_SB_Memory *pMem)\n", types[i]);
			CX::Print(out, "{{\n");
			CX::Print(out, "\tpMem;\n");
			CX::Print(out, "\n");
			CX::Print(out, "\treturn pArray->cUsedCount;\n");
			CX::Print(out, "}\n");
			CX::Print(out, "\n");

			CX::Print(out, "CX_StatusCode CX_SB_{1}Array_Set(CX_SB_{1}Array *pArray, CX_Size cIndex, CX_{1} val, CX_SB_Memory *pMem)\n", types[i]);
			CX::Print(out, "{{\n");
			CX::Print(out, "\tpMem;\n");
			CX::Print(out, "\n");
			CX::Print(out, "\tif (cIndex >= pArray->cUsedCount)\n");
			CX::Print(out, "\t{{\n");
			CX::Print(out, "\t\treturn CX_Status_OutOfBounds;\n");
			CX::Print(out, "\t}\n");
			CX::Print(out, "\tpArray->values[cIndex] = val;\n");
			CX::Print(out, "\n");
			CX::Print(out, "\treturn CX_Status_OK;\n");
			CX::Print(out, "}\n");
			CX::Print(out, "\n");

			CX::Print(out, "CX_{1} CX_SB_{1}Array_Get(CX_SB_{1}Array *pArray, CX_Size cIndex, CX_{1} defval, CX_SB_Memory *pMem)\n", types[i]);
			CX::Print(out, "{{\n");
			CX::Print(out, "\tpMem;\n");
			CX::Print(out, "\n");
			CX::Print(out, "\tif (cIndex >= pArray->cUsedCount)\n");
			CX::Print(out, "\t{{\n");
			CX::Print(out, "\t\treturn defval;\n");
			CX::Print(out, "\t}\n");
			CX::Print(out, "\n");
			CX::Print(out, "\treturn pArray->values[cIndex];\n");
			CX::Print(out, "}\n");
			CX::Print(out, "\n");

			CX::Print(out, "CX_StatusCode CX_SB_{1}Array_Add(CX_SB_{1}Array *pArray, CX_{1} val, CX_SB_Memory *pMem)\n", types[i]);
			CX::Print(out, "{{\n");
			CX::Print(out, "\tCX_StatusCode nStatus;\n");
			CX::Print(out, "\n");
			CX::Print(out, "\tif (pArray->cUsedCount == pArray->cTotalCount)\n");
			CX::Print(out, "\t{{\n");
			CX::Print(out, "\t\tif (CXNOK(nStatus = CX_SB_{1}Array_Reserve(pArray, pArray->cUsedCount + 1, pMem)))\n", types[i]);
			CX::Print(out, "\t\t{{\n");
			CX::Print(out, "\t\t\treturn nStatus;\n");
			CX::Print(out, "\t\t}\n");
			CX::Print(out, "\t}\n");
			CX::Print(out, "\tpArray->values[pArray->cUsedCount] = val;\n");
			CX::Print(out, "\tpArray->cUsedCount++;\n");
			CX::Print(out, "\n");
			CX::Print(out, "\treturn CX_Status_OK;\n");
			CX::Print(out, "}\n");
			CX::Print(out, "\n");

			CX::Print(out, "CX_StatusCode CX_SB_{1}Array_Remove(CX_SB_{1}Array *pArray, CX_Size cIndex, CX_Size cCount, CX_SB_Memory *pMem)\n", types[i]);
			CX::Print(out, "{{\n");
			CX::Print(out, "\tCX_Size i;\n");
			CX::Print(out, "\tCX_Size cSrcIndex;\n");
			CX::Print(out, "\tCX_Size cSrcCount;\n");
			CX::Print(out, "\n");
			CX::Print(out, "\tpMem;\n");
			CX::Print(out, "\n");
			CX::Print(out, "\tif (cIndex >= pArray->cUsedCount)\n");
			CX::Print(out, "\t{{\n");
			CX::Print(out, "\t\treturn CX_Status_OutOfBounds;\n");
			CX::Print(out, "\t}\n");
			CX::Print(out, "\tif (cIndex + cCount > pArray->cUsedCount)\n");
			CX::Print(out, "\t{{\n");
			CX::Print(out, "\t\tcCount = pArray->cUsedCount - cIndex;\n");
			CX::Print(out, "\t}\n");
			CX::Print(out, "\tcSrcIndex = cIndex + cCount;\n");
			CX::Print(out, "\tcSrcCount = cCount;\n");
			CX::Print(out, "\tif (cSrcIndex + cSrcCount > pArray->cUsedCount)\n");
			CX::Print(out, "\t{{\n");
			CX::Print(out, "\t\tcSrcCount = pArray->cUsedCount - cSrcIndex;\n");
			CX::Print(out, "\t}\n");
			CX::Print(out, "\tfor (i = 0; i < cSrcCount; i++)\n");
			CX::Print(out, "\t{{\n");
			CX::Print(out, "\t\tpArray->values[cIndex + i] = pArray->values[cSrcIndex + i];\n");
			CX::Print(out, "\t}\n");
			CX::Print(out, "\tpArray->cUsedCount -= cCount;\n");
			CX::Print(out, "\n");
			CX::Print(out, "\treturn CX_Status_OK;\n");
			CX::Print(out, "}\n");
			CX::Print(out, "\n");

			CX::Print(out, "CX_StatusCode CX_SB_{1}Array_FreeUnusedMem(CX_SB_{1}Array *pArray, CX_SB_Memory *pMem)\n", types[i]);
			CX::Print(out, "{{\n");
			CX::Print(out, "\tvoid *pPtr;\n");
			CX::Print(out, "\n");
			CX::Print(out, "\tif (pArray->cUsedCount == pArray->cTotalCount)\n");
			CX::Print(out, "\t{{\n");
			CX::Print(out, "\treturn CX_Status_OK;\n");
			CX::Print(out, "\t}\n");
			CX::Print(out, "\tif (NULL == (pPtr = pMem->Realloc(pMem->pContext, pArray->values, sizeof(CX_{1}) * pArray->cUsedCount)))\n", types[i]);
			CX::Print(out, "\t{{\n");
			CX::Print(out, "\treturn CX_Status_MemAllocFailed;\n");
			CX::Print(out, "\t}\n");
			CX::Print(out, "\tpArray->cTotalCount = pArray->cUsedCount;\n");
			CX::Print(out, "\n");
			CX::Print(out, "\treturn CX_Status_OK;\n");
			CX::Print(out, "}\n");
			CX::Print(out, "\n");

			CX::Print(out, "CX_StatusCode CX_SB_{1}Array_Reserve(CX_SB_{1}Array *pArray, CX_Size cTotalCount, CX_SB_Memory *pMem)\n", types[i]);
			CX::Print(out, "{{\n");
			CX::Print(out, "\tvoid *pPtr;\n");
			CX::Print(out, "\n");
			CX::Print(out, "\tif (cTotalCount <= pArray->cTotalCount)\n");
			CX::Print(out, "\t{{\n");
			CX::Print(out, "\t\treturn CX_Status_OK;\n");
			CX::Print(out, "\t}\n");
			CX::Print(out, "\tif (NULL == (pPtr = pMem->Realloc(pMem->pContext, pArray->values, sizeof(CX_{1}) * cTotalCount)))\n", types[i]);
			CX::Print(out, "\t{{\n");
			CX::Print(out, "\t\treturn CX_Status_MemAllocFailed;\n");
			CX::Print(out, "\t}\n");
			CX::Print(out, "\tpArray->values      = (CX_{1} *)pPtr;\n", types[i]);
			CX::Print(out, "\tpArray->cTotalCount = cTotalCount;\n");
			CX::Print(out, "\n");
			CX::Print(out, "\treturn CX_Status_OK;\n");
			CX::Print(out, "}\n");
			CX::Print(out, "\n");
		}

		return Status();
	}

private:

};


}//namespace SB

}//namespace CX

