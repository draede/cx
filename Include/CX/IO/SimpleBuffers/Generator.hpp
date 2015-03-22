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


#include "CX/IO/SimpleBuffers/Object.hpp"
#include "CX/IO/FileOutputStream.hpp"
#include "CX/Set.hpp"
#include "CX/Print.hpp"


namespace CX
{

namespace IO
{

namespace SimpleBuffers
{

class Generator
{
public:

	static Status GenerateProtoWithPath(const Object *pObject, const Char *szPath)
	{
		FileOutputStream fos(szPath);

		if (!fos.IsOK())
		{
			return Status(Status_CreateFailed, "Failed to create file '{1}'", szPath);
		}

		return GenerateProto((IOutputStream *)&fos, pObject);
	}

	template <typename OUTPUT>
	static Status GenerateProto(OUTPUT out, const Object *pObject)
	{
		Set<String>::Type  setImports;
		StringArray        vectorNamespaces;
		String             sName;

		ExtractObjectName(pObject->m_sName.c_str(), &sName, &vectorNamespaces);
		Print(out, "\n");
		for (FieldsVector::const_iterator iter = pObject->m_vectorFields.begin(); iter != pObject->m_vectorFields.end(); ++iter)
		{
			if (Field::Type_Object == iter->m_nType && setImports.end() == setImports.find(iter->m_sObjectName))
			{
				setImports.insert(iter->m_sObjectName);

				const Char *pPos = cx_strrchr(iter->m_sObjectName.c_str(), '.');

				if (NULL != pPos)
				{
					pPos++;
				}
				else
				{
					pPos = iter->m_sObjectName.c_str();
				}

				Print(out, "import \"{1}.sb\"\n", pPos);
			}
		}
		if (!setImports.empty())
		{
			Print(out, "\n");
		}
		if (!vectorNamespaces.empty())
		{
			Print(out, "namespace ");
			for (StringArray::iterator iter = vectorNamespaces.begin(); iter != vectorNamespaces.end(); ++iter)
			{
				if (vectorNamespaces.begin() != iter)
				{
					Print(out, ".{1}", *iter);
				}
				else
				{
					Print(out, "{1}", *iter);
				}
			}
			Print(out, "\n\n");
		}
		Print(out, "object {1}\n", sName);
		Print(out, "{{\n");
		for (FieldsVector::const_iterator iter = pObject->m_vectorFields.begin(); iter != pObject->m_vectorFields.end(); ++iter)
		{
			Print(out, "\t{2}: {1};\n", Field::GetStringFromTypeForProto(iter->m_nType, iter->m_bIsVector, 
			                                                            iter->m_sObjectName.c_str()), iter->m_sName);
		}
		Print(out, "}\n");
		Print(out, "\n");

		return Status();
	}

	static Status GenerateCPPWithPath(const Object *pObject, const Char *szPath)
	{
		FileOutputStream fos(szPath);

		if (!fos.IsOK())
		{
			return Status(Status_CreateFailed, "Failed to create file '{1}'", szPath);
		}

		return GenerateCPP((IOutputStream *)&fos, pObject);
	}

