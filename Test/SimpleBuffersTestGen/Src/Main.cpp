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

#include "CX/Util/RndGen.hpp"
#include "CX/SB/Object.hpp"
#include "CX/SB/Generator.hpp"
#include "CX/SB/Parser.hpp"
#include "CX/C/ctype.h"


using namespace CX;


const Char *GetSimpleType()
{
	UInt32 nType = Util::RndGen::Get().GetUInt32() % 16;

	switch (nType)
	{
		case 0: return "bool";
		case 1: return "int8";
		case 2: return "uint8";
		case 3: return "int16";
		case 4: return "uint16";
		case 5: return "int32";
		case 6: return "uint32";
		case 7: return "int64";
		case 8: return "uint64";
		case 9: return "float";
		case 10: return "double";
		case 11: return "string";
		case 12: return "wstring";
	}

	return "";
}

const Char *GetType(const SB::ObjectsMap &mapSimpleObjects, const SB::ObjectsMap &mapComplexObjects, 
                    bool bForceBase, bool bForceUseObj, bool bUseComplex)
{
	bool bBase;
	bool bComplex;

	if (bForceBase)
	{
		bBase    = true;
		bComplex = false;
	}
	else
	{
		if (bForceUseObj)
		{
			if (bUseComplex)
			{
				if (!mapComplexObjects.empty())
				{
					bBase    = false;
					bComplex = true;
				}
				else
				if (!mapSimpleObjects.empty())
				{
					bBase    = false;
					bComplex = false;
				}
				else
				{
					bBase    = true;
					bComplex = false;
				}
			}
			else
			{
				if (!mapSimpleObjects.empty())
				{
					bBase    = false;
					bComplex = false;
				}
				else
				if (!mapComplexObjects.empty())
				{
					bBase    = false;
					bComplex = true;
				}
				else
				{
					bBase    = true;
					bComplex = false;
				}
			}
		}
		else
		{
			bBase = Util::RndGen::Get().GetBool();
			if (bBase)
			{
				bComplex = false;
			}
			else
			{
				if (!mapSimpleObjects.empty())
				{
					bComplex = false;
				}
				else
				if (!mapComplexObjects.empty())
				{
					bComplex = true;
				}
				else
				{
					bBase    = true;
					bComplex = false;
				}
			}
		}
	}

	if (bBase)
	{
		return GetSimpleType();
	}
	else
	{
		if (bComplex)
		{
			SB::ObjectsMap::const_iterator iter   = mapComplexObjects.begin();
			Size                           cIndex = Util::RndGen::Get().GetSize() % mapComplexObjects.size();

			for (Size i = 0; i < cIndex; i++)
			{
				++iter;
			}

			return iter->second.m_sName.c_str();
		}
		else
		{
			SB::ObjectsMap::const_iterator iter   = mapSimpleObjects.begin();
			Size                           cIndex = Util::RndGen::Get().GetSize() % mapSimpleObjects.size();

			for (Size i = 0; i < cIndex; i++)
			{
				++iter;
			}

			return iter->second.m_sName.c_str();
		}
	}

	UInt32 nType = Util::RndGen::Get().GetUInt32() % 16;

	switch (nType)
	{
		case 0: return "bool";
		case 1: return "int8";
		case 2: return "uint8";
		case 3: return "int16";
		case 4: return "uint16";
		case 5: return "int32";
		case 6: return "uint32";
		case 7: return "int64";
		case 8: return "uint64";
		case 9: return "float";
		case 10: return "double";
		case 11: return "string";
		case 12: return "wstring";
	}

	return "";
}

bool IsSimpleType(const Char *szType)
{
	if (0 == cx_strcmp(szType, "bool")) return true;
	else if (0 == cx_strcmp(szType, "int8")) return true;
	else if (0 == cx_strcmp(szType, "uint8")) return true;
	else if (0 == cx_strcmp(szType, "int16")) return true;
	else if (0 == cx_strcmp(szType, "uint16")) return true;
	else if (0 == cx_strcmp(szType, "int32")) return true;
	else if (0 == cx_strcmp(szType, "uint32")) return true;
	else if (0 == cx_strcmp(szType, "int64")) return true;
	else if (0 == cx_strcmp(szType, "uint64")) return true;
	else if (0 == cx_strcmp(szType, "float")) return true;
	else if (0 == cx_strcmp(szType, "double")) return true;
	else if (0 == cx_strcmp(szType, "string")) return true;
	else if (0 == cx_strcmp(szType, "wstring")) return true;
	else return false;
}

