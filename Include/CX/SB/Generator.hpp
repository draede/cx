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

#pragma once


#include "CX/Types.hpp"
#include "CX/IO/FileOutputStream.hpp"
#include "CX/Print.hpp"
#include "CX/SB/Object.hpp"
#include "CX/Util/RndGen.hpp"


namespace CX
{

namespace SB
{

class Generator
{
public:

	static Status GenerateWithPath(const Object &obj, const Char *szPath)
	{
		IO::FileOutputStream fos(szPath);

		if (!fos.IsOK())
		{
			return Status(Status_CreateFailed, "Failed to create file '{1}'", szPath);
		}

		return Generate((IO::IOutputStream *)&fos, obj);
	}

	template <typename OUTPUT>
	static Status Generate(OUTPUT out, const Object &obj)
	{
		Status status;

		Print(out, "\n");
		Print(out, "#pragma once\n");
		Print(out, "\n");
		Print(out, "\n");
		Print(out, "#include \"CX/SB/Types.hpp\"\n");
		Print(out, "#include \"CX/SB/Comparators.hpp\"\n");
		Print(out, "#include \"CX/SB/Hashers.hpp\"\n");
		Print(out, "#include \"CX/SB/DefIniters.hpp\"\n");
		Print(out, "#include \"CX/SB/Readers.hpp\"\n");
		Print(out, "#include \"CX/SB/Writers.hpp\"\n");
		GenerateCPPIncludes(out, obj);
		Print(out, "\n");
		Print(out, "\n");
		if ((status = GenerateCPPClass(out, obj)).IsNOK())
		{
			return status;
		}
		Print(out, "\n");
		Print(out, "namespace CX\n");
		Print(out, "{{\n");
		Print(out, "\n");
		Print(out, "namespace SB\n");
		Print(out, "{{\n");
		Print(out, "\n");
		if ((status = GenerateCPPDefInitFunc(out, obj)).IsNOK())
		{
			return status;
		}
		Print(out, "\n");
		if ((status = GenerateCPPCompareFunc(out, obj)).IsNOK())
		{
			return status;
		}
		Print(out, "\n");
		if ((status = GenerateCPPHashFunc(out, obj)).IsNOK())
		{
			return status;
		}
		Print(out, "\n");
		if ((status = GenerateCPPWriteFunc(out, obj)).IsNOK())
		{
			return status;
		}
		Print(out, "\n");
		if ((status = GenerateCPPReadFunc(out, obj)).IsNOK())
		{
			return status;
		}
		Print(out, "\n");
		Print(out, "}//namespace SB\n");
		Print(out, "\n");
		Print(out, "}//namespace CX\n");
		Print(out, "\n");

		return Status();
	}

	static Status GenerateProtoWithPath(const Object &obj, const Char *szPath)
	{
		IO::FileOutputStream fos(szPath);

		if (!fos.IsOK())
		{
			return Status(Status_CreateFailed, "Failed to create file '{1}'", szPath);
		}

		return GenerateProto((IO::IOutputStream *)&fos, obj);
	}

	template <typename OUTPUT>
	static Status GenerateProto(OUTPUT out, const Object &obj)
	{
		Status status;

		Print(out, "\n");
		Print(out, "object {1}\n", obj.m_sName);
		Print(out, "{{\n");
		for (MembersVector::const_iterator iter = obj.m_vectorMembers.begin(); iter != obj.m_vectorMembers.end(); ++iter)
		{
			Print(out, "\t{1}: {2};\n", iter->m_sName, GetProtoType(*iter));
		}
		Print(out, "}\n");
		Print(out, "\n");

		return Status();
	}

	static Status GenerateTestWithPath(const Object &obj, const Char *szPath)
	{
		IO::FileOutputStream fos(szPath);

		if (!fos.IsOK())
		{
			return Status(Status_CreateFailed, "Failed to create file '{1}'", szPath);
		}

		return GenerateTest((IO::IOutputStream *)&fos, obj);
	}

