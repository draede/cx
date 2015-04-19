/* 
 * CX - C++ framework for general purpose development
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
#include "CX/IO/FileOutputStream.hpp"
#include "CX/String.hpp"
#include "CX/C/ctype.h"


using namespace CX;

typedef Map<String, SB::Member>::Type    MembersMap;

typedef struct _Object
{
	String       sName;
	MembersMap   mapMembers;
}Object;

//use CaseInsensitiveOrderPolicy to avoid duplicate file names (FS is case insensitive...)
typedef Map<String, Object, CaseInsensitiveOrderPolicy>::Type    ObjectsMap;


const Char *GetRandBasicType()
{
	static Set<String>::Type setBasicTypes;

	if (setBasicTypes.empty())
	{
		setBasicTypes.insert("bool");
		setBasicTypes.insert("int8");
		setBasicTypes.insert("uint8");
		setBasicTypes.insert("int16");
		setBasicTypes.insert("uint16");
		setBasicTypes.insert("int32");
		setBasicTypes.insert("uint32");
		setBasicTypes.insert("int64");
		setBasicTypes.insert("uint64");
		setBasicTypes.insert("float");
		setBasicTypes.insert("double");
		setBasicTypes.insert("string");
		setBasicTypes.insert("wstring");
	}

	Size cIndex = Util::RndGen::Get().GetSize() % setBasicTypes.size();

	Set<String>::Type::const_iterator iter = setBasicTypes.begin();

	while (0 < cIndex)
	{
		iter++;
		cIndex--;
	}

	return iter->c_str();
}


void GenerateMember(SB::Member &member, const Object &obj, const ObjectsMap &mapObjects, 
                    const SB::Generator::TestGenRules &rules, bool bSimple)
{
	//generate member name
	for (;;)
	{
		String sName;
		String sNameEx;

		Util::RndGen::Get().GetString(&sName, rules.cMinMemberNameLen, rules.cMaxMemberNameLen, 
		                              "_ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789");

		sNameEx = "sbt_";
		sNameEx += sName;
		if (0 == cx_strcmp(obj.sName.c_str(), sNameEx.c_str()))
		{
			continue;
		}
		if (obj.mapMembers.end() != obj.mapMembers.find(sNameEx))
		{
			continue;
		}
		if (mapObjects.end() != mapObjects.find(sNameEx))
		{
			continue;
		}
		member.m_sName = sNameEx;

		break;
	}

	//generate type

	//favor scalar members
	bool bScalar = (70 >= (Util::RndGen::Get().GetSize() % 100));

	if (bScalar)
	{
		member.m_nType = SB::Member::Type_Scalar;
	}
	else
	{
		member.m_nType = (SB::Member::Type)(1 + (Util::RndGen::Get().GetSize() % 4));
	}

	if (SB::Member::Type_Scalar == member.m_nType || SB::Member::Type_Vector == member.m_nType)
	{
		if (bSimple || mapObjects.empty())
		{
			member.m_sValType = GetRandBasicType();
		}
		else
		{
			Size                       cIndex = Util::RndGen::Get().GetSize() % mapObjects.size();
			ObjectsMap::const_iterator iter   = mapObjects.begin();

			while (0 < cIndex)
			{
				cIndex--;
				iter++;
			}
			member.m_sValType = iter->second.sName;
		}
	}
	else
	if (SB::Member::Type_Set == member.m_nType || SB::Member::Type_HashSet == member.m_nType)
	{
		if (bSimple || mapObjects.empty())
		{
			member.m_sKeyType = GetRandBasicType();
		}
		else
		{
			Size                       cIndex = Util::RndGen::Get().GetSize() % mapObjects.size();
			ObjectsMap::const_iterator iter   = mapObjects.begin();

			while (0 < cIndex)
			{
				cIndex--;
				iter++;
			}
			member.m_sKeyType = iter->second.sName;
		}
	}
	else
	if (SB::Member::Type_Map == member.m_nType || SB::Member::Type_HashMap == member.m_nType)
	{
		if (bSimple || mapObjects.empty())
		{
			member.m_sKeyType = GetRandBasicType();
		}
		else
		{
			Size                       cIndex = Util::RndGen::Get().GetSize() % mapObjects.size();
			ObjectsMap::const_iterator iter   = mapObjects.begin();

			while (0 < cIndex)
			{
				cIndex--;
				iter++;
			}
			member.m_sKeyType = iter->second.sName;
		}
		if (bSimple || mapObjects.empty())
		{
			member.m_sValType = GetRandBasicType();
		}
		else
		{
			Size                       cIndex = Util::RndGen::Get().GetSize() % mapObjects.size();
			ObjectsMap::const_iterator iter = mapObjects.begin();

			while (0 < cIndex)
			{
				cIndex--;
				iter++;
			}
			member.m_sValType = iter->second.sName;
		}
	}
}

void GenerateObject(Object &obj, const ObjectsMap &mapObjects, const SB::Generator::TestGenRules &rules)
{
	//generate object name
	for (;;)
	{
		String sName;
		String sNameEx;

		Util::RndGen::Get().GetString(&sName, rules.cMinObjectNameLen, rules.cMaxObjectNameLen, 
		                              "_ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789");

		sNameEx = "sbt_";
		sNameEx += sName;
		if (mapObjects.end() != mapObjects.find(sNameEx))
		{
			continue;
		}
		obj.sName = sNameEx;

		break;
	}

	Size cMembersCount = rules.cMinMembersCount + 
	                     (Util::RndGen::Get().GetSize() % (rules.cMaxMembersCount - rules.cMinMembersCount + 1));

	Size cMembersSimple = (Size)(cMembersCount * 0.70);
	Size cMembersComplex = cMembersCount - cMembersSimple;

	if (0 == cMembersCount)
	{
		cMembersSimple  = 1;
		cMembersComplex = 1;
	}

	for (Size i = 0; i < cMembersSimple; i++)
	{
		SB::Member member;

		GenerateMember(member, obj, mapObjects, rules, true);

		obj.mapMembers[member.m_sName] = member;
	}

	for (Size i = 0; i < cMembersComplex; i++)
	{
		SB::Member member;

		GenerateMember(member, obj, mapObjects, rules, false);

		obj.mapMembers[member.m_sName] = member;
	}
}

void GenerateObjects(ObjectsMap &mapObjects, const SB::Generator::TestGenRules &rules)
{
	Size cObjectsCount = rules.cMinObjectsCount + 
	                     (Util::RndGen::Get().GetSize() % (rules.cMaxObjectsCount - rules.cMinObjectsCount + 1));

	for (Size i = 0; i < cObjectsCount; i++)
	{
		Object obj;

		GenerateObject(obj, mapObjects, rules);

		mapObjects[obj.sName] = obj;
	}
}

void Convert(const ObjectsMap &mapObjectsIn, SB::ObjectsMap &mapObjectsOut)
{
	for (ObjectsMap::const_iterator iterObjects = mapObjectsIn.begin(); iterObjects != mapObjectsIn.end(); ++iterObjects)
	{
		SB::Object obj;

		obj.m_sName = iterObjects->second.sName;
		for (MembersMap::const_iterator iterMembers = iterObjects->second.mapMembers.begin(); 
		     iterMembers != iterObjects->second.mapMembers.begin(); ++iterMembers)
		{
			obj.m_vectorMembers.push_back(iterMembers->second);
		}

		mapObjectsOut[obj.m_sName] = obj;
	}
}


void GenerateProto(const ObjectsMap &mapObjects)
{
	Status status;

	for (ObjectsMap::const_iterator iter = mapObjects.begin(); iter != mapObjects.end(); ++iter)
	{
		String sPath;

		Print(&sPath, "{1}.sb", iter->second.sName);

		SB::Object obj;

		obj.m_sName = iter->second.sName;
		for (MembersMap::const_iterator iter2 = iter->second.mapMembers.begin(); iter2 != iter->second.mapMembers.end(); ++iter2)
		{
			obj.m_vectorMembers.push_back(iter2->second);
		}

		if ((status = SB::Generator::GenerateProtoWithPath(obj, sPath.c_str())).IsNOK())
		{
			Print(stdout, "GenerateProtoWithPath({1}) failed with error code {2} and error message '{3}'", 
			      obj.m_sName, status.GetCode(), status.GetMsg());
		}
	}
}

void TestProto(const ObjectsMap &mapObjects)
{
	Status status;

	//jest test everything is ok
	for (ObjectsMap::const_iterator iter = mapObjects.begin(); iter != mapObjects.end(); ++iter)
	{
		String sPath;

		Print(&sPath, "{1}.sb", iter->second.sName);

		SB::Parser         parser;
		SB::Parser::Data   data;

		if ((status = parser.Parse(sPath.c_str(), data)).IsNOK())
		{
			Print(stdout, "Parse({1}) failed with error code {2} and error message '{3}'",
			      iter->second.sName, status.GetCode(), status.GetMsg());

			break;
		}
	}
}

void GenerateCPP(const ObjectsMap &mapObjects)
{
	Status status;

	for (ObjectsMap::const_iterator iter = mapObjects.begin(); iter != mapObjects.end(); ++iter)
	{
		String sPath;

		Print(&sPath, "{1}.hpp", iter->second.sName);

		SB::Object obj;

		obj.m_sName = iter->second.sName;
		for (MembersMap::const_iterator iter2 = iter->second.mapMembers.begin(); iter2 != iter->second.mapMembers.end(); ++iter2)
		{
			obj.m_vectorMembers.push_back(iter2->second);
		}

		if ((status = SB::Generator::GenerateWithPath(obj, sPath.c_str())).IsNOK())
		{
			Print(stdout, "GenerateWithPath({1}) failed with error code {2} and error message '{3}'", 
			      obj.m_sName, status.GetCode(), status.GetMsg());
		}
	}
}

void GenerateCPPTest(const ObjectsMap &mapObjects, const SB::Generator::TestGenRules &rules)
{
	Status status;

	for (ObjectsMap::const_iterator iter = mapObjects.begin(); iter != mapObjects.end(); ++iter)
	{
		String sPath;

		Print(&sPath, "{1}Test.hpp", iter->second.sName);

		SB::Object obj;

		obj.m_sName = iter->second.sName;
		for (MembersMap::const_iterator iter2 = iter->second.mapMembers.begin(); iter2 != iter->second.mapMembers.end(); ++iter2)
		{
			obj.m_vectorMembers.push_back(iter2->second);
		}

		if ((status = SB::Generator::GenerateTestWithPath(obj, sPath.c_str(), rules)).IsNOK())
		{
			Print(stdout, "GenerateTestWithPath({1}) failed with error code {2} and error message '{3}'",
			      obj.m_sName, status.GetCode(), status.GetMsg());
		}
	}
}

void GenerateTester(const ObjectsMap &mapObjects)
{
	IO::FileOutputStream fos("all_tests.hpp");
	IO::IOutputStream    *pOS = &fos;
	Status                status;

	Print(pOS, "\n");
	Print(pOS, "#pragma once\n");
	Print(pOS, "\n");
	Print(pOS, "\n");
	Print(pOS, "#include \"CX/SB/Tester.hpp\"\n");
	Print(pOS, "#include \"CX/Print.hpp\"\n");
	for (ObjectsMap::const_iterator iter = mapObjects.begin(); iter != mapObjects.end(); ++iter)
	{
		Print(pOS, "#include \"{1}Test.hpp\"\n", iter->second.sName);
	}
	Print(pOS, "\n");
	Print(pOS, "\n");
	Print(pOS, "void Run_Tests()\n");
	Print(pOS, "{{\n");
	Print(pOS, "\tCX::SB::StatsData trd;\n");
	Print(pOS, "\tCX::SB::StatsData twd;\n");
	Print(pOS, "\tCX::Size cAll = 0;\n");
	Print(pOS, "\tCX::Size cOK = 0;\n");
	Print(pOS, "\tCX::Status status;\n");
	Print(pOS, "\n");
	Print(pOS, "\ttrd.Reset();\n");
	Print(pOS, "\ttwd.Reset();\n");
	for (ObjectsMap::const_iterator iter = mapObjects.begin(); iter != mapObjects.end(); ++iter)
	{
		Print(pOS, "\t{{\n");
		Print(pOS, "\t\tCX::SB::StatsData rd;\n");
		Print(pOS, "\t\tCX::SB::StatsData wd;\n");
		Print(pOS, "\n");
		Print(pOS, "\t\tif ((status = CX::SB::TestSerialize<{1}>(rd, wd)).IsNOK())\n", iter->second.sName);
		Print(pOS, "\t\t{{\n");
		Print(pOS, "\t\t\tcAll++;\n");
		Print(pOS, 
		   "\t\tCX::Print(stdout, \"{1} failed with error code {{1} and error msg '{{2}'\", status.GetCode(), status.GetMsg());\n", 
		   iter->second.sName);
		Print(pOS, "\t\t}\n");
		Print(pOS, "\t\telse\n");
		Print(pOS, "\t\t{{\n");
		Print(pOS, "\t\t\tcAll++;\n");
		Print(pOS, "\t\t\tcOK++;\n");
		Print(pOS, "\t\t}\n");
		Print(pOS, "\t\ttrd.Add(rd);\n");
		Print(pOS, "\t\ttwd.Add(wd);\n");
		Print(pOS, "\t}\n");
	}
	Print(pOS, "\tCX::Print(stdout, \"All tests : {{1}\\n\", cAll);\n");
	Print(pOS, "\tCX::Print(stdout, \"OK tests  : {{1}\\n\", cOK);\n");
	Print(pOS, "\tCX::Print(stdout, \"===== READ STATS =========================================\\n\");\n");
	Print(pOS, "\tCX::Print(stdout, \"   Members: {{1}\\n\", trd.m_cMembers);\n");
	Print(pOS, "\tCX::Print(stdout, \"   Items: {{1}\\n\", trd.m_cItems);\n");
	Print(pOS, "\tCX::Print(stdout, \"   Objects: {{1}\\n\", trd.m_cObjects);\n");
	Print(pOS, "\tCX::Print(stdout, \"   Arrays: {{1}\\n\", trd.m_cArrays);\n");
	Print(pOS, "\tCX::Print(stdout, \"   Bools: {{1}\\n\", trd.m_cBools);\n");
	Print(pOS, "\tCX::Print(stdout, \"   Int8s: {{1}\\n\", trd.m_cInt8s);\n");
	Print(pOS, "\tCX::Print(stdout, \"   UInt8s: {{1}\\n\", trd.m_cUInt8s);\n");
	Print(pOS, "\tCX::Print(stdout, \"   Int16s: {{1}\\n\", trd.m_cInt16s);\n");
	Print(pOS, "\tCX::Print(stdout, \"   UInt16s: {{1}\\n\", trd.m_cUInt16s);\n");
	Print(pOS, "\tCX::Print(stdout, \"   Int32s: {{1}\\n\", trd.m_cInt32s);\n");
	Print(pOS, "\tCX::Print(stdout, \"   UInt32s: {{1}\\n\", trd.m_cUInt32s);\n");
	Print(pOS, "\tCX::Print(stdout, \"   Int64s: {{1}\\n\", trd.m_cInt64s);\n");
	Print(pOS, "\tCX::Print(stdout, \"   UInt64s: {{1}\\n\", trd.m_cUInt64s);\n");
	Print(pOS, "\tCX::Print(stdout, \"   Floats: {{1}\\n\", trd.m_cFloats);\n");
	Print(pOS, "\tCX::Print(stdout, \"   Doubles: {{1}\\n\", trd.m_cDoubles);\n");
	Print(pOS, "\tCX::Print(stdout, \"   Strings: {{1}\\n\", trd.m_cStrings);\n");
	Print(pOS, "\tCX::Print(stdout, \"   WStrings: {{1}\\n\", trd.m_cWStrings);\n");
	Print(pOS, "\tCX::Print(stdout, \"   AllValues: {{1}\\n\", trd.m_cAllValues);\n");
	Print(pOS, "\tCX::Print(stdout, \"   DataSize: {{1:.3} MB\\n\", trd.m_cbDataSize / 1048576.0);\n");
	Print(pOS, "\tCX::Print(stdout, \"   TotalSize: {{:.3} MB\\n\", trd.m_cbTotalSize / 1048576.0);\n");
	Print(pOS, "\tCX::Print(stdout, \"===== WRITE STATS =========================================\\n\");\n");
	Print(pOS, "\tCX::Print(stdout, \"   Members: {{1}\\n\", twd.m_cMembers);\n");
	Print(pOS, "\tCX::Print(stdout, \"   Items: {{1}\\n\", twd.m_cItems);\n");
	Print(pOS, "\tCX::Print(stdout, \"   Objects: {{1}\\n\", twd.m_cObjects);\n");
	Print(pOS, "\tCX::Print(stdout, \"   Arrays: {{1}\\n\", twd.m_cArrays);\n");
	Print(pOS, "\tCX::Print(stdout, \"   Bools: {{1}\\n\", twd.m_cBools);\n");
	Print(pOS, "\tCX::Print(stdout, \"   Int8s: {{1}\\n\", twd.m_cInt8s);\n");
	Print(pOS, "\tCX::Print(stdout, \"   UInt8s: {{1}\\n\", twd.m_cUInt8s);\n");
	Print(pOS, "\tCX::Print(stdout, \"   Int16s: {{1}\\n\", twd.m_cInt16s);\n");
	Print(pOS, "\tCX::Print(stdout, \"   UInt16s: {{1}\\n\", twd.m_cUInt16s);\n");
	Print(pOS, "\tCX::Print(stdout, \"   Int32s: {{1}\\n\", twd.m_cInt32s);\n");
	Print(pOS, "\tCX::Print(stdout, \"   UInt32s: {{1}\\n\", twd.m_cUInt32s);\n");
	Print(pOS, "\tCX::Print(stdout, \"   Int64s: {{1}\\n\", twd.m_cInt64s);\n");
	Print(pOS, "\tCX::Print(stdout, \"   UInt64s: {{1}\\n\", twd.m_cUInt64s);\n");
	Print(pOS, "\tCX::Print(stdout, \"   Floats: {{1}\\n\", twd.m_cFloats);\n");
	Print(pOS, "\tCX::Print(stdout, \"   Doubles: {{1}\\n\", twd.m_cDoubles);\n");
	Print(pOS, "\tCX::Print(stdout, \"   Strings: {{1}\\n\", twd.m_cStrings);\n");
	Print(pOS, "\tCX::Print(stdout, \"   WStrings: {{1}\\n\", twd.m_cWStrings);\n");
	Print(pOS, "\tCX::Print(stdout, \"   AllValues: {{1}\\n\", twd.m_cAllValues);\n");
	Print(pOS, "\tCX::Print(stdout, \"   DataSize: {{1:.3} MB\\n\", twd.m_cbDataSize / 1048576.0);\n");
	Print(pOS, "\tCX::Print(stdout, \"   TotalSize: {{1:.3} MB\\n\", twd.m_cbTotalSize / 1048576.0);\n");
	Print(pOS, "}\n");
}

/*
void Test()
{
	ObjectsMap mapObjects;

	SB::Generator::TestGenRules rules;

	rules.cMinObjectsCount = 20;
	rules.cMaxObjectsCount = 20;
	rules.cMinObjectNameLen = 1;
	rules.cMaxObjectNameLen = 20;
	rules.cMinMembersCount = 1;
	rules.cMaxMembersCount = 10;
	rules.cMinMemberNameLen = 1;
	rules.cMaxMemberNameLen = 10;
	rules.cMinStringLen = 0;
	rules.cMaxStringLen = 30;
	rules.cMinItemsCount = 1;
	rules.cMaxItemsCount = 10;

	{
		Object obj;

		obj.sName = "Test1";

		SB::Member m;

		m.m_sName = "val1";
		m.m_nType = SB::Member::Type_Scalar;
		m.m_sValType = "int32";
		obj.mapMembers[m.m_sName] = m;

		mapObjects[obj.sName] = obj;
	}
	{
		Object obj;

		obj.sName = "Test2";

		SB::Member m;

		m.m_sName = "val2";
		m.m_nType = SB::Member::Type_HashMap;
		m.m_sKeyType = "Test1";
		m.m_sValType = "int32";
		obj.mapMembers[m.m_sName] = m;

		mapObjects[obj.sName] = obj;
	}

	GenerateCPP(mapObjects);
	GenerateCPPTest(mapObjects, rules);
	GenerateTester(mapObjects);
}
*/

int main(int argc, char *argv[])
{
	argc;
	argv;

	SB::Generator::TestGenRules rules;

	rules.cMinObjectsCount = 10;
	rules.cMaxObjectsCount = 10;
	rules.cMinObjectNameLen = 1;
	rules.cMaxObjectNameLen = 20;
	rules.cMinMembersCount = 1;
	rules.cMaxMembersCount = 10;
	rules.cMinMemberNameLen = 1;
	rules.cMaxMemberNameLen = 10;
	rules.cMinStringLen = 0;
	rules.cMaxStringLen = 30;
	rules.cMinItemsCount = 1;
	rules.cMaxItemsCount = 10;

	ObjectsMap mapObjects;

	GenerateObjects(mapObjects, rules);

	GenerateProto(mapObjects);
	TestProto(mapObjects);
	GenerateCPP(mapObjects);
	GenerateCPPTest(mapObjects, rules);
	GenerateTester(mapObjects);

	return 0;
}