void GenerateObject(SB::ObjectsMap &mapSimpleObjects, SB::ObjectsMap &mapComplexObjects,
                    CX::Size cMinFields, CX::Size cMaxFields, bool bComplex)
{
	SB::Object obj;
	bool       bForce;
	bool       bIsComplex;

	if (mapSimpleObjects.empty() && mapComplexObjects.empty())
	{
		bComplex = false;
	}

	bIsComplex = false;

	for (;;)
	{
		CX::Util::RndGen::Get().GetString(&obj.m_sName, 1, 64, "_ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789");

		CX::String sName;

		sName       = "sbt_";
		sName       += obj.m_sName;
		obj.m_sName = sName;

		if (mapSimpleObjects.end() != mapSimpleObjects.find(obj.m_sName))
		{
			continue;
		}
		if (mapComplexObjects.end() != mapComplexObjects.find(obj.m_sName))
		{
			continue;
		}

		break;
	}

	CX::Size cFields = cMinFields + (CX::Util::RndGen::Get().GetSize() % (cMaxFields - cMinFields + 1));

	for (CX::Size i = 0; i < cFields; i++)
	{
		SB::Member member;

		for (;;)
		{
			CX::Util::RndGen::Get().GetString(&member.m_sName, 1, 64, 
			                                  "_ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789");

			CX::String sName;

			sName          = "sbt_";
			sName          += member.m_sName;
			member.m_sName = sName;

			if (mapSimpleObjects.end() != mapSimpleObjects.find(member.m_sName))
			{
				continue;
			}
			if (mapComplexObjects.end() != mapComplexObjects.find(member.m_sName))
			{
				continue;
			}

			member.m_nType = (SB::Member::Type)(Util::RndGen::Get().GetSize() % 6);

			bForce = false;
			if (bComplex)
			{
				bForce = true;
				for (SB::MembersVector::const_iterator iter = obj.m_vectorMembers.begin(); iter != obj.m_vectorMembers.end(); ++iter)
				{
					if (SB::Member::Type_Scalar == member.m_nType)
					{
						if (!IsSimpleType(iter->m_sValType.c_str()))
						{
							bForce = false;
							break;
						}
					}
					else
					if (SB::Member::Type_Vector == member.m_nType)
					{
						if (!IsSimpleType(iter->m_sValType.c_str()))
						{
							bForce = false;
							break;
						}
					}
					else
					if (SB::Member::Type_Set == member.m_nType)
					{
						if (!IsSimpleType(iter->m_sKeyType.c_str()))
						{
							bForce = false;
							break;
						}
					}
					else
					if (SB::Member::Type_Map == member.m_nType)
					{
						if (!IsSimpleType(iter->m_sKeyType.c_str()))
						{
							bForce = false;
							break;
						}
						if (!IsSimpleType(iter->m_sValType.c_str()))
						{
							bForce = false;
							break;
						}
					}
					else
					if (SB::Member::Type_HashSet == member.m_nType)
					{
						if (!IsSimpleType(iter->m_sKeyType.c_str()))
						{
							bForce = false;
							break;
						}
					}
					else
					if (SB::Member::Type_HashMap == member.m_nType)
					{
						if (!IsSimpleType(iter->m_sKeyType.c_str()))
						{
							bForce = false;
							break;
						}
						if (!IsSimpleType(iter->m_sValType.c_str()))
						{
							bForce = false;
							break;
						}
					}
				}
			}

			if (SB::Member::Type_Scalar == member.m_nType)
			{
				member.m_sValType = GetType(mapSimpleObjects, mapComplexObjects, false, bForce, bComplex);
				if (IsSimpleType(member.m_sValType.c_str()))
				{
					bIsComplex = true;
				}
			}
			else
			if (SB::Member::Type_Vector == member.m_nType)
			{
				member.m_sValType = GetType(mapSimpleObjects, mapComplexObjects, false, bForce, bComplex);
				if (IsSimpleType(member.m_sValType.c_str()))
				{
					bIsComplex = true;
				}
			}
			else
			if (SB::Member::Type_Set == member.m_nType)
			{
				member.m_sKeyType = GetType(mapSimpleObjects, mapComplexObjects, true, false, false);
				if (IsSimpleType(member.m_sKeyType.c_str()))
				{
					bIsComplex = true;
				}
			}
			else
			if (SB::Member::Type_Map == member.m_nType)
			{
				member.m_sKeyType = GetType(mapSimpleObjects, mapComplexObjects, true, false, false);
				member.m_sValType = GetType(mapSimpleObjects, mapComplexObjects, false, bForce, bComplex);
				if (IsSimpleType(member.m_sKeyType.c_str()))
				{
					bIsComplex = true;
				}
				if (IsSimpleType(member.m_sValType.c_str()))
				{
					bIsComplex = true;
				}
			}
			else
			if (SB::Member::Type_HashSet == member.m_nType)
			{
				member.m_sKeyType = GetType(mapSimpleObjects, mapComplexObjects, true, false, false);
				if (IsSimpleType(member.m_sKeyType.c_str()))
				{
					bIsComplex = true;
				}
			}
			else
			if (SB::Member::Type_HashMap == member.m_nType)
			{
				member.m_sKeyType = GetType(mapSimpleObjects, mapComplexObjects, true, false, false);
				member.m_sValType = GetType(mapSimpleObjects, mapComplexObjects, false, bForce, bComplex);
				if (IsSimpleType(member.m_sKeyType.c_str()))
				{
					bIsComplex = true;
				}
				if (IsSimpleType(member.m_sValType.c_str()))
				{
					bIsComplex = true;
				}
			}

			CX::Bool bContinue = false;
			for (SB::MembersVector::const_iterator iter = obj.m_vectorMembers.begin(); iter != obj.m_vectorMembers.end(); ++iter)
			{
				if (0 == cx_strcmp(iter->m_sName.c_str(), member.m_sName.c_str()))
				{
					bContinue = true;
					break;
				}
			}
			if (bContinue)
			{
				continue;
			}

			break;
		}
		
		obj.m_vectorMembers.push_back(member);
	}

	if (bIsComplex)
	{
		mapComplexObjects[obj.m_sName] = obj;
	}
	else
	{
		mapSimpleObjects[obj.m_sName] = obj;
	}
}

