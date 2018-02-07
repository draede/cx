/* 
 * CX - C++ framework for general purpose development
 *
 * https://github.com/draede/cx
 * 
 * Copyright (C) 2014 - 2018 draede - draede [at] outlook [dot] com
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
#include "CX/Data/XML/ISAXParserObserver.hpp"
#include "CX/Vector.hpp"
#include "CX/String.hpp"
#include "CX/Status.hpp"
#include "CX/APIDefs.hpp"


namespace CX
{

namespace Data
{

namespace XML
{

class CX_API SAXParser
{
public:

	SAXParser();

	~SAXParser();

	Status BeginParse();

	Status EndParse();

	Status ParseStream(const Char *szPath);

	Status ParseStream(IO::IInputStream *pInputStream);

	Status AddObserver(ISAXParserObserver *pObserver);

	Status RemoveObservers();

	Size GetCurrentLine();

	Size GetCurrentColumn();

	static Status EscapeString(const Char *szStr, String *psStr);

private:

	static const Size BUFFER_SIZE = 8192;

	typedef Vector<ISAXParserObserver *>::Type   ObserversVector;

#pragma warning(push)
#pragma warning(disable: 4251)
	ObserversVector   m_vectorObservers;
#pragma warning(push)

	void   *m_pParser;
	String m_sText;

	static void StartElementHandler(void *pContext, const void *szName, const void **pszAttrs);

	static void EndElementHandler(void *pContext, const void *szName);

	static void CharacterDataHandler(void *pContext, const void *pBuffer, int cbSize);

};

}//namespace XML

}//namespace Data

}//namespace CX
