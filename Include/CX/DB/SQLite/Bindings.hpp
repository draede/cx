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

#pragma once


#include "CX/Types.hpp"
#include "CX/Status.hpp"
#include "CX/String.hpp"
#include "CX/Vector.hpp"
#include "CX/DB/SQLite/Args.hpp"


namespace CX
{

namespace DB
{

namespace SQLite
{

class Bindings
{
public:

	Bindings();

	~Bindings();

	Status AddNull();

	Status AddInt(Int64 nValue);

	Status AddReal(Double lfValue);

	Status AddString(const Char *szString, Size cLen = (Size)-1, ArgStoreType nArgStoreType = ArgStore_Transient, 
	                  FreeArgStoreProc pfnFreeArgStore = NULL);

	Status AddWString(const WChar *wszString, Size cLen = (Size)-1, ArgStoreType nArgStoreType = ArgStore_Transient, 
	                  FreeArgStoreProc pfnFreeArgStore = NULL);

	Status AddBLOB(const void *pData, Size cbSize, ArgStoreType nArgStoreType = ArgStore_Transient, 
	                  FreeArgStoreProc pfnFreeArgStore = NULL);

	Status AddZeroBLOB(Size cbSize);

	Status Clear();

	Size GetArgsCount() const;

	Arg *GetArg(Size cIndex);

private:

	typedef CX::Vector<Arg>::Type   ArgsVector;

	ArgsVector   m_vectorArgs;

	Bindings(const Bindings &bindings);

	Bindings &operator=(const Bindings &bindings);

	static void FreeArgStore(void *pData);

};

}//namespace SQLite

}//namespace DB

}//namespace CX

