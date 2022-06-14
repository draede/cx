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


#include "CX/IO/IInputStream.hpp"
#include "CX/Data/NPY/ISAXDictionaryParserObserver.hpp"
#include "CX/Vector.hpp"
#include "CX/String.hpp"
#include "CX/Status.hpp"
#include "CX/APIDefs.hpp"


namespace CX
{

namespace Data
{

namespace NPY
{

//simple stuff (no fancy string escapes, number formats etc.)
class CX_API SAXDictionaryParser
{
public:

	SAXDictionaryParser();

	~SAXDictionaryParser();

	Status ParseBuffer(const void *pBuffer, Size cbSize, Size *pcbActualSize = NULL);

	Status AddObserver(ISAXDictionaryParserObserver *pObserver);

	Status RemoveObservers();

	static Status EscapeString(const Char *szStr, String *psStr);

protected:

	static const Size MAX_DEPTH = 16;

	typedef Vector<ISAXDictionaryParserObserver *>::Type   ObserversVector;

	struct Context
	{
		const UChar   *pPos;
		Size          cLen;
		Size          cLine;
		Size          cColumn;

		inline void Move(Size cCount = 1)
		{
			pPos += cCount;
			cLen -= cCount;
			cColumn += cCount;
		}
	};

#pragma warning(push)
#pragma warning(disable: 4251)
	String            m_sFilePath;
	ObserversVector   m_vectorObservers;
	Size              m_cIncludeCurrentDepth;
#pragma warning(push)

	Status ParseObject(Context &ctx, Size cDepth, Size cMaxDepth);

	Status ParseArray(Context &ctx, Size cDepth, Size cMaxDepth);

	Status ParseTuple(Context &ctx, Size cDepth, Size cMaxDepth);

	Status ParseAny(Context &ctx, Size cDepth, Size cMaxDepth);

	Status ParseWhitespaces(Context &ctx);

	Status ParseString(Context &ctx, Bool bSingleQuote, const Char **ppKey, Size *pcLen);

};

}//namespace NPY

}//namespace Data

}//namespace CX 
