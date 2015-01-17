
#include "CX/Log/DefaultFormatter.h"
#include "CX/Status.h"
#include "CX/Util/Time.h"
#include "CX/Sys/Thread.h"


namespace CX
{

namespace Log
{

DefaultFormatter::DefaultFormatter(int nFlags/* = FLAGS_MEDIUM*/)
{
	Status::Clear();

	m_nFlags = nFlags;
}

DefaultFormatter::~DefaultFormatter()
{
	Status::Clear();
}

void DefaultFormatter::SetFlags(int nFlags/* = FLAGS_MEDIUM*/)
{
	Status::Clear();

	m_nFlags = nFlags;
}

int DefaultFormatter::GetFlags() const
{
	Status::Clear();

	return m_nFlags;
}

StatusCode DefaultFormatter::Write(IOutput *pOutput, Level nLevel, const Char *szTag, 
                                   const Char *pBuffer, Size cLen)
{
	Status::Clear();

	String sOutput;

	if (Show_Date == (m_nFlags & Show_Date) || Show_Time == (m_nFlags & Show_Time))
	{
		Util::Time t;

		t.GetLocalTime();

		if (Show_Date == (m_nFlags & Show_Date))
		{
			Print(&sOutput, "[{1:'0'4}-{2:'0'2}-{3:'0'2}] ", t.GetYear(), t.GetMonth(), t.GetDay());
		}
		if (Show_Time == (m_nFlags & Show_Time))
		{
			Print(&sOutput, "[{1:'0'2}:{2:'0'2}:{3:'0'2}.{4:'0'3}] ", t.GetHour(), t.GetMinute(), 
			      t.GetSecond(), t.GetMillisecond());
		}
	}
	if (Show_PID == (m_nFlags & Show_PID))
	{
		Print(&sOutput, "[{1:' '6}] ", (UInt64)Sys::Thread::GetCurrentProcessID());
	}
	if (Show_TID == (m_nFlags & Show_TID))
	{
		Print(&sOutput, "[{1:' '6}] ", (UInt64)Sys::Thread::GetCurrentThreadID());
	}
	if (Show_Level == (m_nFlags & Show_Level))
	{
		switch (nLevel)
		{
			case Level_Error:   Print(&sOutput, "[ERR] "); break;
			case Level_Warning: Print(&sOutput, "[WRN] "); break;
			case Level_Info:    Print(&sOutput, "[INF] "); break;
			case Level_Verbose: Print(&sOutput, "[VRB] "); break;
			case Level_Debug:   Print(&sOutput, "[DBG] "); break;
			default:            Print(&sOutput, "[{1:|3}] ", nLevel); break;
		}
	}
	if (Show_Tag == (m_nFlags & Show_Tag))
	{
		Print(&sOutput, "[{1:|' '12}] ", szTag);
	}
	Print(&sOutput, " : {1}", pBuffer);
	if (Show_NewLine == (m_nFlags & Show_NewLine))
	{
		Print(&sOutput, "\n");
	}

	return pOutput->Write(nLevel, szTag, sOutput.c_str(), sOutput.size());
}

}//namespace Log

}//namespace CX

