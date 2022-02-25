/* 
 * CX - C++ framework for general purpose development
 *
 * https://github.com/draede/cx
 * 
 * Copyright (C) 2014 - 2022 draede - draede [at] outlook [dot] com
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
#include "CX/MemPack/Debug.hpp"


namespace CX
{

namespace MemPack
{

Profiler::SlotData   Profiler::slots[Slot_MAX]
{
	{ "Alloc                            " }, //Slot_Alloc,
	{ "Realloc                          " }, //Slot_Realloc,
	{ "Free                             " }, //Slot_Free,
	{ "Unused_Add                       " }, //Slot_Unused_Add,
	{ "Unused_Remove                    " }, //Slot_Unused_Remove,
	{ "Unused_RemoveBestFit             " }, //Slot_Unused_RemoveBestFit,
	{ "Unused_RemoveBestFitFromLastBlock" }, //Slot_Unused_RemoveBestFitFromLastBlock,
	{ "Unused_GetVarBlocksIndexFromSize " }, //Slot_Unused_GetVarBlocksIndexFromSize,
};

}//namespace MemPack

}//namespace CX
