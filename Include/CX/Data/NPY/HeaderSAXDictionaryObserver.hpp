/* 
 * CX - C++ framework for general purpose development
 *
 * https://github.com/draede/cx
 * 
 * Copyright (C) 2014 - 2022 draede - draede [at] outlook [dot] com
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
#include "CX/Stack.hpp"
#include "CX/Data/NPY/SAXDictionaryParser.hpp"
#include "CX/Data/NPY/Format.hpp"
#include "CX/Data/NPY/Header.hpp"


namespace CX
{

namespace Data
{

namespace NPY
{

class HeaderSAXDictionaryObserver : public ISAXDictionaryParserObserver
{
public:

	Header   *m_pHeader;

	static Status Parse(const void *pBuffer, Size cbSize, Header *pHeader);

	virtual Status OnBeginParse();

	virtual Status OnEndParse();

	virtual Status OnBeginObject();

	virtual Status OnEndObject();

	virtual Status OnBeginArray();

	virtual Status OnEndArray();

	virtual Status OnBeginTuple();

	virtual Status OnEndTuple();

	virtual Status OnKey(const Char *pBuffer, Size cLen);

	virtual Status OnBool(Bool bBool);

	virtual Status OnInt(Int64 nInt);

	virtual Status OnUInt(UInt64 uUInt);

	virtual Status OnReal(Double lfReal);

	virtual Status OnString(const Char *pBuffer, Size cLen);

	virtual Status OnTupleDefault();

private:

	typedef Vector<UInt64>::Type   ShapeVector;

	struct Node
	{
		typedef Stack<Node>::Type   Stack;

		enum Type
		{
			Type_Object,
			Type_Array,
			Type_Tuple,

			Type_Named,
		};

		Type     nType;
		String   sKey;
		Size     cItems;
	};

	Node::Stack   m_stackNodes;
	ShapeVector   m_vectorShape;

	Status ParseColumnDesc(const Char *pBuffer, Size cLen);

};

}//namespace NPY

}//namespace Data

}//namespace CX
