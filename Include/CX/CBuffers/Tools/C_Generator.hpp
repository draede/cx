/* 
 * CX - C++ framework for general purpose development
 *
 * https://github.com/draede/cx
 * 
 * Copyright (C) 2014 - 2017 draede - draede [at] outlook [dot] com
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

#pragma once


#include "CX/CBuffers/Tools/IGenerator.hpp"
#include "CX/IO/FileOutputStream.hpp"
#include "CX/Print.hpp"
#include "CX/CBuffers/Types.h"


namespace CX
{

namespace CBuffers
{

namespace Tools
{

class C_Generator : public IGenerator
{
public:

	static const Char *NAME()   { return "C"; }

	static const Char *ID()   { return "c"; }

	virtual const Char *GetName()
	{
		return NAME();
	}

	virtual const Char *GetID()
	{
		return ID();
	}

	virtual Status Generate(const StructsMap &mapStructs, const Char *szPath)
	{
		Status status;
		
		for (auto iter = mapStructs.begin(); iter != mapStructs.end(); ++iter)
		{
			if (!(status = GenerateStruct(szPath, iter->second, mapStructs)))
			{
				return status;
			}
		}

		return Status();
	}
	
private:

	Status GenerateStruct(const Char *szPath, const Struct &strct, const StructsMap &mapStructs)
	{
		String       sFullName;
		String       sName;
		String       sHeaderPath;
		String       sSourcePath;
		const Char   *pszPos;
		Status       status;

		pszPos = strct.sName.c_str();
		for (Size i = 0; i < strct.sName.size(); i++)
		{
			if ('.' == strct.sName[i])
			{
				sFullName += '_';
				pszPos = strct.sName.c_str() + i + 1;
			}
			else
			{
				sFullName += strct.sName[i];
			}
		}
		sName = pszPos;

		sHeaderPath = szPath;
		sSourcePath = szPath;
#ifdef CX_OS_WINDOWS
		sHeaderPath += "\\";
		sSourcePath += "\\";
#else
		sHeaderPath += "/";
		sSourcePath += "/";
#endif
		sHeaderPath += sFullName;
		sSourcePath += sFullName;
		sHeaderPath += ".h";
		sSourcePath += ".c";

		{
			IO::FileOutputStream   os(sHeaderPath.c_str());

			if (!os.IsOK())
			{
				return Status(Status_CreateFailed, "Failed to create '{1}'", sHeaderPath);
			}
			if (!(status = GenerateHeader(&os, sFullName, sName, strct, mapStructs)))
			{
				return status;
			}
		}
		{
			IO::FileOutputStream   os(sSourcePath.c_str());

			if (!os.IsOK())
			{
				return Status(Status_CreateFailed, "Failed to create '{1}'", sSourcePath);
			}
			if (!(status = GenerateSource(&os, sFullName, sName, strct, mapStructs)))
			{
				return status;
			}
		}

		return Status();
	}

	Status GenerateHeader(IO::IOutputStream *pOS, const String &sFullName, const String &sName, 
	                      const Struct &strct, const StructsMap &mapStructs)
	{
		CX_UNUSED(sName);
		CX_UNUSED(mapStructs);

		StringsSet   setIncludes;
		Status       status;

		Print(pOS, "\n");
		Print(pOS, "#pragma once\n");
		Print(pOS, "\n");
		Print(pOS, "\n");
		Print(pOS, "#include \"CX/C/Types.h\"\n");
		Print(pOS, "#include \"CX/C/StatusCodes.h\"\n");
		Print(pOS, "#include \"Buffer.h\"\n");
		Print(pOS, "#include \"Environment.h\"\n");
		Print(pOS, "#include \"Types.h\"\n");
		for (auto iter = strct.vectorMembers.begin(); iter != strct.vectorMembers.end(); ++iter)
		{
			if (Type_Struct != iter->nType)
			{
				continue;
			}
			if (setIncludes.end() != setIncludes.find(iter->sTypeName))
			{
				continue;
			}

			String sMemberType;

			for (Size i = 0; i < iter->sTypeName.size(); i++)
			{
				if ('.' == iter->sTypeName[i])
				{
					sMemberType += '_';
				}
				else
				{
					sMemberType += iter->sTypeName[i];
				}
			}

			setIncludes.insert(iter->sTypeName);

			Print(pOS, "#include \"{1}.h\"\n", sMemberType);
		}
		Print(pOS, "\n");
		Print(pOS, "\n");
		Print(pOS, "#ifdef __cplusplus\n");
		Print(pOS, "extern \"C\" {{\n");
		Print(pOS, "#endif\n");
		Print(pOS, "\n");
		Print(pOS, "\n");

		Print(pOS, "typedef struct _{1}\n", sFullName);
		Print(pOS, "{{\n");
		for (auto iter = strct.vectorMembers.begin(); iter != strct.vectorMembers.end(); ++iter)
		{
			String sMemberType;

			for (Size i = 0; i < iter->sTypeName.size(); i++)
			{
				if ('.' == iter->sTypeName[i])
				{
					sMemberType += '_';
				}
				else
				{
					sMemberType += iter->sTypeName[i];
				}
			}

			if (Type_Struct == iter->nType)
			{
				if (Aggregate_Scalar ==iter->nAggregateType)
				{
					Print(pOS, "\t//You must call Destroy on ppStruct when done\n");
					Print(pOS, "\tCX_StatusCode (* Get{2})(struct _{1} *pThis, {3} **ppStruct);\n", sFullName, iter->sName, sMemberType);
				}
				else
				if (Aggregate_StaticArray == iter->nAggregateType)
				{
					Print(pOS, "\tCX_CB_Size (* Get{2}Count)(struct _{1} *pThis);\n", sFullName, iter->sName);
					Print(pOS, "\t//You must call Destroy on ppStruct when done\n");
					Print(pOS, "\tCX_StatusCode (* Get{2}Item)(struct _{1} *pThis, CX_CB_Size cIndex, {3} **ppStruct);\n", sFullName, iter->sName, sMemberType);
				}
				else
				if (Aggregate_Array == iter->nAggregateType)
				{
					Print(pOS, "\tCX_CB_Size (* Get{2}Count)(struct _{1} *pThis);\n", sFullName, iter->sName);
					Print(pOS, "\tCX_StatusCode (* Set{2}Count)(struct _{1} *pThis, CX_CB_Size cCount);\n", sFullName, iter->sName);
					Print(pOS, "\t//You must call Destroy on ppStruct when done\n");
					Print(pOS, "\tCX_StatusCode (* Get{2}Item)(struct _{1} *pThis, CX_CB_Size cIndex, {3} **ppStruct);\n", sFullName, iter->sName, sMemberType);
				}
				else
				{
					return Status_InvalidArg;
				}
			}
			else
			{
				if (Aggregate_Scalar == iter->nAggregateType)
				{
					Print(pOS, "\tCX_StatusCode (* Set{2})(struct _{1} *pThis, {3} value);\n", sFullName, iter->sName, iter->sActualTypeName);
					Print(pOS, "\t{3} (* Get{2})(struct _{1} *pThis);\n", sFullName, iter->sName, iter->sActualTypeName);
				}
				else
				if (Aggregate_StaticArray == iter->nAggregateType)
				{
					Print(pOS, "\tCX_CB_Size (* Get{2}Count)(struct _{1} *pThis);\n", sFullName, iter->sName);
					Print(pOS, "\t{3} *(* Get{2})(struct _{1} *pThis);\n", sFullName, iter->sName, iter->sActualTypeName);
				}
				else
				if (Aggregate_Array == iter->nAggregateType)
				{
					Print(pOS, "\tCX_CB_Size (* Get{2}Count)(struct _{1} *pThis);\n", sFullName, iter->sName);
					Print(pOS, "\tCX_StatusCode (* Set{2}Count)(struct _{1} *pThis, CX_CB_Size cCount);\n", sFullName, iter->sName);
					Print(pOS, "\t{3} *(* Get{2})(struct _{1} *pThis);\n", sFullName, iter->sName, iter->sActualTypeName);
				}
				else
				{
					return Status_InvalidArg;
				}
			}
		}
		Print(pOS, "\tCX_StatusCode (* Destroy)(struct _{1} *pThis);\n", sFullName);
		Print(pOS, "}{1};\n", sFullName);
		Print(pOS, "\n");
		Print(pOS, "CX_StatusCode {1}_Create({1} **ppStruct, CX_CB_Environment *pEnv, CX_CB_Buffer *pBuffer, CX_CB_Size cbStartOffset);\n", sFullName);
		Print(pOS, "\n");
		Print(pOS, "#ifdef __cplusplus\n");
		Print(pOS, "}\n");
		Print(pOS, "#endif\n");
		Print(pOS, "\n");

		return Status();
	}

	static inline Status GenerateSource(IO::IOutputStream *pOS, const String &sFullName, const String &sName, 
	                                    const Struct &strct, const StructsMap &mapStructs)
	{
		CX_UNUSED(sName);

		Status   status;

		Print(pOS, "\n");
		Print(pOS, "#include \"{1}.h\"\n", sFullName);
		Print(pOS, "#include \"Struct.h\"\n");
		Print(pOS, "\n");
		Print(pOS, "\n");
		Print(pOS, "typedef struct _{1}_Ex\n", sFullName);
		Print(pOS, "{{\n");
		Print(pOS, "\t{1}    base;\n", sFullName);
		Print(pOS, "\tCX_CB_Struct   helper;\n");
		Print(pOS, "}{1}_Ex;\n", sFullName);
		Print(pOS, "\n");

		for (auto iter = strct.vectorMembers.begin(); iter != strct.vectorMembers.end(); ++iter)
		{
			String sMemberType;

			for (Size i = 0; i < iter->sTypeName.size(); i++)
			{
				if ('.' == iter->sTypeName[i])
				{
					sMemberType += '_';
				}
				else
				{
					sMemberType += iter->sTypeName[i];
				}
			}

			if (Type_Struct == iter->nType)
			{
				auto iterEx = mapStructs.find(iter->sTypeName);

				if (mapStructs.end() == iterEx)
				{
					return CX_Status_InvalidArg;
				}

				if (Aggregate_Scalar == iter->nAggregateType)
				{
					Print(pOS, "static CX_StatusCode {1}_Get{2}(struct _{1} *pThis, {3} **ppStruct)\n", sFullName, iter->sName, sMemberType);
					Print(pOS, "{{\n");
					Print(pOS, "\tCX_Byte             *pValue;\n", iter->sActualTypeName);
					Print(pOS, "\tCX_CB_Environment   *pEnv;\n");
					Print(pOS, "\tCX_CB_Buffer        *pBuffer;\n");
					Print(pOS, "\tCX_StatusCode       nStatus;\n");
					Print(pOS, "\n");
					Print(pOS, "\tif (CXNOK(nStatus = CX_CB_Struct_GetStatic_Ex(pThis, CX_SB_OFFSETOF({3}_Ex, helper), {1}, &pValue, {2})))\n", iter->cbOffset, iter->cbSize, sFullName);
					Print(pOS, "\t{{\n");
					Print(pOS, "\t\treturn nStatus;\n");
					Print(pOS, "\t}\n");
					Print(pOS, "\tpEnv    = CX_CB_Struct_GetEnvironment_Ex(pThis, CX_SB_OFFSETOF({1}_Ex, helper));\n", sFullName);
					Print(pOS, "\tpBuffer = CX_CB_Struct_GetBuffer_Ex(pThis, CX_SB_OFFSETOF({1}_Ex, helper));\n", sFullName);
					Print(pOS, "\n");
					Print(pOS, "\treturn {1}_Create(ppStruct, pEnv, pBuffer, pValue - (CX_Byte *)pBuffer->pData);\n", sMemberType);
					Print(pOS, "}\n");
					Print(pOS, "\n");
				}
				else
				if (Aggregate_StaticArray == iter->nAggregateType)
				{
					Print(pOS, "static CX_StatusCode {1}_Get{2}Count(struct _{1} *pThis)\n", sFullName, iter->sName);
					Print(pOS, "{{\n");
					Print(pOS, "\treturn {1};\n", iter->cItemsCount);
					Print(pOS, "}\n");
					Print(pOS, "\n");

					Print(pOS, "static CX_StatusCode {1}_Get{2}Item(struct _{1} *pThis, CX_CB_Size cIndex, {3} **ppStruct)\n", sFullName, iter->sName, sMemberType);
					Print(pOS, "{{\n");
					Print(pOS, "\tCX_Byte             *pValues;\n");
					Print(pOS, "\tCX_CB_Environment   *pEnv;\n");
					Print(pOS, "\tCX_CB_Buffer        *pBuffer;\n");
					Print(pOS, "\tCX_StatusCode       nStatus;\n");
					Print(pOS, "\n");
					Print(pOS, "\tif ({1} <= cIndex)\n", iter->cItemsCount);
					Print(pOS, "\t{{\n");
					Print(pOS, "\t\treturn CX_Status_InvalidArg;\n");
					Print(pOS, "\t}\n");
					Print(pOS, "\tif (CXNOK(nStatus = CX_CB_Struct_GetStatic_Ex(pThis, CX_SB_OFFSETOF({1}_Ex, helper), {2}, &pValues, {3})))\n", sFullName, iter->cbOffset, iter->cbSize);
					Print(pOS, "\t{{\n");
					Print(pOS, "\t\treturn nStatus;\n");
					Print(pOS, "\t}\n");
					Print(pOS, "\tpEnv    = CX_CB_Struct_GetEnvironment_Ex(pThis, CX_SB_OFFSETOF({1}_Ex, helper));\n", sFullName);
					Print(pOS, "\tpBuffer = CX_CB_Struct_GetBuffer_Ex(pThis, CX_SB_OFFSETOF({1}_Ex, helper));\n", sFullName);
					Print(pOS, "\n");
					Print(pOS, "\treturn {1}_Create(ppStruct, pEnv, pBuffer, pValues + cIndex * {2} - (CX_Byte *)pBuffer->pData);\n", sMemberType, iterEx->second.cbSize);
					Print(pOS, "}\n");
					Print(pOS, "\n");
				}
				else
				if (Aggregate_Array == iter->nAggregateType)
				{
					Print(pOS, "static CX_CB_Size {1}_Get{2}Count(struct _{1} *pThis)\n", sFullName, iter->sName);
					Print(pOS, "{{\n");
					Print(pOS, "\tCX_CB_Size      cCount;\n", iter->sActualTypeName);
					Print(pOS, "\tCX_StatusCode   nStatus;\n");
					Print(pOS, "\n");
					Print(pOS, "\tif (CXNOK(nStatus = CX_CB_Struct_GetArrayItemCount_Ex(pThis, CX_SB_OFFSETOF({1}_Ex, helper), {2}, &cCount)))\n", sFullName, iter->cbOffset);
					Print(pOS, "\t{{\n");
					Print(pOS, "\t\treturn 0;\n");
					Print(pOS, "\t}\n");
					Print(pOS, "\n");
					Print(pOS, "\treturn cCount;\n");
					Print(pOS, "}\n");
					Print(pOS, "\n");

					Print(pOS, "static CX_StatusCode {1}_Set{2}Count(struct _{1} *pThis, CX_CB_Size cCount)\n", sFullName, iter->sName);
					Print(pOS, "{{\n");
					Print(pOS, "\treturn CX_CB_Struct_SetArrayItemCount_Ex(pThis, CX_SB_OFFSETOF({1}_Ex, helper), {2}, {3}, cCount);\n", sFullName, iter->cbOffset, iterEx->second.cbSize);
					Print(pOS, "}\n");
					Print(pOS, "\n");

					Print(pOS, "static CX_StatusCode {1}_Get{2}Item(struct _{1} *pThis, CX_CB_Size cIndex, {3} **ppStruct)\n", sFullName, iter->sName, sMemberType);
					Print(pOS, "{{\n");
					Print(pOS, "\tCX_Byte             *pValues;\n");
					Print(pOS, "\tCX_CB_Environment   *pEnv;\n");
					Print(pOS, "\tCX_CB_Buffer        *pBuffer;\n");
					Print(pOS, "\tCX_CB_Size          cCount;\n", iter->sActualTypeName);
					Print(pOS, "\tCX_StatusCode       nStatus;\n");
					Print(pOS, "\n");
					Print(pOS, "\tif (CXNOK(nStatus = CX_CB_Struct_GetArrayItemCount_Ex(pThis, CX_SB_OFFSETOF({1}_Ex, helper), {2}, &cCount)))\n", sFullName, iter->cbOffset);
					Print(pOS, "\t{{\n");
					Print(pOS, "\t\treturn nStatus;\n");
					Print(pOS, "\t}\n");
					Print(pOS, "\tif (cCount <= cIndex)\n");
					Print(pOS, "\t{{\n");
					Print(pOS, "\t\treturn CX_Status_OutOfBounds;\n");
					Print(pOS, "\t}\n");
					Print(pOS, "\tif (CXNOK(nStatus = CX_CB_Struct_GetArrayItems_Ex(pThis, CX_SB_OFFSETOF({1}_Ex, helper), {2}, &pValues)))\n", sFullName, iter->cbOffset);
					Print(pOS, "\t{{\n");
					Print(pOS, "\t\treturn nStatus;\n");
					Print(pOS, "\t}\n");
					Print(pOS, "\tpEnv    = CX_CB_Struct_GetEnvironment_Ex(pThis, CX_SB_OFFSETOF({1}_Ex, helper));\n", sFullName);
					Print(pOS, "\tpBuffer = CX_CB_Struct_GetBuffer_Ex(pThis, CX_SB_OFFSETOF({1}_Ex, helper));\n", sFullName);
					Print(pOS, "\n");
					Print(pOS, "\treturn {1}_Create(ppStruct, pEnv, pBuffer, pValues + cIndex * {2} - (CX_Byte *)pBuffer->pData);\n", sMemberType, iterEx->second.cbSize);
					Print(pOS, "}\n");
					Print(pOS, "\n");
				}
				else
				{
					return CX_Status_InvalidArg;
				}
			}
			else
			{
				if (Aggregate_Scalar == iter->nAggregateType)
				{
					Print(pOS, "static CX_StatusCode {1}_Set{2}(struct _{1} *pThis, {3} value)\n", sFullName, iter->sName, iter->sActualTypeName);
					Print(pOS, "{{\n");
					Print(pOS, "\treturn CX_CB_Struct_SetStatic_Ex(pThis, CX_SB_OFFSETOF({1}_Ex, helper), {2}, &value, sizeof({3}));\n", sFullName, iter->cbOffset, iter->sActualTypeName);
					Print(pOS, "}\n");
					Print(pOS, "\n");

					Print(pOS, "static {3} {1}_Get{2}(struct _{1} *pThis)\n", sFullName, iter->sName, iter->sActualTypeName);
					Print(pOS, "{{\n");
					Print(pOS, "\t{1}   *pValue;\n", iter->sActualTypeName);
					Print(pOS, "\tCX_StatusCode   nStatus;\n");
					Print(pOS, "\n");
					Print(pOS, "\tif (CXNOK(nStatus = CX_CB_Struct_GetStatic_Ex(pThis, CX_SB_OFFSETOF({1}_Ex, helper), {2}, &pValue, sizeof({3}))))\n", sFullName, iter->cbOffset, iter->sActualTypeName);
					Print(pOS, "\t{{\n");
					Print(pOS, "\t\treturn ({1})0;\n", iter->sActualTypeName);
					Print(pOS, "\t}\n");
					Print(pOS, "\n");
					Print(pOS, "\treturn *pValue;\n");
					Print(pOS, "}\n");
					Print(pOS, "\n");
				}
				else
				if (Aggregate_StaticArray == iter->nAggregateType)
				{
					Print(pOS, "static CX_StatusCode {1}_Get{2}Count(struct _{1} *pThis)\n", sFullName, iter->sName);
					Print(pOS, "{{\n");
					Print(pOS, "\treturn {1};\n", iter->cItemsCount);
					Print(pOS, "}\n");
					Print(pOS, "\n");

					Print(pOS, "static {3} *{1}_Get{2}(struct _{1} *pThis)\n", sFullName, iter->sName, iter->sActualTypeName);
					Print(pOS, "{{\n");
					Print(pOS, "\t{1}   *pValues;\n", iter->sActualTypeName);
					Print(pOS, "\tCX_StatusCode   nStatus;\n");
					Print(pOS, "\n");
					Print(pOS, "\tif (CXNOK(nStatus = CX_CB_Struct_GetStatic_Ex(pThis, CX_SB_OFFSETOF({1}_Ex, helper), {2}, &pValues, sizeof({3}))))\n", sFullName, iter->cbOffset, iter->sActualTypeName);
					Print(pOS, "\t{{\n");
					Print(pOS, "\t\treturn NULL;\n");
					Print(pOS, "\t}\n");
					Print(pOS, "\n");
					Print(pOS, "\treturn pValues;\n");
					Print(pOS, "}\n");
					Print(pOS, "\n");
				}
				else
				if (Aggregate_Array == iter->nAggregateType)
				{
					Print(pOS, "static CX_CB_Size {1}_Get{2}Count(struct _{1} *pThis)\n", sFullName, iter->sName);
					Print(pOS, "{{\n");
					Print(pOS, "\tCX_CB_Size      cCount;\n", iter->sActualTypeName);
					Print(pOS, "\tCX_StatusCode   nStatus;\n");
					Print(pOS, "\n");
					Print(pOS, "\tif (CXNOK(nStatus = CX_CB_Struct_GetArrayItemCount_Ex(pThis, CX_SB_OFFSETOF({1}_Ex, helper), {2}, &cCount)))\n", sFullName, iter->cbOffset);
					Print(pOS, "\t{{\n");
					Print(pOS, "\t\treturn 0;\n");
					Print(pOS, "\t}\n");
					Print(pOS, "\n");
					Print(pOS, "\treturn cCount;\n");
					Print(pOS, "}\n");
					Print(pOS, "\n");

					Print(pOS, "static CX_StatusCode {1}_Set{2}Count(struct _{1} *pThis, CX_CB_Size cCount)\n", sFullName, iter->sName);
					Print(pOS, "{{\n");
					Print(pOS, "\treturn CX_CB_Struct_SetArrayItemCount_Ex(pThis, CX_SB_OFFSETOF({1}_Ex, helper), {2}, sizeof({3}), cCount);\n", sFullName, iter->cbOffset, iter->sActualTypeName);
					Print(pOS, "}\n");
					Print(pOS, "\n");

					Print(pOS, "static {3} *{1}_Get{2}(struct _{1} *pThis)\n", sFullName, iter->sName, iter->sActualTypeName);
					Print(pOS, "{{\n");
					Print(pOS, "\t{1}   *pValues;\n", iter->sActualTypeName);
					Print(pOS, "\tCX_StatusCode   nStatus;\n");
					Print(pOS, "\n");
					Print(pOS, "\tif (CXNOK(nStatus = CX_CB_Struct_GetArrayItems_Ex(pThis, CX_SB_OFFSETOF({1}_Ex, helper), {2}, &pValues)))\n", sFullName, iter->cbOffset);
					Print(pOS, "\t{{\n");
					Print(pOS, "\t\treturn NULL;\n");
					Print(pOS, "\t}\n");
					Print(pOS, "\n");
					Print(pOS, "\treturn pValues;\n");
					Print(pOS, "}\n");
					Print(pOS, "\n");
				}
				else
				{
					return CX_Status_InvalidArg;
				}
			}
		}

		Print(pOS, "static CX_StatusCode {1}_Destroy(struct _{1} *pThis)\n", sFullName);
		Print(pOS, "{{\n");
		Print(pOS, "\t{1}_Ex   *pStruct;\n", sFullName);
		Print(pOS, "\tCX_CB_Environment   *pEnv;\n");
		Print(pOS, "\n");
		Print(pOS, "\tif (NULL == pThis)\n");
		Print(pOS, "\t{{\n");
		Print(pOS, "\t\treturn CX_Status_InvalidArg;\n");
		Print(pOS, "\t}\n");
		Print(pOS, "\tpStruct   = ({1}_Ex *)pThis;\n", sFullName);
		Print(pOS, "\tpEnv = pStruct->helper.pEnv;\n");
		Print(pOS, "\tpEnv->MemFree(pEnv, pStruct);\n");
		Print(pOS, "\n");
		Print(pOS, "\treturn CX_Status_OK;\n");
		Print(pOS, "}\n");
		Print(pOS, "\n");

		Print(pOS, "CX_StatusCode {1}_Create({1} **ppStruct, CX_CB_Environment *pEnv, CX_CB_Buffer *pBuffer, CX_CB_Size cbStartOffset)\n", sFullName);
		Print(pOS, "{{\n");
		Print(pOS, "\t{1}_Ex   *pStruct;\n", sFullName);
		Print(pOS, "\tCX_StatusCode   nStatus;\n");
		Print(pOS, "\n");
		Print(pOS, "\tif (NULL == *ppStruct || NULL == pBuffer || NULL == pEnv)\n");
		Print(pOS, "\t{{\n");
		Print(pOS, "\t\treturn CX_Status_InvalidArg;\n");
		Print(pOS, "\t}\n");
		Print(pOS, "\tif (NULL == (pStruct = ({1}_Ex *)pEnv->MemAlloc(pEnv, sizeof({1}_Ex))))\n", sFullName);
		Print(pOS, "\t{{\n");
		Print(pOS, "\t\treturn CX_Status_MemAllocFailed;\n");
		Print(pOS, "\t}\n");
		Print(pOS, "\n");
		Print(pOS, "\tif (CXNOK(nStatus = CX_CB_Struct_Init(&pStruct->helper, pEnv, pBuffer, cbStartOffset)))\n");
		Print(pOS, "\t{{\n");
		Print(pOS, "\t\tpEnv->MemFree(pEnv, pStruct);\n");
		Print(pOS, "\t\n");
		Print(pOS, "\t\treturn nStatus;\n");
		Print(pOS, "\t}\n");
		Print(pOS, "\tif (cbStartOffset == pBuffer->cbUsedSize)\n");
		Print(pOS, "\t{{\n");
		Print(pOS, "\t\tif (CXNOK(nStatus = CX_CB_Buffer_AddSize(pBuffer, {1}, pEnv)))\n", strct.cbSize);
		Print(pOS, "\t\t{{\n");
		Print(pOS, "\t\t\tpEnv->MemFree(pEnv, pStruct);\n");
		Print(pOS, "\t\t\n");
		Print(pOS, "\t\t\treturn nStatus;\n");
		Print(pOS, "\t\t}\n");
		Print(pOS, "\tpEnv->MemSet(pEnv, (CX_Byte *)pBuffer->pData + cbStartOffset, 0, {1});\n", strct.cbSize);
		Print(pOS, "\t}\n");
		for (auto iter = strct.vectorMembers.begin(); iter != strct.vectorMembers.end(); ++iter)
		{
			String sMemberType;

			for (Size i = 0; i < iter->sTypeName.size(); i++)
			{
				if ('.' == iter->sTypeName[i])
				{
					sMemberType += '_';
				}
				else
				{
					sMemberType += iter->sTypeName[i];
				}
			}

			if (Type_Struct == iter->nType)
			{
				if (Aggregate_Scalar == iter->nAggregateType)
				{
					Print(pOS, "\tpStruct->base.Get{2} = {1}_Get{2};\n", sFullName, iter->sName);
				}
				else
				if (Aggregate_StaticArray == iter->nAggregateType)
				{
					Print(pOS, "\tpStruct->base.Get{2}Count = {1}_Get{2}Count;\n", sFullName, iter->sName);
					Print(pOS, "\tpStruct->base.Get{2}Item = {1}_Get{2}Item;\n", sFullName, iter->sName);
				}
				else
				if (Aggregate_Array == iter->nAggregateType)
				{
					Print(pOS, "\tpStruct->base.Get{2}Count = {1}_Get{2}Count;\n", sFullName, iter->sName);
					Print(pOS, "\tpStruct->base.Set{2}Count = {1}_Set{2}Count;\n", sFullName, iter->sName);
					Print(pOS, "\tpStruct->base.Get{2}Item = {1}_Get{2}Item;\n", sFullName, iter->sName);
				}
				else
				{
					return CX_Status_InvalidArg;
				}
			}
			else
			{
				if (Aggregate_Scalar == iter->nAggregateType)
				{
					Print(pOS, "\tpStruct->base.Set{2} = {1}_Set{2};\n", sFullName, iter->sName);
					Print(pOS, "\tpStruct->base.Get{2} = {1}_Get{2};\n", sFullName, iter->sName);
				}
				else
				if (Aggregate_StaticArray == iter->nAggregateType)
				{
					Print(pOS, "\tpStruct->base.Get{2}Count = {1}_Get{2}Count;\n", sFullName, iter->sName);
					Print(pOS, "\tpStruct->base.Get{2} = {1}_Get{2};\n", sFullName, iter->sName);
				}
				else
				if (Aggregate_Array == iter->nAggregateType)
				{
					Print(pOS, "\tpStruct->base.Get{2}Count = {1}_Get{2}Count;\n", sFullName, iter->sName);
					Print(pOS, "\tpStruct->base.Set{2}Count = {1}_Set{2}Count;\n", sFullName, iter->sName);
					Print(pOS, "\tpStruct->base.Get{2} = {1}_Get{2};\n", sFullName, iter->sName);
				}
				else
				{
					return CX_Status_InvalidArg;
				}
			}
		}
		Print(pOS, "\tpStruct->base.Destroy = &{1}_Destroy;\n", sFullName);
		Print(pOS, "\n");
		Print(pOS, "\t*ppStruct = ({1} *)pStruct;\n", sFullName);
		Print(pOS, "\n");
		Print(pOS, "\treturn CX_Status_OK;\n");
		Print(pOS, "}\n");
		Print(pOS, "\n");

		return Status();
	}

};

}//namespace Tools

}//namespace CBuffers

}//namespace CX