/*
void GenerateProto(CX::IO::SimpleBuffers::ObjectsMap *pMapObjects)
{
	CX::Status status;

	for (CX::IO::SimpleBuffers::ObjectsMap::iterator iter = pMapObjects->begin(); iter != pMapObjects->end(); ++iter)
	{
		CX::String sPath;

		CX::Print(&sPath, "{1}.sb", iter->second.m_sName);

		if ((status = CX::IO::SimpleBuffers::Generator::GenerateProtoWithPath(&iter->second, sPath.c_str())).IsNOK())
		{
			CX::Print(stdout, "GenerateProtoWithPath({1}) failed with error code {2} and error message '{3}'", 
			          iter->second.m_sName, status.GetCode(), status.GetMsg());
		}
	}
}

void GenerateCPP(CX::IO::SimpleBuffers::ObjectsMap *pMapObjects)
{
	CX::Status status;

	//jest test everything is ok
	for (CX::IO::SimpleBuffers::ObjectsMap::iterator iter = pMapObjects->begin(); iter != pMapObjects->end(); ++iter)
	{
		CX::String sPath;

		CX::Print(&sPath, "{1}.sb", iter->second.m_sName);

		CX::IO::SimpleBuffers::ProtoParser parser;

		if ((status = parser.BeginParse()).IsNOK())
		{
			CX::Print(stdout, "BeginParse({1}) failed with error code {2} and error message '{3}'", 
			          iter->second.m_sName, status.GetCode(), status.GetMsg());

			break;
		}
		if ((status = parser.Parse(sPath.c_str())).IsNOK())
		{
			CX::Print(stdout, "Parse({1}) failed with error code {2} and error message '{3}'", 
			          iter->second.m_sName, status.GetCode(), status.GetMsg());

			break;
		}
		if ((status = parser.EndParse()).IsNOK())
		{
			CX::Print(stdout, "EndParse({1}) failed with error code {2} and error message '{3}'", 
			          iter->second.m_sName, status.GetCode(), status.GetMsg());

			break;
		}
	}

	for (CX::IO::SimpleBuffers::ObjectsMap::iterator iter = pMapObjects->begin(); iter != pMapObjects->end(); ++iter)
	{
		CX::String sPath;

		CX::Print(&sPath, "{1}.hpp", iter->second.m_sName);

		if ((status = CX::IO::SimpleBuffers::Generator::GenerateCPPWithPath(&iter->second, sPath.c_str())).IsNOK())
		{
			CX::Print(stdout, "GenerateCPPWithPath({1}) failed with error code {2} and error message '{3}'", 
			          iter->second.m_sName, status.GetCode(), status.GetMsg());
		}
	}
}


void GenerateTester(CX::IO::SimpleBuffers::ObjectsMap *pMapSimpleObjects, 
                    CX::IO::SimpleBuffers::ObjectsMap *pMapComplexObjects)
{
	CX::IO::FileOutputStream   fos("simplebuffers_test.hpp");
	CX::IO::IOutputStream      *pOS = &fos;

	CX::Print(pOS, "\n");
	CX::Print(pOS, "#pragma once\n");
	CX::Print(pOS, "\n");
	CX::Print(pOS, "\n");
	CX::Print(pOS, "#include \"CX/IO/SimpleBuffers/ObjectTester.hpp\"\n");
	for (CX::IO::SimpleBuffers::ObjectsMap::iterator iter = pMapSimpleObjects->begin(); iter != pMapSimpleObjects->end(); ++iter)
	{
		CX::Print(pOS, "#include \"{1}.hpp\"\n", iter->second.m_sName);
	}
	for (CX::IO::SimpleBuffers::ObjectsMap::iterator iter = pMapComplexObjects->begin(); iter != pMapComplexObjects->end(); ++iter)
	{
		CX::Print(pOS, "#include \"{1}.hpp\"\n", iter->second.m_sName);
	}
	CX::Print(pOS, "\n");
	CX::Print(pOS, "\n");
	CX::Print(pOS, "void DoSimpleBuffersTest()\n");
	CX::Print(pOS, "{{\n");
	CX::Print(pOS, "\tCX::String sDetail;\n");
	CX::Print(pOS, "\tCX::Bool bOK = true;\n");
	CX::Print(pOS, "\n");
	for (CX::IO::SimpleBuffers::ObjectsMap::iterator iter = pMapSimpleObjects->begin(); iter != pMapSimpleObjects->end(); ++iter)
	{
		CX::Print(pOS, "\tif (!CX::IO::SimpleBuffers::ObjectTester::Test<{1}>(&sDetail))\n", iter->second.m_sName);
		CX::Print(pOS, "\t{{\n");
		CX::Print(pOS, "\t\tbOK = false;\n");
		CX::Print(pOS, "\t\tCX::Print(stdout, \"{1} test failed: {{1}\", sDetail);\n", iter->second.m_sName);
		CX::Print(pOS, "\t}\n");
	}
	for (CX::IO::SimpleBuffers::ObjectsMap::iterator iter = pMapComplexObjects->begin(); iter != pMapComplexObjects->end(); ++iter)
	{
		CX::Print(pOS, "\tif (!CX::IO::SimpleBuffers::ObjectTester::Test<{1}>(&sDetail))\n", iter->second.m_sName);
		CX::Print(pOS, "\t{{\n");
		CX::Print(pOS, "\t\tbOK = false;\n");
		CX::Print(pOS, "\t\tCX::Print(stdout, \"{1} test failed: {{1}\", sDetail);\n", iter->second.m_sName);
		CX::Print(pOS, "\t}\n");
	}
	CX::Print(pOS, "\tif (bOK)\n");
	CX::Print(pOS, "\t{{\n");
	CX::Print(pOS, "\t\tCX::Print(stdout, \"ALL OK\");\n");
	CX::Print(pOS, "\t}\n");
	CX::Print(pOS, "}\n");

	CX::Print(pOS, "\n");
}
*/

