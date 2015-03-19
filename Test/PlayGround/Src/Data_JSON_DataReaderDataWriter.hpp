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


#include "CX/IO/MemInputStream.hpp"
#include "CX/IO/MemOutputStream.hpp"
#include "CX/Data/JSON/DataReader.hpp"
#include "CX/Data/JSON/DataWriter.hpp"
#include "CX/Print.hpp"


inline void Data_JSON_DataReaderDataWriter_Test1()
{
	CX::String                   sOutput;
	CX::Bool                     bValue;
	CX::Int64                    nInt;
	CX::Double                   lfReal;
	CX::String                   sStr;
	CX::WString                  sWStr;
	CX::Status                   status;

	CX::IO::MemOutputStream      mos(&sOutput);
	CX::Data::JSON::DataWriter   w(&mos);

	if ((status = w.Begin()).IsNOK())
	{
		CX::Print(stdout, "w.Begin: error code {1}, error msg '{2}'\n", status.GetCode(), status.GetMsg());
	}
		if ((status = w.BeginObjectObject("obj1")).IsNOK())
		{
			CX::Print(stdout, "w.BeginObjectObject(\"obj1\"): error code {1}, error msg '{2}'\n", status.GetCode(), status.GetMsg());
		}
		if ((status = w.WriteObjectBool("bool", true)).IsNOK())
		{
			CX::Print(stdout, "w.WriteObjectBool(\"bool\", true): error code {1}, error msg '{2}'\n", status.GetCode(), status.GetMsg());
		}
		if ((status = w.WriteObjectInt("int", 123)).IsNOK())
		{
			CX::Print(stdout, "w.WriteObjectInt(\"int\", 123): error code {1}, error msg '{2}'\n", status.GetCode(), status.GetMsg());
		}
		if ((status = w.WriteObjectReal("real", 123.456)).IsNOK())
		{
			CX::Print(stdout, "w.WriteObjectReal(\"real\", 123.456): error code {1}, error msg '{2}'\n", status.GetCode(), status.GetMsg());
		}
		if ((status = w.WriteObjectString("string", "teststring")).IsNOK())
		{
			CX::Print(stdout, "w.WriteObjectString(\"string\", \"teststring\"): error code {1}, error msg '{2}'\n", status.GetCode(), status.GetMsg());
		}
		if ((status = w.WriteObjectWString("wstring", L"testwstring")).IsNOK())
		{
			CX::Print(stdout, "w.WriteObjectWString(\"wstring\", L\"testwstring\"): error code {1}, error msg '{2}'\n", status.GetCode(), status.GetMsg());
		}
		if ((status = w.EndObjectObject()).IsNOK())
		{
			CX::Print(stdout, "w.EndObjectObject(\"obj1\"): error code {1}, error msg '{2}'\n", status.GetCode(), status.GetMsg());
		}
		if ((status = w.BeginObjectArray("arr1")).IsNOK())
		{
			CX::Print(stdout, "w.BeginObjectArray(\arr1\"): error code {1}, error msg '{2}'\n", status.GetCode(), status.GetMsg());
		}
		if ((status = w.WriteArrayBool(true)).IsNOK())
		{
			CX::Print(stdout, "w.WriteArrayBool(true): error code {1}, error msg '{2}'\n", status.GetCode(), status.GetMsg());
		}
		if ((status = w.WriteArrayInt(123)).IsNOK())
		{
			CX::Print(stdout, "w.WriteArrayInt(123): error code {1}, error msg '{2}'\n", status.GetCode(), status.GetMsg());
		}
		if ((status = w.WriteArrayReal(123.456)).IsNOK())
		{
			CX::Print(stdout, "w.WriteArrayReal(123.456): error code {1}, error msg '{2}'\n", status.GetCode(), status.GetMsg());
		}
		if ((status = w.WriteArrayString("teststring")).IsNOK())
		{
			CX::Print(stdout, "w.WriteArrayString(\"teststring\"): error code {1}, error msg '{2}'\n", status.GetCode(), status.GetMsg());
		}
		if ((status = w.WriteArrayWString(L"testwstring")).IsNOK())
		{
			CX::Print(stdout, "w.WriteArrayWString(L\"testwstring\"): error code {1}, error msg '{2}'\n", status.GetCode(), status.GetMsg());
		}
		if ((status = w.EndObjectArray()).IsNOK())
		{
			CX::Print(stdout, "w.EndObjectArray(\"arr1\"): error code {1}, error msg '{2}'\n", status.GetCode(), status.GetMsg());
		}
	if ((status = w.End()).IsNOK())
	{
		CX::Print(stdout, "w.End: error code {1}, error msg '{2}'\n", status.GetCode(), status.GetMsg());
	}

	CX::IO::MemInputStream       mis(&sOutput);
	CX::Data::JSON::DataReader   r(&mis);

	if ((status = r.Begin()).IsNOK())
	{
		CX::Print(stdout, "r.Begin: error code {1}, error msg '{2}'\n", status.GetCode(), status.GetMsg());
	}
		if ((status = r.BeginObjectObject("obj1")).IsNOK())
		{
			CX::Print(stdout, "r.BeginObjectObject(\"obj1\"): error code {1}, error msg '{2}'\n", status.GetCode(), status.GetMsg());
		}
		if ((status = r.ReadObjectBool("bool", &bValue)).IsNOK())
		{
			CX::Print(stdout, "r.ReadObjectBool(\"bool\", &v): error code {1}, error msg '{2}'\n", status.GetCode(), status.GetMsg());
		}
		if ((status = r.ReadObjectInt("int", &nInt)).IsNOK())
		{
			CX::Print(stdout, "r.ReadObjectInt(\"int\", &v): error code {1}, error msg '{2}'\n", status.GetCode(), status.GetMsg());
		}
		if ((status = r.ReadObjectReal("real", &lfReal)).IsNOK())
		{
			CX::Print(stdout, "r.ReadObjectReal(\"real\", &v): error code {1}, error msg '{2}'\n", status.GetCode(), status.GetMsg());
		}
		if ((status = r.ReadObjectString("string", &sStr)).IsNOK())
		{
			CX::Print(stdout, "r.ReadObjectString(\"string\", &v): error code {1}, error msg '{2}'\n", status.GetCode(), status.GetMsg());
		}
		if ((status = r.ReadObjectWString("wstring", &sWStr)).IsNOK())
		{
			CX::Print(stdout, "r.ReadObjectWString(\"wstring\", &v): error code {1}, error msg '{2}'\n", status.GetCode(), status.GetMsg());
		}
		if ((status = r.EndObjectObject()).IsNOK())
		{
			CX::Print(stdout, "r.EndObjectObject(\"obj1\"): error code {1}, error msg '{2}'\n", status.GetCode(), status.GetMsg());
		}
		if ((status = r.BeginObjectArray("arr1")).IsNOK())
		{
			CX::Print(stdout, "r.BeginObjectArray(\"arr1\"): error code {1}, error msg '{2}'\n", status.GetCode(), status.GetMsg());
		}
		if ((status = r.ReadArrayBool(&bValue)).IsNOK())
		{
			CX::Print(stdout, "r.ReadArrayBool(&v): error code {1}, error msg '{2}'\n", status.GetCode(), status.GetMsg());
		}
		if ((status = r.ReadArrayInt(&nInt)).IsNOK())
		{
			CX::Print(stdout, "r.ReadArrayInt(&v): error code {1}, error msg '{2}'\n", status.GetCode(), status.GetMsg());
		}
		if ((status = r.ReadArrayReal(&lfReal)).IsNOK())
		{
			CX::Print(stdout, "r.ReadArrayReal(&v): error code {1}, error msg '{2}'\n", status.GetCode(), status.GetMsg());
		}
		if ((status = r.ReadArrayString(&sStr)).IsNOK())
		{
			CX::Print(stdout, "r.ReadArrayString(&v): error code {1}, error msg '{2}'\n", status.GetCode(), status.GetMsg());
		}
		if ((status = r.ReadArrayWString(&sWStr)).IsNOK())
		{
			CX::Print(stdout, "r.ReadArrayWString(&v): error code {1}, error msg '{2}'\n", status.GetCode(), status.GetMsg());
		}
		if ((status = r.EndObjectArray()).IsNOK())
		{
			CX::Print(stdout, "r.EndObjectArray(\"arr1\"): error code {1}, error msg '{2}'\n", status.GetCode(), status.GetMsg());
		}
	if ((status = r.End()).IsNOK())
	{
		CX::Print(stdout, "r.End: error code {1}, error msg '{2}'\n", status.GetCode(), status.GetMsg());
	}

}

