/* 
 * CX - C++ framework for general purpose development
 *
 * https://github.com/draede/cx
 * 
 * Copyright (C) 2014 - 2018 draede - draede [at] outlook [dot] com
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
#include "CX/CLI/Modules.hpp"
#include "CX/Print.hpp"


namespace CX
{

namespace CLI
{

Modules::Modules()
{

}

Modules::~Modules()
{
}

Modules &Modules::Get()
{
	static Modules modules;

	return modules;
}

Status Modules::SetAppName(const Char *szAppName)
{
	m_sAppName = szAppName;

	return Status();
}

const Char *Modules::GetAppName()
{
	return m_sAppName.c_str();
}

Status Modules::SetAppBanner(const Char *szAppBanner)
{
	m_sAppBanner = szAppBanner;

	return Status();
}

const Char *Modules::GetAppBanner()
{
	return m_sAppBanner.c_str();
}

Status Modules::SetUsagePrefix(const Char *szUsagePrefix)
{
	m_sUsagePrefix = szUsagePrefix;

	return Status();
}

const Char *Modules::GetUsagePrefix()
{
	return m_sUsagePrefix.c_str();
}

Status Modules::RegisterModule(const Char *szModuleName, const Char *szModuleDescription, IModule::Factory pfnFactory)
{
	auto iterModules = m_mapModules.find(szModuleName);

	if (m_mapModules.end() != iterModules)
	{
		iterModules->second.sDescription = szModuleDescription;
		iterModules->second.pfnFactory   = pfnFactory;
	}
	else
	{
		Module   module;

		module.sDescription = szModuleDescription;
		module.pfnFactory   = pfnFactory;

		m_mapModules[szModuleName] = module;
	}

	return Status();
}

Status Modules::RegisterCommand(const Char *szModuleName, const Char *szCommandName, const Char *szCommandDescription, 
                                ICommand::Factory pfnFactory)
{
	auto iterModules = m_mapModules.find(szModuleName);

	if (m_mapModules.end() == iterModules)
	{
		Module   module;

		module.sDescription = "";
		module.pfnFactory   = NULL;

		m_mapModules[szModuleName] = module;

		iterModules = m_mapModules.find(szModuleName);
	}

	auto iterCommands = iterModules->second.mapCommands.find(szCommandName);

	if (iterModules->second.mapCommands.end() != iterCommands)
	{
		return Status_Duplicate;
	}

	Command   command;

	command.sDescription = szCommandDescription;
	command.pfnFactory   = pfnFactory;

	iterModules->second.mapCommands[szCommandName] = command;

	return Status();
}

IModule *Modules::CreateModule(const Char *szModuleName)
{
	auto iterModules = m_mapModules.find(szModuleName);

	if (m_mapModules.end() == iterModules)
	{
		return NULL;
	}
	if (NULL == iterModules->second.pfnFactory)
	{
		return NULL;
	}

	return iterModules->second.pfnFactory();
}

ICommand *Modules::CreateCommand(const Char *szModuleName, const Char *szCommandName)
{
	auto iterModules = m_mapModules.find(szModuleName);

	if (m_mapModules.end() == iterModules)
	{
		return NULL;
	}

	auto iterCommands = iterModules->second.mapCommands.find(szCommandName);

	if (iterModules->second.mapCommands.end() == iterCommands)
	{
		return NULL;
	}
	if (NULL == iterCommands->second.pfnFactory)
	{
		return NULL;
	}

	return iterCommands->second.pfnFactory();
}

Status Modules::ShowUsage()
{
	Print(stdout, "{1}\n", m_sAppBanner);
	Print(stdout, "{1}: {2} <module> <command> [arguments]\n", m_sUsagePrefix, m_sAppName);
	for (auto iterModules = m_mapModules.begin(); iterModules != m_mapModules.end(); ++iterModules)
	{
		Print(stdout, " - {1} : {2}\n", iterModules->first, iterModules->second.sDescription);
		for (auto iterCommands = iterModules->second.mapCommands.begin();
		     iterCommands != iterModules->second.mapCommands.end(); ++iterCommands)
		{
			Print(stdout, "    - {1} : {2}\n", iterCommands->first, iterCommands->second.sDescription);
		}
	}

	return Status();
}

Status Modules::ShowModuleUsage(const Char *szModuleName)
{
	auto iterModules = m_mapModules.find(szModuleName);

	if (m_mapModules.end() == iterModules)
	{
		return Status_NotFound;
	}

	Print(stdout, "{1}\n", m_sAppBanner);
	Print(stdout, "{1}: {2} <module> <command> [arguments]\n", m_sUsagePrefix, m_sAppName);
	Print(stdout, " - {1} : {2}\n", iterModules->first, iterModules->second.sDescription);
	for (auto iterCommands = iterModules->second.mapCommands.begin();
	     iterCommands != iterModules->second.mapCommands.end(); ++iterCommands)
	{
		Print(stdout, "    - {1} : {2}\n", iterCommands->first, iterCommands->second.sDescription);
	}

	return Status();
}

Status Modules::Run(int argc, Char *argv[], const Char *szNoArgs/* = "ERROR: No arguments provided"*/, 
                    const Char *szModuleNotFound/* = "ERROR: Unknown module '{1}'"*/,
                    const Char *szCommandNotFound/* = "ERROR: Unknown command '{1}' in module {2}"*/)
{
	CLI::IModule    *pModule = NULL;
	CLI::ICommand   *pCommand = NULL;
	Status          status;

	for (;;)
	{
		if (1 == argc)
		{
			ShowUsage();
			Print(stdout, "\n");
			status = Status(Status_InvalidArg, "{1}", szNoArgs);

			break;
		}

		argc--;
		argv++;

		if (NULL == (pModule = CLI::Modules::Get().CreateModule(argv[0])))
		{
			ShowUsage();
			Print(stdout, "\n");
			status = Status(Status_InvalidArg, szModuleNotFound, argv[0]);

			break;
		}
		if (!(status = pModule->Init()))
		{
			break;
		}
		if (1 == argc)
		{
			pModule->ShowUsage();

			break;
		}
		if (NULL == (pCommand = CLI::Modules::Get().CreateCommand(argv[0], argv[1])))
		{
			ShowModuleUsage(argv[0]);
			Print(stdout, "\n");
			status = Status(Status_InvalidArg, szCommandNotFound, argv[1], argv[0]);

			break;
		}
		if (!(status = pCommand->Init(argc - 2, argv + 2)))
		{
			break;
		}
		if (!(status = pCommand->Run()))
		{
			break;
		}

		break;
	}
	if (NULL != pCommand)
	{
		pCommand->Uninit();
		pCommand->Destroy();
		pCommand = NULL;
	}
	if (NULL != pModule)
	{
		pModule->Uninit();
		pModule->Destroy();
		pModule = NULL;
	}
	if (!status)
	{
		Print(stdout, "{1}\n", status.GetMsg());
	}

	return status;
}

}//namespace CLI

}//namespace CX