	template <typename OUTPUT>
	static Status GenerateTest(OUTPUT out, const Object &obj)
	{
		Vector<String>::Type vectorNSs;
		String               sName;
		Status               status;

		ExtractNameSpaces(obj.m_sName.c_str(), vectorNSs, sName);

		Print(out, "\n");
		Print(out, "#pragma once\n");
		Print(out, "\n");
		Print(out, "\n");
		Print(out, "#include \"{1}.hpp\"\n", sName);
		Print(out, "#include \"CX/SB/TestIniters.hpp\"\n");
		Print(out, "\n");
		Print(out, "\n");
		Print(out, "namespace CX\n");
		Print(out, "{{\n");
		Print(out, "\n");
		Print(out, "namespace SB\n");
		Print(out, "{{\n");
		Print(out, "\n");
		if ((status = GenerateCPPTestInitFunc(out, obj)).IsNOK())
		{
			return status;
		}
		Print(out, "\n");
		Print(out, "}//namespace SB\n");
		Print(out, "\n");
		Print(out, "}//namespace CX\n");
		Print(out, "\n");

		return Status();
	}

private:

	static const Size MIN_STRING_LEN  = 0;
	static const Size MAX_STRING_LEN  = 40;
	static const Size MIN_ITEMS_COUNT = 0;
	static const Size MAX_ITEMS_COUNT = 20;

	static const Char *GetCharset()
	{
		return "!\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~";
	}

	Generator()
	{
	}

	~Generator()
	{
	}

	static const Char *GetCPPScalarType(const Char *szStr)
	{
		if (0 == cx_strcmp(szStr, "bool"))
		{
			return "CX::Bool";
		}
		else
		if (0 == cx_strcmp(szStr, "int8"))
		{
			return "CX::Int8";
		}
		else
		if (0 == cx_strcmp(szStr, "uint8"))
		{
			return "CX::UInt8";
		}
		else
		if (0 == cx_strcmp(szStr, "int16"))
		{
			return "CX::Int16";
		}
		else
		if (0 == cx_strcmp(szStr, "uint16"))
		{
			return "CX::UInt16";
		}
		else
		if (0 == cx_strcmp(szStr, "int32"))
		{
			return "CX::Int32";
		}
		else
		if (0 == cx_strcmp(szStr, "uint32"))
		{
			return "CX::UInt32";
		}
		else
		if (0 == cx_strcmp(szStr, "int64"))
		{
			return "CX::Int64";
		}
		else
		if (0 == cx_strcmp(szStr, "uint64"))
		{
			return "CX::UInt64";
		}
		else
		if (0 == cx_strcmp(szStr, "float"))
		{
			return "CX::Float";
		}
		else
		if (0 == cx_strcmp(szStr, "double"))
		{
			return "CX::Double";
		}
		else
		if (0 == cx_strcmp(szStr, "string"))
		{
			return "CX::String";
		}
		else
		if (0 == cx_strcmp(szStr, "wstring"))
		{
			return "CX::WString";
		}
		else
		{
			static String sType;
			const Char    *pPos;

			sType.clear();
			pPos = szStr;
			while (0 != *pPos)
			{
				if ('.' == *pPos)
				{
					sType += "::";
				}
				else
				{
					sType += *pPos;
				}
				pPos++;
			}

			return sType.c_str();
		}
	}

	static const Char *GetProtoType(const Member &member)
	{
		static String sType;

		sType.clear();
		if (Member::Type_HashMap == member.m_nType)
		{
			Print(&sType, "hashmap<{1}, {2}>", member.m_sKeyType.c_str(), member.m_sValType.c_str());

			return sType.c_str();
		}
		else
		if (Member::Type_HashSet == member.m_nType)
		{
			Print(&sType, "hashset<{1}>", member.m_sKeyType.c_str());

			return sType.c_str();
		}
		else
		if (Member::Type_Map == member.m_nType)
		{
			Print(&sType, "map<{1}, {2}>", member.m_sKeyType.c_str(), member.m_sValType.c_str());

			return sType.c_str();
		}
		else
		if (Member::Type_Set == member.m_nType)
		{
			Print(&sType, "set<{1}>", member.m_sKeyType.c_str());

			return sType.c_str();
		}
		else
		if (Member::Type_Vector == member.m_nType)
		{
			Print(&sType, "vector<{1}>", member.m_sValType.c_str());

			return sType.c_str();
		}
		else
		{
			return member.m_sValType.c_str();
		}
	}

