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
#include "CX/API/InterfaceHelper.hpp"
#include "CX/Print.hpp"
#include "Tester.hpp"


using namespace CX;


class ITestInterface : public IInterface
{
public:

	CX_DECLARE_INTERFACE("ITestInterface")

	virtual void Test() = 0;

};

class IInfoLogger : public IInterface
{
public:

	CX_DECLARE_INTERFACE("IInfoLogger")

	virtual void Log(const char *szMsg) = 0;

};

class IErrorLogger : public IInterface
{
public:

	CX_DECLARE_INTERFACE("IErrorLogger")

	virtual void Log(const char *szMsg) = 0;

};

class ILogger : public IInterface
{
public:

	CX_DECLARE_INTERFACE("ILogger")

};

class TestInterface : public InterfaceHelper<ITestInterface>
{
public:

	CX_BEGIN_INTERFACE(TestInterface)
	CX_END_INTERFACE()

	virtual void Test()
	{
		Print(stdout, "TEST\n");
	}

protected:

	TestInterface()
	{
		Print(stdout, "TestInterface::TestInterface\n");
	}

	virtual ~TestInterface()
	{
		Print(stdout, "TestInterface::~TestInterface\n");
	}

};

class InfoLogger : public InterfaceHelper<IInfoLogger>
{
public:

	CX_BEGIN_INTERFACE(InfoLogger)
		CX_IMPLEMENT_INTERFACE(TestInterface);
	CX_END_INTERFACE()

	virtual void Log(const char *szMsg)
	{
		Print(stdout, "INFO: {1}\n", szMsg);
	}

protected:

	InfoLogger()
	{
		Print(stdout, "InfoLogger::InfoLogger\n");
	}

	virtual ~InfoLogger()
	{
		Print(stdout, "InfoLogger::~InfoLogger\n");
	}

};

class ErrorLogger : public InterfaceHelper<IErrorLogger>
{
public:

	CX_BEGIN_INTERFACE(ErrorLogger)
	CX_END_INTERFACE()

	virtual void Log(const char *szMsg)
	{
		Print(stdout, "ERROR: {1}\n", szMsg);
	}

protected:

	ErrorLogger()
	{
		Print(stdout, "ErrorLogger::ErrorLogger\n");
	}

	virtual ~ErrorLogger()
	{
		Print(stdout, "ErrorLogger::~ErrorLogger\n");
	}

};

class Logger : public InterfaceHelper<ILogger>
{
public:

	CX_BEGIN_INTERFACE(Logger)
		CX_IMPLEMENT_INTERFACE(InfoLogger);
		CX_IMPLEMENT_INTERFACE(ErrorLogger);
	CX_END_INTERFACE()

protected:

	Logger()
	{
		Print(stdout, "Logger::Logger\n");
	}

	virtual ~Logger()
	{
		Print(stdout, "Logger::~Logger\n");
	}

};

void API_Test1()
{
	ILogger *pLogger = (ILogger *)Logger::Create();

	IInfoLogger *pInfoLogger = (IInfoLogger *)pLogger->Acquire(IInfoLogger::NAME());

	IErrorLogger *pErrorLogger = (IErrorLogger *)pLogger->Acquire(IErrorLogger::NAME());

	pErrorLogger->Log("error log");

	pInfoLogger->Log("info log");

	ITestInterface *pTestInterface = (ITestInterface *)pInfoLogger->Acquire(ITestInterface::NAME());

	pTestInterface->Test();

	pTestInterface->Release();

	pErrorLogger->Release();

	pInfoLogger->Release();

	pLogger->Release();
}


REGISTER_TEST(API_Test1);
