/* 
 * CX - C++ framework for general purpose development
 *
 * https://github.com/draede/cx
 * 
 * Copyright (C) 2014-2015 draede - draede [at] outlook [dot] com
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


#include "CX/Platform.hpp"


#if defined(CX_OS_ANDROID)


#include "CX/Types.hpp"
#include "CX/Status.hpp"
#include "CX/IObject.hpp"
#include "CX/String.hpp"
#include "CX/Vector.hpp"
#include "CX/Print.hpp"
#include "CX/APIDefs.hpp"


namespace CX
{

namespace Util
{

class CX_API StackTrace : public IObject
{
public:

	static const Size MAX_STACK_ENTRIES = 256;
	static const Size MAX_SYMBOL_NAME   = 1023;

	enum Flags
	{
		Flag_IgnoreRuntimeInitCalls = 1,
	};

	typedef struct _Call
	{
		std::string   sFunction;
		std::string   sFile;
		Size          cLine;
	}Call;

	typedef std::vector<Call>   CallsVector;

	static Status GetStackTrace(CallsVector &vectorCalls, Size cMaxEntries = MAX_STACK_ENTRIES, 
	                            unsigned int nFlags = Flag_IgnoreRuntimeInitCalls);

	template <typename OUTPUT>
	static void PrintStackTrace(OUTPUT out, const CallsVector &vectorCalls)
	{
		Size cCount = vectorCalls.size();

		if (0 == cCount)
		{
			Print(out, "<no stack trace available>\n");

			return;
		}
		for (Size i = 0; i < cCount; i++)
		{
			Print(out, "[{1}]: {2} - {3}:{4}\n", cCount - i - 1, vectorCalls[i].sFunction, vectorCalls[i].sFile, 
			      vectorCalls[i].cLine);
		}
	}

private:

	StackTrace();

	~StackTrace();

};

}//namespace Util

}//CX


#endif

