/* 
 * CX - C++ framework for general purpose developement
 *
 * https://github.com/draede/cx
 * 
 * Copyright (C) 2014-2015 draede - draede [at] outlook [dot] com
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


#include "CX/IO/IDataWriter.hpp"
#include "CX/IO/IOutputStream.hpp"
#include "CX/Hash/xxHash32.hpp"
#include "CX/String.hpp"
#include "CX/Stack.hpp"


namespace CX
{

namespace Data
{

namespace BINJSON
{

class CX_API DataWriter : public IO::IDataWriter
{
public:

	DataWriter(IO::IOutputStream *pOutputStream);

	virtual ~DataWriter();

	virtual Status BeginRootObject();

	virtual Status EndRootObject();

	virtual Status BeginRootArray();

	virtual Status EndRootArray();

	//object member
	virtual Status WriteNull(const Char *szName);

	//array item
	virtual Status WriteNull();

	//object member
	virtual Status WriteBool(const Char *szName, Bool bValue);

	//array item
	virtual Status WriteBool(Bool bValue);

	//object member
	virtual Status WriteInt(const Char *szName, Int64 nValue);

	//array item
	virtual Status WriteInt(Int64 nValue);

	//object member
	virtual Status WriteReal(const Char *szName, Double lfValue);

	//array item
	virtual Status WriteReal(Double lfValue);

	//object member
	virtual Status WriteString(const Char *szName, const Char *szValue);

	//array item
	virtual Status WriteString(const Char *szValue);

	//object member
	virtual Status WriteWString(const Char *szName, const WChar *wszValue);

	//array item
	virtual Status WriteWString(const WChar *wszValue);

	//object member
	virtual Status WriteBLOB(const Char *szName, const void *pData, Size cbSize);

	//array item
	virtual Status WriteBLOB(const void *pData, Size cbSize);

	//object member
	virtual Status BeginObject(const Char *szName);

	//array item
	virtual Status BeginObject();

	//object member
	virtual Status BeginArray(const Char *szName);

	//array item
	virtual Status BeginArray();

	virtual Status EndObject();

	virtual Status EndArray();

private:

	enum State
	{
		State_None,
		State_RootObject,
		State_RootArray,
		State_Object,
		State_Array,
	};

	typedef Stack<State>::Type     StatesStack;

	IO::IOutputStream   *m_pOutputStream;
	Hash::xxHash32      m_hash;

#pragma warning(push)
#pragma warning(disable: 4251)
	StatesStack         m_stackStates;
#pragma warning(pop)

	Status Write(const void *pData, Size cbSize);
	
	Status WriteEx(const void *pData, Size cbSize);

	Status WriteHeader();

	Status WriteFooter(UInt32 nHash);

};

}//namespace BINJSON

}//namespace Data

}//namespace CX

