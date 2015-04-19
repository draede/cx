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


#if defined(CX_OS_WINDOWS)


#include "CX/Types.hpp"
#include "CX/Status.hpp"
#include "CX/APIDefs.hpp"
#include "CX/IObject.hpp"


namespace CX
{

namespace Util
{

class CX_API RndGen : public IObject
{
public:

	static RndGen &Get();

	Bool GetBool();

	Char GetChar(const Char *szCharset = NULL);

	WChar GetWChar(const WChar *wszCharset = NULL);

	Char GetCharRange(Char nMin, Char nMax);

	Int8 GetInt8();

	Int8 GetInt8Range(Int8 nMin, Int8 nMax);

	UInt8 GetUInt8();

	UInt8 GetUInt8Range(UInt8 nMin, UInt8 nMax);

	Int16 GetInt16();

	Int16 GetInt16Range(Int16 nMin, Int16 nMax);

	UInt16 GetUInt16();

	UInt16 GetUInt16Range(UInt16 nMin, UInt16 nMax);

	Int32 GetInt32();

	Int32 GetInt32Range(Int32 nMin, Int32 nMax);

	UInt32 GetUInt32();

	UInt32 GetUInt32Range(UInt32 nMin, UInt32 nMax);

	Int64 GetInt64();

	Int64 GetInt64Range(Int64 nMin, Int64 nMax);

	UInt64 GetUInt64();

	UInt64 GetUInt64Range(UInt64 nMin, UInt64 nMax);

	Float GetFloat();

	Double GetDouble();

	Size GetSize();

	UInt8 GetSizeRange(Size nMin, Size nMax);

	void GetString(String *psStr, Size cMinLen, Size cMaxLen, const Char *szCharset = NULL);

	void GetWString(WString *psStr, Size cMinLen, Size cMaxLen, const WChar *wszCharset = NULL);

	const Char *GetString(Size cMinLen, Size cMaxLen, const Char *szCharset = NULL);

	const WChar *GetWString(Size cMinLen, Size cMaxLen, const WChar *wszCharset = NULL);

	//===

	void Seed32(UInt32 nSeed);

	void Seed64(UInt64 nSeed);

private:

	void *m_pState64;
	void *m_pState32;

	RndGen();

	~RndGen();

};

}//namespace Util

}//namespace CX


#endif

