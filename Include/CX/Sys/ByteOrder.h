
#pragma once


#include "CX/Types.h"
#include "../../../Contrib/ByteOrder/Include/byteorder.h"


namespace CX
{

namespace Sys
{

class ByteOrder
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



