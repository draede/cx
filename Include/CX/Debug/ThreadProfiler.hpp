
#pragma once


#include "CX/Types.hpp"
#include "CX/Status.hpp"


namespace CX
{

namespace Debug
{

//this looks like this because of VS2013...
class ThreadProfiler
{
public:

	struct Scope
	{
		Char   *szFileName;
		Char   *szScopeName;
		int    cLineNo;

		UInt64  cMinDuration;
		UInt64  cMaxDuration;
		UInt64  cTotalDuration;
		UInt64  cCalls;

		UInt64  nStart;

		Scope  *pParent;
		Scope  *pFirstChild;
		Scope  *pPrevSibling;
		Scope  *pNextSibling;
	};

	static void EnterScope(const Char *szFileName, const Char *szScopeName, int cLineNo);

	static void LeaveScope();

	static Scope *CreateScope(const Char *szFileName, const Char *szScopeName, int cLineNo);

	static void DestroyScope(Scope *pScope);

	static bool MatchScope(Scope *pScope, const Char *szFileName, const Char *szScopeName, int cLineNo);

	static bool MatchScope(Scope *pScope1, Scope *pScope2);

	static UInt64 GetTimeStamp();

};

}//namespace Sys

}//namespace CX
