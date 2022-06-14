/* 
 * CX - C++ framework for general purpose development
 *
 * https://github.com/draede/cx
 * 
 * Copyright (C) 2014 - 2022 draede
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
#include "CX/Print.hpp"


namespace CX
{

namespace MemPack
{

#pragma pack(push)
#pragma pack(1)
struct Block
{
	static const UInt32   INVALID_OFFSET           = 0;

	static const UInt32   USED_BITMASK             = 0b1000'0000'0000'0000'0000'0000'0000'0000;
	static const UInt32   PREV_USED_BITMASK        = 0b0100'0000'0000'0000'0000'0000'0000'0000;

	static const UInt32   SIZE_BITMASK             = 0b0011'1111'1111'1111'1111'1111'1111'1111;

	static const UInt32   USED_MIN_SIZE            = sizeof(UInt32); 
	                                                 //header
	static const UInt32   UNUSED_MIN_SIZE          = sizeof(UInt32) + sizeof(UInt32) + sizeof(UInt32); 
	                                                 //header       + next           + footer

	static const UInt32   MAX_SIZE                 = 0b0011'1111'1111'1111'1111'1111'1111'1111;

	//max size is also the max size of the storage
	//max size will be restricted more by upper layers....

	UInt32   header;
	UInt32   cbNextOffset; //valid only if this block is unused

	//header methods

	inline Bool IsUsed()
	{
		return (USED_BITMASK == (header & USED_BITMASK));
	}

	inline void SetUsed()
	{
		header |= USED_BITMASK;
	}

	inline Bool IsUnused()
	{
		return (0 == (header & USED_BITMASK));
	}

	inline void SetUnused()
	{
		header &= ~USED_BITMASK;
	}

	inline Bool IsPrevBlockUsed()
	{
		return (PREV_USED_BITMASK == (header & PREV_USED_BITMASK));
	}

	inline void SetPrevBlockUsed()
	{
		header |= PREV_USED_BITMASK;
	}

	inline Bool IsPrevBlockUnused()
	{
		return (0 == (header & PREV_USED_BITMASK));
	}

	inline void SetPrevBlockUnused()
	{
		header &= ~PREV_USED_BITMASK;
	}

	inline UInt32 GetSize()
	{
		return (header & SIZE_BITMASK);
	}

	inline void SetSize(UInt32 cbSize)
	{
		header = (header & USED_BITMASK) | (header & PREV_USED_BITMASK) | (cbSize & SIZE_BITMASK);
	}

	inline void SetUsedSize(UInt32 cbSize)
	{
		header = USED_BITMASK | (header & PREV_USED_BITMASK) | (cbSize & SIZE_BITMASK);
	}

	inline void SetUnusedSize(UInt32 cbSize)
	{
		header = (header & PREV_USED_BITMASK) | (cbSize & SIZE_BITMASK);
	}

	inline void SetUsedPrevBlockUsed()
	{
		header = USED_BITMASK | PREV_USED_BITMASK | (header & SIZE_BITMASK);
	}

	inline void SetUsedPrevBlockUnused()
	{
		header = USED_BITMASK | (header & SIZE_BITMASK);
	}

	inline void SetUnusedPrevBlockUsed()
	{
		header = PREV_USED_BITMASK | (header & SIZE_BITMASK);
	}

	inline void SetUnusedPrevBlockUnused()
	{
		header = (header & SIZE_BITMASK);
	}

	inline void SetUsedPrevBlockUsedSize(UInt32 cbSize)
	{
		header = USED_BITMASK | PREV_USED_BITMASK | (cbSize & SIZE_BITMASK);
	}

	inline void SetUsedPrevBlockUnusedSize(UInt32 cbSize)
	{
		header = USED_BITMASK | (cbSize & SIZE_BITMASK);
	}

	inline void SetUnusedPrevBlockUsedSize(UInt32 cbSize)
	{
		header = PREV_USED_BITMASK | (cbSize & SIZE_BITMASK);
	}

	inline void SetUnusedPrevBlockUnusedSize(UInt32 cbSize)
	{
		header = (cbSize & SIZE_BITMASK);
	}

	//footer methods - footer contains only the size

	inline void SetFooterSize(UInt32 cbSize)
	{
		*(UInt32 *)((Byte *)this + cbSize - sizeof(UInt32)) = cbSize;
	}

	//call this only after SetSize was called!
	inline void SetFooterSize()
	{
		return SetFooterSize(GetSize());
	}

};
#pragma pack(pop)

}//namespace MemPack

}//namespace CX
