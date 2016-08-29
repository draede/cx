
#include "CX/Debug/ScopeProfiler.hpp"
#include "CX/Debug/ThreadProfiler.hpp"
#include "CX/Debug/Profiler.hpp"


namespace CX
{

namespace Debug
{

ScopeProfiler::ScopeProfiler(const Char *szFileName, const Char *szScopeName, int cLineNo)
{
	if (!Profiler::Get().GetEnabled())
	{
		return;
	}

	ThreadProfiler::EnterScope(szFileName, szScopeName, cLineNo);
}

ScopeProfiler::~ScopeProfiler()
{
	if (!Profiler::Get().GetEnabled())
	{
		return;
	}

	ThreadProfiler::LeaveScope();
}

}//namespace Debug

}//namespace CX
