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


#include "CX/IO/IInputStream.hpp"
#include "CX/IO/IDataReader.hpp"
#include "CX/Stack.hpp"
#include "CX/APIDefs.hpp"


namespace CX
{

namespace Data
{

namespace JSON
{

class CX_API DataReader : public IO::IDataReader
{
public:

	DataReader(IO::IInputStream *pInputStream);

	~DataReader();

	virtual Status Begin();

	virtual Status End();

	virtual Status BeginObjectObject(const Char *szName);

	virtual Status EndObjectObject();

	virtual Status BeginObjectArray(const Char *szName);

	virtual Status EndObjectArray();

	virtual Status ReadObjectBool(const Char *szName, Bool *pbValue);

	virtual Status ReadObjectInt(const Char *szName, Int64 *pnValue);

	virtual Status ReadObjectReal(const Char *szName, Double *plfValue);

	virtual Status ReadObjectString(const Char *szName, String *psValue);

	virtual Status ReadObjectWString(const Char *szName, WString *pwsValue);

	virtual Status BeginArrayObject();

	virtual Status EndArrayObject();

	virtual Status BeginArrayArray();

	virtual Status EndArrayArray();

	//will return Status_NoMoreItems at the end of the array
	virtual Status ReadArrayBool(Bool *pbValue);

	//will return Status_NoMoreItems at the end of the array
	virtual Status ReadArrayInt(Int64 *pnValue);

	//will return Status_NoMoreItems at the end of the array
	virtual Status ReadArrayReal(Double *plfValue);

	//will return Status_NoMoreItems at the end of the array
	virtual Status ReadArrayString(String *psValue);

	//will return Status_NoMoreItems at the end of the array
	virtual Status ReadArrayWString(WString *pwsValue);

private:

	enum State
	{
		State_Object,
		State_Array,
	};

	typedef struct _StateData
	{
		_StateData()
		{
		}

		_StateData(State nState)
		{
			this->nState = nState;
			this->cCount = 0;
		}

		State nState;
		Size  cCount;
	}StateData;

	typedef Stack<StateData>::Type   StatesStack;

	static const Size  READ_BUFFER_SIZE   = 16384;

	IO::IInputStream *m_pInputStream;
	Byte             *m_pBuffer;
	Size             m_cbBufSize;
	Size             m_cbBufUsedSize;
	Size             m_cbBufOffset;
	bool             m_bIsEOF;
#pragma warning(push)
#pragma warning(disable: 4251)
	StatesStack         m_stackStates;
#pragma warning(push)

	bool IsEOF();

	bool IsValid();

	Byte Get();

	Status Next();

	Status SkipWhiteSpaces();

	Status SkipChar(Char ch);

	bool CheckArrayEnd();

	Status Read();

	Status ReadBool(Bool *pbValue);

	Status ReadInt(Int64 *pnValue);

	Status ReadReal(Double *plfValue);

	Status ReadString(String *psValue);

};

}//namespace JSON

}//namespace Data

}//namespace CX

