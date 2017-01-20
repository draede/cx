/* 
 * CX - C++ framework for general purpose development
 *
 * https://github.com/draede/cx
 * 
 * Copyright (C) 2014 - 2017 draede - draede [at] outlook [dot] com
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
#include "CX/Vector.hpp"
#include "CX/C/stdarg.h"


namespace CX
{

namespace DB
{

namespace SQLite
{

class Bindings
{
public:

	enum ArgType
	{
		ArgType_Null,
		ArgType_Int,
		ArgType_Real,
		ArgType_String,
		ArgType_WString,
		ArgType_BLOB,
		ArgType_ZeroBLOB,
	};

	Bindings();

	//n = null (no extra args provided)
	//i = Int64
	//r = Double
	//s = String
	//w = WString
	//b = const void *, Size
	//z = Size
	Bindings(const Char *szArgsType, ...);

	//n = null (no extra args provided)
	//i = Int64
	//r = Double
	//s = String
	//w = WString
	//b = const void *, Size
	//z = Size
	Bindings(const Char *szArgsType, va_list vl);

	Bindings(const Bindings &bindings);

	~Bindings();

	Bindings &operator=(const Bindings &bindings);

	Status Copy(const Bindings &bindings);

	Status AddNull();

	Status AddInt(Int64 nValue);

	Status AddReal(Double lfValue);

	Status AddString(const Char *szString);

	Status AddWString(const WChar *wszString);

	Status AddBLOB(const void *pData, Size cbSize);

	Status AddZeroBLOB(Size cbSize);

	Status Clear();

	Size GetArgsCount() const;

	ArgType GetArgType(Size cIndex) const;

	Int64 GetInt(Size cIndex) const;

	Double GetReal(Size cIndex) const;

	const Char *GetString(Size cIndex) const;

	Size GetStringLen(Size cIndex) const;

	const WChar *GetWString(Size cIndex) const;

	Size GetWStringLen(Size cIndex) const;

	const void *GetBLOB(Size cIndex) const;

	Size GetBLOBSize(Size cIndex) const;

private:

	struct BLOB
	{
		void *pData;
		Size cbSize;
	};

	struct Arg
	{
		ArgType nType;
		union
		{
			Int64   nIntValue;
			Double  lfRealValue;
			String  *psString;
			WString *pwsString;
			BLOB    *pBLOB;
			Size    cbZeroBLOB;
		};
	};

	typedef CX::Vector<Arg>::Type   ArgsVector;

	ArgsVector   m_vectorArgs;

};

}//namespace SQLite

}//namespace DB

}//namespace CX

