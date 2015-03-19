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

#include "CX/IO/SimpleBuffers/Generator.hpp"
#include "CX/IO/SimpleBuffers/ProtoParser.hpp"
#include "CX/Print.hpp"


inline void IO_SimpleBuffers_Generator_Test1()
{
	CX::IO::SimpleBuffers::Object obj1;

	obj1.m_sName = "Test1Object";
	obj1.m_vectorFields.push_back(CX::IO::SimpleBuffers::Field("bool_val", CX::IO::SimpleBuffers::Field::Type_Bool));
	obj1.m_vectorFields.push_back(CX::IO::SimpleBuffers::Field("int8_val", CX::IO::SimpleBuffers::Field::Type_Int8));
	obj1.m_vectorFields.push_back(CX::IO::SimpleBuffers::Field("uint8_val", CX::IO::SimpleBuffers::Field::Type_UInt8));
	obj1.m_vectorFields.push_back(CX::IO::SimpleBuffers::Field("int16_val", CX::IO::SimpleBuffers::Field::Type_Int16));
	obj1.m_vectorFields.push_back(CX::IO::SimpleBuffers::Field("uint16_val", CX::IO::SimpleBuffers::Field::Type_UInt16));
	obj1.m_vectorFields.push_back(CX::IO::SimpleBuffers::Field("int32_val", CX::IO::SimpleBuffers::Field::Type_Int32));
	obj1.m_vectorFields.push_back(CX::IO::SimpleBuffers::Field("uint32_val", CX::IO::SimpleBuffers::Field::Type_UInt32));
	obj1.m_vectorFields.push_back(CX::IO::SimpleBuffers::Field("int64_val", CX::IO::SimpleBuffers::Field::Type_Int64));
	obj1.m_vectorFields.push_back(CX::IO::SimpleBuffers::Field("uint64_val", CX::IO::SimpleBuffers::Field::Type_UInt64));
	obj1.m_vectorFields.push_back(CX::IO::SimpleBuffers::Field("float_val", CX::IO::SimpleBuffers::Field::Type_Float));
	obj1.m_vectorFields.push_back(CX::IO::SimpleBuffers::Field("double_val", CX::IO::SimpleBuffers::Field::Type_Double));
	obj1.m_vectorFields.push_back(CX::IO::SimpleBuffers::Field("string_val", CX::IO::SimpleBuffers::Field::Type_String));
	obj1.m_vectorFields.push_back(CX::IO::SimpleBuffers::Field("wstring_val", CX::IO::SimpleBuffers::Field::Type_WString));
	obj1.m_vectorFields.push_back(CX::IO::SimpleBuffers::Field("bool_arr", CX::IO::SimpleBuffers::Field::Type_Bool, true));
	obj1.m_vectorFields.push_back(CX::IO::SimpleBuffers::Field("int8_arr", CX::IO::SimpleBuffers::Field::Type_Int8, true));
	obj1.m_vectorFields.push_back(CX::IO::SimpleBuffers::Field("uint8_arr", CX::IO::SimpleBuffers::Field::Type_UInt8, true));
	obj1.m_vectorFields.push_back(CX::IO::SimpleBuffers::Field("int16_arr", CX::IO::SimpleBuffers::Field::Type_Int16, true));
	obj1.m_vectorFields.push_back(CX::IO::SimpleBuffers::Field("uint16_arr", CX::IO::SimpleBuffers::Field::Type_UInt16, true));
	obj1.m_vectorFields.push_back(CX::IO::SimpleBuffers::Field("int32_arr", CX::IO::SimpleBuffers::Field::Type_Int32, true));
	obj1.m_vectorFields.push_back(CX::IO::SimpleBuffers::Field("uint32_arr", CX::IO::SimpleBuffers::Field::Type_UInt32, true));
	obj1.m_vectorFields.push_back(CX::IO::SimpleBuffers::Field("int64_arr", CX::IO::SimpleBuffers::Field::Type_Int64, true));
	obj1.m_vectorFields.push_back(CX::IO::SimpleBuffers::Field("uint64_arr", CX::IO::SimpleBuffers::Field::Type_UInt64, true));
	obj1.m_vectorFields.push_back(CX::IO::SimpleBuffers::Field("float_arr", CX::IO::SimpleBuffers::Field::Type_Float, true));
	obj1.m_vectorFields.push_back(CX::IO::SimpleBuffers::Field("double_arr", CX::IO::SimpleBuffers::Field::Type_Double, true));
	obj1.m_vectorFields.push_back(CX::IO::SimpleBuffers::Field("string_arr", CX::IO::SimpleBuffers::Field::Type_String, true));
	obj1.m_vectorFields.push_back(CX::IO::SimpleBuffers::Field("wstring_arr", CX::IO::SimpleBuffers::Field::Type_WString, true));

	CX::IO::SimpleBuffers::Generator::GenerateProtoWithPath(&obj1, "Test1Object.sb");
	CX::IO::SimpleBuffers::Generator::GenerateCPPWithPath(&obj1, "Test1Object.hpp");

	CX::IO::SimpleBuffers::Object obj2;

	obj2.m_sName = "Test2Object";
	obj2.m_vectorFields.push_back(CX::IO::SimpleBuffers::Field("bool_val", CX::IO::SimpleBuffers::Field::Type_Bool));
	obj2.m_vectorFields.push_back(CX::IO::SimpleBuffers::Field("int8_val", CX::IO::SimpleBuffers::Field::Type_Int8));
	obj2.m_vectorFields.push_back(CX::IO::SimpleBuffers::Field("uint8_val", CX::IO::SimpleBuffers::Field::Type_UInt8));
	obj2.m_vectorFields.push_back(CX::IO::SimpleBuffers::Field("int16_val", CX::IO::SimpleBuffers::Field::Type_Int16));
	obj2.m_vectorFields.push_back(CX::IO::SimpleBuffers::Field("uint16_val", CX::IO::SimpleBuffers::Field::Type_UInt16));
	obj2.m_vectorFields.push_back(CX::IO::SimpleBuffers::Field("int32_val", CX::IO::SimpleBuffers::Field::Type_Int32));
	obj2.m_vectorFields.push_back(CX::IO::SimpleBuffers::Field("uint32_val", CX::IO::SimpleBuffers::Field::Type_UInt32));
	obj2.m_vectorFields.push_back(CX::IO::SimpleBuffers::Field("int64_val", CX::IO::SimpleBuffers::Field::Type_Int64));
	obj2.m_vectorFields.push_back(CX::IO::SimpleBuffers::Field("uint64_val", CX::IO::SimpleBuffers::Field::Type_UInt64));
	obj2.m_vectorFields.push_back(CX::IO::SimpleBuffers::Field("float_val", CX::IO::SimpleBuffers::Field::Type_Float));
	obj2.m_vectorFields.push_back(CX::IO::SimpleBuffers::Field("double_val", CX::IO::SimpleBuffers::Field::Type_Double));
	obj2.m_vectorFields.push_back(CX::IO::SimpleBuffers::Field("string_val", CX::IO::SimpleBuffers::Field::Type_String));
	obj2.m_vectorFields.push_back(CX::IO::SimpleBuffers::Field("wstring_val", CX::IO::SimpleBuffers::Field::Type_WString));
	obj2.m_vectorFields.push_back(CX::IO::SimpleBuffers::Field("obj1_val", CX::IO::SimpleBuffers::Field::Type_Object, false, 
	                                                           "Test1Object"));
	obj2.m_vectorFields.push_back(CX::IO::SimpleBuffers::Field("bool_arr", CX::IO::SimpleBuffers::Field::Type_Bool, true));
	obj2.m_vectorFields.push_back(CX::IO::SimpleBuffers::Field("int8_arr", CX::IO::SimpleBuffers::Field::Type_Int8, true));
	obj2.m_vectorFields.push_back(CX::IO::SimpleBuffers::Field("uint8_arr", CX::IO::SimpleBuffers::Field::Type_UInt8, true));
	obj2.m_vectorFields.push_back(CX::IO::SimpleBuffers::Field("int16_arr", CX::IO::SimpleBuffers::Field::Type_Int16, true));
	obj2.m_vectorFields.push_back(CX::IO::SimpleBuffers::Field("uint16_arr", CX::IO::SimpleBuffers::Field::Type_UInt16, true));
	obj2.m_vectorFields.push_back(CX::IO::SimpleBuffers::Field("int32_arr", CX::IO::SimpleBuffers::Field::Type_Int32, true));
	obj2.m_vectorFields.push_back(CX::IO::SimpleBuffers::Field("uint32_arr", CX::IO::SimpleBuffers::Field::Type_UInt32, true));
	obj2.m_vectorFields.push_back(CX::IO::SimpleBuffers::Field("int64_arr", CX::IO::SimpleBuffers::Field::Type_Int64, true));
	obj2.m_vectorFields.push_back(CX::IO::SimpleBuffers::Field("uint64_arr", CX::IO::SimpleBuffers::Field::Type_UInt64, true));
	obj2.m_vectorFields.push_back(CX::IO::SimpleBuffers::Field("float_arr", CX::IO::SimpleBuffers::Field::Type_Float, true));
	obj2.m_vectorFields.push_back(CX::IO::SimpleBuffers::Field("double_arr", CX::IO::SimpleBuffers::Field::Type_Double, true));
	obj2.m_vectorFields.push_back(CX::IO::SimpleBuffers::Field("string_arr", CX::IO::SimpleBuffers::Field::Type_String, true));
	obj2.m_vectorFields.push_back(CX::IO::SimpleBuffers::Field("wstring_arr", CX::IO::SimpleBuffers::Field::Type_WString, true));
	obj2.m_vectorFields.push_back(CX::IO::SimpleBuffers::Field("obj1_arr", CX::IO::SimpleBuffers::Field::Type_Object, true, 
	                                                           "Test1Object"));

	CX::IO::SimpleBuffers::Generator::GenerateProtoWithPath(&obj2, "Test2Object.sb");
	CX::IO::SimpleBuffers::Generator::GenerateCPPWithPath(&obj2, "Test2Object.hpp");

	CX::IO::SimpleBuffers::ProtoParser parser;
	CX::Status                         status;

	if ((status = parser.BeginParse()).IsNOK())
	{
		CX::Print(stdout, "BeginParse1: error code {1}, error message '{2}'\n", status.GetCode(), status.GetMsg());
	}
	if ((status = parser.Parse("Test1Object.sb")).IsNOK())
	{
		CX::Print(stdout, "Parse1: error code {1}, error message '{2}'\n", status.GetCode(), status.GetMsg());
	}
	if ((status = parser.EndParse()).IsNOK())
	{
		CX::Print(stdout, "EndParse1: error code {1}, error message '{2}'\n", status.GetCode(), status.GetMsg());
	}
	if ((status = parser.BeginParse()).IsNOK())
	{
		CX::Print(stdout, "BeginParse1: error code {1}, error message '{2}'\n", status.GetCode(), status.GetMsg());
	}
	if ((status = parser.Parse("Test2Object.sb")).IsNOK())
	{
		CX::Print(stdout, "Parse2: error code {1}, error message '{2}'\n", status.GetCode(), status.GetMsg());
	}
	if ((status = parser.EndParse()).IsNOK())
	{
		CX::Print(stdout, "EndParse2: error code {1}, error message '{2}'\n", status.GetCode(), status.GetMsg());
	}
}

