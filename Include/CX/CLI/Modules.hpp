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
 
#pragma once


#include "CX/Types.hpp"
#include "CX/Status.hpp"
#include "CX/String.hpp"
#include "CX/Map.hpp"
#include "CX/CLI/IModule.hpp"
#include "CX/CLI/ICommand.hpp"


namespace CX
{

namespace CLI
{

class Modules
{
public:

	static Modules &Get();

	Status SetAppName(const Char *szAppName);

	const Char *GetAppName();

	Status SetAppBanner(const Char *szAppBanner);

	const Char *GetAppBanner();

	Status SetUsagePrefix(const Char *szUsagePrefix);

	const Char *GetUsagePrefix();

	Status RegisterModule(const Char *szModuleName, const Char *szModuleDescription, IModule::Factory pfnFactory);

	Status RegisterCommand(const Char *szModuleName, const Char *szCommandName, const Char *szCommandDescription, 
	                       ICommand::Factory pfnFactory);

	IModule *CreateModule(const Char *szModuleName);

	ICommand *CreateCommand(const Char *szModuleName, const Char *szCommandName);

	Status ShowUsage();

	Status ShowModuleUsage(const Char *szModuleName);

	Status Run(int argc, Char *argv[], const Char *szNoArgs = "ERROR: No arguments provided", 
	           const Char *szModuleNotFound = "ERROR: Unknown module '{1}'", 
	           const Char *szCommandNotFound = "ERROR: Unknown command '{1}' in module {2}");

private:

	String   m_sAppName;
	String   m_sAppBanner;
	String   m_sUsagePrefix;

	struct Command
	{
		String              sDescription;
		ICommand::Factory   pfnFactory;
	};

	typedef Map<String, Command>::Type   CommandsMap;

	struct Module
	{
		String             sDescription;
		IModule::Factory   pfnFactory;
		CommandsMap        mapCommands;
	};

	typedef Map<String, Module>::Type   ModulesMap;

	ModulesMap   m_mapModules;

	Modules();

	~Modules();

};

}//namespace CLI

}//namespace CX
