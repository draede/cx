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
 
#pragma once


#include "CX/SimpleBuffers/IGenerator.hpp"


namespace CX
{

namespace SimpleBuffers
{

template <typename OUTPUT>
class CPPGenerator : public IGenerator<OUTPUT>
{
public:

	virtual Status Generate(OUTPUT out, const Object &object)
	{
		NamespacesVector vectorNamespaces;
		String           sType;
		String           sFullType;
		Status           status;

		if ((status = ParseTypeName(object.GetName().c_str(), vectorNamespaces, sType)).IsNOK())
		{
			return status;
		}

		Print(out, "\n");
		Print(out, "#pragma once\n");
		Print(out, "\n");
		Print(out, "\n");
		Print(out, "#include \"CX/Types.hpp\"\n");
		Print(out, "#include \"CX/Status.hpp\"\n");
		Print(out, "#include \"CX/String.hpp\"\n");
		Print(out, "#include \"CX/Vector.hpp\"\n");
		Print(out, "#include \"CX/Set.hpp\"\n");
		Print(out, "#include \"CX/Map.hpp\"\n");
		Print(out, "#include \"CX/BLOB.hpp\"\n");
		Print(out, "#include \"CX/SimpleBuffers/MemberType.hpp\"\n");
		Print(out, "#include \"CX/SimpleBuffers/DataWriter.hpp\"\n");
		Print(out, "#include \"CX/SimpleBuffers/DataReader.hpp\"\n");
		Print(out, "#include \"CX/SimpleBuffers/ISimpleBuffer.hpp\"\n");

		const Object::PragmasVector &vectorPrologPragmas = object.GetPragmasByLocation(Object::PRAGMA_LOCATION_PROLOG());

		if (!vectorPrologPragmas.empty())
		{
			Print(out, "\n");
			Print(out, "\n");
			for (Object::PragmasVector::const_iterator iter = vectorPrologPragmas.begin(); iter != vectorPrologPragmas.end(); ++iter)
			{
#pragma warning(push)
#pragma warning(disable: 4996)
				if (0 == cx_stricmp(iter->sID.c_str(), Object::PRAGMA_CPP()))
#pragma warning(pop)
				{
					Print(out, "{1}\n", iter->sValue);
				}
			}
		}
		Print(out, "\n");
		Print(out, "\n");
		
		for (NamespacesVector::iterator iter = vectorNamespaces.begin(); iter != vectorNamespaces.end(); ++iter)
		{
			sFullType += *iter;
			sFullType += "::";
			Print(out, "namespace {1}\n", *iter);
			Print(out, "{{\n");
			Print(out, "\n");
		}
		sFullType += sType;

		Print(out, "class {1} : public CX::SimpleBuffers::ISimpleBuffer\n", sType);
		Print(out, "{{\n");
		Print(out, "public:\n");
		Print(out, "\n");

		//=== getters/setters

		for (Object::MembersVector::const_iterator iter = object.GetMembers().begin(); iter != object.GetMembers().end(); ++iter)
		{
			String sFullType;

			if ((status = GetMemberFullType(iter->GetMemberType(), iter->GetKeyType(), iter->GetValType(), sFullType)).IsNOK())
			{
				return status;
			}
			switch (iter->GetMemberType())
			{
				case MemberType_Scalar:
				{
					Print(out, "\tconst {1} &Get{2}() const\n", sFullType, iter->GetName());
					Print(out, "\t{{\n");
					Print(out, "\t\treturn m_{1};\n", iter->GetName());
					Print(out, "\t}\n");
					Print(out, "\n");
					Print(out, "\tvoid Set{1}(const {2} &p)\n", iter->GetName(), sFullType);
					Print(out, "\t{{\n");
					Print(out, "\t\tm_{1} = p;\n", iter->GetName());
					Print(out, "\t}\n");
					Print(out, "\n");
				}
				break;
				case MemberType_Vector:
				case MemberType_Set:
				case MemberType_Map:
				{
					Print(out, "\tconst {1} &Get{2}() const\n", sFullType, iter->GetName());
					Print(out, "\t{{\n");
					Print(out, "\t\treturn m_{1};\n", iter->GetName());
					Print(out, "\t}\n");
					Print(out, "\n");
					Print(out, "\t{1} &Get{2}()\n", sFullType, iter->GetName());
					Print(out, "\t{{\n");
					Print(out, "\t\treturn m_{1};\n", iter->GetName());
					Print(out, "\t}\n");
					Print(out, "\n");
				}
				break;
				default:
				{
					return Status(Status_InvalidArg, "Member {1} has invalid field type", iter->GetName());
				}
			}
		}

		//=== reader

		Print(out, "\tvirtual CX::Status Read(CX::SimpleBuffers::IReader *pReader, const CX::Char *szName = NULL)\n");
		Print(out, "\t{{\n");
		Print(out, "\t\tCX::Status status;\n");
		Print(out, "\n");
		Print(out, "\t\tif ((status = pReader->BeginObject(szName)).IsNOK())\n");
		Print(out, "\t\t{{\n");
		Print(out, "\t\t\treturn status;\n");
		Print(out, "\t\t}\n");
		for (Object::MembersVector::const_iterator iter = object.GetMembers().begin(); iter != object.GetMembers().end(); ++iter)
		{
			String sKeyType;
			String sValType;

			if ((status = GetMemberFullType(MemberType_Scalar, iter->GetKeyType(), "", sKeyType)).IsNOK())
			{
				return status;
			}
			if (MemberType_Map == iter->GetMemberType())
			{
				if ((status = GetMemberFullType(MemberType_Scalar, iter->GetValType(), "", sValType)).IsNOK())
				{
					return status;
				}
			}
			switch (iter->GetMemberType())
			{
				case MemberType_Scalar:
				{
					Print(out, "\t\tif ((status = CX::SimpleBuffers::DataReader<CX::SimpleBuffers::MemberType_Scalar, {1}>::Read("
					      "pReader, m_{2}, \"{2}\")).IsNOK())\n", sKeyType, iter->GetName());
					Print(out, "\t\t{{\n");
					Print(out, "\t\t\treturn status;\n");
					Print(out, "\t\t}\n");
				}
				break;
				case MemberType_Vector:
				{
					Print(out, "\t\tif ((status = CX::SimpleBuffers::DataReader<CX::SimpleBuffers::MemberType_Vector, {1}>::Read("
					      "pReader, m_{2}, \"{2}\")).IsNOK())\n", sKeyType, iter->GetName());
					Print(out, "\t\t{{\n");
					Print(out, "\t\t\treturn status;\n");
					Print(out, "\t\t}\n");
				}
				break;
				case MemberType_Set:
				{
					Print(out, "\t\tif ((status = CX::SimpleBuffers::DataReader<CX::SimpleBuffers::MemberType_Set, {1}>::Read("
					      "pReader, m_{2}, \"{2}\")).IsNOK())\n", sKeyType, iter->GetName());
					Print(out, "\t\t{{\n");
					Print(out, "\t\t\treturn status;\n");
					Print(out, "\t\t}\n");
				}
				break;
				case MemberType_Map:
				{
					Print(out, "\t\tif ((status = CX::SimpleBuffers::DataReader<CX::SimpleBuffers::MemberType_Map, {1}, {2}>::Read("
					      "pReader, m_{3}, \"{3}\")).IsNOK())\n", sKeyType, sValType, iter->GetName());
					Print(out, "\t\t{{\n");
					Print(out, "\t\t\treturn status;\n");
					Print(out, "\t\t}\n");
				}
				break;
				default:
				{
					return Status(Status_InvalidArg, "Member {1} has invalid field type", iter->GetName());
				}
			}
		}
		Print(out, "\t\tif ((status = pReader->EndObject()).IsNOK())\n");
		Print(out, "\t\t{{\n");
		Print(out, "\t\t\treturn status;\n");
		Print(out, "\t\t}\n");
		Print(out, "\n");
		Print(out, "\t\treturn CX::Status();\n");
		Print(out, "\t}\n");
		Print(out, "\n");

		//=== writer

		Print(out, "\tvirtual CX::Status Write(CX::SimpleBuffers::IWriter *pWriter, const CX::Char *szName = NULL) const\n");
		Print(out, "\t{{\n");
		Print(out, "\t\tCX::Status status;\n");
		Print(out, "\n");
		Print(out, "\t\tif ((status = pWriter->BeginObject(szName)).IsNOK())\n");
		Print(out, "\t\t{{\n");
		Print(out, "\t\t\treturn status;\n");
		Print(out, "\t\t}\n");
		for (Object::MembersVector::const_iterator iter = object.GetMembers().begin(); iter != object.GetMembers().end(); ++iter)
		{
			String sKeyType;
			String sValType;

			if ((status = GetMemberFullType(MemberType_Scalar, iter->GetKeyType(), "", sKeyType)).IsNOK())
			{
				return status;
			}
			if (MemberType_Map == iter->GetMemberType())
			{
				if ((status = GetMemberFullType(MemberType_Scalar, iter->GetValType(), "", sValType)).IsNOK())
				{
					return status;
				}
			}
			switch (iter->GetMemberType())
			{
				case MemberType_Scalar:
				{
					Print(out, "\t\tif ((status = CX::SimpleBuffers::DataWriter<CX::SimpleBuffers::MemberType_Scalar, {1}>::Write("
					      "pWriter, m_{2}, \"{2}\")).IsNOK())\n", sKeyType, iter->GetName());
					Print(out, "\t\t{{\n");
					Print(out, "\t\t\treturn status;\n");
					Print(out, "\t\t}\n");
				}
				break;
				case MemberType_Vector:
				{
					Print(out, "\t\tif ((status = CX::SimpleBuffers::DataWriter<CX::SimpleBuffers::MemberType_Vector, {1}>::Write("
					      "pWriter, m_{2}, \"{2}\")).IsNOK())\n", sKeyType, iter->GetName());
					Print(out, "\t\t{{\n");
					Print(out, "\t\t\treturn status;\n");
					Print(out, "\t\t}\n");
				}
				break;
				case MemberType_Set:
				{
					Print(out, "\t\tif ((status = CX::SimpleBuffers::DataWriter<CX::SimpleBuffers::MemberType_Set, {1}>::Write("
					      "pWriter, m_{2}, \"{2}\")).IsNOK())\n", sKeyType, iter->GetName());
					Print(out, "\t\t{{\n");
					Print(out, "\t\t\treturn status;\n");
					Print(out, "\t\t}\n");
				}
				break;
				case MemberType_Map:
				{
					Print(out, "\t\tif ((status = CX::SimpleBuffers::DataWriter<CX::SimpleBuffers::MemberType_Map, {1}, {2}>::Write("
					      "pWriter, m_{3}, \"{3}\")).IsNOK())\n", sKeyType, sValType, iter->GetName());
					Print(out, "\t\t{{\n");
					Print(out, "\t\t\treturn status;\n");
					Print(out, "\t\t}\n");
				}
				break;
				default:
				{
					return Status(Status_InvalidArg, "Member {1} has invalid field type", iter->GetName());
				}
			}
		}
		Print(out, "\t\tif ((status = pWriter->EndObject()).IsNOK())\n");
		Print(out, "\t\t{{\n");
		Print(out, "\t\t\treturn status;\n");
		Print(out, "\t\t}\n");
		Print(out, "\n");
		Print(out, "\t\treturn CX::Status();\n");
		Print(out, "\t}\n");
		Print(out, "\n");

		//=== <

		Print(out, "\tbool operator<(const {1} &p) const\n", sType);
		Print(out, "\t{{\n");
		if (object.GetMembers().empty())
		{
			Print(out, "\t\treturn false;\n");
		}
		else
		{
			for (Object::MembersVector::const_iterator iter = object.GetMembers().begin(); iter != object.GetMembers().end(); ++iter)
			{
				Print(out, "\t\tif (m_{1} < p.m_{1})\n", iter->GetName());
				Print(out, "\t\t{{\n");
				Print(out, "\t\t\treturn true;\n");
				Print(out, "\t\t}\n");
				Print(out, "\t\telse\n");
				Print(out, "\t\tif (m_{1} > p.m_{1})\n", iter->GetName());
				Print(out, "\t\t{{\n");
				Print(out, "\t\t\treturn false;\n");
				Print(out, "\t\t}\n");
			}
			Print(out, "\n");
			Print(out, "\t\treturn false;\n");
		}
		Print(out, "\t}\n");
		Print(out, "\n");

		//=== >

		Print(out, "\tbool operator>(const {1} &p) const\n", sType);
		Print(out, "\t{{\n");
		if (object.GetMembers().empty())
		{
			Print(out, "\t\treturn false;\n");
		}
		else
		{
			for (Object::MembersVector::const_iterator iter = object.GetMembers().begin(); iter != object.GetMembers().end(); ++iter)
			{
				Print(out, "\t\tif (m_{1} > p.m_{1})\n", iter->GetName());
				Print(out, "\t\t{{\n");
				Print(out, "\t\t\treturn true;\n");
				Print(out, "\t\t}\n");
				Print(out, "\t\telse\n");
				Print(out, "\t\tif (m_{1} < p.m_{1})\n", iter->GetName());
				Print(out, "\t\t{{\n");
				Print(out, "\t\t\treturn false;\n");
				Print(out, "\t\t}\n");
			}
			Print(out, "\n");
			Print(out, "\t\treturn false;\n");
		}
		Print(out, "\t}\n");
		Print(out, "\n");

		//=== members ===

		Print(out, "private:\n");
		Print(out, "\n");

		Size cMaxDataTypeLen = 0;

		for (Object::MembersVector::const_iterator iter = object.GetMembers().begin(); iter != object.GetMembers().end(); ++iter)
		{
			String sFullType;

			if ((status = GetMemberFullType(iter->GetMemberType(), iter->GetKeyType(), iter->GetValType(), sFullType)).IsNOK())
			{
				return status;
			}

			if (cMaxDataTypeLen < sFullType.size())
			{
				cMaxDataTypeLen = sFullType.size();
			}
		}

		for (Object::MembersVector::const_iterator iter = object.GetMembers().begin(); iter != object.GetMembers().end(); ++iter)
		{
			String sFullType;

			if ((status = GetMemberFullType(iter->GetMemberType(), iter->GetKeyType(), iter->GetValType(), sFullType)).IsNOK())
			{
				return status;
			}

			String     sPadding(cMaxDataTypeLen - sFullType.size() + 1, ' ');

			Print(out, "\t{1}{2}m_{3};\n", sFullType, sPadding, iter->GetName());
		}

		Print(out, "\n");
		Print(out, "};\n");

		Print(out, "\n");

		for (NamespacesVector::reverse_iterator iter = vectorNamespaces.rbegin(); iter != vectorNamespaces.rend(); ++iter)
		{
			Print(out, "\n");
			Print(out, "}//namespace {1}\n", *iter);
		}

		//=== external writer / reader

		Print(out, "\n");
		Print(out, "namespace CX\n");
		Print(out, "{{\n");
		Print(out, "\n");
		Print(out, "namespace SimpleBuffers\n");
		Print(out, "{{\n");
		Print(out, "template <>\n");
		Print(out, "struct DataWriter<MemberType_Scalar, {1}>\n", sFullType);
		Print(out, "{{\n");
		Print(out, "\tstatic Status Write(IWriter *pWriter, const {1} &v, const CX::Char *szName = NULL)\n", sFullType);
		Print(out, "\t{{\n");
		Print(out, "\t\treturn v.Write(pWriter, szName);\n");
		Print(out, "\t}\n");
		Print(out, "};\n");
		Print(out, "\n");
		Print(out, "template <>\n");
		Print(out, "struct DataReader<MemberType_Scalar, {1}>\n", sFullType);
		Print(out, "{{\n");
		Print(out, "\tstatic Status Read(IReader *pReader, {1} &v, const CX::Char *szName = NULL)\n", sFullType);
		Print(out, "\t{{\n");
		Print(out, "\t\treturn v.Read(pReader, szName);\n");
		Print(out, "\t}\n");
		Print(out, "};\n");
		Print(out, "\n");
		Print(out, "}//namespace SimpleBuffers\n");
		Print(out, "\n");
		Print(out, "}//namespace CX\n");
		Print(out, "\n");

		const Object::PragmasVector &vectorEpilogPragmas = object.GetPragmasByLocation(Object::PRAGMA_LOCATION_PROLOG());

		if (!vectorEpilogPragmas.empty())
		{
			Print(out, "\n");
			Print(out, "\n");
			for (Object::PragmasVector::const_iterator iter = vectorEpilogPragmas.begin(); iter != vectorEpilogPragmas.end(); ++iter)
			{
#pragma warning(push)
#pragma warning(disable: 4996)
				if (0 == cx_stricmp(iter->sID.c_str(), Object::PRAGMA_CPP()))
#pragma warning(pop)
				{
					Print(out, "{1}\n", iter->sValue);
				}
			}
		}

		Print(out, "\n");

		return Status();
	}
	
private:

