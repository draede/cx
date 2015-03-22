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

#include "Test1Object.hpp"
#include "Test2Object.hpp"
#include "CX/IO/MemInputStream.hpp"
#include "CX/IO/MemOutputStream.hpp"
#include "CX/IO/FileInputStream.hpp"
#include "CX/IO/FileOutputStream.hpp"
#include "CX/Data/JSON/DataReader.hpp"
#include "CX/Data/JSON/DataWriter.hpp"
#include "CX/Print.hpp"


template <typename TESTOBJ>
inline void Internal_IO_SimpleBuffersSetup(TESTOBJ &obj)
{
	obj.Reset();
	obj.bool_val = false;
	obj.int8_val = 1;
	obj.uint8_val = 2;
	obj.int16_val = 3;
	obj.uint16_val = 4;
	obj.int32_val = 5;
	obj.uint32_val = 6;
	obj.int64_val = 7;
	obj.uint64_val = 8;
	obj.float_val = 9;
	obj.double_val = 10;
	obj.string_val = "11";
	obj.wstring_val = L"12";
	
	obj.bool_arr.push_back(false);
	
	obj.int8_arr.push_back(1);
	obj.int8_arr.push_back(2);

	obj.uint8_arr.push_back(1);
	obj.uint8_arr.push_back(2);
	obj.uint8_arr.push_back(3);

	obj.int16_arr.push_back(1);
	obj.int16_arr.push_back(2);
	obj.int16_arr.push_back(3);
	obj.int16_arr.push_back(4);

	obj.uint16_arr.push_back(1);
	obj.uint16_arr.push_back(2);
	obj.uint16_arr.push_back(3);
	obj.uint16_arr.push_back(4);
	obj.uint16_arr.push_back(5);

	obj.int32_arr.push_back(1);
	obj.int32_arr.push_back(2);
	obj.int32_arr.push_back(3);
	obj.int32_arr.push_back(4);
	obj.int32_arr.push_back(5);
	obj.int32_arr.push_back(6);

	obj.uint32_arr.push_back(1);
	obj.uint32_arr.push_back(2);
	obj.uint32_arr.push_back(3);
	obj.uint32_arr.push_back(4);
	obj.uint32_arr.push_back(5);
	obj.uint32_arr.push_back(6);
	obj.uint32_arr.push_back(7);

	obj.int64_arr.push_back(1);
	obj.int64_arr.push_back(2);
	obj.int64_arr.push_back(3);
	obj.int64_arr.push_back(4);
	obj.int64_arr.push_back(5);
	obj.int64_arr.push_back(6);
	obj.int64_arr.push_back(7);
	obj.int64_arr.push_back(8);

	obj.uint64_arr.push_back(1);
	obj.uint64_arr.push_back(2);
	obj.uint64_arr.push_back(3);
	obj.uint64_arr.push_back(4);
	obj.uint64_arr.push_back(5);
	obj.uint64_arr.push_back(6);
	obj.uint64_arr.push_back(7);
	obj.uint64_arr.push_back(8);
	obj.uint64_arr.push_back(9);

	obj.float_arr.push_back(1);
	obj.float_arr.push_back(2);
	obj.float_arr.push_back(3);
	obj.float_arr.push_back(4);
	obj.float_arr.push_back(5);
	obj.float_arr.push_back(6);
	obj.float_arr.push_back(7);
	obj.float_arr.push_back(8);
	obj.float_arr.push_back(9);
	obj.float_arr.push_back(10);

	obj.double_arr.push_back(1);
	obj.double_arr.push_back(2);
	obj.double_arr.push_back(3);
	obj.double_arr.push_back(4);
	obj.double_arr.push_back(5);
	obj.double_arr.push_back(6);
	obj.double_arr.push_back(7);
	obj.double_arr.push_back(8);
	obj.double_arr.push_back(9);
	obj.double_arr.push_back(10);
	obj.double_arr.push_back(11);

	obj.string_arr.push_back("1");
	obj.string_arr.push_back("2");
	obj.string_arr.push_back("3");
	obj.string_arr.push_back("4");
	obj.string_arr.push_back("5");
	obj.string_arr.push_back("6");
	obj.string_arr.push_back("7");
	obj.string_arr.push_back("8");
	obj.string_arr.push_back("9");
	obj.string_arr.push_back("10");
	obj.string_arr.push_back("11");
	obj.string_arr.push_back("12");

	obj.wstring_arr.push_back(L"1");
	obj.wstring_arr.push_back(L"2");
	obj.wstring_arr.push_back(L"3");
	obj.wstring_arr.push_back(L"4");
	obj.wstring_arr.push_back(L"5");
	obj.wstring_arr.push_back(L"6");
	obj.wstring_arr.push_back(L"7");
	obj.wstring_arr.push_back(L"8");
	obj.wstring_arr.push_back(L"9");
	obj.wstring_arr.push_back(L"10");
	obj.wstring_arr.push_back(L"11");
	obj.wstring_arr.push_back(L"12");
	obj.wstring_arr.push_back(L"13");
	obj.wstring_arr.push_back(L"14");
}

