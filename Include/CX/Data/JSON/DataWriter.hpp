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


#include "CX/IO/IOutputStream.hpp"
#include "CX/IO/IDataWriter.hpp"
#include "CX/Stack.hpp"
#include "CX/APIDefs.hpp"


namespace CX
{

namespace Data
{

namespace JSON
{

class CX_API DataWriter : public IO::IDataWriter
{
public:

	DataWriter(IO::IOutputStream *pOutputStream);

	~DataWriter();

	virtual Status Begin();

	virtual Status End();

	virtual Status BeginObjectObject(const Char *szName);

	virtual Status EndObjectObject();

	virtual Status BeginObjectArray(const Char *szName);

	virtual Status EndObjectArray();

	virtual Status WriteObjectBool(const Char *szName, Bool bValue);

	virtual Status WriteObjectInt(const Char *szName, Int64 nValue);

	virtual Status WriteObjectReal(const Char *szName, Double lfValue);

	virtual Status WriteObjectString(const Char *szName, const Char *szValue);

	virtual Status WriteObjectWString(const Char *szName, const WChar *wszValue);

	virtual Status BeginArrayObject();

	virtual Status EndArrayObject();

	virtual Status BeginArrayArray();

	virtual Status EndArrayArray();

	virtual Status WriteArrayBool(Bool bValue);

	virtual Status WriteArrayInt(Int64 nValue);

	virtual Status WriteArrayReal(Double lfValue);

	virtual Status WriteArrayString(const Char *szValue);

	virtual Status WriteArrayWString(const WChar *wszValue);

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

	IO::IOutputStream   *m_pOutputStream;
#pragma warning(push)
#pragma warning(disable: 4251)
	StatesStack         m_stackStates;
	String              m_sIndent;
#pragma warning(push)

	Status WriteIndent();

	Status WriteItemSeparator(bool bForceNoComma = false);

};

}//namespace JSON

}//namespace Data

}//namespace CX

