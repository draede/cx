/* 
 * CX - C++ framework for general purpose development
 *
 * https://github.com/draede/cx
 * 
 * Copyright (C) 2014 - 2017 draede, draede [at] outlook [dot] com
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

#include "CX/Log/Logger.hpp"
#include "CX/Log/DefaultFormatter.hpp"


namespace CX
{

namespace Log
{

Logger::Logger()
{
	m_nLevel = Level_Info;
	if (NULL == (m_pFormatter = new (std::nothrow) DefaultFormatter()))
	{
		//
	}
}

Logger::~Logger()
{
	RemoveOutputs();
	SetFormatter(NULL);
}

Logger &Logger::GetDefaultLogger()
{
	static Logger logger;

	return logger;
}

Status Logger::SetLevel(Level nLevel)
{
	m_nLevel = nLevel;

	return Status();
}

Level Logger::GetLevel() const
{
	return m_nLevel;
}

Status Logger::SetFormatter(IFormatter *pFormatter)
{
	Sys::Locker   locker(&m_fmLogger);

	if (NULL != m_pFormatter)
	{
		delete m_pFormatter;
		m_pFormatter = NULL;
	}
	m_pFormatter = pFormatter;

	return Status();
}

IFormatter *Logger::GetFormatter()
{
	Sys::Locker   locker(&m_fmLogger);

	return m_pFormatter;
}

Status Logger::AddOutput(IOutput *pOutput)
{
	Sys::Locker   locker(&m_fmLogger);

	m_vectorOutputs.push_back(pOutput);

	return Status();
}

Status Logger::RemoveOutputs()
{
	Sys::Locker   locker(&m_fmLogger);

	for (OutputsVector::iterator iter = m_vectorOutputs.begin(); 
	     iter != m_vectorOutputs.end(); ++iter)
	{
		delete *iter;
	}
	m_vectorOutputs.clear();

	return Status();
}

}//namespace Log

}//namespace CX

