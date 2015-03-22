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
#include "CX/IO/SimpleBuffers/Object.hpp"
#include "CX/IO/SimpleBuffers/Generator.hpp"
#include "CX/IO/SimpleBuffers/ProtoParser.hpp"
#include "CX/C/ctype.h"


void GenerateObject(CX::IO::SimpleBuffers::ObjectsMap *pMapSimpleObjects, 
                    CX::IO::SimpleBuffers::ObjectsMap *pMapComplexObjects,
                    CX::Size cMinFields, CX::Size cMaxFields, bool bComplex)
{
	CX::IO::SimpleBuffers::Object obj;

	for (;;)
	{
		CX::Util::RndGen::Get().GetString(&obj.m_sName, 1, 64, "_ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789");

		CX::String sName;

		sName       = "sbt_";
		sName       += obj.m_sName;
		obj.m_sName = sName;

		if (pMapSimpleObjects->end() != pMapSimpleObjects->find(obj.m_sName))
		{
			continue;
		}
		if (pMapComplexObjects->end() != pMapComplexObjects->find(obj.m_sName))
		{
			continue;
		}

		break;
	}

	CX::Size cFields = cMinFields + (CX::Util::RndGen::Get().GetSize() % (cMaxFields - cMinFields + 1));

	for (CX::Size i = 0; i < cFields; i++)
	{
		CX::IO::SimpleBuffers::Field field;

		for (;;)
		{
			CX::Util::RndGen::Get().GetString(&field.m_sName, 1, 64, 
			                                  "_ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789");

			CX::String sName;

			sName         = "sbt_";
			sName         += field.m_sName;
			field.m_sName = sName;

			if (pMapSimpleObjects->end() != pMapSimpleObjects->find(field.m_sName))
			{
				continue;
			}
			if (pMapComplexObjects->end() != pMapComplexObjects->find(field.m_sName))
			{
				continue;
			}

			CX::Bool bContinue = false;
			for (CX::IO::SimpleBuffers::FieldsVector::iterator iter = obj.m_vectorFields.begin(); 
			     iter != obj.m_vectorFields.end(); ++iter)
			{
				if (0 == cx_strcmp(iter->m_sName.c_str(), field.m_sName.c_str()))
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
		
		field.m_bIsVector = CX::Util::RndGen::Get().GetBool();

		if (bComplex && !pMapSimpleObjects->empty())
		{
			field.m_nType = (CX::IO::SimpleBuffers::Field::Type)((int)CX::IO::SimpleBuffers::Field::Type_Bool + 
			                                                     (CX::Util::RndGen::Get().GetUInt32() % 13));

			if (i + 1 == cFields && CX::IO::SimpleBuffers::Field::Type_Object != field.m_nType)
			{
				CX::Bool bHasComplex = false;

				for (CX::Size k = 0; k < i; k++)
				{
					if (CX::IO::SimpleBuffers::Field::Type_Object == obj.m_vectorFields[k].m_nType)
					{
						bHasComplex = true;
						break;
					}
				}
				if (!bHasComplex)
				{
					field.m_nType = CX::IO::SimpleBuffers::Field::Type_Object;
				}
			}

			if (CX::IO::SimpleBuffers::Field::Type_Object == field.m_nType)
			{
				CX::Bool bComplexField = CX::Util::RndGen::Get().GetBool();

				CX::IO::SimpleBuffers::ObjectsMap::iterator iter;
				CX::Size                                    cIndex;

				if (pMapComplexObjects->empty())
				{
					bComplexField = false;
				}
				if (bComplexField)
				{
					iter   = pMapComplexObjects->begin();
					if (0 == pMapComplexObjects->size())
					{
						cIndex = 0;
					}
					else
					{
						cIndex = CX::Util::RndGen::Get().GetSize() % pMapComplexObjects->size();
					}
				}
				else
				{
					iter   = pMapSimpleObjects->begin();
					if (0 == pMapSimpleObjects->size())
					{
						cIndex = 0;
					}
					else
					{
						cIndex = CX::Util::RndGen::Get().GetSize() % pMapSimpleObjects->size();
					}
				}
				while (0 < cIndex)
				{
					iter++;
					cIndex--;
				}
				field.m_sObjectName = iter->second.m_sName;
			}
		}
		else
		{
			field.m_nType = (CX::IO::SimpleBuffers::Field::Type)((int)CX::IO::SimpleBuffers::Field::Type_Bool + 
			                                                     (CX::Util::RndGen::Get().GetUInt32() % 12));

			//for now avoid issue with float/double conversion
			if (CX::IO::SimpleBuffers::Field::Type_Float == field.m_nType)
			{
				field.m_nType = CX::IO::SimpleBuffers::Field::Type_UInt32;
			}
			else
			if (CX::IO::SimpleBuffers::Field::Type_Double == field.m_nType)
			{
				field.m_nType = CX::IO::SimpleBuffers::Field::Type_UInt64;
			}
		}

		obj.m_vectorFields.push_back(field);
	}

	if (bComplex)
	{
		(*pMapComplexObjects)[obj.m_sName] = obj;
	}
	else
	{
		(*pMapSimpleObjects)[obj.m_sName] = obj;
	}
}

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

int main(int argc, char *argv[])
{
	argc;
	argv;

	CX::IO::SimpleBuffers::ObjectsMap mapSimpleObjects;
	CX::IO::SimpleBuffers::ObjectsMap mapComplexObjects;

	CX::Size cMinSimpleObjects = 200;
	CX::Size cMaxSimpleObjects = 200;
	CX::Size cSimpleObjects    = cMinSimpleObjects + 
	                             CX::Util::RndGen::Get().GetSize() % (cMaxSimpleObjects - cMinSimpleObjects + 1);

	CX::Size cMinComplexObjects = 200;
	CX::Size cMaxComplexObjects = 200;
	CX::Size cComplexObjects    = cMinComplexObjects + 
	                              CX::Util::RndGen::Get().GetSize() % (cMaxComplexObjects - cMinComplexObjects + 1);


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
	
	return 0;
}

