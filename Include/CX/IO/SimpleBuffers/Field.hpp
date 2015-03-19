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
#include "CX/String.hpp"
#include "CX/Vector.hpp"
#include "CX/Print.hpp"


namespace CX
{

namespace IO
{

namespace SimpleBuffers
{

class Field
{
public:

	enum Type
	{
		Type_Bool,
		Type_Int8,
		Type_UInt8,
		Type_Int16,
		Type_UInt16,
		Type_Int32,
		Type_UInt32,
		Type_Int64,
		Type_UInt64,
		Type_Float,
		Type_Double,
		Type_String,
		Type_WString,
		Type_Object,
	};

	Type    m_nType;
	bool    m_bIsVector;
#pragma warning(push)
#pragma warning(disable: 4251)
	String  m_sName;
	String  m_sObjectName;
#pragma warning(pop)

	Field()
	{
	}

	Field(const Char *szName, Type nType, bool bIsVector = false, const Char *szObjectName = NULL)
	{
		m_sName     = szName;
		m_nType     = nType;
		m_bIsVector = bIsVector;
		if (Type_Object == nType)
		{
			m_sObjectName = szObjectName;
		}
	}

	static Type GetTypeFromProtoString(const Char *szStr)
	{
		if (0 == cx_strcmp(szStr, "bool"))
		{
			return Field::Type_Bool;
		}
		else
		if (0 == cx_strcmp(szStr, "int8"))
		{
			return Field::Type_Int8;
		}
		else
		if (0 == cx_strcmp(szStr, "uint8"))
		{
			return Field::Type_UInt8;
		}
		else
		if (0 == cx_strcmp(szStr, "int16"))
		{
			return Field::Type_Int16;
		}
		else
		if (0 == cx_strcmp(szStr, "uint16"))
		{
			return Field::Type_UInt16;
		}
		else
		if (0 == cx_strcmp(szStr, "int32"))
		{
			return Field::Type_Int32;
		}
		else
		if (0 == cx_strcmp(szStr, "uint32"))
		{
			return Field::Type_UInt32;
		}
		else
		if (0 == cx_strcmp(szStr, "int64"))
		{
			return Field::Type_Int64;
		}
		else
		if (0 == cx_strcmp(szStr, "uint64"))
		{
			return Field::Type_UInt64;
		}
		else
		if (0 == cx_strcmp(szStr, "float"))
		{
			return Field::Type_Float;
		}
		else
		if (0 == cx_strcmp(szStr, "double"))
		{
			return Field::Type_Double;
		}
		else
		if (0 == cx_strcmp(szStr, "string"))
		{
			return Field::Type_String;
		}
		else
		if (0 == cx_strcmp(szStr, "wstring"))
		{
			return Field::Type_WString;
		}
		else
		{
			return Field::Type_Object;
		}
	}

	static const Char *GetProtoStringFromType(Type nType, const Char *szObjectName)
	{
		if (Type_Bool == nType)
		{
			return "bool";
		}
		else
		if (Type_Int8 == nType)
		{
			return "int8";
		}
		else
		if (Type_UInt8 == nType)
		{
			return "uint8";
		}
		else
		if (Type_Int16 == nType)
		{
			return "int16";
		}
		else
		if (Type_UInt16 == nType)
		{
			return "uint16";
		}
		else
		if (Type_Int32 == nType)
		{
			return "int32";
		}
		else
		if (Type_UInt32 == nType)
		{
			return "uint32";
		}
		else
		if (Type_Int64 == nType)
		{
			return "int64";
		}
		else
		if (Type_UInt64 == nType)
		{
			return "uint64";
		}
		else
		if (Type_Float == nType)
		{
			return "float";
		}
		else
		if (Type_Double == nType)
		{
			return "double";
		}
		else
		if (Type_String == nType)
		{
			return "string";
		}
		else
		if (Type_WString == nType)
		{
			return "wstring";
		}
		else
		if (Type_Object == nType)
		{
			return szObjectName;
		}
		else
		{
			return "";
		}
	}