	static const Char *GetCPPType(const Member &member)
	{
		static String sType;

		sType.clear();
		if (Member::Type_HashMap == member.m_nType)
		{
			Print(&sType, "CX::SB::HashMap<{1}, {2}>::Type", GetCPPScalarType(member.m_sKeyType.c_str()), 
			      GetCPPScalarType(member.m_sValType.c_str()));

			return sType.c_str();
		}
		else
		if (Member::Type_HashSet == member.m_nType)
		{
			Print(&sType, "CX::SB::HashSet<{1}>::Type", GetCPPScalarType(member.m_sKeyType.c_str()));

			return sType.c_str();
		}
		else
		if (Member::Type_Map == member.m_nType)
		{
			Print(&sType, "CX::SB::Map<{1}, {2}>::Type", GetCPPScalarType(member.m_sKeyType.c_str()), 
			      GetCPPScalarType(member.m_sValType.c_str()));

			return sType.c_str();
		}
		else
		if (Member::Type_Set == member.m_nType)
		{
			Print(&sType, "CX::SB::Set<{1}>::Type", GetCPPScalarType(member.m_sKeyType.c_str()));

			return sType.c_str();
		}
		else
		if (Member::Type_Vector == member.m_nType)
		{
			Print(&sType, "CX::SB::Vector<{1}>::Type", GetCPPScalarType(member.m_sValType.c_str()));

			return sType.c_str();
		}
		else
		{
			return GetCPPScalarType(member.m_sValType.c_str());
		}
	}

	static const Char *GetRndInit(const Char *szStr)
	{
		static String sStr;

		sStr.clear();
		if (0 == cx_strcmp(szStr, "bool"))
		{
			Print(&sStr, "{1}", Util::RndGen::Get().GetBool());
		}
		else
		if (0 == cx_strcmp(szStr, "int8"))
		{
			Print(&sStr, "{1}", Util::RndGen::Get().GetInt8());
		}
		else
		if (0 == cx_strcmp(szStr, "uint8"))
		{
			Print(&sStr, "{1}", Util::RndGen::Get().GetUInt8());
		}
		else
		if (0 == cx_strcmp(szStr, "int16"))
		{
			Print(&sStr, "{1}", Util::RndGen::Get().GetInt16());
		}
		else
		if (0 == cx_strcmp(szStr, "uint16"))
		{
			Print(&sStr, "{1}", Util::RndGen::Get().GetUInt16());
		}
		else
		if (0 == cx_strcmp(szStr, "int32"))
		{
			Print(&sStr, "{1}", Util::RndGen::Get().GetInt32());
		}
		else
		if (0 == cx_strcmp(szStr, "uint32"))
		{
			Print(&sStr, "{1}", Util::RndGen::Get().GetUInt32());
		}
		else
		if (0 == cx_strcmp(szStr, "int64"))
		{
			Print(&sStr, "{1}", Util::RndGen::Get().GetInt64());
		}
		else
		if (0 == cx_strcmp(szStr, "uint64"))
		{
			Print(&sStr, "{1}", Util::RndGen::Get().GetUInt64());
		}
		else
		if (0 == cx_strcmp(szStr, "float"))
		{
			Print(&sStr, "{1}f", Util::RndGen::Get().GetFloat());
		}
		else
		if (0 == cx_strcmp(szStr, "double"))
		{
			Print(&sStr, "{1}", Util::RndGen::Get().GetDouble());
		}
		else
		if (0 == cx_strcmp(szStr, "string"))
		{
			String sTmp;

			Util::RndGen::Get().GetString(&sTmp, MIN_STRING_LEN, MAX_STRING_LEN, GetCharset());
			sStr += "\"";
			for (Size i = 0; i < sTmp.size(); i++)
			{
				if ('"' == sTmp[i])
				{
					sStr += "\\\"";
				}
				else
				if ('\\' == sTmp[i])
				{
					sStr += "\\\\";
				}
				else
				{
					sStr += sTmp[i];
				}
			}
			sStr += "\"";
		}
		else
		if (0 == cx_strcmp(szStr, "wstring"))
		{
			String sTmp;

			Util::RndGen::Get().GetString(&sTmp, MIN_STRING_LEN, MAX_STRING_LEN, GetCharset());
			sStr += "L\"";
			for (Size i = 0; i < sTmp.size(); i++)
			{
				if ('"' == sTmp[i])
				{
					sStr += "\\\"";
				}
				else
				if ('\\' == sTmp[i])
				{
					sStr += "\\\\";
				}
				else
				{
					sStr += sTmp[i];
				}
			}
			sStr += "\"";

		}
		else
		{
			return NULL;
		}

		return sStr.c_str();
	}

