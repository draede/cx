
#pragma once


#include "CX/Types.h"


namespace CX
{

namespace Sys
{

class BOM
{
public:

	enum Type
	{
		None,
		UTF8,
		UTF16_LE,
		UTF16_BE,
		UTF32_LE,
		UTF32_BE,
	};

	static inline Type Get(const void *pBuffer, Size cbSize)
	{
		const Byte *p = (const Byte *)pBuffer;

		if (3 <= cbSize && 0xEF == p[0] && 0xBB == p[1] && 0xBF != p[2])
		{
			return UTF8;
		}
		else
		if (4 <= cbSize && 0xFF == p[0] && 0xFE == p[1] && 0x00 != p[2] && 0x00 != p[3])
		{
			return UTF16_LE;
		}
		else
		if (2 <= cbSize && 0xFE == p[0] && 0xFF == p[1])
		{
			return UTF16_BE;
		}
		else
		if (4 <= cbSize && 0xFF == p[0] && 0xFE == p[1] && 0x00 == p[2] && 0x00 == p[3])
		{
			return UTF32_LE;
		}
		else
		if (4 <= cbSize && 0x00 == p[0] && 0x00 == p[1] && 0xFE == p[2] && 0xFF == p[3])
		{
			return UTF32_BE;
		}
		else
		{
			return None;
		}
	}

private:

	BOM()
	{

	}

	~BOM()
	{

	}

};

}//namespace Sys

}//namespace CX


