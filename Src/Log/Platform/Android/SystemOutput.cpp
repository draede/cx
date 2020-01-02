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
#include "CX/Platform.hpp"


#if defined(CX_OS_ANDROID)


#include "CX/Log/SystemOutput.hpp"
#include "CX/Status.hpp"
#include "CX/Str/UTF8.hpp"
#include <syslog.h>


namespace CX
{

namespace Log
{

SystemOutput::SystemOutput()
{
}

SystemOutput::~SystemOutput()
{
}

Status SystemOutput::Write(Level nLevel, const Char *szTag, const Char *pBuffer, Size cLen)
{
	String sOut(pBuffer, cLen);

	switch(nLevel)
	{
		case Level_Error:   syslog(LOG_ERR, "%s", sOut.c_str()); break;
		case Level_Info:    syslog(LOG_INFO, "%s", sOut.c_str()); break;
		case Level_Warning: syslog(LOG_WARNING, "%s", sOut.c_str()); break;
		case Level_Verbose: syslog(LOG_DEBUG, "%s", sOut.c_str()); break;
		case Level_Debug:   syslog(LOG_DEBUG, "%s", sOut.c_str()); break;
	}

	return Status();
}

}//namespace Log

}//namespace CX


#endif


