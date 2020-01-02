/* 
 * CX - C++ framework for general purpose development
 *
 * https://github.com/draede/cx
 * 
 * Copyright (C) 2014 - 2020 draede - draede [at] outlook [dot] com
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
#include "CX/Map.hpp"


namespace CX
{

namespace CBuffers
{

namespace Tools
{

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
	Type_Char,
	Type_WChar,
	Type_Struct,
};

enum AggregateType
{
	Aggregate_Scalar,
	Aggregate_StaticArray,
	Aggregate_Array,
};

struct Member
{
	String          sName;
	Size            cbOffset;
	Size            cbSize;
	Type            nType;
	AggregateType   nAggregateType;
	Size            cItemsCount;
	String          sTypeName;
	String          sActualTypeName;

	Bool Compare(const Member &m)
	{
		if (nType != m.nType)
		{
			return False;
		}
		if (nAggregateType != m.nAggregateType)
		{
			return False;
		}
		if (Aggregate_StaticArray == nAggregateType)
		{
			if (cItemsCount != m.cItemsCount)
			{
				return false;
			}
		}
#pragma warning(push)
#pragma warning(disable: 4996)
		if (0 != cx_strcmp(sName.c_str(), m.sName.c_str()))
#pragma warning(pop)
		{
			return False;
		}

		return True;
	}
};

typedef Vector<Member>::Type   MembersVector;

struct Struct
{
	static const Size   MAX_STATIC_ARRAY_COUNT = 67108864;

	String          sName;
	Size            cbSize;
	MembersVector   vectorMembers;

	Bool Compare(const Struct &s)
	{
		if (vectorMembers.size() != s.vectorMembers.size())
		{
			return False;
		}
#pragma warning(push)
#pragma warning(disable: 4996)
		if (0 != cx_strcmp(sName.c_str(), s.sName.c_str()))
#pragma warning(pop)
		{
			return False;
		}

		Size cCount = vectorMembers.size();

		for (Size i = 0; i < cCount; i++)
		{
			if (!vectorMembers[i].Compare(s.vectorMembers[i]))
			{
				return False;
			}
		}

		return True;
	}
};

typedef Map<String, Struct>::Type   StructsMap;

}//namespace Tools

}//namespace CBuffers

}//namespace CX
