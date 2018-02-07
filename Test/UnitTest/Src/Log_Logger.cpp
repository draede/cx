/* 
 * CX - C++ framework for general purpose development
 *
 * https://github.com/draede/cx
 * 
 * Copyright (C) 2014 - 2018 draede - draede [at] outlook [dot] com
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

#include "CX/Types.hpp"
#include "CX/Log/Logger.hpp"
#include "CX/Log/DefaultFormatter.hpp"

#pragma warning(push)
#pragma warning(disable: 4702)
#include "../../../Contrib/Catch/Include/catch.hpp"
#pragma warning(pop)


using namespace CX;


class MyOutput : public Log::IOutput
{
public:

	String m_sOutput;

	virtual Status Write(Log::Level nLevel, const Char *szTag, const Char *pBuffer, Size cLen)
	{
		nLevel;
		szTag;

		m_sOutput.assign(pBuffer, cLen);

		return Status();
	}

};

TEST_CASE("Log::Logger tests", "[CX::Log::Logger]")
{
	SECTION("no args")
	{
		MyOutput *pOutput = new (std::nothrow) MyOutput();

		((Log::DefaultFormatter *)Log::Logger::GetDefaultLogger().GetFormatter())->
		     SetFlags(Log::DefaultFormatter::Show_Tag | Log::DefaultFormatter::Show_Level | 
		              Log::DefaultFormatter::Show_NewLine);
		Log::Logger::GetDefaultLogger().AddOutput(pOutput);
		Log::Logger::GetDefaultLogger().SetLevel(Log::Level_Debug);

		pOutput->m_sOutput.clear();
		CXERR("mytag", "test");
		REQUIRE(String("[ERR] [    mytag   ]  : test\n") == pOutput->m_sOutput);
		
		pOutput->m_sOutput.clear();
		CXERRIF(True, "mytag", "test");
		REQUIRE(String("[ERR] [    mytag   ]  : test\n") == pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXERRIF(False, "mytag", "test");
		REQUIRE(String("") == pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXWARN("mytag", "test");
		REQUIRE(String("[WRN] [    mytag   ]  : test\n") == pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXWARNIF(True, "mytag", "test");
		REQUIRE(String("[WRN] [    mytag   ]  : test\n") == pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXWARNIF(False, "mytag", "test");
		REQUIRE(String("") == pOutput->m_sOutput);


		pOutput->m_sOutput.clear();
		CXINFO("mytag", "test");
		REQUIRE(String("[INF] [    mytag   ]  : test\n") == pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXINFOIF(True, "mytag", "test");
		REQUIRE(String("[INF] [    mytag   ]  : test\n") == pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXINFOIF(False, "mytag", "test");
		REQUIRE(String("") == pOutput->m_sOutput);


		pOutput->m_sOutput.clear();
		CXDBG("mytag", "test");
		REQUIRE(String("[DBG] [    mytag   ]  : test\n") == pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXDBGIF(True, "mytag", "test");
		REQUIRE(String("[DBG] [    mytag   ]  : test\n") == pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXDBGIF(False, "mytag", "test");
		REQUIRE(String("") == pOutput->m_sOutput);


		pOutput->m_sOutput.clear();
		CXVERB("mytag", "test");
		REQUIRE(String("[VRB] [    mytag   ]  : test\n") == pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXVERBIF(True, "mytag", "test");
		REQUIRE(String("[VRB] [    mytag   ]  : test\n") == pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXVERBIF(False, "mytag", "test");
		REQUIRE(String("") == pOutput->m_sOutput);

		Log::Logger::GetDefaultLogger().SetLevel(Log::Level_Error);

		pOutput->m_sOutput.clear();
		CXINFO("mytag", "test");
		REQUIRE(String("") == pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXINFOIF(True, "mytag", "test");
		REQUIRE(String("") == pOutput->m_sOutput);
	}

	SECTION("1 arg")
	{
		MyOutput *pOutput = new (std::nothrow) MyOutput();

		((Log::DefaultFormatter *)Log::Logger::GetDefaultLogger().GetFormatter())->
		     SetFlags(Log::DefaultFormatter::Show_Tag | Log::DefaultFormatter::Show_Level | 
		              Log::DefaultFormatter::Show_NewLine);
		Log::Logger::GetDefaultLogger().AddOutput(pOutput);
		Log::Logger::GetDefaultLogger().SetLevel(Log::Level_Debug);

		pOutput->m_sOutput.clear();
		CXERR("mytag", "test {1}", 123);
		REQUIRE(String("[ERR] [    mytag   ]  : test 123\n") == pOutput->m_sOutput);
		
		pOutput->m_sOutput.clear();
		CXERRIF(True, "mytag", "test {1}", 123);
		REQUIRE(String("[ERR] [    mytag   ]  : test 123\n") == pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXERRIF(False, "mytag", "test {1}", 123);
		REQUIRE(String("") == pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXWARN("mytag", "test {1}", 123);
		REQUIRE(String("[WRN] [    mytag   ]  : test 123\n") == pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXWARNIF(True, "mytag", "test {1}", 123);
		REQUIRE(String("[WRN] [    mytag   ]  : test 123\n") == pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXWARNIF(False, "mytag", "test {1}", 123);
		REQUIRE(String("") == pOutput->m_sOutput);


		pOutput->m_sOutput.clear();
		CXINFO("mytag", "test {1}", 123);
		REQUIRE(String("[INF] [    mytag   ]  : test 123\n") == pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXINFOIF(True, "mytag", "test {1}", 123);
		REQUIRE(String("[INF] [    mytag   ]  : test 123\n") == pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXINFOIF(False, "mytag", "test {1}", 123);
		REQUIRE(String("") == pOutput->m_sOutput);


		pOutput->m_sOutput.clear();
		CXDBG("mytag", "test {1}", 123);
		REQUIRE(String("[DBG] [    mytag   ]  : test 123\n") == pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXDBGIF(True, "mytag", "test {1}", 123);
		REQUIRE(String("[DBG] [    mytag   ]  : test 123\n") == pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXDBGIF(False, "mytag", "test {1}", 123);
		REQUIRE(String("") == pOutput->m_sOutput);


		pOutput->m_sOutput.clear();
		CXVERB("mytag", "test {1}", 123);
		REQUIRE(String("[VRB] [    mytag   ]  : test 123\n") == pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXVERBIF(True, "mytag", "test {1}", 123);
		REQUIRE(String("[VRB] [    mytag   ]  : test 123\n") == pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXVERBIF(False, "mytag", "test {1}", 123);
		REQUIRE(String("") == pOutput->m_sOutput);

		Log::Logger::GetDefaultLogger().SetLevel(Log::Level_Error);

		pOutput->m_sOutput.clear();
		CXINFO("mytag", "test {1}", 123);
		REQUIRE(String("") == pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXINFOIF(True, "mytag", "test {1}", 123);
		REQUIRE(String("") == pOutput->m_sOutput);
	}

	SECTION("2 args")
	{
		MyOutput *pOutput = new (std::nothrow) MyOutput();

		((Log::DefaultFormatter *)Log::Logger::GetDefaultLogger().GetFormatter())->
		     SetFlags(Log::DefaultFormatter::Show_Tag | Log::DefaultFormatter::Show_Level | 
		              Log::DefaultFormatter::Show_NewLine);
		Log::Logger::GetDefaultLogger().AddOutput(pOutput);
		Log::Logger::GetDefaultLogger().SetLevel(Log::Level_Debug);

		pOutput->m_sOutput.clear();
		CXERR("mytag", "test {1} {2}", 123, 'x');
		REQUIRE(String("[ERR] [    mytag   ]  : test 123 x\n") == pOutput->m_sOutput);
		
		pOutput->m_sOutput.clear();
		CXERRIF(True, "mytag", "test {1} {2}", 123, 'x');
		REQUIRE(String("[ERR] [    mytag   ]  : test 123 x\n") == pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXERRIF(False, "mytag", "test {1} {2}", 123, 'x');
		REQUIRE(String("") == pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXWARN("mytag", "test {1} {2}", 123, 'x');
		REQUIRE(String("[WRN] [    mytag   ]  : test 123 x\n") == pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXWARNIF(True, "mytag", "test {1} {2}", 123, 'x');
		REQUIRE(String("[WRN] [    mytag   ]  : test 123 x\n") == pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXWARNIF(False, "mytag", "test {1} {2}", 123, 'x');
		REQUIRE(String("") == pOutput->m_sOutput);


		pOutput->m_sOutput.clear();
		CXINFO("mytag", "test {1} {2}", 123, 'x');
		REQUIRE(String("[INF] [    mytag   ]  : test 123 x\n") == pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXINFOIF(True, "mytag", "test {1} {2}", 123, 'x');
		REQUIRE(String("[INF] [    mytag   ]  : test 123 x\n") == pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXINFOIF(False, "mytag", "test {1} {2}", 123, 'x');
		REQUIRE(String("") == pOutput->m_sOutput);


		pOutput->m_sOutput.clear();
		CXDBG("mytag", "test {1} {2}", 123, 'x');
		REQUIRE(String("[DBG] [    mytag   ]  : test 123 x\n") == pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXDBGIF(True, "mytag", "test {1} {2}", 123, 'x');
		REQUIRE(String("[DBG] [    mytag   ]  : test 123 x\n") == pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXDBGIF(False, "mytag", "test {1} {2}", 123, 'x');
		REQUIRE(String("") == pOutput->m_sOutput);


		pOutput->m_sOutput.clear();
		CXVERB("mytag", "test {1} {2}", 123, 'x');
		REQUIRE(String("[VRB] [    mytag   ]  : test 123 x\n") == pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXVERBIF(True, "mytag", "test {1} {2}", 123, 'x');
		REQUIRE(String("[VRB] [    mytag   ]  : test 123 x\n") == pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXVERBIF(False, "mytag", "test {1} {2}", 123, 'x');
		REQUIRE(String("") == pOutput->m_sOutput);

		Log::Logger::GetDefaultLogger().SetLevel(Log::Level_Error);

		pOutput->m_sOutput.clear();
		CXINFO("mytag", "test {1} {2}", 123, 'x');
		REQUIRE(String("") == pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXINFOIF(True, "mytag", "test {1} {2}", 123, 'x');
		REQUIRE(String("") == pOutput->m_sOutput);
	}

	SECTION("3 args")
	{
		MyOutput *pOutput = new (std::nothrow) MyOutput();

		((Log::DefaultFormatter *)Log::Logger::GetDefaultLogger().GetFormatter())->
		     SetFlags(Log::DefaultFormatter::Show_Tag | Log::DefaultFormatter::Show_Level | 
		              Log::DefaultFormatter::Show_NewLine);
		Log::Logger::GetDefaultLogger().AddOutput(pOutput);
		Log::Logger::GetDefaultLogger().SetLevel(Log::Level_Debug);

		pOutput->m_sOutput.clear();
		CXERR("mytag", "test {1} {2} {3}", 123, 'x', False);
		REQUIRE(String("[ERR] [    mytag   ]  : test 123 x false\n") == pOutput->m_sOutput);
		
		pOutput->m_sOutput.clear();
		CXERRIF(True, "mytag", "test {1} {2} {3}", 123, 'x', False);
		REQUIRE(String("[ERR] [    mytag   ]  : test 123 x false\n") == pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXERRIF(False, "mytag", "test {1} {2} {3}", 123, 'x', False);
		REQUIRE(String("") == pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXWARN("mytag", "test {1} {2} {3}", 123, 'x', False);
		REQUIRE(String("[WRN] [    mytag   ]  : test 123 x false\n") == pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXWARNIF(True, "mytag", "test {1} {2} {3}", 123, 'x', False);
		REQUIRE(String("[WRN] [    mytag   ]  : test 123 x false\n") == pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXWARNIF(False, "mytag", "test {1} {2} {3}", 123, 'x', False);
		REQUIRE(String("") == pOutput->m_sOutput);


		pOutput->m_sOutput.clear();
		CXINFO("mytag", "test {1} {2} {3}", 123, 'x', False);
		REQUIRE(String("[INF] [    mytag   ]  : test 123 x false\n") == pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXINFOIF(True, "mytag", "test {1} {2} {3}", 123, 'x', False);
		REQUIRE(String("[INF] [    mytag   ]  : test 123 x false\n") == pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXINFOIF(False, "mytag", "test {1} {2} {3}", 123, 'x', False);
		REQUIRE(String("") == pOutput->m_sOutput);


		pOutput->m_sOutput.clear();
		CXDBG("mytag", "test {1} {2} {3}", 123, 'x', False);
		REQUIRE(String("[DBG] [    mytag   ]  : test 123 x false\n") == pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXDBGIF(True, "mytag", "test {1} {2} {3}", 123, 'x', False);
		REQUIRE(String("[DBG] [    mytag   ]  : test 123 x false\n") == pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXDBGIF(False, "mytag", "test {1} {2} {3}", 123, 'x', False);
		REQUIRE(String("") == pOutput->m_sOutput);


		pOutput->m_sOutput.clear();
		CXVERB("mytag", "test {1} {2} {3}", 123, 'x', False);
		REQUIRE(String("[VRB] [    mytag   ]  : test 123 x false\n") == pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXVERBIF(True, "mytag", "test {1} {2} {3}", 123, 'x', False);
		REQUIRE(String("[VRB] [    mytag   ]  : test 123 x false\n") == pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXVERBIF(False, "mytag", "test {1} {2} {3}", 123, 'x', False);
		REQUIRE(String("") == pOutput->m_sOutput);

		Log::Logger::GetDefaultLogger().SetLevel(Log::Level_Error);

		pOutput->m_sOutput.clear();
		CXINFO("mytag", "test {1} {2} {3}", 123, 'x', False);
		REQUIRE(String("") == pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXINFOIF(True, "mytag", "test {1} {2} {3}", 123, 'x', False);
		REQUIRE(String("") == pOutput->m_sOutput);
	}

	SECTION("4 args")
	{
		MyOutput *pOutput = new (std::nothrow) MyOutput();

		((Log::DefaultFormatter *)Log::Logger::GetDefaultLogger().GetFormatter())->
		     SetFlags(Log::DefaultFormatter::Show_Tag | Log::DefaultFormatter::Show_Level | 
		              Log::DefaultFormatter::Show_NewLine);
		Log::Logger::GetDefaultLogger().AddOutput(pOutput);
		Log::Logger::GetDefaultLogger().SetLevel(Log::Level_Debug);

		pOutput->m_sOutput.clear();
		CXERR("mytag", "test {1} {2} {3} {4:'0'.3}", 123, 'x', False, 123.456);
		REQUIRE(String("[ERR] [    mytag   ]  : test 123 x false 123.456\n") == pOutput->m_sOutput);
		
		pOutput->m_sOutput.clear();
		CXERRIF(True, "mytag", "test {1} {2} {3} {4:'0'.3}", 123, 'x', False, 123.456);
		REQUIRE(String("[ERR] [    mytag   ]  : test 123 x false 123.456\n") == pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXERRIF(False, "mytag", "test {1} {2} {3} {4:'0'.3}", 123, 'x', False, 123.456);
		REQUIRE(String("") == pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXWARN("mytag", "test {1} {2} {3} {4:'0'.3}", 123, 'x', False, 123.456);
		REQUIRE(String("[WRN] [    mytag   ]  : test 123 x false 123.456\n") == pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXWARNIF(True, "mytag", "test {1} {2} {3} {4:'0'.3}", 123, 'x', False, 123.456);
		REQUIRE(String("[WRN] [    mytag   ]  : test 123 x false 123.456\n") == pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXWARNIF(False, "mytag", "test {1} {2} {3} {4:'0'.3}", 123, 'x', False, 123.456);
		REQUIRE(String("") == pOutput->m_sOutput);


		pOutput->m_sOutput.clear();
		CXINFO("mytag", "test {1} {2} {3} {4:'0'.3}", 123, 'x', False, 123.456);
		REQUIRE(String("[INF] [    mytag   ]  : test 123 x false 123.456\n") == pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXINFOIF(True, "mytag", "test {1} {2} {3} {4:'0'.3}", 123, 'x', False, 123.456);
		REQUIRE(String("[INF] [    mytag   ]  : test 123 x false 123.456\n") == pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXINFOIF(False, "mytag", "test {1} {2} {3} {4:'0'.3}", 123, 'x', False, 123.456);
		REQUIRE(String("") == pOutput->m_sOutput);


		pOutput->m_sOutput.clear();
		CXDBG("mytag", "test {1} {2} {3} {4:'0'.3}", 123, 'x', False, 123.456);
		REQUIRE(String("[DBG] [    mytag   ]  : test 123 x false 123.456\n") == pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXDBGIF(True, "mytag", "test {1} {2} {3} {4:'0'.3}", 123, 'x', False, 123.456);
		REQUIRE(String("[DBG] [    mytag   ]  : test 123 x false 123.456\n") == pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXDBGIF(False, "mytag", "test {1} {2} {3} {4:'0'.3}", 123, 'x', False, 123.456);
		REQUIRE(String("") == pOutput->m_sOutput);


		pOutput->m_sOutput.clear();
		CXVERB("mytag", "test {1} {2} {3} {4:'0'.3}", 123, 'x', False, 123.456);
		REQUIRE(String("[VRB] [    mytag   ]  : test 123 x false 123.456\n") == pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXVERBIF(True, "mytag", "test {1} {2} {3} {4:'0'.3}", 123, 'x', False, 123.456);
		REQUIRE(String("[VRB] [    mytag   ]  : test 123 x false 123.456\n") == pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXVERBIF(False, "mytag", "test {1} {2} {3} {4:'0'.3}", 123, 'x', False, 123.456);
		REQUIRE(String("") == pOutput->m_sOutput);

		Log::Logger::GetDefaultLogger().SetLevel(Log::Level_Error);

		pOutput->m_sOutput.clear();
		CXINFO("mytag", "test {1} {2} {3} {4:'0'.3}", 123, 'x', False, 123.456);
		REQUIRE(String("") == pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXINFOIF(True, "mytag", "test {1} {2} {3} {4:'0'.3}", 123, 'x', False, 123.456);
		REQUIRE(String("") == pOutput->m_sOutput);
	}

	SECTION("5 args")
	{
		MyOutput *pOutput = new (std::nothrow) MyOutput();

		((Log::DefaultFormatter *)Log::Logger::GetDefaultLogger().GetFormatter())->
		     SetFlags(Log::DefaultFormatter::Show_Tag | Log::DefaultFormatter::Show_Level | 
		              Log::DefaultFormatter::Show_NewLine);
		Log::Logger::GetDefaultLogger().AddOutput(pOutput);
		Log::Logger::GetDefaultLogger().SetLevel(Log::Level_Debug);

		pOutput->m_sOutput.clear();
		CXERR("mytag", "test {1} {2} {3} {4:'0'.3} {5}", 123, 'x', False, 123.456, "abc");
		REQUIRE(String("[ERR] [    mytag   ]  : test 123 x false 123.456 abc\n") == pOutput->m_sOutput);
		
		pOutput->m_sOutput.clear();
		CXERRIF(True, "mytag", "test {1} {2} {3} {4:'0'.3} {5}", 123, 'x', False, 123.456, "abc");
		REQUIRE(String("[ERR] [    mytag   ]  : test 123 x false 123.456 abc\n") == pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXERRIF(False, "mytag", "test {1} {2} {3} {4:'0'.3} {5}", 123, 'x', False, 123.456, "abc");
		REQUIRE(String("") == pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXWARN("mytag", "test {1} {2} {3} {4:'0'.3} {5}", 123, 'x', False, 123.456, "abc");
		REQUIRE(String("[WRN] [    mytag   ]  : test 123 x false 123.456 abc\n") == pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXWARNIF(True, "mytag", "test {1} {2} {3} {4:'0'.3} {5}", 123, 'x', False, 123.456, "abc");
		REQUIRE(String("[WRN] [    mytag   ]  : test 123 x false 123.456 abc\n") == pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXWARNIF(False, "mytag", "test {1} {2} {3} {4:'0'.3} {5}", 123, 'x', False, 123.456, "abc");
		REQUIRE(String("") == pOutput->m_sOutput);


		pOutput->m_sOutput.clear();
		CXINFO("mytag", "test {1} {2} {3} {4:'0'.3} {5}", 123, 'x', False, 123.456, "abc");
		REQUIRE(String("[INF] [    mytag   ]  : test 123 x false 123.456 abc\n") == pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXINFOIF(True, "mytag", "test {1} {2} {3} {4:'0'.3} {5}", 123, 'x', False, 123.456, "abc");
		REQUIRE(String("[INF] [    mytag   ]  : test 123 x false 123.456 abc\n") == pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXINFOIF(False, "mytag", "test {1} {2} {3} {4:'0'.3} {5}", 123, 'x', False, 123.456, "abc");
		REQUIRE(String("") == pOutput->m_sOutput);


		pOutput->m_sOutput.clear();
		CXDBG("mytag", "test {1} {2} {3} {4:'0'.3} {5}", 123, 'x', False, 123.456, "abc");
		REQUIRE(String("[DBG] [    mytag   ]  : test 123 x false 123.456 abc\n") == pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXDBGIF(True, "mytag", "test {1} {2} {3} {4:'0'.3} {5}", 123, 'x', False, 123.456, "abc");
		REQUIRE(String("[DBG] [    mytag   ]  : test 123 x false 123.456 abc\n") == pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXDBGIF(False, "mytag", "test {1} {2} {3} {4:'0'.3} {5}", 123, 'x', False, 123.456, "abc");
		REQUIRE(String("") == pOutput->m_sOutput);


		pOutput->m_sOutput.clear();
		CXVERB("mytag", "test {1} {2} {3} {4:'0'.3} {5}", 123, 'x', False, 123.456, "abc");
		REQUIRE(String("[VRB] [    mytag   ]  : test 123 x false 123.456 abc\n") == pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXVERBIF(True, "mytag", "test {1} {2} {3} {4:'0'.3} {5}", 123, 'x', False, 123.456, "abc");
		REQUIRE(String("[VRB] [    mytag   ]  : test 123 x false 123.456 abc\n") == pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXVERBIF(False, "mytag", "test {1} {2} {3} {4:'0'.3} {5}", 123, 'x', False, 123.456, "abc");
		REQUIRE(String("") == pOutput->m_sOutput);

		Log::Logger::GetDefaultLogger().SetLevel(Log::Level_Error);

		pOutput->m_sOutput.clear();
		CXINFO("mytag", "test {1} {2} {3} {4:'0'.3} {5}", 123, 'x', False, 123.456, "abc");
		REQUIRE(String("") == pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXINFOIF(True, "mytag", "test {1} {2} {3} {4:'0'.3} {5}", 123, 'x', False, 123.456, "abc");
		REQUIRE(String("") == pOutput->m_sOutput);
	}

	SECTION("6 args")
	{
		MyOutput *pOutput = new (std::nothrow) MyOutput();

		((Log::DefaultFormatter *)Log::Logger::GetDefaultLogger().GetFormatter())->
		     SetFlags(Log::DefaultFormatter::Show_Tag | Log::DefaultFormatter::Show_Level | 
		              Log::DefaultFormatter::Show_NewLine);
		Log::Logger::GetDefaultLogger().AddOutput(pOutput);
		Log::Logger::GetDefaultLogger().SetLevel(Log::Level_Debug);

		pOutput->m_sOutput.clear();
		CXERR("mytag", "test {1} {2} {3} {4:'0'.3} {5} {6}", 123, 'x', False, 123.456, "abc", 
		      (Size)100);
		REQUIRE(String("[ERR] [    mytag   ]  : test 123 x false 123.456 abc 100\n") == 
		        pOutput->m_sOutput);
		
		pOutput->m_sOutput.clear();
		CXERRIF(True, "mytag", "test {1} {2} {3} {4:'0'.3} {5} {6}", 123, 'x', False, 123.456, "abc", 
		        (Size)100);
		REQUIRE(String("[ERR] [    mytag   ]  : test 123 x false 123.456 abc 100\n") == 
		        pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXERRIF(False, "mytag", "test {1} {2} {3} {4:'0'.3} {5} {6}", 123, 'x', False, 123.456, 
		        "abc", (Size)100);
		REQUIRE(String("") == pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXWARN("mytag", "test {1} {2} {3} {4:'0'.3} {5} {6}", 123, 'x', False, 123.456, "abc", 
		       (Size)100);
		REQUIRE(String("[WRN] [    mytag   ]  : test 123 x false 123.456 abc 100\n") == 
		        pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXWARNIF(True, "mytag", "test {1} {2} {3} {4:'0'.3} {5} {6}", 123, 'x', False, 123.456, 
		         "abc", (Size)100);
		REQUIRE(String("[WRN] [    mytag   ]  : test 123 x false 123.456 abc 100\n") == 
		        pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXWARNIF(False, "mytag", "test {1} {2} {3} {4:'0'.3} {5} {6}", 123, 'x', False, 123.456, 
		         "abc", (Size)100);
		REQUIRE(String("") == pOutput->m_sOutput);


		pOutput->m_sOutput.clear();
		CXINFO("mytag", "test {1} {2} {3} {4:'0'.3} {5} {6}", 123, 'x', False, 123.456, "abc", 
		       (Size)100);
		REQUIRE(String("[INF] [    mytag   ]  : test 123 x false 123.456 abc 100\n") == 
		        pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXINFOIF(True, "mytag", "test {1} {2} {3} {4:'0'.3} {5} {6}", 123, 'x', False, 123.456, 
		         "abc", (Size)100);
		REQUIRE(String("[INF] [    mytag   ]  : test 123 x false 123.456 abc 100\n") == 
		        pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXINFOIF(False, "mytag", "test {1} {2} {3} {4:'0'.3} {5} {6}", 123, 'x', False, 123.456, 
		         "abc", (Size)100);
		REQUIRE(String("") == pOutput->m_sOutput);


		pOutput->m_sOutput.clear();
		CXDBG("mytag", "test {1} {2} {3} {4:'0'.3} {5} {6}", 123, 'x', False, 123.456, "abc", 
		      (Size)100);
		REQUIRE(String("[DBG] [    mytag   ]  : test 123 x false 123.456 abc 100\n") == 
		        pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXDBGIF(True, "mytag", "test {1} {2} {3} {4:'0'.3} {5} {6}", 123, 'x', False, 123.456, 
		        "abc", (Size)100);
		REQUIRE(String("[DBG] [    mytag   ]  : test 123 x false 123.456 abc 100\n") == 
		        pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXDBGIF(False, "mytag", "test {1} {2} {3} {4:'0'.3} {5} {6}", 123, 'x', False, 123.456, 
		        "abc", (Size)100);
		REQUIRE(String("") == pOutput->m_sOutput);


		pOutput->m_sOutput.clear();
		CXVERB("mytag", "test {1} {2} {3} {4:'0'.3} {5} {6}", 123, 'x', False, 123.456, "abc", 
		       (Size)100);
		REQUIRE(String("[VRB] [    mytag   ]  : test 123 x false 123.456 abc 100\n") == 
		        pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXVERBIF(True, "mytag", "test {1} {2} {3} {4:'0'.3} {5} {6}", 123, 'x', False, 123.456, 
		         "abc", (Size)100);
		REQUIRE(String("[VRB] [    mytag   ]  : test 123 x false 123.456 abc 100\n") == 
		         pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXVERBIF(False, "mytag", "test {1} {2} {3} {4:'0'.3} {5} {6}", 123, 'x', False, 123.456, 
		         "abc", (Size)100);
		REQUIRE(String("") == pOutput->m_sOutput);

		Log::Logger::GetDefaultLogger().SetLevel(Log::Level_Error);

		pOutput->m_sOutput.clear();
		CXINFO("mytag", "test {1} {2} {3} {4:'0'.3} {5} {6}", 123, 'x', False, 123.456, "abc", 
		       (Size)100);
		REQUIRE(String("") == pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXINFOIF(True, "mytag", "test {1} {2} {3} {4:'0'.3} {5} {6}", 123, 'x', False, 123.456, 
		         "abc", (Size)100);
		REQUIRE(String("") == pOutput->m_sOutput);
	}

	SECTION("7 args")
	{
		MyOutput *pOutput = new (std::nothrow) MyOutput();

		((Log::DefaultFormatter *)Log::Logger::GetDefaultLogger().GetFormatter())->
		     SetFlags(Log::DefaultFormatter::Show_Tag | Log::DefaultFormatter::Show_Level | 
		              Log::DefaultFormatter::Show_NewLine);
		Log::Logger::GetDefaultLogger().AddOutput(pOutput);
		Log::Logger::GetDefaultLogger().SetLevel(Log::Level_Debug);

		pOutput->m_sOutput.clear();
		CXERR("mytag", "test {1} {2} {3} {4:'0'.3} {5} {6} {7}", 123, 'x', False, 123.456, "abc", 
		      (Size)100, -100);
		REQUIRE(String("[ERR] [    mytag   ]  : test 123 x false 123.456 abc 100 -100\n") == 
		        pOutput->m_sOutput);
		
		pOutput->m_sOutput.clear();
		CXERRIF(True, "mytag", "test {1} {2} {3} {4:'0'.3} {5} {6} {7}", 123, 'x', False, 123.456, 
		        "abc", (Size)100, -100);
		REQUIRE(String("[ERR] [    mytag   ]  : test 123 x false 123.456 abc 100 -100\n") == 
		        pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXERRIF(False, "mytag", "test {1} {2} {3} {4:'0'.3} {5} {6} {7}", 123, 'x', False, 123.456, 
		        "abc", (Size)100, -100);
		REQUIRE(String("") == pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXWARN("mytag", "test {1} {2} {3} {4:'0'.3} {5} {6} {7}", 123, 'x', False, 123.456, "abc", 
		       (Size)100, -100);
		REQUIRE(String("[WRN] [    mytag   ]  : test 123 x false 123.456 abc 100 -100\n") == 
		        pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXWARNIF(True, "mytag", "test {1} {2} {3} {4:'0'.3} {5} {6} {7}", 123, 'x', False, 123.456, 
		         "abc", (Size)100, -100);
		REQUIRE(String("[WRN] [    mytag   ]  : test 123 x false 123.456 abc 100 -100\n") == 
		        pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXWARNIF(False, "mytag", "test {1} {2} {3} {4:'0'.3} {5} {6} {7}", 123, 'x', False, 123.456, 
		         "abc", (Size)100, -100);
		REQUIRE(String("") == pOutput->m_sOutput);


		pOutput->m_sOutput.clear();
		CXINFO("mytag", "test {1} {2} {3} {4:'0'.3} {5} {6} {7}", 123, 'x', False, 123.456, "abc", 
		       (Size)100, -100);
		REQUIRE(String("[INF] [    mytag   ]  : test 123 x false 123.456 abc 100 -100\n") == 
		        pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXINFOIF(True, "mytag", "test {1} {2} {3} {4:'0'.3} {5} {6} {7}", 123, 'x', False, 123.456, 
		         "abc", (Size)100, -100);
		REQUIRE(String("[INF] [    mytag   ]  : test 123 x false 123.456 abc 100 -100\n") == 
		        pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXINFOIF(False, "mytag", "test {1} {2} {3} {4:'0'.3} {5} {6} {7}", 123, 'x', False, 123.456, 
		         "abc", (Size)100, -100);
		REQUIRE(String("") == pOutput->m_sOutput);


		pOutput->m_sOutput.clear();
		CXDBG("mytag", "test {1} {2} {3} {4:'0'.3} {5} {6} {7}", 123, 'x', False, 123.456, "abc", 
		      (Size)100, -100);
		REQUIRE(String("[DBG] [    mytag   ]  : test 123 x false 123.456 abc 100 -100\n") == 
		        pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXDBGIF(True, "mytag", "test {1} {2} {3} {4:'0'.3} {5} {6} {7}", 123, 'x', False, 123.456, 
		        "abc", (Size)100, -100);
		REQUIRE(String("[DBG] [    mytag   ]  : test 123 x false 123.456 abc 100 -100\n") == 
		        pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXDBGIF(False, "mytag", "test {1} {2} {3} {4:'0'.3} {5} {6} {7}", 123, 'x', False, 123.456, 
		        "abc", (Size)100, -100);
		REQUIRE(String("") == pOutput->m_sOutput);


		pOutput->m_sOutput.clear();
		CXVERB("mytag", "test {1} {2} {3} {4:'0'.3} {5} {6} {7}", 123, 'x', False, 123.456, "abc", 
		       (Size)100, -100);
		REQUIRE(String("[VRB] [    mytag   ]  : test 123 x false 123.456 abc 100 -100\n") == 
		        pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXVERBIF(True, "mytag", "test {1} {2} {3} {4:'0'.3} {5} {6} {7}", 123, 'x', False, 123.456, 
		         "abc", (Size)100, -100);
		REQUIRE(String("[VRB] [    mytag   ]  : test 123 x false 123.456 abc 100 -100\n") == 
		         pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXVERBIF(False, "mytag", "test {1} {2} {3} {4:'0'.3} {5} {6} {7}", 123, 'x', False, 123.456, 
		         "abc", (Size)100, -100);
		REQUIRE(String("") == pOutput->m_sOutput);

		Log::Logger::GetDefaultLogger().SetLevel(Log::Level_Error);

		pOutput->m_sOutput.clear();
		CXINFO("mytag", "test {1} {2} {3} {4:'0'.3} {5} {6} {7}", 123, 'x', False, 123.456, "abc", 
		       (Size)100, -100);
		REQUIRE(String("") == pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXINFOIF(True, "mytag", "test {1} {2} {3} {4:'0'.3} {5} {6} {7}", 123, 'x', False, 123.456, 
		         "abc", (Size)100, -100);
		REQUIRE(String("") == pOutput->m_sOutput);
	}

	SECTION("8 args")
	{
		MyOutput *pOutput = new (std::nothrow) MyOutput();

		((Log::DefaultFormatter *)Log::Logger::GetDefaultLogger().GetFormatter())->
		     SetFlags(Log::DefaultFormatter::Show_Tag | Log::DefaultFormatter::Show_Level | 
		              Log::DefaultFormatter::Show_NewLine);
		Log::Logger::GetDefaultLogger().AddOutput(pOutput);
		Log::Logger::GetDefaultLogger().SetLevel(Log::Level_Debug);

		pOutput->m_sOutput.clear();
		CXERR("mytag", "test {1} {2} {3} {4:'0'.3} {5} {6} {7} {8}", 123, 'x', False, 123.456, "abc", 
		      (Size)100, -100, "xyz");
		REQUIRE(String("[ERR] [    mytag   ]  : test 123 x false 123.456 abc 100 -100 xyz\n") == 
		        pOutput->m_sOutput);
		
		pOutput->m_sOutput.clear();
		CXERRIF(True, "mytag", "test {1} {2} {3} {4:'0'.3} {5} {6} {7} {8}", 123, 'x', False, 
		        123.456, "abc", (Size)100, -100, "xyz");
		REQUIRE(String("[ERR] [    mytag   ]  : test 123 x false 123.456 abc 100 -100 xyz\n") == 
		        pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXERRIF(False, "mytag", "test {1} {2} {3} {4:'0'.3} {5} {6} {7} {8}", 123, 'x', False, 
		        123.456, "abc", (Size)100, -100, "xyz");
		REQUIRE(String("") == pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXWARN("mytag", "test {1} {2} {3} {4:'0'.3} {5} {6} {7} {8}", 123, 'x', False, 123.456, 
		       "abc", (Size)100, -100, "xyz");
		REQUIRE(String("[WRN] [    mytag   ]  : test 123 x false 123.456 abc 100 -100 xyz\n") == 
		        pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXWARNIF(True, "mytag", "test {1} {2} {3} {4:'0'.3} {5} {6} {7} {8}", 123, 'x', False, 
		         123.456, "abc", (Size)100, -100, "xyz");
		REQUIRE(String("[WRN] [    mytag   ]  : test 123 x false 123.456 abc 100 -100 xyz\n") == 
		        pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXWARNIF(False, "mytag", "test {1} {2} {3} {4:'0'.3} {5} {6} {7} {8}", 123, 'x', False, 
		         123.456, "abc", (Size)100, -100, "xyz");
		REQUIRE(String("") == pOutput->m_sOutput);


		pOutput->m_sOutput.clear();
		CXINFO("mytag", "test {1} {2} {3} {4:'0'.3} {5} {6} {7} {8}", 123, 'x', False, 123.456, 
		       "abc", (Size)100, -100, "xyz");
		REQUIRE(String("[INF] [    mytag   ]  : test 123 x false 123.456 abc 100 -100 xyz\n") == 
		        pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXINFOIF(True, "mytag", "test {1} {2} {3} {4:'0'.3} {5} {6} {7} {8}", 123, 'x', False, 
		         123.456, "abc", (Size)100, -100, "xyz");
		REQUIRE(String("[INF] [    mytag   ]  : test 123 x false 123.456 abc 100 -100 xyz\n") == 
		        pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXINFOIF(False, "mytag", "test {1} {2} {3} {4:'0'.3} {5} {6} {7} {8}", 123, 'x', False, 
		         123.456, "abc", (Size)100, -100, "xyz");
		REQUIRE(String("") == pOutput->m_sOutput);


		pOutput->m_sOutput.clear();
		CXDBG("mytag", "test {1} {2} {3} {4:'0'.3} {5} {6} {7} {8}", 123, 'x', False, 123.456, "abc", 
		      (Size)100, -100, "xyz");
		REQUIRE(String("[DBG] [    mytag   ]  : test 123 x false 123.456 abc 100 -100 xyz\n") == 
		        pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXDBGIF(True, "mytag", "test {1} {2} {3} {4:'0'.3} {5} {6} {7} {8}", 123, 'x', False, 
		        123.456, "abc", (Size)100, -100, "xyz");
		REQUIRE(String("[DBG] [    mytag   ]  : test 123 x false 123.456 abc 100 -100 xyz\n") == 
		        pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXDBGIF(False, "mytag", "test {1} {2} {3} {4:'0'.3} {5} {6} {7} {8}", 123, 'x', False, 
		        123.456, "abc", (Size)100, -100, "xyz");
		REQUIRE(String("") == pOutput->m_sOutput);


		pOutput->m_sOutput.clear();
		CXVERB("mytag", "test {1} {2} {3} {4:'0'.3} {5} {6} {7} {8}", 123, 'x', False, 123.456, 
		       "abc", (Size)100, -100, "xyz");
		REQUIRE(String("[VRB] [    mytag   ]  : test 123 x false 123.456 abc 100 -100 xyz\n") == 
		        pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXVERBIF(True, "mytag", "test {1} {2} {3} {4:'0'.3} {5} {6} {7} {8}", 123, 'x', False, 
		         123.456, "abc", (Size)100, -100, "xyz");
		REQUIRE(String("[VRB] [    mytag   ]  : test 123 x false 123.456 abc 100 -100 xyz\n") == 
		         pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXVERBIF(False, "mytag", "test {1} {2} {3} {4:'0'.3} {5} {6} {7} {8}", 123, 'x', False, 
		         123.456, "abc", (Size)100, -100, "xyz");
		REQUIRE(String("") == pOutput->m_sOutput);

		Log::Logger::GetDefaultLogger().SetLevel(Log::Level_Error);

		pOutput->m_sOutput.clear();
		CXINFO("mytag", "test {1} {2} {3} {4:'0'.3} {5} {6} {7} {8}", 123, 'x', False, 123.456, 
		       "abc", (Size)100, -100, "xyz");
		REQUIRE(String("") == pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXINFOIF(True, "mytag", "test {1} {2} {3} {4:'0'.3} {5} {6} {7} {8}", 123, 'x', False, 
		         123.456, "abc", (Size)100, -100, "xyz");
		REQUIRE(String("") == pOutput->m_sOutput);
	}

	SECTION("9 args")
	{
		MyOutput *pOutput = new (std::nothrow) MyOutput();

		((Log::DefaultFormatter *)Log::Logger::GetDefaultLogger().GetFormatter())->
		     SetFlags(Log::DefaultFormatter::Show_Tag | Log::DefaultFormatter::Show_Level | 
		              Log::DefaultFormatter::Show_NewLine);
		Log::Logger::GetDefaultLogger().AddOutput(pOutput);
		Log::Logger::GetDefaultLogger().SetLevel(Log::Level_Debug);

		pOutput->m_sOutput.clear();
		CXERR("mytag", "test {1} {2} {3} {4:'0'.3} {5} {6} {7} {8} {9:'0'.2}", 123, 'x', False, 
		      123.456, "abc", (Size)100, -100, "xyz", 12.34f);
		REQUIRE(String("[ERR] [    mytag   ]  : test 123 x false 123.456 abc 100 -100 xyz 12.34\n") 
		        == pOutput->m_sOutput);
		
		pOutput->m_sOutput.clear();
		CXERRIF(True, "mytag", "test {1} {2} {3} {4:'0'.3} {5} {6} {7} {8} {9:'0'.2}", 123, 'x', 
		        False, 123.456, "abc", (Size)100, -100, "xyz", 12.34f);
		REQUIRE(String("[ERR] [    mytag   ]  : test 123 x false 123.456 abc 100 -100 xyz 12.34\n") 
		        == pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXERRIF(False, "mytag", "test {1} {2} {3} {4:'0'.3} {5} {6} {7} {8} {9:'0'.2}", 123, 'x', 
		        False, 123.456, "abc", (Size)100, -100, "xyz", 12.34f);
		REQUIRE(String("") == pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXWARN("mytag", "test {1} {2} {3} {4:'0'.3} {5} {6} {7} {8} {9:'0'.2}", 123, 'x', False, 
		       123.456, "abc", (Size)100, -100, "xyz", 12.34f);
		REQUIRE(String("[WRN] [    mytag   ]  : test 123 x false 123.456 abc 100 -100 xyz 12.34\n") 
		        == pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXWARNIF(True, "mytag", "test {1} {2} {3} {4:'0'.3} {5} {6} {7} {8} {9:'0'.2}", 123, 'x', 
		         False, 123.456, "abc", (Size)100, -100, "xyz", 12.34f);
		REQUIRE(String("[WRN] [    mytag   ]  : test 123 x false 123.456 abc 100 -100 xyz 12.34\n") 
		        == pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXWARNIF(False, "mytag", "test {1} {2} {3} {4:'0'.3} {5} {6} {7} {8} {9:'0'.2}", 123, 'x', 
		         False, 123.456, "abc", (Size)100, -100, "xyz", 12.34f);
		REQUIRE(String("") == pOutput->m_sOutput);


		pOutput->m_sOutput.clear();
		CXINFO("mytag", "test {1} {2} {3} {4:'0'.3} {5} {6} {7} {8} {9:'0'.2}", 123, 'x', False, 
		       123.456, "abc", (Size)100, -100, "xyz", 12.34f);
		REQUIRE(String("[INF] [    mytag   ]  : test 123 x false 123.456 abc 100 -100 xyz 12.34\n") 
		        == pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXINFOIF(True, "mytag", "test {1} {2} {3} {4:'0'.3} {5} {6} {7} {8} {9:'0'.2}", 123, 'x', 
		         False, 123.456, "abc", (Size)100, -100, "xyz", 12.34f);
		REQUIRE(String("[INF] [    mytag   ]  : test 123 x false 123.456 abc 100 -100 xyz 12.34\n") 
		        == pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXINFOIF(False, "mytag", "test {1} {2} {3} {4:'0'.3} {5} {6} {7} {8} {9:'0'.2}", 123, 'x', 
		         False, 123.456, "abc", (Size)100, -100, "xyz", 12.34f);
		REQUIRE(String("") == pOutput->m_sOutput);


		pOutput->m_sOutput.clear();
		CXDBG("mytag", "test {1} {2} {3} {4:'0'.3} {5} {6} {7} {8} {9:'0'.2}", 123, 'x', False, 
		      123.456, "abc", (Size)100, -100, "xyz", 12.34f);
		REQUIRE(String("[DBG] [    mytag   ]  : test 123 x false 123.456 abc 100 -100 xyz 12.34\n") 
		        == pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXDBGIF(True, "mytag", "test {1} {2} {3} {4:'0'.3} {5} {6} {7} {8} {9:'0'.2}", 123, 'x', 
		        False, 123.456, "abc", (Size)100, -100, "xyz", 12.34f);
		REQUIRE(String("[DBG] [    mytag   ]  : test 123 x false 123.456 abc 100 -100 xyz 12.34\n") 
		        == pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXDBGIF(False, "mytag", "test {1} {2} {3} {4:'0'.3} {5} {6} {7} {8} {9:'0'.2}", 123, 'x', 
		        False, 123.456, "abc", (Size)100, -100, "xyz", 12.34f);
		REQUIRE(String("") == pOutput->m_sOutput);


		pOutput->m_sOutput.clear();
		CXVERB("mytag", "test {1} {2} {3} {4:'0'.3} {5} {6} {7} {8} {9:'0'.2}", 123, 'x', False, 
		       123.456, "abc", (Size)100, -100, "xyz", 12.34f);
		REQUIRE(String("[VRB] [    mytag   ]  : test 123 x false 123.456 abc 100 -100 xyz 12.34\n") 
		        == pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXVERBIF(True, "mytag", "test {1} {2} {3} {4:'0'.3} {5} {6} {7} {8} {9:'0'.2}", 123, 'x', 
		         False, 123.456, "abc", (Size)100, -100, "xyz", 12.34f);
		REQUIRE(String("[VRB] [    mytag   ]  : test 123 x false 123.456 abc 100 -100 xyz 12.34\n") 
		        == pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXVERBIF(False, "mytag", "test {1} {2} {3} {4:'0'.3} {5} {6} {7} {8} {9:'0'.2}", 123, 'x', 
		         False, 123.456, "abc", (Size)100, -100, "xyz", 12.34f);
		REQUIRE(String("") == pOutput->m_sOutput);

		Log::Logger::GetDefaultLogger().SetLevel(Log::Level_Error);

		pOutput->m_sOutput.clear();
		CXINFO("mytag", "test {1} {2} {3} {4:'0'.3} {5} {6} {7} {8} {9:'0'.2}", 123, 'x', False, 
		       123.456, "abc", (Size)100, -100, "xyz", 12.34f);
		REQUIRE(String("") == pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXINFOIF(True, "mytag", "test {1} {2} {3} {4:'0'.3} {5} {6} {7} {8} {9:'0'.2}", 123, 'x', 
		         False, 123.456, "abc", (Size)100, -100, "xyz", 12.34f);
		REQUIRE(String("") == pOutput->m_sOutput);
	}

	SECTION("10 args")
	{
		MyOutput *pOutput = new (std::nothrow) MyOutput();

		((Log::DefaultFormatter *)Log::Logger::GetDefaultLogger().GetFormatter())->
		     SetFlags(Log::DefaultFormatter::Show_Tag | Log::DefaultFormatter::Show_Level | 
		              Log::DefaultFormatter::Show_NewLine);
		Log::Logger::GetDefaultLogger().AddOutput(pOutput);
		Log::Logger::GetDefaultLogger().SetLevel(Log::Level_Debug);

		pOutput->m_sOutput.clear();
		CXERR("mytag", "test {1} {2} {3} {4:'0'.3} {5} {6} {7} {8} {9:'0'.2} {10}", 123, 'x', False, 
		      123.456, "abc", (Size)100, -100, "xyz", 12.34f, 1);
		REQUIRE(String("[ERR] [    mytag   ]  : test 123 x false 123.456 abc 100 -100 xyz 12.34 1\n") 
		        == pOutput->m_sOutput);
		
		pOutput->m_sOutput.clear();
		CXERRIF(True, "mytag", "test {1} {2} {3} {4:'0'.3} {5} {6} {7} {8} {9:'0'.2} {10}", 123, 'x', 
		        False, 123.456, "abc", (Size)100, -100, "xyz", 12.34f, 1);
		REQUIRE(String("[ERR] [    mytag   ]  : test 123 x false 123.456 abc 100 -100 xyz 12.34 1\n") 
		        == pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXERRIF(False, "mytag", "test {1} {2} {3} {4:'0'.3} {5} {6} {7} {8} {9:'0'.2} {10}", 123, 
		        'x', False, 123.456, "abc", (Size)100, -100, "xyz", 12.34f, 1);
		REQUIRE(String("") == pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXWARN("mytag", "test {1} {2} {3} {4:'0'.3} {5} {6} {7} {8} {9:'0'.2} {10}", 123, 'x', False, 
		       123.456, "abc", (Size)100, -100, "xyz", 12.34f, 1);
		REQUIRE(String("[WRN] [    mytag   ]  : test 123 x false 123.456 abc 100 -100 xyz 12.34 1\n") 
		        == pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXWARNIF(True, "mytag", "test {1} {2} {3} {4:'0'.3} {5} {6} {7} {8} {9:'0'.2} {10}", 123, 
		         'x', False, 123.456, "abc", (Size)100, -100, "xyz", 12.34f, 1);
		REQUIRE(String("[WRN] [    mytag   ]  : test 123 x false 123.456 abc 100 -100 xyz 12.34 1\n") 
		        == pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXWARNIF(False, "mytag", "test {1} {2} {3} {4:'0'.3} {5} {6} {7} {8} {9:'0'.2} {10}", 123, 
		         'x', False, 123.456, "abc", (Size)100, -100, "xyz", 12.34f, 1);
		REQUIRE(String("") == pOutput->m_sOutput);


		pOutput->m_sOutput.clear();
		CXINFO("mytag", "test {1} {2} {3} {4:'0'.3} {5} {6} {7} {8} {9:'0'.2} {10}", 123, 'x', False, 
		       123.456, "abc", (Size)100, -100, "xyz", 12.34f, 1);
		REQUIRE(String("[INF] [    mytag   ]  : test 123 x false 123.456 abc 100 -100 xyz 12.34 1\n") 
		        == pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXINFOIF(True, "mytag", "test {1} {2} {3} {4:'0'.3} {5} {6} {7} {8} {9:'0'.2} {10}", 123, 
		         'x', False, 123.456, "abc", (Size)100, -100, "xyz", 12.34f, 1);
		REQUIRE(String("[INF] [    mytag   ]  : test 123 x false 123.456 abc 100 -100 xyz 12.34 1\n") 
		        == pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXINFOIF(False, "mytag", "test {1} {2} {3} {4:'0'.3} {5} {6} {7} {8} {9:'0'.2} {10}", 123, 
		         'x', False, 123.456, "abc", (Size)100, -100, "xyz", 12.34f, 1);
		REQUIRE(String("") == pOutput->m_sOutput);


		pOutput->m_sOutput.clear();
		CXDBG("mytag", "test {1} {2} {3} {4:'0'.3} {5} {6} {7} {8} {9:'0'.2} {10}", 123, 'x', False, 
		      123.456, "abc", (Size)100, -100, "xyz", 12.34f, 1);
		REQUIRE(String("[DBG] [    mytag   ]  : test 123 x false 123.456 abc 100 -100 xyz 12.34 1\n") 
		        == pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXDBGIF(True, "mytag", "test {1} {2} {3} {4:'0'.3} {5} {6} {7} {8} {9:'0'.2} {10}", 123, 'x', 
		        False, 123.456, "abc", (Size)100, -100, "xyz", 12.34f, 1);
		REQUIRE(String("[DBG] [    mytag   ]  : test 123 x false 123.456 abc 100 -100 xyz 12.34 1\n") 
		        == pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXDBGIF(False, "mytag", "test {1} {2} {3} {4:'0'.3} {5} {6} {7} {8} {9:'0'.2} {10}", 123, 
		        'x', False, 123.456, "abc", (Size)100, -100, "xyz", 12.34f, 1);
		REQUIRE(String("") == pOutput->m_sOutput);


		pOutput->m_sOutput.clear();
		CXVERB("mytag", "test {1} {2} {3} {4:'0'.3} {5} {6} {7} {8} {9:'0'.2} {10}", 123, 'x', False, 
		       123.456, "abc", (Size)100, -100, "xyz", 12.34f, 1);
		REQUIRE(String("[VRB] [    mytag   ]  : test 123 x false 123.456 abc 100 -100 xyz 12.34 1\n") 
		        == pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXVERBIF(True, "mytag", "test {1} {2} {3} {4:'0'.3} {5} {6} {7} {8} {9:'0'.2} {10}", 123, 
		         'x', False, 123.456, "abc", (Size)100, -100, "xyz", 12.34f, 1);
		REQUIRE(String("[VRB] [    mytag   ]  : test 123 x false 123.456 abc 100 -100 xyz 12.34 1\n") 
		        == pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXVERBIF(False, "mytag", "test {1} {2} {3} {4:'0'.3} {5} {6} {7} {8} {9:'0'.2} {10}", 123, 
		         'x', False, 123.456, "abc", (Size)100, -100, "xyz", 12.34f, 1);
		REQUIRE(String("") == pOutput->m_sOutput);

		Log::Logger::GetDefaultLogger().SetLevel(Log::Level_Error);

		pOutput->m_sOutput.clear();
		CXINFO("mytag", "test {1} {2} {3} {4:'0'.3} {5} {6} {7} {8} {9:'0'.2} {10}", 123, 'x', False, 
		       123.456, "abc", (Size)100, -100, "xyz", 12.34f, 1);
		REQUIRE(String("") == pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXINFOIF(True, "mytag", "test {1} {2} {3} {4:'0'.3} {5} {6} {7} {8} {9:'0'.2} {10}", 123, 
		         'x', False, 123.456, "abc", (Size)100, -100, "xyz", 12.34f, 1);
		REQUIRE(String("") == pOutput->m_sOutput);
	}

	SECTION("11 args")
	{
		MyOutput *pOutput = new (std::nothrow) MyOutput();

		((Log::DefaultFormatter *)Log::Logger::GetDefaultLogger().GetFormatter())->
		     SetFlags(Log::DefaultFormatter::Show_Tag | Log::DefaultFormatter::Show_Level | 
		              Log::DefaultFormatter::Show_NewLine);
		Log::Logger::GetDefaultLogger().AddOutput(pOutput);
		Log::Logger::GetDefaultLogger().SetLevel(Log::Level_Debug);

		pOutput->m_sOutput.clear();
		CXERR("mytag", "test {1} {2} {3} {4:'0'.3} {5} {6} {7} {8} {9:'0'.2} {10} {11}", 123, 'x', 
		      False, 123.456, "abc", (Size)100, -100, "xyz", 12.34f, 1, 2);
		REQUIRE(String("[ERR] [    mytag   ]  : test 123 x false 123.456 abc 100 -100 xyz 12.34 1 "
		        "2\n") == pOutput->m_sOutput);
		
		pOutput->m_sOutput.clear();
		CXERRIF(True, "mytag", "test {1} {2} {3} {4:'0'.3} {5} {6} {7} {8} {9:'0'.2} {10} {11}", 123, 
		        'x', False, 123.456, "abc", (Size)100, -100, "xyz", 12.34f, 1, 2);
		REQUIRE(String("[ERR] [    mytag   ]  : test 123 x false 123.456 abc 100 -100 xyz 12.34 1 "
		        "2\n") == pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXERRIF(False, "mytag", "test {1} {2} {3} {4:'0'.3} {5} {6} {7} {8} {9:'0'.2} {10} {11}", 
		        123, 'x', False, 123.456, "abc", (Size)100, -100, "xyz", 12.34f, 1, 2);
		REQUIRE(String("") == pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXWARN("mytag", "test {1} {2} {3} {4:'0'.3} {5} {6} {7} {8} {9:'0'.2} {10} {11}", 123, 'x', 
		       False, 123.456, "abc", (Size)100, -100, "xyz", 12.34f, 1, 2);
		REQUIRE(String("[WRN] [    mytag   ]  : test 123 x false 123.456 abc 100 -100 xyz 12.34 1 "
		        "2\n") == pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXWARNIF(True, "mytag", "test {1} {2} {3} {4:'0'.3} {5} {6} {7} {8} {9:'0'.2} {10} {11}", 
		         123, 'x', False, 123.456, "abc", (Size)100, -100, "xyz", 12.34f, 1, 2);
		REQUIRE(String("[WRN] [    mytag   ]  : test 123 x false 123.456 abc 100 -100 xyz 12.34 1 "
		        "2\n") == pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXWARNIF(False, "mytag", "test {1} {2} {3} {4:'0'.3} {5} {6} {7} {8} {9:'0'.2} {10} {11}", 
		         123, 'x', False, 123.456, "abc", (Size)100, -100, "xyz", 12.34f, 1, 2);
		REQUIRE(String("") == pOutput->m_sOutput);


		pOutput->m_sOutput.clear();
		CXINFO("mytag", "test {1} {2} {3} {4:'0'.3} {5} {6} {7} {8} {9:'0'.2} {10} {11}", 123, 'x', 
		       False, 123.456, "abc", (Size)100, -100, "xyz", 12.34f, 1, 2);
		REQUIRE(String("[INF] [    mytag   ]  : test 123 x false 123.456 abc 100 -100 xyz 12.34 1 "
		        "2\n") == pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXINFOIF(True, "mytag", "test {1} {2} {3} {4:'0'.3} {5} {6} {7} {8} {9:'0'.2} {10} {11}", 
		         123, 'x', False, 123.456, "abc", (Size)100, -100, "xyz", 12.34f, 1, 2);
		REQUIRE(String("[INF] [    mytag   ]  : test 123 x false 123.456 abc 100 -100 xyz 12.34 1 "
		        "2\n") == pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXINFOIF(False, "mytag", "test {1} {2} {3} {4:'0'.3} {5} {6} {7} {8} {9:'0'.2} {10} {11}", 
		         123, 'x', False, 123.456, "abc", (Size)100, -100, "xyz", 12.34f, 1, 2);
		REQUIRE(String("") == pOutput->m_sOutput);


		pOutput->m_sOutput.clear();
		CXDBG("mytag", "test {1} {2} {3} {4:'0'.3} {5} {6} {7} {8} {9:'0'.2} {10} {11}", 123, 'x', 
		      False, 123.456, "abc", (Size)100, -100, "xyz", 12.34f, 1, 2);
		REQUIRE(String("[DBG] [    mytag   ]  : test 123 x false 123.456 abc 100 -100 xyz 12.34 1 "
		        "2\n") == pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXDBGIF(True, "mytag", "test {1} {2} {3} {4:'0'.3} {5} {6} {7} {8} {9:'0'.2} {10} {11}", 123, 
		        'x', False, 123.456, "abc", (Size)100, -100, "xyz", 12.34f, 1, 2);
		REQUIRE(String("[DBG] [    mytag   ]  : test 123 x false 123.456 abc 100 -100 xyz 12.34 1 "
		        "2\n") == pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXDBGIF(False, "mytag", "test {1} {2} {3} {4:'0'.3} {5} {6} {7} {8} {9:'0'.2} {10} {11}", 
		        123, 'x', False, 123.456, "abc", (Size)100, -100, "xyz", 12.34f, 1, 2);
		REQUIRE(String("") == pOutput->m_sOutput);


		pOutput->m_sOutput.clear();
		CXVERB("mytag", "test {1} {2} {3} {4:'0'.3} {5} {6} {7} {8} {9:'0'.2} {10} {11}", 123, 'x', 
		       False, 123.456, "abc", (Size)100, -100, "xyz", 12.34f, 1, 2);
		REQUIRE(String("[VRB] [    mytag   ]  : test 123 x false 123.456 abc 100 -100 xyz 12.34 1 "
		        "2\n") == pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXVERBIF(True, "mytag", "test {1} {2} {3} {4:'0'.3} {5} {6} {7} {8} {9:'0'.2} {10} {11}", 
		         123, 'x', False, 123.456, "abc", (Size)100, -100, "xyz", 12.34f, 1, 2);
		REQUIRE(String("[VRB] [    mytag   ]  : test 123 x false 123.456 abc 100 -100 xyz 12.34 1 "
		        "2\n") == pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXVERBIF(False, "mytag", "test {1} {2} {3} {4:'0'.3} {5} {6} {7} {8} {9:'0'.2} {10} {11}", 
		         123, 'x', False, 123.456, "abc", (Size)100, -100, "xyz", 12.34f, 1, 2);
		REQUIRE(String("") == pOutput->m_sOutput);

		Log::Logger::GetDefaultLogger().SetLevel(Log::Level_Error);

		pOutput->m_sOutput.clear();
		CXINFO("mytag", "test {1} {2} {3} {4:'0'.3} {5} {6} {7} {8} {9:'0'.2} {10} {11}", 123, 'x', 
		       False, 123.456, "abc", (Size)100, -100, "xyz", 12.34f, 1, 2);
		REQUIRE(String("") == pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXINFOIF(True, "mytag", "test {1} {2} {3} {4:'0'.3} {5} {6} {7} {8} {9:'0'.2} {10} {11}", 
		         123, 'x', False, 123.456, "abc", (Size)100, -100, "xyz", 12.34f, 1, 2);
		REQUIRE(String("") == pOutput->m_sOutput);
	}

	SECTION("12 args")
	{
		MyOutput *pOutput = new (std::nothrow) MyOutput();

		((Log::DefaultFormatter *)Log::Logger::GetDefaultLogger().GetFormatter())->
		     SetFlags(Log::DefaultFormatter::Show_Tag | Log::DefaultFormatter::Show_Level | 
		              Log::DefaultFormatter::Show_NewLine);
		Log::Logger::GetDefaultLogger().AddOutput(pOutput);
		Log::Logger::GetDefaultLogger().SetLevel(Log::Level_Debug);

		pOutput->m_sOutput.clear();
		CXERR("mytag", "test {1} {2} {3} {4:'0'.3} {5} {6} {7} {8} {9:'0'.2} {10} {11} {12}", 123, 
		      'x', False, 123.456, "abc", (Size)100, -100, "xyz", 12.34f, 1, 2, 3);
		REQUIRE(String("[ERR] [    mytag   ]  : test 123 x false 123.456 abc 100 -100 xyz 12.34 1 "
		        "2 3\n") == pOutput->m_sOutput);
		
		pOutput->m_sOutput.clear();
		CXERRIF(True, "mytag", "test {1} {2} {3} {4:'0'.3} {5} {6} {7} {8} {9:'0'.2} {10} {11} {12}", 
		        123, 'x', False, 123.456, "abc", (Size)100, -100, "xyz", 12.34f, 1, 2, 3);
		REQUIRE(String("[ERR] [    mytag   ]  : test 123 x false 123.456 abc 100 -100 xyz 12.34 1 "
		        "2 3\n") == pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXERRIF(False, "mytag", "test {1} {2} {3} {4:'0'.3} {5} {6} {7} {8} {9:'0'.2} {10} {11} "
		        "{12}", 123, 'x', False, 123.456, "abc", (Size)100, -100, "xyz", 12.34f, 1, 2, 3);
		REQUIRE(String("") == pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXWARN("mytag", "test {1} {2} {3} {4:'0'.3} {5} {6} {7} {8} {9:'0'.2} {10} {11} {12}", 123, 
		       'x', False, 123.456, "abc", (Size)100, -100, "xyz", 12.34f, 1, 2, 3);
		REQUIRE(String("[WRN] [    mytag   ]  : test 123 x false 123.456 abc 100 -100 xyz 12.34 1 "
		        "2 3\n") == pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXWARNIF(True, "mytag", "test {1} {2} {3} {4:'0'.3} {5} {6} {7} {8} {9:'0'.2} {10} {11} "
		         "{12}", 123, 'x', False, 123.456, "abc", (Size)100, -100, "xyz", 12.34f, 1, 2, 3);
		REQUIRE(String("[WRN] [    mytag   ]  : test 123 x false 123.456 abc 100 -100 xyz 12.34 1 "
		        "2 3\n") == pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXWARNIF(False, "mytag", "test {1} {2} {3} {4:'0'.3} {5} {6} {7} {8} {9:'0'.2} {10} {11} "
		         "{12}", 123, 'x', False, 123.456, "abc", (Size)100, -100, "xyz", 12.34f, 1, 2, 3);
		REQUIRE(String("") == pOutput->m_sOutput);


		pOutput->m_sOutput.clear();
		CXINFO("mytag", "test {1} {2} {3} {4:'0'.3} {5} {6} {7} {8} {9:'0'.2} {10} {11} {12}", 123, 
		       'x', False, 123.456, "abc", (Size)100, -100, "xyz", 12.34f, 1, 2, 3);
		REQUIRE(String("[INF] [    mytag   ]  : test 123 x false 123.456 abc 100 -100 xyz 12.34 1 "
		        "2 3\n") == pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXINFOIF(True, "mytag", "test {1} {2} {3} {4:'0'.3} {5} {6} {7} {8} {9:'0'.2} {10} {11} "
		         "{12}", 123, 'x', False, 123.456, "abc", (Size)100, -100, "xyz", 12.34f, 1, 2, 3);
		REQUIRE(String("[INF] [    mytag   ]  : test 123 x false 123.456 abc 100 -100 xyz 12.34 1 "
		        "2 3\n") == pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXINFOIF(False, "mytag", "test {1} {2} {3} {4:'0'.3} {5} {6} {7} {8} {9:'0'.2} {10} {11} "
		         "{12}", 123, 'x', False, 123.456, "abc", (Size)100, -100, "xyz", 12.34f, 1, 2, 3);
		REQUIRE(String("") == pOutput->m_sOutput);


		pOutput->m_sOutput.clear();
		CXDBG("mytag", "test {1} {2} {3} {4:'0'.3} {5} {6} {7} {8} {9:'0'.2} {10} {11} {12}", 123, 
		      'x', False, 123.456, "abc", (Size)100, -100, "xyz", 12.34f, 1, 2, 3);
		REQUIRE(String("[DBG] [    mytag   ]  : test 123 x false 123.456 abc 100 -100 xyz 12.34 1 "
		        "2 3\n") == pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXDBGIF(True, "mytag", "test {1} {2} {3} {4:'0'.3} {5} {6} {7} {8} {9:'0'.2} {10} {11} {12}", 
		        123, 'x', False, 123.456, "abc", (Size)100, -100, "xyz", 12.34f, 1, 2, 3);
		REQUIRE(String("[DBG] [    mytag   ]  : test 123 x false 123.456 abc 100 -100 xyz 12.34 1 "
		        "2 3\n") == pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXDBGIF(False, "mytag", "test {1} {2} {3} {4:'0'.3} {5} {6} {7} {8} {9:'0'.2} {10} {11} "
		        "{12}", 123, 'x', False, 123.456, "abc", (Size)100, -100, "xyz", 12.34f, 1, 2, 3);
		REQUIRE(String("") == pOutput->m_sOutput);


		pOutput->m_sOutput.clear();
		CXVERB("mytag", "test {1} {2} {3} {4:'0'.3} {5} {6} {7} {8} {9:'0'.2} {10} {11} {12}", 123, 
		       'x', False, 123.456, "abc", (Size)100, -100, "xyz", 12.34f, 1, 2, 3);
		REQUIRE(String("[VRB] [    mytag   ]  : test 123 x false 123.456 abc 100 -100 xyz 12.34 1 "
		        "2 3\n") == pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXVERBIF(True, "mytag", "test {1} {2} {3} {4:'0'.3} {5} {6} {7} {8} {9:'0'.2} {10} {11} "
		         "{12}", 123, 'x', False, 123.456, "abc", (Size)100, -100, "xyz", 12.34f, 1, 2, 3);
		REQUIRE(String("[VRB] [    mytag   ]  : test 123 x false 123.456 abc 100 -100 xyz 12.34 1 "
		        "2 3\n") == pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXVERBIF(False, "mytag", "test {1} {2} {3} {4:'0'.3} {5} {6} {7} {8} {9:'0'.2} {10} {11} "
		         "{12}", 123, 'x', False, 123.456, "abc", (Size)100, -100, "xyz", 12.34f, 1, 2, 3);
		REQUIRE(String("") == pOutput->m_sOutput);

		Log::Logger::GetDefaultLogger().SetLevel(Log::Level_Error);

		pOutput->m_sOutput.clear();
		CXINFO("mytag", "test {1} {2} {3} {4:'0'.3} {5} {6} {7} {8} {9:'0'.2} {10} {11} {12}", 123, 
		       'x', False, 123.456, "abc", (Size)100, -100, "xyz", 12.34f, 1, 2, 3);
		REQUIRE(String("") == pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXINFOIF(True, "mytag", "test {1} {2} {3} {4:'0'.3} {5} {6} {7} {8} {9:'0'.2} {10} {11} "
		         "{12}", 123, 'x', False, 123.456, "abc", (Size)100, -100, "xyz", 12.34f, 1, 2, 3);
		REQUIRE(String("") == pOutput->m_sOutput);
	}

	SECTION("13 args")
	{
		MyOutput *pOutput = new (std::nothrow) MyOutput();

		((Log::DefaultFormatter *)Log::Logger::GetDefaultLogger().GetFormatter())->
		     SetFlags(Log::DefaultFormatter::Show_Tag | Log::DefaultFormatter::Show_Level | 
		              Log::DefaultFormatter::Show_NewLine);
		Log::Logger::GetDefaultLogger().AddOutput(pOutput);
		Log::Logger::GetDefaultLogger().SetLevel(Log::Level_Debug);

		pOutput->m_sOutput.clear();
		CXERR("mytag", "test {1} {2} {3} {4:'0'.3} {5} {6} {7} {8} {9:'0'.2} {10} {11} {12} {13}", 
		      123, 'x', False, 123.456, "abc", (Size)100, -100, "xyz", 12.34f, 1, 2, 3, 4);
		REQUIRE(String("[ERR] [    mytag   ]  : test 123 x false 123.456 abc 100 -100 xyz 12.34 1 "
		        "2 3 4\n") == pOutput->m_sOutput);
		
		pOutput->m_sOutput.clear();
		CXERRIF(True, "mytag", "test {1} {2} {3} {4:'0'.3} {5} {6} {7} {8} {9:'0'.2} {10} {11} {12} "
		        "{13}", 123, 'x', False, 123.456, "abc", (Size)100, -100, "xyz", 12.34f, 1, 2, 3, 4);
		REQUIRE(String("[ERR] [    mytag   ]  : test 123 x false 123.456 abc 100 -100 xyz 12.34 1 "
		        "2 3 4\n") == pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXERRIF(False, "mytag", "test {1} {2} {3} {4:'0'.3} {5} {6} {7} {8} {9:'0'.2} {10} {11} "
		        "{12} {13}", 123, 'x', False, 123.456, "abc", (Size)100, -100, "xyz", 12.34f, 1, 2, 
		        3, 4);
		REQUIRE(String("") == pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXWARN("mytag", "test {1} {2} {3} {4:'0'.3} {5} {6} {7} {8} {9:'0'.2} {10} {11} {12} {13}", 
		       123, 'x', False, 123.456, "abc", (Size)100, -100, "xyz", 12.34f, 1, 2, 3, 4);
		REQUIRE(String("[WRN] [    mytag   ]  : test 123 x false 123.456 abc 100 -100 xyz 12.34 1 "
		        "2 3 4\n") == pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXWARNIF(True, "mytag", "test {1} {2} {3} {4:'0'.3} {5} {6} {7} {8} {9:'0'.2} {10} {11} "
		         "{12} {13}", 123, 'x', False, 123.456, "abc", (Size)100, -100, "xyz", 12.34f, 1, 2, 
		         3, 4);
		REQUIRE(String("[WRN] [    mytag   ]  : test 123 x false 123.456 abc 100 -100 xyz 12.34 1 "
		        "2 3 4\n") == pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXWARNIF(False, "mytag", "test {1} {2} {3} {4:'0'.3} {5} {6} {7} {8} {9:'0'.2} {10} {11} "
		         "{12} {13}", 123, 'x', False, 123.456, "abc", (Size)100, -100, "xyz", 12.34f, 1, 2, 
		         3, 4);
		REQUIRE(String("") == pOutput->m_sOutput);


		pOutput->m_sOutput.clear();
		CXINFO("mytag", "test {1} {2} {3} {4:'0'.3} {5} {6} {7} {8} {9:'0'.2} {10} {11} {12} {13}", 
		       123, 'x', False, 123.456, "abc", (Size)100, -100, "xyz", 12.34f, 1, 2, 3, 4);
		REQUIRE(String("[INF] [    mytag   ]  : test 123 x false 123.456 abc 100 -100 xyz 12.34 1 "
		        "2 3 4\n") == pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXINFOIF(True, "mytag", "test {1} {2} {3} {4:'0'.3} {5} {6} {7} {8} {9:'0'.2} {10} {11} "
		         "{12} {13}", 123, 'x', False, 123.456, "abc", (Size)100, -100, "xyz", 12.34f, 1, 2, 
		         3, 4);
		REQUIRE(String("[INF] [    mytag   ]  : test 123 x false 123.456 abc 100 -100 xyz 12.34 1 "
		        "2 3 4\n") == pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXINFOIF(False, "mytag", "test {1} {2} {3} {4:'0'.3} {5} {6} {7} {8} {9:'0'.2} {10} {11} "
		         "{12} {13}", 123, 'x', False, 123.456, "abc", (Size)100, -100, "xyz", 12.34f, 1, 2, 
		         3, 4);
		REQUIRE(String("") == pOutput->m_sOutput);


		pOutput->m_sOutput.clear();
		CXDBG("mytag", "test {1} {2} {3} {4:'0'.3} {5} {6} {7} {8} {9:'0'.2} {10} {11} {12} {13}", 
		      123, 'x', False, 123.456, "abc", (Size)100, -100, "xyz", 12.34f, 1, 2, 3, 4);
		REQUIRE(String("[DBG] [    mytag   ]  : test 123 x false 123.456 abc 100 -100 xyz 12.34 1 "
		        "2 3 4\n") == pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXDBGIF(True, "mytag", "test {1} {2} {3} {4:'0'.3} {5} {6} {7} {8} {9:'0'.2} {10} {11} {12} "
		        "{13}", 123, 'x', False, 123.456, "abc", (Size)100, -100, "xyz", 12.34f, 1, 2, 3, 4);
		REQUIRE(String("[DBG] [    mytag   ]  : test 123 x false 123.456 abc 100 -100 xyz 12.34 1 "
		        "2 3 4\n") == pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXDBGIF(False, "mytag", "test {1} {2} {3} {4:'0'.3} {5} {6} {7} {8} {9:'0'.2} {10} {11} "
		        "{12} {13}", 123, 'x', False, 123.456, "abc", (Size)100, -100, "xyz", 12.34f, 1, 2, 
		        3, 4);
		REQUIRE(String("") == pOutput->m_sOutput);


		pOutput->m_sOutput.clear();
		CXVERB("mytag", "test {1} {2} {3} {4:'0'.3} {5} {6} {7} {8} {9:'0'.2} {10} {11} {12} {13}", 
		       123, 'x', False, 123.456, "abc", (Size)100, -100, "xyz", 12.34f, 1, 2, 3, 4);
		REQUIRE(String("[VRB] [    mytag   ]  : test 123 x false 123.456 abc 100 -100 xyz 12.34 1 "
		        "2 3 4\n") == pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXVERBIF(True, "mytag", "test {1} {2} {3} {4:'0'.3} {5} {6} {7} {8} {9:'0'.2} {10} {11} "
		         "{12} {13}", 123, 'x', False, 123.456, "abc", (Size)100, -100, "xyz", 12.34f, 1, 2, 
		         3, 4);
		REQUIRE(String("[VRB] [    mytag   ]  : test 123 x false 123.456 abc 100 -100 xyz 12.34 1 "
		        "2 3 4\n") == pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXVERBIF(False, "mytag", "test {1} {2} {3} {4:'0'.3} {5} {6} {7} {8} {9:'0'.2} {10} {11} "
		         "{12} {13}", 123, 'x', False, 123.456, "abc", (Size)100, -100, "xyz", 12.34f, 1, 2, 
		         3, 4);
		REQUIRE(String("") == pOutput->m_sOutput);

		Log::Logger::GetDefaultLogger().SetLevel(Log::Level_Error);

		pOutput->m_sOutput.clear();
		CXINFO("mytag", "test {1} {2} {3} {4:'0'.3} {5} {6} {7} {8} {9:'0'.2} {10} {11} {12} {13}", 
		       123, 'x', False, 123.456, "abc", (Size)100, -100, "xyz", 12.34f, 1, 2, 3, 4);
		REQUIRE(String("") == pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXINFOIF(True, "mytag", "test {1} {2} {3} {4:'0'.3} {5} {6} {7} {8} {9:'0'.2} {10} {11} "
		         "{12} {13}", 123, 'x', False, 123.456, "abc", (Size)100, -100, "xyz", 12.34f, 1, 2, 
		         3, 4);
		REQUIRE(String("") == pOutput->m_sOutput);
	}

	SECTION("14 args")
	{
		MyOutput *pOutput = new (std::nothrow) MyOutput();

		((Log::DefaultFormatter *)Log::Logger::GetDefaultLogger().GetFormatter())->
		     SetFlags(Log::DefaultFormatter::Show_Tag | Log::DefaultFormatter::Show_Level | 
		              Log::DefaultFormatter::Show_NewLine);
		Log::Logger::GetDefaultLogger().AddOutput(pOutput);
		Log::Logger::GetDefaultLogger().SetLevel(Log::Level_Debug);

		pOutput->m_sOutput.clear();
		CXERR("mytag", "test {1} {2} {3} {4:'0'.3} {5} {6} {7} {8} {9:'0'.2} {10} {11} {12} {13} "
		      "{14}", 123, 'x', False, 123.456, "abc", (Size)100, -100, "xyz", 12.34f, 1, 2, 3, 4, 5);
		REQUIRE(String("[ERR] [    mytag   ]  : test 123 x false 123.456 abc 100 -100 xyz 12.34 1 "
		        "2 3 4 5\n") == pOutput->m_sOutput);
		
		pOutput->m_sOutput.clear();
		CXERRIF(True, "mytag", "test {1} {2} {3} {4:'0'.3} {5} {6} {7} {8} {9:'0'.2} {10} {11} {12} "
		        "{13} {14}", 123, 'x', False, 123.456, "abc", (Size)100, -100, "xyz", 12.34f, 1, 2, 
		        3, 4, 5);
		REQUIRE(String("[ERR] [    mytag   ]  : test 123 x false 123.456 abc 100 -100 xyz 12.34 1 "
		        "2 3 4 5\n") == pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXERRIF(False, "mytag", "test {1} {2} {3} {4:'0'.3} {5} {6} {7} {8} {9:'0'.2} {10} {11} "
		        "{12} {13} {14}", 123, 'x', False, 123.456, "abc", (Size)100, -100, "xyz", 12.34f, 1, 
		        2, 3, 4, 5);
		REQUIRE(String("") == pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXWARN("mytag", "test {1} {2} {3} {4:'0'.3} {5} {6} {7} {8} {9:'0'.2} {10} {11} {12} {13} "
		       "{14}", 123, 'x', False, 123.456, "abc", (Size)100, -100, "xyz", 12.34f, 1, 2, 3, 4, 
		       5);
		REQUIRE(String("[WRN] [    mytag   ]  : test 123 x false 123.456 abc 100 -100 xyz 12.34 1 "
		        "2 3 4 5\n") == pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXWARNIF(True, "mytag", "test {1} {2} {3} {4:'0'.3} {5} {6} {7} {8} {9:'0'.2} {10} {11} "
		         "{12} {13} {14}", 123, 'x', False, 123.456, "abc", (Size)100, -100, "xyz", 12.34f, 
		         1, 2, 3, 4, 5);
		REQUIRE(String("[WRN] [    mytag   ]  : test 123 x false 123.456 abc 100 -100 xyz 12.34 1 "
		        "2 3 4 5\n") == pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXWARNIF(False, "mytag", "test {1} {2} {3} {4:'0'.3} {5} {6} {7} {8} {9:'0'.2} {10} {11} "
		         "{12} {13} {14}", 123, 'x', False, 123.456, "abc", (Size)100, -100, "xyz", 12.34f, 
		         1, 2, 3, 4, 5);
		REQUIRE(String("") == pOutput->m_sOutput);


		pOutput->m_sOutput.clear();
		CXINFO("mytag", "test {1} {2} {3} {4:'0'.3} {5} {6} {7} {8} {9:'0'.2} {10} {11} {12} {13} "
		       "{14}", 123, 'x', False, 123.456, "abc", (Size)100, -100, "xyz", 12.34f, 1, 2, 3, 4, 
		       5);
		REQUIRE(String("[INF] [    mytag   ]  : test 123 x false 123.456 abc 100 -100 xyz 12.34 1 "
		        "2 3 4 5\n") == pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXINFOIF(True, "mytag", "test {1} {2} {3} {4:'0'.3} {5} {6} {7} {8} {9:'0'.2} {10} {11} "
		         "{12} {13} {14}", 123, 'x', False, 123.456, "abc", (Size)100, -100, "xyz", 12.34f, 
		         1, 2, 3, 4, 5);
		REQUIRE(String("[INF] [    mytag   ]  : test 123 x false 123.456 abc 100 -100 xyz 12.34 1 "
		        "2 3 4 5\n") == pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXINFOIF(False, "mytag", "test {1} {2} {3} {4:'0'.3} {5} {6} {7} {8} {9:'0'.2} {10} {11} "
		         "{12} {13} {14}", 123, 'x', False, 123.456, "abc", (Size)100, -100, "xyz", 12.34f, 
		         1, 2, 3, 4, 5);
		REQUIRE(String("") == pOutput->m_sOutput);


		pOutput->m_sOutput.clear();
		CXDBG("mytag", "test {1} {2} {3} {4:'0'.3} {5} {6} {7} {8} {9:'0'.2} {10} {11} {12} {13} "
		      "{14}", 123, 'x', False, 123.456, "abc", (Size)100, -100, "xyz", 12.34f, 1, 2, 3, 4, 5);
		REQUIRE(String("[DBG] [    mytag   ]  : test 123 x false 123.456 abc 100 -100 xyz 12.34 1 "
		        "2 3 4 5\n") == pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXDBGIF(True, "mytag", "test {1} {2} {3} {4:'0'.3} {5} {6} {7} {8} {9:'0'.2} {10} {11} {12} "
		        "{13} {14}", 123, 'x', False, 123.456, "abc", (Size)100, -100, "xyz", 12.34f, 1, 2, 
		        3, 4, 5);
		REQUIRE(String("[DBG] [    mytag   ]  : test 123 x false 123.456 abc 100 -100 xyz 12.34 1 "
		        "2 3 4 5\n") == pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXDBGIF(False, "mytag", "test {1} {2} {3} {4:'0'.3} {5} {6} {7} {8} {9:'0'.2} {10} {11} "
		        "{12} {13} {14}", 123, 'x', False, 123.456, "abc", (Size)100, -100, "xyz", 12.34f, 1, 
		        2, 3, 4, 5);
		REQUIRE(String("") == pOutput->m_sOutput);


		pOutput->m_sOutput.clear();
		CXVERB("mytag", "test {1} {2} {3} {4:'0'.3} {5} {6} {7} {8} {9:'0'.2} {10} {11} {12} {13} "
		       "{14}", 123, 'x', False, 123.456, "abc", (Size)100, -100, "xyz", 12.34f, 1, 2, 3, 4, 
		       5);
		REQUIRE(String("[VRB] [    mytag   ]  : test 123 x false 123.456 abc 100 -100 xyz 12.34 1 "
		        "2 3 4 5\n") == pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXVERBIF(True, "mytag", "test {1} {2} {3} {4:'0'.3} {5} {6} {7} {8} {9:'0'.2} {10} {11} "
		         "{12} {13} {14}", 123, 'x', False, 123.456, "abc", (Size)100, -100, "xyz", 12.34f, 
		         1, 2, 3, 4, 5);
		REQUIRE(String("[VRB] [    mytag   ]  : test 123 x false 123.456 abc 100 -100 xyz 12.34 1 "
		        "2 3 4 5\n") == pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXVERBIF(False, "mytag", "test {1} {2} {3} {4:'0'.3} {5} {6} {7} {8} {9:'0'.2} {10} {11} "
		         "{12} {13} {14}", 123, 'x', False, 123.456, "abc", (Size)100, -100, "xyz", 12.34f, 
		         1, 2, 3, 4, 5);
		REQUIRE(String("") == pOutput->m_sOutput);

		Log::Logger::GetDefaultLogger().SetLevel(Log::Level_Error);

		pOutput->m_sOutput.clear();
		CXINFO("mytag", "test {1} {2} {3} {4:'0'.3} {5} {6} {7} {8} {9:'0'.2} {10} {11} {12} {13} "
		       "{14}", 123, 'x', False, 123.456, "abc", (Size)100, -100, "xyz", 12.34f, 1, 2, 3, 4, 
		       5);
		REQUIRE(String("") == pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXINFOIF(True, "mytag", "test {1} {2} {3} {4:'0'.3} {5} {6} {7} {8} {9:'0'.2} {10} {11} "
		         "{12} {13} {14}", 123, 'x', False, 123.456, "abc", (Size)100, -100, "xyz", 12.34f, 
		         1, 2, 3, 4, 5);
		REQUIRE(String("") == pOutput->m_sOutput);
	}

	SECTION("15 args")
	{
		MyOutput *pOutput = new (std::nothrow) MyOutput();

		((Log::DefaultFormatter *)Log::Logger::GetDefaultLogger().GetFormatter())->
		     SetFlags(Log::DefaultFormatter::Show_Tag | Log::DefaultFormatter::Show_Level | 
		              Log::DefaultFormatter::Show_NewLine);
		Log::Logger::GetDefaultLogger().AddOutput(pOutput);
		Log::Logger::GetDefaultLogger().SetLevel(Log::Level_Debug);

		pOutput->m_sOutput.clear();
		CXERR("mytag", "test {1} {2} {3} {4:'0'.3} {5} {6} {7} {8} {9:'0'.2} {10} {11} {12} {13} "
		      "{14} {15}", 123, 'x', False, 123.456, "abc", (Size)100, -100, "xyz", 12.34f, 1, 2, 3, 
		      4, 5, 6);
		REQUIRE(String("[ERR] [    mytag   ]  : test 123 x false 123.456 abc 100 -100 xyz 12.34 1 "
		        "2 3 4 5 6\n") == pOutput->m_sOutput);
		
		pOutput->m_sOutput.clear();
		CXERRIF(True, "mytag", "test {1} {2} {3} {4:'0'.3} {5} {6} {7} {8} {9:'0'.2} {10} {11} {12} "
		        "{13} {14} {15}", 123, 'x', False, 123.456, "abc", (Size)100, -100, "xyz", 12.34f, 1,
		        2, 3, 4, 5, 6);
		REQUIRE(String("[ERR] [    mytag   ]  : test 123 x false 123.456 abc 100 -100 xyz 12.34 1 "
		        "2 3 4 5 6\n") == pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXERRIF(False, "mytag", "test {1} {2} {3} {4:'0'.3} {5} {6} {7} {8} {9:'0'.2} {10} {11} "
		        "{12} {13} {14} {15}", 123, 'x', False, 123.456, "abc", (Size)100, -100, "xyz", 
		        12.34f, 1, 2, 3, 4, 5, 6);
		REQUIRE(String("") == pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXWARN("mytag", "test {1} {2} {3} {4:'0'.3} {5} {6} {7} {8} {9:'0'.2} {10} {11} {12} {13} "
		       "{14} {15}", 123, 'x', False, 123.456, "abc", (Size)100, -100, "xyz", 12.34f, 1, 2, 3, 
		       4, 5, 6);
		REQUIRE(String("[WRN] [    mytag   ]  : test 123 x false 123.456 abc 100 -100 xyz 12.34 1 "
		        "2 3 4 5 6\n") == pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXWARNIF(True, "mytag", "test {1} {2} {3} {4:'0'.3} {5} {6} {7} {8} {9:'0'.2} {10} {11} "
		         "{12} {13} {14} {15}", 123, 'x', False, 123.456, "abc", (Size)100, -100, "xyz", 
		         12.34f, 1, 2, 3, 4, 5, 6);
		REQUIRE(String("[WRN] [    mytag   ]  : test 123 x false 123.456 abc 100 -100 xyz 12.34 1 "
		        "2 3 4 5 6\n") == pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXWARNIF(False, "mytag", "test {1} {2} {3} {4:'0'.3} {5} {6} {7} {8} {9:'0'.2} {10} {11} "
		         "{12} {13} {14} {15}", 123, 'x', False, 123.456, "abc", (Size)100, -100, "xyz", 
		         12.34f, 1, 2, 3, 4, 5, 6);
		REQUIRE(String("") == pOutput->m_sOutput);


		pOutput->m_sOutput.clear();
		CXINFO("mytag", "test {1} {2} {3} {4:'0'.3} {5} {6} {7} {8} {9:'0'.2} {10} {11} {12} {13} "
		       "{14} {15}", 123, 'x', False, 123.456, "abc", (Size)100, -100, "xyz", 12.34f, 1, 2, 3, 
		       4, 5, 6);
		REQUIRE(String("[INF] [    mytag   ]  : test 123 x false 123.456 abc 100 -100 xyz 12.34 1 "
		        "2 3 4 5 6\n") == pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXINFOIF(True, "mytag", "test {1} {2} {3} {4:'0'.3} {5} {6} {7} {8} {9:'0'.2} {10} {11} "
		         "{12} {13} {14} {15}", 123, 'x', False, 123.456, "abc", (Size)100, -100, "xyz", 
		         12.34f, 1, 2, 3, 4, 5, 6);
		REQUIRE(String("[INF] [    mytag   ]  : test 123 x false 123.456 abc 100 -100 xyz 12.34 1 "
		        "2 3 4 5 6\n") == pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXINFOIF(False, "mytag", "test {1} {2} {3} {4:'0'.3} {5} {6} {7} {8} {9:'0'.2} {10} {11} "
		         "{12} {13} {14} {15}", 123, 'x', False, 123.456, "abc", (Size)100, -100, "xyz", 
		         12.34f, 1, 2, 3, 4, 5, 6);
		REQUIRE(String("") == pOutput->m_sOutput);


		pOutput->m_sOutput.clear();
		CXDBG("mytag", "test {1} {2} {3} {4:'0'.3} {5} {6} {7} {8} {9:'0'.2} {10} {11} {12} {13} "
		      "{14} {15}", 123, 'x', False, 123.456, "abc", (Size)100, -100, "xyz", 12.34f, 1, 2, 3, 
		      4, 5, 6);
		REQUIRE(String("[DBG] [    mytag   ]  : test 123 x false 123.456 abc 100 -100 xyz 12.34 1 "
		        "2 3 4 5 6\n") == pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXDBGIF(True, "mytag", "test {1} {2} {3} {4:'0'.3} {5} {6} {7} {8} {9:'0'.2} {10} {11} {12} "
		        "{13} {14} {15}", 123, 'x', False, 123.456, "abc", (Size)100, -100, "xyz", 12.34f, 1, 
		        2, 3, 4, 5, 6);
		REQUIRE(String("[DBG] [    mytag   ]  : test 123 x false 123.456 abc 100 -100 xyz 12.34 1 "
		        "2 3 4 5 6\n") == pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXDBGIF(False, "mytag", "test {1} {2} {3} {4:'0'.3} {5} {6} {7} {8} {9:'0'.2} {10} {11} "
		        "{12} {13} {14} {15}", 123, 'x', False, 123.456, "abc", (Size)100, -100, "xyz", 
		        12.34f, 1, 2, 3, 4, 5, 6);
		REQUIRE(String("") == pOutput->m_sOutput);


		pOutput->m_sOutput.clear();
		CXVERB("mytag", "test {1} {2} {3} {4:'0'.3} {5} {6} {7} {8} {9:'0'.2} {10} {11} {12} {13} "
		       "{14} {15}", 123, 'x', False, 123.456, "abc", (Size)100, -100, "xyz", 12.34f, 1, 2, 3, 
		       4, 5, 6);
		REQUIRE(String("[VRB] [    mytag   ]  : test 123 x false 123.456 abc 100 -100 xyz 12.34 1 "
		        "2 3 4 5 6\n") == pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXVERBIF(True, "mytag", "test {1} {2} {3} {4:'0'.3} {5} {6} {7} {8} {9:'0'.2} {10} {11} "
		         "{12} {13} {14} {15}", 123, 'x', False, 123.456, "abc", (Size)100, -100, "xyz", 
		         12.34f, 1, 2, 3, 4, 5, 6);
		REQUIRE(String("[VRB] [    mytag   ]  : test 123 x false 123.456 abc 100 -100 xyz 12.34 1 "
		        "2 3 4 5 6\n") == pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXVERBIF(False, "mytag", "test {1} {2} {3} {4:'0'.3} {5} {6} {7} {8} {9:'0'.2} {10} {11} "
		         "{12} {13} {14} {15}", 123, 'x', False, 123.456, "abc", (Size)100, -100, "xyz", 
		         12.34f, 1, 2, 3, 4, 5, 6);
		REQUIRE(String("") == pOutput->m_sOutput);

		Log::Logger::GetDefaultLogger().SetLevel(Log::Level_Error);

		pOutput->m_sOutput.clear();
		CXINFO("mytag", "test {1} {2} {3} {4:'0'.3} {5} {6} {7} {8} {9:'0'.2} {10} {11} {12} {13} "
		       "{14} {15}", 123, 'x', False, 123.456, "abc", (Size)100, -100, "xyz", 12.34f, 1, 2, 3, 
		       4, 5, 6);
		REQUIRE(String("") == pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXINFOIF(True, "mytag", "test {1} {2} {3} {4:'0'.3} {5} {6} {7} {8} {9:'0'.2} {10} {11} "
		         "{12} {13} {14} {15}", 123, 'x', False, 123.456, "abc", (Size)100, -100, "xyz", 
		         12.34f, 1, 2, 3, 4, 5, 6);
		REQUIRE(String("") == pOutput->m_sOutput);
	}

	SECTION("16 args")
	{
		MyOutput *pOutput = new (std::nothrow) MyOutput();

		((Log::DefaultFormatter *)Log::Logger::GetDefaultLogger().GetFormatter())->
		     SetFlags(Log::DefaultFormatter::Show_Tag | Log::DefaultFormatter::Show_Level | 
		              Log::DefaultFormatter::Show_NewLine);
		Log::Logger::GetDefaultLogger().AddOutput(pOutput);
		Log::Logger::GetDefaultLogger().SetLevel(Log::Level_Debug);

		pOutput->m_sOutput.clear();
		CXERR("mytag", "test {1} {2} {3} {4:'0'.3} {5} {6} {7} {8} {9:'0'.2} {10} {11} {12} {13} "
		      "{14} {15} {16}", 123, 'x', False, 123.456, "abc", (Size)100, -100, "xyz", 12.34f, 1, 
		      2, 3, 4, 5, 6, 7);
		REQUIRE(String("[ERR] [    mytag   ]  : test 123 x false 123.456 abc 100 -100 xyz 12.34 1 "
		        "2 3 4 5 6 7\n") == pOutput->m_sOutput);
		
		pOutput->m_sOutput.clear();
		CXERRIF(True, "mytag", "test {1} {2} {3} {4:'0'.3} {5} {6} {7} {8} {9:'0'.2} {10} {11} {12} "
		        "{13} {14} {15} {16}", 123, 'x', False, 123.456, "abc", (Size)100, -100, "xyz", 
		        12.34f, 1, 2, 3, 4, 5, 6, 7);
		REQUIRE(String("[ERR] [    mytag   ]  : test 123 x false 123.456 abc 100 -100 xyz 12.34 1 "
		        "2 3 4 5 6 7\n") == pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXERRIF(False, "mytag", "test {1} {2} {3} {4:'0'.3} {5} {6} {7} {8} {9:'0'.2} {10} {11} "
		        "{12} {13} {14} {15} {16}", 123, 'x', False, 123.456, "abc", (Size)100, -100, "xyz", 
		        12.34f, 1, 2, 3, 4, 5, 6, 7);
		REQUIRE(String("") == pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXWARN("mytag", "test {1} {2} {3} {4:'0'.3} {5} {6} {7} {8} {9:'0'.2} {10} {11} {12} {13} "
		       "{14} {15} {16}", 123, 'x', False, 123.456, "abc", (Size)100, -100, "xyz", 12.34f, 1, 
		       2, 3, 4, 5, 6, 7);
		REQUIRE(String("[WRN] [    mytag   ]  : test 123 x false 123.456 abc 100 -100 xyz 12.34 1 "
		        "2 3 4 5 6 7\n") == pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXWARNIF(True, "mytag", "test {1} {2} {3} {4:'0'.3} {5} {6} {7} {8} {9:'0'.2} {10} {11} "
		         "{12} {13} {14} {15} {16}", 123, 'x', False, 123.456, "abc", (Size)100, -100, "xyz", 
		         12.34f, 1, 2, 3, 4, 5, 6, 7);
		REQUIRE(String("[WRN] [    mytag   ]  : test 123 x false 123.456 abc 100 -100 xyz 12.34 1 "
		        "2 3 4 5 6 7\n") == pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXWARNIF(False, "mytag", "test {1} {2} {3} {4:'0'.3} {5} {6} {7} {8} {9:'0'.2} {10} {11} "
		         "{12} {13} {14} {15} {16}", 123, 'x', False, 123.456, "abc", (Size)100, -100, "xyz", 
		         12.34f, 1, 2, 3, 4, 5, 6, 7);
		REQUIRE(String("") == pOutput->m_sOutput);


		pOutput->m_sOutput.clear();
		CXINFO("mytag", "test {1} {2} {3} {4:'0'.3} {5} {6} {7} {8} {9:'0'.2} {10} {11} {12} {13} "
		       "{14} {15} {16}", 123, 'x', False, 123.456, "abc", (Size)100, -100, "xyz", 12.34f, 1, 
		       2, 3, 4, 5, 6, 7);
		REQUIRE(String("[INF] [    mytag   ]  : test 123 x false 123.456 abc 100 -100 xyz 12.34 1 "
		        "2 3 4 5 6 7\n") == pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXINFOIF(True, "mytag", "test {1} {2} {3} {4:'0'.3} {5} {6} {7} {8} {9:'0'.2} {10} {11} "
		         "{12} {13} {14} {15} {16}", 123, 'x', False, 123.456, "abc", (Size)100, -100, "xyz", 
		         12.34f, 1, 2, 3, 4, 5, 6, 7);
		REQUIRE(String("[INF] [    mytag   ]  : test 123 x false 123.456 abc 100 -100 xyz 12.34 1 "
		        "2 3 4 5 6 7\n") == pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXINFOIF(False, "mytag", "test {1} {2} {3} {4:'0'.3} {5} {6} {7} {8} {9:'0'.2} {10} {11} "
		         "{12} {13} {14} {15} {16}", 123, 'x', False, 123.456, "abc", (Size)100, -100, "xyz", 
		         12.34f, 1, 2, 3, 4, 5, 6, 7);
		REQUIRE(String("") == pOutput->m_sOutput);


		pOutput->m_sOutput.clear();
		CXDBG("mytag", "test {1} {2} {3} {4:'0'.3} {5} {6} {7} {8} {9:'0'.2} {10} {11} {12} {13} "
		      "{14} {15} {16}", 123, 'x', False, 123.456, "abc", (Size)100, -100, "xyz", 12.34f, 1, 
		      2, 3, 4, 5, 6, 7);
		REQUIRE(String("[DBG] [    mytag   ]  : test 123 x false 123.456 abc 100 -100 xyz 12.34 1 "
		        "2 3 4 5 6 7\n") == pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXDBGIF(True, "mytag", "test {1} {2} {3} {4:'0'.3} {5} {6} {7} {8} {9:'0'.2} {10} {11} {12} "
		        "{13} {14} {15} {16}", 123, 'x', False, 123.456, "abc", (Size)100, -100, "xyz", 
		        12.34f, 1, 2, 3, 4, 5, 6, 7);
		REQUIRE(String("[DBG] [    mytag   ]  : test 123 x false 123.456 abc 100 -100 xyz 12.34 1 "
		        "2 3 4 5 6 7\n") == pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXDBGIF(False, "mytag", "test {1} {2} {3} {4:'0'.3} {5} {6} {7} {8} {9:'0'.2} {10} {11} "
		        "{12} {13} {14} {15} {16}", 123, 'x', False, 123.456, "abc", (Size)100, -100, "xyz", 
		        12.34f, 1, 2, 3, 4, 5, 6, 7);
		REQUIRE(String("") == pOutput->m_sOutput);


		pOutput->m_sOutput.clear();
		CXVERB("mytag", "test {1} {2} {3} {4:'0'.3} {5} {6} {7} {8} {9:'0'.2} {10} {11} {12} {13} "
		       "{14} {15} {16}", 123, 'x', False, 123.456, "abc", (Size)100, -100, "xyz", 12.34f, 1, 
		       2, 3, 4, 5, 6, 7);
		REQUIRE(String("[VRB] [    mytag   ]  : test 123 x false 123.456 abc 100 -100 xyz 12.34 1 "
		        "2 3 4 5 6 7\n") == pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXVERBIF(True, "mytag", "test {1} {2} {3} {4:'0'.3} {5} {6} {7} {8} {9:'0'.2} {10} {11} "
		         "{12} {13} {14} {15} {16}", 123, 'x', False, 123.456, "abc", (Size)100, -100, "xyz", 
		         12.34f, 1, 2, 3, 4, 5, 6, 7);
		REQUIRE(String("[VRB] [    mytag   ]  : test 123 x false 123.456 abc 100 -100 xyz 12.34 1 "
		        "2 3 4 5 6 7\n") == pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXVERBIF(False, "mytag", "test {1} {2} {3} {4:'0'.3} {5} {6} {7} {8} {9:'0'.2} {10} {11} "
		         "{12} {13} {14} {15} {16}", 123, 'x', False, 123.456, "abc", (Size)100, -100, "xyz", 
		         12.34f, 1, 2, 3, 4, 5, 6, 7);
		REQUIRE(String("") == pOutput->m_sOutput);

		Log::Logger::GetDefaultLogger().SetLevel(Log::Level_Error);

		pOutput->m_sOutput.clear();
		CXINFO("mytag", "test {1} {2} {3} {4:'0'.3} {5} {6} {7} {8} {9:'0'.2} {10} {11} {12} {13} "
		       "{14} {15} {16}", 123, 'x', False, 123.456, "abc", (Size)100, -100, "xyz", 12.34f, 1, 
		       2, 3, 4, 5, 6, 7);
		REQUIRE(String("") == pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXINFOIF(True, "mytag", "test {1} {2} {3} {4:'0'.3} {5} {6} {7} {8} {9:'0'.2} {10} {11} "
		         "{12} {13} {14} {15} {16}", 123, 'x', False, 123.456, "abc", (Size)100, -100, "xyz", 
		         12.34f, 1, 2, 3, 4, 5, 6, 7);
		REQUIRE(String("") == pOutput->m_sOutput);
	}
}

