/* 
 * CX - C++ framework for general purpose development
 *
 * https://github.com/draede/cx
 * 
 * Copyright (C) 2014 - 2019 draede - draede [at] outlook [dot] com
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


#include "CX/SimpleBuffers/IReader.hpp"
#include "CX/SimpleBuffers/IWriter.hpp"
#include "CX/Types.hpp"
#include "CX/Status.hpp"
#include "CX/APIDefs.hpp"


namespace CX
{

namespace SimpleBuffers
{

class CX_API ISimpleBuffer
{
public:

	virtual ~ISimpleBuffer() { }

	static const Char *OBJECT_NAME() { return "CX.SimpleBuffers.ISimpleBuffer"; }

	virtual const Char *GetObjectName() const { return OBJECT_NAME(); }

	virtual void Init() = 0;

	virtual Size GetMembersCount() const = 0;

	virtual Bool GetMemberLoaded(Size cIndex) const = 0;

	virtual Bool GetMemberLoaded(const Char *szName) const = 0;

	virtual Status SetMemberLoaded(Size cIndex, Bool bLoaded) = 0;

	virtual Status SetMemberLoaded(const Char *szName, Bool bLoaded) = 0;

	virtual Status SetAllMembersLoaded(Bool bLoaded) = 0;

	virtual Status Read(IReader *pReader, const Char *szName = NULL) = 0;

	virtual Status Write(IWriter *pWriter, const Char *szName = NULL) const = 0;

};

}//namespace SimpleBuffers

}//namespace CX
