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

#include "CX/IO/Helper.hpp"
#include "CX/Stack.hpp"


namespace CX
{

namespace IO
{

Helper::Helper()
{
}

Helper::~Helper()
{
}

Status Helper::CopyStream(IInputStream *pInputStream, IOutputStream *pOutputStream)
{
	Byte   buffer[COPY_STREAM_BUFFER];
	Size   cbAckSize;
	Size   cbAckSize2;
	Status status;

	while (!pInputStream->IsEOF())
	{
		if ((status = pInputStream->Read(buffer, sizeof(buffer), &cbAckSize)).IsNOK())
		{
			return status;
		}
		if (0 < cbAckSize)
		{
			if ((status = pOutputStream->Write(buffer, cbAckSize, &cbAckSize2)).IsNOK())
			{
				return status;
			}
			if (cbAckSize2 != cbAckSize)
			{
				return Status(Status_WriteFailed, "Failed to write all bytes");
			}
		}
	}

	return Status();
}

Status Helper::CopyData(IDataReader *pDataReader, IDataWriter *pDataWriter)
{
	typedef Stack<Bool>::Type   EntriesStack;

	EntriesStack stack;
	Status       status;

	if (IDataReader::EntryType_Object == pDataReader->GetRootEntryType())
	{
		stack.push(True);
		if ((status = pDataReader->BeginRootObject()).IsNOK())
		{
			return status;
		}
		if ((status = pDataWriter->BeginRootObject()).IsNOK())
		{
			return status;
		}
	}
	else
	if (IDataReader::EntryType_Array == pDataReader->GetRootEntryType())
	{
		stack.push(False);
		if ((status = pDataReader->BeginRootArray()).IsNOK())
		{
			return status;
		}
		if ((status = pDataWriter->BeginRootArray()).IsNOK())
		{
			return status;
		}
	}
	else
	{
		return Status(Status_InvalidArg, "Invalid data reader");
	}

	IDataReader::EntryType nEntryType;

	while (!stack.empty())
	{
		nEntryType = pDataReader->GetEntryType();
		status     = Status();
		switch (nEntryType)
		{
			case IDataReader::EntryType_Invalid:
			{
				return Status(Status_InvalidArg, "Invalid data reader");
			}
			break;
			case IDataReader::EntryType_EOG:
			{
				status = Status(Status_OutOfBounds);
			}
			break;
			case IDataReader::EntryType_Null:
			{
				if (stack.top())
				{
					String sName;

					if ((status = pDataReader->ReadNull(&sName)).IsNOK())
					{
						if (status.GetCode() == Status_OutOfBounds)
						{
							break;
						}
						else
						{
							return status;
						}
					}
					else
					if ((status = pDataWriter->WriteNull(sName.c_str())).IsNOK())
					{
						return status;
					}
				}
				else
				{
					if ((status = pDataReader->ReadNull()).IsNOK())
					{
						if (status.GetCode() == Status_OutOfBounds)
						{
							break;
						}
						else
						{
							return status;
						}
					}
					else
					if ((status = pDataWriter->WriteNull()).IsNOK())
					{
						return status;
					}
				}
			}
			break;
			case IDataReader::EntryType_Bool:
			{
				Bool val;

				if (stack.top())
				{
					String sName;

					if ((status = pDataReader->ReadBool(&sName, &val)).IsNOK())
					{
						if (status.GetCode() == Status_OutOfBounds)
						{
							break;
						}
						else
						{
							return status;
						}
					}
					else
					if ((status = pDataWriter->WriteBool(sName.c_str(), val)).IsNOK())
					{
						return status;
					}
				}
				else
				{
					if ((status = pDataReader->ReadBool(&val)).IsNOK())
					{
						if (status.GetCode() == Status_OutOfBounds)
						{
							break;
						}
						else
						{
							return status;
						}
					}
					else
					if ((status = pDataWriter->WriteBool(val)).IsNOK())
					{
						return status;
					}
				}
			}
			break;
			case IDataReader::EntryType_Int:
			{
				Int64 val;

				if (stack.top())
				{
					String sName;

					if ((status = pDataReader->ReadInt(&sName, &val)).IsNOK())
					{
						if (status.GetCode() == Status_OutOfBounds)
						{
							break;
						}
						else
						{
							return status;
						}
					}
					else
					if ((status = pDataWriter->WriteInt(sName.c_str(), val)).IsNOK())
					{
						return status;
					}
				}
				else
				{
					if ((status = pDataReader->ReadInt(&val)).IsNOK())
					{
						if (status.GetCode() == Status_OutOfBounds)
						{
							break;
						}
						else
						{
							return status;
						}
					}
					else
					if ((status = pDataWriter->WriteInt(val)).IsNOK())
					{
						return status;
					}
				}
			}
			break;
			case IDataReader::EntryType_Real:
			{
				Double val;

				if (stack.top())
				{
					String sName;

					if ((status = pDataReader->ReadReal(&sName, &val)).IsNOK())
					{
						if (status.GetCode() == Status_OutOfBounds)
						{
							break;
						}
						else
						{
							return status;
						}
					}
					else
					if ((status = pDataWriter->WriteReal(sName.c_str(), val)).IsNOK())
					{
						return status;
					}
				}
				else
				{
					if ((status = pDataReader->ReadReal(&val)).IsNOK())
					{
						if (status.GetCode() == Status_OutOfBounds)
						{
							break;
						}
						else
						{
							return status;
						}
					}
					else
					if ((status = pDataWriter->WriteReal(val)).IsNOK())
					{
						return status;
					}
				}
			}
			break;
			case IDataReader::EntryType_String:
			{
				String val;

				if (stack.top())
				{
					String sName;

					if ((status = pDataReader->ReadString(&sName, &val)).IsNOK())
					{
						if (status.GetCode() == Status_OutOfBounds)
						{
							break;
						}
						else
						{
							return status;
						}
					}
					else
					if ((status = pDataWriter->WriteString(sName.c_str(), val.c_str())).IsNOK())
					{
						return status;
					}
				}
				else
				{
					if ((status = pDataReader->ReadString(&val)).IsNOK())
					{
						if (status.GetCode() == Status_OutOfBounds)
						{
							break;
						}
						else
						{
							return status;
						}
					}
					else
					if ((status = pDataWriter->WriteString(val.c_str())).IsNOK())
					{
						return status;
					}
				}
			}
			break;
			case IDataReader::EntryType_BLOB:
			{
				void *pData;
				Size cbSize;

				if (stack.top())
				{
					String sName;

					if ((status = pDataReader->ReadBLOB(&sName, &pData, &cbSize)).IsNOK())
					{
						if (status.GetCode() == Status_OutOfBounds)
						{
							break;
						}
						else
						{
							return status;
						}
					}
					else
					if ((status = pDataWriter->WriteBLOB(sName.c_str(), pData, cbSize)).IsNOK())
					{
						Free(pData);

						return status;
					}
					else
					{
						Free(pData);
					}
				}
				else
				{
					if ((status = pDataReader->ReadBLOB(&pData, &cbSize)).IsNOK())
					{
						if (status.GetCode() == Status_OutOfBounds)
						{
							break;
						}
						else
						{
							return status;
						}
					}
					else
					if ((status = pDataWriter->WriteBLOB(pData, cbSize)).IsNOK())
					{
						Free(pData);

						return status;
					}
					else
					{
						Free(pData);
					}
				}
			}
			break;
			case IDataReader::EntryType_Object:
			{
				if (stack.top())
				{
					String sName;

					if ((status = pDataReader->BeginObject(&sName)).IsNOK())
					{
						if (status.GetCode() == Status_OutOfBounds)
						{
							break;
						}
						else
						{
							return status;
						}
					}
					else
					if ((status = pDataWriter->BeginObject(sName.c_str())).IsNOK())
					{
						return status;
					}
				}
				else
				{
					if ((status = pDataReader->BeginObject()).IsNOK())
					{
						if (status.GetCode() == Status_OutOfBounds)
						{
							break;
						}
						else
						{
							return status;
						}
					}
					else
					if ((status = pDataWriter->BeginObject()).IsNOK())
					{
						return status;
					}
				}
				stack.push(True);
			}
			break;
			case IDataReader::EntryType_Array:
			{
				if (stack.top())
				{
					String sName;

					if ((status = pDataReader->BeginArray(&sName)).IsNOK())
					{
						if (status.GetCode() == Status_OutOfBounds)
						{
							break;
						}
						else
						{
							return status;
						}
					}
					else
					if ((status = pDataWriter->BeginArray(sName.c_str())).IsNOK())
					{
						return status;
					}
				}
				else
				{
					if ((status = pDataReader->BeginArray()).IsNOK())
					{
						if (status.GetCode() == Status_OutOfBounds)
						{
							break;
						}
						else
						{
							return status;
						}
					}
					else
					if ((status = pDataWriter->BeginArray()).IsNOK())
					{
						return status;
					}
				}
				stack.push(False);
			}
			break;
		}
		if (Status_OutOfBounds == status.GetCode())
		{
			if (stack.top())
			{
				stack.pop();
				if (stack.empty())
				{
					if ((status = pDataReader->EndRootObject()).IsNOK())
					{
						return status;
					}
					if ((status = pDataWriter->EndRootObject()).IsNOK())
					{
						return status;
					}
				}
				else
				{
					if ((status = pDataReader->EndObject()).IsNOK())
					{
						return status;
					}
					if ((status = pDataWriter->EndObject()).IsNOK())
					{
						return status;
					}
				}
			}
			else
			{
				stack.pop();
				if (stack.empty())
				{
					if ((status = pDataReader->EndRootArray()).IsNOK())
					{
						return status;
					}
					if ((status = pDataWriter->EndRootArray()).IsNOK())
					{
						return status;
					}
				}
				else
				{
					if ((status = pDataReader->EndArray()).IsNOK())
					{
						return status;
					}
					if ((status = pDataWriter->EndArray()).IsNOK())
					{
						return status;
					}
				}
			}
		}
	}

	return Status();
}

}//namespace IO

}//namespace CX

