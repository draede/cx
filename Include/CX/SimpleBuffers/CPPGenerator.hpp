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


#include "CX/SimpleBuffers/IGenerator.hpp"


#define CX_SB_UNIX_LINE_TERMINATOR   "\n"
#define CX_SB_WIN_LINE_TERMINATOR    "\r\n"
#define CX_SB_LINE_TERMINATOR        CX_SB_WIN_LINE_TERMINATOR


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

		Print(out, CX_SB_LINE_TERMINATOR);
		Print(out, "#pragma once" CX_SB_LINE_TERMINATOR);
		Print(out, CX_SB_LINE_TERMINATOR);
		Print(out, CX_SB_LINE_TERMINATOR);
		Print(out, "#include \"CX/Types.hpp\"" CX_SB_LINE_TERMINATOR);
		Print(out, "#include \"CX/Status.hpp\"" CX_SB_LINE_TERMINATOR);
		Print(out, "#include \"CX/String.hpp\"" CX_SB_LINE_TERMINATOR);
		Print(out, "#include \"CX/Vector.hpp\"" CX_SB_LINE_TERMINATOR);
		Print(out, "#include \"CX/Set.hpp\"" CX_SB_LINE_TERMINATOR);
		Print(out, "#include \"CX/Map.hpp\"" CX_SB_LINE_TERMINATOR);
		Print(out, "#include \"CX/BLOB.hpp\"" CX_SB_LINE_TERMINATOR);
		Print(out, "#include \"CX/BitSet.hpp\"" CX_SB_LINE_TERMINATOR);
		Print(out, "#include \"CX/C/string.h\"" CX_SB_LINE_TERMINATOR);
		Print(out, "#include \"CX/SimpleBuffers/MemberType.hpp\"" CX_SB_LINE_TERMINATOR);
		Print(out, "#include \"CX/SimpleBuffers/DataWriter.hpp\"" CX_SB_LINE_TERMINATOR);
		Print(out, "#include \"CX/SimpleBuffers/DataReader.hpp\"" CX_SB_LINE_TERMINATOR);
		Print(out, "#include \"CX/SimpleBuffers/DataIniter.hpp\"" CX_SB_LINE_TERMINATOR);
		Print(out, "#include \"CX/SimpleBuffers/ISimpleBuffer.hpp\"" CX_SB_LINE_TERMINATOR);

		const Object::PragmasVector &vectorPrologPragmas = object.GetPragmasByLocation(Object::PRAGMA_LOCATION_PROLOG());

		if (!vectorPrologPragmas.empty())
		{
			Print(out, CX_SB_LINE_TERMINATOR);
			Print(out, CX_SB_LINE_TERMINATOR);
			for (Object::PragmasVector::const_iterator iter = vectorPrologPragmas.begin(); iter != vectorPrologPragmas.end(); ++iter)
			{
#pragma warning(push)
#pragma warning(disable: 4996)
				if (0 == cx_stricmp(iter->sID.c_str(), Object::PRAGMA_CPP()))
#pragma warning(pop)
				{
					Print(out, "{1}" CX_SB_LINE_TERMINATOR, iter->sValue);
				}
			}
		}
		Print(out, CX_SB_LINE_TERMINATOR);
		Print(out, CX_SB_LINE_TERMINATOR);
		
		for (NamespacesVector::iterator iter = vectorNamespaces.begin(); iter != vectorNamespaces.end(); ++iter)
		{
			sFullType += *iter;
			sFullType += "::";
			Print(out, "namespace {1}" CX_SB_LINE_TERMINATOR, *iter);
			Print(out, "{{" CX_SB_LINE_TERMINATOR);
			Print(out, CX_SB_LINE_TERMINATOR);
		}
		sFullType += sType;

		Print(out, "class {1} : public CX::SimpleBuffers::ISimpleBuffer" CX_SB_LINE_TERMINATOR, sType);
		Print(out, "{{" CX_SB_LINE_TERMINATOR);
		Print(out, "public:" CX_SB_LINE_TERMINATOR);
		Print(out, CX_SB_LINE_TERMINATOR);
		Print(out, "\tstatic const CX::Char *OBJECT_NAME() {{ return \"{1}\"; }" CX_SB_LINE_TERMINATOR, object.GetName());
		Print(out, CX_SB_LINE_TERMINATOR);
		Print(out, "\tvirtual const CX::Char *GetObjectName() const {{ return OBJECT_NAME(); }" CX_SB_LINE_TERMINATOR);
		Print(out, CX_SB_LINE_TERMINATOR);

		//=== consts

		if (!object.GetConsts().empty())
		{
			Size cMaxDataTypeLen = 0;

			for (Object::ConstsVector::const_iterator iter = object.GetConsts().begin(); iter != object.GetConsts().end(); ++iter)
			{
				String sTmpFullType;

				if ((status = GetMemberFullType(MemberType_Scalar, iter->sType.c_str(), "", sTmpFullType)).IsNOK())
				{
					return status;
				}

				if (cMaxDataTypeLen < sTmpFullType.size())
				{
					cMaxDataTypeLen = sTmpFullType.size();
				}
			}
			for (Object::ConstsVector::const_iterator iter = object.GetConsts().begin(); iter != object.GetConsts().end(); ++iter)
			{
				String sTmpFullType;

				if ((status = GetMemberFullType(MemberType_Scalar, iter->sType.c_str(), "", sTmpFullType)).IsNOK())
				{
					return status;
				}

				String sPaddingType(cMaxDataTypeLen - sTmpFullType.size() + 1, ' ');

				Print(out, "\tstatic const {1}{2}{3}() {{ return {4}; }\n", sTmpFullType, sPaddingType, iter->sName, iter->sValue);
			}
			Print(out, CX_SB_LINE_TERMINATOR);
		}

		//=== constructor

		Print(out, "\t{1}()" CX_SB_LINE_TERMINATOR, sType);
		Print(out, "\t{{" CX_SB_LINE_TERMINATOR);
		Print(out, "\t\tInit();" CX_SB_LINE_TERMINATOR);
		Print(out, "\t}" CX_SB_LINE_TERMINATOR);
		Print(out, CX_SB_LINE_TERMINATOR);

		//=== init

		Print(out, "\tvirtual void Init()" CX_SB_LINE_TERMINATOR);
		Print(out, "\t{{" CX_SB_LINE_TERMINATOR);
		for (Object::MembersVector::const_iterator iter = object.GetMembers().begin(); iter != object.GetMembers().end(); ++iter)
		{
			if (iter->GetDefault().empty())
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
						Print(out, "\t\tCX::SimpleBuffers::DataIniter<CX::SimpleBuffers::MemberType_Scalar, {1}>::Init("
								"this->{2});" CX_SB_LINE_TERMINATOR, sKeyType, iter->GetMemberName());
					}
					break;
					case MemberType_Vector:
					{
						Print(out, "\t\tCX::SimpleBuffers::DataIniter<CX::SimpleBuffers::MemberType_Vector, {1}>::Init("
								"this->{2});" CX_SB_LINE_TERMINATOR, sKeyType, iter->GetMemberName());
					}
					break;
					case MemberType_Set:
					{
						Print(out, "\t\tCX::SimpleBuffers::DataIniter<CX::SimpleBuffers::MemberType_Set, {1}>::Init("
								"this->{2});" CX_SB_LINE_TERMINATOR, sKeyType, iter->GetMemberName());
					}
					break;
					case MemberType_Map:
					{
						Print(out, "\t\tCX::SimpleBuffers::DataIniter<CX::SimpleBuffers::MemberType_Map, {1}, {2}>::Init("
								"this->{3});" CX_SB_LINE_TERMINATOR, sKeyType, sValType, iter->GetMemberName());
					}
					break;
					default:
					{
						return Status(Status_InvalidArg, "Member {1} has invalid field type", iter->GetName());
					}
				}
			}
			else
			{
				String sDefault;

				if ('"' == *iter->GetDefault().begin())
				{
					sDefault = iter->GetDefault();
				}
				else
				{
					const Char *pszPos;

					pszPos = iter->GetDefault().c_str();
					while (0 != *pszPos)
					{
						if ('.' == *pszPos)
						{
							sDefault += "::";
						}
						else
						{
							sDefault += *pszPos;
						}
						pszPos++;
					}
				}
				Print(out, "\t\t{1} = {2};\n", iter->GetMemberName(), sDefault);
			}
		}
		Print(out, "\t}" CX_SB_LINE_TERMINATOR);
		Print(out, CX_SB_LINE_TERMINATOR);

		//=== getters/setters

		for (Object::MembersVector::const_iterator iter = object.GetMembers().begin(); iter != object.GetMembers().end(); ++iter)
		{
			String sTmpFullType;

			if ((status = GetMemberFullType(iter->GetMemberType(), iter->GetKeyType(), iter->GetValType(), sTmpFullType)).IsNOK())
			{
				return status;
			}
			switch (iter->GetMemberType())
			{
				case MemberType_Scalar:
				{
					Print(out, "\tconst {1} &{2}() const" CX_SB_LINE_TERMINATOR, sTmpFullType, iter->GetGetterName());
					Print(out, "\t{{" CX_SB_LINE_TERMINATOR);
					Print(out, "\t\treturn this->{1};" CX_SB_LINE_TERMINATOR, iter->GetMemberName());
					Print(out, "\t}" CX_SB_LINE_TERMINATOR);
					Print(out, CX_SB_LINE_TERMINATOR);
					Print(out, "\t{1} &{2}()" CX_SB_LINE_TERMINATOR, sTmpFullType, iter->GetGetterName());
					Print(out, "\t{{" CX_SB_LINE_TERMINATOR);
					Print(out, "\t\treturn this->{1};" CX_SB_LINE_TERMINATOR, iter->GetMemberName());
					Print(out, "\t}" CX_SB_LINE_TERMINATOR);
					Print(out, CX_SB_LINE_TERMINATOR);
					Print(out, "\tvoid {1}(const {2} &p)" CX_SB_LINE_TERMINATOR, iter->GetSetterName(), sTmpFullType);
					Print(out, "\t{{" CX_SB_LINE_TERMINATOR);
					Print(out, "\t\tthis->{1} = p;" CX_SB_LINE_TERMINATOR, iter->GetMemberName());
					Print(out, "\t}" CX_SB_LINE_TERMINATOR);
					Print(out, CX_SB_LINE_TERMINATOR);
				}
				break;
				case MemberType_Vector:
				{
					Print(out, "\tconst {1} &{2}() const" CX_SB_LINE_TERMINATOR, sTmpFullType, iter->GetGetterName());
					Print(out, "\t{{" CX_SB_LINE_TERMINATOR);
					Print(out, "\t\treturn this->{1};" CX_SB_LINE_TERMINATOR, iter->GetMemberName());
					Print(out, "\t}" CX_SB_LINE_TERMINATOR);
					Print(out, CX_SB_LINE_TERMINATOR);
					Print(out, "\t{1} &{2}()" CX_SB_LINE_TERMINATOR, sTmpFullType, iter->GetGetterName());
					Print(out, "\t{{" CX_SB_LINE_TERMINATOR);
					Print(out, "\t\treturn this->{1};" CX_SB_LINE_TERMINATOR, iter->GetMemberName());
					Print(out, "\t}" CX_SB_LINE_TERMINATOR);
					Print(out, CX_SB_LINE_TERMINATOR);
				}
				break;
				case MemberType_Set:
				{
					Print(out, "\tconst {1} &{2}() const" CX_SB_LINE_TERMINATOR, sTmpFullType, iter->GetGetterName());
					Print(out, "\t{{" CX_SB_LINE_TERMINATOR);
					Print(out, "\t\treturn this->{1};" CX_SB_LINE_TERMINATOR, iter->GetMemberName());
					Print(out, "\t}" CX_SB_LINE_TERMINATOR);
					Print(out, CX_SB_LINE_TERMINATOR);
					Print(out, "\t{1} &{2}()" CX_SB_LINE_TERMINATOR, sTmpFullType, iter->GetGetterName());
					Print(out, "\t{{" CX_SB_LINE_TERMINATOR);
					Print(out, "\t\treturn this->{1};" CX_SB_LINE_TERMINATOR, iter->GetMemberName());
					Print(out, "\t}" CX_SB_LINE_TERMINATOR);
					Print(out, CX_SB_LINE_TERMINATOR);
				}
				break;
				case MemberType_Map:
				{
					Print(out, "\tconst {1} &{2}() const" CX_SB_LINE_TERMINATOR, sTmpFullType, iter->GetGetterName());
					Print(out, "\t{{" CX_SB_LINE_TERMINATOR);
					Print(out, "\t\treturn this->{1};" CX_SB_LINE_TERMINATOR, iter->GetMemberName());
					Print(out, "\t}" CX_SB_LINE_TERMINATOR);
					Print(out, CX_SB_LINE_TERMINATOR);
					Print(out, "\t{1} &{2}()" CX_SB_LINE_TERMINATOR, sTmpFullType, iter->GetGetterName());
					Print(out, "\t{{" CX_SB_LINE_TERMINATOR);
					Print(out, "\t\treturn this->{1};" CX_SB_LINE_TERMINATOR, iter->GetMemberName());
					Print(out, "\t}" CX_SB_LINE_TERMINATOR);
					Print(out, CX_SB_LINE_TERMINATOR);
				}
				break;
				default:
				{
					return Status(Status_InvalidArg, "Member {1} has invalid field type", iter->GetName());
				}
			}
		}

		Size cMemberIndex;

		//=== helpers

		Print(out, "\tvirtual CX::Size GetMembersCount() const" CX_SB_LINE_TERMINATOR);
		Print(out, "\t{{" CX_SB_LINE_TERMINATOR);
		Print(out, "\t\treturn {1};" CX_SB_LINE_TERMINATOR, object.GetMembers().size());
		Print(out, "\t}" CX_SB_LINE_TERMINATOR);
		Print(out, CX_SB_LINE_TERMINATOR);

		Print(out, "\tvirtual CX::Bool GetMemberLoaded(CX::Size cIndex) const" CX_SB_LINE_TERMINATOR);
		Print(out, "\t{{" CX_SB_LINE_TERMINATOR);
		Print(out, "\t\tif (m_bitsetLoadedMembers.size() <= cIndex)" CX_SB_LINE_TERMINATOR);
		Print(out, "\t\t{{" CX_SB_LINE_TERMINATOR);
		Print(out, "\t\t\treturn CX::False;" CX_SB_LINE_TERMINATOR);
		Print(out, "\t\t}" CX_SB_LINE_TERMINATOR);
		Print(out, CX_SB_LINE_TERMINATOR);
		Print(out, "\t\treturn m_bitsetLoadedMembers[cIndex];" CX_SB_LINE_TERMINATOR);
		Print(out, "\t}" CX_SB_LINE_TERMINATOR);
		Print(out, CX_SB_LINE_TERMINATOR);

		Print(out, "\tvirtual CX::Bool GetMemberLoaded(const CX::Char *szName) const" CX_SB_LINE_TERMINATOR);
		Print(out, "\t{{" CX_SB_LINE_TERMINATOR);
		cMemberIndex = 0;
		for (Object::MembersVector::const_iterator iter = object.GetMembers().begin(); iter != object.GetMembers().end(); ++iter)
		{
			Print(out, "#pragma warning(push)" CX_SB_LINE_TERMINATOR, iter->GetName());
			Print(out, "#pragma warning(disable: 4996)" CX_SB_LINE_TERMINATOR, iter->GetName());
			Print(out, "\t\tif (0 == cx_strcmp(\"{1}\", szName))" CX_SB_LINE_TERMINATOR, iter->GetName());
			Print(out, "#pragma warning(pop)" CX_SB_LINE_TERMINATOR, iter->GetName());
			Print(out, "\t\t{{" CX_SB_LINE_TERMINATOR);
			Print(out, "\t\t\treturn m_bitsetLoadedMembers[{1}];" CX_SB_LINE_TERMINATOR, cMemberIndex);
			Print(out, "\t\t}" CX_SB_LINE_TERMINATOR);
			cMemberIndex++;
		}
		Print(out, CX_SB_LINE_TERMINATOR);
		Print(out, "\t\treturn CX::False;" CX_SB_LINE_TERMINATOR);
		Print(out, "\t}" CX_SB_LINE_TERMINATOR);
		Print(out, CX_SB_LINE_TERMINATOR);

		Print(out, "\tvirtual CX::Status SetMemberLoaded(CX::Size cIndex, CX::Bool bLoaded)" CX_SB_LINE_TERMINATOR);
		Print(out, "\t{{" CX_SB_LINE_TERMINATOR);
		Print(out, "\t\tif (m_bitsetLoadedMembers.size() <= cIndex)" CX_SB_LINE_TERMINATOR);
		Print(out, "\t\t{{" CX_SB_LINE_TERMINATOR);
		Print(out, "\t\t\treturn CX::Status_InvalidArg;" CX_SB_LINE_TERMINATOR);
		Print(out, "\t\t}" CX_SB_LINE_TERMINATOR);
		Print(out, "\t\tm_bitsetLoadedMembers[cIndex] = bLoaded;" CX_SB_LINE_TERMINATOR);
		Print(out, CX_SB_LINE_TERMINATOR);
		Print(out, "\t\treturn CX::Status();" CX_SB_LINE_TERMINATOR);
		Print(out, "\t}" CX_SB_LINE_TERMINATOR);
		Print(out, CX_SB_LINE_TERMINATOR);

		Print(out, "\tvirtual CX::Status SetMemberLoaded(const CX::Char *szName, CX::Bool bLoaded)" CX_SB_LINE_TERMINATOR);
		Print(out, "\t{{" CX_SB_LINE_TERMINATOR);
		cMemberIndex = 0;
		for (Object::MembersVector::const_iterator iter = object.GetMembers().begin(); iter != object.GetMembers().end(); ++iter)
		{
			Print(out, "#pragma warning(push)" CX_SB_LINE_TERMINATOR, iter->GetName());
			Print(out, "#pragma warning(disable: 4996)" CX_SB_LINE_TERMINATOR, iter->GetName());
			Print(out, "\t\tif (0 == cx_strcmp(\"{1}\", szName))" CX_SB_LINE_TERMINATOR, iter->GetName());
			Print(out, "#pragma warning(pop)" CX_SB_LINE_TERMINATOR, iter->GetName());
			Print(out, "\t\t{{" CX_SB_LINE_TERMINATOR);
			Print(out, "\t\t\tm_bitsetLoadedMembers[{1}] = bLoaded;" CX_SB_LINE_TERMINATOR, cMemberIndex);
			Print(out, CX_SB_LINE_TERMINATOR);
			Print(out, "\t\t\treturn CX::Status();" CX_SB_LINE_TERMINATOR);
			Print(out, "\t\t}" CX_SB_LINE_TERMINATOR);
			cMemberIndex++;
		}
		Print(out, CX_SB_LINE_TERMINATOR);
		Print(out, "\t\treturn CX::Status_InvalidArg;" CX_SB_LINE_TERMINATOR);
		Print(out, "\t}" CX_SB_LINE_TERMINATOR);
		Print(out, CX_SB_LINE_TERMINATOR);

		Print(out, "\tvirtual CX::Status SetAllMembersLoaded(CX::Bool bLoaded)" CX_SB_LINE_TERMINATOR);
		Print(out, "\t{{" CX_SB_LINE_TERMINATOR);
		Print(out, "\t\tif (bLoaded)" CX_SB_LINE_TERMINATOR);
		Print(out, "\t\t{{" CX_SB_LINE_TERMINATOR);
		Print(out, "\t\t\tm_bitsetLoadedMembers.set();" CX_SB_LINE_TERMINATOR);
		Print(out, "\t\t}" CX_SB_LINE_TERMINATOR);
		Print(out, "\t\telse" CX_SB_LINE_TERMINATOR);
		Print(out, "\t\t{{" CX_SB_LINE_TERMINATOR);
		Print(out, "\t\t\tm_bitsetLoadedMembers.reset();" CX_SB_LINE_TERMINATOR);
		Print(out, "\t\t}" CX_SB_LINE_TERMINATOR);
		Print(out, CX_SB_LINE_TERMINATOR);
		Print(out, "\t\treturn CX::Status();" CX_SB_LINE_TERMINATOR);
		Print(out, "\t}" CX_SB_LINE_TERMINATOR);
		Print(out, CX_SB_LINE_TERMINATOR);

		//=== reader

		Print(out, "\tvirtual CX::Status Read(CX::SimpleBuffers::IReader *pReader, const CX::Char *szName = NULL)" CX_SB_LINE_TERMINATOR);
		Print(out, "\t{{" CX_SB_LINE_TERMINATOR);
		Print(out, "\t\tCX::Status status;" CX_SB_LINE_TERMINATOR);
		Print(out, CX_SB_LINE_TERMINATOR);
		Print(out, "\t\tInit();" CX_SB_LINE_TERMINATOR);
		Print(out, "\t\tm_bitsetLoadedMembers.reset();" CX_SB_LINE_TERMINATOR);
		Print(out, "\t\tif ((status = pReader->BeginObject(szName)).IsNOK())" CX_SB_LINE_TERMINATOR);
		Print(out, "\t\t{{" CX_SB_LINE_TERMINATOR);
		Print(out, "\t\t\treturn status;" CX_SB_LINE_TERMINATOR);
		Print(out, "\t\t}" CX_SB_LINE_TERMINATOR);

		cMemberIndex = 0;
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
					      "pReader, this->{2}, \"{3}\")).IsNOK())" CX_SB_LINE_TERMINATOR, sKeyType, iter->GetMemberName(), iter->GetName());
					Print(out, "\t\t{{" CX_SB_LINE_TERMINATOR);
					if (!iter->GetOptional())
					{
						Print(out, "\t\t\tif (CX::Status_NotFound == status.GetCode())" CX_SB_LINE_TERMINATOR);
						Print(out, "\t\t\t{{" CX_SB_LINE_TERMINATOR);
						Print(out, "\t\t\t\treturn CX::Status(CX::Status_NoMoreItems, \"{1} : {{1}\", status.GetMsg());" CX_SB_LINE_TERMINATOR, object.GetName());
						Print(out, "\t\t\t}" CX_SB_LINE_TERMINATOR);
						Print(out, "\t\t\telse" CX_SB_LINE_TERMINATOR);
						Print(out, "\t\t\t{{" CX_SB_LINE_TERMINATOR);
						Print(out, "\t\t\t\treturn status;" CX_SB_LINE_TERMINATOR);
						Print(out, "\t\t\t}" CX_SB_LINE_TERMINATOR);
					}
					else
					{
						Print(out, "\t\t\tif (CX::Status_NotFound != status.GetCode())" CX_SB_LINE_TERMINATOR);
						Print(out, "\t\t\t{{" CX_SB_LINE_TERMINATOR);
						Print(out, "\t\t\t\treturn status;" CX_SB_LINE_TERMINATOR);
						Print(out, "\t\t\t}" CX_SB_LINE_TERMINATOR);
						
					}
					Print(out, "\t\t}" CX_SB_LINE_TERMINATOR);
					Print(out, "\t\telse" CX_SB_LINE_TERMINATOR);
					Print(out, "\t\t{{" CX_SB_LINE_TERMINATOR);
					Print(out, "\t\t\tm_bitsetLoadedMembers[{1}] = true;" CX_SB_LINE_TERMINATOR, cMemberIndex);
					Print(out, "\t\t}" CX_SB_LINE_TERMINATOR);
				}
				break;
				case MemberType_Vector:
				{
					Print(out, "\t\tif ((status = CX::SimpleBuffers::DataReader<CX::SimpleBuffers::MemberType_Vector, {1}>::Read("
					      "pReader, this->{2}, \"{3}\")).IsNOK())" CX_SB_LINE_TERMINATOR, sKeyType, iter->GetMemberName(), iter->GetName());
					Print(out, "\t\t{{" CX_SB_LINE_TERMINATOR);
					if (!iter->GetOptional())
					{
						Print(out, "\t\t\tif (CX::Status_NotFound == status.GetCode())" CX_SB_LINE_TERMINATOR);
						Print(out, "\t\t\t{{" CX_SB_LINE_TERMINATOR);
						Print(out, "\t\t\t\treturn CX::Status(CX::Status_NoMoreItems, \"{1} : {{1}\", status.GetMsg());" CX_SB_LINE_TERMINATOR, object.GetName());
						Print(out, "\t\t\t}" CX_SB_LINE_TERMINATOR);
						Print(out, "\t\t\telse" CX_SB_LINE_TERMINATOR);
						Print(out, "\t\t\t{{" CX_SB_LINE_TERMINATOR);
						Print(out, "\t\t\t\treturn status;" CX_SB_LINE_TERMINATOR);
						Print(out, "\t\t\t}" CX_SB_LINE_TERMINATOR);
					}
					else
					{
						Print(out, "\t\t\tif (CX::Status_NotFound != status.GetCode())" CX_SB_LINE_TERMINATOR);
						Print(out, "\t\t\t{{" CX_SB_LINE_TERMINATOR);
						Print(out, "\t\t\t\treturn status;" CX_SB_LINE_TERMINATOR);
						Print(out, "\t\t\t}" CX_SB_LINE_TERMINATOR);
					}
					Print(out, "\t\t}" CX_SB_LINE_TERMINATOR);
					Print(out, "\t\telse" CX_SB_LINE_TERMINATOR);
					Print(out, "\t\t{{" CX_SB_LINE_TERMINATOR);
					Print(out, "\t\t\tm_bitsetLoadedMembers[{1}] = true;" CX_SB_LINE_TERMINATOR, cMemberIndex);
					Print(out, "\t\t}" CX_SB_LINE_TERMINATOR);
				}
				break;
				case MemberType_Set:
				{
					Print(out, "\t\tif ((status = CX::SimpleBuffers::DataReader<CX::SimpleBuffers::MemberType_Set, {1}>::Read("
					      "pReader, this->{2}, \"{3}\")).IsNOK())" CX_SB_LINE_TERMINATOR, sKeyType, iter->GetMemberName(), iter->GetName());
					Print(out, "\t\t{{" CX_SB_LINE_TERMINATOR);
					if (!iter->GetOptional())
					{
						Print(out, "\t\t\tif (CX::Status_NotFound == status.GetCode())" CX_SB_LINE_TERMINATOR);
						Print(out, "\t\t\t{{" CX_SB_LINE_TERMINATOR);
						Print(out, "\t\t\t\treturn CX::Status(CX::Status_NoMoreItems, \"{1} : {{1}\", status.GetMsg());" CX_SB_LINE_TERMINATOR, object.GetName());
						Print(out, "\t\t\t}" CX_SB_LINE_TERMINATOR);
						Print(out, "\t\t\telse" CX_SB_LINE_TERMINATOR);
						Print(out, "\t\t\t{{" CX_SB_LINE_TERMINATOR);
						Print(out, "\t\t\t\treturn status;" CX_SB_LINE_TERMINATOR);
						Print(out, "\t\t\t}" CX_SB_LINE_TERMINATOR);
					}
					else
					{
						Print(out, "\t\t\tif (CX::Status_NotFound != status.GetCode())" CX_SB_LINE_TERMINATOR);
						Print(out, "\t\t\t{{" CX_SB_LINE_TERMINATOR);
						Print(out, "\t\t\t\treturn status;" CX_SB_LINE_TERMINATOR);
						Print(out, "\t\t\t}" CX_SB_LINE_TERMINATOR);
					}
					Print(out, "\t\t}" CX_SB_LINE_TERMINATOR);
					Print(out, "\t\telse" CX_SB_LINE_TERMINATOR);
					Print(out, "\t\t{{" CX_SB_LINE_TERMINATOR);
					Print(out, "\t\t\tm_bitsetLoadedMembers[{1}] = true;" CX_SB_LINE_TERMINATOR, cMemberIndex);
					Print(out, "\t\t}" CX_SB_LINE_TERMINATOR);
				}
				break;
				case MemberType_Map:
				{
					Print(out, "\t\tif ((status = CX::SimpleBuffers::DataReader<CX::SimpleBuffers::MemberType_Map, {1}, {2}>::Read("
					      "pReader, this->{3}, \"{4}\")).IsNOK())" CX_SB_LINE_TERMINATOR, sKeyType, sValType, iter->GetMemberName(), iter->GetName());
					Print(out, "\t\t{{" CX_SB_LINE_TERMINATOR);
					if (!iter->GetOptional())
					{
						Print(out, "\t\t\tif (CX::Status_NotFound == status.GetCode())" CX_SB_LINE_TERMINATOR);
						Print(out, "\t\t\t{{" CX_SB_LINE_TERMINATOR);
						Print(out, "\t\t\t\treturn CX::Status(CX::Status_NoMoreItems, \"{1} : {{1}\", status.GetMsg());" CX_SB_LINE_TERMINATOR, object.GetName());
						Print(out, "\t\t\t}" CX_SB_LINE_TERMINATOR);
						Print(out, "\t\t\telse" CX_SB_LINE_TERMINATOR);
						Print(out, "\t\t\t{{" CX_SB_LINE_TERMINATOR);
						Print(out, "\t\t\t\treturn status;" CX_SB_LINE_TERMINATOR);
						Print(out, "\t\t\t}" CX_SB_LINE_TERMINATOR);
					}
					else
					{
						Print(out, "\t\t\tif (CX::Status_NotFound != status.GetCode())" CX_SB_LINE_TERMINATOR);
						Print(out, "\t\t\t{{" CX_SB_LINE_TERMINATOR);
						Print(out, "\t\t\t\treturn status;" CX_SB_LINE_TERMINATOR);
						Print(out, "\t\t\t}" CX_SB_LINE_TERMINATOR);
					}
					Print(out, "\t\t}" CX_SB_LINE_TERMINATOR);
					Print(out, "\t\telse" CX_SB_LINE_TERMINATOR);
					Print(out, "\t\t{{" CX_SB_LINE_TERMINATOR);
					Print(out, "\t\t\tm_bitsetLoadedMembers[{1}] = true;" CX_SB_LINE_TERMINATOR, cMemberIndex);
					Print(out, "\t\t}" CX_SB_LINE_TERMINATOR);
				}
				break;
				default:
				{
					return Status(Status_InvalidArg, "Member {1} has invalid field type", iter->GetName());
				}
			}
			cMemberIndex++;
		}
		Print(out, "\t\tif ((status = pReader->EndObject()).IsNOK())" CX_SB_LINE_TERMINATOR);
		Print(out, "\t\t{{" CX_SB_LINE_TERMINATOR);
		Print(out, "\t\t\treturn status;" CX_SB_LINE_TERMINATOR);
		Print(out, "\t\t}" CX_SB_LINE_TERMINATOR);
		Print(out, CX_SB_LINE_TERMINATOR);
		Print(out, "\t\treturn CX::Status();" CX_SB_LINE_TERMINATOR);
		Print(out, "\t}" CX_SB_LINE_TERMINATOR);
		Print(out, CX_SB_LINE_TERMINATOR);

		//=== writer

		Print(out, "\tvirtual CX::Status Write(CX::SimpleBuffers::IWriter *pWriter, const CX::Char *szName = NULL) const" CX_SB_LINE_TERMINATOR);
		Print(out, "\t{{" CX_SB_LINE_TERMINATOR);
		Print(out, "\t\tCX::Status status;" CX_SB_LINE_TERMINATOR);
		Print(out, CX_SB_LINE_TERMINATOR);
		Print(out, "\t\tif ((status = pWriter->BeginObject(szName)).IsNOK())" CX_SB_LINE_TERMINATOR);
		Print(out, "\t\t{{" CX_SB_LINE_TERMINATOR);
		Print(out, "\t\t\treturn status;" CX_SB_LINE_TERMINATOR);
		Print(out, "\t\t}" CX_SB_LINE_TERMINATOR);
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
					      "pWriter, this->{2}, \"{3}\")).IsNOK())" CX_SB_LINE_TERMINATOR, sKeyType, iter->GetMemberName(), iter->GetName());
					Print(out, "\t\t{{" CX_SB_LINE_TERMINATOR);
					Print(out, "\t\t\treturn status;" CX_SB_LINE_TERMINATOR);
					Print(out, "\t\t}" CX_SB_LINE_TERMINATOR);
				}
				break;
				case MemberType_Vector:
				{
					Print(out, "\t\tif ((status = CX::SimpleBuffers::DataWriter<CX::SimpleBuffers::MemberType_Vector, {1}>::Write("
					      "pWriter, this->{2}, \"{3}\")).IsNOK())" CX_SB_LINE_TERMINATOR, sKeyType, iter->GetMemberName(), iter->GetName());
					Print(out, "\t\t{{" CX_SB_LINE_TERMINATOR);
					Print(out, "\t\t\treturn status;" CX_SB_LINE_TERMINATOR);
					Print(out, "\t\t}" CX_SB_LINE_TERMINATOR);
				}
				break;
				case MemberType_Set:
				{
					Print(out, "\t\tif ((status = CX::SimpleBuffers::DataWriter<CX::SimpleBuffers::MemberType_Set, {1}>::Write("
					      "pWriter, this->{2}, \"{3}\")).IsNOK())" CX_SB_LINE_TERMINATOR, sKeyType, iter->GetMemberName(), iter->GetName());
					Print(out, "\t\t{{" CX_SB_LINE_TERMINATOR);
					Print(out, "\t\t\treturn status;" CX_SB_LINE_TERMINATOR);
					Print(out, "\t\t}" CX_SB_LINE_TERMINATOR);
				}
				break;
				case MemberType_Map:
				{
					Print(out, "\t\tif ((status = CX::SimpleBuffers::DataWriter<CX::SimpleBuffers::MemberType_Map, {1}, {2}>::Write("
					      "pWriter, this->{3}, \"{4}\")).IsNOK())" CX_SB_LINE_TERMINATOR, sKeyType, sValType, iter->GetMemberName(), iter->GetName());
					Print(out, "\t\t{{" CX_SB_LINE_TERMINATOR);
					Print(out, "\t\t\treturn status;" CX_SB_LINE_TERMINATOR);
					Print(out, "\t\t}" CX_SB_LINE_TERMINATOR);
				}
				break;
				default:
				{
					return Status(Status_InvalidArg, "Member {1} has invalid field type", iter->GetName());
				}
			}
		}
		Print(out, "\t\tif ((status = pWriter->EndObject()).IsNOK())" CX_SB_LINE_TERMINATOR);
		Print(out, "\t\t{{" CX_SB_LINE_TERMINATOR);
		Print(out, "\t\t\treturn status;" CX_SB_LINE_TERMINATOR);
		Print(out, "\t\t}" CX_SB_LINE_TERMINATOR);
		Print(out, CX_SB_LINE_TERMINATOR);
		Print(out, "\t\treturn CX::Status();" CX_SB_LINE_TERMINATOR);
		Print(out, "\t}" CX_SB_LINE_TERMINATOR);
		Print(out, CX_SB_LINE_TERMINATOR);

		//=== <

		Print(out, "\tbool operator<(const {1} &p) const" CX_SB_LINE_TERMINATOR, sType);
		Print(out, "\t{{" CX_SB_LINE_TERMINATOR);
		if (object.GetMembers().empty())
		{
			Print(out, "\t\treturn false;" CX_SB_LINE_TERMINATOR);
		}
		else
		{
			for (Object::MembersVector::const_iterator iter = object.GetMembers().begin(); iter != object.GetMembers().end(); ++iter)
			{
				Print(out, "\t\tif (this->{1} < p.{1})" CX_SB_LINE_TERMINATOR, iter->GetMemberName());
				Print(out, "\t\t{{" CX_SB_LINE_TERMINATOR);
				Print(out, "\t\t\treturn true;" CX_SB_LINE_TERMINATOR);
				Print(out, "\t\t}" CX_SB_LINE_TERMINATOR);
				Print(out, "\t\telse" CX_SB_LINE_TERMINATOR);
				Print(out, "\t\tif (this->{1} > p.{1})" CX_SB_LINE_TERMINATOR, iter->GetMemberName());
				Print(out, "\t\t{{" CX_SB_LINE_TERMINATOR);
				Print(out, "\t\t\treturn false;" CX_SB_LINE_TERMINATOR);
				Print(out, "\t\t}" CX_SB_LINE_TERMINATOR);
			}
			Print(out, CX_SB_LINE_TERMINATOR);
			Print(out, "\t\treturn false;" CX_SB_LINE_TERMINATOR);
		}
		Print(out, "\t}" CX_SB_LINE_TERMINATOR);
		Print(out, CX_SB_LINE_TERMINATOR);

		//=== >

		Print(out, "\tbool operator>(const {1} &p) const" CX_SB_LINE_TERMINATOR, sType);
		Print(out, "\t{{" CX_SB_LINE_TERMINATOR);
		if (object.GetMembers().empty())
		{
			Print(out, "\t\treturn false;" CX_SB_LINE_TERMINATOR);
		}
		else
		{
			for (Object::MembersVector::const_iterator iter = object.GetMembers().begin(); iter != object.GetMembers().end(); ++iter)
			{
				Print(out, "\t\tif (this->{1} > p.{1})" CX_SB_LINE_TERMINATOR, iter->GetMemberName());
				Print(out, "\t\t{{" CX_SB_LINE_TERMINATOR);
				Print(out, "\t\t\treturn true;" CX_SB_LINE_TERMINATOR);
				Print(out, "\t\t}" CX_SB_LINE_TERMINATOR);
				Print(out, "\t\telse" CX_SB_LINE_TERMINATOR);
				Print(out, "\t\tif (this->{1} < p.{1})" CX_SB_LINE_TERMINATOR, iter->GetMemberName());
				Print(out, "\t\t{{" CX_SB_LINE_TERMINATOR);
				Print(out, "\t\t\treturn false;" CX_SB_LINE_TERMINATOR);
				Print(out, "\t\t}" CX_SB_LINE_TERMINATOR);
			}
			Print(out, CX_SB_LINE_TERMINATOR);
			Print(out, "\t\treturn false;" CX_SB_LINE_TERMINATOR);
		}
		Print(out, "\t}" CX_SB_LINE_TERMINATOR);
		Print(out, CX_SB_LINE_TERMINATOR);

		//=== members ===

		Print(out, "private:" CX_SB_LINE_TERMINATOR);
		Print(out, CX_SB_LINE_TERMINATOR);

		String sBitSet;

		Print(&sBitSet, "CX::BitSet<{1}>::Type", object.GetMembers().size());

		Size cMaxDataTypeLen = sBitSet.size();

		for (Object::MembersVector::const_iterator iter = object.GetMembers().begin(); iter != object.GetMembers().end(); ++iter)
		{
			String sTmpFullType;

			if ((status = GetMemberFullType(iter->GetMemberType(), iter->GetKeyType(), iter->GetValType(), sTmpFullType)).IsNOK())
			{
				return status;
			}

			if (cMaxDataTypeLen < sTmpFullType.size())
			{
				cMaxDataTypeLen = sTmpFullType.size();
			}
		}

		for (Object::MembersVector::const_iterator iter = object.GetMembers().begin(); iter != object.GetMembers().end(); ++iter)
		{
			String sTmpFullType;

			if ((status = GetMemberFullType(iter->GetMemberType(), iter->GetKeyType(), iter->GetValType(), sTmpFullType)).IsNOK())
			{
				return status;
			}

			String     sPadding(cMaxDataTypeLen - sTmpFullType.size() + 1, ' ');

			Print(out, "\t{1}{2}{3};" CX_SB_LINE_TERMINATOR, sTmpFullType, sPadding, iter->GetMemberName());
		}

		{
			String     sPadding(cMaxDataTypeLen - sBitSet.size() + 1, ' ');

			Print(out, "\t{1}{2}m_bitsetLoadedMembers;" CX_SB_LINE_TERMINATOR, sBitSet, sPadding);
		}

		Print(out, CX_SB_LINE_TERMINATOR);
		Print(out, "};" CX_SB_LINE_TERMINATOR);

		Print(out, CX_SB_LINE_TERMINATOR);

		for (NamespacesVector::reverse_iterator iter = vectorNamespaces.rbegin(); iter != vectorNamespaces.rend(); ++iter)
		{
			Print(out, CX_SB_LINE_TERMINATOR);
			Print(out, "}//namespace {1}" CX_SB_LINE_TERMINATOR, *iter);
		}

		//=== external init/writer/reader

		Print(out, CX_SB_LINE_TERMINATOR);
		Print(out, "namespace CX" CX_SB_LINE_TERMINATOR);
		Print(out, "{{" CX_SB_LINE_TERMINATOR);
		Print(out, CX_SB_LINE_TERMINATOR);
		Print(out, "namespace SimpleBuffers" CX_SB_LINE_TERMINATOR);
		Print(out, "{{" CX_SB_LINE_TERMINATOR);

		//=== external init

		Print(out, CX_SB_LINE_TERMINATOR);
		Print(out, "template <>" CX_SB_LINE_TERMINATOR);
		Print(out, "struct DataIniter<MemberType_Scalar, {1}>" CX_SB_LINE_TERMINATOR, sFullType);
		Print(out, "{{" CX_SB_LINE_TERMINATOR);
		Print(out, "\tstatic void Init({1} &v)" CX_SB_LINE_TERMINATOR, sFullType);
		Print(out, "\t{{" CX_SB_LINE_TERMINATOR);
		Print(out, "\t\tv.Init();" CX_SB_LINE_TERMINATOR);
		Print(out, "\t}" CX_SB_LINE_TERMINATOR);
		Print(out, "};" CX_SB_LINE_TERMINATOR);

		//=== external writer

		Print(out, CX_SB_LINE_TERMINATOR);
		Print(out, "template <>" CX_SB_LINE_TERMINATOR);
		Print(out, "struct DataWriter<MemberType_Scalar, {1}>" CX_SB_LINE_TERMINATOR, sFullType);
		Print(out, "{{" CX_SB_LINE_TERMINATOR);
		Print(out, "\tstatic Status Write(IWriter *pWriter, const {1} &v, const CX::Char *szName = NULL)" CX_SB_LINE_TERMINATOR, sFullType);
		Print(out, "\t{{" CX_SB_LINE_TERMINATOR);
		Print(out, "\t\treturn v.Write(pWriter, szName);" CX_SB_LINE_TERMINATOR);
		Print(out, "\t}" CX_SB_LINE_TERMINATOR);
		Print(out, "};" CX_SB_LINE_TERMINATOR);

		//=== external reader

		Print(out, CX_SB_LINE_TERMINATOR);
		Print(out, "template <>" CX_SB_LINE_TERMINATOR);
		Print(out, "struct DataReader<MemberType_Scalar, {1}>" CX_SB_LINE_TERMINATOR, sFullType);
		Print(out, "{{" CX_SB_LINE_TERMINATOR);
		Print(out, "\tstatic Status Read(IReader *pReader, {1} &v, const CX::Char *szName = NULL)" CX_SB_LINE_TERMINATOR, sFullType);
		Print(out, "\t{{" CX_SB_LINE_TERMINATOR);
		Print(out, "\t\treturn v.Read(pReader, szName);" CX_SB_LINE_TERMINATOR);
		Print(out, "\t}" CX_SB_LINE_TERMINATOR);
		Print(out, "};" CX_SB_LINE_TERMINATOR);

		Print(out, CX_SB_LINE_TERMINATOR);
		Print(out, "}//namespace SimpleBuffers" CX_SB_LINE_TERMINATOR);
		Print(out, CX_SB_LINE_TERMINATOR);
		Print(out, "}//namespace CX" CX_SB_LINE_TERMINATOR);
		Print(out, CX_SB_LINE_TERMINATOR);

		const Object::PragmasVector &vectorEpilogPragmas = object.GetPragmasByLocation(Object::PRAGMA_LOCATION_EPILOG());

		if (!vectorEpilogPragmas.empty())
		{
			Print(out, CX_SB_LINE_TERMINATOR);
			Print(out, CX_SB_LINE_TERMINATOR);
			for (Object::PragmasVector::const_iterator iter = vectorEpilogPragmas.begin(); iter != vectorEpilogPragmas.end(); ++iter)
			{
#pragma warning(push)
#pragma warning(disable: 4996)
				if (0 == cx_stricmp(iter->sID.c_str(), Object::PRAGMA_CPP()))
#pragma warning(pop)
				{
					Print(out, "{1}" CX_SB_LINE_TERMINATOR, iter->sValue);
				}
			}
		}

		Print(out, CX_SB_LINE_TERMINATOR);

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