	static bool IsSimpleType(const Char *szType)
	{
		if (0 == cx_strcmp(szType, "bool") || 
		    0 == cx_strcmp(szType, "int8") || 0 == cx_strcmp(szType, "uint8") || 
		    0 == cx_strcmp(szType, "int16") || 0 == cx_strcmp(szType, "uint16") || 
		    0 == cx_strcmp(szType, "int32") || 0 == cx_strcmp(szType, "uint32") || 
		    0 == cx_strcmp(szType, "int64") || 0 == cx_strcmp(szType, "uint64") || 
		    0 == cx_strcmp(szType, "float") || 0 == cx_strcmp(szType, "double") || 
		    0 == cx_strcmp(szType, "string") || 0 == cx_strcmp(szType, "wstring"))
		{
			return true;
		}
		
		return false;
	}

	static const Char *GetIncludePath(const Char *szType)
	{
		static String sPath;

		Vector<String>::Type vectorNSs;
		String               sName;

		ExtractNameSpaces(szType, vectorNSs, sName);

		sPath.clear();
		for (Vector<String>::Type::const_iterator iter = vectorNSs.begin(); iter != vectorNSs.end(); ++iter)
		{
			if (!sPath.empty())
			{
				sPath += "/";
			}
			sPath += *iter;
		}
		if (!sPath.empty())
		{
			sPath += "/";
		}
		sPath += sName;
		sPath += ".hpp";

		return sPath.c_str();
	}

	template <typename OUTPUT>
	static Status GenerateCPPIncludes(OUTPUT out, const Object &obj)
	{
		for (MembersVector::const_iterator iter = obj.m_vectorMembers.begin(); iter != obj.m_vectorMembers.end(); ++iter)
		{
			if (Member::Type_Scalar == iter->m_nType || Member::Type_Vector == iter->m_nType)
			{
				if (!IsSimpleType(iter->m_sValType.c_str()))
				{
					Print(out, "#include \"{1}\"\n", GetIncludePath(iter->m_sValType.c_str()));
				}
			}
			else
			if (Member::Type_Set == iter->m_nType || Member::Type_HashSet == iter->m_nType)
			{
				if (!IsSimpleType(iter->m_sKeyType.c_str()))
				{
					Print(out, "#include \"{1}\"\n", GetIncludePath(iter->m_sKeyType.c_str()));
				}
			}
			else
			if (Member::Type_Map == iter->m_nType || Member::Type_HashMap == iter->m_nType)
			{
				if (!IsSimpleType(iter->m_sKeyType.c_str()))
				{
					Print(out, "#include \"{1}\"\n", GetIncludePath(iter->m_sKeyType.c_str()));
				}
				if (!IsSimpleType(iter->m_sValType.c_str()))
				{
					Print(out, "#include \"{1}\"\n", GetIncludePath(iter->m_sValType.c_str()));
				}
			}
		}

		return Status();
	}

	static void ExtractNameSpaces(const Char *szType, Vector<String>::Type &vectorNSs, String &sName)
	{
		const Char   *pPos;
		const Char   *pStartPos;

		vectorNSs.clear();
		pPos = pStartPos = szType;
		while (0 != *pPos)
		{
			if ('.' == *pPos)
			{
				vectorNSs.push_back(String(pStartPos, pPos - pStartPos));
				pPos++;
				pStartPos = pPos;
			}
			else
			{
				pPos++;
			}
		}
		if (pStartPos < pPos)
		{
			vectorNSs.push_back(String(pStartPos, pPos - pStartPos));
		}

		Vector<String>::Type::iterator iterNS = vectorNSs.begin();

		iterNS += vectorNSs.size() - 1;
		sName = *iterNS;
		vectorNSs.erase(iterNS);
	}

