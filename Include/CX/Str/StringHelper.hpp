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
#include "CX/String.hpp"
#include "CX/Vector.hpp"
#include "CX/APIDefs.hpp"


namespace CX
{

namespace Str
{

class CX_API StringHelper : public IObject
{
public:

	typedef Vector<String>::Type   StringsVector;

	enum StripType
	{
		Strip_Begin = 1,
		Strip_End   = 2,
		Strip_Both  = 3,
	};

	// if 0 == cText1Len => use null terminator
	// if 0 == cText2Len => use null terminator
	static bool Compare(const Char *pStr1, Size cStr1Len, const Char *pStr2, Size cStr2Len, bool bCaseSensitive, Size cMaxLen = 0);

	// if 0 == cTextLen => use null terminator
	static Status FindSubStr(const Char *pText, Size cTextLen, const Char *pWhat, Size cWhatLen, 
	                         bool bCaseSensitive, const Char **pPos);

	// if 0 == cTextLen => use null terminator
	static Status FindWithMarkers(const Char *pText, Size cTextLen, const Char *pPrefix, Size cPrefixLen, 
	                              const Char *pPostfix, Size cPostfixLen, bool bCaseSensitive, 
	                              const Char **pBegin, const Char **pEnd);

	// if 0 == cTextLen => use null terminator
	static Status Split(const Char *pText, Size cTextLen, const Char *pSeparator, Size cSeparatorLen, 
	                    bool bCaseSensitive, StringsVector &vectorStrings);

	// if 0 == cTextLen => use null terminator
	static Status Strip(const Char *pText, Size cTextLen, const Char *pStrip, Size cStripLen, 
	                    bool bCaseSensitive, const Char **pBegin, const Char **pEnd, StripType nStripType = Strip_Both);

	// if 0 == cTextLen => use null terminator
	static Status Replace(const Char *pText, Size cTextLen, const Char *pWhat, Size cWhatLen, const Char *pWithWhat, 
	                      Size cWithWhatLen, bool bCaseSensitive, String &sString, bool bAll = true);

private:

	StringHelper();

	~StringHelper();

};

}//namespace Str

}//namespace CX

