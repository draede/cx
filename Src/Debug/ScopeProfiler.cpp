
#include "CX/Debug/ScopeProfiler.hpp"
#include "CX/Debug/ThreadProfiler.hpp"


namespace CX
{

namespace Debug
{

ScopeProfiler::ScopeProfiler(const Char *szFileName, const Char *szScopeName, int cLineNo)
{
	ThreadProfiler::EnterScope(szFileName, szScopeName, cLineNo);
}

ScopeProfiler::~ScopeProfiler()
{
	ThreadProfiler::LeaveScope();
}

}//namespace Debug

}//namespace CX