	template <typename OUTPUT>
	static Status GenerateCPPClass(OUTPUT out, const Object &obj)
	{
		Vector<String>::Type vectorNSs;
		String               sName;

		ExtractNameSpaces(obj.m_sName.c_str(), vectorNSs, sName);
		for (Vector<String>::Type::iterator iter = vectorNSs.begin(); iter != vectorNSs.end(); ++iter)
		{
			Print(out, "namespace {1}\n", *iter);
			Print(out, "{{\n");
			Print(out, "\n");
		}
		Print(out, "class {1}\n", sName);
		Print(out, "{{\n");
		Print(out, "public:\n");
		Print(out, "\n");
		for (MembersVector::const_iterator iter = obj.m_vectorMembers.begin(); iter != obj.m_vectorMembers.end(); ++iter)
		{
			Print(out, "\t{1} {2};\n", GetCPPType(*iter), iter->m_sName);
		}
		Print(out, "\n");
		Print(out, "};\n");
		Print(out, "\n");
		for (Vector<String>::Type::reverse_iterator iter = vectorNSs.rbegin(); iter != vectorNSs.rend(); ++iter)
		{
			Print(out, "}//namespace {1}\n", *iter);
			Print(out, "\n");
		}

		return Status();
	}

	template <typename OUTPUT>
	static Status GenerateCPPCompareFunc(OUTPUT out, const Object &obj)
	{
		Print(out, "template <> static inline int Compare<{1}>(const {1} &a, const {1} &b)\n", GetCPPScalarType(obj.m_sName.c_str()));
		Print(out, "{{\n");
		Print(out, "\tint nCmp;\n");
		Print(out, "\n");
		for (MembersVector::const_iterator iter = obj.m_vectorMembers.begin(); iter != obj.m_vectorMembers.end(); ++iter)
		{
			Print(out, "\tif (0 != (nCmp = Compare(a.{1}, b.{1})))\n", iter->m_sName);
			Print(out, "\t{{\n");
			Print(out, "\t\treturn nCmp;\n");
			Print(out, "\t}\n");
		}
		Print(out, "\n");
		Print(out, "\treturn 0;\n");
		Print(out, "}\n");

		return Status();
	}

	template <typename OUTPUT>
	static Status GenerateCPPHashFunc(OUTPUT out, const Object &obj)
	{
		Print(out, "template <> static inline Size Hash<{1}>(const {1} &p, HasherHelper *pHasher)\n", GetCPPScalarType(obj.m_sName.c_str()));
		Print(out, "{{\n");
		Print(out, "\tHasherHelper hh;\n");
		Print(out, "\n");
		Print(out, "\tif (NULL == pHasher)\n");
		Print(out, "\t{{\n");
		Print(out, "\t\thh.Init();\n");
		Print(out, "\t\tpHasher = &hh;\n");
		Print(out, "\t}\n");
		for (MembersVector::const_iterator iter = obj.m_vectorMembers.begin(); iter != obj.m_vectorMembers.end(); ++iter)
		{
			Print(out, "\tHash(p.{1}, pHasher);\n", iter->m_sName);
		}
		Print(out, "\tif (&hh == pHasher)\n");
		Print(out, "\t{{\n");
		Print(out, "\t\treturn hh.Done();\n");
		Print(out, "\t}\n");
		Print(out, "\telse\n");
		Print(out, "\t{{\n");
		Print(out, "\t\treturn 0;\n");
		Print(out, "\t}\n");
		Print(out, "\n");
		Print(out, "\treturn true;\n");
		Print(out, "}\n");

		return Status();
	}

