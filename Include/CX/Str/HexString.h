
#pragma once


#include "CX/Types.h"
#include "CX/String.h"
#include "CX/APIDefs.h"


namespace CX
{

namespace Str
{

class CX_API HexString
{
public:

	enum Flag
	{
		Lowercase = 1,
		Uppercase = 2,
	};

	static StatusCode ToString(const Byte *pBinary, Size cbBinarySize, String *psString, 
	                       int nFlags = Flag::Lowercase);

	static StatusCode FromString(const Char *szString, Byte *pBinary, Size cbBinarySize);

	static Size GetStringLen(Size cbBinarySize);

	static Size GetBinarySize(Size cStringLen);

private:

	HexString();

	~HexString();

};

}//namespace Str

}//namespace CX

