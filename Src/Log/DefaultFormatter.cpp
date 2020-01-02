/* 
 * CX - C++ framework for general purpose development
 *
 * https://github.com/draede/cx
 * 
 * Copyright (C) 2014 - 2020 draede - draede [at] outlook [dot] com
 *
 * Released under the MIT License.
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */ 

#include "CX/precomp.hpp"
#include "CX/Log/DefaultFormatter.hpp"
#include "CX/Status.hpp"
#include "CX/Util/Time.hpp"
#include "CX/Sys/Process.hpp"
#include "CX/Sys/Thread.hpp"


namespace CX
{

namespace Log
{

DefaultFormatter::DefaultFormatter(int nFlags/* = FLAGS_MEDIUM*/)
{
	m_nFlags = nFlags;
}

DefaultFormatter::~DefaultFormatter()
{
}

void DefaultFormatter::SetFlags(int nFlags/* = FLAGS_MEDIUM*/)
{
	m_nFlags = nFlags;
}

int DefaultFormatter::GetFlags() const
{
	return m_nFlags;
}

Status DefaultFormatter::Write(IOutput *pOutput, Level nLevel, const Char *szTag, 
                                   const Char *pBuffer, Size cLen)
{
	String sOutput;

	CX_UNUSED(cLen);

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
		Print(&sOutput, "[{1:' '6}] ", (UInt64)Sys::Process::GetCurrentProcessID());
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
	if (sOutput.empty())
	{
		Print(&sOutput, "{1}", pBuffer);
	}
	else
	{
		Print(&sOutput, ": {1}", pBuffer);
	}
	if (Show_NewLine == (m_nFlags & Show_NewLine))
	{
		Print(&sOutput, "\n");
	}

	return pOutput->Write(nLevel, szTag, sOutput.c_str(), sOutput.size());
}

}//namespace Log

}//namespace CX

