
#pragma once


#include "CX/Types.hpp"
#include "CX/String.hpp"
#include "CX/Vector.hpp"
#include "CX/Print.hpp"


namespace CX
{

namespace SB
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
		Type_Struct,
	};

	String  m_sName;
	Type    m_nType;
	bool    m_bIsVector;
	String  m_sStructName;

	Field()
	{
	}

	Field(const Char *szName, Type nType, bool bIsVector, const Char *szStructName = NULL)
	{
		m_sName     = szName;
		m_nType     = nType;
		m_bIsVector = bIsVector;
		if (Type_Struct == nType)
		{
			m_sStructName = szStructName;
		}
	}

	static Type GetTypeFromString(const Char *szStr)
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
			return Field::Type_Struct;
		}
	}

	static const Char *GetStringFromType(Type nType, bool bIsVector, const Char *szStructName)
	{
		if (!bIsVector)
		{
			switch (nType)
			{
				case Type_Bool:     return "CX_Bool";
				case Type_Int8:     return "CX_Int8";
				case Type_UInt8:    return "CX_UInt8";
				case Type_Int16:    return "CX_Int16";
				case Type_UInt16:   return "CX_UInt16";
				case Type_Int32:    return "CX_Int32";
				case Type_UInt32:   return "CX_UInt32";
				case Type_Int64:    return "CX_Int64";
				case Type_UInt64:   return "CX_UInt64";
				case Type_Float:    return "CX_Float";
				case Type_Double:   return "CX_Double";
				case Type_String:   return "CX_Char *";
				case Type_WString:  return "CX_WChar *";
				case Type_Struct:   return szStructName;
			}
		}
		else
		{
			switch (nType)
			{
				case Type_Bool:     return "CX_SB_BoolArray";
				case Type_Int8:     return "CX_SB_Int8Array";
				case Type_UInt8:    return "CX_SB_UInt8Array";
				case Type_Int16:    return "CX_SB_Int16Array";
				case Type_UInt16:   return "CX_SB_UInt16Array";
				case Type_Int32:    return "CX_SB_Int32Array";
				case Type_UInt32:   return "CX_SB_UInt32Array";
				case Type_Int64:    return "CX_SB_Int64Array";
				case Type_UInt64:   return "CX_SB_UInt64Array";
				case Type_Float:    return "CX_SB_FloatArray";
				case Type_Double:   return "CX_SB_DoubleArray";
				case Type_String:   return "CX_SB_StringArray";
				case Type_WString:  return "CX_SB_WStringArray";
				case Type_Struct:
				{
					static String sStr;

					sStr.clear();
					Print(&sStr, "{1}Array", szStructName);

					return sStr.c_str();
				}
				break;
			}
		}

		return "";
	}

};

typedef Vector<Field>::Type   FieldsVector;


}//namespace SB

}//namespace CX

