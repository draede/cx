
#pragma once


#include "CX/Platform.h"


#if defined(CX_OS_WINDOWS)


#include "CX/Types.h"
#include "CX/String.h"
#include "CX/Limits.h"


namespace CX
{

namespace Str
{

class UTF8
{
public:

	static StatusCode ToWChar(const Char *szUTF8, WString *psWChar, Size cUTF8Len = SIZET_MAX);

	static StatusCode FromWChar(const WChar *wszWChar, String *psUTF8, Size cWCharLen = SIZET_MAX);

private:

	UTF8();

	~UTF8();

};

}//namespace Str

}//namespace CX


#endif

