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


#include "CX/String.hpp"
#include "CX/APIDefs.hpp"
#include "CX/Types.hpp"


namespace CX
{

namespace Util
{

class CX_API RCMemBuffer
{
public:

	RCMemBuffer();

	RCMemBuffer(const RCMemBuffer &mb);

	RCMemBuffer(Size cbSize);

	RCMemBuffer(const void *pMem, Size cbSize, bool bStatic = false);

	RCMemBuffer(const Char *szStr, bool bStatic = false);

	RCMemBuffer(const WChar *wszStr, bool bStatic = false);

	RCMemBuffer(const String &sStr, bool bStatic = false);

	RCMemBuffer(const WString &wsStr, bool bStatic = false);

	virtual ~RCMemBuffer();

	RCMemBuffer &operator=(const RCMemBuffer &mb);

	void *GetMem() const;

	Size GetSize() const;

protected:

	bool IsStatic();

private:

	void   *m_pMem;
	Size   m_cbSize;
	bool   m_bStatic;

};

}//namespace Util

}//namespace CX

