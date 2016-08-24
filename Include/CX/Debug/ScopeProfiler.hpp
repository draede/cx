
#pragma once


#include "CX/Types.hpp"
#include "CX/Status.hpp"


#define CX_PROF_CONCAT(x, y)         x##y
#define CX_PROF_MACRO_CONCAT(x, y)   CX_PROF_CONCAT(x, y)

#ifdef CX_PROFILER
#define CX_SCOPEPROF(name)                                                                                             \
CX::Debug::ScopeProfiler   CX_PROF_MACRO_CONCAT(__cx_scope_prof__, __COUNTER)(__FILE__, name, __LINE__);
#define CX_ASCOPEPROF                CX_SCOPEPROF(# __LINE__)
#define CX_FUNCPROF                  CX_SCOPEPROF(__FUNCTION__);
#else
#define CX_SCOPEPROF(name)
#define CX_ASCOPEPROF
#define CX_FUNCPROF
#endif


namespace CX
{

namespace Debug
{

class ScopeProfiler
{
public:

	ScopeProfiler(const Char *szFileName, const Char *szScopeName, int cLineNo);

	~ScopeProfiler();

};

}//namespace Debug

}//namespace CX

