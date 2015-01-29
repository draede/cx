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


#include "CX/IO/IInputStream.h"
#include "CX/Data/JSON/IJSONSAXParserObserver.h"
#include "CX/Vector.h"
#include "CX/String.h"
#include "CX/Util/IFunction.h"
#include "CX/APIDefs.h"


struct CX_Data_JSON_SAX_Handler;


namespace CX
{

namespace Data
{

namespace JSON
{

class CX_API JSONSAXParser
{
public:

	JSONSAXParser();

	~JSONSAXParser();

	StatusCode ParseStream(IO::IInputStream *pInputStream);

	StatusCode ParseBuffer(const void *pBuffer, Size cbSize);

	StatusCode ParseString(const Char *szString);

	StatusCode ParseString(const String &sString);

	StatusCode AddObserver(IJSONSAXParserObserver *pObserver);

	StatusCode RemoveObservers();

private:

	typedef Vector<IJSONSAXParserObserver *>::Type   ObserversVector;

#pragma warning(push)
#pragma warning(disable: 4251)
	ObserversVector   m_vectorObservers;
#pragma warning(push)

	CX_Data_JSON_SAX_Handler *m_pHandler;

};

}//namespace JSON

}//namespace Data

}//namespace CX
