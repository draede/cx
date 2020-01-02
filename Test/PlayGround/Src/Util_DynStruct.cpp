/* 
 * CX - C++ framework for general purpose development
 *
 * https://github.com/draede/cx
 * 
 * Copyright (C) 2014 - 2020 draede - draede [at] outlook [dot] com
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

#include "CX/Util/DynStruct.hpp"
#include "Tester.hpp"
#include "CX/Print.hpp"


using namespace CX;


#define MYTEST(expr, msg)     if (!(expr)) { Print(stdout, "ERROR: {1}\n", msg); }


void Util_DynStruct_Test1()
{
	Util::DynStruct ds;

	MYTEST(ds.SetBool("Bool", true).IsOK(), "SetBool");
	MYTEST(ds.GetBool("Bool") == true, "GetBool");
	MYTEST(ds.SetInt8("Int8", 123).IsOK(), "SetInt8");
	MYTEST(ds.GetInt8("Int8") == 123, "GetInt8");
	MYTEST(ds.SetUInt8("UInt8", 123).IsOK(), "SetUInt8");
	MYTEST(ds.GetUInt8("UInt8") == 123, "GetUInt8");
	MYTEST(ds.SetInt16("Int16", 12345).IsOK(), "SetInt16");
	MYTEST(ds.GetInt16("Int16") == 12345, "GetInt16");
	MYTEST(ds.SetUInt16("UInt16", 12345).IsOK(), "SetUInt16");
	MYTEST(ds.GetUInt16("UInt16") == 12345, "GetUInt16");
	MYTEST(ds.SetInt32("Int32", 1234567).IsOK(), "SetInt32");
	MYTEST(ds.GetInt32("Int32") == 1234567, "GetInt32");
	MYTEST(ds.SetUInt32("UInt32", 1234567).IsOK(), "SetUInt32");
	MYTEST(ds.GetUInt32("UInt32") == 1234567, "GetUInt32");
	MYTEST(ds.SetInt64("Int64", 123456789).IsOK(), "SetInt64");
	MYTEST(ds.GetInt64("Int64") == 123456789, "GetInt64");
	MYTEST(ds.SetUInt64("UInt64", 123456789).IsOK(), "SetUInt64");
	MYTEST(ds.GetUInt64("UInt64") == 123456789, "GetUInt64");
	MYTEST(ds.SetFloat("Float", 123.456f).IsOK(), "SetFloat");
	MYTEST(ds.GetFloat("Float") == 123.456f, "GetFloat");
	MYTEST(ds.SetDouble("Double", 123.456).IsOK(), "SetDouble");
	MYTEST(ds.GetDouble("Double") == 123.456, "GetDouble");
	MYTEST(ds.SetChar("Char", 'x').IsOK(), "SetChar");
	MYTEST(ds.GetChar("Char") == 'x', "GetChar");
	MYTEST(ds.SetWChar("WChar", 'a').IsOK(), "SetWChar");
	MYTEST(ds.GetWChar("WChar") == 'a', "GetWChar");
	MYTEST(ds.SetString("String", "abc").IsOK(), "SetString");
	MYTEST(ds.GetString("String") == "abc", "GetString");
	MYTEST(ds.SetWString("WString", L"xyz").IsOK(), "SetWString");
	MYTEST(ds.GetWString("WString") == WString(L"xyz"), "GetWString");
}

REGISTER_TEST(Util_DynStruct_Test1);
