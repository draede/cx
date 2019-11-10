/* 
 * CX - C++ framework for general purpose development
 *
 * https://github.com/draede/cx
 * 
 * Copyright (C) 2014 - 2019 draede - draede [at] outlook [dot] com
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
#include "CX/Data/NPY/Converter.hpp"
#include "CX/IO/FileOutputStream.hpp"
#include "CX/IO/BufferedOutputStream.hpp"
#include "CX/Data/CSV/SAXParser.hpp"
#include "CX/Print.hpp"
#include <sstream>


namespace CX
{

namespace Data
{

namespace NPY
{

Converter::Converter()
{
}

Converter::~Converter()
{
}

Status Converter::CSV2NPY(const Char *szCSVPath, const Char *szNPYPath, const Column *columns, Size cColumns, 
                          Bool bSkipFirstRow/* = True*/)
{
	Writer   writer;
	Status   status;

	if (!(status = writer.Create(szNPYPath, columns, cColumns)))
	{
		return status;
	}

	status = CSV2NPY(szCSVPath, &writer, bSkipFirstRow);

	if (!status)
	{
		::_unlink(szCSVPath);
	}

	return status;
}

Status Converter::CSV2NPY(const WChar *wszCSVPath, const WChar *wszNPYPath, const Column *columns, Size cColumns, 
                          Bool bSkipFirstRow/* = True*/)
{
	Writer   writer;
	Status   status;

	if (!(status = writer.Create(wszNPYPath, columns, cColumns)))
	{
		return status;
	}

	status = CSV2NPY(wszCSVPath, &writer, bSkipFirstRow);

	if (!status)
	{
		::_wunlink(wszCSVPath);
	}

	return status;
}

Status Converter::CSV2NPY(const Char *szCSVPath, Writer *pWriter, Bool bSkipFirstRow/* = True*/)
{
	Data::CSV::SAXParser   parser;
	SAXCSVObserver         observer;
	Status                 status;

	observer.m_pWriter       = pWriter;
	observer.m_bSkipFirstRow = bSkipFirstRow;
	if (!(status = parser.AddObserver(&observer)))
	{
		return status;
	}
	if (!(status = parser.BeginParse()))
	{
		return status;
	}
	if (!(status = parser.ParseStream(szCSVPath)))
	{
		return status;
	}
	if (!(status = parser.EndParse()))
	{
		return status;
	}

	return Status();
}

Status Converter::CSV2NPY(const WChar *wszCSVPath, Writer *pWriter, Bool bSkipFirstRow/* = True*/)
{
	Data::CSV::SAXParser   parser;
	SAXCSVObserver         observer;
	Status                 status;

	observer.m_pWriter       = pWriter;
	observer.m_bSkipFirstRow = bSkipFirstRow;
	if (!(status = parser.AddObserver(&observer)))
	{
		return status;
	}
	if (!(status = parser.BeginParse()))
	{
		return status;
	}
	if (!(status = parser.ParseStream(wszCSVPath)))
	{
		return status;
	}
	if (!(status = parser.EndParse()))
	{
		return status;
	}

	return Status();

}

void Converter::SAXCSVObserver::OnBeginParse()
{
}

void Converter::SAXCSVObserver::OnEndParse()
{
}

template <typename T>
Bool Convert(const String &sVal, Writer *pWriter)
{
	std::istringstream   iss;
	T                    val;
	Status               status;

	iss.str(sVal.c_str());
	iss >> val;
	if (iss.fail() || !iss.eof())
	{
		return False;
	}
	if (!(status = pWriter->Write(&val, sizeof(val))))
	{
		return False;
	}

	return True;
}

template <>
Bool Convert<Int8>(const String &sVal, Writer *pWriter)
{
	std::istringstream   iss;
	Int8                 val;
	Int16                val2;
	Status               status;

	iss.str(sVal.c_str());
	iss >> val2;
	if (iss.fail() || !iss.eof())
	{
		return False;
	}
	val = (Int8)val2;
	if (!(status = pWriter->Write(&val, sizeof(val))))
	{
		return False;
	}

	return True;
}

