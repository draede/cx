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

#pragma once


#include "CX/Types.hpp"
#include "CX/Status.hpp"
#include "CX/Log/Level.hpp"
#include "CX/Log/IFormatter.hpp"
#include "CX/Log/IOutput.hpp"
#include "CX/Print.hpp"
#include "CX/Sys/Lock.hpp"
#include "CX/APIDefs.hpp"
#include "CX/Vector.hpp"
#include "CX/Scope.hpp"
#include "CX/Status.hpp"


#ifndef CX_LOG_LEVEL
	#define CX_LOG_LEVEL                        5
#endif


#define CXHLPLOG(level, tag, fmt, ...)                                                                                 \
{                                                                                                                      \
	if (CX::Log::Logger::GetDefaultLogger().GetLevel() >= level)                                                        \
	{                                                                                                                   \
		CX::Log::Logger::GetDefaultLogger().Log(level, tag, fmt, ## __VA_ARGS__);                                        \
	}                                                                                                                   \
}

#define CXHLPLOGIF(cnd, level, tag, fmt, ...)                                                                          \
{                                                                                                                      \
	if (CX::Log::Logger::GetDefaultLogger().GetLevel() >= level)                                                        \
	{                                                                                                                   \
		CX::Log::Logger::GetDefaultLogger().LogIf(cnd, level, tag, fmt, ## __VA_ARGS__);                                 \
	}                                                                                                                   \
}

#if CX_LOG_LEVEL >= CX_LOG_ERROR
	#define CXERR(tag, fmt, ...)           CXHLPLOG(CX_LOG_ERROR, tag, fmt, ## __VA_ARGS__)
	#define CXERRIF(cnd, tag, fmt, ...)    CXHLPLOGIF(cnd, CX_LOG_ERROR, tag, fmt, ## __VA_ARGS__)
#else
	#define CXERR(tag, fmt, ...)
	#define CXERRIF(cnd, tag, fmt, ...)
#endif

#if CX_LOG_LEVEL >= CX_LOG_WARN
	#define CXWARN(tag, fmt, ...)          CXHLPLOG(CX_LOG_WARN, tag, fmt, ## __VA_ARGS__)
	#define CXWARNIF(cnd, tag, fmt, ...)   CXHLPLOGIF(cnd, CX_LOG_WARN, tag, fmt, ## __VA_ARGS__)
#else
	#define CXWARN(tag, fmt, ...)
	#define CXWARNIF(cnd, tag, fmt, ...)
#endif

#if CX_LOG_LEVEL >= CX_LOG_INFO
	#define CXINFO(tag, fmt, ...)          CXHLPLOG(CX_LOG_INFO, tag, fmt, ## __VA_ARGS__)
	#define CXINFOIF(cnd, tag, fmt, ...)   CXHLPLOGIF(cnd, CX_LOG_INFO, tag, fmt, ## __VA_ARGS__)
#else
	#define CXINFO(tag, fmt, ...)
	#define CXINFOINF(cnd, tag, fmt, ...)
#endif

#if CX_LOG_LEVEL >= CX_LOG_VERB
	#define CXVERB(tag, fmt, ...)          CXHLPLOG(CX_LOG_VERB, tag, fmt, ## __VA_ARGS__)
	#define CXVERBIF(cnd, tag, fmt, ...)   CXHLPLOGIF(cnd, CX_LOG_VERB, tag, fmt, ## __VA_ARGS__)
#else
	#define CXVERB(tag, fmt, ...)
	#define CXVERBIF(cnd, tag, fmt, ...)
#endif

#if CX_LOG_LEVEL >= CX_LOG_DEBUG
	#define CXDBG(tag, fmt, ...)           CXHLPLOG(CX_LOG_DEBUG, tag, fmt, ## __VA_ARGS__)
	#define CXDBGIF(cnd, tag, fmt, ...)    CXHLPLOGIF(cnd, CX_LOG_DEBUG, tag, fmt, ## __VA_ARGS__)
#else
	#define CXDBG(tag, fmt, ...)
	#define CXDBGIF(cnd, tag, fmt, ...)
#endif


namespace CX
{

namespace Log
{

class CX_API Logger
{
public:

	typedef void (* FreeOutputFunc)(IOutput *pOutput);

	static void DeleteOutput(IOutput *pOutput) { delete pOutput; }

	Logger();

	virtual ~Logger();

	static Logger &GetDefaultLogger();

	Status SetLevel(Level nLevel);

	Level GetLevel() const;

	Status SetFormatter(IFormatter *pFormatter);

	IFormatter *GetFormatter();

	Status AddOutput(IOutput *pOutput, FreeOutputFunc pfnFreeOutput = &Logger::DeleteOutput);

	Status RemoveOutputs();

	Status Log(Level nLevel, const Char *szTag, const Char *szMsg)
	{
		if (nLevel > m_nLevel)
		{
			return Status(Status_Denied, "Level not allowed");
		}

		Sys::Locker locker(&m_fmLogger);

		if (NULL == m_pFormatter)
		{
			return Status(Status_NotInitialized, "No formatter defined");
		}

		if (m_vectorOutputs.empty())
		{
			return Status(Status_NotInitialized, "No outputs defined");
		}

		String sOutput;
		Status status;

		status = Print(&sOutput, szMsg);
		if (status.IsNOK())
		{
			return status;
		}

		for (OutputsVector::iterator iter = m_vectorOutputs.begin(); iter != m_vectorOutputs.end(); ++iter)
		{
			if (NULL != iter->pOutput)
			{
				m_pFormatter->Write(iter->pOutput, nLevel, szTag, sOutput.c_str(), sOutput.size());
			}
		}

		return Status();
	}

	template <typename T1>
	Status Log(Level nLevel, const Char *szTag, const Char *szFormat, T1 p1)
	{
		if (nLevel > m_nLevel)
		{
			return Status(Status_Denied, "Level not allowed");
		}

		Sys::Locker locker(&m_fmLogger);

		if (m_vectorOutputs.empty())
		{
			return Status(Status_NotInitialized, "No outputs defined");
		}

		String sOutput;
		Status status;

		status = Print(&sOutput, szFormat, p1);
		if (status.IsNOK())
		{
			return status;
		}

		for (OutputsVector::iterator iter = m_vectorOutputs.begin(); iter != m_vectorOutputs.end(); ++iter)
		{
			if (NULL != iter->pOutput)
			{
				m_pFormatter->Write(iter->pOutput, nLevel, szTag, sOutput.c_str(), sOutput.size());
			}
		}

		return Status();
	}

	template <typename T1, typename T2>
	Status Log(Level nLevel, const Char *szTag, const Char *szFormat, T1 p1, T2 p2)
	{
		if (nLevel > m_nLevel)
		{
			return Status(Status_Denied, "Level not allowed");
		}

		Sys::Locker locker(&m_fmLogger);

		if (m_vectorOutputs.empty())
		{
			return Status(Status_NotInitialized, "No outputs defined");
		}

		String sOutput;
		Status status;

		status = Print(&sOutput, szFormat, p1, p2);
		if (status.IsNOK())
		{
			return status;
		}

		for (OutputsVector::iterator iter = m_vectorOutputs.begin(); iter != m_vectorOutputs.end(); ++iter)
		{
			if (NULL != iter->pOutput)
			{
				m_pFormatter->Write(iter->pOutput, nLevel, szTag, sOutput.c_str(), sOutput.size());
			}
		}

		return Status();
	}

	template <typename T1, typename T2, typename T3>
	Status Log(Level nLevel, const Char *szTag, const Char *szFormat, T1 p1, T2 p2, T3 p3)
	{
		if (nLevel > m_nLevel)
		{
			return Status(Status_Denied, "Level not allowed");
		}

		Sys::Locker locker(&m_fmLogger);

		if (m_vectorOutputs.empty())
		{
			return Status(Status_NotInitialized, "No outputs defined");
		}

		String sOutput;
		Status status;

		status = Print(&sOutput, szFormat, p1, p2, p3);
		if (status.IsNOK())
		{
			return status;
		}

		for (OutputsVector::iterator iter = m_vectorOutputs.begin(); iter != m_vectorOutputs.end(); ++iter)
		{
			if (NULL != iter->pOutput)
			{
				m_pFormatter->Write(iter->pOutput, nLevel, szTag, sOutput.c_str(), sOutput.size());
			}
		}

		return Status();
	}

	template <typename T1, typename T2, typename T3, typename T4>
	Status Log(Level nLevel, const Char *szTag, const Char *szFormat, T1 p1, T2 p2, T3 p3, T4 p4)
	{
		if (nLevel > m_nLevel)
		{
			return Status(Status_Denied, "Level not allowed");
		}

		Sys::Locker locker(&m_fmLogger);

		if (m_vectorOutputs.empty())
		{
			return Status(Status_NotInitialized, "No outputs defined");
		}

		String sOutput;
		Status status;

		status = Print(&sOutput, szFormat, p1, p2, p3, p4);
		if (status.IsNOK())
		{
			return status;
		}

		for (OutputsVector::iterator iter = m_vectorOutputs.begin(); iter != m_vectorOutputs.end(); ++iter)
		{
			if (NULL != iter->pOutput)
			{
				m_pFormatter->Write(iter->pOutput, nLevel, szTag, sOutput.c_str(), sOutput.size());
			}
		}

		return Status();
	}

	template <typename T1, typename T2, typename T3, typename T4, typename T5>
	Status Log(Level nLevel, const Char *szTag, const Char *szFormat, T1 p1, T2 p2, T3 p3, 
	               T4 p4, T5 p5)
	{
		if (nLevel > m_nLevel)
		{
			return Status(Status_Denied, "Level not allowed");
		}

		Sys::Locker locker(&m_fmLogger);

		if (m_vectorOutputs.empty())
		{
			return Status(Status_NotInitialized, "No outputs defined");
		}

		String sOutput;
		Status status;

		status = Print(&sOutput, szFormat, p1, p2, p3, p4, p5);
		if (status.IsNOK())
		{
			return status;
		}

		for (OutputsVector::iterator iter = m_vectorOutputs.begin(); iter != m_vectorOutputs.end(); ++iter)
		{
			if (NULL != iter->pOutput)
			{
				m_pFormatter->Write(iter->pOutput, nLevel, szTag, sOutput.c_str(), sOutput.size());
			}
		}

		return Status();
	}

	template <typename T1, typename T2, typename T3, typename T4, typename T5, typename T6>
	Status Log(Level nLevel, const Char *szTag, const Char *szFormat, T1 p1, T2 p2, T3 p3, 
	               T4 p4, T5 p5, T6 p6)
	{
		if (nLevel > m_nLevel)
		{
			return Status(Status_Denied, "Level not allowed");
		}

		Sys::Locker locker(&m_fmLogger);

		if (m_vectorOutputs.empty())
		{
			return Status(Status_NotInitialized, "No outputs defined");
		}

		String sOutput;
		Status status;

		status = Print(&sOutput, szFormat, p1, p2, p3, p4, p5, p6);
		if (status.IsNOK())
		{
			return status;
		}

		for (OutputsVector::iterator iter = m_vectorOutputs.begin(); iter != m_vectorOutputs.end(); ++iter)
		{
			if (NULL != iter->pOutput)
			{
				m_pFormatter->Write(iter->pOutput, nLevel, szTag, sOutput.c_str(), sOutput.size());
			}
		}

		return Status();
	}

	template <typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, 
	          typename T7>
	Status Log(Level nLevel, const Char *szTag, const Char *szFormat, T1 p1, T2 p2, T3 p3, 
	               T4 p4, T5 p5, T6 p6, T7 p7)
	{
		if (nLevel > m_nLevel)
		{
			return Status(Status_Denied, "Level not allowed");
		}

		Sys::Locker locker(&m_fmLogger);

		if (m_vectorOutputs.empty())
		{
			return Status(Status_NotInitialized, "No outputs defined");
		}

		String sOutput;
		Status status;

		status = Print(&sOutput, szFormat, p1, p2, p3, p4, p5, p6, p7);
		if (status.IsNOK())
		{
			return status;
		}

		for (OutputsVector::iterator iter = m_vectorOutputs.begin(); iter != m_vectorOutputs.end(); ++iter)
		{
			if (NULL != iter->pOutput)
			{
				m_pFormatter->Write(iter->pOutput, nLevel, szTag, sOutput.c_str(), sOutput.size());
			}
		}

		return Status();
	}

	template <typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, 
	          typename T7, typename T8>
	Status Log(Level nLevel, const Char *szTag, const Char *szFormat, T1 p1, T2 p2, T3 p3, 
	               T4 p4, T5 p5, T6 p6, T7 p7, T8 p8)
	{
		if (nLevel > m_nLevel)
		{
			return Status(Status_Denied, "Level not allowed");
		}

		Sys::Locker locker(&m_fmLogger);

		if (m_vectorOutputs.empty())
		{
			return Status(Status_NotInitialized, "No outputs defined");
		}

		String sOutput;
		Status status;

		status = Print(&sOutput, szFormat, p1, p2, p3, p4, p5, p6, p7, p8);
		if (status.IsNOK())
		{
			return status;
		}

		for (OutputsVector::iterator iter = m_vectorOutputs.begin(); iter != m_vectorOutputs.end(); ++iter)
		{
			if (NULL != iter->pOutput)
			{
				m_pFormatter->Write(iter->pOutput, nLevel, szTag, sOutput.c_str(), sOutput.size());
			}
		}

		return Status();
	}

	template <typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, 
	          typename T7, typename T8, typename T9>
	Status Log(Level nLevel, const Char *szTag, const Char *szFormat, T1 p1, T2 p2, T3 p3, 
	               T4 p4, T5 p5, T6 p6, T7 p7, T8 p8, T9 p9)
	{
		if (nLevel > m_nLevel)
		{
			return Status(Status_Denied, "Level not allowed");
		}

		Sys::Locker locker(&m_fmLogger);

		if (m_vectorOutputs.empty())
		{
			return Status(Status_NotInitialized, "No outputs defined");
		}

		String sOutput;
		Status status;

		status = Print(&sOutput, szFormat, p1, p2, p3, p4, p5, p6, p7, p8, p9);
		if (status.IsNOK())
		{
			return status;
		}

		for (OutputsVector::iterator iter = m_vectorOutputs.begin(); iter != m_vectorOutputs.end(); ++iter)
		{
			if (NULL != iter->pOutput)
			{
				m_pFormatter->Write(iter->pOutput, nLevel, szTag, sOutput.c_str(), sOutput.size());
			}
		}

		return Status();
	}

	template <typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, 
	          typename T7, typename T8, typename T9, typename T10>
	Status Log(Level nLevel, const Char *szTag, const Char *szFormat, T1 p1, T2 p2, T3 p3, 
	               T4 p4, T5 p5, T6 p6, T7 p7, T8 p8, T9 p9, T10 p10)
	{
		if (nLevel > m_nLevel)
		{
			return Status(Status_Denied, "Level not allowed");
		}

		Sys::Locker locker(&m_fmLogger);

		if (m_vectorOutputs.empty())
		{
			return Status(Status_NotInitialized, "No outputs defined");
		}

		String sOutput;
		Status status;

		status = Print(&sOutput, szFormat, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10);
		if (status.IsNOK())
		{
			return status;
		}

		for (OutputsVector::iterator iter = m_vectorOutputs.begin(); iter != m_vectorOutputs.end(); ++iter)
		{
			if (NULL != iter->pOutput)
			{
				m_pFormatter->Write(iter->pOutput, nLevel, szTag, sOutput.c_str(), sOutput.size());
			}
		}

		return Status();
	}

	template <typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, 
	          typename T7, typename T8, typename T9, typename T10, typename T11>
	Status Log(Level nLevel, const Char *szTag, const Char *szFormat, T1 p1, T2 p2, T3 p3, 
	               T4 p4, T5 p5, T6 p6, T7 p7, T8 p8, T9 p9, T10 p10, T11 p11)
	{
		if (nLevel > m_nLevel)
		{
			return Status(Status_Denied, "Level not allowed");
		}

		Sys::Locker locker(&m_fmLogger);

		if (m_vectorOutputs.empty())
		{
			return Status(Status_NotInitialized, "No outputs defined");
		}

		String sOutput;
		Status status;

		status = Print(&sOutput, szFormat, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11);
		if (status.IsNOK())
		{
			return status;
		}

		for (OutputsVector::iterator iter = m_vectorOutputs.begin(); iter != m_vectorOutputs.end(); ++iter)
		{
			if (NULL != iter->pOutput)
			{
				m_pFormatter->Write(iter->pOutput, nLevel, szTag, sOutput.c_str(), sOutput.size());
			}
		}

		return Status();
	}

	template <typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, 
	          typename T7, typename T8, typename T9, typename T10, typename T11, typename T12>
	Status Log(Level nLevel, const Char *szTag, const Char *szFormat, T1 p1, T2 p2, T3 p3, 
	               T4 p4, T5 p5, T6 p6, T7 p7, T8 p8, T9 p9, T10 p10, T11 p11, T12 p12)
	{
		if (nLevel > m_nLevel)
		{
			return Status(Status_Denied, "Level not allowed");
		}

		Sys::Locker locker(&m_fmLogger);

		if (m_vectorOutputs.empty())
		{
			return Status(Status_NotInitialized, "No outputs defined");
		}

		String sOutput;
		Status status;

		status = Print(&sOutput, szFormat, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12);
		if (status.IsNOK())
		{
			return status;
		}

		for (OutputsVector::iterator iter = m_vectorOutputs.begin(); iter != m_vectorOutputs.end(); ++iter)
		{
			if (NULL != iter->pOutput)
			{
				m_pFormatter->Write(iter->pOutput, nLevel, szTag, sOutput.c_str(), sOutput.size());
			}
		}

		return Status();
	}

	template <typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, 
	          typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, 
	          typename T13>
	Status Log(Level nLevel, const Char *szTag, const Char *szFormat, T1 p1, T2 p2, T3 p3, 
	               T4 p4, T5 p5, T6 p6, T7 p7, T8 p8, T9 p9, T10 p10, T11 p11, T12 p12, T13 p13)
	{
		if (nLevel > m_nLevel)
		{
			return Status(Status_Denied, "Level not allowed");
		}

		Sys::Locker locker(&m_fmLogger);

		if (m_vectorOutputs.empty())
		{
			return Status(Status_NotInitialized, "No outputs defined");
		}

		String sOutput;
		Status status;

		status = Print(&sOutput, szFormat, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13);
		if (status.IsNOK())
		{
			return status;
		}

		for (OutputsVector::iterator iter = m_vectorOutputs.begin(); iter != m_vectorOutputs.end(); ++iter)
		{
			if (NULL != iter->pOutput)
			{
				m_pFormatter->Write(iter->pOutput, nLevel, szTag, sOutput.c_str(), sOutput.size());
			}
		}

		return Status();
	}

	template <typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, 
	          typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, 
	          typename T13, typename T14>
	Status Log(Level nLevel, const Char *szTag, const Char *szFormat, T1 p1, T2 p2, T3 p3, 
	               T4 p4, T5 p5, T6 p6, T7 p7, T8 p8, T9 p9, T10 p10, T11 p11, T12 p12, T13 p13, 
	               T14 p14)
	{
		if (nLevel > m_nLevel)
		{
			return Status(Status_Denied, "Level not allowed");
		}

		Sys::Locker locker(&m_fmLogger);

		if (m_vectorOutputs.empty())
		{
			return Status(Status_NotInitialized, "No outputs defined");
		}

		String sOutput;
		Status status;

		status = Print(&sOutput, szFormat, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, 
		               p14);
		if (status.IsNOK())
		{
			return status;
		}

		for (OutputsVector::iterator iter = m_vectorOutputs.begin(); iter != m_vectorOutputs.end(); ++iter)
		{
			if (NULL != iter->pOutput)
			{
				m_pFormatter->Write(iter->pOutput, nLevel, szTag, sOutput.c_str(), sOutput.size());
			}
		}

		return Status();
	}

	template <typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, 
	          typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, 
	          typename T13, typename T14, typename T15>
	Status Log(Level nLevel, const Char *szTag, const Char *szFormat, T1 p1, T2 p2, T3 p3, 
	               T4 p4, T5 p5, T6 p6, T7 p7, T8 p8, T9 p9, T10 p10, T11 p11, T12 p12, T13 p13, 
	               T14 p14, T15 p15)
	{
		if (nLevel > m_nLevel)
		{
			return Status(Status_Denied, "Level not allowed");
		}

		Sys::Locker locker(&m_fmLogger);

		if (m_vectorOutputs.empty())
		{
			return Status(Status_NotInitialized, "No outputs defined");
		}

		String sOutput;
		Status status;

		status = Print(&sOutput, szFormat, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13,
		                p14, p15);
		if (status.IsNOK())
		{
			return status;
		}

		for (OutputsVector::iterator iter = m_vectorOutputs.begin(); iter != m_vectorOutputs.end(); ++iter)
		{
			if (NULL != iter->pOutput)
			{
				m_pFormatter->Write(iter->pOutput, nLevel, szTag, sOutput.c_str(), sOutput.size());
			}
		}

		return Status();
	}

	template <typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, 
	          typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, 
	          typename T13, typename T14, typename T15, typename T16>
	Status Log(Level nLevel, const Char *szTag, const Char *szFormat, T1 p1, T2 p2, T3 p3, 
	               T4 p4, T5 p5, T6 p6, T7 p7, T8 p8, T9 p9, T10 p10, T11 p11, T12 p12, T13 p13, 
	               T14 p14, T15 p15, T16 p16)
	{
		if (nLevel > m_nLevel)
		{
			return Status(Status_Denied, "Level not allowed");
		}

		Sys::Locker locker(&m_fmLogger);

		if (m_vectorOutputs.empty())
		{
			return Status(Status_NotInitialized, "No outputs defined");
		}

		String sOutput;
		Status status;

		status = Print(&sOutput, szFormat, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13,
		                p14, p15, p16);
		if (status.IsNOK())
		{
			return status;
		}

		for (OutputsVector::iterator iter = m_vectorOutputs.begin(); iter != m_vectorOutputs.end(); ++iter)
		{
			if (NULL != iter->pOutput)
			{
				m_pFormatter->Write(iter->pOutput, nLevel, szTag, sOutput.c_str(), sOutput.size());
			}
		}

		return Status();
	}

	Status LogIf(Bool bCond, Level nLevel, const Char *szTag, const Char *szMsg)
	{
		if (!bCond)
		{
			return Status(Status_Denied, "Cond not allowed");
		}

		if (nLevel > m_nLevel)
		{
			return Status(Status_Denied, "Level not allowed");
		}

		Sys::Locker locker(&m_fmLogger);

		if (m_vectorOutputs.empty())
		{
			return Status(Status_NotInitialized, "No outputs defined");
		}

		String sOutput;
		Status status;

		status = Print(&sOutput, szMsg);
		if (status.IsNOK())
		{
			return status;
		}

		for (OutputsVector::iterator iter = m_vectorOutputs.begin(); iter != m_vectorOutputs.end(); ++iter)
		{
			if (NULL != iter->pOutput)
			{
				m_pFormatter->Write(iter->pOutput, nLevel, szTag, sOutput.c_str(), sOutput.size());
			}
		}

		return Status();
	}

	template <typename T1>
	Status LogIf(Bool bCond, Level nLevel, const Char *szTag, const Char *szFormat, T1 p1)
	{
		if (!bCond)
		{
			return Status(Status_Denied, "Cond not allowed");
		}

		if (nLevel > m_nLevel)
		{
			return Status(Status_Denied, "Level not allowed");
		}

		Sys::Locker locker(&m_fmLogger);

		if (m_vectorOutputs.empty())
		{
			return Status(Status_NotInitialized, "No outputs defined");
		}

		String sOutput;
		Status status;

		status = Print(&sOutput, szFormat, p1);
		if (status.IsNOK())
		{
			return status;
		}

		for (OutputsVector::iterator iter = m_vectorOutputs.begin(); iter != m_vectorOutputs.end(); ++iter)
		{
			if (NULL != iter->pOutput)
			{
				m_pFormatter->Write(iter->pOutput, nLevel, szTag, sOutput.c_str(), sOutput.size());
			}
		}

		return Status();
	}

	template <typename T1, typename T2>
	Status LogIf(Bool bCond, Level nLevel, const Char *szTag, const Char *szFormat, T1 p1, T2 p2)
	{
		if (!bCond)
		{
			return Status(Status_Denied, "Cond not allowed");
		}

		if (nLevel > m_nLevel)
		{
			return Status(Status_Denied, "Level not allowed");
		}

		Sys::Locker locker(&m_fmLogger);

		if (m_vectorOutputs.empty())
		{
			return Status(Status_NotInitialized, "No outputs defined");
		}

		String sOutput;
		Status status;

		status = Print(&sOutput, szFormat, p1, p2);
		if (status.IsNOK())
		{
			return status;
		}

		for (OutputsVector::iterator iter = m_vectorOutputs.begin(); iter != m_vectorOutputs.end(); ++iter)
		{
			if (NULL != iter->pOutput)
			{
				m_pFormatter->Write(iter->pOutput, nLevel, szTag, sOutput.c_str(), sOutput.size());
			}
		}

		return Status();
	}

	template <typename T1, typename T2, typename T3>
	Status LogIf(Bool bCond, Level nLevel, const Char *szTag, const Char *szFormat, T1 p1, 
	                 T2 p2, T3 p3)
	{
		if (!bCond)
		{
			return Status(Status_Denied, "Cond not allowed");
		}

		if (nLevel > m_nLevel)
		{
			return Status(Status_Denied, "Level not allowed");
		}

		Sys::Locker locker(&m_fmLogger);

		if (m_vectorOutputs.empty())
		{
			return Status(Status_NotInitialized, "No outputs defined");
		}

		String sOutput;
		Status status;

		status = Print(&sOutput, szFormat, p1, p2, p3);
		if (status.IsNOK())
		{
			return status;
		}

		for (OutputsVector::iterator iter = m_vectorOutputs.begin(); iter != m_vectorOutputs.end(); ++iter)
		{
			if (NULL != iter->pOutput)
			{
				m_pFormatter->Write(iter->pOutput, nLevel, szTag, sOutput.c_str(), sOutput.size());
			}
		}

		return Status();
	}

	template <typename T1, typename T2, typename T3, typename T4>
	Status LogIf(Bool bCond, Level nLevel, const Char *szTag, const Char *szFormat, T1 p1, 
	                 T2 p2, T3 p3, T4 p4)
	{
		if (!bCond)
		{
			return Status(Status_Denied, "Cond not allowed");
		}

		if (nLevel > m_nLevel)
		{
			return Status(Status_Denied, "Level not allowed");
		}

		Sys::Locker locker(&m_fmLogger);

		if (m_vectorOutputs.empty())
		{
			return Status(Status_NotInitialized, "No outputs defined");
		}

		String sOutput;
		Status status;

		status = Print(&sOutput, szFormat, p1, p2, p3, p4);
		if (status.IsNOK())
		{
			return status;
		}

		for (OutputsVector::iterator iter = m_vectorOutputs.begin(); iter != m_vectorOutputs.end(); ++iter)
		{
			if (NULL != iter->pOutput)
			{
				m_pFormatter->Write(iter->pOutput, nLevel, szTag, sOutput.c_str(), sOutput.size());
			}
		}

		return Status();
	}

	template <typename T1, typename T2, typename T3, typename T4, typename T5>
	Status LogIf(Bool bCond, Level nLevel, const Char *szTag, const Char *szFormat, T1 p1, 
	                 T2 p2, T3 p3, T4 p4, T5 p5)
	{
		if (!bCond)
		{
			return Status(Status_Denied, "Cond not allowed");
		}

		if (nLevel > m_nLevel)
		{
			return Status(Status_Denied, "Level not allowed");
		}

		Sys::Locker locker(&m_fmLogger);

		if (m_vectorOutputs.empty())
		{
			return Status(Status_NotInitialized, "No outputs defined");
		}

		String sOutput;
		Status status;

		status = Print(&sOutput, szFormat, p1, p2, p3, p4, p5);
		if (status.IsNOK())
		{
			return status;
		}

		for (OutputsVector::iterator iter = m_vectorOutputs.begin(); iter != m_vectorOutputs.end(); ++iter)
		{
			if (NULL != iter->pOutput)
			{
				m_pFormatter->Write(iter->pOutput, nLevel, szTag, sOutput.c_str(), sOutput.size());
			}
		}

		return Status();
	}

	template <typename T1, typename T2, typename T3, typename T4, typename T5, typename T6>
	Status LogIf(Bool bCond, Level nLevel, const Char *szTag, const Char *szFormat, T1 p1, 
	                 T2 p2, T3 p3, T4 p4, T5 p5, T6 p6)
	{
		if (!bCond)
		{
			return Status(Status_Denied, "Cond not allowed");
		}

		if (nLevel > m_nLevel)
		{
			return Status(Status_Denied, "Level not allowed");
		}

		Sys::Locker locker(&m_fmLogger);

		if (m_vectorOutputs.empty())
		{
			return Status(Status_NotInitialized, "No outputs defined");
		}

		String sOutput;
		Status status;

		status = Print(&sOutput, szFormat, p1, p2, p3, p4, p5, p6);
		if (status.IsNOK())
		{
			return status;
		}

		for (OutputsVector::iterator iter = m_vectorOutputs.begin(); iter != m_vectorOutputs.end(); ++iter)
		{
			if (NULL != iter->pOutput)
			{
				m_pFormatter->Write(iter->pOutput, nLevel, szTag, sOutput.c_str(), sOutput.size());
			}
		}

		return Status();
	}

	template <typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, 
	          typename T7>
	Status LogIf(Bool bCond, Level nLevel, const Char *szTag, const Char *szFormat, T1 p1, 
	                 T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7)
	{
		if (!bCond)
		{
			return Status(Status_Denied, "Cond not allowed");
		}

		if (nLevel > m_nLevel)
		{
			return Status(Status_Denied, "Level not allowed");
		}

		Sys::Locker locker(&m_fmLogger);

		if (m_vectorOutputs.empty())
		{
			return Status(Status_NotInitialized, "No outputs defined");
		}

		String sOutput;
		Status status;

		status = Print(&sOutput, szFormat, p1, p2, p3, p4, p5, p6, p7);
		if (status.IsNOK())
		{
			return status;
		}

		for (OutputsVector::iterator iter = m_vectorOutputs.begin(); iter != m_vectorOutputs.end(); ++iter)
		{
			if (NULL != iter->pOutput)
			{
				m_pFormatter->Write(iter->pOutput, nLevel, szTag, sOutput.c_str(), sOutput.size());
			}
		}

		return Status();
	}

	template <typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, 
	          typename T7, typename T8>
	Status LogIf(Bool bCond, Level nLevel, const Char *szTag, const Char *szFormat, T1 p1, 
	                 T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, T8 p8)
	{
		if (!bCond)
		{
			return Status(Status_Denied, "Cond not allowed");
		}

		if (nLevel > m_nLevel)
		{
			return Status(Status_Denied, "Level not allowed");
		}

		Sys::Locker locker(&m_fmLogger);

		if (m_vectorOutputs.empty())
		{
			return Status(Status_NotInitialized, "No outputs defined");
		}

		String sOutput;
		Status status;

		status = Print(&sOutput, szFormat, p1, p2, p3, p4, p5, p6, p7, p8);
		if (status.IsNOK())
		{
			return status;
		}

		for (OutputsVector::iterator iter = m_vectorOutputs.begin(); iter != m_vectorOutputs.end(); ++iter)
		{
			if (NULL != iter->pOutput)
			{
				m_pFormatter->Write(iter->pOutput, nLevel, szTag, sOutput.c_str(), sOutput.size());
			}
		}

		return Status();
	}

	template <typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, 
	          typename T7, typename T8, typename T9>
	Status LogIf(Bool bCond, Level nLevel, const Char *szTag, const Char *szFormat, T1 p1, 
	                 T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, T8 p8, T9 p9)
	{
		if (!bCond)
		{
			return Status(Status_Denied, "Cond not allowed");
		}

		if (nLevel > m_nLevel)
		{
			return Status(Status_Denied, "Level not allowed");
		}

		Sys::Locker locker(&m_fmLogger);

		if (m_vectorOutputs.empty())
		{
			return Status(Status_NotInitialized, "No outputs defined");
		}

		String sOutput;
		Status status;

		status = Print(&sOutput, szFormat, p1, p2, p3, p4, p5, p6, p7, p8, p9);
		if (status.IsNOK())
		{
			return status;
		}

		for (OutputsVector::iterator iter = m_vectorOutputs.begin(); iter != m_vectorOutputs.end(); ++iter)
		{
			if (NULL != iter->pOutput)
			{
				m_pFormatter->Write(iter->pOutput, nLevel, szTag, sOutput.c_str(), sOutput.size());
			}
		}

		return Status();
	}

	template <typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, 
	          typename T7, typename T8, typename T9, typename T10>
	Status LogIf(Bool bCond, Level nLevel, const Char *szTag, const Char *szFormat, T1 p1, 
	                 T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, T8 p8, T9 p9, T10 p10)
	{
		if (!bCond)
		{
			return Status(Status_Denied, "Cond not allowed");
		}

		if (nLevel > m_nLevel)
		{
			return Status(Status_Denied, "Level not allowed");
		}

		Sys::Locker locker(&m_fmLogger);

		if (m_vectorOutputs.empty())
		{
			return Status(Status_NotInitialized, "No outputs defined");
		}

		String sOutput;
		Status status;

		status = Print(&sOutput, szFormat, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10);
		if (status.IsNOK())
		{
			return status;
		}

		for (OutputsVector::iterator iter = m_vectorOutputs.begin(); iter != m_vectorOutputs.end(); ++iter)
		{
			if (NULL != iter->pOutput)
			{
				m_pFormatter->Write(iter->pOutput, nLevel, szTag, sOutput.c_str(), sOutput.size());
			}
		}

		return Status();
	}

	template <typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, 
	          typename T7, typename T8, typename T9, typename T10, typename T11>
	Status LogIf(Bool bCond, Level nLevel, const Char *szTag, const Char *szFormat, T1 p1, 
	                 T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, T8 p8, T9 p9, T10 p10, T11 p11)
	{
		if (!bCond)
		{
			return Status(Status_Denied, "Cond not allowed");
		}

		if (nLevel > m_nLevel)
		{
			return Status(Status_Denied, "Level not allowed");
		}

		Sys::Locker locker(&m_fmLogger);

		if (m_vectorOutputs.empty())
		{
			return Status(Status_NotInitialized, "No outputs defined");
		}

		String sOutput;
		Status status;

		status = Print(&sOutput, szFormat, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11);
		if (status.IsNOK())
		{
			return status;
		}

		for (OutputsVector::iterator iter = m_vectorOutputs.begin(); iter != m_vectorOutputs.end(); ++iter)
		{
			if (NULL != iter->pOutput)
			{
				m_pFormatter->Write(iter->pOutput, nLevel, szTag, sOutput.c_str(), sOutput.size());
			}
		}

		return Status();
	}

	template <typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, 
	          typename T7, typename T8, typename T9, typename T10, typename T11, typename T12>
	Status LogIf(Bool bCond, Level nLevel, const Char *szTag, const Char *szFormat, T1 p1, 
	                 T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, T8 p8, T9 p9, T10 p10, T11 p11, 
	                 T12 p12)
	{
		if (!bCond)
		{
			return Status(Status_Denied, "Cond not allowed");
		}

		if (nLevel > m_nLevel)
		{
			return Status(Status_Denied, "Level not allowed");
		}

		Sys::Locker locker(&m_fmLogger);

		if (m_vectorOutputs.empty())
		{
			return Status(Status_NotInitialized, "No outputs defined");
		}

		String sOutput;
		Status status;

		status = Print(&sOutput, szFormat, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12);
		if (status.IsNOK())
		{
			return status;
		}

		for (OutputsVector::iterator iter = m_vectorOutputs.begin(); iter != m_vectorOutputs.end(); ++iter)
		{
			if (NULL != iter->pOutput)
			{
				m_pFormatter->Write(iter->pOutput, nLevel, szTag, sOutput.c_str(), sOutput.size());
			}
		}

		return Status();
	}

	template <typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, 
	          typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, 
	          typename T13>
	Status LogIf(Bool bCond, Level nLevel, const Char *szTag, const Char *szFormat, T1 p1, 
	                 T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, T8 p8, T9 p9, T10 p10, T11 p11, 
	                 T12 p12, T13 p13)
	{
		if (!bCond)
		{
			return Status(Status_Denied, "Cond not allowed");
		}

		if (nLevel > m_nLevel)
		{
			return Status(Status_Denied, "Level not allowed");
		}

		Sys::Locker locker(&m_fmLogger);

		if (m_vectorOutputs.empty())
		{
			return Status(Status_NotInitialized, "No outputs defined");
		}

		String sOutput;
		Status status;

		status = Print(&sOutput, szFormat, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13);
		if (status.IsNOK())
		{
			return status;
		}

		for (OutputsVector::iterator iter = m_vectorOutputs.begin(); iter != m_vectorOutputs.end(); ++iter)
		{
			if (NULL != iter->pOutput)
			{
				m_pFormatter->Write(iter->pOutput, nLevel, szTag, sOutput.c_str(), sOutput.size());
			}
		}

		return Status();
	}

	template <typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, 
	          typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, 
	          typename T13, typename T14>
	Status LogIf(Bool bCond, Level nLevel, const Char *szTag, const Char *szFormat, T1 p1, 
	                 T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, T8 p8, T9 p9, T10 p10, T11 p11, 
	                 T12 p12, T13 p13, T14 p14)
	{
		if (!bCond)
		{
			return Status(Status_Denied, "Cond not allowed");
		}

		if (nLevel > m_nLevel)
		{
			return Status(Status_Denied, "Level not allowed");
		}

		Sys::Locker locker(&m_fmLogger);

		if (m_vectorOutputs.empty())
		{
			return Status(Status_NotInitialized, "No outputs defined");
		}

		String sOutput;
		Status status;

		status = Print(&sOutput, szFormat, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13,
		               p14);
		if (status.IsNOK())
		{
			return status;
		}

		for (OutputsVector::iterator iter = m_vectorOutputs.begin(); iter != m_vectorOutputs.end(); ++iter)
		{
			if (NULL != iter->pOutput)
			{
				m_pFormatter->Write(iter->pOutput, nLevel, szTag, sOutput.c_str(), sOutput.size());
			}
		}

		return Status();
	}

	template <typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, 
	          typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, 
	          typename T13, typename T14, typename T15>
	Status LogIf(Bool bCond, Level nLevel, const Char *szTag, const Char *szFormat, T1 p1, 
	                 T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, T8 p8, T9 p9, T10 p10, T11 p11, 
	                 T12 p12, T13 p13, T14 p14, T15 p15)
	{
		if (!bCond)
		{
			return Status(Status_Denied, "Cond not allowed");
		}

		if (nLevel > m_nLevel)
		{
			return Status(Status_Denied, "Level not allowed");
		}

		Sys::Locker locker(&m_fmLogger);

		if (m_vectorOutputs.empty())
		{
			return Status(Status_NotInitialized, "No outputs defined");
		}

		String sOutput;
		Status status;

		status = Print(&sOutput, szFormat, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13,
		               p14, p15);
		if (status.IsNOK())
		{
			return status;
		}

		for (OutputsVector::iterator iter = m_vectorOutputs.begin(); iter != m_vectorOutputs.end(); ++iter)
		{
			if (NULL != iter->pOutput)
			{
				m_pFormatter->Write(iter->pOutput, nLevel, szTag, sOutput.c_str(), sOutput.size());
			}
		}

		return Status();
	}

	template <typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, 
	          typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, 
	          typename T13, typename T14, typename T15, typename T16>
	Status LogIf(Bool bCond, Level nLevel, const Char *szTag, const Char *szFormat, T1 p1, 
	                 T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, T8 p8, T9 p9, T10 p10, T11 p11, 
	                 T12 p12, T13 p13, T14 p14, T15 p15, T16 p16)
	{
		if (!bCond)
		{
			return Status(Status_Denied, "Cond not allowed");
		}

		if (nLevel > m_nLevel)
		{
			return Status(Status_Denied, "Level not allowed");
		}

		Sys::Locker locker(&m_fmLogger);

		if (m_vectorOutputs.empty())
		{
			return Status(Status_NotInitialized, "No outputs defined");
		}

		String sOutput;
		Status status;

		status = Print(&sOutput, szFormat, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13,
		               p14, p15, p16);
		if (status.IsNOK())
		{
			return status;
		}

		for (OutputsVector::iterator iter = m_vectorOutputs.begin(); iter != m_vectorOutputs.end(); ++iter)
		{
			if (NULL != iter->pOutput)
			{
				m_pFormatter->Write(iter->pOutput, nLevel, szTag, sOutput.c_str(), sOutput.size());
			}
		}

		return Status();
	}

private:

	struct Output
	{
		IOutput          *pOutput;
		FreeOutputFunc   pfnFreeOutput;
	};

	typedef Vector<Output>::Type   OutputsVector;

	Level            m_nLevel;
	IFormatter       *m_pFormatter;
#pragma warning(push)
#pragma warning(disable: 4251)
	OutputsVector    m_vectorOutputs;
#pragma warning(pop)
	Sys::Lock        m_fmLogger;

};

}//namespace Log

}//namespace CX

