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

#include "CX/Detail/DetailPrint/DetailPrint.h"
#include "../../../Contrib/DoubleConversion/Include/double-conversion.h"
#include "CX/IO/IOutputStream.h"
#include "CX/C/stdio.h"


namespace CX
{

namespace Detail
{

namespace DetailPrint
{

bool DoubleToString(Double lfValue, Char *szOutput, Size cLen, Size cPrecision)
{
	double_conversion::DoubleToStringConverter   cvt(
	                   double_conversion::DoubleToStringConverter::Flags::NO_FLAGS, 
	                   "#", "*", 'E', 0, 0, 0, 0);
	double_conversion::StringBuilder             sb(szOutput, (int)cLen);

	return cvt.ToFixed(lfValue, (int)cPrecision, &sb);
}

StatusCode WriteStream(IO::IOutputStream *pOutputStream, const Char *pBuffer, Size cLen)
{
	Size cbAckSize;

	return pOutputStream->Write(pBuffer, cLen * sizeof(Char), &cbAckSize).GetCode();
}

}//namespace DetailPrint

}//namespace Detail

}//namespace CX

