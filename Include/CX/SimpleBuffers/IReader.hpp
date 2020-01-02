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
#include "CX/Status.hpp"
#include "CX/String.hpp"
#include "CX/IO/IInputStream.hpp"
#include "CX/BLOB.hpp"
#include "CX/Map.hpp"
#include "CX/C/string.h"
#include "CX/C/stdarg.h"
#include "CX/Mem.hpp"
#include "CX/SimpleBuffers/Custom.hpp"
#include "CX/APIDefs.hpp"


namespace CX
{

namespace SimpleBuffers
{

class CX_API IReader
{
public:

	virtual ~IReader() { }

	virtual Status Begin(IO::IInputStream *pInputStream) = 0;

	virtual Status End() = 0;

	virtual Status ReadBool(Bool &v, const Char *szName = NULL) = 0;

	virtual Status ReadInt8(Int8 &v, const Char *szName = NULL) = 0;

	virtual Status ReadUInt8(UInt8 &v, const Char *szName = NULL) = 0;

	virtual Status ReadInt16(Int16 &v, const Char *szName = NULL) = 0;

	virtual Status ReadUInt16(UInt16 &v, const Char *szName = NULL) = 0;

	virtual Status ReadInt32(Int32 &v, const Char *szName = NULL) = 0;

	virtual Status ReadUInt32(UInt32 &v, const Char *szName = NULL) = 0;

	virtual Status ReadInt64(Int64 &v, const Char *szName = NULL) = 0;

	virtual Status ReadUInt64(UInt64 &v, const Char *szName = NULL) = 0;

	virtual Status ReadFloat(Float &v, const Char *szName = NULL) = 0;

	virtual Status ReadDouble(Double &v, const Char *szName = NULL) = 0;

	virtual Status ReadString(String &v, const Char *szName = NULL) = 0;

	virtual Status ReadBLOB(BLOB &v, const Char *szName = NULL) = 0;
	
	virtual Status ReadCustom(ICustom *pCustom, const Char *szName = NULL) = 0;

	virtual Status ReadCustom(ICustom::Type nType, void *pData, ICustom::Allocator *pAllocator, 
	                          const Char *szName = NULL) = 0;

	virtual Status BeginObject(const Char *szName = NULL) = 0;

	virtual Status EndObject() = 0;

	virtual Status BeginArray(const Char *szName = NULL) = 0;

	virtual Status EndArray() = 0;

	template <typename T>
	Status ReadEnum(T &v, const Char *szName, Size cValsCount, ...)
	{
		va_list    vl;
		String     sStr;
		bool       bOK = false;
		Status     status;

		if (!(status = ReadString(sStr, szName)))
		{
			return status;
		}
		va_start(vl, cValsCount);
		for (Size i = 0; i < cValsCount; i++)
		{
			T          val    = va_arg(vl, T);
			const Char *szStr = va_arg(vl, const Char *);

	#pragma warning(push)
	#pragma warning(disable: 4996)
			if (!bOK && 0 == cx_stricmp(szStr, sStr.c_str()))
	#pragma warning(pop)
			{
				bOK = true;
				v   = val;
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

