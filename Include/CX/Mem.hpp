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


#include "CX/Types.hpp"
#include "CX/IObject.hpp"
#include <string>
#include <map>
#include <vector>
#include "CX/Platform.hpp"
#include "CX/APIDefs.hpp"


namespace CX
{

namespace Sys
{

class Lock;

}//namespace Sys


class MemAllocInfo;

class CX_API Mem : public IObject
{
public:

	enum Flags
	{
		Flag_None           = 0,    //default on release builds
		Flag_SimpleMemTrack = 1,    //default on debug builds - no stack traces
		Flag_SourceMemTrack = 2,    //show stack traces - uses more memory internally & slower
	};

	typedef struct _Call
	{
		std::string sFunction;
		std::string sFile;
		Size        cLine;
	}Call;

	typedef std::vector<Call>   CallsVector;

	typedef struct _AllocData
	{
		void          *pMem;
		Size          cbSize;
		CallsVector   vectorCalls;
	}AllocData;

	typedef std::vector<AllocData>   AllocsVector;

	static void *Alloc(Size cbSize);

	static void *Realloc(void *pPtr, Size cbSize);

	static void Free(void *pPtr);

	static void SetFlags(unsigned int nFlags);

	static unsigned int GetFlags();

	static void GetCurrentAllocs(AllocsVector &vectorAllocs);

	static void PrintAllocs(std::string &sOut, const AllocsVector &vectorAllocs);

private:

	Mem();

	~Mem();

	static unsigned int   m_nFlags;

	static Sys::Lock *GetLock();

	static std::map<void *, MemAllocInfo> *GetAllocs();

};

}//CX 

