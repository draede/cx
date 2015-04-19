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


#include "CX/Types.hpp"
#include "../../../Contrib/ByteOrder/Include/byteorder.h"
#include "CX/IObject.hpp"


namespace CX
{

namespace Sys
{

class ByteOrder : public IObject
{
public:

	enum Type
	{
		LittleEndian,
		BigEndian,
	};

	static inline Type Get()
	{		
		if (BIG_ENDIAN == ByteOrderTest())
		{
			return BigEndian;
		}
		else
		{
			return LittleEndian;
		}
	}

	static inline UInt16 H2LE(UInt16 nVal)
	{
		return htole16(nVal);
	}

	static inline UInt32 H2LE(UInt32 nVal)
	{
		return htole32(nVal);
	}

	static inline UInt64 H2LE(UInt64 nVal)
	{
		return htole64(nVal);
	}

	static inline UInt16 H2BE(UInt16 nVal)
	{
		return htobe16(nVal);
	}

	static inline UInt32 H2BE(UInt32 nVal)
	{
		return htobe32(nVal);
	}

	static inline UInt64 H2BE(UInt64 nVal)
	{
		return htobe64(nVal);
	}


	static inline UInt16 LE2H(UInt16 nVal)
	{
		return letoh16(nVal);
	}

	static inline UInt32 LE2H(UInt32 nVal)
	{
		return letoh32(nVal);
	}

	static inline UInt64 LE2H(UInt64 nVal)
	{
		return letoh64(nVal);
	}

	static inline UInt16 BE2H(UInt16 nVal)
	{
		return betoh16(nVal);
	}

	static inline UInt32 BE2H(UInt32 nVal)
	{
		return betoh32(nVal);
	}

	static inline UInt64 BE2H(UInt64 nVal)
	{
		return betoh64(nVal);
	}


	static inline UInt16 LE2BE(UInt16 nVal)
	{
		return bswap16(nVal);
	}

	static inline UInt32 LE2BE(UInt32 nVal)
	{
		return bswap32(nVal);
	}

	static inline UInt64 LE2BE(UInt64 nVal)
	{
		return bswap64(nVal);
	}

	static inline UInt16 BE2LE(UInt16 nVal)
	{
		return bswap16(nVal);
	}

	static inline UInt32 BE2LE(UInt32 nVal)
	{
		return bswap32(nVal);
	}

	static inline UInt64 BE2LE(UInt64 nVal)
	{
		return bswap64(nVal);
	}

private:

	ByteOrder()
	{

	}

	~ByteOrder()
	{

	}

};

}//namespace Sys

}//namespace CX



