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

#pragma once


#include "CX/Types.hpp"
#include "CX/Hash/xxHash32.hpp"
#include "CX/APIDefs.hpp"
#include "CX/IObject.hpp"


namespace CX
{

namespace SB
{

class CX_API BinaryData : public IObject
{
public:

	typedef Hash::xxHash32  CheckSum;
	typedef UInt32          CheckSumType;
	typedef UInt64          MagicType;

	static const MagicType MAGIC         = 0x302E315642535843; // 'CXSBV1.0'

	static const Byte STATE_BEGINMEMBER  = 1;
	static const Byte STATE_ENDMEMBER    = 2;
	static const Byte STATE_BEGINITEM    = 3;
	static const Byte STATE_ENDITEM      = 4;
	static const Byte STATE_BEGINOBJECT  = 5;
	static const Byte STATE_ENDOBJECT    = 6;
	static const Byte STATE_BEGINARRAY   = 7;
	static const Byte STATE_ENDARRAY     = 8;
	static const Byte STATE_BOOL         = 9;
	static const Byte STATE_INT8         = 10;
	static const Byte STATE_UINT8        = 11;
	static const Byte STATE_INT16        = 12;
	static const Byte STATE_UINT16       = 13;
	static const Byte STATE_INT32        = 14;
	static const Byte STATE_UINT32       = 15;
	static const Byte STATE_INT64        = 16;
	static const Byte STATE_UINT64       = 17;
	static const Byte STATE_FLOAT        = 18;
	static const Byte STATE_DOUBLE       = 19;
	static const Byte STATE_STRING       = 20;
	static const Byte STATE_WSTRING      = 21;

	static const Size MAX_STRING_LEN     = 1048576;
	static const Size MAX_ARRAY_ITEMS    = 16777216;
	static const Size MAX_OBJECT_ITEMS   = 16777216;

private:

	BinaryData()
	{
	}

	~BinaryData()
	{
	}

};

}//namespace SB

}//namespace CX

