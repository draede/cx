/* 
 * CX - C++ framework for general purpose developement
 *
 * https://github.com/draede/cx
 * 
 * Copyright (C) 2014 draede - draede [at] outlook [dot] com
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


#include "CX/Types.h"
#include "CX/APIDefs.h"


namespace CX
{

namespace Data
{

namespace JSON
{

class CX_API IJSONSAXParserObserver
{
public:

	virtual ~IJSONSAXParserObserver() { }

	virtual bool OnBeginParse() = 0;

	virtual bool OnEndParse() = 0;

	virtual bool OnBeginObject() = 0;

	virtual bool OnEndObject() = 0;

	virtual bool OnBeginArray() = 0;

	virtual bool OnEndArray() = 0;

	virtual bool OnKey(const Char *pBuffer, Size cLen) = 0;

	virtual bool OnNullValue() = 0;

	virtual bool OnBoolValue(bool bBool) = 0;

	virtual bool OnIntValue(Int64 nInt) = 0;

	virtual bool OnRealValue(Double lfReal) = 0;

	virtual bool OnStringValue(const Char *pBuffer, Size cLen) = 0;

};

}//namespace JSON

}//namespace Data

}//namespace CX
