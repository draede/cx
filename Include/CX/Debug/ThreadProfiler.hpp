/* 
 * CX - C++ framework for general purpose development
 *
 * https://github.com/draede/cx
 * 
 * Copyright (C) 2014 - 2018 draede, draede [at] outlook [dot] com
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
#include "CX/Util/Timer.hpp"


namespace CX
{

namespace Debug
{

//this looks like this because of VS2013...
class ThreadProfiler
{
public:

	struct Scope
	{
		Char          *szFileName;
		Char          *szScopeName;
		int           cLineNo;

		UInt64        cMinDuration;
		UInt64        cMaxDuration;
		UInt64        cTotalDuration;
		UInt64        cCalls;

		Util::Timer   timer;

		Scope         *pParent;
		Scope         *pFirstChild;
		Scope         *pPrevSibling;
		Scope         *pNextSibling;
	};

	static void EnterScope(const Char *szFileName, const Char *szScopeName, int cLineNo);

	static void LeaveScope();

	static Scope *CreateScope(const Char *szFileName, const Char *szScopeName, int cLineNo);

	static void DestroyScope(Scope *pScope);

	static bool MatchScope(Scope *pScope, const Char *szFileName, const Char *szScopeName, int cLineNo);

	static bool MatchScope(Scope *pScope1, Scope *pScope2);

};

}//namespace Sys

}//namespace CX
