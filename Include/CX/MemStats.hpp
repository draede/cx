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
#include "CX/Status.hpp"
#include "CX/Print.hpp"
#include "CX/C/MemStats.h"
#include "CX/IObject.hpp"
#include "CX/Platform.hpp"
#include "CX/APIDefs.hpp"


namespace CX
{

class CX_API MemStats : public IObject
{
public:

	static Status Activate();

	static Bool IsActive();

	static Size GetCurrentAllocsSize();

	static Size GetCurrentAllocsCount();

	static Size GetMaxAllocsSize();

	static Size GetMaxAllocsCount();

	template <typename OUTPUT>
	static void PrintAllocs(OUTPUT out, 
	                        const Char *exceptions[] = NULL, Size cExceptions = 0, 
	                        const Char *ignoresUpper[] = NULL, Size cIgnoresUpper = 0, 
	                        const Char *ignoresLower[] = NULL, Size cIgnoresLower = 0)
	{
		MemInfo<OUTPUT> info(&out);

		CX_MemStats_GetAllocs(&MemStats::AllocStatsHandler<OUTPUT>, &info, exceptions, cExceptions, ignoresUpper, cIgnoresUpper, 
		                      ignoresLower, cIgnoresLower);
		if (0 < info.cAllocsCount)
		{
			Print(out, "Summary:\n{1} allocations found ({2} bytes)\n", info.cAllocsCount, info.cbAlocsSize);
		}
		else
		{
			Print(out, "No memory allocation found.\n");
		}
	}

private:

	static const Size MEM_PREVIEW_ROW_SIZE = 16;
	static const Size MEM_PREVIEW_MAX_SIZE = MEM_PREVIEW_ROW_SIZE * 4;

	template <typename OUTPUT>
	struct MemInfo
	{
		MemInfo(OUTPUT *o) : pOut(o)
		{
			cbAlocsSize  = 0;
			cAllocsCount = 0;
		}

		OUTPUT *pOut;
		Size   cAllocsCount;
		Size   cbAlocsSize;
	};

	MemStats();

	~MemStats();

	template <typename OUTPUT>
	static void AllocStatsHandler(const CX_MemAllocInfo *pInfo, void *pUsrCtx)
	{
		static const Char hexdigits[] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F' };

		MemInfo<OUTPUT> *pMemInfo = (MemInfo<OUTPUT> *)pUsrCtx;

		pMemInfo->cAllocsCount++;
		pMemInfo->cbAlocsSize += pInfo->cbSize;
#ifdef CX_64BIT_ARCH
		Print(*pMemInfo->pOut, "{1} bytes at 0x{2:'0'16}\n", pInfo->cbSize, pInfo->pPtr);
#else
		Print(*pMemInfo->pOut, "{1} bytes at 0x{2:'0'8}\n", pInfo->cbSize, pInfo->pPtr);
#endif

		std::string   sContent;
		Size          cIndex = 0;
		const Byte    *pPos = (const Byte *)pInfo->pPtr;

		while (cIndex < pInfo->cbSize && cIndex < MEM_PREVIEW_MAX_SIZE)
		{
			if (!sContent.empty())
			{
				sContent += "\n";
			}

			sContent += "[";

			for (Size i = 0; i < MEM_PREVIEW_ROW_SIZE; i++)
			{
				if (cIndex + i < pInfo->cbSize)
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

			for (Size i = 0; i < MEM_PREVIEW_ROW_SIZE; i++)
			{
				if (cIndex + i < pInfo->cbSize)
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

			cIndex += MEM_PREVIEW_ROW_SIZE;
			pPos += MEM_PREVIEW_ROW_SIZE;
		}

		Print(*pMemInfo->pOut, "Memory:\n{1}\n", sContent);

		Print(*pMemInfo->pOut, "Call stack:\n");
		for (Size i = 0; i < pInfo->cCallsCount; i++)
		{
			Print(*pMemInfo->pOut, "{1} - {2}:{3}\n", pInfo->calls[i].szFunction, pInfo->calls[i].szFile, 
			      pInfo->calls[i].cLineNumber);
		}

		Print(*pMemInfo->pOut, "\n");
	}

};

}//CX 

