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


#include "CX/IO/IInputStream.hpp"
#include "CX/Data/JSON/ISAXParserObserver.hpp"
#include "CX/Vector.hpp"
#include "CX/String.hpp"
#include "CX/Status.hpp"
#include "CX/APIDefs.hpp"
#include "CX/IObject.hpp"


struct CX_Data_JSON_SAX_Handler;


namespace CX
{

namespace Data
{

namespace JSON
{

class CX_API SAXParser : public IObject
{
public:

	SAXParser();

	~SAXParser();

	Status ParseStream(IO::IInputStream *pInputStream);

	Status ParseBuffer(const void *pBuffer, Size cbSize);

	Status ParseString(const Char *szString);

	Status ParseString(const String &sString);

	Status AddObserver(ISAXParserObserver *pObserver);

	Status RemoveObservers();

	static Status EscapeString(const Char *szStr, String *psStr);

private:

	typedef Vector<ISAXParserObserver *>::Type   ObserversVector;

#pragma warning(push)
#pragma warning(disable: 4251)
	ObserversVector   m_vectorObservers;
#pragma warning(push)

	CX_Data_JSON_SAX_Handler *m_pHandler;

};

}//namespace JSON

}//namespace Data

}//namespace CX