	template <typename OUTPUT>
	static Status GenerateCPP(OUTPUT out, const Object *pObject)
	{
		Set<String>::Type  setIncludes;
		String             sFullName;
		StringArray        vectorNamespaces;
		String             sName;

		ExtractObjectName(pObject->m_sName.c_str(), &sName, &vectorNamespaces);
		for (StringArray::iterator iter = vectorNamespaces.begin(); iter != vectorNamespaces.end(); ++iter)
		{
			if (!sFullName.empty())
			{
				sFullName += "::";
			}
			sFullName += *iter;
		}
		if (!sFullName.empty())
		{
			sFullName += "::";
		}
		sFullName += sName;

		Print(out, "\n");
		Print(out, "#pragma once\n");
		Print(out, "\n");
		Print(out, "\n");
		Print(out, "#include \"CX/IO/SimpleBuffers/Types.hpp\"\n");
		Print(out, "#include \"CX/C/string.h\"\n");
		for (FieldsVector::const_iterator iter = pObject->m_vectorFields.begin(); iter != pObject->m_vectorFields.end(); ++iter)
		{
			if (Field::Type_Object == iter->m_nType && setIncludes.end() == setIncludes.find(iter->m_sObjectName))
			{
				setIncludes.insert(iter->m_sObjectName);

				String sInclude;
				const Char *pPos;

				pPos = iter->m_sObjectName.c_str();
				while (0 != *pPos)
				{
					if ('.' == *pPos)
					{
						sInclude += "/";
					}
					else
					{
						sInclude += *pPos;
					}
					pPos++;
				}

				Print(out, "#include \"{1}.hpp\"\n", sInclude);
			}
		}
		Print(out, "\n");
		Print(out, "\n");
		for (StringArray::iterator iter = vectorNamespaces.begin(); iter != vectorNamespaces.end(); ++iter)
		{
			Print(out, "namespace {1}\n", *iter);
			Print(out, "{{\n");
			Print(out, "\n");
		}
		Print(out, "class {1} : public CX::IO::SimpleBuffers::IObject\n", sName);
		Print(out, "{{\n");
		Print(out, "public:\n");
		Print(out, "\n");
		for (FieldsVector::const_iterator iter = pObject->m_vectorFields.begin(); iter != pObject->m_vectorFields.end(); ++iter)
		{
			Print(out, "\t{1} {2};\n", Field::GetStringFromType(iter->m_nType, iter->m_bIsVector, iter->m_sObjectName.c_str()), 
			      iter->m_sName);
		}
		Print(out, "\n");
		Print(out, "\tvirtual void Reset()\n");
		Print(out, "\t{{\n");
		for (FieldsVector::const_iterator iter = pObject->m_vectorFields.begin(); iter != pObject->m_vectorFields.end(); ++iter)
		{
			if (!iter->m_bIsVector)
			{
				switch (iter->m_nType)
				{
					case Field::Type_Bool:
					{
						Print(out, "\t\t{1} = false;\n", iter->m_sName);
					}
						break;
					case Field::Type_Int8:
					{
						Print(out, "\t\t{1} = 0;\n", iter->m_sName);
					}
						break;
					case Field::Type_UInt8:
					{
						Print(out, "\t\t{1} = 0;\n", iter->m_sName);
					}
						break;
					case Field::Type_Int16:
					{
						Print(out, "\t\t{1} = 0;\n", iter->m_sName);
					}
						break;
					case Field::Type_UInt16:
					{
						Print(out, "\t\t{1} = 0;\n", iter->m_sName);
					}
						break;
					case Field::Type_Int32:
					{
						Print(out, "\t\t{1} = 0;\n", iter->m_sName);
					}
						break;
					case Field::Type_UInt32:
					{
						Print(out, "\t\t{1} = 0;\n", iter->m_sName);
					}
						break;
					case Field::Type_Int64:
					{
						Print(out, "\t\t{1} = 0;\n", iter->m_sName);
					}
						break;
					case Field::Type_UInt64:
					{
						Print(out, "\t\t{1} = 0;\n", iter->m_sName);
					}
						break;
					case Field::Type_Float:
					{
						Print(out, "\t\t{1} = 0.0f;\n", iter->m_sName);
					}
						break;
					case Field::Type_Double:
					{
						Print(out, "\t\t{1} = 0.0;\n", iter->m_sName);
					}
						break;
					case Field::Type_String:
					{
						Print(out, "\t\t{1}.clear();\n", iter->m_sName);
					}
						break;
					case Field::Type_WString:
					{
						Print(out, "\t\t{1}.clear();\n", iter->m_sName);
					}
						break;
					case Field::Type_Object:
					{
						Print(out, "\t\t{1}.Reset();\n", iter->m_sName);
					}
					break;
				}
			}
			else
			{
				Print(out, "\t\t{1}.clear();\n", iter->m_sName);
			}
		}
		Print(out, "\t}\n");
		Print(out, "\n");
		Print(out, "\tvirtual void SetupWithSomeValues()\n");
		Print(out, "\t{{\n");
		Print(out, "\t\tReset();\n");
		for (FieldsVector::const_iterator iter = pObject->m_vectorFields.begin(); iter != pObject->m_vectorFields.end(); ++iter)
		{
			if (!iter->m_bIsVector)
			{
				switch (iter->m_nType)
				{
					case Field::Type_Bool:
					{
						Print(out, "\t\t{1} = {2};\n", iter->m_sName, Util::RndGen::Get().GetBool() ? "true" : "false");
					}
					break;
					case Field::Type_Int8:
					{
						Print(out, "\t\t{1} = {2};\n", iter->m_sName, Util::RndGen::Get().GetInt8());
					}
					break;
					case Field::Type_UInt8:
					{
						Print(out, "\t\t{1} = {2};\n", iter->m_sName, Util::RndGen::Get().GetUInt8());
					}
					break;
					case Field::Type_Int16:
					{
						Print(out, "\t\t{1} = {2};\n", iter->m_sName, Util::RndGen::Get().GetInt16());
					}
					break;
					case Field::Type_UInt16:
					{
						Print(out, "\t\t{1} = {2};\n", iter->m_sName, Util::RndGen::Get().GetUInt16());
					}
					break;
					case Field::Type_Int32:
					{
						Print(out, "\t\t{1} = {2};\n", iter->m_sName, Util::RndGen::Get().GetInt32());
					}
					break;
					case Field::Type_UInt32:
					{
						Print(out, "\t\t{1} = {2};\n", iter->m_sName, Util::RndGen::Get().GetUInt32());
					}
					break;
					case Field::Type_Int64:
					{
						Print(out, "\t\t{1} = {2};\n", iter->m_sName, Util::RndGen::Get().GetInt64());
					}
					break;
					case Field::Type_UInt64:
					{
						Print(out, "\t\t{1} = {2};\n", iter->m_sName, Util::RndGen::Get().GetUInt64());
					}
					break;
					case Field::Type_Float:
					{
						Print(out, "\t\t{1} = {2}f;\n", iter->m_sName, Util::RndGen::Get().GetFloat());
					}
					break;
					case Field::Type_Double:
					{
						Print(out, "\t\t{1} = {2};\n", iter->m_sName, Util::RndGen::Get().GetDouble());
					}
					break;
					case Field::Type_String:
					{
						String sTmp1;
						String sTmp2;

						Util::RndGen::Get().GetString(&sTmp1, 0, 64);
						for (Size i = 0; i < sTmp1.size(); i++)
						{
							if ('\\' == sTmp1[i] || '"' == sTmp1[i])
							{
								sTmp2 += "\\";
							}
							sTmp2 += sTmp1[i];
						}
						Print(out, "\t\t{1} = \"{2}\";\n", iter->m_sName, sTmp2);
					}
					break;
					case Field::Type_WString:
					{
						String sTmp1;
						String sTmp2;

						Util::RndGen::Get().GetString(&sTmp1, 0, 64);
						for (Size i = 0; i < sTmp1.size(); i++)
						{
							if ('\\' == sTmp1[i] || '"' == sTmp1[i])
							{
								sTmp2 += "\\";
							}
							sTmp2 += sTmp1[i];
						}
						Print(out, "\t\t{1} = L\"{2}\";\n", iter->m_sName, sTmp2);
					}
					break;
					case Field::Type_Object:
					{
						Print(out, "\t\t{1}.SetupWithSomeValues();\n", iter->m_sName);
					}
					break;
				}
			}
			else
			{
				Size cCount = Util::RndGen::Get().GetSize() % 17;

				if (0 < cCount)
				{
					Print(out, "\t\tfor (CX::Size i = 0; i < {1}; i++)\n", cCount);
					Print(out, "\t\t{{\n");
					switch (iter->m_nType)
					{
						case Field::Type_Bool:
						{
							Print(out, "\t\t\t{1}.push_back({2});\n", iter->m_sName, Util::RndGen::Get().GetBool());
						}
						break;
						case Field::Type_Int8:
						{
							Print(out, "\t\t\t{1}.push_back({2});\n", iter->m_sName, Util::RndGen::Get().GetInt8());
						}
						break;
						case Field::Type_UInt8:
						{
							Print(out, "\t\t\t{1}.push_back({2});\n", iter->m_sName, Util::RndGen::Get().GetUInt8());
						}
						break;
						case Field::Type_Int16:
						{
							Print(out, "\t\t\t{1}.push_back({2});\n", iter->m_sName, Util::RndGen::Get().GetInt16());
						}
						break;
						case Field::Type_UInt16:
						{
							Print(out, "\t\t\t{1}.push_back({2});\n", iter->m_sName, Util::RndGen::Get().GetUInt16());
						}
						break;
						case Field::Type_Int32:
						{
							Print(out, "\t\t\t{1}.push_back({2});\n", iter->m_sName, Util::RndGen::Get().GetInt32());
						}
						break;
						case Field::Type_UInt32:
						{
							Print(out, "\t\t\t{1}.push_back({2});\n", iter->m_sName, Util::RndGen::Get().GetUInt32());
						}
						break;
						case Field::Type_Int64:
						{
							Print(out, "\t\t\t{1}.push_back({2});\n", iter->m_sName, Util::RndGen::Get().GetInt64());
						}
						break;
						case Field::Type_UInt64:
						{
							Print(out, "\t\t\t{1}.push_back({2});\n", iter->m_sName, Util::RndGen::Get().GetUInt64());
						}
						break;
						case Field::Type_Float:
						{
							Print(out, "\t\t\t{1}.push_back({2}f);\n", iter->m_sName, Util::RndGen::Get().GetFloat());
						}
						break;
						case Field::Type_Double:
						{
							Print(out, "\t\t\t{1}.push_back({2});\n", iter->m_sName, Util::RndGen::Get().GetDouble());
						}
						break;
						case Field::Type_String:
						{
							String sTmp1;
							String sTmp2;

							Util::RndGen::Get().GetString(&sTmp1, 0, 64);
							for (Size i = 0; i < sTmp1.size(); i++)
							{
								if ('\\' == sTmp1[i] || '"' == sTmp1[i])
								{
									sTmp2 += "\\";
								}
								sTmp2 += sTmp1[i];
							}
							Print(out, "\t\t\t{1}.push_back(\"{2}\");\n", iter->m_sName, sTmp2);
						}
						break;
						case Field::Type_WString:
						{
							String sTmp1;
							String sTmp2;

							Util::RndGen::Get().GetString(&sTmp1, 0, 64);
							for (Size i = 0; i < sTmp1.size(); i++)
							{
								if ('\\' == sTmp1[i] || '"' == sTmp1[i])
								{
									sTmp2 += "\\";
								}
								sTmp2 += sTmp1[i];
							}
							Print(out, "\t\t\t{1}.push_back(L\"{2}\");\n", iter->m_sName, sTmp2);
						}
						break;
						case Field::Type_Object:
						{
							Print(out, "\t\t\t{1} v;\n", Field::GetStringFromType(iter->m_nType, false, iter->m_sObjectName.c_str()));
							Print(out, "\n");
							Print(out, "\t\t\tv.SetupWithSomeValues();\n");
							Print(out, "\t\t\t{1}.push_back(v);\n", iter->m_sName);
						}
						break;
					}
					Print(out, "\t\t}\n");
				}
			}
		}
		Print(out, "\t}\n");
		Print(out, "\n");
		Print(out, "\tvirtual bool Compare(const CX::IO::SimpleBuffers::IObject *pObj)\n");
		Print(out, "\t{{\n");
		Print(out, "\t\tconst {1} *pObject = dynamic_cast<const {1} *>(pObj);\n", sName);
		Print(out, "\t\n");
		Print(out, "\t\tif (NULL == pObject)\n");
		Print(out, "\t\t{{\n");
		Print(out, "\t\t\treturn false;\n");
		Print(out, "\t\t}\n");
		for (FieldsVector::const_iterator iter = pObject->m_vectorFields.begin(); iter != pObject->m_vectorFields.end(); ++iter)
		{
			if (!iter->m_bIsVector)
			{
				switch (iter->m_nType)
				{
					case Field::Type_Bool:
					case Field::Type_Int8:
					case Field::Type_UInt8:
					case Field::Type_Int16:
					case Field::Type_UInt16:
					case Field::Type_Int32:
					case Field::Type_UInt32:
					case Field::Type_Int64:
					case Field::Type_UInt64:
					case Field::Type_Float:
					case Field::Type_Double:
					{
						Print(out, "\t\tif ({1} != pObject->{1})\n", iter->m_sName);
						Print(out, "\t\t{{\n");
						Print(out, "\t\t\treturn false;\n");
						Print(out, "\t\t}\n");
					}
					break;
					case Field::Type_String:
					{
						Print(out, "\t\tif (0 != cx_strcmp({1}.c_str(), pObject->{1}.c_str()))\n", iter->m_sName);
						Print(out, "\t\t{{\n");
						Print(out, "\t\t\treturn false;\n");
						Print(out, "\t\t}\n");
					}
					break;
					case Field::Type_WString:
					{
						Print(out, "\t\tif (0 != cxw_strcmp({1}.c_str(), pObject->{1}.c_str()))\n", iter->m_sName);
						Print(out, "\t\t{{\n");
						Print(out, "\t\t\treturn false;\n");
						Print(out, "\t\t}\n");
					}
					break;
					case Field::Type_Object:
					{
						Print(out, "\t\tif (!{1}.Compare(&pObject->{1}))\n", iter->m_sName);
						Print(out, "\t\t{{\n");
						Print(out, "\t\t\treturn false;\n");
						Print(out, "\t\t}\n");
					}
					break;
				}
			}
			else
			{
				Print(out, "\t\tif ({1}.size() != pObject->{1}.size())\n", iter->m_sName);
				Print(out, "\t\t{{\n");
				Print(out, "\t\t\treturn false;\n");
				Print(out, "\t\t}\n");
				Print(out, "\t\tfor (CX::Size i = 0; i < {1}.size(); i++)\n", iter->m_sName);
				Print(out, "\t\t{{\n");
				switch (iter->m_nType)
				{
					case Field::Type_Bool:
					case Field::Type_Int8:
					case Field::Type_UInt8:
					case Field::Type_Int16:
					case Field::Type_UInt16:
					case Field::Type_Int32:
					case Field::Type_UInt32:
					case Field::Type_Int64:
					case Field::Type_UInt64:
					case Field::Type_Float:
					case Field::Type_Double:
					{
						Print(out, "\t\t\tif ({1}[i] != pObject->{1}[i])\n", iter->m_sName);
						Print(out, "\t\t\t{{\n");
						Print(out, "\t\t\t\treturn false;\n");
						Print(out, "\t\t\t}\n");
					}
					break;
					case Field::Type_String:
					{
						Print(out, "\t\t\tif (0 != cx_strcmp({1}[i].c_str(), pObject->{1}[i].c_str()))\n", iter->m_sName);
						Print(out, "\t\t\t{{\n");
						Print(out, "\t\t\t\treturn false;\n");
						Print(out, "\t\t\t}\n");
					}
					break;
					case Field::Type_WString:
					{
						Print(out, "\t\t\tif (0 != cxw_strcmp({1}[i].c_str(), pObject->{1}[i].c_str()))\n", iter->m_sName);
						Print(out, "\t\t\t{{\n");
						Print(out, "\t\t\t\treturn false;\n");
						Print(out, "\t\t\t}\n");
					}
					break;
					case Field::Type_Object:
					{
						Print(out, "\t\t\tif (!{1}[i].Compare(&pObject->{1}[i]))\n", iter->m_sName);
						Print(out, "\t\t\t{{\n");
						Print(out, "\t\t\t\treturn false;\n");
						Print(out, "\t\t\t}\n");
					}
					break;
				}
				Print(out, "\t\t}\n");
			}
		}
		Print(out, "\n");
		Print(out, "\t\treturn true;\n");
		Print(out, "\t}\n");
		Print(out, "\n");

		Print(out, "\tvirtual CX::Status Read(CX::IO::IDataReader *pReader)\n");
		Print(out, "\t{{\n");
		for (FieldsVector::const_iterator iter = pObject->m_vectorFields.begin(); iter != pObject->m_vectorFields.end(); ++iter)
		{
			if (Field::Type_Bool == iter->m_nType && iter->m_bIsVector)
			{
				Print(out, "\t\tCX::Bool bValue;\n");
				break;
			}
		}
		for (FieldsVector::const_iterator iter = pObject->m_vectorFields.begin(); iter != pObject->m_vectorFields.end(); ++iter)
		{
			if (Field::Type_Int8 == iter->m_nType || Field::Type_UInt8 == iter->m_nType || 
			     Field::Type_Int16 == iter->m_nType || Field::Type_UInt16 == iter->m_nType || 
			     Field::Type_Int32 == iter->m_nType || Field::Type_UInt32 == iter->m_nType || 
			     Field::Type_Int64 == iter->m_nType || Field::Type_UInt64 == iter->m_nType)
			{
				Print(out, "\t\tCX::Int64 nValue;\n");
				break;
			}
		}
		for (FieldsVector::const_iterator iter = pObject->m_vectorFields.begin(); iter != pObject->m_vectorFields.end(); ++iter)
		{
			if (Field::Type_Float == iter->m_nType || Field::Type_Double == iter->m_nType)
			{
				Print(out, "\t\tCX::Double lfValue;\n");
				break;
			}
		}
		for (FieldsVector::const_iterator iter = pObject->m_vectorFields.begin(); iter != pObject->m_vectorFields.end(); ++iter)
		{
			if (Field::Type_String == iter->m_nType && iter->m_bIsVector)
			{
				Print(out, "\t\tCX::String sValue;\n");
				break;
			}
		}
		for (FieldsVector::const_iterator iter = pObject->m_vectorFields.begin(); iter != pObject->m_vectorFields.end(); ++iter)
		{
			if (Field::Type_WString == iter->m_nType && iter->m_bIsVector)
			{
				Print(out, "\t\tCX::WString wsValue;\n");
				break;
			}
		}
		Print(out, "\t\tCX::Status status;\n");
		Print(out, "\n");
		Print(out, "\t\tReset();\n");
		for (FieldsVector::const_iterator iter = pObject->m_vectorFields.begin(); iter != pObject->m_vectorFields.end(); ++iter)
		{
			if (!iter->m_bIsVector)
			{
				switch (iter->m_nType)
				{
					case Field::Type_Bool:
					{
						Print(out, "\t\tif ((status = pReader->ReadObjectBool(\"{1}\", &{1})).IsNOK())\n", iter->m_sName);
						Print(out, "\t\t{{\n");
						Print(out, "\t\t\treturn status;\n");
						Print(out, "\t\t}\n");
					}
					break;
					case Field::Type_Int8:
					{
						Print(out, "\t\tif ((status = pReader->ReadObjectInt(\"{1}\", &nValue)).IsNOK())\n", iter->m_sName);
						Print(out, "\t\t{{\n");
						Print(out, "\t\t\treturn status;\n");
						Print(out, "\t\t}\n");
						Print(out, "\t\t{1} = (CX::Int8)nValue;\n", iter->m_sName);
					}
					break;
					case Field::Type_UInt8:
					{
						Print(out, "\t\tif ((status = pReader->ReadObjectInt(\"{1}\", &nValue)).IsNOK())\n", iter->m_sName);
						Print(out, "\t\t{{\n");
						Print(out, "\t\t\treturn status;\n");
						Print(out, "\t\t}\n");
						Print(out, "\t\t{1} = (CX::UInt8)nValue;\n", iter->m_sName);
					}
					break;
					case Field::Type_Int16:
					{
						Print(out, "\t\tif ((status = pReader->ReadObjectInt(\"{1}\", &nValue)).IsNOK())\n", iter->m_sName);
						Print(out, "\t\t{{\n");
						Print(out, "\t\t\treturn status;\n");
						Print(out, "\t\t}\n");
						Print(out, "\t\t{1} = (CX::Int16)nValue;\n", iter->m_sName);
					}
					break;
					case Field::Type_UInt16:
					{
						Print(out, "\t\tif ((status = pReader->ReadObjectInt(\"{1}\", &nValue)).IsNOK())\n", iter->m_sName);
						Print(out, "\t\t{{\n");
						Print(out, "\t\t\treturn status;\n");
						Print(out, "\t\t}\n");
						Print(out, "\t\t{1} = (CX::UInt16)nValue;\n", iter->m_sName);
					}
					break;
					case Field::Type_Int32:
					{
						Print(out, "\t\tif ((status = pReader->ReadObjectInt(\"{1}\", &nValue)).IsNOK())\n", iter->m_sName);
						Print(out, "\t\t{{\n");
						Print(out, "\t\t\treturn status;\n");
						Print(out, "\t\t}\n");
						Print(out, "\t\t{1} = (CX::Int32)nValue;\n", iter->m_sName);
					}
					break;
					case Field::Type_UInt32:
					{
						Print(out, "\t\tif ((status = pReader->ReadObjectInt(\"{1}\", &nValue)).IsNOK())\n", iter->m_sName);
						Print(out, "\t\t{{\n");
						Print(out, "\t\t\treturn status;\n");
						Print(out, "\t\t}\n");
						Print(out, "\t\t{1} = (CX::UInt32)nValue;\n", iter->m_sName);
					}
					break;
					case Field::Type_Int64:
					{
						Print(out, "\t\tif ((status = pReader->ReadObjectInt(\"{1}\", &nValue)).IsNOK())\n", iter->m_sName);
						Print(out, "\t\t{{\n");
						Print(out, "\t\t\treturn status;\n");
						Print(out, "\t\t}\n");
						Print(out, "\t\t{1} = (CX::Int64)nValue;\n", iter->m_sName);
					}
					break;
					case Field::Type_UInt64:
					{
						Print(out, "\t\tif ((status = pReader->ReadObjectInt(\"{1}\", &nValue)).IsNOK())\n", iter->m_sName);
						Print(out, "\t\t{{\n");
						Print(out, "\t\t\treturn status;\n");
						Print(out, "\t\t}\n");
						Print(out, "\t\t{1} = (CX::UInt64)nValue;\n", iter->m_sName);
					}
					break;
					case Field::Type_Float:
					{
						Print(out, "\t\tif ((status = pReader->ReadObjectReal(\"{1}\", &lfValue)).IsNOK())\n", iter->m_sName);
						Print(out, "\t\t{{\n");
						Print(out, "\t\t\treturn status;\n");
						Print(out, "\t\t}\n");
						Print(out, "\t\t{1} = (CX::Float)lfValue;\n", iter->m_sName);
					}
					break;
					case Field::Type_Double:
					{
						Print(out, "\t\tif ((status = pReader->ReadObjectReal(\"{1}\", &lfValue)).IsNOK())\n", iter->m_sName);
						Print(out, "\t\t{{\n");
						Print(out, "\t\t\treturn status;\n");
						Print(out, "\t\t}\n");
						Print(out, "\t\t{1} = (CX::Double)lfValue;\n", iter->m_sName);
					}
					break;
					case Field::Type_String:
					{
						Print(out, "\t\tif ((status = pReader->ReadObjectString(\"{1}\", &{1})).IsNOK())\n", iter->m_sName);
						Print(out, "\t\t{{\n");
						Print(out, "\t\t\treturn status;\n");
						Print(out, "\t\t}\n");
					}
					break;
					case Field::Type_WString:
					{
						Print(out, "\t\tif ((status = pReader->ReadObjectWString(\"{1}\", &{1})).IsNOK())\n", iter->m_sName);
						Print(out, "\t\t{{\n");
						Print(out, "\t\t\treturn status;\n");
						Print(out, "\t\t}\n");
					}
					break;
					case Field::Type_Object:
					{
						Print(out, "\t\tif ((status = pReader->BeginObjectObject(\"{1}\")).IsNOK())\n", iter->m_sName);
						Print(out, "\t\t{{\n");
						Print(out, "\t\t\treturn status;\n");
						Print(out, "\t\t}\n");
						Print(out, "\t\tif ((status = {1}.Read(pReader)).IsNOK())\n", iter->m_sName);
						Print(out, "\t\t{{\n");
						Print(out, "\t\t\treturn status;\n");
						Print(out, "\t\t}\n");
						Print(out, "\t\tif ((status = pReader->EndObjectObject()).IsNOK())\n");
						Print(out, "\t\t{{\n");
						Print(out, "\t\t\treturn status;\n");
						Print(out, "\t\t}\n");
					}
					break;
				}
			}
			else
			{
				Print(out, "\t\tif ((status = pReader->BeginObjectArray(\"{1}\")).IsNOK())\n", iter->m_sName);
				Print(out, "\t\t{{\n");
				Print(out, "\t\t\treturn status;\n");
				Print(out, "\t\t}\n");
				Print(out, "\t\tfor (;;)\n");
				Print(out, "\t\t{{\n");
				switch (iter->m_nType)
				{
					case Field::Type_Bool:
					{
						Print(out, "\t\t\tif ((status = pReader->ReadArrayBool(&bValue)).IsNOK())\n");
						Print(out, "\t\t\t{{\n");
						Print(out, "\t\t\t\tif (CX::Status_NoMoreItems == status.GetCode())\n");
						Print(out, "\t\t\t\t{{\n");
						Print(out, "\t\t\t\t\tbreak;\n");
						Print(out, "\t\t\t\t}\n");
						Print(out, "\t\t\t\treturn status;\n");
						Print(out, "\t\t\t}\n");
						Print(out, "\t\t\t{1}.push_back(bValue);\n", iter->m_sName);
					}
					break;
					case Field::Type_Int8:
					{
						Print(out, "\t\t\tif ((status = pReader->ReadArrayInt(&nValue)).IsNOK())\n");
						Print(out, "\t\t\t{{\n");
						Print(out, "\t\t\t\tif (CX::Status_NoMoreItems == status.GetCode())\n");
						Print(out, "\t\t\t\t{{\n");
						Print(out, "\t\t\t\t\tbreak;\n");
						Print(out, "\t\t\t\t}\n");
						Print(out, "\t\t\t\treturn status;\n");
						Print(out, "\t\t\t}\n");
						Print(out, "\t\t\t{1}.push_back((CX::Int8)nValue);\n", iter->m_sName);
					}
					break;
					case Field::Type_UInt8:
					{
						Print(out, "\t\t\tif ((status = pReader->ReadArrayInt(&nValue)).IsNOK())\n");
						Print(out, "\t\t\t{{\n");
						Print(out, "\t\t\t\tif (CX::Status_NoMoreItems == status.GetCode())\n");
						Print(out, "\t\t\t\t{{\n");
						Print(out, "\t\t\t\t\tbreak;\n");
						Print(out, "\t\t\t\t}\n");
						Print(out, "\t\t\t\treturn status;\n");
						Print(out, "\t\t\t}\n");
						Print(out, "\t\t\t{1}.push_back((CX::UInt8)nValue);\n", iter->m_sName);
					}
					break;
					case Field::Type_Int16:
					{
						Print(out, "\t\t\tif ((status = pReader->ReadArrayInt(&nValue)).IsNOK())\n");
						Print(out, "\t\t\t{{\n");
						Print(out, "\t\t\t\tif (CX::Status_NoMoreItems == status.GetCode())\n");
						Print(out, "\t\t\t\t{{\n");
						Print(out, "\t\t\t\t\tbreak;\n");
						Print(out, "\t\t\t\t}\n");
						Print(out, "\t\t\t\treturn status;\n");
						Print(out, "\t\t\t}\n");
						Print(out, "\t\t\t{1}.push_back((CX::Int16)nValue);\n", iter->m_sName);
					}
					break;
					case Field::Type_UInt16:
					{
						Print(out, "\t\t\tif ((status = pReader->ReadArrayInt(&nValue)).IsNOK())\n");
						Print(out, "\t\t\t{{\n");
						Print(out, "\t\t\t\tif (CX::Status_NoMoreItems == status.GetCode())\n");
						Print(out, "\t\t\t\t{{\n");
						Print(out, "\t\t\t\t\tbreak;\n");
						Print(out, "\t\t\t\t}\n");
						Print(out, "\t\t\t\treturn status;\n");
						Print(out, "\t\t\t}\n");
						Print(out, "\t\t\t{1}.push_back((CX::UInt16)nValue);\n", iter->m_sName);
					}
					break;
					case Field::Type_Int32:
					{
						Print(out, "\t\t\tif ((status = pReader->ReadArrayInt(&nValue)).IsNOK())\n");
						Print(out, "\t\t\t{{\n");
						Print(out, "\t\t\t\tif (CX::Status_NoMoreItems == status.GetCode())\n");
						Print(out, "\t\t\t\t{{\n");
						Print(out, "\t\t\t\t\tbreak;\n");
						Print(out, "\t\t\t\t}\n");
						Print(out, "\t\t\t\treturn status;\n");
						Print(out, "\t\t\t}\n");
						Print(out, "\t\t\t{1}.push_back((CX::Int32)nValue);\n", iter->m_sName);
					}
					break;
					case Field::Type_UInt32:
					{
						Print(out, "\t\t\tif ((status = pReader->ReadArrayInt(&nValue)).IsNOK())\n");
						Print(out, "\t\t\t{{\n");
						Print(out, "\t\t\t\tif (CX::Status_NoMoreItems == status.GetCode())\n");
						Print(out, "\t\t\t\t{{\n");
						Print(out, "\t\t\t\t\tbreak;\n");
						Print(out, "\t\t\t\t}\n");
						Print(out, "\t\t\t\treturn status;\n");
						Print(out, "\t\t\t}\n");
						Print(out, "\t\t\t{1}.push_back((CX::UInt32)nValue);\n", iter->m_sName);
					}
					break;
					case Field::Type_Int64:
					{
						Print(out, "\t\t\tif ((status = pReader->ReadArrayInt(&nValue)).IsNOK())\n");
						Print(out, "\t\t\t{{\n");
						Print(out, "\t\t\t\tif (CX::Status_NoMoreItems == status.GetCode())\n");
						Print(out, "\t\t\t\t{{\n");
						Print(out, "\t\t\t\t\tbreak;\n");
						Print(out, "\t\t\t\t}\n");
						Print(out, "\t\t\t\treturn status;\n");
						Print(out, "\t\t\t}\n");
						Print(out, "\t\t\t{1}.push_back((CX::Int64)nValue);\n", iter->m_sName);
					}
					break;
					case Field::Type_UInt64:
					{
						Print(out, "\t\t\tif ((status = pReader->ReadArrayInt(&nValue)).IsNOK())\n");
						Print(out, "\t\t\t{{\n");
						Print(out, "\t\t\t\tif (CX::Status_NoMoreItems == status.GetCode())\n");
						Print(out, "\t\t\t\t{{\n");
						Print(out, "\t\t\t\t\tbreak;\n");
						Print(out, "\t\t\t\t}\n");
						Print(out, "\t\t\t\treturn status;\n");
						Print(out, "\t\t\t}\n");
						Print(out, "\t\t\t{1}.push_back((CX::UInt64)nValue);\n", iter->m_sName);
					}
					break;
					case Field::Type_Float:
					{
						Print(out, "\t\t\tif ((status = pReader->ReadArrayReal(&lfValue)).IsNOK())\n");
						Print(out, "\t\t\t{{\n");
						Print(out, "\t\t\t\tif (CX::Status_NoMoreItems == status.GetCode())\n");
						Print(out, "\t\t\t\t{{\n");
						Print(out, "\t\t\t\t\tbreak;\n");
						Print(out, "\t\t\t\t}\n");
						Print(out, "\t\t\t\treturn status;\n");
						Print(out, "\t\t\t}\n");
						Print(out, "\t\t\t{1}.push_back((CX::Float)lfValue);\n", iter->m_sName);
					}
					break;
					case Field::Type_Double:
					{
						Print(out, "\t\t\tif ((status = pReader->ReadArrayReal(&lfValue)).IsNOK())\n");
						Print(out, "\t\t\t{{\n");
						Print(out, "\t\t\t\tif (CX::Status_NoMoreItems == status.GetCode())\n");
						Print(out, "\t\t\t\t{{\n");
						Print(out, "\t\t\t\t\tbreak;\n");
						Print(out, "\t\t\t\t}\n");
						Print(out, "\t\t\t\treturn status;\n");
						Print(out, "\t\t\t}\n");
						Print(out, "\t\t\t{1}.push_back((CX::Double)lfValue);\n", iter->m_sName);
					}
					break;
					case Field::Type_String:
					{
						Print(out, "\t\t\tif ((status = pReader->ReadArrayString(&sValue)).IsNOK())\n");
						Print(out, "\t\t\t{{\n");
						Print(out, "\t\t\t\tif (CX::Status_NoMoreItems == status.GetCode())\n");
						Print(out, "\t\t\t\t{{\n");
						Print(out, "\t\t\t\t\tbreak;\n");
						Print(out, "\t\t\t\t}\n");
						Print(out, "\t\t\t\treturn status;\n");
						Print(out, "\t\t\t}\n");
						Print(out, "\t\t\t{1}.push_back(sValue);\n", iter->m_sName);
					}
					break;
					case Field::Type_WString:
					{
						Print(out, "\t\t\tif ((status = pReader->ReadArrayWString(&wsValue)).IsNOK())\n");
						Print(out, "\t\t\t{{\n");
						Print(out, "\t\t\t\tif (CX::Status_NoMoreItems == status.GetCode())\n");
						Print(out, "\t\t\t\t{{\n");
						Print(out, "\t\t\t\t\tbreak;\n");
						Print(out, "\t\t\t\t}\n");
						Print(out, "\t\t\t\treturn status;\n");
						Print(out, "\t\t\t}\n");
						Print(out, "\t\t\t{1}.push_back(wsValue);\n", iter->m_sName);
					}
					break;
					case Field::Type_Object:
					{
						Print(out, "\t\t\t{1} tmp;\n", Field::GetStringFromType(iter->m_nType, false, iter->m_sObjectName.c_str()));
						Print(out, "\n");
						Print(out, "\t\t\tif ((status = pReader->BeginArrayObject()).IsNOK())\n");
						Print(out, "\t\t\t{{\n");
						Print(out, "\t\t\t\tif (CX::Status_NoMoreItems == status.GetCode())\n");
						Print(out, "\t\t\t\t{{\n");
						Print(out, "\t\t\t\t\tbreak;\n");
						Print(out, "\t\t\t\t}\n");
						Print(out, "\t\t\t\treturn status;\n");
						Print(out, "\t\t\t}\n");
						Print(out, "\t\t\tif ((status = tmp.Read(pReader)).IsNOK())\n");
						Print(out, "\t\t\t{{\n");
						Print(out, "\t\t\t\treturn status;\n");
						Print(out, "\t\t\t}\n");
						Print(out, "\t\t\t{1}.push_back(tmp);\n", iter->m_sName);
						Print(out, "\t\t\tif ((status = pReader->EndArrayObject()).IsNOK())\n");
						Print(out, "\t\t\t{{\n");
						Print(out, "\t\t\t\treturn status;\n");
						Print(out, "\t\t\t}\n");
					}
					break;
				}
				Print(out, "\t\t}\n");
				Print(out, "\t\tif ((status = pReader->EndObjectArray()).IsNOK())\n");
				Print(out, "\t\t{{\n");
				Print(out, "\t\t\treturn status;\n");
				Print(out, "\t\t}\n");
			}
		}
		Print(out, "\n");
		Print(out, "\t\treturn CX::Status();\n");
		Print(out, "\t}\n");
		Print(out, "\n");

		Print(out, "\tvirtual CX::Status Write(CX::IO::IDataWriter *pWriter) const\n");
		Print(out, "\t{{\n");
		Print(out, "\t\tCX::Status status;\n");
		Print(out, "\n");
		for (FieldsVector::const_iterator iter = pObject->m_vectorFields.begin(); iter != pObject->m_vectorFields.end(); ++iter)
		{
			if (!iter->m_bIsVector)
			{
				switch (iter->m_nType)
				{
					case Field::Type_Bool:
					{
						Print(out, "\t\tif ((status = pWriter->WriteObjectBool(\"{1}\", {1})).IsNOK())\n", iter->m_sName);
						Print(out, "\t\t{{\n");
						Print(out, "\t\t\treturn status;\n");
						Print(out, "\t\t}\n");
					}
					break;
					case Field::Type_Int8:
					{
						Print(out, "\t\tif ((status = pWriter->WriteObjectInt(\"{1}\", (CX::Int64){1})).IsNOK())\n", iter->m_sName);
						Print(out, "\t\t{{\n");
						Print(out, "\t\t\treturn status;\n");
						Print(out, "\t\t}\n");
					}
					break;
					case Field::Type_UInt8:
					{
						Print(out, "\t\tif ((status = pWriter->WriteObjectInt(\"{1}\", (CX::Int64){1})).IsNOK())\n", iter->m_sName);
						Print(out, "\t\t{{\n");
						Print(out, "\t\t\treturn status;\n");
						Print(out, "\t\t}\n");
					}
					break;
					case Field::Type_Int16:
					{
						Print(out, "\t\tif ((status = pWriter->WriteObjectInt(\"{1}\", (CX::Int64){1})).IsNOK())\n", iter->m_sName);
						Print(out, "\t\t{{\n");
						Print(out, "\t\t\treturn status;\n");
						Print(out, "\t\t}\n");
					}
					break;
					case Field::Type_UInt16:
					{
						Print(out, "\t\tif ((status = pWriter->WriteObjectInt(\"{1}\", (CX::Int64){1})).IsNOK())\n", iter->m_sName);
						Print(out, "\t\t{{\n");
						Print(out, "\t\t\treturn status;\n");
						Print(out, "\t\t}\n");
					}
					break;
					case Field::Type_Int32:
					{
						Print(out, "\t\tif ((status = pWriter->WriteObjectInt(\"{1}\", (CX::Int64){1})).IsNOK())\n", iter->m_sName);
						Print(out, "\t\t{{\n");
						Print(out, "\t\t\treturn status;\n");
						Print(out, "\t\t}\n");
					}
					break;
					case Field::Type_UInt32:
					{
						Print(out, "\t\tif ((status = pWriter->WriteObjectInt(\"{1}\", (CX::Int64){1})).IsNOK())\n", iter->m_sName);
						Print(out, "\t\t{{\n");
						Print(out, "\t\t\treturn status;\n");
						Print(out, "\t\t}\n");
					}
					break;
					case Field::Type_Int64:
					{
						Print(out, "\t\tif ((status = pWriter->WriteObjectInt(\"{1}\", (CX::Int64){1})).IsNOK())\n", iter->m_sName);
						Print(out, "\t\t{{\n");
						Print(out, "\t\t\treturn status;\n");
						Print(out, "\t\t}\n");
					}
					break;
					case Field::Type_UInt64:
					{
						Print(out, "\t\tif ((status = pWriter->WriteObjectInt(\"{1}\", (CX::Int64){1})).IsNOK())\n", iter->m_sName);
						Print(out, "\t\t{{\n");
						Print(out, "\t\t\treturn status;\n");
						Print(out, "\t\t}\n");
					}
					break;
					case Field::Type_Float:
					{
						Print(out, "\t\tif ((status = pWriter->WriteObjectReal(\"{1}\", (CX::Double){1})).IsNOK())\n", iter->m_sName);
						Print(out, "\t\t{{\n");
						Print(out, "\t\t\treturn status;\n");
						Print(out, "\t\t}\n");
					}
					break;
					case Field::Type_Double:
					{
						Print(out, "\t\tif ((status = pWriter->WriteObjectReal(\"{1}\", (CX::Double){1})).IsNOK())\n", iter->m_sName);
						Print(out, "\t\t{{\n");
						Print(out, "\t\t\treturn status;\n");
						Print(out, "\t\t}\n");
					}
					break;
					case Field::Type_String:
					{
						Print(out, "\t\tif ((status = pWriter->WriteObjectString(\"{1}\", {1}.c_str())).IsNOK())\n", iter->m_sName);
						Print(out, "\t\t{{\n");
						Print(out, "\t\t\treturn status;\n");
						Print(out, "\t\t}\n");
					}
					break;
					case Field::Type_WString:
					{
						Print(out, "\t\tif ((status = pWriter->WriteObjectWString(\"{1}\", {1}.c_str())).IsNOK())\n", iter->m_sName);
						Print(out, "\t\t{{\n");
						Print(out, "\t\t\treturn status;\n");
						Print(out, "\t\t}\n");
					}
					break;
					case Field::Type_Object:
					{
						Print(out, "\t\tif ((status = pWriter->BeginObjectObject(\"{1}\")).IsNOK())\n", iter->m_sName);
						Print(out, "\t\t{{\n");
						Print(out, "\t\t\treturn status;\n");
						Print(out, "\t\t}\n");
						Print(out, "\t\tif ((status = {1}.Write(pWriter)).IsNOK())\n", iter->m_sName);
						Print(out, "\t\t{{\n");
						Print(out, "\t\t\treturn status;\n");
						Print(out, "\t\t}\n");
						Print(out, "\t\tif ((status = pWriter->EndObjectObject()).IsNOK())\n", iter->m_sName);
						Print(out, "\t\t{{\n");
						Print(out, "\t\t\treturn status;\n");
						Print(out, "\t\t}\n");
					}
					break;
				}
			}
			else
			{
				Print(out, "\t\tif ((status = pWriter->BeginObjectArray(\"{1}\")).IsNOK())\n", iter->m_sName);
				Print(out, "\t\t{{\n");
				Print(out, "\t\t\treturn status;\n");
				Print(out, "\t\t}\n");
				Print(out, "\t\tfor ({2}::const_iterator iter = {1}.begin(); iter != {1}.end(); ++iter)\n", iter->m_sName, 
				      Field::GetStringFromType(iter->m_nType, iter->m_bIsVector, iter->m_sObjectName.c_str()));
				Print(out, "\t\t{{\n");
				switch (iter->m_nType)
				{
					case Field::Type_Bool:
					{
						Print(out, "\t\t\tif ((status = pWriter->WriteArrayBool(*iter)).IsNOK())\n");
						Print(out, "\t\t\t{{\n");
						Print(out, "\t\t\t\treturn status;\n");
						Print(out, "\t\t\t}\n");
					}
					break;
					case Field::Type_Int8:
					{
						Print(out, "\t\t\tif ((status = pWriter->WriteArrayInt((CX::Int64)*iter)).IsNOK())\n");
						Print(out, "\t\t\t{{\n");
						Print(out, "\t\t\t\treturn status;\n");
						Print(out, "\t\t\t}\n");
					}
					break;
					case Field::Type_UInt8:
					{
						Print(out, "\t\t\tif ((status = pWriter->WriteArrayInt((CX::Int64)*iter)).IsNOK())\n");
						Print(out, "\t\t\t{{\n");
						Print(out, "\t\t\t\treturn status;\n");
						Print(out, "\t\t\t}\n");
					}
					break;
					case Field::Type_Int16:
					{
						Print(out, "\t\t\tif ((status = pWriter->WriteArrayInt((CX::Int64)*iter)).IsNOK())\n");
						Print(out, "\t\t\t{{\n");
						Print(out, "\t\t\t\treturn status;\n");
						Print(out, "\t\t\t}\n");
					}
					break;
					case Field::Type_UInt16:
					{
						Print(out, "\t\t\tif ((status = pWriter->WriteArrayInt((CX::Int64)*iter)).IsNOK())\n");
						Print(out, "\t\t\t{{\n");
						Print(out, "\t\t\t\treturn status;\n");
						Print(out, "\t\t\t}\n");
					}
					break;
					case Field::Type_Int32:
					{
						Print(out, "\t\t\tif ((status = pWriter->WriteArrayInt((CX::Int64)*iter)).IsNOK())\n");
						Print(out, "\t\t\t{{\n");
						Print(out, "\t\t\t\treturn status;\n");
						Print(out, "\t\t\t}\n");
					}
					break;
					case Field::Type_UInt32:
					{
						Print(out, "\t\t\tif ((status = pWriter->WriteArrayInt((CX::Int64)*iter)).IsNOK())\n");
						Print(out, "\t\t\t{{\n");
						Print(out, "\t\t\t\treturn status;\n");
						Print(out, "\t\t\t}\n");
					}
					break;
					case Field::Type_Int64:
					{
						Print(out, "\t\t\tif ((status = pWriter->WriteArrayInt((CX::Int64)*iter)).IsNOK())\n");
						Print(out, "\t\t\t{{\n");
						Print(out, "\t\t\t\treturn status;\n");
						Print(out, "\t\t\t}\n");
					}
					break;
					case Field::Type_UInt64:
					{
						Print(out, "\t\t\tif ((status = pWriter->WriteArrayInt((CX::Int64)*iter)).IsNOK())\n");
						Print(out, "\t\t\t{{\n");
						Print(out, "\t\t\t\treturn status;\n");
						Print(out, "\t\t\t}\n");
					}
					break;
					case Field::Type_Float:
					{
						Print(out, "\t\t\tif ((status = pWriter->WriteArrayReal((CX::Double)*iter)).IsNOK())\n");
						Print(out, "\t\t\t{{\n");
						Print(out, "\t\t\t\treturn status;\n");
						Print(out, "\t\t\t}\n");
					}
					break;
					case Field::Type_Double:
					{
						Print(out, "\t\t\tif ((status = pWriter->WriteArrayReal((CX::Double)*iter)).IsNOK())\n");
						Print(out, "\t\t\t{{\n");
						Print(out, "\t\t\t\treturn status;\n");
						Print(out, "\t\t\t}\n");
					}
					break;
					case Field::Type_String:
					{
						Print(out, "\t\t\tif ((status = pWriter->WriteArrayString((*iter).c_str())).IsNOK())\n");
						Print(out, "\t\t\t{{\n");
						Print(out, "\t\t\t\treturn status;\n");
						Print(out, "\t\t\t}\n");
					}
					break;
					case Field::Type_WString:
					{
						Print(out, "\t\t\tif ((status = pWriter->WriteArrayWString((*iter).c_str())).IsNOK())\n");
						Print(out, "\t\t\t{{\n");
						Print(out, "\t\t\t\treturn status;\n");
						Print(out, "\t\t\t}\n");
					}
					break;
					case Field::Type_Object:
					{
						Print(out, "\t\t\tif ((status = pWriter->BeginArrayObject()).IsNOK())\n");
						Print(out, "\t\t\t{{\n");
						Print(out, "\t\t\t\treturn status;\n");
						Print(out, "\t\t\t}\n");
						Print(out, "\t\t\tif ((status = (*iter).Write(pWriter)).IsNOK())\n");
						Print(out, "\t\t\t{{\n");
						Print(out, "\t\t\t\treturn status;\n");
						Print(out, "\t\t\t}\n");
						Print(out, "\t\t\tif ((status = pWriter->EndArrayObject()).IsNOK())\n");
						Print(out, "\t\t\t{{\n");
						Print(out, "\t\t\t\treturn status;\n");
						Print(out, "\t\t\t}\n");
					}
					break;
				}
				Print(out, "\t\t}\n");
				Print(out, "\t\tif ((status = pWriter->EndObjectArray()).IsNOK())\n");
				Print(out, "\t\t{{\n");
				Print(out, "\t\t\treturn status;\n");
				Print(out, "\t\t}\n");
			}
		}
		Print(out, "\n");
		Print(out, "\t\treturn CX::Status();\n");
		Print(out, "\t}\n");
		Print(out, "\n");
		
		Print(out, "};\n");
		Print(out, "\n");
		Print(out, "typedef CX::Vector<{1}>::Type {1}Array;\n", sName);
		Print(out, "\n");
		for (StringArray::reverse_iterator iter = vectorNamespaces.rbegin(); iter != vectorNamespaces.rend(); ++iter)
		{
			Print(out, "}//namespace {1}\n", *iter);
			Print(out, "\n");
		}

		return Status();
	}

