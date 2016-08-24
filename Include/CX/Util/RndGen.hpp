/* 
 * CX - C++ framework for general purpose development
 *
 * https://github.com/draede/cx
 * 
 * Copyright (C) 2014 - 2016 draede - draede [at] outlook [dot] com
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
#include "CX/BLOB.hpp"
#include "CX/APIDefs.hpp"


namespace CX
{

namespace Util
{

class CX_API RndGen
{
public:

	RndGen();

	~RndGen();

	static RndGen &Get();
	
	void Copy(const RndGen &rndgen);

	UInt8 GetUInt8();

	UInt8 GetUInt8Range(UInt8 nMin, UInt8 nMax);

	UInt16 GetUInt16();

	UInt16 GetUInt16Range(UInt16 nMin, UInt16 nMax);

	UInt32 GetUInt32();

	UInt32 GetUInt32Range(UInt32 nMin, UInt32 nMax);

	UInt64 GetUInt64();

	UInt64 GetUInt64Range(UInt64 nMin, UInt64 nMax);

	Size GetSize();

	Size GetSizeRange(Size nMin, Size nMax);

	Bool GetBool();

	Float GetFloat();

	Double GetDouble();

	Char GetChar(const Char *szCharset = NULL);

	WChar GetWChar(const WChar *wszCharset = NULL);

	Byte GetByte(Byte *pByteSet = NULL, Size cByteSetSize = 0);

	void GetString(Char *pStr, Size cSize, const Char *szCharset = NULL);

	void GetWString(WChar *pwStr, Size cSize, const WChar *wszCharset = NULL);

	void GetBytes(Byte *pBuffer, Size cSize, Byte *pByteSet = NULL, Size cByteSetSize = 0);

	void GetString(String *psStr, Size cSize, const Char *szCharset = NULL);

	void GetWString(WString *pwsStr, Size cSize, const WChar *wszCharset = NULL);

	void GetBytes(BLOB *pBuffer, Size cSize, Byte *pByteSet = NULL, Size cByteSetSize = 0);

	//===

	void Seed32(UInt32 nSeed);

	void Seed64(UInt64 nSeed);

private:

	void *m_pState64;
	void *m_pState32;

	UInt32 GetRange32(UInt32 cValues);

	UInt64 GetRange64(UInt64 cValues);

};

}//namespace Util

}//namespace CX

