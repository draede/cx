/* 
 * CX - C++ framework for general purpose development
 *
 * https://github.com/draede/cx
 * 
 * Copyright (C) 2014-2016 draede, draede [at] outlook [dot] com
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
#include "CX/Log/IFormatter.hpp"
#include "CX/APIDefs.hpp"


namespace CX
{

namespace Log
{

class CX_API DefaultFormatter : public IFormatter, public IObject
{
public:

	enum Flag
	{
		Show_Date    = 1,
		Show_Time    = 2,
		Show_Level   = 4,
		Show_Tag     = 8,
		Show_NewLine = 16,
		Show_TID     = 32,
		Show_PID     = 64,
	};

	static const int FLAGS_MINIMAL = Show_Level | Show_Tag | Show_NewLine;
	static const int FLAGS_MEDIUM  = Show_Date | Show_Time | Show_Level | Show_Tag | 
	                                 Show_NewLine;
	static const int FLAGS_FULL    = Show_Date | Show_Time | Show_Level | Show_Tag | 
	                                 Show_NewLine | Show_TID | Show_PID;

	DefaultFormatter(int nFlags = FLAGS_MEDIUM);

	~DefaultFormatter();

	void SetFlags(int nFlags = FLAGS_MEDIUM);

	int GetFlags() const;

	virtual Status Write(IOutput *pOutput, Level nLevel, const Char *szTag, 
	                         const Char *pBuffer, Size cLen);

private:

	int m_nFlags;

};

}//namespace Log

}//namespace CX