	static const Char *GetStringFromType(Type nType, bool bIsVector, const Char *szObjectName)
	{
		if (!bIsVector)
		{
			switch (nType)
			{
				case Type_Bool:     return "CX::Bool";
				case Type_Int8:     return "CX::Int8";
				case Type_UInt8:    return "CX::UInt8";
				case Type_Int16:    return "CX::Int16";
				case Type_UInt16:   return "CX::UInt16";
				case Type_Int32:    return "CX::Int32";
				case Type_UInt32:   return "CX::UInt32";
				case Type_Int64:    return "CX::Int64";
				case Type_UInt64:   return "CX::UInt64";
				case Type_Float:    return "CX::Float";
				case Type_Double:   return "CX::Double";
				case Type_String:   return "CX::String";
				case Type_WString:  return "CX::WString";
				case Type_Object:   return szObjectName;
			}
		}
		else
		{
			switch (nType)
			{
				case Type_Bool:     return "CX::IO::SimpleBuffers::BoolArray";
				case Type_Int8:     return "CX::IO::SimpleBuffers::Int8Array";
				case Type_UInt8:    return "CX::IO::SimpleBuffers::UInt8Array";
				case Type_Int16:    return "CX::IO::SimpleBuffers::Int16Array";
				case Type_UInt16:   return "CX::IO::SimpleBuffers::UInt16Array";
				case Type_Int32:    return "CX::IO::SimpleBuffers::Int32Array";
				case Type_UInt32:   return "CX::IO::SimpleBuffers::UInt32Array";
				case Type_Int64:    return "CX::IO::SimpleBuffers::Int64Array";
				case Type_UInt64:   return "CX::IO::SimpleBuffers::UInt64Array";
				case Type_Float:    return "CX::IO::SimpleBuffers::FloatArray";
				case Type_Double:   return "CX::IO::SimpleBuffers::DoubleArray";
				case Type_String:   return "CX::IO::SimpleBuffers::StringArray";
				case Type_WString:  return "CX::IO::SimpleBuffers::WStringArray";
				case Type_Object:
				{
					static String sStr;

					sStr.clear();
					Print(&sStr, "{1}Array", szObjectName);

					return sStr.c_str();
				}
				break;
			}
		}

		return "";
	}

	static const Char *GetStringFromTypeForProto(Type nType, bool bIsVector, const Char *szObjectName)
	{
		if (!bIsVector)
		{
			switch (nType)
			{
				case Type_Bool:     return "bool";
				case Type_Int8:     return "int8";
				case Type_UInt8:    return "uint8";
				case Type_Int16:    return "int16";
				case Type_UInt16:   return "uint16";
				case Type_Int32:    return "int32";
				case Type_UInt32:   return "uint32";
				case Type_Int64:    return "int64";
				case Type_UInt64:   return "uint64";
				case Type_Float:    return "float";
				case Type_Double:   return "double";
				case Type_String:   return "string";
				case Type_WString:  return "wstring";
				case Type_Object:   return szObjectName;
			}
		}
		else
		{
			switch (nType)
			{
				case Type_Bool:     return "bool[]";
				case Type_Int8:     return "int8[]";
				case Type_UInt8:    return "uint8[]";
				case Type_Int16:    return "int16[]";
				case Type_UInt16:   return "uint16[]";
				case Type_Int32:    return "int32[]";
				case Type_UInt32:   return "uint32[]";
				case Type_Int64:    return "int64[]";
				case Type_UInt64:   return "uint64[]";
				case Type_Float:    return "float[]";
				case Type_Double:   return "double[]";
				case Type_String:   return "string[]";
				case Type_WString:  return "wstring[]";
				case Type_Object:
				{
					static String sStr;

					sStr.clear();
					Print(&sStr, "{1}[]", szObjectName);

					return sStr.c_str();
				}
				break;
			}
		}

		return "";
	}

};

typedef Vector<Field>::Type   FieldsVector;

}//namespace SimpleBuffers

}//namespace IO

}//namespace CX

