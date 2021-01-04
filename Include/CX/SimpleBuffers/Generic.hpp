/* 
 * CX - C++ framework for general purpose development
 *
 * https://github.com/draede/cx
 * 
 * Copyright (C) 2014 - 2021 draede - draede [at] outlook [dot] com
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


#include "CX/Vector.hpp"
#include "CX/Status.hpp"
#include "CX/String.hpp"
#include "CX/Stack.hpp"
#include "CX/SimpleBuffers/IReader.hpp"
#include "CX/SimpleBuffers/DataReader.hpp"
#include "CX/SimpleBuffers/DataWriter.hpp"
#include "CX/SimpleBuffers/DataIniter.hpp"
#include "CX/Value.hpp"
#include "CX/SimpleBuffers/BSONValue.hpp"


namespace CX
{

namespace SimpleBuffers
{

template <>
struct DataWriter<MemberType_Scalar, Value>
{
	static Status Write(IWriter *pWriter, const Value &v, const CX::Char *szName = NULL)
	{
		struct Node
		{
			const Value *pValue;
			int         cIndex;
		};

		typedef Stack<Node>::Type   NodesStack;

		NodesStack  stackNodes;
		const Value *pValue;
		Status      status;

		pValue = &v;
		for (;;)
		{
			String     sKey;
			const Char *szKey = NULL;

			if (!stackNodes.empty())
			{
				stackNodes.top().cIndex++;
				if (stackNodes.top().pValue->IsObject())
				{
					if ((Size)stackNodes.top().cIndex < (int)stackNodes.top().pValue->GetMembersCount())
					{
						pValue = &stackNodes.top().pValue->GetMemberByIndex((Size)stackNodes.top().cIndex, &sKey, &status);
						if (!status)
						{
							return status;
						}
						szKey = sKey.c_str();
					}
					else
					{
						if (!(status = pWriter->EndObject()))
						{
							return status;
						}
						stackNodes.pop();
						if (stackNodes.empty())
						{
							break;
						}
						else
						{
							continue;
						}
					}
				}
				else
				{
					if ((Size)stackNodes.top().cIndex < (int)stackNodes.top().pValue->GetItemsCount())
					{
						pValue = &stackNodes.top().pValue->GetItem((Size)stackNodes.top().cIndex, &status);
						if (!status)
						{
							return status;
						}
					}
					else
					{
						if (!(status = pWriter->EndArray()))
						{
							return status;
						}
						stackNodes.pop();
						if (stackNodes.empty())
						{
							break;
						}
						else
						{
							continue;
						}
					}
				}
			}
			else
			{
				szKey = szName;
			}

			if (pValue->IsBool())
			{
				if (!(status = pWriter->WriteBool(pValue->GetBool(), szKey)))
				{
					return status;
				}
			}
			else
			if (pValue->IsInt())
			{
				if (!(status = pWriter->WriteInt64(pValue->GetInt(), szKey)))
				{
					return status;
				}
			}
			else
			if (pValue->IsUInt())
			{
				if (!(status = pWriter->WriteUInt64(pValue->GetUInt(), szKey)))
				{
					return status;
				}
			}
			else
			if (pValue->IsUInt())
			{
				if (!(status = pWriter->WriteUInt64(pValue->GetUInt(), szKey)))
				{
					return status;
				}
			}
			else
			if (pValue->IsReal())
			{
				if (!(status = pWriter->WriteDouble(pValue->GetReal(), szKey)))
				{
					return status;
				}
			}
			else
			if (pValue->IsString())
			{
				if (!(status = pWriter->WriteString(pValue->GetString().c_str(), szKey)))
				{
					return status;
				}
			}
			else
			if (pValue->IsObject())
			{
				if (!(status = pWriter->BeginObject(szKey)))
				{
					return status;
				}

				Node node;

				node.pValue = pValue;
				node.cIndex = -1;

				stackNodes.push(node);
			}
			else
			if (pValue->IsArray())
			{
				if (!(status = pWriter->BeginArray(szKey)))
				{
					return status;
				}

				Node node;

				node.pValue = pValue;
				node.cIndex = -1;

				stackNodes.push(node);
			}
			else
			{
				return Status_OperationFailed;
			}

			if (stackNodes.empty())
			{
				break;
			}
		}

		return Status();
	}
};

template <>
struct DataReader<MemberType_Scalar, Value>
{
	static Status Read(IReader *pReader, Value &v, const CX::Char *szName = NULL)
	{

		struct Helper : public ICustom
		{
			Value *pValue;
			Value *pCurrent;

			virtual Status OnBeginObject(const Char *szName = NULL)
			{
				Status status;

				if (NULL == pCurrent)
				{
					if (!(status = pValue->SetAsObject()))
					{
						return status;
					}
					pCurrent = pValue;
				}
				else
				{
					if (pCurrent->IsObject())
					{
						if (NULL == szName)
						{
							return Status_InvalidCall;
						}

						Value &val = pCurrent->AddMember(szName, &status);
						
						if (!status)
						{
							return status;
						}
						if (!(status = val.SetAsObject()))
						{
							return status;
						}
						pCurrent = &val;
					}
					else
					{
						if (NULL != szName)
						{
							return Status_InvalidCall;
						}

						Value &val = pCurrent->AddItem(&status);

						if (!status)
						{
							return status;
						}
						if (!(status = val.SetAsObject()))
						{
							return status;
						}
						pCurrent = &val;
					}
				}

				return Status();
			}

			virtual Status OnEndObject()
			{
				if (NULL == pCurrent)
				{
					return Status_InvalidCall;
				}
				if (!pCurrent->IsObject())
				{
					return Status_InvalidCall;
				}
				pCurrent = &pCurrent->GetParent();

				return Status();
			}

			virtual Status OnBeginArray(const Char *szName = NULL)
			{
				Status status;

				if (NULL == pCurrent)
				{
					if (!(status = pValue->SetAsArray()))
					{
						return status;
					}
					pCurrent = pValue;
				}
				else
				{
					if (pCurrent->IsObject())
					{
						if (NULL == szName)
						{
							return Status_InvalidCall;
						}

						Value &val = pCurrent->AddMember(szName, &status);
						
						if (!status)
						{
							return status;
						}
						if (!(status = val.SetAsArray()))
						{
							return status;
						}
						pCurrent = &val;
					}
					else
					{
						if (NULL != szName)
						{
							return Status_InvalidCall;
						}

						Value &val = pCurrent->AddItem(&status);

						if (!status)
						{
							return status;
						}
						if (!(status = val.SetAsArray()))
						{
							return status;
						}
						pCurrent = &val;
					}
				}

				return Status();
			}

			virtual Status OnEndArray()
			{
				if (NULL == pCurrent)
				{
					return Status_InvalidCall;
				}
				if (!pCurrent->IsArray())
				{
					return Status_InvalidCall;
				}
				pCurrent = &pCurrent->GetParent();

				return Status();
			}

			virtual Status OnBool(Bool v, const Char *szName = NULL)
			{
				Status status;

				if (NULL == pCurrent)
				{
					if (!(status = pValue->SetBool(v)))
					{
						return status;
					}
				}
				else
				{
					if (pCurrent->IsObject())
					{
						if (NULL == szName)
						{
							return Status_InvalidCall;
						}

						Value &val = pCurrent->AddMember(szName, &status);
						
						if (!status)
						{
							return status;
						}
						if (!(status = val.SetBool(v)))
						{
							return status;
						}
					}
					else
					{
						if (NULL != szName)
						{
							return Status_InvalidCall;
						}

						Value &val = pCurrent->AddItem(&status);

						if (!status)
						{
							return status;
						}
						if (!(status = val.SetBool(v)))
						{
							return status;
						}
					}
				}

				return Status();
			}

			virtual Status OnInt(Int64 v, const Char *szName = NULL)
			{
				Status status;

				if (NULL == pCurrent)
				{
					if (!(status = pValue->SetInt(v)))
					{
						return status;
					}
				}
				else
				{
					if (pCurrent->IsObject())
					{
						if (NULL == szName)
						{
							return Status_InvalidCall;
						}

						Value &val = pCurrent->AddMember(szName, &status);

						if (!status)
						{
							return status;
						}
						if (!(status = val.SetInt(v)))
						{
							return status;
						}
					}
					else
					{
						if (NULL != szName)
						{
							return Status_InvalidCall;
						}

						Value &val = pCurrent->AddItem(&status);

						if (!status)
						{
							return status;
						}
						if (!(status = val.SetInt(v)))
						{
							return status;
						}
					}
				}

				return Status();
			}

			virtual Status OnDouble(Double v, const Char *szName = NULL)
			{
				Status status;

				if (NULL == pCurrent)
				{
					if (!(status = pValue->SetReal(v)))
					{
						return status;
					}
				}
				else
				{
					if (pCurrent->IsObject())
					{
						if (NULL == szName)
						{
							return Status_InvalidCall;
						}

						Value &val = pCurrent->AddMember(szName, &status);
						
						if (!status)
						{
							return status;
						}
						if (!(status = val.SetReal(v)))
						{
							return status;
						}
					}
					else
					{
						if (NULL != szName)
						{
							return Status_InvalidCall;
						}

						Value &val = pCurrent->AddItem(&status);

						if (!status)
						{
							return status;
						}
						if (!(status = val.SetReal(v)))
						{
							return status;
						}
					}
				}

				return Status();
			}

			virtual Status OnString(const Char *v, const Char *szName = NULL)
			{
				Status status;

				if (NULL == pCurrent)
				{
					if (!(status = pValue->SetString(v)))
					{
						return status;
					}
				}
				else
				{
					if (pCurrent->IsObject())
					{
						if (NULL == szName)
						{
							return Status_InvalidCall;
						}

						Value &val = pCurrent->AddMember(szName, &status);
						
						if (!status)
						{
							return status;
						}
						if (!(status = val.SetString(v)))
						{
							return status;
						}
					}
					else
					{
						if (NULL != szName)
						{
							return Status_InvalidCall;
						}

						Value &val = pCurrent->AddItem(&status);

						if (!status)
						{
							return status;
						}
						if (!(status = val.SetString(v)))
						{
							return status;
						}
					}
				}

				return Status();
			}

		};

		Helper helper;
		Status status;

		helper.pValue   = &v;
		helper.pCurrent = NULL;

		if (!(status = pReader->ReadCustom(&helper, szName)))
		{
			return status;
		}

		return Status();
	}
};

template <>
struct DataIniter<MemberType_Scalar, Value>
{
	static void Init(Value &v)
	{
		v.SetNull();
	}
};

template <>
struct DataWriter<MemberType_Scalar, BSONValue>
{
	static Status Write(IWriter *pWriter, const BSONValue &v, const CX::Char *szName = NULL)
	{
		return v.Write(pWriter, szName);
	}
};

template <>
struct DataReader<MemberType_Scalar, BSONValue>
{
	static Status Read(IReader *pReader, BSONValue &v, const CX::Char *szName = NULL)
	{
		return v.Read(pReader, szName);
	}
};

template <>
struct DataIniter<MemberType_Scalar, BSONValue>
{
	static void Init(BSONValue &v)
	{
		v.Init();
	}
};

}//namespace SimpleBuffers

}//namespace CX
