/* 
 * CX - C++ framework for general purpose development
 *
 * https://github.com/draede/cx
 * 
 * Copyright (C) 2014 - 2017 draede - draede [at] outlook [dot] com
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
#include "CX/IO/IInputStream.hpp"
#include "CX/IO/IOutputStream.hpp"
#include "CX/IO/FileInputStream.hpp"
#include "CX/IO/FileOutputStream.hpp"
#include "CX/IO/MemInputStream.hpp"
#include "CX/IO/MemOutputStream.hpp"
#include "CX/SimpleBuffers/BSONReader.hpp"
#include "CX/SimpleBuffers/BSONWriter.hpp"
#include "CX/SimpleBuffers/JSONReader.hpp"
#include "CX/SimpleBuffers/JSONWriter.hpp"
#include "CX/Sys/ByteOrder.hpp"


namespace CX
{

namespace SimpleBuffers
{

class Helper
{
public:

	enum Type
	{
		Type_JSON,
		Type_BSON,
	};

	template <typename SIMPLEBUFFER>
	static Status LoadJSON(SIMPLEBUFFER *pSB, IO::IInputStream *pInputStream)
	{
		SimpleBuffers::JSONReader reader;
		Status                    status;

		if (!(status = reader.Begin(pInputStream)))
		{
			return status;
		}
		if (!(status = pSB->Read(&reader)))
		{
			return status;
		}
		if (!(status = reader.End()))
		{
			return status;
		}

		return Status();
	}

	template <typename SIMPLEBUFFER>
	static Status LoadJSON(SIMPLEBUFFER *pSB, const Char *szPath)
	{
		IO::FileInputStream is(szPath);

		if (!is.IsOK())
		{
			return Status_OpenFailed;
		}

		return LoadJSON(pSB, &is);
	}

	template <typename SIMPLEBUFFER>
	static Status LoadJSON(SIMPLEBUFFER *pSB, const WChar *wszPath)
	{
		IO::FileInputStream is(wszPath);

		if (!is.IsOK())
		{
			return Status_OpenFailed;
		}

		return LoadJSON(pSB, &is);
	}

	template <typename SIMPLEBUFFER>
	static Status LoadJSON(SIMPLEBUFFER *pSB, const void *pData, Size cbSize)
	{
		IO::MemInputStream is(pData, cbSize);

		return LoadJSON(pSB, &is);
	}

	template <typename SIMPLEBUFFER>
	static Status SaveJSON(const SIMPLEBUFFER *pSB, IO::IOutputStream *pOutputStream)
	{
		SimpleBuffers::JSONWriter writer;
		Status                    status;

		if (!(status = writer.Begin(pOutputStream)))
		{
			return status;
		}
		if (!(status = pSB->Write(&writer)))
		{
			return status;
		}
		if (!(status = writer.End()))
		{
			return status;
		}

		return Status();
	}

	template <typename SIMPLEBUFFER>
	static Status SaveJSON(const SIMPLEBUFFER *pSB, const Char *szPath)
	{
		IO::FileOutputStream os(szPath);

		if (!os.IsOK())
		{
			return Status_CreateFailed;
		}

		return SaveJSON(pSB, &os);
	}

	template <typename SIMPLEBUFFER>
	static Status SaveJSON(const SIMPLEBUFFER *pSB, const WChar *wszPath)
	{
		IO::FileOutputStream os(wszPath);

		if (!os.IsOK())
		{
			return Status_CreateFailed;
		}

		return SaveJSON(pSB, &os);
	}

	template <typename SIMPLEBUFFER>
	static Status LoadBSON(SIMPLEBUFFER *pSB, IO::IInputStream *pInputStream)
	{
		SimpleBuffers::BSONReader reader;
		Status                    status;

		if (!(status = reader.Begin(pInputStream)))
		{
			return status;
		}
		if (!(status = pSB->Read(&reader)))
		{
			return status;
		}
		if (!(status = reader.End()))
		{
			return status;
		}

		return Status();
	}

	template <typename SIMPLEBUFFER>
	static Status LoadBSON(SIMPLEBUFFER *pSB, const Char *szPath)
	{
		IO::FileInputStream is(szPath);

		if (!is.IsOK())
		{
			return Status_OpenFailed;
		}

		return LoadBSON(pSB, &is);
	}

	template <typename SIMPLEBUFFER>
	static Status LoadBSON(SIMPLEBUFFER *pSB, const WChar *wszPath)
	{
		IO::FileInputStream is(wszPath);

		if (!is.IsOK())
		{
			return Status_OpenFailed;
		}

		return LoadBSON(pSB, &is);
	}

	template <typename SIMPLEBUFFER>
	static Status LoadBSON(SIMPLEBUFFER *pSB, const void *pData, Size cbSize)
	{
		IO::MemInputStream is(pData, cbSize);

		return LoadBSON(pSB, &is);
	}

	template <typename SIMPLEBUFFER>
	static Status SaveBSON(const SIMPLEBUFFER *pSB, IO::IOutputStream *pOutputStream)
	{
		SimpleBuffers::BSONWriter writer;
		Status                    status;

		if (!(status = writer.Begin(pOutputStream)))
		{
			return status;
		}
		if (!(status = pSB->Write(&writer)))
		{
			return status;
		}
		if (!(status = writer.End()))
		{
			return status;
		}

		return Status();
	}

	template <typename SIMPLEBUFFER>
	static Status SaveBSON(const SIMPLEBUFFER *pSB, const Char *szPath)
	{
		IO::FileOutputStream os(szPath);

		if (!os.IsOK())
		{
			return Status_CreateFailed;
		}

		return SaveBSON(pSB, &os);
	}

	template <typename SIMPLEBUFFER>
	static Status SaveBSON(const SIMPLEBUFFER *pSB, const WChar *wszPath)
	{
		IO::FileOutputStream os(wszPath);

		if (!os.IsOK())
		{
			return Status_CreateFailed;
		}

		return SaveBSON(pSB, &os);
	}

	template <typename SIMPLEBUFFER>
	static Status Load(Type nType, SIMPLEBUFFER *pSB, IO::IInputStream *pInputStream)
	{
		if (Type_JSON == nType)
		{
			return LoadJSON(pSB, pInputStream);
		}
		else
		if (Type_BSON == nType)
		{
			return LoadBSON(pSB, pInputStream);
		}
		else
		{
			return Status_InvalidArg;
		}
	}

	template <typename SIMPLEBUFFER>
	static Status Load(Type nType, SIMPLEBUFFER *pSB, const Char *szPath)
	{
		if (Type_JSON == nType)
		{
			return LoadJSON(pSB, szPath);
		}
		else
		if (Type_BSON == nType)
		{
			return LoadBSON(pSB, szPath);
		}
		else
		{
			return Status_InvalidArg;
		}
	}

	template <typename SIMPLEBUFFER>
	static Status Load(Type nType, SIMPLEBUFFER *pSB, const WChar *wszPath)
	{
		if (Type_JSON == nType)
		{
			return LoadJSON(pSB, wszPath);
		}
		else
		if (Type_BSON == nType)
		{
			return LoadBSON(pSB, wszPath);
		}
		else
		{
			return Status_InvalidArg;
		}
	}

	template <typename SIMPLEBUFFER>
	static Status Load(Type nType, SIMPLEBUFFER *pSB, const void *pData, Size cbSize)
	{
		if (Type_JSON == nType)
		{
			return LoadJSON(pSB, pData, cbSize);
		}
		else
		if (Type_BSON == nType)
		{
			return LoadBSON(pSB, pData, cbSize);
		}
		else
		{
			return Status_InvalidArg;
		}
	}

	template <typename SIMPLEBUFFER>
	static Status Save(Type nType, const SIMPLEBUFFER *pSB, IO::IOutputStream *pOutputStream)
	{
		if (Type_JSON == nType)
		{
			return SaveJSON(pSB, pOutputStream);
		}
		else
		if (Type_BSON == nType)
		{
			return SaveBSON(pSB, pOutputStream);
		}
		else
		{
			return Status_InvalidArg;
		}
	}

	template <typename SIMPLEBUFFER>
	static Status Save(Type nType, const SIMPLEBUFFER *pSB, const Char *szPath)
	{
		if (Type_JSON == nType)
		{
			return SaveJSON(pSB, szPath);
		}
		else
		if (Type_BSON == nType)
		{
			return SaveBSON(pSB, szPath);
		}
		else
		{
			return Status_InvalidArg;
		}
	}

	template <typename SIMPLEBUFFER>
	static Status Save(Type nType, const SIMPLEBUFFER *pSB, const WChar *wszPath)
	{
		if (Type_JSON == nType)
		{
			return SaveJSON(pSB, wszPath);
		}
		else
		if (Type_BSON == nType)
		{
			return SaveBSON(pSB, wszPath);
		}
		else
		{
			return Status_InvalidArg;
		}
	}

	template <typename SIMPLEBUFFER>
	static Status Load(SIMPLEBUFFER *pSB, IO::IInputStream *pInputStream, Type *pnDetectedType)
	{
		Type   nType;
		Status status;

		if (!(status = DetectType(pInputStream, &nType)))
		{
			return status;
		}
		if (NULL != pnDetectedType)
		{
			*pnDetectedType = nType;
		}
		if (Type_JSON == nType)
		{
			return LoadJSON(pSB, pInputStream);
		}
		else
		if (Type_BSON == nType)
		{
			return LoadBSON(pSB, pInputStream);
		}
		else
		{
			return Status_InvalidArg;
		}
	}

	template <typename SIMPLEBUFFER>
	static Status Load(SIMPLEBUFFER *pSB, const Char *szPath, Type *pnDetectedType)
	{
		IO::FileInputStream is(szPath);

		if (!is.IsOK())
		{
			return Status_OpenFailed;
		}

		return Load(pSB, &is, pnDetectedType);
	}

	template <typename SIMPLEBUFFER>
	static Status Load(SIMPLEBUFFER *pSB, const WChar *wszPath, Type *pnDetectedType)
	{
		IO::FileInputStream is(wszPath);

		if (!is.IsOK())
		{
			return Status_OpenFailed;
		}

		return Load(pSB, &is, pnDetectedType);
	}

	template <typename SIMPLEBUFFER>
	static Status Load(SIMPLEBUFFER *pSB, const void *pData, Size cbSize, Type *pnDetectedType)
	{
		IO::MemInputStream is(pData, cbSize);

		return Load(pSB, &is, pnDetectedType);
	}

	static Status DetectType(IO::IInputStream *pInputStream, Type *pnDetectedType)
	{
		Int32    nData;
		UInt64   cbPos;
		UInt64   cbSize;
		Size     cbAckSize;
		Status   status;

		if (!(status = pInputStream->GetSize(&cbSize)))
		{
			return status;
		}
		if (TYPE_INT32_MAX < cbSize)
		{
			return Status_TooBig;
		}
		if (!(status = pInputStream->GetPos(&cbPos)))
		{
			return status;
		}
		if (!(status = pInputStream->Read(&nData, sizeof(nData), &cbAckSize)))
		{
			return status;
		}
		if (sizeof(nData) != cbAckSize)
		{
			return Status_ReadFailed;
		}
		if (!(status = pInputStream->SetPos(cbPos)))
		{
			return status;
		}
		nData = (Int32)Sys::ByteOrder::LE2H((UInt32)nData);
		if (nData == (Int32)cbSize)
		{
			*pnDetectedType = Type_BSON;
		}
		else
		{
			*pnDetectedType = Type_JSON;
		}

		return Status();
	}

	class IOutput
	{
	public:

		virtual ~IOutput() { }

		virtual Status Add(const void *pData, Size cbSize) = 0;

	};

	static Status BSON2JSON(const void *pData, Size cbSize, IOutput *pOutput);

	static Status JSON2BSON(const void *pData, Size cbSize, IOutput *pOutput);

private:

	Helper();

	~Helper();

	static Status AddIndent(Size cIndent, IOutput *pOutput);

};

}//namespace SimpleBuffers

}//namespace CX