template <>
Bool Convert<UInt8>(const String &sVal, Writer *pWriter)
{
	std::istringstream   iss;
	UInt8                val;
	UInt16               val2;
	Status               status;

	iss.str(sVal.c_str());
	iss >> val2;
	if (iss.fail() || !iss.eof())
	{
		return False;
	}
	val = (UInt8)val2;
	if (!(status = pWriter->Write(&val, sizeof(val))))
	{
		return False;
	}

	return True;
}

Bool Converter::SAXCSVObserver::OnRow(Size cRowIndex, const ColumnsVector &vectorColumns)
{
	Size           cColumns = m_pWriter->GetColumnsCount();
	const Column   *columns = m_pWriter->GetColumns();
	Status         status;

	if (vectorColumns.size() != cColumns)
	{
		return False;
	}


	if (0 == cRowIndex && m_bSkipFirstRow)
	{
		return True;
	}

	for (Size i = 0; i < vectorColumns.size(); i++)
	{
		switch (columns[i].nType)
		{
			case Type_Int8:   if (!Convert<Int8>(vectorColumns[i], m_pWriter)) return False; break;
			case Type_UInt8:  if (!Convert<UInt8>(vectorColumns[i], m_pWriter)) return False; break;
			case Type_Int16:  if (!Convert<Int16>(vectorColumns[i], m_pWriter)) return False; break;
			case Type_UInt16: if (!Convert<UInt16>(vectorColumns[i], m_pWriter)) return False; break;
			case Type_Int32:  if (!Convert<Int32>(vectorColumns[i], m_pWriter)) return False; break;
			case Type_UInt32: if (!Convert<UInt32>(vectorColumns[i], m_pWriter)) return False; break;
			case Type_Int64:  if (!Convert<Int64>(vectorColumns[i], m_pWriter)) return False; break;
			case Type_UInt64: if (!Convert<UInt64>(vectorColumns[i], m_pWriter)) return False; break;
			case Type_Float:  if (!Convert<Float>(vectorColumns[i], m_pWriter)) return False; break;
			case Type_Double: if (!Convert<Double>(vectorColumns[i], m_pWriter)) return False; break;
			default:          return False;
		}
	}

	return True;
}

Status Converter::NPY2CSV(const Char *szNPYPath, const Char *szCSVPath, 
                          Size cNPYInputBufferRowsCount/* = NPY_INPUT_BUFFER_ROWS_COUNT*/, 
                          Size cbCSVBufferOutputSize/* = CSV_OUTPUT_BUFFER_SIZE*/)
{
	Reader   reader;
	Status   status;

	if (!(status = reader.Open(szNPYPath)))
	{
		return status;
	}

	{
		IO::FileOutputStream   fos(szCSVPath);

		if (fos.IsOK())
		{
			IO::BufferedOutputStream   bos(&fos, False, cbCSVBufferOutputSize);

			status = NPY2CSV(&reader, &bos, cNPYInputBufferRowsCount, cbCSVBufferOutputSize);
		}
		else
		{
			status = Status_CreateFailed;
		}
	}

	if (!status)
	{
		::_unlink(szCSVPath);
	}

	return status;
}

Status Converter::NPY2CSV(const WChar *wszNPYPath, const WChar *wszCSVPath, 
                          Size cNPYInputBufferRowsCount/* = NPY_INPUT_BUFFER_ROWS_COUNT*/, 
                          Size cbCSVBufferOutputSize/* = CSV_OUTPUT_BUFFER_SIZE*/)
{
	Reader   reader;
	Status   status;

	if (!(status = reader.Open(wszNPYPath)))
	{
		return status;
	}

	{
		IO::FileOutputStream   fos(wszCSVPath);

		if (fos.IsOK())
		{
			IO::BufferedOutputStream   bos(&fos, False, cbCSVBufferOutputSize);

			status = NPY2CSV(&reader, &bos, cNPYInputBufferRowsCount, cbCSVBufferOutputSize);
		}
		else
		{
			status = Status_CreateFailed;
		}
	}

	if (!status)
	{
		::_wunlink(wszCSVPath);
	}

	return status;
}