	template <typename OUTPUT>
	static Status GenerateCPPWriteFunc(OUTPUT out, const Object &obj)
	{
		Print(out, "template <> static inline Status Write<{1}>({1} p, IDataWriter *pDataWriter)\n", 
		      GetCPPScalarType(obj.m_sName.c_str()));
		Print(out, "{{\n");
		Print(out, "\tStatus status;\n");
		Print(out, "\n");
		Print(out, "\tif ((status = pDataWriter->BeginObject({1})).IsNOK())\n", obj.m_vectorMembers.size());
		Print(out, "\t{{\n");
		Print(out, "\t\treturn status;\n");
		Print(out, "\t}\n");
		for (MembersVector::const_iterator iter = obj.m_vectorMembers.begin(); iter != obj.m_vectorMembers.end(); ++iter)
		{
			Print(out, "\tif ((status = pDataWriter->WriteMember(\"{1}\", p.{1})).IsNOK())\n", iter->m_sName);
			Print(out, "\t{{\n");
			Print(out, "\t	return status;\n");
			Print(out, "\t}\n");
		}
		Print(out, "\tif ((status = pDataWriter->EndObject()).IsNOK())\n");
		Print(out, "\t{{\n");
		Print(out, "\t\treturn status;\n");
		Print(out, "\t}\n");
		Print(out, "\n");
		Print(out, "\treturn Status();\n");

		Print(out, "}\n");

		return Status();
	}

	template <typename OUTPUT>
	static Status GenerateCPPReadFunc(OUTPUT out, const Object &obj)
	{
		Print(out, "template <> static inline Status Read<{1}>({1} &p, IDataReader *pDataReader)\n", 
		      GetCPPScalarType(obj.m_sName.c_str()));
		Print(out, "{{\n");
		Print(out, "\tSize   cCount;\n");
		Print(out, "\tStatus status;\n");
		Print(out, "\n");
		Print(out, "\tDefInit(p);\n");
		Print(out, "\tif ((status = pDataReader->BeginObject(&cCount)).IsNOK())\n");
		Print(out, "\t{{\n");
		Print(out, "\t\treturn status;\n");
		Print(out, "\t}\n");
		Print(out, "\tif ({1} != cCount)\n", obj.m_vectorMembers.size());
		Print(out, "\t{{\n");
		Print(out, "\t\treturn Status(Status_InvalidArg, \"Expected {1} members\");\n", obj.m_vectorMembers.size());
		Print(out, "\t}\n");
		for (MembersVector::const_iterator iter = obj.m_vectorMembers.begin(); iter != obj.m_vectorMembers.end(); ++iter)
		{
			Print(out, "\tif ((status = pDataReader->ReadMember(\"{1}\", p.{1})).IsNOK())\n", iter->m_sName);
			Print(out, "\t{{\n");
			Print(out, "\t	return status;\n");
			Print(out, "\t}\n");
		}
		Print(out, "\tif ((status = pDataReader->EndObject()).IsNOK())\n");
		Print(out, "\t{{\n");
		Print(out, "\t\treturn status;\n");
		Print(out, "\t}\n");
		Print(out, "\n");
		Print(out, "\treturn Status();\n");

		Print(out, "}\n");

		return Status();
	}

	template <typename OUTPUT>
	static Status GenerateCPPDefInitFunc(OUTPUT out, const Object &obj)
	{
		Print(out, "static inline void DefInit({1} &p)\n", GetCPPScalarType(obj.m_sName.c_str()));
		Print(out, "{{\n");
		for (MembersVector::const_iterator iter = obj.m_vectorMembers.begin(); iter != obj.m_vectorMembers.end(); ++iter)
		{
			Print(out, "\tDefInit(p.{1});\n", iter->m_sName);
		}
		Print(out, "}\n");

		return Status();
	}

