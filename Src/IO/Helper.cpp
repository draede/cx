/* 
 * CX - C++ framework for general purpose development
 *
 * https://github.com/draede/cx
 * 
 * Copyright (C) 2014 - 2018 draede, draede [at] outlook [dot] com
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

Status Helper::CopyStream(IInputStream *pInputStream, IOutputStream *pOutputStream, UInt64 *pcbSize/* = NULL*/)
{
	Byte   buffer[COPY_STREAM_BUFFER];
	Size   cbAckSize;
	Size   cbAckSize2;
	Status status;

	if (NULL != pcbSize)
	{
		*pcbSize = 0;
	}
	while (!pInputStream->IsEOF())
	{
		if ((status = pInputStream->Read(buffer, sizeof(buffer), &cbAckSize)).IsNOK())
		{
			if (Status_EOF != (StatusCode)status)
			{
				return status;
			}
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
			if (NULL != pcbSize)
			{
				(*pcbSize) += cbAckSize;
			}
		}
	}

	return Status();
}

Status Helper::LoadStream(IInputStream *pInputStream, Vector<Byte>::Type &vectorData)
{
	UInt64 cbSize;
	Size   cbReqSize;
	Size   cbAckSize;
	Status status;

	if ((status = pInputStream->GetSize(&cbSize)).IsNOK())
	{
		return status;
	}
	if (0 < cbSize)
	{
		cbReqSize = (Size)cbSize;
		try
		{
			vectorData.resize(cbReqSize);
		}
		catch (...)
		{
			return Status(Status_MemAllocFailed);
		}
		if ((status = pInputStream->Read(&vectorData[0], cbReqSize, &cbAckSize)).IsNOK())
		{
			return status;
		}
		if (cbAckSize != cbReqSize)
		{
			return Status(Status_ReadFailed);
		}
	}
	else
	{
		vectorData.resize(0);
	}

	return Status();
}

}//namespace IO

}//namespace CX

