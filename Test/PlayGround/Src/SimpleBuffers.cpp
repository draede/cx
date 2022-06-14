/* 
 * CX - C++ framework for general purpose development
 *
 * https://github.com/draede/cx
 * 
 * Copyright (C) 2014 - 2021 draede
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

#include "CX/Str/StringHelper.hpp"
#include "CX/SimpleBuffers/BSONReader.hpp"
#include "CX/SimpleBuffers/BSONWriter.hpp"
#include "CX/SimpleBuffers/JSONReader.hpp"
#include "CX/SimpleBuffers/JSONWriter.hpp"
#include "CX/IO/MemOutputStream.hpp"
#include "CX/IO/MemInputStream.hpp"
#include "TestSimpleBuffers2.hpp"
#include "Tester.hpp"


using namespace CX;

struct BSONSBNAME
{
	static const Char *NAME() { return "BSON"; }
};

struct JSONSBNAME
{
	static const Char *NAME() { return "JSON"; }
};

template <typename READER, typename WRITER, typename SBNAME>
void SimpleBuffers_TestReaderWriter()
{
	String   sOutput1;
	String   sOutput2;

	{
		WRITER                   writer;
		IO::MemOutputStream      mos(&sOutput1);
		TestSimpleBuffers2       t2;
		Byte                     blob[] = "test blob";
		Status                   status;

		t2.Setvalue_bool(false);
		t2.Setvalue_int8(1);
		t2.Setvalue_uint8(2);
		t2.Setvalue_int16(3);
		t2.Setvalue_uint16(4);
		t2.Setvalue_int32(5);
		t2.Setvalue_uint32(6);
		t2.Setvalue_int64(7);
		t2.Setvalue_uint64(8);
		t2.Setvalue_float(9.0f);
		t2.Setvalue_double(10.0);
		t2.Setvalue_string("test string");
		t2.Setvalue_blob(CX::BLOB(blob, blob + sizeof(blob) - 1));
		t2.GettestSimpleBuffers1().Setvalue_bool(false);
		t2.GettestSimpleBuffers1().Setvalue_int8(1);
		t2.GettestSimpleBuffers1().Setvalue_uint8(2);
		t2.GettestSimpleBuffers1().Setvalue_int16(3);
		t2.GettestSimpleBuffers1().Setvalue_uint16(4);
		t2.GettestSimpleBuffers1().Setvalue_int32(5);
		t2.GettestSimpleBuffers1().Setvalue_uint32(6);
		t2.GettestSimpleBuffers1().Setvalue_int64(7);
		t2.GettestSimpleBuffers1().Setvalue_uint64(8);
		t2.GettestSimpleBuffers1().Setvalue_float(9.0f);
		t2.GettestSimpleBuffers1().Setvalue_double(10.0);
		t2.GettestSimpleBuffers1().Setvalue_string("test string");
		t2.GettestSimpleBuffers1().Setvalue_blob(CX::BLOB(blob, blob + sizeof(blob) - 1));

		status = writer.Begin(&mos);
		status = t2.Write(&writer);
		status = writer.End();
	}
	{
		READER                   reader;
		IO::MemInputStream       fis(&sOutput1);
		TestSimpleBuffers2       t2;
		Status                   status;

		status = reader.Begin(&fis);
		status = t2.Read(&reader);
		status = reader.End();

		{
			WRITER              writer;
			IO::MemOutputStream mos(&sOutput2);

			status = writer.Begin(&mos);
			status = t2.Write(&writer);
			status = writer.End();
		}
	}
	if (sOutput1.size() != sOutput2.size())
	{
		Print(stdout, "SimpleBuffers({1}): size mismatched ({2} != {3})\n", SBNAME::NAME(), sOutput1.size(), sOutput2.size());
	}
	else
	{
		if (sOutput1 != sOutput2)
		{
			Print(stdout, "SimpleBuffers({1}): outputs mismatched\n", SBNAME::NAME());
		}
		else
		{
			Print(stdout, "SimpleBuffers({1}): OK\n", SBNAME::NAME());
		}
	}
}

void SimpleBuffers_TestBinReaderWriter()
{
	SimpleBuffers_TestReaderWriter<SimpleBuffers::BSONReader, SimpleBuffers::BSONWriter, BSONSBNAME>();
}

void SimpleBuffers_TestJSONReaderWriter()
{
	SimpleBuffers_TestReaderWriter<SimpleBuffers::JSONReader, SimpleBuffers::JSONWriter, JSONSBNAME>();
}

REGISTER_TEST(SimpleBuffers_TestBinReaderWriter);
REGISTER_TEST(SimpleBuffers_TestJSONReaderWriter);

