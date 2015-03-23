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


#include "Test2Object.hpp"
#include "CX/IO/FileOutputStream.hpp"
#include "CX/IO/FileInputStream.hpp"
#include "CX/IO/SimpleBuffers/BinaryDataWriter.hpp"
#include "CX/IO/SimpleBuffers/BinaryDataReader.hpp"
#include "CX/Print.hpp"


inline void IO_SimpleBuffers_Data_Test1()
{
	My::Test::NameSpace::Test2Object        obj1;
	My::Test::NameSpace::Test2Object        obj2;

	{
		CX::IO::FileOutputStream                fos("testbindata.dat");
		CX::IO::SimpleBuffers::BinaryDataWriter writer(&fos);
		CX::Status                              status;

		obj1.SetupWithSomeValues();

		if ((status = writer.Begin()).IsNOK())
		{
			CX::Print(stdout, "writer.Begin failed with error code {1} and error message \"{2}\"", status.GetCode(), 
			          status.GetMsg());
		}
		if ((status = obj1.Write(&writer)).IsNOK())
		{
			CX::Print(stdout, "Write failed with error code {1} and error message \"{2}\"", status.GetCode(), status.GetMsg());
		}
		if ((status = writer.End()).IsNOK())
		{
			CX::Print(stdout, "writer.End failed with error code {1} and error message \"{2}\"", status.GetCode(), status.GetMsg());
		}
	}
	{
		CX::IO::FileInputStream                 fis("testbindata.dat");
		CX::IO::SimpleBuffers::BinaryDataReader reader(&fis);
		CX::Status                              status;

		if ((status = reader.Begin()).IsNOK())
		{
			CX::Print(stdout, "reader.Begin failed with error code {1} and error message \"{2}\"", status.GetCode(), 
			          status.GetMsg());
		}
		if ((status = obj2.Read(&reader)).IsNOK())
		{
			CX::Print(stdout, "Read failed with error code {1} and error message \"{2}\"", status.GetCode(), status.GetMsg());
		}
		if ((status = reader.End()).IsNOK())
		{
			CX::Print(stdout, "reader.End failed with error code {1} and error message \"{2}\"", status.GetCode(), status.GetMsg());
		}
	}
	if (!obj1.Compare(&obj2))
	{
		CX::Print(stdout, "Objects are different");
	}
}

