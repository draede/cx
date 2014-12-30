
#pragma once


#include "CX/C/Platform.h"


#if defined(CX_OS_WINDOWS)
   #include "CX/C/Platform/Windows/string.h"
#else	
	#error "string.h not implemented on this platform"
#endif

static inline bool cx_strcopy(CX_Char *szDst, CX_Size cDstLen, const CX_Char *szSrc, CX_Size *pcSrcLen)
{
	CX_Char			*pszDst;
	const CX_Char	*pszSrc;

	*pcSrcLen	= 0;
	pszSrc		= szSrc;
	pszDst		= szDst;
	while (*pcSrcLen + 1 < cDstLen && '\0' != *pszSrc)
	{
		*pszDst = *pszSrc;
		pszSrc++;
		pszDst++;
		(*pcSrcLen)++;
	}
	*pszDst = 0;

	return ('\0' == *pszSrc);
}