	template <typename OUTPUT>
	static Status GenerateCPPTestInitFunc(OUTPUT out, const Object &obj)
	{
		Print(out, "template <> static inline void TestInit<{1}>({1} &p)\n", 
		      GetCPPScalarType(obj.m_sName.c_str()));
		Print(out, "{{\n");
		for (MembersVector::const_iterator iter = obj.m_vectorMembers.begin(); iter != obj.m_vectorMembers.end(); ++iter)
		{
			if (Member::Type_Scalar == iter->m_nType)
			{
				const Char *szStr = GetRndInit(iter->m_sValType.c_str());

				if (NULL == szStr)
				{
					Print(out, "\tTestInit(p.{1});\n", iter->m_sName);
				}
				else
				{
					Print(out, "\tp.{1} = {2};\n", iter->m_sName, szStr);
				}
			}
			else
			if (Member::Type_Vector == iter->m_nType)
			{
				Size cCount = MIN_ITEMS_COUNT + (Util::RndGen::Get().GetSize() % (MAX_ITEMS_COUNT - MIN_ITEMS_COUNT + 1));

				for (Size i = 0; i < cCount; i++)
				{
					const Char *szStr = GetRndInit(iter->m_sValType.c_str());

					if (NULL == szStr)
					{
						Print(out, "\t{{\n");
						Print(out, "\t\t{1} k;\n", GetCPPScalarType(iter->m_sValType.c_str()));
						Print(out, "\n");
						Print(out, "\t\tTestInit(k);\n");
						Print(out, "\t\tp.{1}.push_back(k);\n", iter->m_sName);
						Print(out, "\t}\n");
					}
					else
					{
						Print(out, "\tp.{1}.push_back({2});\n", iter->m_sName, szStr);
					}
				}
			}
			else
			if (Member::Type_Set == iter->m_nType || Member::Type_HashSet == iter->m_nType)
			{
				Size cCount = MIN_ITEMS_COUNT + (Util::RndGen::Get().GetSize() % (MAX_ITEMS_COUNT - MIN_ITEMS_COUNT + 1));

				for (Size i = 0; i < cCount; i++)
				{
					const Char *szStr = GetRndInit(iter->m_sKeyType.c_str());

					if (NULL == szStr)
					{
						Print(out, "\t{{\n");
						Print(out, "\t\t{1} k;\n", GetCPPScalarType(iter->m_sKeyType.c_str()));
						Print(out, "\n");
						Print(out, "\t\tTestInit(k);\n");
						Print(out, "\t\tp.{1}.insert(k);\n", iter->m_sName);
						Print(out, "\t}\n");
					}
					else
					{
						Print(out, "\tp.{1}.insert({2});\n", iter->m_sName, szStr);
					}
				}
			}
			else
			if (Member::Type_Map == iter->m_nType || Member::Type_HashMap == iter->m_nType)
			{
				Size cCount = MIN_ITEMS_COUNT + (Util::RndGen::Get().GetSize() % (MAX_ITEMS_COUNT - MIN_ITEMS_COUNT + 1));

				for (Size i = 0; i < cCount; i++)
				{
					const Char *szStr;

					szStr = GetRndInit(iter->m_sKeyType.c_str());
					String sStrKey = NULL == szStr ? "" : szStr;
					szStr = GetRndInit(iter->m_sValType.c_str());
					String sStrVal = NULL == szStr ? "" : szStr;

					if (sStrKey.empty())
					{
						if (sStrVal.empty())
						{
							Print(out, "\t{{\n");
							Print(out, "\t\t{1} k;\n", GetCPPScalarType(iter->m_sKeyType.c_str()));
							Print(out, "\t\t{1} v;\n", GetCPPScalarType(iter->m_sValType.c_str()));
							Print(out, "\n");
							Print(out, "\t\tTestInit(k);\n");
							Print(out, "\t\tTestInit(v);\n");
							Print(out, "\t\tp.{1}[k] = v;\n", iter->m_sName);
							Print(out, "\t}\n");
						}
						else
						{
							Print(out, "\t{{\n");
							Print(out, "\t\t{1} k;\n", GetCPPScalarType(iter->m_sKeyType.c_str()));
							Print(out, "\n");
							Print(out, "\t\tTestInit(k);\n");
							Print(out, "\t\tp.{1}[k] = {2};\n", iter->m_sName, sStrVal);
							Print(out, "\t}\n");
						}
					}
					else
					{
						if (sStrVal.empty())
						{
							Print(out, "\t{{\n");
							Print(out, "\t\t{1} v;\n", GetCPPScalarType(iter->m_sValType.c_str()));
							Print(out, "\n");
							Print(out, "\t\tTestInit(v);\n");
							Print(out, "\t\tp.{1}[{2}] = v;\n", iter->m_sName, sStrKey);
							Print(out, "\t}\n");
						}
						else
						{
							Print(out, "\tp.{1}[{2}] = {3};\n", iter->m_sName, sStrKey, sStrVal);
						}
					}
				}
			}
		}
		Print(out, "}\n");

		return Status();
	}

};

}//namespace SB

}//namespace CX