Status Converter::NPY2CSV(Reader *pReader, IO::IOutputStream *pOutputStream, 
                          Size cNPYInputBufferRowsCount/* = NPY_INPUT_BUFFER_ROWS_COUNT*/, 
                          Size cbCSVBufferOutputSize/* = CSV_OUTPUT_BUFFER_SIZE*/)
{
	IO::BufferedOutputStream   bos(pOutputStream, False, cbCSVBufferOutputSize);
	const Byte                 *data;
	Size                       cRowIndex = 0;
	Size                       cRows     = pReader->GetRowsCount();
	Size                       cColumns  = pReader->GetColumnsCount();
	const Column               *columns = pReader->GetColumns();
	Size                       cChunkRows;
	Size                       cAckRows;

	for (Size cColIdx = 0; cColIdx < cColumns; cColIdx++)
	{
		if (cColIdx + 1 < cColumns)
		{
			Print((IO::IOutputStream *)&bos, "{1},", columns[cColIdx].sName);
		}
		else
		{
			Print((IO::IOutputStream *)&bos, "{1}", columns[cColIdx].sName);
		}
	}
	Print((IO::IOutputStream *)&bos, "\n");

	while (cRowIndex < cRows)
	{
		if (cRows - cRowIndex > cNPYInputBufferRowsCount)
		{
			cChunkRows = cNPYInputBufferRowsCount;
		}
		else
		{
			cChunkRows = cRows - cRowIndex;
		}

		data = (const Byte *)pReader->GetRows(cRowIndex, cChunkRows, &cAckRows);
		if (cAckRows != cChunkRows)
		{
			return Status_ReadFailed;
		}

		for (Size cRowIdx = 0; cRowIdx < cChunkRows; cRowIdx++)
		{
			for (Size cColIdx = 0; cColIdx < cColumns; cColIdx++)
			{
				switch (columns[cColIdx].nType)
				{
					case Type_Int8:   Print((IO::IOutputStream *)&bos, "{1}", *(Int8 *)data); data += sizeof(Int8); break;
					case Type_UInt8:  Print((IO::IOutputStream *)&bos, "{1}", *(UInt8 *)data); data += sizeof(UInt8); break;
					case Type_Int16:  Print((IO::IOutputStream *)&bos, "{1}", *(Int16 *)data); data += sizeof(Int16); break;
					case Type_UInt16: Print((IO::IOutputStream *)&bos, "{1}", *(UInt16 *)data); data += sizeof(UInt16); break;
					case Type_Int32:  Print((IO::IOutputStream *)&bos, "{1}", *(Int32 *)data); data += sizeof(Int32); break;
					case Type_UInt32: Print((IO::IOutputStream *)&bos, "{1}", *(UInt32 *)data); data += sizeof(UInt32); break;
					case Type_Int64:  Print((IO::IOutputStream *)&bos, "{1}", *(Int64 *)data); data += sizeof(Int64); break;
					case Type_UInt64: Print((IO::IOutputStream *)&bos, "{1}", *(UInt64 *)data); data += sizeof(UInt64); break;
					case Type_Float:  Print((IO::IOutputStream *)&bos, "{1:.6}", *(Float *)data); data += sizeof(Float); break;
					case Type_Double: Print((IO::IOutputStream *)&bos, "{1:.12}", *(Double *)data); data += sizeof(Double); break;
					default:          return Status_InvalidArg;
				}
				if (cColIdx + 1 < cColumns)
				{
					Print((IO::IOutputStream *)&bos, ",");
				}
			}
			Print((IO::IOutputStream *)&bos, "\n");
		}

		cRowIndex += cChunkRows;
	}

	return Status();
}

}//namespace NPY

}//namespace Data

}//namespace CX
