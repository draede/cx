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

#include "CX/precomp.hpp"
#include "CX/Data/NPY/Header.hpp"
#include "CX/Data/NPY/HeaderSAXDictionaryObserver.hpp"


namespace CX
{

namespace Data
{

namespace NPY
{

const Byte Header::MAGIC[6]       = { 0x93, (Byte)'N', (Byte)'U', (Byte)'M', (Byte)'P', (Byte)'Y' };
const Byte Header::MAGIC_V_1_0[8] = { 0x93, (Byte)'N', (Byte)'U', (Byte)'M', (Byte)'P', (Byte)'Y', 0x01, 0x00 };
const Byte Header::MAGIC_V_2_0[8] = { 0x93, (Byte)'N', (Byte)'U', (Byte)'M', (Byte)'P', (Byte)'Y', 0x02, 0x00 };
const Byte Header::MAGIC_V_3_0[8] = { 0x93, (Byte)'N', (Byte)'U', (Byte)'M', (Byte)'P', (Byte)'Y', 0x03, 0x00 };

Status Header::Read(const void *pBuffer, Size cbSize)
{
	Size     cbDictSize;
	Size     cbLenSize;
	Status   status;

	if (sizeof(Header::MAGIC_V_1_0) > cbSize)
	{
		return Status(Status_TooSmall, "Buffer too small");
	}

	const Byte *header = (const Byte *)pBuffer;

	if (0 != memcmp(header, Header::MAGIC, sizeof(Header::MAGIC)))
	{
		return Status(Status_InvalidArg, "Invalid header");
	}

	if (0 == memcmp(header, Header::MAGIC_V_1_0, sizeof(Header::MAGIC_V_1_0)))
	{
		nVersion = Version_1_0;
	}
	else
	if (0 == memcmp(header, Header::MAGIC_V_2_0, sizeof(Header::MAGIC_V_2_0)))
	{
		nVersion = Version_2_0;
	}
	else
	if (0 == memcmp(header, Header::MAGIC_V_3_0, sizeof(Header::MAGIC_V_3_0)))
	{
		nVersion = Version_3_0;
	}
	else
	{
		return Status(Status_InvalidArg, "Invalid version");
	}

	if (Version_1_0 == nVersion)
	{
		if (sizeof(Header::MAGIC_V_1_0) + sizeof(UInt16) > cbSize)
		{
			return Status(Status_TooSmall, "Buffer too small");
		}
		cbLenSize = sizeof(UInt16);

		cbDictSize = (Size)*(UInt16 *)(header + sizeof(Header::MAGIC_V_1_0));
	}
	else
	{
		if (sizeof(Header::MAGIC_V_1_0) + sizeof(UInt32) > cbSize)
		{
			return Status(Status_TooSmall, "Buffer too small");
		}
		cbLenSize = sizeof(UInt32);

		cbDictSize = (Size)*(UInt32 *)(header + sizeof(Header::MAGIC_V_1_0));
	}

	if (sizeof(Header::MAGIC_V_1_0) + cbLenSize + cbDictSize > cbSize)
	{
		return Status(Status_TooSmall, "Buffer too small");
	}

	if (!(status = HeaderSAXDictionaryObserver::Parse(header + sizeof(Header::MAGIC_V_1_0) + cbLenSize, cbDictSize, 
	                                                  this)))
	{
		return status;
	}

	cbTotalSize = sizeof(Header::MAGIC_V_1_0) + cbLenSize + cbDictSize;

	return Status();
}

Status Header::Write(String *psHeader, Size cbMinExtraPadding/* = 0*/)
{
	Size   cbPrefixSize;
	Size   cbLenIndex;

	psHeader->clear();
	switch (nVersion)
	{
		case Version_1_0: 
		{
			psHeader->append((const Char *)Header::MAGIC_V_1_0, sizeof(Header::MAGIC_V_1_0)); 
		}
		break;
		case Version_2_0: 
		{
			psHeader->append((const Char *)Header::MAGIC_V_2_0, sizeof(Header::MAGIC_V_2_0)); 
		}
		break;
		case Version_3_0: 
		{
			psHeader->append((const Char *)Header::MAGIC_V_3_0, sizeof(Header::MAGIC_V_3_0)); 
		}
		break;
		default: return Status_InvalidArg;
	}
	cbLenIndex = psHeader->size();
	if (Version_1_0 == nVersion)
	{
		UInt16   cbLen = 0;

		psHeader->append((const Char *)&cbLen, sizeof(cbLen));
	}
	else
	{
		UInt32   cbLen = 0;

		psHeader->append((const Char *)&cbLen, sizeof(cbLen));
	}

	cbPrefixSize = psHeader->size();

	Bool bSameColumn = True;

	if (1 == vectorColumns.size())
	{
		bSameColumn = vectorColumns[0].sName.empty();
	}
	else
	{
		for (Size i = 1; i < vectorColumns.size(); i++)
		{
			if (!vectorColumns[i].sName.empty())
			{
				bSameColumn = False;

				break;
			}
			if (vectorColumns[i].nByteOrder != vectorColumns[0].nByteOrder)
			{
				bSameColumn = False;

				break;
			}
			if (vectorColumns[i].nType != vectorColumns[0].nType)
			{
				bSameColumn = False;

				break;
			}
		}
	}

	if (bSameColumn)
	{
		(*psHeader) += "{'descr': '";
		if (Type_Int8 == vectorColumns[0].nType || Type_UInt8 == vectorColumns[0].nType)
		{
			(*psHeader) += '|';
		}
		else
		{
			switch (vectorColumns[0].nByteOrder)
			{
				case ByteOrder_LittleEndian: (*psHeader) += "<"; break;
				case ByteOrder_BigEndian:    (*psHeader) += ">"; break;
				default:                     return Status_InvalidArg;
			}
		}
		switch (vectorColumns[0].nType)
		{
			case Type_Int8:   (*psHeader) += "i1"; break;
			case Type_UInt8:  (*psHeader) += "u1"; break;
			case Type_Int16:  (*psHeader) += "i2"; break;
			case Type_UInt16: (*psHeader) += "u2"; break;
			case Type_Int32:  (*psHeader) += "i4"; break;
			case Type_UInt32: (*psHeader) += "u4"; break;
			case Type_Int64:  (*psHeader) += "i8"; break;
			case Type_UInt64: (*psHeader) += "u8"; break;
			case Type_Float:  (*psHeader) += "f4"; break;
			case Type_Double: (*psHeader) += "f8"; break;
			default:          return Status_InvalidArg;
		}
		(*psHeader) += "'";
	}
	else
	{
		(*psHeader) += "{'descr': [";
		for (Size i = 0; i < vectorColumns.size(); i++)
		{
			Print(psHeader, "('{1}', '", vectorColumns[i].sName.c_str());
			if (Type_Int8 == vectorColumns[i].nType || Type_UInt8 == vectorColumns[i].nType)
			{
				(*psHeader) += '|';
			}
			else
			{
				switch (vectorColumns[i].nByteOrder)
				{
					case ByteOrder_LittleEndian: (*psHeader) += "<"; break;
					case ByteOrder_BigEndian:    (*psHeader) += ">"; break;
					default:                     return Status_InvalidArg;
				}
			}
			switch (vectorColumns[i].nType)
			{
				case Type_Int8:   (*psHeader) += "i1"; break;
				case Type_UInt8:  (*psHeader) += "u1"; break;
				case Type_Int16:  (*psHeader) += "i2"; break;
				case Type_UInt16: (*psHeader) += "u2"; break;
				case Type_Int32:  (*psHeader) += "i4"; break;
				case Type_UInt32: (*psHeader) += "u4"; break;
				case Type_Int64:  (*psHeader) += "i8"; break;
				case Type_UInt64: (*psHeader) += "u8"; break;
				case Type_Float:  (*psHeader) += "f4"; break;
				case Type_Double: (*psHeader) += "f8"; break;
				default:                     return Status_InvalidArg;
			}
			(*psHeader) += "')";
			if (i + 1 < vectorColumns.size())
			{
				(*psHeader) += ", ";
			}
		}
		(*psHeader) += "]";
	}
	(*psHeader) += ", 'fortran_order': ";
	switch (nFormat)
	{
		case Format_C:       *psHeader += "False"; break;
		case Format_Fortran: *psHeader += "True"; break;
		default:             return Status_InvalidArg;
	}
	Print(psHeader, ", 'shape': ({1}, {2}, ), }", cRows, vectorColumns.size());

	cbUsedSize = psHeader->size();

	while (0 < cbMinExtraPadding)
	{
		(*psHeader) += ' ';
		cbMinExtraPadding--;
	}

	while (63!= (psHeader->size() % 64))
	{
		(*psHeader) += ' ';
	}
	(*psHeader) += '\n';

	if (Version_1_0 == nVersion)
	{
		UInt16   *pcbLen = (UInt16 *)(psHeader->c_str() + cbLenIndex);

		*pcbLen = (UInt16)(psHeader->size() - cbPrefixSize);
	}
	else
	{
		UInt32   *pcbLen = (UInt32 *)(psHeader->c_str() + cbLenIndex);

		*pcbLen = (UInt32)(psHeader->size() - cbPrefixSize);
	}

	cbTotalSize = psHeader->size();

	return Status();
}

Status Header::ComputeRowSize()
{
	Size   cbTypeSize;

	if (vectorColumns.empty())
	{
		return Status_NotInitialized;
	}
	cbRowSize = 0;
	for (Size i = 0; i < vectorColumns.size(); i++)
	{
		if (0 == (cbTypeSize = TypeSize::Get(vectorColumns[i].nType)))
		{
			return Status(Status_InvalidArg, "Invalid column type for column index {1}", i);
		}
		cbRowSize += cbTypeSize;
	}

	return Status();
}

}//namespace NPY

}//namespace Data

}//namespace CX
