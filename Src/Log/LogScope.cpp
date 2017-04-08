
#include "CX/Log/LogScope.hpp"


namespace CX
{

namespace Log
{

LogScope::LogScope(const Char *szTag, const Char *szName)
{
	m_sTag  = szTag;
	m_sName = szName;
	CXDBG(m_sTag.c_str(), "ENTER {1}", m_sName);
	m_timer.ResetTimer();
}

LogScope::~LogScope()
{
	CXDBG(m_sTag.c_str(), "LEAVE {1} ({2:.6} ms)", m_sName, m_timer.GetElapsedTime() * 1000.0);
}

}//namespace Log

}//namespace CX

