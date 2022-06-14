/* 
 * CX - C++ framework for general purpose development
 *
 * https://github.com/draede/cx
 * 
 * Copyright (C) 2014 - 2022 draede
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
#include "CX/Vector.hpp"
#include "CX/String.hpp"
#include "CX/IO/IInputStream.hpp"
#include "CX/Data/CSV/ISAXParserObserver.hpp"
#include "CX/APIDefs.hpp"


namespace CX
{

namespace Data
{

namespace CSV
{

class CX_API SAXParser
{
public:

	enum Flag
	{
		Flag_Enable_Multi_Line_Strings = 0x01,
	};

	SAXParser();

	~SAXParser();

	Status BeginParse(Char chDelimiter = ',');

	Status EndParse();

	Status ParseStream(const Char *szPath);

	Status ParseStream(const WChar *wszPath);

	Status ParseStream(IO::IInputStream *pInputStream);

	Status ParseBuffer(const void *pBuffer, Size cbSize);

	Status AddObserver(ISAXParserObserver *pObserver);

	Status RemoveObservers();

	//call this before BeginParse
	Status SetFlags(UInt64 uFlags);

	UInt64 GetFlags() const;

private:

	static const Size   BUFFER_SIZE = 15000;

	typedef Vector<ISAXParserObserver *>::Type   ObserversVector;

	enum State
	{
		State_None,
		State_Row,
		State_Quote,
		State_EOL,
	};

#pragma warning(push)
#pragma warning(disable: 4251)
	ObserversVector                       m_vectorObservers;
	ISAXParserObserver::ColumnsVector     m_vectorColumns;
	String                                m_sColumn;
#pragma warning(push)

	Char                                  m_chDelimiter;
	State                                 m_nState;
	Bool                                  m_bInQuote;
	Size                                  m_cRowIndex;
	UInt64                                m_uFlags;

};

}//namespace CSV

}//namespace Data

}//namespace CX
