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
#include "CX/API/IInterface.hpp"
#include "CX/API/Object.hpp"
#include "CX/Print.hpp"
#include "Tester.hpp"


using namespace CX;


class IInfoLogger : public IInterface
{
public:

	CX_DECLARE_INTERFACE("IInfoLogger")

	virtual void Log(const char *szMsg) = 0;

};

class IWarningLogger : public IInterface
{
public:

	CX_DECLARE_INTERFACE("IWarningLogger")

	virtual void Log(const char *szMsg) = 0;

};

class IErrorLogger : public IInterface
{
public:

	CX_DECLARE_INTERFACE("IErrorLogger")

	virtual void Log(const char *szMsg) = 0;

};

class Logger : public Object<Logger, IInfoLogger, IWarningLogger, IErrorLogger>
{
public:

	Logger()
	{
		Print(stdout, "Logger::Logger\n");
	}

	virtual ~Logger()
	{
		Print(stdout, "Logger::~Logger\n");
	}

	virtual void IInfoLogger::Log(const char *szMsg)
	{
		Print(stdout, "INFO: {1}\n", szMsg);
	}

	virtual void IWarningLogger::Log(const char *szMsg)
	{
		Print(stdout, "WARNING: {1}\n", szMsg);
	}

	virtual void IErrorLogger::Log(const char *szMsg)
	{
		Print(stdout, "ERROR: {1}\n", szMsg);
	}

};

void API_Test1()
{
	IObject *pLogger = (IObject *)Logger::Create();

	IInfoLogger *pInfoLogger = pLogger->Acquire<IInfoLogger>();

	IWarningLogger *pWarningLogger = pLogger->Acquire<IWarningLogger>();

	IErrorLogger *pErrorLogger = pLogger->Acquire<IErrorLogger>();

	pInfoLogger->Log("info log");

	pWarningLogger->Log("warning log");

	pErrorLogger->Log("error log");

	pErrorLogger->Release();

	pInfoLogger->Release();

	pWarningLogger->Release();

	pLogger->Release();
}


REGISTER_TEST(API_Test1);
