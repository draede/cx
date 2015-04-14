/* 
 * CX - C++ framework for general purpose developement
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

	template <typename OUTPUT>
	static void PrintAllocs(OUTPUT out, const AllocsVector &vectorAllocs)
	{
		if (vectorAllocs.empty())
		{
			Print(out, "No allocations found!\n");
			return;
		}
		for (AllocsVector::const_iterator iter = vectorAllocs.begin(); iter != vectorAllocs.end(); ++iter)
		{
			static const Char hexdigits[] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F' };

			if (vectorAllocs.begin() != iter)
			{
				Print(out, "\n");
			}
#ifdef CX_64BIT_ARCH
			CX::Print(out, "Detected {1} bytes at 0x{2:'0'16}\n", iter->cbSize, (void *)iter->pMem);
#else
			CX::Print(out, "Detected {1} bytes at 0x{2:'0'8}\n", iter->cbSize, (void *)iter->pMem);
#endif

			String     sContent;
			Size       cIndex = 0;
			const Byte *pPos  = (const Byte *)iter->pMem;

			while (cIndex < iter->cbSize)
			{
				if (!sContent.empty())
				{
					sContent += "\n";
				}

				sContent += "[";

				for (Size i = 0; i < 16; i++)
				{
					if (cIndex + i < iter->cbSize)
					{
						if (32 > *(pPos + i) || 127 < *(pPos + i))
						{
							sContent += ".";
						}
						else
						{
							sContent += *(pPos + i);
						}
					}
					else
					{
						sContent += " ";
					}
				}

				sContent += "] [";

				for (Size i = 0; i < 16; i++)
				{
					if (cIndex + i < iter->cbSize)
					{
						if (0 < i)
						{
							sContent += " ";
						}
						sContent += hexdigits[(*(pPos + i)) / 16];
						sContent += hexdigits[(*(pPos + i)) % 16];
					}
					else
					{
						if (0 < i)
						{
							sContent += " ";
						}
						sContent += "  ";
					}
				}

				sContent += "]";

				cIndex += 16;
				pPos += 16;
			}

			Print(out, "{1}\n", sContent);

			Size cCount = iter->vectorCalls.size();

			if (0 == cCount)
			{
				Print(out, "<no stack trace available>\n");

				return;
			}
			for (Size i = 0; i < cCount; i++)
			{
				Print(out, "[{1}]: {2} - {3}:{4}\n", cCount - i - 1, 
				      iter->vectorCalls[i].sFunction, iter->vectorCalls[i].sFile, iter->vectorCalls[i].cLine);
			}
		}
	}

private:

	Mem();

	~Mem();

	static unsigned int   m_nFlags;

	static Sys::Lock *GetLock();

	static std::map<void *, MemAllocInfo> *GetAllocs();

};

}//CX 

