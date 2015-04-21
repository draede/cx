/* 
 * CX - C++ framework for general purpose development
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


#include "CX/IO/IInputStream.hpp"
#include "CX/IO/IOutputStream.hpp"
#include "CX/Util/RCMemBuffer.hpp"
#include "CX/Hash/IHash.hpp"


namespace CX
{

namespace KVDB
{

class MemDBSerialize : public IObject
{
public:

	static const Size MAX_STRING_LEN  = 65536;
	static const Size MAX_BUFFER_SIZE = 16777216;

	static Status WriteData(IO::IOutputStream *pOutputStream, const void *pData, Size cbSize, Hash::IHash *pHash)
	{
		Size   cbAckSize;
		Status status;

		pHash->Update(pData, cbSize);
		if ((status = pOutputStream->Write(pData, cbSize, &cbAckSize)).IsNOK())
		{
			return status;
		}
		if (cbAckSize != cbSize)
		{
			return Status(Status_WriteFailed, "Failed to write file");
		}

		return Status();
	}

	static Status WriteCount(IO::IOutputStream *pOutputStream, Size cCount, Hash::IHash *pHash)
	{
		UInt64 cTmpCount;
		Status status;

		cTmpCount = (UInt64)cCount;
		if ((status = WriteData(pOutputStream, &cTmpCount, sizeof(cTmpCount), pHash)).IsNOK())
		{
			return status;
		}
		
		return Status();
	}

	static Status WriteString(IO::IOutputStream *pOutputStream, const String &sStr, Hash::IHash *pHash)
	{
		Status status;

		if ((status = WriteCount(pOutputStream, sStr.size(), pHash)).IsNOK())
		{
			return status;
		}
		if ((status = WriteData(pOutputStream, sStr.c_str(), sStr.size() * sizeof(Char), pHash)).IsNOK())
		{
			return status;
		}

		return Status();
	}

	static Status WriteMemBuffer(IO::IOutputStream *pOutputStream, const Util::RCMemBuffer &buf, Hash::IHash *pHash)
	{
		Status status;

		if ((status = WriteCount(pOutputStream, buf.GetSize(), pHash)).IsNOK())
		{
			return status;
		}
		if ((status = WriteData(pOutputStream, buf.GetMem(), buf.GetSize(), pHash)).IsNOK())
		{
			return status;
		}

		return Status();
	}
	

	static Status ReadData(IO::IInputStream *pInputStream, void *pData, Size cbSize, Hash::IHash *pHash)
	{
		Size   cbAckSize;
		Status status;

		if ((status = pInputStream->Read(pData, cbSize, &cbAckSize)).IsNOK())
		{
			return status;
		}
		if (cbAckSize != cbSize)
		{
			return Status(Status_WriteFailed, "Failed to write file");
		}
		pHash->Update(pData, cbSize);

		return Status();
	}

	static Status ReadCount(IO::IInputStream *pInputStream, Size &cCount, Hash::IHash *pHash)
	{
		UInt64 cTmpCount;
		Status status;

		if ((status = ReadData(pInputStream, &cTmpCount, sizeof(cTmpCount), pHash)).IsNOK())
		{
			return status;
		}
		cCount = (Size)cTmpCount;
	
		return Status();
	}

	static Status ReadString(IO::IInputStream *pInputStream, String &sStr, Hash::IHash *pHash)
	{
		Char   buffer[8192];
		Size   cReadLen;
		Size   cLen;
		Status status;

		if ((status = ReadCount(pInputStream, cLen, pHash)).IsNOK())
		{
			return status;
		}
		if (MAX_STRING_LEN < cLen)
		{
			return Status(Status_ReadFailed, "String too long (max is {1}, {2} found)", MAX_STRING_LEN, cLen);
		}
		sStr.clear();
		while (0 < cLen)
		{
			if (cLen > sizeof(buffer) / sizeof(buffer[0]))
			{
				cReadLen = sizeof(buffer) / sizeof(buffer[0]);
			}
			else
			{
				cReadLen = cLen; 
			}
			if ((status = ReadData(pInputStream, buffer, cReadLen * sizeof(Char), pHash)).IsNOK())
			{
				return status;
			}
			sStr.append(buffer, cReadLen);
			cLen -= cReadLen;
		}

		return Status();
	}

	static Status ReadMemBuffer(IO::IInputStream *pInputStream, Util::RCMemBuffer &buf, Hash::IHash *pHash)
	{
		Size   cbSize;
		Status status;

		if ((status = ReadCount(pInputStream, cbSize, pHash)).IsNOK())
		{
			return status;
		}
		if (MAX_BUFFER_SIZE < cbSize)
		{
			return Status(Status_ReadFailed, "Key or value too long (max is {1}, {2} found)", MAX_BUFFER_SIZE, cbSize);
		}

		Util::RCMemBuffer tmp(cbSize);

		if ((status = ReadData(pInputStream, tmp.GetMem(), cbSize, pHash)).IsNOK())
		{
			return status;
		}

		buf = tmp;

		return Status();
	}

};

}//namespace KVDB

}//namespace CX

