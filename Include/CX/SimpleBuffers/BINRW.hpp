/* 
 * CX - C++ framework for general purpose development
 *
 * https://github.com/draede/cx
 * 
 * Copyright (C) 2014-2016 draede, draede [at] outlook [dot] com
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
#include "CX/APIDefs.hpp"


namespace CX
{

namespace SimpleBuffers
{

struct CX_API BINRW
{
	typedef UInt32   MagicType;
	typedef Byte     RecordType;

	static const Size         MAX_STRING_LEN      = 1048576;
	static const Size         MAX_BLOB_SIZE       = 16777216;

	static const MagicType    BEGIN_MAGIC         = 0x42535843;
	static const MagicType    END_MAGIC           = 0x43585342;

	static const RecordType   RECORD_BEGIN_OBJECT = 1;
	static const RecordType   RECORD_END_OBJECT   = 2;
	static const RecordType   RECORD_BEGIN_ARRAY  = 3;
	static const RecordType   RECORD_END_ARRAY    = 4;
	static const RecordType   RECORD_BOOL         = 5;
	static const RecordType   RECORD_INT8         = 6;
	static const RecordType   RECORD_UINT8        = 7;
	static const RecordType   RECORD_INT16        = 8;
	static const RecordType   RECORD_UINT16       = 9;
	static const RecordType   RECORD_INT32        = 10;
	static const RecordType   RECORD_UINT32       = 11;
	static const RecordType   RECORD_INT64        = 12;
	static const RecordType   RECORD_UINT64       = 13;
	static const RecordType   RECORD_FLOAT        = 14;
	static const RecordType   RECORD_DOUBLE       = 15;
	static const RecordType   RECORD_STRING_1     = 16;
	static const RecordType   RECORD_STRING_2     = 17;
	static const RecordType   RECORD_STRING_4     = 18;
	static const RecordType   RECORD_BLOB_1       = 19;
	static const RecordType   RECORD_BLOB_2       = 20;
	static const RecordType   RECORD_BLOB_4       = 21;

};

}//namespace SimpleBuffers

}//namespace CX