	static Status GenerateCPPTestWithPath(const Object *pObject, const Char *szPath)
	{
		FileOutputStream fos(szPath);

		if (!fos.IsOK())
		{
			return Status(Status_CreateFailed, "Failed to create file '{1}'", szPath);
		}

		return GenerateCPP((IOutputStream *)&fos, pObject);
	}

	template <typename OUTPUT>
	static Status GenerateCPPTest(OUTPUT out, const Object *pObject)
	{
		const Char *pPos;
		String     sTmp;

		Print(out, "\n");
		Print(out, "#pragma once\n");
		Print(out, "\n");
		Print(out, "\n");
		pPos = pObject->m_sName.c_str();
		while (0 != *pPos)
		{
			if ('.' == *pPos)
			{
				sTmp += "/";
			}
			else
			{
				sTmp += *pPos;
			}
			pPos;
		}
		Print(out, "#include \"{1}.hpp\"", sTmp);
		Print(out, "#include \"CX/Util/RndGen.hpp\"\n");
		Print(out, "\n");
		Print(out, "\n");
		sTmp.clear();
		pPos = pObject->m_sName.c_str();
		while (0 != *pPos)
		{
			if ('.' == *pPos)
			{
				sTmp += "_";
			}
			else
			{
				sTmp += *pPos;
			}
			pPos;
		}
		

		return Status();
	}

private:

	static void ExtractObjectName(const Char *szFullName, String *psName, StringArray *pVectorNamespaces)
	{
		const Char *pStartPos;
		const Char *pPos;

		psName->clear();
		pVectorNamespaces->clear();
		pPos = pStartPos = szFullName;
		for (;;)
		{
			if ('.' == *pPos)
			{
				pVectorNamespaces->push_back(String(pStartPos, pPos - pStartPos));
				pPos++;
				pStartPos = pPos;
			}
			else
			if (0 == *pPos)
			{
				psName->assign(pStartPos, pPos - pStartPos);
				
				break;
			}
			else
			{
				pPos++;
			}
		}
	}

};

}//namespace SimpleBuffers

}//namespace IO

}//namespace CX

