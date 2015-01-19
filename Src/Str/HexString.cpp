
#include "CX/Str/HexString.h"
#include "CX/Status.h"


namespace CX
{

namespace Str
{

HexString::HexString()
{
	Status::Clear();
}

HexString::~HexString()
{
	Status::Clear();
}

StatusCode HexString::ToString(const Byte *pBinary, Size cbBinarySize, String *psString, 
                               int nFlags/* = Flag::Lowercase*/)
{
	static Char hexdigits_u[] =
	{
		'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'
	};
	static Char hexdigits_l[] =
	{
		'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'
	};
	Byte hi;
	Byte lo;

	Status::Clear();

	for (Size i = 0; i < cbBinarySize; i++)
	{
		hi = pBinary[i] / 16;
		lo = pBinary[i] % 16;
		if (10 <= hi)
		{
			if (Flag::Uppercase == nFlags)
			{
				*psString += hexdigits_u[hi];
			}
			else
			{
				*psString += hexdigits_l[hi];
			}
		}
		else
		{
			*psString += '0' + hi;
		}
		if (10 <= lo)
		{
			if (Flag::Uppercase == nFlags)
			{
				*psString += hexdigits_u[lo];
			}
			else
			{
				*psString += hexdigits_l[lo];
			}
		}
		else
		{
			*psString += '0' + lo;
		}
	}

	return Status_OK;
}

StatusCode HexString::FromString(const Char *szString, Byte *pBinary, Size cbBinarySize)
{
	const Char   *pszPos;
	Size         cIndex;

	Status::Clear();

	cIndex = 0;
	pszPos = szString;
	while('\0' != *pszPos && cIndex < cbBinarySize)
	{
		pBinary[cIndex] = 0;
		if ('A' <= *pszPos && 'F' >= *pszPos)
		{
			pBinary[cIndex] += (*pszPos - 'A' + 10) * 16;
		}
		else
		if ('0' <= *pszPos && '9' >= *pszPos)
		{
			pBinary[cIndex] += (*pszPos - '0') * 16;
		}
		else
		{
			return Status_ParseFailed;
		}
		pszPos++;
		if ('A' <= *pszPos && 'F' >= *pszPos)
		{
			pBinary[cIndex] += (*pszPos - 'A' + 10);
		}
		else
		if ('0' <= *pszPos && '9' >= *pszPos)
		{
			pBinary[cIndex] += (*pszPos - '0');
		}
		else
		{
			return Status_ParseFailed;
		}
		pszPos++;
		cIndex++;
	}
	if (cIndex != cbBinarySize || '\0' != *pszPos)
	{
		return Status_ParseFailed;
	}

	return Status_OK;
}

Size HexString::GetStringLen(Size cbBinarySize)
{
	Status::Clear();

	return cbBinarySize * 2;
}

Size HexString::GetBinarySize(Size cStringLen)
{
	Status::Clear();

	return cStringLen / 2;
}

}//namespace Str

}//namespace CX

