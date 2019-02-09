/* 
 * CX - C++ framework for general purpose development
 *
 * https://github.com/draede/cx
 * 
 * Copyright (C) 2014 - 2019 draede - draede [at] outlook [dot] com
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
#include "CX/CLI/Commands.hpp"
#include "CX/Print.hpp"


namespace CX
{

namespace CLI
{

Commands::Commands()
{

}

Commands::~Commands()
{
}

Commands &Commands::Get()
{
	static Commands Commands;

	return Commands;
}

Status Commands::SetAppName(const Char *szAppName)
{
	m_sAppName = szAppName;

	return Status();
}

const Char *Commands::GetAppName()
{
	return m_sAppName.c_str();
}

Status Commands::SetAppBanner(const Char *szAppBanner)
{
	m_sAppBanner = szAppBanner;

	return Status();
}

const Char *Commands::GetAppBanner()
{
	return m_sAppBanner.c_str();
}

Status Commands::SetUsagePrefix(const Char *szUsagePrefix)
{
	m_sUsagePrefix = szUsagePrefix;

	return Status();
}

const Char *Commands::GetUsagePrefix()
{
	return m_sUsagePrefix.c_str();
}

Status Commands::RegisterCommand(const Char *szName, const Char *szDescription, ICommand::Factory pfnFactory)
{
	auto iterCommands = m_mapCommands.find(szName);

	if (m_mapCommands.end() != iterCommands)
	{
		iterCommands->second.sName        = szName;
		iterCommands->second.sDescription = szDescription;
		iterCommands->second.pfnFactory   = pfnFactory;
	}
	else
	{
		Command   cmd;

		cmd.sName        = szName;
		cmd.sDescription = szDescription;
		cmd.pfnFactory   = pfnFactory;

		m_mapCommands[szName] = cmd;
	}

	return Status();
}

Status Commands::Run(int argc, Char *argv[], const Char *szNoArgs/* = "ERROR: No arguments provided"*/, 
                     const Char *szCommandNotFound/* = "ERROR: Unknown module '{1}'"*/)
{
	ICommand   *pCommand        = NULL;
	Bool       bInitialized = False;
	Status     status;

	for (;;)
	{
		if (1 >= argc)
		{
			ShowUsage();
			status = Status(Status_InvalidArg, "{1}", szNoArgs);

			break;
		}

		argc--;
		argv++;

		auto iter = m_mapCommands.find(argv[0]);

		if (m_mapCommands.end() == iter)
		{
			ShowUsage();
			status = Status(Status_NotFound, szCommandNotFound, argv[0]);

			break;
		}

		if (NULL == (pCommand = iter->second.pfnFactory()))
		{
			ShowUsage();
			status = Status(Status_OperationFailed, "Failed to create '{1}' command", argv[0]);

			break;
		}

		argc--;
		argv++;

		if (!(status = pCommand->Init(argc, (const Char **)argv)))
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
		if (bInitialized)
		{
			pCommand->Uninit();
		}
		pCommand->Destroy();
		pCommand = NULL;
	}

	return status;
}

void Commands::ShowUsage()
{
	Print(stdout, "{1}\n", m_sAppBanner);
	Print(stdout, "{1}: {2} <command>\n", m_sUsagePrefix, m_sAppName);
	for (auto iter = m_mapCommands.begin(); iter != m_mapCommands.end(); ++iter)
	{
		Print(stdout, "  {1} : {2}\n", iter->second.sName, iter->second.sDescription);
	}
}

}//namespace CLI

}//namespace CX

