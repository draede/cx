
#pragma once


#include "CX/Log/Logger.hpp"
#include "CX/Util/Timer.hpp"


#define CX_LOG_CONCAT_IMPL(a, b)      a ## b
#define CX_LOG_CONCAT(a, b)           CX_LOG_CONCAT_IMPL(a, b)
#define USE_LOCK( lock ) TLockUse MACRO_CONCAT( LockUse, __COUNTER__ )( lock )

#if CX_LOG_LEVEL >= CX_LOG_DEBUG
	#define CXLOGSCOPE(tag, name)      CX::Log::LogScope   CX_LOG_CONCAT(__logscope__, __COUNTER__)()(tag, name);
	#define CXLOGFUNC(tag)             CX::Log::LogScope   CX_LOG_CONCAT(__logscope__, __COUNTER__)()(tag, __FUNCTION__);
#else
	#define CXLOGSCOPE(tag, name);
	#define CXLOGFUNC(tag);
#endif


namespace CX
{

namespace Log
{

class LogScope
{
public:

	LogScope(const Char *szTag, const Char *szName);

	~LogScope();

private:

	String      m_sTag;
	String      m_sName;
	Util::Timer m_timer;

};

}//namespace Log

}//namespace CX

