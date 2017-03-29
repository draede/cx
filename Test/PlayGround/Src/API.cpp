/* 
 * CX - C++ framework for general purpose development
 *
 * https://github.com/draede/cx
 * 
 * Copyright (C) 2014 - 2017 draede - draede [at] outlook [dot] com
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
#include "CX/API/IObject.hpp"
#include "CX/API/Object.hpp"
#include "CX/Print.hpp"
#include "Tester.hpp"


using namespace CX;


class ICustomLoggerName : public IObject
{
public:

	CX_DECLARE_INTERFACE("ICustomLoggerName")

	virtual void Set(const char *szName) = 0;

	virtual const char *Get() const = 0;

};

class CustomLoggerName : public Object<CustomLoggerName, ICustomLoggerName>
{
public:

	virtual void Set(const char *szName)
	{
		m_sName = szName;
	}

	virtual const char *Get() const
	{
		return m_sName.c_str();
	}

private:

	String m_sName;

};

class IInfoLogger : public IObject
{
public:

	CX_DECLARE_INTERFACE("IInfoLogger")

	virtual void Log(const char *szMsg) = 0;

};

class InfoLogger : public Object<InfoLogger, IInfoLogger, CustomLoggerName>
{
public:

	virtual void Log(const char *szMsg)
	{
		ICustomLoggerName *pCustomLoggerName = (CustomLoggerName *)Acquire(ICustomLoggerName::OBJECT_NAME());

		Print(stdout, "{1}: {2}\n", pCustomLoggerName->Get(), szMsg);

		pCustomLoggerName->Release();
	}

	virtual void OnInitObject()
	{
		ICustomLoggerName *pCustomLoggerName = (CustomLoggerName *)Acquire(ICustomLoggerName::OBJECT_NAME());

		pCustomLoggerName->Set("INFO");

		pCustomLoggerName->Release();
	}

	InfoLogger()
	{
		Print(stdout, "InfoLogger::InfoLogger\n");
	}

	~InfoLogger()
	{
		Print(stdout, "InfoLogger::~InfoLogger\n");
	}

};

class IWarningLogger : public IObject
{
public:

	CX_DECLARE_INTERFACE("IWarningLogger")

	virtual void Log(const char *szMsg) = 0;

};

class WarningLogger : public Object<WarningLogger, IWarningLogger, CustomLoggerName>
{
public:

	virtual void Log(const char *szMsg)
	{
		ICustomLoggerName *pCustomLoggerName = (CustomLoggerName *)Acquire(ICustomLoggerName::OBJECT_NAME());

		Print(stdout, "{1}: {2}\n", pCustomLoggerName->Get(), szMsg);

		pCustomLoggerName->Release();
	}

	virtual void OnInitObject()
	{
		ICustomLoggerName *pCustomLoggerName = (CustomLoggerName *)Acquire(ICustomLoggerName::OBJECT_NAME());

		pCustomLoggerName->Set("WARNING");

		pCustomLoggerName->Release();
	}

	WarningLogger()
	{
		Print(stdout, "WarningLogger::WarningLogger\n");
	}

	~WarningLogger()
	{
		Print(stdout, "WarningLogger::~WarningLogger\n");
	}

};

class IErrorLogger : public IObject
{
public:

	CX_DECLARE_INTERFACE("IErrorLogger")

	virtual void Log(const char *szMsg) = 0;

};

class ErrorLogger : public Object<ErrorLogger, IErrorLogger, CustomLoggerName>
{
public:

	virtual void Log(const char *szMsg)
	{
		ICustomLoggerName *pCustomLoggerName = (CustomLoggerName *)Acquire(ICustomLoggerName::OBJECT_NAME());

		Print(stdout, "{1}: {2}\n", pCustomLoggerName->Get(), szMsg);

		pCustomLoggerName->Release();
	}

	virtual void OnInitObject()
	{
		ICustomLoggerName *pCustomLoggerName = (CustomLoggerName *)Acquire(ICustomLoggerName::OBJECT_NAME());

		pCustomLoggerName->Set("ERROR");

		pCustomLoggerName->Release();
	}

	ErrorLogger()
	{
		Print(stdout, "ErrorLogger::ErrorLogger\n");
	}

	~ErrorLogger()
	{
		Print(stdout, "ErrorLogger::~ErrorLogger\n");
	}

};

class ILogger : public IObject
{
public:

	CX_DECLARE_INTERFACE("ILogger")

	enum Level
	{
		Level_Info,
		Level_Warning,
		Level_Error,
	};

	virtual void Log(Level nLevel, const char *szMsg) = 0;

};

class Logger : public Object<Logger, ILogger, InfoLogger, WarningLogger, ErrorLogger>
{
public:

	virtual void Log(Level nLevel, const char *szMsg)
	{
		if (Level_Info == nLevel)
		{
			IInfoLogger *pLogger = (IInfoLogger *)Acquire(IInfoLogger::OBJECT_NAME());

			pLogger->Log(szMsg);

			pLogger->Release();
		}
		else
		if (Level_Warning == nLevel)
		{
			IWarningLogger *pLogger = (IWarningLogger *)Acquire(IWarningLogger::OBJECT_NAME());

			pLogger->Log(szMsg);

			pLogger->Release();
		}
		else
		if (Level_Error == nLevel)
		{
			IErrorLogger *pLogger = (IErrorLogger *)Acquire(IErrorLogger::OBJECT_NAME());

			pLogger->Log(szMsg);

			pLogger->Release();
		}
	}

	Logger()
	{
		Print(stdout, "Logger::Logger\n");
	}

	~Logger()
	{
		Print(stdout, "Logger::~Logger\n");
	}

};

void API_Test1()
{
	ILogger           *pLogger;
	IInfoLogger       *pInfoLogger;
	IWarningLogger    *pWarningLogger;
	IErrorLogger      *pErrorLogger;
	ICustomLoggerName *pCustomLoggerName;

	pLogger = Logger::Create();

	pInfoLogger    = pLogger->Acquire<IInfoLogger>();
	pWarningLogger = pLogger->Acquire<IWarningLogger>();
	pErrorLogger   = pLogger->Acquire<IErrorLogger>();

	pLogger->Log(ILogger::Level_Info, "info from main logger");
	pLogger->Log(ILogger::Level_Warning, "warning from main logger");
	pLogger->Log(ILogger::Level_Error, "error from main logger");

	pCustomLoggerName = pInfoLogger->Acquire<ICustomLoggerName>();
	pCustomLoggerName->Set("MYINFO");
	pCustomLoggerName->Release();

	pCustomLoggerName = pWarningLogger->Acquire<ICustomLoggerName>();
	pCustomLoggerName->Set("MYWARNING");
	pCustomLoggerName->Release();

	pCustomLoggerName = pErrorLogger->Acquire<ICustomLoggerName>();
	pCustomLoggerName->Set("MYERROR");
	pCustomLoggerName->Release();

	pInfoLogger->Log("info from info logger");
	pWarningLogger->Log("warning from warning logger");
	pErrorLogger->Log("error from error logger");

	pInfoLogger->Release();
	pWarningLogger->Release();
	pErrorLogger->Release();

	pLogger->Release();
}


REGISTER_TEST(API_Test1);
