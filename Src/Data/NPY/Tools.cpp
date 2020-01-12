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
#include "CX/Data/NPY/Tools.hpp"
#include "CX/Data/NPY/AppendWriter.hpp"
#include "CX/Data/NPY/Reader.hpp"
#include "CX/Data/NPY/Reader.hpp"
#include "CX/Str/UTF8.hpp"
#include "CX/Util/MemPool.hpp"


namespace CX
{

namespace Data
{

namespace NPY
{

Tools::Tools()
{
}

Tools::~Tools()
{
}

Status Tools::Merge(const Char **paths, Size cPaths)
{
	WPathsVector   vectorWPaths;
	Status         status;

	for (Size i = 0; i < cPaths; i++)
	{
		WString   wsPath;

		if (!(status = Str::UTF8::ToWChar(paths[i], &wsPath)))
		{
			return status;
		}
		vectorWPaths.push_back(wsPath);
	}

	return Merge(vectorWPaths);
}

Status Tools::Merge(const WChar **paths, Size cPaths)
{
	WPathsVector   vectorWPaths;
	Status         status;

	for (Size i = 0; i < cPaths; i++)
	{
		vectorWPaths.push_back(paths[i]);
	}

	return Merge(vectorWPaths);
}

Status Tools::Merge(const PathsVector &vectorPaths)
{
	WPathsVector   vectorWPaths;
	Status         status;

	for (Size i = 0; i < vectorPaths.size(); i++)
	{
		WString   wsPath;

		if (!(status = Str::UTF8::ToWChar(vectorPaths[i].c_str(), &wsPath)))
		{
			return status;
		}
		vectorWPaths.push_back(wsPath);
	}

	return Merge(vectorWPaths);
}

Status Tools::Merge(const WPathsVector &vectorPaths)
{
	if (2 > vectorPaths.size())
	{
		return Status_InvalidArg;
	}

	AppendWriter   writer;
	Reader         reader;
	const Column   *columns;
	Size           cColumns;
	const Column   *columnsTmp;
	Size           cColumnsTmp;
	Size           cMaxRowsPerChunk;
	Size           cChunkRows;
	Size           cCurrentRows;
	Size           cTotalRows;
	Size           cAckRows;
	const void     *rows;
	Status         status;

	if (!(status = writer.Open(vectorPaths[0].c_str())))
	{
		return status;
	}
	columns  = writer.GetColumns();
	cColumns = writer.GetColumnsCount();
	for (Size i = 1; i < vectorPaths.size(); i++)
	{
		if (!(status = reader.Open(vectorPaths[i].c_str())))
		{
			return status;
		}
		columnsTmp  = reader.GetColumns();
		cColumnsTmp = reader.GetColumnsCount();
		if (cColumnsTmp != cColumns)
		{
			return Status_InvalidArg;
		}
		for (Size j = 0; j < cColumns; j++)
		{
			if (columns[j].nByteOrder != columnsTmp[j].nByteOrder)
			{
				return Status_InvalidArg;
			}
			if (columns[j].nType != columnsTmp[j].nType)
			{
				return Status_InvalidArg;
			}
			if (0 != cx_strcmp(columns[j].sName.c_str(), columnsTmp[j].sName.c_str()))
			{
				return Status_InvalidArg;
			}
		}

		cMaxRowsPerChunk = MAX_READ_BUFFER_SIZE / reader.GetRowSize();
		cTotalRows       = reader.GetRowsCount();
		cCurrentRows     = 0;

		while (cCurrentRows < cTotalRows)
		{
			if (cMaxRowsPerChunk > cTotalRows - cCurrentRows)
			{
				cChunkRows = cTotalRows - cCurrentRows;
			}
			else
			{
				cChunkRows = cMaxRowsPerChunk;
			}

			rows = reader.GetRows(cCurrentRows, cChunkRows, &cAckRows);

			if (!(status = writer.Write(rows, cAckRows * reader.GetRowSize())))
			{
				return status;
			}

			cCurrentRows += cAckRows;
		}

		reader.Close();
	}

	return Status();
}

Status Tools::Truncate(const Char *szPath, Size cRows)
{
	HANDLE   hFile;
	Status   status;

	if (INVALID_HANDLE_VALUE == (hFile = CreateFileA(szPath, GENERIC_WRITE | GENERIC_READ, FILE_SHARE_READ, NULL, 
	                                                 OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL)))
	{
		return Status(Status_CreateFailed, "CreateFileA failed with error {1}", ::GetLastError());
	}
	status = Truncate(hFile, cRows);
	CloseHandle(hFile);

	return status;
}

Status Tools::Truncate(const WChar *wszPath, Size cRows)
{
	HANDLE   hFile;
	Status   status;

	if (INVALID_HANDLE_VALUE == (hFile = CreateFileW(wszPath, GENERIC_WRITE | GENERIC_READ, FILE_SHARE_READ, NULL, 
	                                                 OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL)))
	{
		return Status(Status_CreateFailed, "CreateFileW failed with error {1}", ::GetLastError());
	}
	status = Truncate(hFile, cRows);
	CloseHandle(hFile);

	return status;
}

Status Tools::Truncate(void *pFile, Size cRows)
{
	LARGE_INTEGER      liPos = { 0 };
	Header             header;
	String             sHeader;
	Util::DynMemPool   mem;
	DWORD              dwAckSize;
	Size               cbInitiaHeaderSize;
	Status             status;

	if (!(status = mem.SetSize(MAX_HEADER_SIZE)))
	{
		return status;
	}
	if (!ReadFile(pFile, mem.GetMem(), (DWORD)mem.GetSize(), &dwAckSize, NULL))
	{
		return Status_ReadFailed;
	}
	if (!(status = header.Read(mem.GetMem(), (Size)dwAckSize)))
	{
		return status;
	}
	if (cRows > header.cRows)
	{
		return Status();
	}
	if (!::SetFilePointerEx(pFile, liPos, NULL, FILE_BEGIN))
	{
		return Status_SetPos;
	}
	cbInitiaHeaderSize = header.cbTotalSize;
	header.cRows       = cRows;
	if (!(status = header.Write(&sHeader)))
	{
		return status;
	}
	if (header.cbTotalSize > cbInitiaHeaderSize)
	{
		return Status(Status_OperationFailed, "Final header is larger than the initial one!");
	}
	if (sHeader.size() < cbInitiaHeaderSize)
	{
		if (!(status = header.Write(&sHeader, cbInitiaHeaderSize - sHeader.size())))
		{
			return status;
		}
	}
	if (!::WriteFile(pFile, sHeader.c_str(), (DWORD)sHeader.size(), &dwAckSize, NULL))
	{
		return Status(Status_WriteFailed, "WriteFile failed with error {1}", ::GetLastError());
	}
	if (dwAckSize != (DWORD)sHeader.size())
	{
		return Status(Status_WriteFailed, "WriteFile failed");
	}
	liPos.QuadPart = header.cbTotalSize + header.cbRowSize * cRows;
	if (!::SetFilePointerEx(pFile, liPos, NULL, FILE_BEGIN))
	{
		return Status_SetPos;
	}
	if (!SetEndOfFile(pFile))
	{
		return Status_OperationFailed;
	}

	return Status();
}

}//namespace NPY

}//namespace Data

}//namespace CX
