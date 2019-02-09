/* 
 * CX - C++ framework for general purpose development
 *
 * https://github.com/draede/cx
 * 
 * Copyright (C) 2014 - 2019 draede - draede [at] outlook [dot] com
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

class CPP_Generator : public IGenerator
{
public:

	static const Char *NAME()   { return "CPP"; }

	static const Char *ID()   { return "cpp"; }

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
		StringsVector   vectorNamespaces;
		String          sNamespace;
		String          sName;
		String          sHeaderPath;
		String          sSourcePath;
		const Char      *pszPos;
		Status          status;

		pszPos = strct.sName.c_str();
		for (Size i = 0; i < strct.sName.size(); i++)
		{
			if ('.' == strct.sName[i])
			{
				vectorNamespaces.push_back(sNamespace);
				sNamespace.clear();
				pszPos = strct.sName.c_str() + i + 1;
			}
			else
			{
				sNamespace += strct.sName[i];
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
		sHeaderPath += sName;
		sSourcePath += sName;
		sHeaderPath += ".hpp";
		sSourcePath += ".cpp";

		{
			IO::FileOutputStream   os(sHeaderPath.c_str());

			if (!os.IsOK())
			{
				return Status(Status_CreateFailed, "Failed to create '{1}'", sHeaderPath);
			}
			if (!(status = GenerateHeader(&os, vectorNamespaces, sName, strct, mapStructs)))
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
			if (!(status = GenerateSource(&os, vectorNamespaces, sName, strct, mapStructs)))
			{
				return status;
			}
		}

		return Status();
	}

	Status GenerateHeader(IO::IOutputStream *pOS, const StringsVector &vectorNamespaces, const String &sName, 
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
		Print(pOS, "#include \"CX/Types.hpp\"\n");
		Print(pOS, "#include \"CX/Status.hpp\"\n");
		Print(pOS, "#include \"CX/CBuffers/Buffer.h\"\n");
		Print(pOS, "#include \"CX/CBuffers/Environment.h\"\n");
		Print(pOS, "#include \"CX/CBuffers/Types.h\"\n");
		Print(pOS, "#include \"CX/CBuffers/Struct.h\"\n");
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

			const Char *pszPos = iter->sTypeName.c_str();
			for (Size i = 0; i < iter->sTypeName.size(); i++)
			{
				if ('.' == strct.sName[i])
				{
					pszPos = iter->sTypeName.c_str() + i + 1;
				}
			}
			
			String sMemberType = pszPos;

			setIncludes.insert(iter->sTypeName);

			Print(pOS, "#include \"{1}.hpp\"\n", sMemberType);
		}
		Print(pOS, "\n");
		Print(pOS, "\n");
		for (auto iter = vectorNamespaces.begin(); iter != vectorNamespaces.end(); ++iter)
		{
			Print(pOS, "namespace {1}\n", *iter);
			Print(pOS, "{{\n");
			Print(pOS, "\n");
		}
		Print(pOS, "class {1}\n", sName);
		Print(pOS, "{{\n");
		Print(pOS, "public:\n");
		Print(pOS, "\n");

		Print(pOS, "\t{1}();\n\n", sName);
		Print(pOS, "\t~{1}();\n\n", sName);
		Print(pOS, "\tstatic {1} *Create(CX_CB_Environment *pEnv, CX_CB_Buffer *pBuffer, CX_CB_Size cbStartOffset = 0);\n\n", sName);
		Print(pOS, "\tvoid Destroy();\n\n");

		for (auto iter = strct.vectorMembers.begin(); iter != strct.vectorMembers.end(); ++iter)
		{
			String sMemberType;

			for (Size i = 0; i < iter->sTypeName.size(); i++)
			{
				if ('.' == iter->sTypeName[i])
				{
					sMemberType += "::";
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
					Print(pOS, "\t//You must call Destroy on ppStruct when done\n");
					Print(pOS, "\tCX::Status Get{1}({2} **ppStruct);\n\n", iter->sName, sMemberType);
				}
				else
				if (Aggregate_StaticArray == iter->nAggregateType)
				{
					Print(pOS, "\tCX_CB_Size Get{1}Count();\n\n", iter->sName);
					Print(pOS, "\t//You must call Destroy on ppStruct when done\n");
					Print(pOS, "\tCX::Status Get{1}Item(CX_CB_Size cIndex, {2} **ppStruct);\n\n", iter->sName, sMemberType);
				}
				else
				if (Aggregate_Array == iter->nAggregateType)
				{
					Print(pOS, "\tCX_CB_Size Get{1}Count();\n\n", iter->sName);
					Print(pOS, "\tCX::Status Set{1}Count(CX_CB_Size cCount);\n\n", iter->sName);
					Print(pOS, "\t//You must call Destroy on ppStruct when done\n");
					Print(pOS, "\tCX::Status Get{1}Item(CX_CB_Size cIndex, {2} **ppStruct);\n\n", iter->sName, sMemberType);
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
					Print(pOS, "\tCX::Status Set{1}({2} value);\n\n", iter->sName, iter->sActualTypeName);
					Print(pOS, "\t{2} Get{1}();\n\n", iter->sName, iter->sActualTypeName);
				}
				else
				if (Aggregate_StaticArray == iter->nAggregateType)
				{
					Print(pOS, "\tCX_CB_Size Get{1}Count();\n\n", iter->sName);
					Print(pOS, "\t{2} *Get{1}();\n\n", iter->sName, iter->sActualTypeName);
				}
				else
				if (Aggregate_Array == iter->nAggregateType)
				{
					Print(pOS, "\tCX_CB_Size Get{1}Count();\n\n", iter->sName);
					Print(pOS, "\tCX::Status Set{1}Count(CX_CB_Size cCount);\n\n", iter->sName);
					Print(pOS, "\t{2} *Get{1}();\n\n", iter->sName, iter->sActualTypeName);
				}
				else
				{
					return Status_InvalidArg;
				}
			}
		}
		Print(pOS, "private:\n");
		Print(pOS, "\n");
		Print(pOS, "\tCX_CB_Struct   m_struct;\n");
		Print(pOS, "\n");
		Print(pOS, "\ttemplate <typename STRUCT>\n");
		Print(pOS, "\tstatic CX::Status CreateHelper(STRUCT **ppStruct, CX_CB_Environment *pEnv, CX_CB_Buffer *pBuffer, CX_CB_Size cbStartOffset/* = 0*/)\n", sName);
		Print(pOS, "\t{{\n");
		Print(pOS, "\t\tSTRUCT *pStruct;\n");
		Print(pOS, "\n");
		Print(pOS, "\t\tif (NULL == (pStruct = STRUCT::Create(pEnv, pBuffer, cbStartOffset)))\n");
		Print(pOS, "\t\t{{\n");
		Print(pOS, "\t\t\treturn CX::Status_MemAllocFailed;\n");
		Print(pOS, "\t\t}\n");
		Print(pOS, "\t*ppStruct = pStruct;\n");
		Print(pOS, "\n");
		Print(pOS, "\treturn CX::Status();\n");
		Print(pOS, "\t}\n");
		Print(pOS, "\n");
		Print(pOS, "};\n");
		Print(pOS, "\n");
		for (auto iter = vectorNamespaces.rbegin(); iter != vectorNamespaces.rend(); ++iter)
		{
			Print(pOS, "}//namespace {1}\n", *iter);
			Print(pOS, "\n");
		}

		return Status();
	}

	Status GenerateSource(IO::IOutputStream *pOS, const StringsVector &vectorNamespaces, 
	                                    const String &sName, const Struct &strct, const StructsMap &mapStructs)
	{
		CX_UNUSED(sName);

		Status   status;

		Print(pOS, "\n");
		Print(pOS, "#include \"{1}.hpp\"\n", sName);
		Print(pOS, "#include \"CX/CBuffers/Struct.h\"\n");
		Print(pOS, "#include <new>\n");
		Print(pOS, "\n");
		Print(pOS, "\n");

		for (auto iter = vectorNamespaces.begin(); iter != vectorNamespaces.end(); ++iter)
		{
			Print(pOS, "namespace {1}\n", *iter);
			Print(pOS, "{{\n");
			Print(pOS, "\n");
		}

		Print(pOS, "{1}::{1}()\n", sName);
		Print(pOS, "{{\n");
		Print(pOS, "\tm_struct.pEnv          = NULL;\n");
		Print(pOS, "\tm_struct.pBuffer       = NULL;\n");
		Print(pOS, "\tm_struct.cbStartOffset = 0;\n");
		Print(pOS, "}\n\n");

		Print(pOS, "{1}::~{1}()\n", sName);
		Print(pOS, "{{\n");
		Print(pOS, "}\n\n");

		Print(pOS, "{1} *{1}::Create(CX_CB_Environment *pEnv, CX_CB_Buffer *pBuffer, CX_CB_Size cbStartOffset/* = 0*/)\n", sName);
		Print(pOS, "{{\n");
		Print(pOS, "\t{1} *pStruct;\n", sName);
		Print(pOS, "\tCX::Status   status;\n");
		Print(pOS, "\n");
		Print(pOS, "\tif (NULL == (pStruct = new (std::nothrow) {1}()))\n", sName);
		Print(pOS, "\t{{\n");
		Print(pOS, "\t\treturn NULL;\n");
		Print(pOS, "\t}\n");
		Print(pOS, "\tif (!(status = CX_CB_Struct_Init(&pStruct->m_struct, pEnv, pBuffer, cbStartOffset)))\n");
		Print(pOS, "\t{{\n");
		Print(pOS, "\t\tdelete pStruct;\n");
		Print(pOS, "\n");
		Print(pOS, "\t\treturn NULL;\n");
		Print(pOS, "\t}\n");
		Print(pOS, "\tif (cbStartOffset == pBuffer->cbUsedSize)\n");
		Print(pOS, "\t{{\n");
		Print(pOS, "\t\tif (!(status = CX_CB_Buffer_AddSize(pBuffer, {1}, pEnv)))\n", strct.cbSize);
		Print(pOS, "\t\t{{\n");
		Print(pOS, "\t\t\tdelete pStruct;\n");
		Print(pOS, "\n");
		Print(pOS, "\t\t\treturn NULL;\n");
		Print(pOS, "\t\t}\n");
		Print(pOS, "\t\tpEnv->MemSet(pEnv, (CX_Byte *)pBuffer->pData + cbStartOffset, 0, {1});\n", strct.cbSize);
		Print(pOS, "\t}\n");
		Print(pOS, "\n");
		Print(pOS, "\treturn pStruct;\n");
		Print(pOS, "}\n\n");

		Print(pOS, "void {1}::Destroy()\n", sName);
		Print(pOS, "{{\n");
		Print(pOS, "\tdelete this;\n");
		Print(pOS, "}\n\n");

		for (auto iter = strct.vectorMembers.begin(); iter != strct.vectorMembers.end(); ++iter)
		{
			String sMemberType;

			for (Size i = 0; i < iter->sTypeName.size(); i++)
			{
				if ('.' == iter->sTypeName[i])
				{
					sMemberType += "::";
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
					Print(pOS, "CX::Status {1}::Get{2}({3} **ppStruct)\n", sName, iter->sName, sMemberType);
					Print(pOS, "{{\n");
					Print(pOS, "\tCX::Byte      *pValue;\n");
					Print(pOS, "\tCX::Status    status;\n");
					Print(pOS, "\n");
					Print(pOS, "\tif (!(status = CX_CB_Struct_GetStatic(&m_struct, {1}, (void **)&pValue, {2})))\n", iter->cbOffset, iter->cbSize);
					Print(pOS, "\t{{\n");
					Print(pOS, "\t\treturn status;\n");
					Print(pOS, "\t}\n");
					Print(pOS, "\n");
					Print(pOS, "\treturn CreateHelper(ppStruct, m_struct.pEnv, m_struct.pBuffer, pValue - (CX_Byte *)m_struct.pBuffer->pData);\n", sMemberType);
					Print(pOS, "}\n");
					Print(pOS, "\n");
				}
				else
				if (Aggregate_StaticArray == iter->nAggregateType)
				{
					Print(pOS, "CX_CB_Size {1}::Get{2}Count()\n", sName, iter->sName);
					Print(pOS, "{{\n");
					Print(pOS, "\treturn {1};\n", iter->cItemsCount);
					Print(pOS, "}\n");
					Print(pOS, "\n");

					Print(pOS, "CX::Status {1}::Get{2}Item(CX_CB_Size cIndex, {3} **ppStruct)\n", sName, iter->sName, sMemberType);
					Print(pOS, "{{\n");
					Print(pOS, "\tCX::Byte     *pValues;\n");
					Print(pOS, "\tCX::Status   status;\n");
					Print(pOS, "\n");
					Print(pOS, "\tif ({1} <= cIndex)\n", iter->cItemsCount);
					Print(pOS, "\t{{\n");
					Print(pOS, "\t\treturn CX::Status_InvalidArg;\n");
					Print(pOS, "\t}\n");
					Print(pOS, "\tif (!(status = CX_CB_Struct_GetStatic(&m_struct, {1}, (void **)&pValues, {2})))\n", iter->cbOffset, iter->cbSize);
					Print(pOS, "\t{{\n");
					Print(pOS, "\t\treturn status;\n");
					Print(pOS, "\t}\n");
					Print(pOS, "\n");
					Print(pOS, "\treturn CreateHelper(ppStruct, m_struct.pEnv, m_struct.pBuffer, pValues + cIndex * {2} - (CX_Byte *)m_struct.pBuffer->pData);\n", sMemberType, iterEx->second.cbSize);
					Print(pOS, "}\n");
					Print(pOS, "\n");
				}
				else
				if (Aggregate_Array == iter->nAggregateType)
				{
					Print(pOS, "CX_CB_Size {1}::Get{2}Count()\n", sName, iter->sName);
					Print(pOS, "{{\n");
					Print(pOS, "\tCX_CB_Size   cCount;\n");
					Print(pOS, "\tCX::Status   status;\n");
					Print(pOS, "\n");
					Print(pOS, "\tif (!(status = CX_CB_Struct_GetArrayItemCount(&m_struct, {1}, &cCount)))\n", iter->cbOffset);
					Print(pOS, "\t{{\n");
					Print(pOS, "\t\treturn 0;\n");
					Print(pOS, "\t}\n");
					Print(pOS, "\n");
					Print(pOS, "\treturn cCount;\n");
					Print(pOS, "}\n");
					Print(pOS, "\n");

					Print(pOS, "CX::Status {1}::Set{2}Count(CX_CB_Size cCount)\n", sName, iter->sName);
					Print(pOS, "{{\n");
					Print(pOS, "\treturn CX_CB_Struct_SetArrayItemCount(&m_struct, {1}, {2}, cCount);\n", iter->cbOffset, iterEx->second.cbSize);
					Print(pOS, "}\n");
					Print(pOS, "\n");

					Print(pOS, "CX::Status {1}::Get{2}Item(CX_CB_Size cIndex, {3} **ppStruct)\n", sName, iter->sName, sMemberType);
					Print(pOS, "{{\n");
					Print(pOS, "\tCX::Byte     *pValues;\n");
					Print(pOS, "\tCX_CB_Size   cCount;\n");
					Print(pOS, "\tCX::Status   status;\n");
					Print(pOS, "\n");
					Print(pOS, "\tif (!(status = CX_CB_Struct_GetArrayItemCount(&m_struct, {1}, &cCount)))\n", iter->cbOffset);
					Print(pOS, "\t{{\n");
					Print(pOS, "\t\treturn status;\n");
					Print(pOS, "\t}\n");
					Print(pOS, "\tif (cCount <= cIndex)\n");
					Print(pOS, "\t{{\n");
					Print(pOS, "\t\treturn CX::Status_OutOfBounds;\n");
					Print(pOS, "\t}\n");
					Print(pOS, "\tif (!(status = CX_CB_Struct_GetArrayItems(&m_struct, {1}, (void **)&pValues)))\n", iter->cbOffset);
					Print(pOS, "\t{{\n");
					Print(pOS, "\t\treturn status;\n");
					Print(pOS, "\t}\n");
					Print(pOS, "\n");
					Print(pOS, "\treturn CreateHelper(ppStruct, m_struct.pEnv, m_struct.pBuffer, pValues + cIndex * {2} - (CX_Byte *)m_struct.pBuffer->pData);\n", sMemberType, iterEx->second.cbSize);
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
					Print(pOS, "CX::Status {1}::Set{2}({3} value)\n", sName, iter->sName, iter->sActualTypeName);
					Print(pOS, "{{\n");
					Print(pOS, "\treturn CX_CB_Struct_SetStatic(&m_struct, {1}, &value, sizeof({2}));\n", iter->cbOffset, iter->sActualTypeName);
					Print(pOS, "}\n");
					Print(pOS, "\n");

					Print(pOS, "{3} {1}::Get{2}()\n", sName, iter->sName, iter->sActualTypeName);
					Print(pOS, "{{\n");
					Print(pOS, "\t{1}   *pValue;\n", iter->sActualTypeName);
					Print(pOS, "\tCX::Status   status;\n");
					Print(pOS, "\n");
					Print(pOS, "\tif (!(status = CX_CB_Struct_GetStatic(&m_struct, {1}, (void **)&pValue, sizeof({2}))))\n", iter->cbOffset, iter->sActualTypeName);
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
					Print(pOS, "CX_CB_Size {1}::Get{2}Count()\n", sName, iter->sName);
					Print(pOS, "{{\n");
					Print(pOS, "\treturn {1};\n", iter->cItemsCount);
					Print(pOS, "}\n");
					Print(pOS, "\n");

					Print(pOS, "{3} *{1}::Get{2}()\n", sName, iter->sName, iter->sActualTypeName);
					Print(pOS, "{{\n");
					Print(pOS, "\t{1}   *pValues;\n", iter->sActualTypeName);
					Print(pOS, "\tCX::Status   status;\n");
					Print(pOS, "\n");
					Print(pOS, "\tif (!(status = CX_CB_Struct_GetStatic(&m_struct, {1}, (void **)&pValues, sizeof({2}))))\n", iter->cbOffset, iter->sActualTypeName);
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
					Print(pOS, "CX_CB_Size {1}::Get{2}Count()\n", sName, iter->sName);
					Print(pOS, "{{\n");
					Print(pOS, "\tCX_CB_Size   cCount;\n");
					Print(pOS, "\tCX::Status   status;\n");
					Print(pOS, "\n");
					Print(pOS, "\tif (!(status = CX_CB_Struct_GetArrayItemCount(&m_struct, {1}, &cCount)))\n", iter->cbOffset);
					Print(pOS, "\t{{\n");
					Print(pOS, "\t\treturn 0;\n");
					Print(pOS, "\t}\n");
					Print(pOS, "\n");
					Print(pOS, "\treturn cCount;\n");
					Print(pOS, "}\n");
					Print(pOS, "\n");

					Print(pOS, "CX::Status {1}::Set{2}Count(CX_CB_Size cCount)\n", sName, iter->sName);
					Print(pOS, "{{\n");
					Print(pOS, "\treturn CX_CB_Struct_SetArrayItemCount(&m_struct, {1}, sizeof({2}), cCount);\n", iter->cbOffset, iter->sActualTypeName);
					Print(pOS, "}\n");
					Print(pOS, "\n");

					Print(pOS, "{3} *{1}::Get{2}()\n", sName, iter->sName, iter->sActualTypeName);
					Print(pOS, "{{\n");
					Print(pOS, "\t{1}   *pValues;\n", iter->sActualTypeName);
					Print(pOS, "\tCX::Status   status;\n");
					Print(pOS, "\n");
					Print(pOS, "\tif (!(status = CX_CB_Struct_GetArrayItems(&m_struct, {1}, (void **)&pValues)))\n", iter->cbOffset);
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

		for (auto iter = vectorNamespaces.rbegin(); iter != vectorNamespaces.rend(); ++iter)
		{
			Print(pOS, "}//namespace {1}\n", *iter);
			Print(pOS, "\n");
		}

		return Status();
	}

};

}//namespace Tools

}//namespace CBuffers

}//namespace CX
