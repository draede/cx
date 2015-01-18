
#include "CX/Log/Logger.h"
#include "CX/Log/DefaultFormatter.h"


namespace CX
{

namespace Log
{

Logger::Logger()
{
	Status::Clear();

	m_nLevel = Level_Info;
	if (NULL == (m_pFormatter = New<DefaultFormatter>()))
	{
		Status::Set(Status_MemAllocFailed, "Failed to allocate formatter");
	}
}

Logger::~Logger()
{
	Status::Clear();

	RemoveOutputs();
	SetFormatter(NULL);
}

Logger &Logger::GetDefaultLogger()
{
	static Logger logger;

	Status::Clear();

	return logger;
}

StatusCode Logger::SetLevel(Level nLevel)
{
	Status::Clear();

	m_nLevel = nLevel;

	return Status_OK;
}

Level Logger::GetLevel() const
{
	Status::Clear();

	return m_nLevel;
}

StatusCode Logger::SetFormatter(IFormatter *pFormatter)
{
	Sys::Locker   locker(&m_fmLogger);

	Status::Clear();

	if (NULL != m_pFormatter)
	{
		Delete(m_pFormatter);
		m_pFormatter = NULL;
	}
	m_pFormatter = pFormatter;

	return Status_OK;
}

IFormatter *Logger::GetFormatter()
{
	Sys::Locker   locker(&m_fmLogger);

	Status::Clear();

	return m_pFormatter;
}

StatusCode Logger::AddOutput(IOutput *pOutput)
{
	Sys::Locker   locker(&m_fmLogger);

	Status::Clear();

	m_vectorOutputs.push_back(pOutput);

	return Status_OK;
}

StatusCode Logger::RemoveOutputs()
{
	Sys::Locker   locker(&m_fmLogger);

	Status::Clear();

	for (OutputsVector::iterator iter = m_vectorOutputs.begin(); 
	     iter != m_vectorOutputs.end(); ++iter)
	{
		Delete(*iter);
	}
	m_vectorOutputs.clear();

	return Status_OK;
}

}//namespace Log

}//namespace CX