int main(int argc, char *argv[])
{
	argc;
	argv;

	SB::ObjectsMap mapSimpleObjects;
	SB::ObjectsMap mapComplexObjects;

	CX::Size cMinSimpleObjects = 1;
	CX::Size cMaxSimpleObjects = 1;
	CX::Size cSimpleObjects    = cMinSimpleObjects + 
	                             CX::Util::RndGen::Get().GetSize() % (cMaxSimpleObjects - cMinSimpleObjects + 1);

	CX::Size cMinComplexObjects = 1;
	CX::Size cMaxComplexObjects = 1;
	CX::Size cComplexObjects    = cMinComplexObjects + 
	                              CX::Util::RndGen::Get().GetSize() % (cMaxComplexObjects - cMinComplexObjects + 1);

	/*
	for (CX::Size i = 0; i < cSimpleObjects; i++)
	{
		GenerateObject(&mapSimpleObjects, &mapComplexObjects, 0, 30, false);
	}

	for (CX::Size i = 0; i < cComplexObjects; i++)
	{
		GenerateObject(&mapSimpleObjects, &mapComplexObjects, 0, 30, true);
	}

	GenerateProto(&mapSimpleObjects);
	GenerateProto(&mapComplexObjects);

	GenerateCPP(&mapSimpleObjects);
	GenerateCPP(&mapComplexObjects);

	GenerateTester(&mapSimpleObjects, &mapComplexObjects);
	*/

	return 0;
}

