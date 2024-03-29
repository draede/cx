/* 
 * CX - C++ framework for general purpose development
 *
 * https://github.com/draede/cx
 * 
 * Copyright (C) 2014 - 2022 draede
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
#include "CX/IO/IOutputStream.hpp"
#include "CX/BLOB.hpp"
#include "CX/C/stdarg.h"
#include "CX/SimpleBuffers/Custom.hpp"
#include "CX/APIDefs.hpp"


namespace CX
{

namespace SimpleBuffers
{

class CX_API IWriter
{
public:

	virtual ~IWriter() { }

	virtual Status Begin(IO::IOutputStream *pOutputStream) = 0;

	virtual Status End() = 0;

	virtual Status WriteBool(Bool v, const Char *szName = NULL) = 0;

	virtual Status WriteInt8(Int8 v, const Char *szName = NULL) = 0;

	virtual Status WriteUInt8(UInt8 v, const Char *szName = NULL) = 0;

	virtual Status WriteInt16(Int16 v, const Char *szName = NULL) = 0;

	virtual Status WriteUInt16(UInt16 v, const Char *szName = NULL) = 0;

	virtual Status WriteInt32(Int32 v, const Char *szName = NULL) = 0;

	virtual Status WriteUInt32(UInt32 v, const Char *szName = NULL) = 0;

	virtual Status WriteInt64(Int64 v, const Char *szName = NULL) = 0;

	virtual Status WriteUInt64(UInt64 v, const Char *szName = NULL) = 0;

	virtual Status WriteFloat(Float v, const Char *szName = NULL) = 0;

	virtual Status WriteDouble(Double v, const Char *szName = NULL) = 0;

	virtual Status WriteString(const String &v, const Char *szName = NULL) = 0;

	virtual Status WriteBLOB(const void *pData, Size cbSize, const Char *szName = NULL) = 0;

	virtual Status WriteBLOB(const BLOB &blob, const Char *szName = NULL)
	{
		if (!blob.empty())
		{
			return WriteBLOB(&blob[0], blob.size(), szName);
		}
		else
		{
			return WriteBLOB("", 0, szName);
		}
	}

	virtual Status WriteCustom(ICustom::Type nType, const void *pData, const Char *szName = NULL) = 0;

	virtual Status BeginObject(const Char *szName = NULL) = 0;

	virtual Status EndObject() = 0;

	virtual Status BeginArray(const Char *szName = NULL) = 0;

	virtual Status EndArray() = 0;

	template <typename T>
	Status WriteEnum(const T &v, const Char *szName, Size cValsCount, ...)
	{
		va_list    vl;
		bool       bOK = false;
		Status     status;

		va_start(vl, cValsCount);
		for (Size i = 0; i < cValsCount; i++)
		{
			T          val    = va_arg(vl, T);
			const Char *szStr = va_arg(vl, const Char *);

			if (!bOK && val == v)
			{
				bOK = true;
				if (!(status = WriteString(szStr, szName)))
				{
					return status;
				}
			}
		}
		va_end(vl);
		if (!bOK)
		{
			return Status(Status_InvalidArg);
		}

		return Status();
	}

};

}//namespace SimpleBuffers

}//namespace CX