template <typename TESTARR, typename TESTOBJ>
inline void Internal_IO_SimpleBuffersSetupArray(TESTARR &arr, CX::Size cCount)
{
	for (CX::Size i = 0; i < cCount; i++)
	{
		TESTOBJ obj;

		Internal_IO_SimpleBuffersSetup(obj);
		arr.push_back(obj);
	}
}

inline void IO_SimpleBuffers_Test1()
{
	CX::String                         sOutput;
	CX::String                         sOutput2;
	My::Test::NameSpace::Test2Object   test2_1;
	My::Test::NameSpace::Test2Object   test2_2;
	CX::Status                         status;

	test2_1.Reset();
	test2_2.Reset();

	Internal_IO_SimpleBuffersSetup(test2_1);
	Internal_IO_SimpleBuffersSetup(test2_1.obj1_val);
	Internal_IO_SimpleBuffersSetupArray<My::Test::NameSpace::Test1ObjectArray, 
	                                    My::Test::NameSpace::Test1Object>(test2_1.obj1_arr, 3);

	{
		CX::IO::MemOutputStream      mos(&sOutput);
		CX::Data::JSON::DataWriter   w(&mos);

		if ((status = w.Begin()).IsNOK())
		{
			CX::Print(stdout, "w.Begin1: error code {1}, error msg '{2}'\n", status.GetCode(), status.GetMsg());
		}
		if ((status = test2_1.Write(&w)).IsNOK())
		{
			CX::Print(stdout, "test2_1.Write1: error code {1}, error msg '{2}'\n", status.GetCode(), status.GetMsg());
		}
		if ((status = w.End()).IsNOK())
		{
			CX::Print(stdout, "w.End1: error code {1}, error msg '{2}'\n", status.GetCode(), status.GetMsg());
		}
	}

	{
		CX::IO::FileOutputStream fos("test1.json");

		CX::Print((CX::IO::IOutputStream *)&fos, "{1}", sOutput);
	}

	{
		CX::IO::MemInputStream       mis(&sOutput);
		CX::Data::JSON::DataReader   r(&mis);

		if ((status = r.Begin()).IsNOK())
		{
			CX::Print(stdout, "r.Begin: error code {1}, error msg '{2}'\n", status.GetCode(), status.GetMsg());
		}
		if ((status = test2_2.Read(&r)).IsNOK())
		{
			CX::Print(stdout, "test2_2.Read: error code {1}, error msg '{2}'\n", status.GetCode(), status.GetMsg());
		}
		if ((status = r.End()).IsNOK())
		{
			CX::Print(stdout, "r.End: error code {1}, error msg '{2}'\n", status.GetCode(), status.GetMsg());
		}

		CX::IO::MemOutputStream      mos(&sOutput2);
		CX::Data::JSON::DataWriter   w(&mos);

		if ((status = w.Begin()).IsNOK())
		{
			CX::Print(stdout, "w.Begin2: error code {1}, error msg '{2}'\n", status.GetCode(), status.GetMsg());
		}
		if ((status = test2_2.Write(&w)).IsNOK())
		{
			CX::Print(stdout, "test2_2.Write2: error code {1}, error msg '{2}'\n", status.GetCode(), status.GetMsg());
		}
		if ((status = w.End()).IsNOK())
		{
			CX::Print(stdout, "w.End2: error code {1}, error msg '{2}'\n", status.GetCode(), status.GetMsg());
		}
	}

	{
		CX::IO::FileOutputStream fos("test2.json");

		CX::Print((CX::IO::IOutputStream *)&fos, "{1}", sOutput2);
	}

	if (!test2_1.Compare(&test2_2))
	{
		CX::Print(stdout, "Objects are different\n");
	}

	if (sOutput.size() != sOutput2.size())
	{
		CX::Print(stdout, "Output sizes are different ({1} != {2})\n", sOutput.size(), sOutput2.size());
	}

	if (0 != cx_strcmp(sOutput.c_str(), sOutput2.c_str()))
	{
		CX::Print(stdout, "Outputs are different\n");
	}
}

