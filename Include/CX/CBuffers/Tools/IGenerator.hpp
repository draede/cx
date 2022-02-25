/* 
 * CX - C++ framework for general purpose development
 *
 * https://github.com/draede/cx
 * 
 * Copyright (C) 2014 - 2022 draede - draede [at] outlook [dot] com
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
#include "CX/Status.hpp"
#include "CX/String.hpp"
#include "CX/Vector.hpp"
#include "CX/Map.hpp"
#include "CX/Set.hpp"
#include "CX/CBuffers/Tools/ParserData.hpp"
#include "CX/CBuffers/Types.h"


namespace CX
{

namespace CBuffers
{

namespace Tools
{

class IGenerator
{
public:

	typedef Vector<String>::Type   StringsVector;
	typedef Set<String>::Type      StringsSet;

	virtual ~IGenerator() { }

	virtual const Char *GetName() = 0;

	virtual const Char *GetID() = 0;

	virtual Status Generate(const StructsMap &mapStructs, const Char *szPath) = 0;

	static inline Status PrepareStructs(StructsMap &mapStructs)
	{
		StringsVector vectorNames;
		Status        status;

		if (!(status = GetStructsOrder(vectorNames, mapStructs)))
		{
			return status;
		}
		for (auto iter = vectorNames.begin(); iter != vectorNames.end(); ++iter)
		{
			auto iterEx = mapStructs.find(*iter);

			if (mapStructs.end() == iterEx)
			{
				return Status(Status_OperationFailed, "Failed to find struct from order");
			}
			if (!(status = GetStructSize(iterEx->second, mapStructs)))
			{
				return status;
			}
		}

		return Status();
	}

	static inline Status GetStructsOrder(StringsVector &vectorNames, StructsMap &mapStructs)
	{
		StringsSet   setProcessedNames;
		StringsSet   setLeftOverNames;
		Size         cProcessed;
		Bool         bOK;

		for (auto iter = mapStructs.begin(); iter != mapStructs.end(); ++iter)
		{
			setLeftOverNames.insert(iter->first);
		}
		while (!setLeftOverNames.empty())
		{
			cProcessed = 0;

			auto iter = setLeftOverNames.begin();
			while (iter != setLeftOverNames.end())
			{
				auto iterStructs = mapStructs.find(*iter);

				bOK = True;
				for (auto iterMembers = iterStructs->second.vectorMembers.begin();
				     iterMembers != iterStructs->second.vectorMembers.end(); ++iterMembers)
				{
					if (Type_Struct == iterMembers->nType)
					{
						if (mapStructs.end() == mapStructs.find(iterMembers->sTypeName))
						{
							return Status(Status_InvalidArg, "Unknown type for {1}.{2}", 
							              iterStructs->first, iterMembers->sName);
						}
						if (setProcessedNames.end() == setProcessedNames.find(iterMembers->sTypeName))
						{
							bOK = False;

							break;
						}
					}
				}
				if (bOK)
				{
					cProcessed++;
					vectorNames.push_back(*iter);
					setProcessedNames.insert(*iter);
					iter = setLeftOverNames.erase(iter);
				}
				else
				{
					++iter;
				}
			}
			if (0 == cProcessed)
			{
				break;
			}
		}
		if (!setLeftOverNames.empty())
		{
			return Status(Status_InvalidArg, "Recursive inclusion detected!");
		}

		return Status();
	}

	static inline Status GetStructSize(Struct &s, StructsMap &mapStructs)
	{
		Status status;

		s.cbSize = 0;
		for (auto iter = s.vectorMembers.begin(); iter != s.vectorMembers.end(); ++iter)
		{
			if (!(status = GetMemberSize(*iter, mapStructs)))
			{
				return status;
			}
			iter->cbOffset = s.cbSize;
			s.cbSize += iter->cbSize;
		}

		return Status();
	}

private:

	static inline Status GetMemberSize(Member &m, StructsMap &mapStructs)
	{
		switch (m.nType)
		{
			case Type_Bool:
			{
				if (Aggregate_Array == m.nAggregateType)
				{
					m.cbSize = sizeof(CX_CB_Size) * 2;
				}
				else
				if (Aggregate_StaticArray == m.nAggregateType)
				{
					m.cbSize = sizeof(UInt8) * m.cItemsCount;
				}
				else
				{
					m.cbSize = sizeof(UInt8);
				}
			}
			break;
			case Type_Int8:
			{
				if (Aggregate_Array == m.nAggregateType)
				{
					m.cbSize = sizeof(CX_CB_Size) * 2;
				}
				else
				if (Aggregate_StaticArray == m.nAggregateType)
				{
					m.cbSize = sizeof(Int8) * m.cItemsCount;
				}
				else
				{
					m.cbSize = sizeof(Int8);
				}
			}
			break;
			case Type_UInt8:
			{
				m.cbSize = sizeof(UInt8);
				if (Aggregate_Array == m.nAggregateType)
				{
					m.cbSize = sizeof(CX_CB_Size) * 2;
				}
				else
				if (Aggregate_StaticArray == m.nAggregateType)
				{
					m.cbSize = sizeof(UInt8) * m.cItemsCount;
				}
				else
				{
					m.cbSize = sizeof(UInt8);
				}
			}
			break;
			case Type_Int16: 
			{
				if (Aggregate_Array == m.nAggregateType)
				{
					m.cbSize = sizeof(CX_CB_Size) * 2;
				}
				else
				if (Aggregate_StaticArray == m.nAggregateType)
				{
					m.cbSize = sizeof(Int16) * m.cItemsCount;
				}
				else
				{
					m.cbSize = sizeof(Int16);
				}
			}
			break;
			case Type_UInt16:
			{
				if (Aggregate_Array == m.nAggregateType)
				{
					m.cbSize = sizeof(CX_CB_Size) * 2;
				}
				else
				if (Aggregate_StaticArray == m.nAggregateType)
				{
					m.cbSize = sizeof(UInt16) * m.cItemsCount;
				}
				else
				{
					m.cbSize = sizeof(UInt16);
				}
			}
			break;
			case Type_Int32:
			{
				if (Aggregate_Array == m.nAggregateType)
				{
					m.cbSize = sizeof(CX_CB_Size) * 2;
				}
				else
				if (Aggregate_StaticArray == m.nAggregateType)
				{
					m.cbSize = sizeof(Int32) * m.cItemsCount;
				}
				else
				{
					m.cbSize = sizeof(Int32);
				}
			}
			break;
			case Type_UInt32:
			{
				if (Aggregate_Array == m.nAggregateType)
				{
					m.cbSize = sizeof(CX_CB_Size) * 2;
				}
				else
				if (Aggregate_StaticArray == m.nAggregateType)
				{
					m.cbSize = sizeof(UInt32) * m.cItemsCount;
				}
				else
				{
					m.cbSize = sizeof(UInt32);
				}
			}
			break;
			case Type_Int64:
			{
				if (Aggregate_Array == m.nAggregateType)
				{
					m.cbSize = sizeof(CX_CB_Size) * 2;
				}
				else
				if (Aggregate_StaticArray == m.nAggregateType)
				{
					m.cbSize = sizeof(Int64) * m.cItemsCount;
				}
				else
				{
					m.cbSize = sizeof(Int64);
				}
			}
			break;
			case Type_UInt64:
			{
				if (Aggregate_Array == m.nAggregateType)
				{
					m.cbSize = sizeof(CX_CB_Size) * 2;
				}
				else
				if (Aggregate_StaticArray == m.nAggregateType)
				{
					m.cbSize = sizeof(UInt64) * m.cItemsCount;
				}
				else
				{
					m.cbSize = sizeof(UInt64);
				}
			}
			break;
			case Type_Float:
			{
				if (Aggregate_Array == m.nAggregateType)
				{
					m.cbSize = sizeof(CX_CB_Size) * 2;
				}
				else
				if (Aggregate_StaticArray == m.nAggregateType)
				{
					m.cbSize = sizeof(Float) * m.cItemsCount;
				}
				else
				{
					m.cbSize = sizeof(Float);
				}
			}
			break;
			case Type_Double:
			{
				if (Aggregate_Array == m.nAggregateType)
				{
					m.cbSize = sizeof(CX_CB_Size) * 2;
				}
				else
				if (Aggregate_StaticArray == m.nAggregateType)
				{
					m.cbSize = sizeof(Double) * m.cItemsCount;
				}
				else
				{
					m.cbSize = sizeof(Double);
				}
			}
			break;
			case Type_Char:
			{
				if (Aggregate_Array == m.nAggregateType)
				{
					m.cbSize = sizeof(CX_CB_Size) * 2;
				}
				else
				if (Aggregate_StaticArray == m.nAggregateType)
				{
					m.cbSize = sizeof(Char) * m.cItemsCount;
				}
				else
				{
					m.cbSize = sizeof(Char);
				}
			}
			break;
			case Type_WChar:
			{
				if (Aggregate_Array == m.nAggregateType)
				{
					m.cbSize = sizeof(CX_CB_Size) * 2;
				}
				else
				if (Aggregate_StaticArray == m.nAggregateType)
				{
					m.cbSize = sizeof(WChar) * m.cItemsCount;
				}
				else
				{
					m.cbSize = sizeof(WChar);
				}
			}
			break;
			case Type_Struct:
			{
				auto iterEx = mapStructs.find(m.sTypeName);

				if (mapStructs.end() == iterEx)
				{
					return Status(Status_InvalidArg, "Unknown type for {1}", m.sName);
				}

				if (Aggregate_Array == m.nAggregateType)
				{
					m.cbSize = sizeof(CX_CB_Size) * 2;
				}
				else
				if (Aggregate_StaticArray == m.nAggregateType)
				{
					m.cbSize = iterEx->second.cbSize * m.cItemsCount;
				}
				else
				{
					m.cbSize = iterEx->second.cbSize;
				}
			}
			break;
			default: return Status(Status_InvalidArg, "Invalid type for {1}", m.sName);
		}

		return Status();
	}

};

}//namespace Tools

}//namespace CBuffers

}//namespace CX