	typedef Vector<String>::Type   NamespacesVector;

	Status ParseTypeName(const Char *szType, NamespacesVector &vectorNamespaces, String &sName)
	{
		const Char *pszStart;
		const Char *pszPos;

		vectorNamespaces.clear();
		sName.clear();
		pszStart = pszPos = szType;
		while (0 != *pszPos)
		{
			if ('.' == *pszPos)
			{
				if (pszPos > pszStart)
				{
					String sToken(pszStart, pszPos - pszStart);
					
					vectorNamespaces.push_back(sToken);
				}
				else
				{
					return Status(Status_InvalidArg, "Invalid type '{1}'", szType);
				}
				pszPos++;
				pszStart = pszPos;
			}
			else
			{
				pszPos++;
			}
		}
		if (pszPos > pszStart)
		{
			sName.assign(pszStart, pszPos - pszStart);
		}
		else
		{
			return Status(Status_InvalidArg, "Invalid type '{1}'", szType);
		}

		return Status();
	}

	Status GetFullType(const NamespacesVector &vectorNamespaces, const String &sName, String &sFullType)
	{
		sFullType.clear();

		for (NamespacesVector::const_iterator iter = vectorNamespaces.begin(); iter != vectorNamespaces.end(); ++iter)
		{
			sFullType += *iter;
			sFullType += "::";
		}
		sFullType += sName;

		return Status();
	}

