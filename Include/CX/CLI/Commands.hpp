/* 
 * CX - C++ framework for general purpose development
 *
 * https://github.com/draede/cx
 * 
 * Copyright (C) 2014 - 2021 draede - draede [at] outlook [dot] com
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
#include "CX/String.hpp"
#include "CX/Print.hpp"
#include "CX/Map.hpp"
#include "CX/CLI/ICommand.hpp"


namespace CX
{

namespace CLI
{

template <typename CHAR_TYPE, typename STRING_TYPE>
class CommandsBase
{
public:

	static CommandsBase &Get()
	{
		static CommandsBase   commands;

		return commands;
	}

	Status SetAppName(const CHAR_TYPE *szAppName)
	{
		m_sAppName = szAppName;

		return Status();
	}

	const CHAR_TYPE *GetAppName()
	{
		return m_sAppName.c_str();
	}

	Status SetAppBanner(const CHAR_TYPE *szAppBanner)
	{
		m_sAppBanner = szAppBanner;

		return Status();
	}

	const CHAR_TYPE *GetAppBanner()
	{
		return m_sAppBanner.c_str();
	}

	Status SetUsagePrefix(const CHAR_TYPE *szUsagePrefix)
	{
		m_sUsagePrefix = szUsagePrefix;

		return Status();
	}

	const CHAR_TYPE *GetUsagePrefix()
	{
		return m_sUsagePrefix.c_str();
	}

	Status RegisterCommand(const CHAR_TYPE *szName, const CHAR_TYPE *szDescription, 
	                       typename ICommandBase<CHAR_TYPE>::Factory pfnFactory)
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

	Status Run(int argc, CHAR_TYPE *argv[], const Char *szNoArgs = "ERROR: No arguments provided", 
	           const Char *szCommandNotFound = "ERROR: Unknown command '{1}")
	{
		ICommandBase<CHAR_TYPE>   *pCommand    = NULL;
		Bool                      bInitialized = False;
		Status                    status;

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

			if (!(status = pCommand->Init(argc, (const CHAR_TYPE **)argv)))
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

	void ShowUsage()
	{
		Print(stdout, "{1}\n", m_sAppBanner);
		Print(stdout, "{1}: {2} <command>\n", m_sUsagePrefix, m_sAppName);
		for (auto iter = m_mapCommands.begin(); iter != m_mapCommands.end(); ++iter)
		{
			Print(stdout, "  {1} : {2}\n", iter->second.sName, iter->second.sDescription);
		}
	}

private:

	STRING_TYPE   m_sAppName;
	STRING_TYPE   m_sAppBanner;
	STRING_TYPE   m_sUsagePrefix;

	struct Command
	{
		STRING_TYPE                                 sName;
		STRING_TYPE                                 sDescription;
		typename ICommandBase<CHAR_TYPE>::Factory   pfnFactory;
	};

	typedef typename Map<STRING_TYPE, Command>::Type   CommandsMap;

	CommandsMap   m_mapCommands;

	CommandsBase()
	{
	}

	~CommandsBase()
	{
	}

};

typedef CommandsBase<Char, String>     Commands;
typedef CommandsBase<WChar, WString>   CommandsW;

}//namespace CLI

}//namespace CX
