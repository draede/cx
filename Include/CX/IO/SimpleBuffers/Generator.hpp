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

	static Status GenerateProtoWithPath(Object *pObject, const Char *szPath)
	{
		FileOutputStream fos(szPath);

		if (!fos.IsOK())
		{
			return Status(Status_CreateFailed, "Failed to create file '{1}'", szPath);
		}

		return GenerateProto((IOutputStream *)&fos, pObject);
	}

	template <typename OUTPUT>
	static Status GenerateProto(OUTPUT out, Object *pObject)
	{
		Set<String>::Type  setImports;

		Print(out, "\n");
		for (FieldsVector::const_iterator iter = pObject->m_vectorFields.begin(); iter != pObject->m_vectorFields.end(); ++iter)
		{
			if (Field::Type_Object == iter->m_nType && setImports.end() == setImports.find(iter->m_sObjectName))
			{
				setImports.insert(iter->m_sObjectName);
				Print(out, "import \"{1}.sb\"\n", iter->m_sObjectName);
			}
		}
		if (!setImports.empty())
		{
			Print(out, "\n");
		}
		Print(out, "object {1}\n", pObject->m_sName);
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

	static Status GenerateCPPWithPath(Object *pObject, const Char *szPath)
	{
		FileOutputStream fos(szPath);

		if (!fos.IsOK())
		{
			return Status(Status_CreateFailed, "Failed to create file '{1}'", szPath);
		}

		return GenerateCPP((IOutputStream *)&fos, pObject);
	}

	template <typename OUTPUT>
	static Status GenerateCPP(OUTPUT out, Object *pObject)
	{
		Set<String>::Type  setIncludes;

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
				Print(out, "#include \"{1}.hpp\"\n", iter->m_sObjectName);
			}
		}
		Print(out, "\n");
		Print(out, "\n");
		Print(out, "class {1} : public CX::IO::SimpleBuffers::IObject\n", pObject->m_sName);
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
		Print(out, "\tvirtual bool Compare(const CX::IO::SimpleBuffers::IObject *pObj)\n");
		Print(out, "\t{{\n");
		Print(out, "\t\tconst {1} *pObject = dynamic_cast<const {1} *>(pObj);\n", pObject->m_sName);
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
						Print(out, "\t\tif ({1}[i] != pObject->{1}[i])\n", iter->m_sName);
						Print(out, "\t\t{{\n");
						Print(out, "\t\t\treturn false;\n");
						Print(out, "\t\t}\n");
					}
					break;
					case Field::Type_String:
					{
						Print(out, "\t\tif (0 != cx_strcmp({1}[i].c_str(), pObject->{1}[i].c_str()))\n", iter->m_sName);
						Print(out, "\t\t{{\n");
						Print(out, "\t\t\treturn false;\n");
						Print(out, "\t\t}\n");
					}
					break;
					case Field::Type_WString:
					{
						Print(out, "\t\tif (0 != cxw_strcmp({1}[i].c_str(), pObject->{1}[i].c_str()))\n", iter->m_sName);
						Print(out, "\t\t{{\n");
						Print(out, "\t\t\treturn false;\n");
						Print(out, "\t\t}\n");
					}
					break;
					case Field::Type_Object:
					{
						Print(out, "\t\tif (!{1}[i].Compare(&pObject->{1}[i]))\n", iter->m_sName);
						Print(out, "\t\t{{\n");
						Print(out, "\t\t\treturn false;\n");
						Print(out, "\t\t}\n");
					}
					break;
				}
				Print(out, "\t\t}\n");
			}
		}
		Print(out, "\n");
		Print(out, "\treturn true;\n");
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
			     Field::Type_UInt64 == iter->m_nType)
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
				Print(out, "\t\tfor (;;)\n");
				Print(out, "\t\t{{\n");
				switch (iter->m_nType)
				{
					case Field::Type_Bool:
					{
						Print(out, "\t\t\tif ((status = pReader->ReadObjectBool(\"{1}\", &bValue)).IsNOK())\n", iter->m_sName);
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
						Print(out, "\t\t\tif ((status = pReader->ReadObjectInt(\"{1}\", &nValue)).IsNOK())\n", iter->m_sName);
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
						Print(out, "\t\t\tif ((status = pReader->ReadObjectInt(\"{1}\", &nValue)).IsNOK())\n", iter->m_sName);
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
						Print(out, "\t\t\tif ((status = pReader->ReadObjectInt(\"{1}\", &nValue)).IsNOK())\n", iter->m_sName);
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
						Print(out, "\t\t\tif ((status = pReader->ReadObjectInt(\"{1}\", &nValue)).IsNOK())\n", iter->m_sName);
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
						Print(out, "\t\t\tif ((status = pReader->ReadObjectInt(\"{1}\", &nValue)).IsNOK())\n", iter->m_sName);
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
						Print(out, "\t\t\tif ((status = pReader->ReadObjectInt(\"{1}\", &nValue)).IsNOK())\n", iter->m_sName);
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
						Print(out, "\t\t\tif ((status = pReader->ReadObjectInt(\"{1}\", &nValue)).IsNOK())\n", iter->m_sName);
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
						Print(out, "\t\t\tif ((status = pReader->ReadObjectInt(\"{1}\", &nValue)).IsNOK())\n", iter->m_sName);
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
						Print(out, "\t\t\tif ((status = pReader->ReadObjectReal(\"{1}\", &lfValue)).IsNOK())\n", iter->m_sName);
						Print(out, "\t\t\t{{\n");
						Print(out, "\t\t\t\tif (CX::Status_NoMoreItems == status.GetCode())\n");
						Print(out, "\t\t\t\t{{\n");
						Print(out, "\t\t\t\t\tbreak;\n");
						Print(out, "\t\t\t\t}\n");
						Print(out, "\t\t\t\treturn status;\n");
						Print(out, "\t\t\t}\n");
						Print(out, "\t\t\t{1}.push_back((CX::Float)nValue);\n", iter->m_sName);
					}
					break;
					case Field::Type_Double:
					{
						Print(out, "\t\t\tif ((status = pReader->ReadObjectReal(\"{1}\", &lfValue)).IsNOK())\n", iter->m_sName);
						Print(out, "\t\t\t{{\n");
						Print(out, "\t\t\t\tif (CX::Status_NoMoreItems == status.GetCode())\n");
						Print(out, "\t\t\t\t{{\n");
						Print(out, "\t\t\t\t\tbreak;\n");
						Print(out, "\t\t\t\t}\n");
						Print(out, "\t\t\t\treturn status;\n");
						Print(out, "\t\t\t}\n");
						Print(out, "\t\t\t{1}.push_back((CX::Double)nValue);\n", iter->m_sName);
					}
					break;
					case Field::Type_String:
					{
						Print(out, "\t\t\tif ((status = pReader->ReadObjectString(\"{1}\", &sValue)).IsNOK())\n", iter->m_sName);
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
						Print(out, "\t\t\tif ((status = pReader->ReadObjectWString(\"{1}\", &wsValue)).IsNOK())\n", iter->m_sName);
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
		Print(out, "typedef CX::Vector<{1}>::Type {1}Array;\n", pObject->m_sName);
		Print(out, "\n");

		return Status();
	}

};

}//namespace SimpleBuffers

}//namespace IO

}//namespace CX