	Status GetMemberFullType(MemberType nMemberType, const String &sKeyType, const String &sValType, String &sFullType)
	{
		Status status;

		switch (nMemberType)
		{
			case MemberType_Scalar:
			{
				NamespacesVector vectorKeyNamespaces;
				String           sKeyName;

				if ((status = ParseTypeName(sKeyType.c_str(), vectorKeyNamespaces, sKeyName)).IsNOK())
				{
					return status;
				}
				if ((status = GetFullType(vectorKeyNamespaces, sKeyName, sFullType)).IsNOK())
				{
					return status;
				}

				return Status();
			}
			break;
			case MemberType_Vector:
			{
				NamespacesVector vectorKeyNamespaces;
				String           sKeyName;
				String           sKeyTmp;

				if ((status = ParseTypeName(sKeyType.c_str(), vectorKeyNamespaces, sKeyName)).IsNOK())
				{
					return status;
				}
				if ((status = GetFullType(vectorKeyNamespaces, sKeyName, sKeyTmp)).IsNOK())
				{
					return status;
				}
				Print(&sFullType, "CX::Vector<{1}>::Type", sKeyTmp);

				return Status();
			}
			break;
			case MemberType_Set:
			{
				NamespacesVector vectorKeyNamespaces;
				String           sKeyName;
				String           sKeyTmp;

				if ((status = ParseTypeName(sKeyType.c_str(), vectorKeyNamespaces, sKeyName)).IsNOK())
				{
					return status;
				}
				if ((status = GetFullType(vectorKeyNamespaces, sKeyName, sKeyTmp)).IsNOK())
				{
					return status;
				}
				Print(&sFullType, "CX::Set<{1}>::Type", sKeyTmp);

				return Status();
			}
			break;
			case MemberType_Map:
			{
				NamespacesVector vectorKeyNamespaces;
				String           sKeyName;
				String           sKeyTmp;
				NamespacesVector vectorValNamespaces;
				String           sValName;
				String           sValTmp;

				if ((status = ParseTypeName(sKeyType.c_str(), vectorKeyNamespaces, sKeyName)).IsNOK())
				{
					return status;
				}
				if ((status = GetFullType(vectorKeyNamespaces, sKeyName, sKeyTmp)).IsNOK())
				{
					return status;
				}
				if ((status = ParseTypeName(sValType.c_str(), vectorValNamespaces, sValName)).IsNOK())
				{
					return status;
				}
				if ((status = GetFullType(vectorValNamespaces, sValName, sValTmp)).IsNOK())
				{
					return status;
				}
				Print(&sFullType, "CX::Map<{1}, {2}>::Type", sKeyTmp, sValTmp);

				return Status();
			}
			break;
			default:
			{
				return Status(Status_InvalidArg, "Member has invalid member type");
			}
		}
	}

};

}//namespace SimpleBuffers

}//namespace CX
