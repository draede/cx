
#pragma once


#include "CX/Types.h"
#include "CX/Log/Level.h"
#include "CX/Log/IFormatter.h"
#include "CX/Log/IOutput.h"
#include "CX/Print.h"
#include "CX/Sys/Lock.h"
#include "CX/APIDefs.h"
#include "CX/Vector.h"
#include "CX/Scope.h"
#include "CX/Status.h"


#ifndef CX_LOG_LEVEL
	#define CX_LOG_LEVEL                        5
#endif

#if CX_LOG_LEVEL >= CX_LOG_ERROR
	#define CXERR(tag, fmt, ...)           CX::Log::Logger::GetDefaultLogger().Log                  \
	                                             (CX_LOG_ERROR, tag, fmt, ## __VA_ARGS__)
	#define CXERRIF(cnd, tag, fmt, ...)    CX::Log::Logger::GetDefaultLogger().LogIf                \
	                                             (cnd, CX_LOG_ERROR, tag, fmt, ## __VA_ARGS__)
#else
	#define CXERR(tag, fmt, ...)
	#define CXERRIF(cnd, tag, fmt, ...)
#endif

#if CX_LOG_LEVEL >= CX_LOG_WARN
	#define CXWARN(tag, fmt, ...)          CX::Log::Logger::GetDefaultLogger().Log                  \
	                                             (CX_LOG_WARN, tag, fmt, ## __VA_ARGS__)
	#define CXWARNIF(cnd, tag, fmt, ...)   CX::Log::Logger::GetDefaultLogger().LogIf                \
	                                             (cnd, CX_LOG_WARN, tag, fmt, ## __VA_ARGS__)
#else
	#define CXWARN(tag, fmt, ...)
	#define CXWARNIF(cnd, tag, fmt, ...)
#endif

#if CX_LOG_LEVEL >= CX_LOG_INFO
	#define CXINFO(tag, fmt, ...)          CX::Log::Logger::GetDefaultLogger().Log                  \
	                                             (CX_LOG_INFO, tag, fmt, ## __VA_ARGS__)
	#define CXINFOIF(cnd, tag, fmt, ...)   CX::Log::Logger::GetDefaultLogger().LogIf                \
	                                             (cnd, CX_LOG_INFO, tag, fmt, ## __VA_ARGS__)
#else
	#define CXINFO(tag, fmt, ...)
	#define CXINFOINF(cnd, tag, fmt, ...)
#endif

#if CX_LOG_LEVEL >= CX_LOG_VERB
	#define CXVERB(tag, fmt, ...)          CX::Log::Logger::GetDefaultLogger().Log                  \
	                                             (CX_LOG_VERB, tag, fmt, ## __VA_ARGS__)
	#define CXVERBIF(cnd, tag, fmt, ...)   CX::Log::Logger::GetDefaultLogger().LogIf                \
	                                             (cnd, CX_LOG_VERB, tag, fmt, ## __VA_ARGS__)
#else
	#define CXVERB(tag, fmt, ...)
	#define CXVERBIF(cnd, tag, fmt, ...)
#endif

#if CX_LOG_LEVEL >= CX_LOG_DEBUG
	#define CXDBG(tag, fmt, ...)           CX::Log::Logger::GetDefaultLogger().Log                  \
	                                             (CX_LOG_DEBUG, tag, fmt, ## __VA_ARGS__)
	#define CXDBGIF(cnd, tag, fmt, ...)    CX::Log::Logger::GetDefaultLogger().LogIf                \
	                                             (cnd, CX_LOG_DEBUG, tag, fmt, ## __VA_ARGS__)
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

	Logger();

	virtual ~Logger();

	static Logger &GetDefaultLogger();

	StatusCode SetLevel(Level nLevel);

	Level GetLevel() const;

	//pFormatter was created with New
	StatusCode SetFormatter(IFormatter *pFormatter);

	IFormatter *GetFormatter();

	//pOutput was created with New
	StatusCode AddOutput(IOutput *pOutput);

	StatusCode RemoveOutputs();

	StatusCode Log(Level nLevel, const Char *szTag, const Char *szMsg)
	{
		Status::Clear();

		if (nLevel > m_nLevel)
		{
			return Status::Set(Status_Denied, "Level not allowed");
		}

		Sys::Locker locker(&m_fmLogger);

		if (NULL == m_pFormatter)
		{
			return Status::Set(Status_NotInitialized, "No formatter defined");
		}

		if (m_vectorOutputs.empty())
		{
			return Status::Set(Status_NotInitialized, "No outputs defined");
		}

		String sOutput;

		if (CXNOK(Print(&sOutput, szMsg)))
		{
			return Status::GetCode();
		}

		for (OutputsVector::iterator iter = m_vectorOutputs.begin(); 
		     iter != m_vectorOutputs.end(); ++iter)
		{
			m_pFormatter->Write(*iter, nLevel, szTag, sOutput.c_str(), sOutput.size());
		}

		return Status_OK;
	}

	template <typename T1>
	StatusCode Log(Level nLevel, const Char *szTag, const Char *szFormat, T1 p1)
	{
		Status::Clear();

		if (nLevel > m_nLevel)
		{
			return Status::Set(Status_Denied, "Level not allowed");
		}

		Sys::Locker locker(&m_fmLogger);

		if (m_vectorOutputs.empty())
		{
			return Status::Set(Status_NotInitialized, "No outputs defined");
		}

		String sOutput;

		if (CXNOK(Print(&sOutput, szFormat, p1)))
		{
			return Status::GetCode();
		}

		for (OutputsVector::iterator iter = m_vectorOutputs.begin(); 
		     iter != m_vectorOutputs.end(); ++iter)
		{
			m_pFormatter->Write(*iter, nLevel, szTag, sOutput.c_str(), sOutput.size());
		}

		return Status_OK;
	}

	template <typename T1, typename T2>
	StatusCode Log(Level nLevel, const Char *szTag, const Char *szFormat, T1 p1, T2 p2)
	{
		Status::Clear();

		if (nLevel > m_nLevel)
		{
			return Status::Set(Status_Denied, "Level not allowed");
		}

		Sys::Locker locker(&m_fmLogger);

		if (m_vectorOutputs.empty())
		{
			return Status::Set(Status_NotInitialized, "No outputs defined");
		}

		String sOutput;

		if (CXNOK(Print(&sOutput, szFormat, p1, p2)))
		{
			return Status::GetCode();
		}

		for (OutputsVector::iterator iter = m_vectorOutputs.begin(); 
		     iter != m_vectorOutputs.end(); ++iter)
		{
			m_pFormatter->Write(*iter, nLevel, szTag, sOutput.c_str(), sOutput.size());
		}

		return Status_OK;
	}

	template <typename T1, typename T2, typename T3>
	StatusCode Log(Level nLevel, const Char *szTag, const Char *szFormat, T1 p1, T2 p2, T3 p3)
	{
		Status::Clear();

		if (nLevel > m_nLevel)
		{
			return Status::Set(Status_Denied, "Level not allowed");
		}

		Sys::Locker locker(&m_fmLogger);

		if (m_vectorOutputs.empty())
		{
			return Status::Set(Status_NotInitialized, "No outputs defined");
		}

		String sOutput;

		if (CXNOK(Print(&sOutput, szFormat, p1, p2, p3)))
		{
			return Status::GetCode();
		}

		for (OutputsVector::iterator iter = m_vectorOutputs.begin(); 
		     iter != m_vectorOutputs.end(); ++iter)
		{
			m_pFormatter->Write(*iter, nLevel, szTag, sOutput.c_str(), sOutput.size());
		}

		return Status_OK;
	}

	template <typename T1, typename T2, typename T3, typename T4>
	StatusCode Log(Level nLevel, const Char *szTag, const Char *szFormat, T1 p1, T2 p2, T3 p3, T4 p4)
	{
		Status::Clear();

		if (nLevel > m_nLevel)
		{
			return Status::Set(Status_Denied, "Level not allowed");
		}

		Sys::Locker locker(&m_fmLogger);

		if (m_vectorOutputs.empty())
		{
			return Status::Set(Status_NotInitialized, "No outputs defined");
		}

		String sOutput;

		if (CXNOK(Print(&sOutput, szFormat, p1, p2, p3, p4)))
		{
			return Status::GetCode();
		}

		for (OutputsVector::iterator iter = m_vectorOutputs.begin(); 
		     iter != m_vectorOutputs.end(); ++iter)
		{
			m_pFormatter->Write(*iter, nLevel, szTag, sOutput.c_str(), sOutput.size());
		}

		return Status_OK;
	}

	template <typename T1, typename T2, typename T3, typename T4, typename T5>
	StatusCode Log(Level nLevel, const Char *szTag, const Char *szFormat, T1 p1, T2 p2, T3 p3, 
	               T4 p4, T5 p5)
	{
		Status::Clear();

		if (nLevel > m_nLevel)
		{
			return Status::Set(Status_Denied, "Level not allowed");
		}

		Sys::Locker locker(&m_fmLogger);

		if (m_vectorOutputs.empty())
		{
			return Status::Set(Status_NotInitialized, "No outputs defined");
		}

		String sOutput;

		if (CXNOK(Print(&sOutput, szFormat, p1, p2, p3, p4, p5)))
		{
			return Status::GetCode();
		}

		for (OutputsVector::iterator iter = m_vectorOutputs.begin(); 
		     iter != m_vectorOutputs.end(); ++iter)
		{
			m_pFormatter->Write(*iter, nLevel, szTag, sOutput.c_str(), sOutput.size());
		}

		return Status_OK;
	}

	template <typename T1, typename T2, typename T3, typename T4, typename T5, typename T6>
	StatusCode Log(Level nLevel, const Char *szTag, const Char *szFormat, T1 p1, T2 p2, T3 p3, 
	               T4 p4, T5 p5, T6 p6)
	{
		Status::Clear();

		if (nLevel > m_nLevel)
		{
			return Status::Set(Status_Denied, "Level not allowed");
		}

		Sys::Locker locker(&m_fmLogger);

		if (m_vectorOutputs.empty())
		{
			return Status::Set(Status_NotInitialized, "No outputs defined");
		}

		String sOutput;

		if (CXNOK(Print(&sOutput, szFormat, p1, p2, p3,  p4, p5, p6)))
		{
			return Status::GetCode();
		}

		for (OutputsVector::iterator iter = m_vectorOutputs.begin(); 
		     iter != m_vectorOutputs.end(); ++iter)
		{
			m_pFormatter->Write(*iter, nLevel, szTag, sOutput.c_str(), sOutput.size());
		}

		return Status_OK;
	}

	template <typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, 
	          typename T7>
	StatusCode Log(Level nLevel, const Char *szTag, const Char *szFormat, T1 p1, T2 p2, T3 p3, 
	               T4 p4, T5 p5, T6 p6, T7 p7)
	{
		Status::Clear();

		if (nLevel > m_nLevel)
		{
			return Status::Set(Status_Denied, "Level not allowed");
		}

		Sys::Locker locker(&m_fmLogger);

		if (m_vectorOutputs.empty())
		{
			return Status::Set(Status_NotInitialized, "No outputs defined");
		}

		String sOutput;

		if (CXNOK(Print(&sOutput, szFormat, p1, p2, p3, p4, p5, p6, p7)))
		{
			return Status::GetCode();
		}

		for (OutputsVector::iterator iter = m_vectorOutputs.begin(); 
		     iter != m_vectorOutputs.end(); ++iter)
		{
			m_pFormatter->Write(*iter, nLevel, szTag, sOutput.c_str(), sOutput.size());
		}

		return Status_OK;
	}

	template <typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, 
	          typename T7, typename T8>
	StatusCode Log(Level nLevel, const Char *szTag, const Char *szFormat, T1 p1, T2 p2, T3 p3, 
	               T4 p4, T5 p5, T6 p6, T7 p7, T8 p8)
	{
		Status::Clear();

		if (nLevel > m_nLevel)
		{
			return Status::Set(Status_Denied, "Level not allowed");
		}

		Sys::Locker locker(&m_fmLogger);

		if (m_vectorOutputs.empty())
		{
			return Status::Set(Status_NotInitialized, "No outputs defined");
		}

		String sOutput;

		if (CXNOK(Print(&sOutput, szFormat, p1, p2, p3, p4, p5, p6, p7, p8)))
		{
			return Status::GetCode();
		}

		for (OutputsVector::iterator iter = m_vectorOutputs.begin(); 
		     iter != m_vectorOutputs.end(); ++iter)
		{
			m_pFormatter->Write(*iter, nLevel, szTag, sOutput.c_str(), sOutput.size());
		}

		return Status_OK;
	}

	template <typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, 
	          typename T7, typename T8, typename T9>
	StatusCode Log(Level nLevel, const Char *szTag, const Char *szFormat, T1 p1, T2 p2, T3 p3, 
	               T4 p4, T5 p5, T6 p6, T7 p7, T8 p8, T9 p9)
	{
		Status::Clear();

		if (nLevel > m_nLevel)
		{
			return Status::Set(Status_Denied, "Level not allowed");
		}

		Sys::Locker locker(&m_fmLogger);

		if (m_vectorOutputs.empty())
		{
			return Status::Set(Status_NotInitialized, "No outputs defined");
		}

		String sOutput;

		if (CXNOK(Print(&sOutput, szFormat, p1, p2, p3, p4, p5, p6, p7, p8, p9)))
		{
			return Status::GetCode();
		}

		for (OutputsVector::iterator iter = m_vectorOutputs.begin(); 
		     iter != m_vectorOutputs.end(); ++iter)
		{
			m_pFormatter->Write(*iter, nLevel, szTag, sOutput.c_str(), sOutput.size());
		}

		return Status_OK;
	}

	template <typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, 
	          typename T7, typename T8, typename T9, typename T10>
	StatusCode Log(Level nLevel, const Char *szTag, const Char *szFormat, T1 p1, T2 p2, T3 p3, 
	               T4 p4, T5 p5, T6 p6, T7 p7, T8 p8, T9 p9, T10 p10)
	{
		Status::Clear();

		if (nLevel > m_nLevel)
		{
			return Status::Set(Status_Denied, "Level not allowed");
		}

		Sys::Locker locker(&m_fmLogger);

		if (m_vectorOutputs.empty())
		{
			return Status::Set(Status_NotInitialized, "No outputs defined");
		}

		String sOutput;

		if (CXNOK(Print(&sOutput, szFormat, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)))
		{
			return Status::GetCode();
		}

		for (OutputsVector::iterator iter = m_vectorOutputs.begin(); 
		     iter != m_vectorOutputs.end(); ++iter)
		{
			m_pFormatter->Write(*iter, nLevel, szTag, sOutput.c_str(), sOutput.size());
		}

		return Status_OK;
	}

	template <typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, 
	          typename T7, typename T8, typename T9, typename T10, typename T11>
	StatusCode Log(Level nLevel, const Char *szTag, const Char *szFormat, T1 p1, T2 p2, T3 p3, 
	               T4 p4, T5 p5, T6 p6, T7 p7, T8 p8, T9 p9, T10 p10, T11 p11)
	{
		Status::Clear();

		if (nLevel > m_nLevel)
		{
			return Status::Set(Status_Denied, "Level not allowed");
		}

		Sys::Locker locker(&m_fmLogger);

		if (m_vectorOutputs.empty())
		{
			return Status::Set(Status_NotInitialized, "No outputs defined");
		}

		String sOutput;

		if (CXNOK(Print(&sOutput, szFormat, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11)))
		{
			return Status::GetCode();
		}

		for (OutputsVector::iterator iter = m_vectorOutputs.begin(); 
		     iter != m_vectorOutputs.end(); ++iter)
		{
			m_pFormatter->Write(*iter, nLevel, szTag, sOutput.c_str(), sOutput.size());
		}

		return Status_OK;
	}

	template <typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, 
	          typename T7, typename T8, typename T9, typename T10, typename T11, typename T12>
	StatusCode Log(Level nLevel, const Char *szTag, const Char *szFormat, T1 p1, T2 p2, T3 p3, 
	               T4 p4, T5 p5, T6 p6, T7 p7, T8 p8, T9 p9, T10 p10, T11 p11, T12 p12)
	{
		Status::Clear();

		if (nLevel > m_nLevel)
		{
			return Status::Set(Status_Denied, "Level not allowed");
		}

		Sys::Locker locker(&m_fmLogger);

		if (m_vectorOutputs.empty())
		{
			return Status::Set(Status_NotInitialized, "No outputs defined");
		}

		String sOutput;

		if (CXNOK(Print(&sOutput, szFormat, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12)))
		{
			return Status::GetCode();
		}

		for (OutputsVector::iterator iter = m_vectorOutputs.begin(); 
		     iter != m_vectorOutputs.end(); ++iter)
		{
			m_pFormatter->Write(*iter, nLevel, szTag, sOutput.c_str(), sOutput.size());
		}

		return Status_OK;
	}

	template <typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, 
	          typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, 
	          typename T13>
	StatusCode Log(Level nLevel, const Char *szTag, const Char *szFormat, T1 p1, T2 p2, T3 p3, 
	               T4 p4, T5 p5, T6 p6, T7 p7, T8 p8, T9 p9, T10 p10, T11 p11, T12 p12, T13 p13)
	{
		Status::Clear();

		if (nLevel > m_nLevel)
		{
			return Status::Set(Status_Denied, "Level not allowed");
		}

		Sys::Locker locker(&m_fmLogger);

		if (m_vectorOutputs.empty())
		{
			return Status::Set(Status_NotInitialized, "No outputs defined");
		}

		String sOutput;

		if (CXNOK(Print(&sOutput, szFormat, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13)))
		{
			return Status::GetCode();
		}

		for (OutputsVector::iterator iter = m_vectorOutputs.begin(); 
		     iter != m_vectorOutputs.end(); ++iter)
		{
			m_pFormatter->Write(*iter, nLevel, szTag, sOutput.c_str(), sOutput.size());
		}

		return Status_OK;
	}

	template <typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, 
	          typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, 
	          typename T13, typename T14>
	StatusCode Log(Level nLevel, const Char *szTag, const Char *szFormat, T1 p1, T2 p2, T3 p3, 
	               T4 p4, T5 p5, T6 p6, T7 p7, T8 p8, T9 p9, T10 p10, T11 p11, T12 p12, T13 p13, 
	               T14 p14)
	{
		Status::Clear();

		if (nLevel > m_nLevel)
		{
			return Status::Set(Status_Denied, "Level not allowed");
		}

		Sys::Locker locker(&m_fmLogger);

		if (m_vectorOutputs.empty())
		{
			return Status::Set(Status_NotInitialized, "No outputs defined");
		}

		String sOutput;

		if (CXNOK(Print(&sOutput, szFormat, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, 
		                p14)))
		{
			return Status::GetCode();
		}

		for (OutputsVector::iterator iter = m_vectorOutputs.begin(); 
		     iter != m_vectorOutputs.end(); ++iter)
		{
			m_pFormatter->Write(*iter, nLevel, szTag, sOutput.c_str(), sOutput.size());
		}

		return Status_OK;
	}

	template <typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, 
	          typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, 
	          typename T13, typename T14, typename T15>
	StatusCode Log(Level nLevel, const Char *szTag, const Char *szFormat, T1 p1, T2 p2, T3 p3, 
	               T4 p4, T5 p5, T6 p6, T7 p7, T8 p8, T9 p9, T10 p10, T11 p11, T12 p12, T13 p13, 
	               T14 p14, T15 p15)
	{
		Status::Clear();

		if (nLevel > m_nLevel)
		{
			return Status::Set(Status_Denied, "Level not allowed");
		}

		Sys::Locker locker(&m_fmLogger);

		if (m_vectorOutputs.empty())
		{
			return Status::Set(Status_NotInitialized, "No outputs defined");
		}

		String sOutput;

		if (CXNOK(Print(&sOutput, szFormat, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, 
		                p14, p15)))
		{
			return Status::GetCode();
		}

		for (OutputsVector::iterator iter = m_vectorOutputs.begin(); 
		     iter != m_vectorOutputs.end(); ++iter)
		{
			m_pFormatter->Write(*iter, nLevel, szTag, sOutput.c_str(), sOutput.size());
		}

		return Status_OK;
	}

	template <typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, 
	          typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, 
	          typename T13, typename T14, typename T15, typename T16>
	StatusCode Log(Level nLevel, const Char *szTag, const Char *szFormat, T1 p1, T2 p2, T3 p3, 
	               T4 p4, T5 p5, T6 p6, T7 p7, T8 p8, T9 p9, T10 p10, T11 p11, T12 p12, T13 p13, 
	               T14 p14, T15 p15, T16 p16)
	{
		Status::Clear();

		if (nLevel > m_nLevel)
		{
			return Status::Set(Status_Denied, "Level not allowed");
		}

		Sys::Locker locker(&m_fmLogger);

		if (m_vectorOutputs.empty())
		{
			return Status::Set(Status_NotInitialized, "No outputs defined");
		}

		String sOutput;

		if (CXNOK(Print(&sOutput, szFormat, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, 
		                p14, p15, p16)))
		{
			return Status::GetCode();
		}

		for (OutputsVector::iterator iter = m_vectorOutputs.begin(); 
		     iter != m_vectorOutputs.end(); ++iter)
		{
			m_pFormatter->Write(*iter, nLevel, szTag, sOutput.c_str(), sOutput.size());
		}

		return Status_OK;
	}

	StatusCode LogIf(bool bCond, Level nLevel, const Char *szTag, const Char *szMsg)
	{
		Status::Clear();

		if (!bCond)
		{
			return Status::Set(Status_Denied, "Cond not allowed");
		}

		if (nLevel > m_nLevel)
		{
			return Status::Set(Status_Denied, "Level not allowed");
		}

		Sys::Locker locker(&m_fmLogger);

		if (m_vectorOutputs.empty())
		{
			return Status::Set(Status_NotInitialized, "No outputs defined");
		}

		String sOutput;

		if (CXNOK(Print(&sOutput, szMsg)))
		{
			return Status::GetCode();
		}

		for (OutputsVector::iterator iter = m_vectorOutputs.begin(); 
		     iter != m_vectorOutputs.end(); ++iter)
		{
			m_pFormatter->Write(*iter, nLevel, szTag, sOutput.c_str(), sOutput.size());
		}

		return Status_OK;
	}

	template <typename T1>
	StatusCode LogIf(bool bCond, Level nLevel, const Char *szTag, const Char *szFormat, T1 p1)
	{
		Status::Clear();

		if (!bCond)
		{
			return Status::Set(Status_Denied, "Cond not allowed");
		}

		if (nLevel > m_nLevel)
		{
			return Status::Set(Status_Denied, "Level not allowed");
		}

		Sys::Locker locker(&m_fmLogger);

		if (m_vectorOutputs.empty())
		{
			return Status::Set(Status_NotInitialized, "No outputs defined");
		}

		String sOutput;

		if (CXNOK(Print(&sOutput, szFormat, p1)))
		{
			return Status::GetCode();
		}

		for (OutputsVector::iterator iter = m_vectorOutputs.begin(); 
		     iter != m_vectorOutputs.end(); ++iter)
		{
			m_pFormatter->Write(*iter, nLevel, szTag, sOutput.c_str(), sOutput.size());
		}

		return Status_OK;
	}

	template <typename T1, typename T2>
	StatusCode LogIf(bool bCond, Level nLevel, const Char *szTag, const Char *szFormat, T1 p1, T2 p2)
	{
		Status::Clear();

		if (!bCond)
		{
			return Status::Set(Status_Denied, "Cond not allowed");
		}

		if (nLevel > m_nLevel)
		{
			return Status::Set(Status_Denied, "Level not allowed");
		}

		Sys::Locker locker(&m_fmLogger);

		if (m_vectorOutputs.empty())
		{
			return Status::Set(Status_NotInitialized, "No outputs defined");
		}

		String sOutput;

		if (CXNOK(Print(&sOutput, szFormat, p1, p2)))
		{
			return Status::GetCode();
		}

		for (OutputsVector::iterator iter = m_vectorOutputs.begin(); 
		     iter != m_vectorOutputs.end(); ++iter)
		{
			m_pFormatter->Write(*iter, nLevel, szTag, sOutput.c_str(), sOutput.size());
		}

		return Status_OK;
	}

	template <typename T1, typename T2, typename T3>
	StatusCode LogIf(bool bCond, Level nLevel, const Char *szTag, const Char *szFormat, T1 p1, 
	                 T2 p2, T3 p3)
	{
		Status::Clear();

		if (!bCond)
		{
			return Status::Set(Status_Denied, "Cond not allowed");
		}

		if (nLevel > m_nLevel)
		{
			return Status::Set(Status_Denied, "Level not allowed");
		}

		Sys::Locker locker(&m_fmLogger);

		if (m_vectorOutputs.empty())
		{
			return Status::Set(Status_NotInitialized, "No outputs defined");
		}

		String sOutput;

		if (CXNOK(Print(&sOutput, szFormat, p1, p2, p3)))
		{
			return Status::GetCode();
		}

		for (OutputsVector::iterator iter = m_vectorOutputs.begin(); 
		     iter != m_vectorOutputs.end(); ++iter)
		{
			m_pFormatter->Write(*iter, nLevel, szTag, sOutput.c_str(), sOutput.size());
		}

		return Status_OK;
	}

	template <typename T1, typename T2, typename T3, typename T4>
	StatusCode LogIf(bool bCond, Level nLevel, const Char *szTag, const Char *szFormat, T1 p1, 
	                 T2 p2, T3 p3, T4 p4)
	{
		Status::Clear();

		if (!bCond)
		{
			return Status::Set(Status_Denied, "Cond not allowed");
		}

		if (nLevel > m_nLevel)
		{
			return Status::Set(Status_Denied, "Level not allowed");
		}

		Sys::Locker locker(&m_fmLogger);

		if (m_vectorOutputs.empty())
		{
			return Status::Set(Status_NotInitialized, "No outputs defined");
		}

		String sOutput;

		if (CXNOK(Print(&sOutput, szFormat, p1, p2, p3, p4)))
		{
			return Status::GetCode();
		}

		for (OutputsVector::iterator iter = m_vectorOutputs.begin(); 
		     iter != m_vectorOutputs.end(); ++iter)
		{
			m_pFormatter->Write(*iter, nLevel, szTag, sOutput.c_str(), sOutput.size());
		}

		return Status_OK;
	}

	template <typename T1, typename T2, typename T3, typename T4, typename T5>
	StatusCode LogIf(bool bCond, Level nLevel, const Char *szTag, const Char *szFormat, T1 p1, 
	                 T2 p2, T3 p3, T4 p4, T5 p5)
	{
		Status::Clear();

		if (!bCond)
		{
			return Status::Set(Status_Denied, "Cond not allowed");
		}

		if (nLevel > m_nLevel)
		{
			return Status::Set(Status_Denied, "Level not allowed");
		}

		Sys::Locker locker(&m_fmLogger);

		if (m_vectorOutputs.empty())
		{
			return Status::Set(Status_NotInitialized, "No outputs defined");
		}

		String sOutput;

		if (CXNOK(Print(&sOutput, szFormat, p1, p2, p3, p4, p5)))
		{
			return Status::GetCode();
		}

		for (OutputsVector::iterator iter = m_vectorOutputs.begin(); 
		     iter != m_vectorOutputs.end(); ++iter)
		{
			m_pFormatter->Write(*iter, nLevel, szTag, sOutput.c_str(), sOutput.size());
		}

		return Status_OK;
	}

	template <typename T1, typename T2, typename T3, typename T4, typename T5, typename T6>
	StatusCode LogIf(bool bCond, Level nLevel, const Char *szTag, const Char *szFormat, T1 p1, 
	                 T2 p2, T3 p3, T4 p4, T5 p5, T6 p6)
	{
		Status::Clear();

		if (!bCond)
		{
			return Status::Set(Status_Denied, "Cond not allowed");
		}

		if (nLevel > m_nLevel)
		{
			return Status::Set(Status_Denied, "Level not allowed");
		}

		Sys::Locker locker(&m_fmLogger);

		if (m_vectorOutputs.empty())
		{
			return Status::Set(Status_NotInitialized, "No outputs defined");
		}

		String sOutput;

		if (CXNOK(Print(&sOutput, szFormat, p1, p2, p3, p4, p5, p6)))
		{
			return Status::GetCode();
		}

		for (OutputsVector::iterator iter = m_vectorOutputs.begin(); 
		     iter != m_vectorOutputs.end(); ++iter)
		{
			m_pFormatter->Write(*iter, nLevel, szTag, sOutput.c_str(), sOutput.size());
		}

		return Status_OK;
	}

	template <typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, 
	          typename T7>
	StatusCode LogIf(bool bCond, Level nLevel, const Char *szTag, const Char *szFormat, T1 p1, 
	                 T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7)
	{
		Status::Clear();

		if (!bCond)
		{
			return Status::Set(Status_Denied, "Cond not allowed");
		}

		if (nLevel > m_nLevel)
		{
			return Status::Set(Status_Denied, "Level not allowed");
		}

		Sys::Locker locker(&m_fmLogger);

		if (m_vectorOutputs.empty())
		{
			return Status::Set(Status_NotInitialized, "No outputs defined");
		}

		String sOutput;

		if (CXNOK(Print(&sOutput, szFormat, p1, p2, p3, p4, p5, p6, p7)))
		{
			return Status::GetCode();
		}

		for (OutputsVector::iterator iter = m_vectorOutputs.begin(); 
		     iter != m_vectorOutputs.end(); ++iter)
		{
			m_pFormatter->Write(*iter, nLevel, szTag, sOutput.c_str(), sOutput.size());
		}

		return Status_OK;
	}

	template <typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, 
	          typename T7, typename T8>
	StatusCode LogIf(bool bCond, Level nLevel, const Char *szTag, const Char *szFormat, T1 p1, 
	                 T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, T8 p8)
	{
		Status::Clear();

		if (!bCond)
		{
			return Status::Set(Status_Denied, "Cond not allowed");
		}

		if (nLevel > m_nLevel)
		{
			return Status::Set(Status_Denied, "Level not allowed");
		}

		Sys::Locker locker(&m_fmLogger);

		if (m_vectorOutputs.empty())
		{
			return Status::Set(Status_NotInitialized, "No outputs defined");
		}

		String sOutput;

		if (CXNOK(Print(&sOutput, szFormat, p1, p2, p3, p4, p5, p6, p7, p8)))
		{
			return Status::GetCode();
		}

		for (OutputsVector::iterator iter = m_vectorOutputs.begin(); 
		     iter != m_vectorOutputs.end(); ++iter)
		{
			m_pFormatter->Write(*iter, nLevel, szTag, sOutput.c_str(), sOutput.size());
		}

		return Status_OK;
	}

	template <typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, 
	          typename T7, typename T8, typename T9>
	StatusCode LogIf(bool bCond, Level nLevel, const Char *szTag, const Char *szFormat, T1 p1, 
	                 T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, T8 p8, T9 p9)
	{
		Status::Clear();

		if (!bCond)
		{
			return Status::Set(Status_Denied, "Cond not allowed");
		}

		if (nLevel > m_nLevel)
		{
			return Status::Set(Status_Denied, "Level not allowed");
		}

		Sys::Locker locker(&m_fmLogger);

		if (m_vectorOutputs.empty())
		{
			return Status::Set(Status_NotInitialized, "No outputs defined");
		}

		String sOutput;

		if (CXNOK(Print(&sOutput, szFormat, p1, p2, p3, p4, p5, p6, p7, p8, p9)))
		{
			return Status::GetCode();
		}

		for (OutputsVector::iterator iter = m_vectorOutputs.begin(); 
		     iter != m_vectorOutputs.end(); ++iter)
		{
			m_pFormatter->Write(*iter, nLevel, szTag, sOutput.c_str(), sOutput.size());
		}

		return Status_OK;
	}

	template <typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, 
	          typename T7, typename T8, typename T9, typename T10>
	StatusCode LogIf(bool bCond, Level nLevel, const Char *szTag, const Char *szFormat, T1 p1, 
	                 T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, T8 p8, T9 p9, T10 p10)
	{
		Status::Clear();

		if (!bCond)
		{
			return Status::Set(Status_Denied, "Cond not allowed");
		}

		if (nLevel > m_nLevel)
		{
			return Status::Set(Status_Denied, "Level not allowed");
		}

		Sys::Locker locker(&m_fmLogger);

		if (m_vectorOutputs.empty())
		{
			return Status::Set(Status_NotInitialized, "No outputs defined");
		}

		String sOutput;

		if (CXNOK(Print(&sOutput, szFormat, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)))
		{
			return Status::GetCode();
		}

		for (OutputsVector::iterator iter = m_vectorOutputs.begin(); 
		     iter != m_vectorOutputs.end(); ++iter)
		{
			m_pFormatter->Write(*iter, nLevel, szTag, sOutput.c_str(), sOutput.size());
		}

		return Status_OK;
	}

	template <typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, 
	          typename T7, typename T8, typename T9, typename T10, typename T11>
	StatusCode LogIf(bool bCond, Level nLevel, const Char *szTag, const Char *szFormat, T1 p1, 
	                 T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, T8 p8, T9 p9, T10 p10, T11 p11)
	{
		Status::Clear();

		if (!bCond)
		{
			return Status::Set(Status_Denied, "Cond not allowed");
		}

		if (nLevel > m_nLevel)
		{
			return Status::Set(Status_Denied, "Level not allowed");
		}

		Sys::Locker locker(&m_fmLogger);

		if (m_vectorOutputs.empty())
		{
			return Status::Set(Status_NotInitialized, "No outputs defined");
		}

		String sOutput;

		if (CXNOK(Print(&sOutput, szFormat, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11)))
		{
			return Status::GetCode();
		}

		for (OutputsVector::iterator iter = m_vectorOutputs.begin(); 
		     iter != m_vectorOutputs.end(); ++iter)
		{
			m_pFormatter->Write(*iter, nLevel, szTag, sOutput.c_str(), sOutput.size());
		}

		return Status_OK;
	}

	template <typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, 
	          typename T7, typename T8, typename T9, typename T10, typename T11, typename T12>
	StatusCode LogIf(bool bCond, Level nLevel, const Char *szTag, const Char *szFormat, T1 p1, 
	                 T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, T8 p8, T9 p9, T10 p10, T11 p11, 
	                 T12 p12)
	{
		Status::Clear();

		if (!bCond)
		{
			return Status::Set(Status_Denied, "Cond not allowed");
		}

		if (nLevel > m_nLevel)
		{
			return Status::Set(Status_Denied, "Level not allowed");
		}

		Sys::Locker locker(&m_fmLogger);

		if (m_vectorOutputs.empty())
		{
			return Status::Set(Status_NotInitialized, "No outputs defined");
		}

		String sOutput;

		if (CXNOK(Print(&sOutput, szFormat, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12)))
		{
			return Status::GetCode();
		}

		for (OutputsVector::iterator iter = m_vectorOutputs.begin(); 
		     iter != m_vectorOutputs.end(); ++iter)
		{
			m_pFormatter->Write(*iter, nLevel, szTag, sOutput.c_str(), sOutput.size());
		}

		return Status_OK;
	}

	template <typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, 
	          typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, 
	          typename T13>
	StatusCode LogIf(bool bCond, Level nLevel, const Char *szTag, const Char *szFormat, T1 p1, 
	                 T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, T8 p8, T9 p9, T10 p10, T11 p11, 
	                 T12 p12, T13 p13)
	{
		Status::Clear();

		if (!bCond)
		{
			return Status::Set(Status_Denied, "Cond not allowed");
		}

		if (nLevel > m_nLevel)
		{
			return Status::Set(Status_Denied, "Level not allowed");
		}

		Sys::Locker locker(&m_fmLogger);

		if (m_vectorOutputs.empty())
		{
			return Status::Set(Status_NotInitialized, "No outputs defined");
		}

		String sOutput;

		if (CXNOK(Print(&sOutput, szFormat, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13)))
		{
			return Status::GetCode();
		}

		for (OutputsVector::iterator iter = m_vectorOutputs.begin(); 
		     iter != m_vectorOutputs.end(); ++iter)
		{
			m_pFormatter->Write(*iter, nLevel, szTag, sOutput.c_str(), sOutput.size());
		}

		return Status_OK;
	}

	template <typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, 
	          typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, 
	          typename T13, typename T14>
	StatusCode LogIf(bool bCond, Level nLevel, const Char *szTag, const Char *szFormat, T1 p1, 
	                 T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, T8 p8, T9 p9, T10 p10, T11 p11, 
	                 T12 p12, T13 p13, T14 p14)
	{
		Status::Clear();

		if (!bCond)
		{
			return Status::Set(Status_Denied, "Cond not allowed");
		}

		if (nLevel > m_nLevel)
		{
			return Status::Set(Status_Denied, "Level not allowed");
		}

		Sys::Locker locker(&m_fmLogger);

		if (m_vectorOutputs.empty())
		{
			return Status::Set(Status_NotInitialized, "No outputs defined");
		}

		String sOutput;

		if (CXNOK(Print(&sOutput, szFormat, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, 
		                p14)))
		{
			return Status::GetCode();
		}

		for (OutputsVector::iterator iter = m_vectorOutputs.begin(); 
		     iter != m_vectorOutputs.end(); ++iter)
		{
			m_pFormatter->Write(*iter, nLevel, szTag, sOutput.c_str(), sOutput.size());
		}

		return Status_OK;
	}

	template <typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, 
	          typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, 
	          typename T13, typename T14, typename T15>
	StatusCode LogIf(bool bCond, Level nLevel, const Char *szTag, const Char *szFormat, T1 p1, 
	                 T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, T8 p8, T9 p9, T10 p10, T11 p11, 
	                 T12 p12, T13 p13, T14 p14, T15 p15)
	{
		Status::Clear();

		if (!bCond)
		{
			return Status::Set(Status_Denied, "Cond not allowed");
		}

		if (nLevel > m_nLevel)
		{
			return Status::Set(Status_Denied, "Level not allowed");
		}

		Sys::Locker locker(&m_fmLogger);

		if (m_vectorOutputs.empty())
		{
			return Status::Set(Status_NotInitialized, "No outputs defined");
		}

		String sOutput;

		if (CXNOK(Print(&sOutput, szFormat, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, 
		                p14, p15)))
		{
			return Status::GetCode();
		}

		for (OutputsVector::iterator iter = m_vectorOutputs.begin(); 
		     iter != m_vectorOutputs.end(); ++iter)
		{
			m_pFormatter->Write(*iter, nLevel, szTag, sOutput.c_str(), sOutput.size());
		}

		return Status_OK;
	}

	template <typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, 
	          typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, 
	          typename T13, typename T14, typename T15, typename T16>
	StatusCode LogIf(bool bCond, Level nLevel, const Char *szTag, const Char *szFormat, T1 p1, 
	                 T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, T8 p8, T9 p9, T10 p10, T11 p11, 
	                 T12 p12, T13 p13, T14 p14, T15 p15, T16 p16)
	{
		Status::Clear();

		if (!bCond)
		{
			return Status::Set(Status_Denied, "Cond not allowed");
		}

		if (nLevel > m_nLevel)
		{
			return Status::Set(Status_Denied, "Level not allowed");
		}

		Sys::Locker locker(&m_fmLogger);

		if (m_vectorOutputs.empty())
		{
			return Status::Set(Status_NotInitialized, "No outputs defined");
		}

		String sOutput;

		if (CXNOK(Print(&sOutput, szFormat, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, 
		                p14, p15, p16)))
		{
			return Status::GetCode();
		}

		for (OutputsVector::iterator iter = m_vectorOutputs.begin(); 
		     iter != m_vectorOutputs.end(); ++iter)
		{
			m_pFormatter->Write(*iter, nLevel, szTag, sOutput.c_str(), sOutput.size());
		}

		return Status_OK;
	}

private:

	typedef Vector<IOutput *>::Type   OutputsVector;

	Level            m_nLevel;
	IFormatter       *m_pFormatter;
#pragma warning(push)
#pragma warning(disable: 4251)
	OutputsVector    m_vectorOutputs;
#pragma warning(pop)
	Sys::Lock   m_fmLogger;

};

}//namespace Log

}//namespace CX

