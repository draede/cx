
#include "CX/CX.h"
#include "CX/Log/Logger.h"
#include "CX/Log/DefaultFormatter.h"
#include "../../../Contrib/Catch/Include/catch.hpp"


using namespace CX;


class MyOutput : public Log::IOutput
{
public:

	String m_sOutput;

	virtual Status Write(Log::Level nLevel, const Char *szTag, const Char *pBuffer, Size cLen)
	{
		m_sOutput.assign(pBuffer, cLen);

		return Status();
	}

};

TEST_CASE("Log::Logger tests", "[CX::Log::Logger]")
{
	SECTION("no args")
	{
		MyOutput *pOutput = New<MyOutput>();

		((Log::DefaultFormatter *)Log::Logger::GetDefaultLogger().GetFormatter())->
		     SetFlags(Log::DefaultFormatter::Show_Tag | Log::DefaultFormatter::Show_Level | 
		              Log::DefaultFormatter::Show_NewLine);
		Log::Logger::GetDefaultLogger().AddOutput(pOutput);
		Log::Logger::GetDefaultLogger().SetLevel(Log::Level_Debug);

		pOutput->m_sOutput.clear();
		CXERR("mytag", "test");
		REQUIRE(String("[ERR] [    mytag   ]  : test\n") == pOutput->m_sOutput);
		
		pOutput->m_sOutput.clear();
		CXERRIF(true, "mytag", "test");
		REQUIRE(String("[ERR] [    mytag   ]  : test\n") == pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXERRIF(false, "mytag", "test");
		REQUIRE(String("") == pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXWARN("mytag", "test");
		REQUIRE(String("[WRN] [    mytag   ]  : test\n") == pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXWARNIF(true, "mytag", "test");
		REQUIRE(String("[WRN] [    mytag   ]  : test\n") == pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXWARNIF(false, "mytag", "test");
		REQUIRE(String("") == pOutput->m_sOutput);


		pOutput->m_sOutput.clear();
		CXINFO("mytag", "test");
		REQUIRE(String("[INF] [    mytag   ]  : test\n") == pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXINFOIF(true, "mytag", "test");
		REQUIRE(String("[INF] [    mytag   ]  : test\n") == pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXINFOIF(false, "mytag", "test");
		REQUIRE(String("") == pOutput->m_sOutput);


		pOutput->m_sOutput.clear();
		CXDBG("mytag", "test");
		REQUIRE(String("[DBG] [    mytag   ]  : test\n") == pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXDBGIF(true, "mytag", "test");
		REQUIRE(String("[DBG] [    mytag   ]  : test\n") == pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXDBGIF(false, "mytag", "test");
		REQUIRE(String("") == pOutput->m_sOutput);


		pOutput->m_sOutput.clear();
		CXVERB("mytag", "test");
		REQUIRE(String("[VRB] [    mytag   ]  : test\n") == pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXVERBIF(true, "mytag", "test");
		REQUIRE(String("[VRB] [    mytag   ]  : test\n") == pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXVERBIF(false, "mytag", "test");
		REQUIRE(String("") == pOutput->m_sOutput);

		Log::Logger::GetDefaultLogger().SetLevel(Log::Level_Error);

		pOutput->m_sOutput.clear();
		CXINFO("mytag", "test");
		REQUIRE(String("") == pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXINFOIF(true, "mytag", "test");
		REQUIRE(String("") == pOutput->m_sOutput);
	}

	SECTION("1 arg")
	{
		MyOutput *pOutput = New<MyOutput>();

		((Log::DefaultFormatter *)Log::Logger::GetDefaultLogger().GetFormatter())->
		     SetFlags(Log::DefaultFormatter::Show_Tag | Log::DefaultFormatter::Show_Level | 
		              Log::DefaultFormatter::Show_NewLine);
		Log::Logger::GetDefaultLogger().AddOutput(pOutput);
		Log::Logger::GetDefaultLogger().SetLevel(Log::Level_Debug);

		pOutput->m_sOutput.clear();
		CXERR("mytag", "test {1}", 123);
		REQUIRE(String("[ERR] [    mytag   ]  : test 123\n") == pOutput->m_sOutput);
		
		pOutput->m_sOutput.clear();
		CXERRIF(true, "mytag", "test {1}", 123);
		REQUIRE(String("[ERR] [    mytag   ]  : test 123\n") == pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXERRIF(false, "mytag", "test {1}", 123);
		REQUIRE(String("") == pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXWARN("mytag", "test {1}", 123);
		REQUIRE(String("[WRN] [    mytag   ]  : test 123\n") == pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXWARNIF(true, "mytag", "test {1}", 123);
		REQUIRE(String("[WRN] [    mytag   ]  : test 123\n") == pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXWARNIF(false, "mytag", "test {1}", 123);
		REQUIRE(String("") == pOutput->m_sOutput);


		pOutput->m_sOutput.clear();
		CXINFO("mytag", "test {1}", 123);
		REQUIRE(String("[INF] [    mytag   ]  : test 123\n") == pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXINFOIF(true, "mytag", "test {1}", 123);
		REQUIRE(String("[INF] [    mytag   ]  : test 123\n") == pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXINFOIF(false, "mytag", "test {1}", 123);
		REQUIRE(String("") == pOutput->m_sOutput);


		pOutput->m_sOutput.clear();
		CXDBG("mytag", "test {1}", 123);
		REQUIRE(String("[DBG] [    mytag   ]  : test 123\n") == pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXDBGIF(true, "mytag", "test {1}", 123);
		REQUIRE(String("[DBG] [    mytag   ]  : test 123\n") == pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXDBGIF(false, "mytag", "test {1}", 123);
		REQUIRE(String("") == pOutput->m_sOutput);


		pOutput->m_sOutput.clear();
		CXVERB("mytag", "test {1}", 123);
		REQUIRE(String("[VRB] [    mytag   ]  : test 123\n") == pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXVERBIF(true, "mytag", "test {1}", 123);
		REQUIRE(String("[VRB] [    mytag   ]  : test 123\n") == pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXVERBIF(false, "mytag", "test {1}", 123);
		REQUIRE(String("") == pOutput->m_sOutput);

		Log::Logger::GetDefaultLogger().SetLevel(Log::Level_Error);

		pOutput->m_sOutput.clear();
		CXINFO("mytag", "test {1}", 123);
		REQUIRE(String("") == pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXINFOIF(true, "mytag", "test {1}", 123);
		REQUIRE(String("") == pOutput->m_sOutput);
	}

	SECTION("2 args")
	{
		MyOutput *pOutput = New<MyOutput>();

		((Log::DefaultFormatter *)Log::Logger::GetDefaultLogger().GetFormatter())->
		     SetFlags(Log::DefaultFormatter::Show_Tag | Log::DefaultFormatter::Show_Level | 
		              Log::DefaultFormatter::Show_NewLine);
		Log::Logger::GetDefaultLogger().AddOutput(pOutput);
		Log::Logger::GetDefaultLogger().SetLevel(Log::Level_Debug);

		pOutput->m_sOutput.clear();
		CXERR("mytag", "test {1} {2}", 123, 'x');
		REQUIRE(String("[ERR] [    mytag   ]  : test 123 x\n") == pOutput->m_sOutput);
		
		pOutput->m_sOutput.clear();
		CXERRIF(true, "mytag", "test {1} {2}", 123, 'x');
		REQUIRE(String("[ERR] [    mytag   ]  : test 123 x\n") == pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXERRIF(false, "mytag", "test {1} {2}", 123, 'x');
		REQUIRE(String("") == pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXWARN("mytag", "test {1} {2}", 123, 'x');
		REQUIRE(String("[WRN] [    mytag   ]  : test 123 x\n") == pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXWARNIF(true, "mytag", "test {1} {2}", 123, 'x');
		REQUIRE(String("[WRN] [    mytag   ]  : test 123 x\n") == pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXWARNIF(false, "mytag", "test {1} {2}", 123, 'x');
		REQUIRE(String("") == pOutput->m_sOutput);


		pOutput->m_sOutput.clear();
		CXINFO("mytag", "test {1} {2}", 123, 'x');
		REQUIRE(String("[INF] [    mytag   ]  : test 123 x\n") == pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXINFOIF(true, "mytag", "test {1} {2}", 123, 'x');
		REQUIRE(String("[INF] [    mytag   ]  : test 123 x\n") == pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXINFOIF(false, "mytag", "test {1} {2}", 123, 'x');
		REQUIRE(String("") == pOutput->m_sOutput);


		pOutput->m_sOutput.clear();
		CXDBG("mytag", "test {1} {2}", 123, 'x');
		REQUIRE(String("[DBG] [    mytag   ]  : test 123 x\n") == pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXDBGIF(true, "mytag", "test {1} {2}", 123, 'x');
		REQUIRE(String("[DBG] [    mytag   ]  : test 123 x\n") == pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXDBGIF(false, "mytag", "test {1} {2}", 123, 'x');
		REQUIRE(String("") == pOutput->m_sOutput);


		pOutput->m_sOutput.clear();
		CXVERB("mytag", "test {1} {2}", 123, 'x');
		REQUIRE(String("[VRB] [    mytag   ]  : test 123 x\n") == pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXVERBIF(true, "mytag", "test {1} {2}", 123, 'x');
		REQUIRE(String("[VRB] [    mytag   ]  : test 123 x\n") == pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXVERBIF(false, "mytag", "test {1} {2}", 123, 'x');
		REQUIRE(String("") == pOutput->m_sOutput);

		Log::Logger::GetDefaultLogger().SetLevel(Log::Level_Error);

		pOutput->m_sOutput.clear();
		CXINFO("mytag", "test {1} {2}", 123, 'x');
		REQUIRE(String("") == pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXINFOIF(true, "mytag", "test {1} {2}", 123, 'x');
		REQUIRE(String("") == pOutput->m_sOutput);
	}

	SECTION("3 args")
	{
		MyOutput *pOutput = New<MyOutput>();

		((Log::DefaultFormatter *)Log::Logger::GetDefaultLogger().GetFormatter())->
		     SetFlags(Log::DefaultFormatter::Show_Tag | Log::DefaultFormatter::Show_Level | 
		              Log::DefaultFormatter::Show_NewLine);
		Log::Logger::GetDefaultLogger().AddOutput(pOutput);
		Log::Logger::GetDefaultLogger().SetLevel(Log::Level_Debug);

		pOutput->m_sOutput.clear();
		CXERR("mytag", "test {1} {2} {3}", 123, 'x', false);
		REQUIRE(String("[ERR] [    mytag   ]  : test 123 x false\n") == pOutput->m_sOutput);
		
		pOutput->m_sOutput.clear();
		CXERRIF(true, "mytag", "test {1} {2} {3}", 123, 'x', false);
		REQUIRE(String("[ERR] [    mytag   ]  : test 123 x false\n") == pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXERRIF(false, "mytag", "test {1} {2} {3}", 123, 'x', false);
		REQUIRE(String("") == pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXWARN("mytag", "test {1} {2} {3}", 123, 'x', false);
		REQUIRE(String("[WRN] [    mytag   ]  : test 123 x false\n") == pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXWARNIF(true, "mytag", "test {1} {2} {3}", 123, 'x', false);
		REQUIRE(String("[WRN] [    mytag   ]  : test 123 x false\n") == pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXWARNIF(false, "mytag", "test {1} {2} {3}", 123, 'x', false);
		REQUIRE(String("") == pOutput->m_sOutput);


		pOutput->m_sOutput.clear();
		CXINFO("mytag", "test {1} {2} {3}", 123, 'x', false);
		REQUIRE(String("[INF] [    mytag   ]  : test 123 x false\n") == pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXINFOIF(true, "mytag", "test {1} {2} {3}", 123, 'x', false);
		REQUIRE(String("[INF] [    mytag   ]  : test 123 x false\n") == pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXINFOIF(false, "mytag", "test {1} {2} {3}", 123, 'x', false);
		REQUIRE(String("") == pOutput->m_sOutput);


		pOutput->m_sOutput.clear();
		CXDBG("mytag", "test {1} {2} {3}", 123, 'x', false);
		REQUIRE(String("[DBG] [    mytag   ]  : test 123 x false\n") == pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXDBGIF(true, "mytag", "test {1} {2} {3}", 123, 'x', false);
		REQUIRE(String("[DBG] [    mytag   ]  : test 123 x false\n") == pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXDBGIF(false, "mytag", "test {1} {2} {3}", 123, 'x', false);
		REQUIRE(String("") == pOutput->m_sOutput);


		pOutput->m_sOutput.clear();
		CXVERB("mytag", "test {1} {2} {3}", 123, 'x', false);
		REQUIRE(String("[VRB] [    mytag   ]  : test 123 x false\n") == pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXVERBIF(true, "mytag", "test {1} {2} {3}", 123, 'x', false);
		REQUIRE(String("[VRB] [    mytag   ]  : test 123 x false\n") == pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXVERBIF(false, "mytag", "test {1} {2} {3}", 123, 'x', false);
		REQUIRE(String("") == pOutput->m_sOutput);

		Log::Logger::GetDefaultLogger().SetLevel(Log::Level_Error);

		pOutput->m_sOutput.clear();
		CXINFO("mytag", "test {1} {2} {3}", 123, 'x', false);
		REQUIRE(String("") == pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXINFOIF(true, "mytag", "test {1} {2} {3}", 123, 'x', false);
		REQUIRE(String("") == pOutput->m_sOutput);
	}

	SECTION("4 args")
	{
		MyOutput *pOutput = New<MyOutput>();

		((Log::DefaultFormatter *)Log::Logger::GetDefaultLogger().GetFormatter())->
		     SetFlags(Log::DefaultFormatter::Show_Tag | Log::DefaultFormatter::Show_Level | 
		              Log::DefaultFormatter::Show_NewLine);
		Log::Logger::GetDefaultLogger().AddOutput(pOutput);
		Log::Logger::GetDefaultLogger().SetLevel(Log::Level_Debug);

		pOutput->m_sOutput.clear();
		CXERR("mytag", "test {1} {2} {3} {4:'0'.3}", 123, 'x', false, 123.456);
		REQUIRE(String("[ERR] [    mytag   ]  : test 123 x false 123.456\n") == pOutput->m_sOutput);
		
		pOutput->m_sOutput.clear();
		CXERRIF(true, "mytag", "test {1} {2} {3} {4:'0'.3}", 123, 'x', false, 123.456);
		REQUIRE(String("[ERR] [    mytag   ]  : test 123 x false 123.456\n") == pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXERRIF(false, "mytag", "test {1} {2} {3} {4:'0'.3}", 123, 'x', false, 123.456);
		REQUIRE(String("") == pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXWARN("mytag", "test {1} {2} {3} {4:'0'.3}", 123, 'x', false, 123.456);
		REQUIRE(String("[WRN] [    mytag   ]  : test 123 x false 123.456\n") == pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXWARNIF(true, "mytag", "test {1} {2} {3} {4:'0'.3}", 123, 'x', false, 123.456);
		REQUIRE(String("[WRN] [    mytag   ]  : test 123 x false 123.456\n") == pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXWARNIF(false, "mytag", "test {1} {2} {3} {4:'0'.3}", 123, 'x', false, 123.456);
		REQUIRE(String("") == pOutput->m_sOutput);


		pOutput->m_sOutput.clear();
		CXINFO("mytag", "test {1} {2} {3} {4:'0'.3}", 123, 'x', false, 123.456);
		REQUIRE(String("[INF] [    mytag   ]  : test 123 x false 123.456\n") == pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXINFOIF(true, "mytag", "test {1} {2} {3} {4:'0'.3}", 123, 'x', false, 123.456);
		REQUIRE(String("[INF] [    mytag   ]  : test 123 x false 123.456\n") == pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXINFOIF(false, "mytag", "test {1} {2} {3} {4:'0'.3}", 123, 'x', false, 123.456);
		REQUIRE(String("") == pOutput->m_sOutput);


		pOutput->m_sOutput.clear();
		CXDBG("mytag", "test {1} {2} {3} {4:'0'.3}", 123, 'x', false, 123.456);
		REQUIRE(String("[DBG] [    mytag   ]  : test 123 x false 123.456\n") == pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXDBGIF(true, "mytag", "test {1} {2} {3} {4:'0'.3}", 123, 'x', false, 123.456);
		REQUIRE(String("[DBG] [    mytag   ]  : test 123 x false 123.456\n") == pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXDBGIF(false, "mytag", "test {1} {2} {3} {4:'0'.3}", 123, 'x', false, 123.456);
		REQUIRE(String("") == pOutput->m_sOutput);


		pOutput->m_sOutput.clear();
		CXVERB("mytag", "test {1} {2} {3} {4:'0'.3}", 123, 'x', false, 123.456);
		REQUIRE(String("[VRB] [    mytag   ]  : test 123 x false 123.456\n") == pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXVERBIF(true, "mytag", "test {1} {2} {3} {4:'0'.3}", 123, 'x', false, 123.456);
		REQUIRE(String("[VRB] [    mytag   ]  : test 123 x false 123.456\n") == pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXVERBIF(false, "mytag", "test {1} {2} {3} {4:'0'.3}", 123, 'x', false, 123.456);
		REQUIRE(String("") == pOutput->m_sOutput);

		Log::Logger::GetDefaultLogger().SetLevel(Log::Level_Error);

		pOutput->m_sOutput.clear();
		CXINFO("mytag", "test {1} {2} {3} {4:'0'.3}", 123, 'x', false, 123.456);
		REQUIRE(String("") == pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXINFOIF(true, "mytag", "test {1} {2} {3} {4:'0'.3}", 123, 'x', false, 123.456);
		REQUIRE(String("") == pOutput->m_sOutput);
	}

	SECTION("5 args")
	{
		MyOutput *pOutput = New<MyOutput>();

		((Log::DefaultFormatter *)Log::Logger::GetDefaultLogger().GetFormatter())->
		     SetFlags(Log::DefaultFormatter::Show_Tag | Log::DefaultFormatter::Show_Level | 
		              Log::DefaultFormatter::Show_NewLine);
		Log::Logger::GetDefaultLogger().AddOutput(pOutput);
		Log::Logger::GetDefaultLogger().SetLevel(Log::Level_Debug);

		pOutput->m_sOutput.clear();
		CXERR("mytag", "test {1} {2} {3} {4:'0'.3} {5}", 123, 'x', false, 123.456, "abc");
		REQUIRE(String("[ERR] [    mytag   ]  : test 123 x false 123.456 abc\n") == pOutput->m_sOutput);
		
		pOutput->m_sOutput.clear();
		CXERRIF(true, "mytag", "test {1} {2} {3} {4:'0'.3} {5}", 123, 'x', false, 123.456, "abc");
		REQUIRE(String("[ERR] [    mytag   ]  : test 123 x false 123.456 abc\n") == pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXERRIF(false, "mytag", "test {1} {2} {3} {4:'0'.3} {5}", 123, 'x', false, 123.456, "abc");
		REQUIRE(String("") == pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXWARN("mytag", "test {1} {2} {3} {4:'0'.3} {5}", 123, 'x', false, 123.456, "abc");
		REQUIRE(String("[WRN] [    mytag   ]  : test 123 x false 123.456 abc\n") == pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXWARNIF(true, "mytag", "test {1} {2} {3} {4:'0'.3} {5}", 123, 'x', false, 123.456, "abc");
		REQUIRE(String("[WRN] [    mytag   ]  : test 123 x false 123.456 abc\n") == pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXWARNIF(false, "mytag", "test {1} {2} {3} {4:'0'.3} {5}", 123, 'x', false, 123.456, "abc");
		REQUIRE(String("") == pOutput->m_sOutput);


		pOutput->m_sOutput.clear();
		CXINFO("mytag", "test {1} {2} {3} {4:'0'.3} {5}", 123, 'x', false, 123.456, "abc");
		REQUIRE(String("[INF] [    mytag   ]  : test 123 x false 123.456 abc\n") == pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXINFOIF(true, "mytag", "test {1} {2} {3} {4:'0'.3} {5}", 123, 'x', false, 123.456, "abc");
		REQUIRE(String("[INF] [    mytag   ]  : test 123 x false 123.456 abc\n") == pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXINFOIF(false, "mytag", "test {1} {2} {3} {4:'0'.3} {5}", 123, 'x', false, 123.456, "abc");
		REQUIRE(String("") == pOutput->m_sOutput);


		pOutput->m_sOutput.clear();
		CXDBG("mytag", "test {1} {2} {3} {4:'0'.3} {5}", 123, 'x', false, 123.456, "abc");
		REQUIRE(String("[DBG] [    mytag   ]  : test 123 x false 123.456 abc\n") == pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXDBGIF(true, "mytag", "test {1} {2} {3} {4:'0'.3} {5}", 123, 'x', false, 123.456, "abc");
		REQUIRE(String("[DBG] [    mytag   ]  : test 123 x false 123.456 abc\n") == pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXDBGIF(false, "mytag", "test {1} {2} {3} {4:'0'.3} {5}", 123, 'x', false, 123.456, "abc");
		REQUIRE(String("") == pOutput->m_sOutput);


		pOutput->m_sOutput.clear();
		CXVERB("mytag", "test {1} {2} {3} {4:'0'.3} {5}", 123, 'x', false, 123.456, "abc");
		REQUIRE(String("[VRB] [    mytag   ]  : test 123 x false 123.456 abc\n") == pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXVERBIF(true, "mytag", "test {1} {2} {3} {4:'0'.3} {5}", 123, 'x', false, 123.456, "abc");
		REQUIRE(String("[VRB] [    mytag   ]  : test 123 x false 123.456 abc\n") == pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXVERBIF(false, "mytag", "test {1} {2} {3} {4:'0'.3} {5}", 123, 'x', false, 123.456, "abc");
		REQUIRE(String("") == pOutput->m_sOutput);

		Log::Logger::GetDefaultLogger().SetLevel(Log::Level_Error);

		pOutput->m_sOutput.clear();
		CXINFO("mytag", "test {1} {2} {3} {4:'0'.3} {5}", 123, 'x', false, 123.456, "abc");
		REQUIRE(String("") == pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXINFOIF(true, "mytag", "test {1} {2} {3} {4:'0'.3} {5}", 123, 'x', false, 123.456, "abc");
		REQUIRE(String("") == pOutput->m_sOutput);
	}

	SECTION("6 args")
	{
		MyOutput *pOutput = New<MyOutput>();

		((Log::DefaultFormatter *)Log::Logger::GetDefaultLogger().GetFormatter())->
		     SetFlags(Log::DefaultFormatter::Show_Tag | Log::DefaultFormatter::Show_Level | 
		              Log::DefaultFormatter::Show_NewLine);
		Log::Logger::GetDefaultLogger().AddOutput(pOutput);
		Log::Logger::GetDefaultLogger().SetLevel(Log::Level_Debug);

		pOutput->m_sOutput.clear();
		CXERR("mytag", "test {1} {2} {3} {4:'0'.3} {5} {6}", 123, 'x', false, 123.456, "abc", 
		      (Size)100);
		REQUIRE(String("[ERR] [    mytag   ]  : test 123 x false 123.456 abc 100\n") == 
		        pOutput->m_sOutput);
		
		pOutput->m_sOutput.clear();
		CXERRIF(true, "mytag", "test {1} {2} {3} {4:'0'.3} {5} {6}", 123, 'x', false, 123.456, "abc", 
		        (Size)100);
		REQUIRE(String("[ERR] [    mytag   ]  : test 123 x false 123.456 abc 100\n") == 
		        pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXERRIF(false, "mytag", "test {1} {2} {3} {4:'0'.3} {5} {6}", 123, 'x', false, 123.456, 
		        "abc", (Size)100);
		REQUIRE(String("") == pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXWARN("mytag", "test {1} {2} {3} {4:'0'.3} {5} {6}", 123, 'x', false, 123.456, "abc", 
		       (Size)100);
		REQUIRE(String("[WRN] [    mytag   ]  : test 123 x false 123.456 abc 100\n") == 
		        pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXWARNIF(true, "mytag", "test {1} {2} {3} {4:'0'.3} {5} {6}", 123, 'x', false, 123.456, 
		         "abc", (Size)100);
		REQUIRE(String("[WRN] [    mytag   ]  : test 123 x false 123.456 abc 100\n") == 
		        pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXWARNIF(false, "mytag", "test {1} {2} {3} {4:'0'.3} {5} {6}", 123, 'x', false, 123.456, 
		         "abc", (Size)100);
		REQUIRE(String("") == pOutput->m_sOutput);


		pOutput->m_sOutput.clear();
		CXINFO("mytag", "test {1} {2} {3} {4:'0'.3} {5} {6}", 123, 'x', false, 123.456, "abc", 
		       (Size)100);
		REQUIRE(String("[INF] [    mytag   ]  : test 123 x false 123.456 abc 100\n") == 
		        pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXINFOIF(true, "mytag", "test {1} {2} {3} {4:'0'.3} {5} {6}", 123, 'x', false, 123.456, 
		         "abc", (Size)100);
		REQUIRE(String("[INF] [    mytag   ]  : test 123 x false 123.456 abc 100\n") == 
		        pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXINFOIF(false, "mytag", "test {1} {2} {3} {4:'0'.3} {5} {6}", 123, 'x', false, 123.456, 
		         "abc", (Size)100);
		REQUIRE(String("") == pOutput->m_sOutput);


		pOutput->m_sOutput.clear();
		CXDBG("mytag", "test {1} {2} {3} {4:'0'.3} {5} {6}", 123, 'x', false, 123.456, "abc", 
		      (Size)100);
		REQUIRE(String("[DBG] [    mytag   ]  : test 123 x false 123.456 abc 100\n") == 
		        pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXDBGIF(true, "mytag", "test {1} {2} {3} {4:'0'.3} {5} {6}", 123, 'x', false, 123.456, 
		        "abc", (Size)100);
		REQUIRE(String("[DBG] [    mytag   ]  : test 123 x false 123.456 abc 100\n") == 
		        pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXDBGIF(false, "mytag", "test {1} {2} {3} {4:'0'.3} {5} {6}", 123, 'x', false, 123.456, 
		        "abc", (Size)100);
		REQUIRE(String("") == pOutput->m_sOutput);


		pOutput->m_sOutput.clear();
		CXVERB("mytag", "test {1} {2} {3} {4:'0'.3} {5} {6}", 123, 'x', false, 123.456, "abc", 
		       (Size)100);
		REQUIRE(String("[VRB] [    mytag   ]  : test 123 x false 123.456 abc 100\n") == 
		        pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXVERBIF(true, "mytag", "test {1} {2} {3} {4:'0'.3} {5} {6}", 123, 'x', false, 123.456, 
		         "abc", (Size)100);
		REQUIRE(String("[VRB] [    mytag   ]  : test 123 x false 123.456 abc 100\n") == 
		         pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXVERBIF(false, "mytag", "test {1} {2} {3} {4:'0'.3} {5} {6}", 123, 'x', false, 123.456, 
		         "abc", (Size)100);
		REQUIRE(String("") == pOutput->m_sOutput);

		Log::Logger::GetDefaultLogger().SetLevel(Log::Level_Error);

		pOutput->m_sOutput.clear();
		CXINFO("mytag", "test {1} {2} {3} {4:'0'.3} {5} {6}", 123, 'x', false, 123.456, "abc", 
		       (Size)100);
		REQUIRE(String("") == pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXINFOIF(true, "mytag", "test {1} {2} {3} {4:'0'.3} {5} {6}", 123, 'x', false, 123.456, 
		         "abc", (Size)100);
		REQUIRE(String("") == pOutput->m_sOutput);
	}

	SECTION("7 args")
	{
		MyOutput *pOutput = New<MyOutput>();

		((Log::DefaultFormatter *)Log::Logger::GetDefaultLogger().GetFormatter())->
		     SetFlags(Log::DefaultFormatter::Show_Tag | Log::DefaultFormatter::Show_Level | 
		              Log::DefaultFormatter::Show_NewLine);
		Log::Logger::GetDefaultLogger().AddOutput(pOutput);
		Log::Logger::GetDefaultLogger().SetLevel(Log::Level_Debug);

		pOutput->m_sOutput.clear();
		CXERR("mytag", "test {1} {2} {3} {4:'0'.3} {5} {6} {7}", 123, 'x', false, 123.456, "abc", 
		      (Size)100, -100);
		REQUIRE(String("[ERR] [    mytag   ]  : test 123 x false 123.456 abc 100 -100\n") == 
		        pOutput->m_sOutput);
		
		pOutput->m_sOutput.clear();
		CXERRIF(true, "mytag", "test {1} {2} {3} {4:'0'.3} {5} {6} {7}", 123, 'x', false, 123.456, 
		        "abc", (Size)100, -100);
		REQUIRE(String("[ERR] [    mytag   ]  : test 123 x false 123.456 abc 100 -100\n") == 
		        pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXERRIF(false, "mytag", "test {1} {2} {3} {4:'0'.3} {5} {6} {7}", 123, 'x', false, 123.456, 
		        "abc", (Size)100, -100);
		REQUIRE(String("") == pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXWARN("mytag", "test {1} {2} {3} {4:'0'.3} {5} {6} {7}", 123, 'x', false, 123.456, "abc", 
		       (Size)100, -100);
		REQUIRE(String("[WRN] [    mytag   ]  : test 123 x false 123.456 abc 100 -100\n") == 
		        pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXWARNIF(true, "mytag", "test {1} {2} {3} {4:'0'.3} {5} {6} {7}", 123, 'x', false, 123.456, 
		         "abc", (Size)100, -100);
		REQUIRE(String("[WRN] [    mytag   ]  : test 123 x false 123.456 abc 100 -100\n") == 
		        pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXWARNIF(false, "mytag", "test {1} {2} {3} {4:'0'.3} {5} {6} {7}", 123, 'x', false, 123.456, 
		         "abc", (Size)100, -100);
		REQUIRE(String("") == pOutput->m_sOutput);


		pOutput->m_sOutput.clear();
		CXINFO("mytag", "test {1} {2} {3} {4:'0'.3} {5} {6} {7}", 123, 'x', false, 123.456, "abc", 
		       (Size)100, -100);
		REQUIRE(String("[INF] [    mytag   ]  : test 123 x false 123.456 abc 100 -100\n") == 
		        pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXINFOIF(true, "mytag", "test {1} {2} {3} {4:'0'.3} {5} {6} {7}", 123, 'x', false, 123.456, 
		         "abc", (Size)100, -100);
		REQUIRE(String("[INF] [    mytag   ]  : test 123 x false 123.456 abc 100 -100\n") == 
		        pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXINFOIF(false, "mytag", "test {1} {2} {3} {4:'0'.3} {5} {6} {7}", 123, 'x', false, 123.456, 
		         "abc", (Size)100, -100);
		REQUIRE(String("") == pOutput->m_sOutput);


		pOutput->m_sOutput.clear();
		CXDBG("mytag", "test {1} {2} {3} {4:'0'.3} {5} {6} {7}", 123, 'x', false, 123.456, "abc", 
		      (Size)100, -100);
		REQUIRE(String("[DBG] [    mytag   ]  : test 123 x false 123.456 abc 100 -100\n") == 
		        pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXDBGIF(true, "mytag", "test {1} {2} {3} {4:'0'.3} {5} {6} {7}", 123, 'x', false, 123.456, 
		        "abc", (Size)100, -100);
		REQUIRE(String("[DBG] [    mytag   ]  : test 123 x false 123.456 abc 100 -100\n") == 
		        pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXDBGIF(false, "mytag", "test {1} {2} {3} {4:'0'.3} {5} {6} {7}", 123, 'x', false, 123.456, 
		        "abc", (Size)100, -100);
		REQUIRE(String("") == pOutput->m_sOutput);


		pOutput->m_sOutput.clear();
		CXVERB("mytag", "test {1} {2} {3} {4:'0'.3} {5} {6} {7}", 123, 'x', false, 123.456, "abc", 
		       (Size)100, -100);
		REQUIRE(String("[VRB] [    mytag   ]  : test 123 x false 123.456 abc 100 -100\n") == 
		        pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXVERBIF(true, "mytag", "test {1} {2} {3} {4:'0'.3} {5} {6} {7}", 123, 'x', false, 123.456, 
		         "abc", (Size)100, -100);
		REQUIRE(String("[VRB] [    mytag   ]  : test 123 x false 123.456 abc 100 -100\n") == 
		         pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXVERBIF(false, "mytag", "test {1} {2} {3} {4:'0'.3} {5} {6} {7}", 123, 'x', false, 123.456, 
		         "abc", (Size)100, -100);
		REQUIRE(String("") == pOutput->m_sOutput);

		Log::Logger::GetDefaultLogger().SetLevel(Log::Level_Error);

		pOutput->m_sOutput.clear();
		CXINFO("mytag", "test {1} {2} {3} {4:'0'.3} {5} {6} {7}", 123, 'x', false, 123.456, "abc", 
		       (Size)100, -100);
		REQUIRE(String("") == pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXINFOIF(true, "mytag", "test {1} {2} {3} {4:'0'.3} {5} {6} {7}", 123, 'x', false, 123.456, 
		         "abc", (Size)100, -100);
		REQUIRE(String("") == pOutput->m_sOutput);
	}

	SECTION("8 args")
	{
		MyOutput *pOutput = New<MyOutput>();

		((Log::DefaultFormatter *)Log::Logger::GetDefaultLogger().GetFormatter())->
		     SetFlags(Log::DefaultFormatter::Show_Tag | Log::DefaultFormatter::Show_Level | 
		              Log::DefaultFormatter::Show_NewLine);
		Log::Logger::GetDefaultLogger().AddOutput(pOutput);
		Log::Logger::GetDefaultLogger().SetLevel(Log::Level_Debug);

		pOutput->m_sOutput.clear();
		CXERR("mytag", "test {1} {2} {3} {4:'0'.3} {5} {6} {7} {8}", 123, 'x', false, 123.456, "abc", 
		      (Size)100, -100, "xyz");
		REQUIRE(String("[ERR] [    mytag   ]  : test 123 x false 123.456 abc 100 -100 xyz\n") == 
		        pOutput->m_sOutput);
		
		pOutput->m_sOutput.clear();
		CXERRIF(true, "mytag", "test {1} {2} {3} {4:'0'.3} {5} {6} {7} {8}", 123, 'x', false, 
		        123.456, "abc", (Size)100, -100, "xyz");
		REQUIRE(String("[ERR] [    mytag   ]  : test 123 x false 123.456 abc 100 -100 xyz\n") == 
		        pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXERRIF(false, "mytag", "test {1} {2} {3} {4:'0'.3} {5} {6} {7} {8}", 123, 'x', false, 
		        123.456, "abc", (Size)100, -100, "xyz");
		REQUIRE(String("") == pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXWARN("mytag", "test {1} {2} {3} {4:'0'.3} {5} {6} {7} {8}", 123, 'x', false, 123.456, 
		       "abc", (Size)100, -100, "xyz");
		REQUIRE(String("[WRN] [    mytag   ]  : test 123 x false 123.456 abc 100 -100 xyz\n") == 
		        pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXWARNIF(true, "mytag", "test {1} {2} {3} {4:'0'.3} {5} {6} {7} {8}", 123, 'x', false, 
		         123.456, "abc", (Size)100, -100, "xyz");
		REQUIRE(String("[WRN] [    mytag   ]  : test 123 x false 123.456 abc 100 -100 xyz\n") == 
		        pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXWARNIF(false, "mytag", "test {1} {2} {3} {4:'0'.3} {5} {6} {7} {8}", 123, 'x', false, 
		         123.456, "abc", (Size)100, -100, "xyz");
		REQUIRE(String("") == pOutput->m_sOutput);


		pOutput->m_sOutput.clear();
		CXINFO("mytag", "test {1} {2} {3} {4:'0'.3} {5} {6} {7} {8}", 123, 'x', false, 123.456, 
		       "abc", (Size)100, -100, "xyz");
		REQUIRE(String("[INF] [    mytag   ]  : test 123 x false 123.456 abc 100 -100 xyz\n") == 
		        pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXINFOIF(true, "mytag", "test {1} {2} {3} {4:'0'.3} {5} {6} {7} {8}", 123, 'x', false, 
		         123.456, "abc", (Size)100, -100, "xyz");
		REQUIRE(String("[INF] [    mytag   ]  : test 123 x false 123.456 abc 100 -100 xyz\n") == 
		        pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXINFOIF(false, "mytag", "test {1} {2} {3} {4:'0'.3} {5} {6} {7} {8}", 123, 'x', false, 
		         123.456, "abc", (Size)100, -100, "xyz");
		REQUIRE(String("") == pOutput->m_sOutput);


		pOutput->m_sOutput.clear();
		CXDBG("mytag", "test {1} {2} {3} {4:'0'.3} {5} {6} {7} {8}", 123, 'x', false, 123.456, "abc", 
		      (Size)100, -100, "xyz");
		REQUIRE(String("[DBG] [    mytag   ]  : test 123 x false 123.456 abc 100 -100 xyz\n") == 
		        pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXDBGIF(true, "mytag", "test {1} {2} {3} {4:'0'.3} {5} {6} {7} {8}", 123, 'x', false, 
		        123.456, "abc", (Size)100, -100, "xyz");
		REQUIRE(String("[DBG] [    mytag   ]  : test 123 x false 123.456 abc 100 -100 xyz\n") == 
		        pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXDBGIF(false, "mytag", "test {1} {2} {3} {4:'0'.3} {5} {6} {7} {8}", 123, 'x', false, 
		        123.456, "abc", (Size)100, -100, "xyz");
		REQUIRE(String("") == pOutput->m_sOutput);


		pOutput->m_sOutput.clear();
		CXVERB("mytag", "test {1} {2} {3} {4:'0'.3} {5} {6} {7} {8}", 123, 'x', false, 123.456, 
		       "abc", (Size)100, -100, "xyz");
		REQUIRE(String("[VRB] [    mytag   ]  : test 123 x false 123.456 abc 100 -100 xyz\n") == 
		        pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXVERBIF(true, "mytag", "test {1} {2} {3} {4:'0'.3} {5} {6} {7} {8}", 123, 'x', false, 
		         123.456, "abc", (Size)100, -100, "xyz");
		REQUIRE(String("[VRB] [    mytag   ]  : test 123 x false 123.456 abc 100 -100 xyz\n") == 
		         pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXVERBIF(false, "mytag", "test {1} {2} {3} {4:'0'.3} {5} {6} {7} {8}", 123, 'x', false, 
		         123.456, "abc", (Size)100, -100, "xyz");
		REQUIRE(String("") == pOutput->m_sOutput);

		Log::Logger::GetDefaultLogger().SetLevel(Log::Level_Error);

		pOutput->m_sOutput.clear();
		CXINFO("mytag", "test {1} {2} {3} {4:'0'.3} {5} {6} {7} {8}", 123, 'x', false, 123.456, 
		       "abc", (Size)100, -100, "xyz");
		REQUIRE(String("") == pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXINFOIF(true, "mytag", "test {1} {2} {3} {4:'0'.3} {5} {6} {7} {8}", 123, 'x', false, 
		         123.456, "abc", (Size)100, -100, "xyz");
		REQUIRE(String("") == pOutput->m_sOutput);
	}

	SECTION("9 args")
	{
		MyOutput *pOutput = New<MyOutput>();

		((Log::DefaultFormatter *)Log::Logger::GetDefaultLogger().GetFormatter())->
		     SetFlags(Log::DefaultFormatter::Show_Tag | Log::DefaultFormatter::Show_Level | 
		              Log::DefaultFormatter::Show_NewLine);
		Log::Logger::GetDefaultLogger().AddOutput(pOutput);
		Log::Logger::GetDefaultLogger().SetLevel(Log::Level_Debug);

		pOutput->m_sOutput.clear();
		CXERR("mytag", "test {1} {2} {3} {4:'0'.3} {5} {6} {7} {8} {9:'0'.2}", 123, 'x', false, 
		      123.456, "abc", (Size)100, -100, "xyz", 12.34f);
		REQUIRE(String("[ERR] [    mytag   ]  : test 123 x false 123.456 abc 100 -100 xyz 12.34\n") 
		        == pOutput->m_sOutput);
		
		pOutput->m_sOutput.clear();
		CXERRIF(true, "mytag", "test {1} {2} {3} {4:'0'.3} {5} {6} {7} {8} {9:'0'.2}", 123, 'x', 
		        false, 123.456, "abc", (Size)100, -100, "xyz", 12.34f);
		REQUIRE(String("[ERR] [    mytag   ]  : test 123 x false 123.456 abc 100 -100 xyz 12.34\n") 
		        == pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXERRIF(false, "mytag", "test {1} {2} {3} {4:'0'.3} {5} {6} {7} {8} {9:'0'.2}", 123, 'x', 
		        false, 123.456, "abc", (Size)100, -100, "xyz", 12.34f);
		REQUIRE(String("") == pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXWARN("mytag", "test {1} {2} {3} {4:'0'.3} {5} {6} {7} {8} {9:'0'.2}", 123, 'x', false, 
		       123.456, "abc", (Size)100, -100, "xyz", 12.34f);
		REQUIRE(String("[WRN] [    mytag   ]  : test 123 x false 123.456 abc 100 -100 xyz 12.34\n") 
		        == pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXWARNIF(true, "mytag", "test {1} {2} {3} {4:'0'.3} {5} {6} {7} {8} {9:'0'.2}", 123, 'x', 
		         false, 123.456, "abc", (Size)100, -100, "xyz", 12.34f);
		REQUIRE(String("[WRN] [    mytag   ]  : test 123 x false 123.456 abc 100 -100 xyz 12.34\n") 
		        == pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXWARNIF(false, "mytag", "test {1} {2} {3} {4:'0'.3} {5} {6} {7} {8} {9:'0'.2}", 123, 'x', 
		         false, 123.456, "abc", (Size)100, -100, "xyz", 12.34f);
		REQUIRE(String("") == pOutput->m_sOutput);


		pOutput->m_sOutput.clear();
		CXINFO("mytag", "test {1} {2} {3} {4:'0'.3} {5} {6} {7} {8} {9:'0'.2}", 123, 'x', false, 
		       123.456, "abc", (Size)100, -100, "xyz", 12.34f);
		REQUIRE(String("[INF] [    mytag   ]  : test 123 x false 123.456 abc 100 -100 xyz 12.34\n") 
		        == pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXINFOIF(true, "mytag", "test {1} {2} {3} {4:'0'.3} {5} {6} {7} {8} {9:'0'.2}", 123, 'x', 
		         false, 123.456, "abc", (Size)100, -100, "xyz", 12.34f);
		REQUIRE(String("[INF] [    mytag   ]  : test 123 x false 123.456 abc 100 -100 xyz 12.34\n") 
		        == pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXINFOIF(false, "mytag", "test {1} {2} {3} {4:'0'.3} {5} {6} {7} {8} {9:'0'.2}", 123, 'x', 
		         false, 123.456, "abc", (Size)100, -100, "xyz", 12.34f);
		REQUIRE(String("") == pOutput->m_sOutput);


		pOutput->m_sOutput.clear();
		CXDBG("mytag", "test {1} {2} {3} {4:'0'.3} {5} {6} {7} {8} {9:'0'.2}", 123, 'x', false, 
		      123.456, "abc", (Size)100, -100, "xyz", 12.34f);
		REQUIRE(String("[DBG] [    mytag   ]  : test 123 x false 123.456 abc 100 -100 xyz 12.34\n") 
		        == pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXDBGIF(true, "mytag", "test {1} {2} {3} {4:'0'.3} {5} {6} {7} {8} {9:'0'.2}", 123, 'x', 
		        false, 123.456, "abc", (Size)100, -100, "xyz", 12.34f);
		REQUIRE(String("[DBG] [    mytag   ]  : test 123 x false 123.456 abc 100 -100 xyz 12.34\n") 
		        == pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXDBGIF(false, "mytag", "test {1} {2} {3} {4:'0'.3} {5} {6} {7} {8} {9:'0'.2}", 123, 'x', 
		        false, 123.456, "abc", (Size)100, -100, "xyz", 12.34f);
		REQUIRE(String("") == pOutput->m_sOutput);


		pOutput->m_sOutput.clear();
		CXVERB("mytag", "test {1} {2} {3} {4:'0'.3} {5} {6} {7} {8} {9:'0'.2}", 123, 'x', false, 
		       123.456, "abc", (Size)100, -100, "xyz", 12.34f);
		REQUIRE(String("[VRB] [    mytag   ]  : test 123 x false 123.456 abc 100 -100 xyz 12.34\n") 
		        == pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXVERBIF(true, "mytag", "test {1} {2} {3} {4:'0'.3} {5} {6} {7} {8} {9:'0'.2}", 123, 'x', 
		         false, 123.456, "abc", (Size)100, -100, "xyz", 12.34f);
		REQUIRE(String("[VRB] [    mytag   ]  : test 123 x false 123.456 abc 100 -100 xyz 12.34\n") 
		        == pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXVERBIF(false, "mytag", "test {1} {2} {3} {4:'0'.3} {5} {6} {7} {8} {9:'0'.2}", 123, 'x', 
		         false, 123.456, "abc", (Size)100, -100, "xyz", 12.34f);
		REQUIRE(String("") == pOutput->m_sOutput);

		Log::Logger::GetDefaultLogger().SetLevel(Log::Level_Error);

		pOutput->m_sOutput.clear();
		CXINFO("mytag", "test {1} {2} {3} {4:'0'.3} {5} {6} {7} {8} {9:'0'.2}", 123, 'x', false, 
		       123.456, "abc", (Size)100, -100, "xyz", 12.34f);
		REQUIRE(String("") == pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXINFOIF(true, "mytag", "test {1} {2} {3} {4:'0'.3} {5} {6} {7} {8} {9:'0'.2}", 123, 'x', 
		         false, 123.456, "abc", (Size)100, -100, "xyz", 12.34f);
		REQUIRE(String("") == pOutput->m_sOutput);
	}

	SECTION("10 args")
	{
		MyOutput *pOutput = New<MyOutput>();

		((Log::DefaultFormatter *)Log::Logger::GetDefaultLogger().GetFormatter())->
		     SetFlags(Log::DefaultFormatter::Show_Tag | Log::DefaultFormatter::Show_Level | 
		              Log::DefaultFormatter::Show_NewLine);
		Log::Logger::GetDefaultLogger().AddOutput(pOutput);
		Log::Logger::GetDefaultLogger().SetLevel(Log::Level_Debug);

		pOutput->m_sOutput.clear();
		CXERR("mytag", "test {1} {2} {3} {4:'0'.3} {5} {6} {7} {8} {9:'0'.2} {10}", 123, 'x', false, 
		      123.456, "abc", (Size)100, -100, "xyz", 12.34f, 1);
		REQUIRE(String("[ERR] [    mytag   ]  : test 123 x false 123.456 abc 100 -100 xyz 12.34 1\n") 
		        == pOutput->m_sOutput);
		
		pOutput->m_sOutput.clear();
		CXERRIF(true, "mytag", "test {1} {2} {3} {4:'0'.3} {5} {6} {7} {8} {9:'0'.2} {10}", 123, 'x', 
		        false, 123.456, "abc", (Size)100, -100, "xyz", 12.34f, 1);
		REQUIRE(String("[ERR] [    mytag   ]  : test 123 x false 123.456 abc 100 -100 xyz 12.34 1\n") 
		        == pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXERRIF(false, "mytag", "test {1} {2} {3} {4:'0'.3} {5} {6} {7} {8} {9:'0'.2} {10}", 123, 
		        'x', false, 123.456, "abc", (Size)100, -100, "xyz", 12.34f, 1);
		REQUIRE(String("") == pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXWARN("mytag", "test {1} {2} {3} {4:'0'.3} {5} {6} {7} {8} {9:'0'.2} {10}", 123, 'x', false, 
		       123.456, "abc", (Size)100, -100, "xyz", 12.34f, 1);
		REQUIRE(String("[WRN] [    mytag   ]  : test 123 x false 123.456 abc 100 -100 xyz 12.34 1\n") 
		        == pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXWARNIF(true, "mytag", "test {1} {2} {3} {4:'0'.3} {5} {6} {7} {8} {9:'0'.2} {10}", 123, 
		         'x', false, 123.456, "abc", (Size)100, -100, "xyz", 12.34f, 1);
		REQUIRE(String("[WRN] [    mytag   ]  : test 123 x false 123.456 abc 100 -100 xyz 12.34 1\n") 
		        == pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXWARNIF(false, "mytag", "test {1} {2} {3} {4:'0'.3} {5} {6} {7} {8} {9:'0'.2} {10}", 123, 
		         'x', false, 123.456, "abc", (Size)100, -100, "xyz", 12.34f, 1);
		REQUIRE(String("") == pOutput->m_sOutput);


		pOutput->m_sOutput.clear();
		CXINFO("mytag", "test {1} {2} {3} {4:'0'.3} {5} {6} {7} {8} {9:'0'.2} {10}", 123, 'x', false, 
		       123.456, "abc", (Size)100, -100, "xyz", 12.34f, 1);
		REQUIRE(String("[INF] [    mytag   ]  : test 123 x false 123.456 abc 100 -100 xyz 12.34 1\n") 
		        == pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXINFOIF(true, "mytag", "test {1} {2} {3} {4:'0'.3} {5} {6} {7} {8} {9:'0'.2} {10}", 123, 
		         'x', false, 123.456, "abc", (Size)100, -100, "xyz", 12.34f, 1);
		REQUIRE(String("[INF] [    mytag   ]  : test 123 x false 123.456 abc 100 -100 xyz 12.34 1\n") 
		        == pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXINFOIF(false, "mytag", "test {1} {2} {3} {4:'0'.3} {5} {6} {7} {8} {9:'0'.2} {10}", 123, 
		         'x', false, 123.456, "abc", (Size)100, -100, "xyz", 12.34f, 1);
		REQUIRE(String("") == pOutput->m_sOutput);


		pOutput->m_sOutput.clear();
		CXDBG("mytag", "test {1} {2} {3} {4:'0'.3} {5} {6} {7} {8} {9:'0'.2} {10}", 123, 'x', false, 
		      123.456, "abc", (Size)100, -100, "xyz", 12.34f, 1);
		REQUIRE(String("[DBG] [    mytag   ]  : test 123 x false 123.456 abc 100 -100 xyz 12.34 1\n") 
		        == pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXDBGIF(true, "mytag", "test {1} {2} {3} {4:'0'.3} {5} {6} {7} {8} {9:'0'.2} {10}", 123, 'x', 
		        false, 123.456, "abc", (Size)100, -100, "xyz", 12.34f, 1);
		REQUIRE(String("[DBG] [    mytag   ]  : test 123 x false 123.456 abc 100 -100 xyz 12.34 1\n") 
		        == pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXDBGIF(false, "mytag", "test {1} {2} {3} {4:'0'.3} {5} {6} {7} {8} {9:'0'.2} {10}", 123, 
		        'x', false, 123.456, "abc", (Size)100, -100, "xyz", 12.34f, 1);
		REQUIRE(String("") == pOutput->m_sOutput);


		pOutput->m_sOutput.clear();
		CXVERB("mytag", "test {1} {2} {3} {4:'0'.3} {5} {6} {7} {8} {9:'0'.2} {10}", 123, 'x', false, 
		       123.456, "abc", (Size)100, -100, "xyz", 12.34f, 1);
		REQUIRE(String("[VRB] [    mytag   ]  : test 123 x false 123.456 abc 100 -100 xyz 12.34 1\n") 
		        == pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXVERBIF(true, "mytag", "test {1} {2} {3} {4:'0'.3} {5} {6} {7} {8} {9:'0'.2} {10}", 123, 
		         'x', false, 123.456, "abc", (Size)100, -100, "xyz", 12.34f, 1);
		REQUIRE(String("[VRB] [    mytag   ]  : test 123 x false 123.456 abc 100 -100 xyz 12.34 1\n") 
		        == pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXVERBIF(false, "mytag", "test {1} {2} {3} {4:'0'.3} {5} {6} {7} {8} {9:'0'.2} {10}", 123, 
		         'x', false, 123.456, "abc", (Size)100, -100, "xyz", 12.34f, 1);
		REQUIRE(String("") == pOutput->m_sOutput);

		Log::Logger::GetDefaultLogger().SetLevel(Log::Level_Error);

		pOutput->m_sOutput.clear();
		CXINFO("mytag", "test {1} {2} {3} {4:'0'.3} {5} {6} {7} {8} {9:'0'.2} {10}", 123, 'x', false, 
		       123.456, "abc", (Size)100, -100, "xyz", 12.34f, 1);
		REQUIRE(String("") == pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXINFOIF(true, "mytag", "test {1} {2} {3} {4:'0'.3} {5} {6} {7} {8} {9:'0'.2} {10}", 123, 
		         'x', false, 123.456, "abc", (Size)100, -100, "xyz", 12.34f, 1);
		REQUIRE(String("") == pOutput->m_sOutput);
	}

	SECTION("11 args")
	{
		MyOutput *pOutput = New<MyOutput>();

		((Log::DefaultFormatter *)Log::Logger::GetDefaultLogger().GetFormatter())->
		     SetFlags(Log::DefaultFormatter::Show_Tag | Log::DefaultFormatter::Show_Level | 
		              Log::DefaultFormatter::Show_NewLine);
		Log::Logger::GetDefaultLogger().AddOutput(pOutput);
		Log::Logger::GetDefaultLogger().SetLevel(Log::Level_Debug);

		pOutput->m_sOutput.clear();
		CXERR("mytag", "test {1} {2} {3} {4:'0'.3} {5} {6} {7} {8} {9:'0'.2} {10} {11}", 123, 'x', 
		      false, 123.456, "abc", (Size)100, -100, "xyz", 12.34f, 1, 2);
		REQUIRE(String("[ERR] [    mytag   ]  : test 123 x false 123.456 abc 100 -100 xyz 12.34 1 "
		        "2\n") == pOutput->m_sOutput);
		
		pOutput->m_sOutput.clear();
		CXERRIF(true, "mytag", "test {1} {2} {3} {4:'0'.3} {5} {6} {7} {8} {9:'0'.2} {10} {11}", 123, 
		        'x', false, 123.456, "abc", (Size)100, -100, "xyz", 12.34f, 1, 2);
		REQUIRE(String("[ERR] [    mytag   ]  : test 123 x false 123.456 abc 100 -100 xyz 12.34 1 "
		        "2\n") == pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXERRIF(false, "mytag", "test {1} {2} {3} {4:'0'.3} {5} {6} {7} {8} {9:'0'.2} {10} {11}", 
		        123, 'x', false, 123.456, "abc", (Size)100, -100, "xyz", 12.34f, 1, 2);
		REQUIRE(String("") == pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXWARN("mytag", "test {1} {2} {3} {4:'0'.3} {5} {6} {7} {8} {9:'0'.2} {10} {11}", 123, 'x', 
		       false, 123.456, "abc", (Size)100, -100, "xyz", 12.34f, 1, 2);
		REQUIRE(String("[WRN] [    mytag   ]  : test 123 x false 123.456 abc 100 -100 xyz 12.34 1 "
		        "2\n") == pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXWARNIF(true, "mytag", "test {1} {2} {3} {4:'0'.3} {5} {6} {7} {8} {9:'0'.2} {10} {11}", 
		         123, 'x', false, 123.456, "abc", (Size)100, -100, "xyz", 12.34f, 1, 2);
		REQUIRE(String("[WRN] [    mytag   ]  : test 123 x false 123.456 abc 100 -100 xyz 12.34 1 "
		        "2\n") == pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXWARNIF(false, "mytag", "test {1} {2} {3} {4:'0'.3} {5} {6} {7} {8} {9:'0'.2} {10} {11}", 
		         123, 'x', false, 123.456, "abc", (Size)100, -100, "xyz", 12.34f, 1, 2);
		REQUIRE(String("") == pOutput->m_sOutput);


		pOutput->m_sOutput.clear();
		CXINFO("mytag", "test {1} {2} {3} {4:'0'.3} {5} {6} {7} {8} {9:'0'.2} {10} {11}", 123, 'x', 
		       false, 123.456, "abc", (Size)100, -100, "xyz", 12.34f, 1, 2);
		REQUIRE(String("[INF] [    mytag   ]  : test 123 x false 123.456 abc 100 -100 xyz 12.34 1 "
		        "2\n") == pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXINFOIF(true, "mytag", "test {1} {2} {3} {4:'0'.3} {5} {6} {7} {8} {9:'0'.2} {10} {11}", 
		         123, 'x', false, 123.456, "abc", (Size)100, -100, "xyz", 12.34f, 1, 2);
		REQUIRE(String("[INF] [    mytag   ]  : test 123 x false 123.456 abc 100 -100 xyz 12.34 1 "
		        "2\n") == pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXINFOIF(false, "mytag", "test {1} {2} {3} {4:'0'.3} {5} {6} {7} {8} {9:'0'.2} {10} {11}", 
		         123, 'x', false, 123.456, "abc", (Size)100, -100, "xyz", 12.34f, 1, 2);
		REQUIRE(String("") == pOutput->m_sOutput);


		pOutput->m_sOutput.clear();
		CXDBG("mytag", "test {1} {2} {3} {4:'0'.3} {5} {6} {7} {8} {9:'0'.2} {10} {11}", 123, 'x', 
		      false, 123.456, "abc", (Size)100, -100, "xyz", 12.34f, 1, 2);
		REQUIRE(String("[DBG] [    mytag   ]  : test 123 x false 123.456 abc 100 -100 xyz 12.34 1 "
		        "2\n") == pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXDBGIF(true, "mytag", "test {1} {2} {3} {4:'0'.3} {5} {6} {7} {8} {9:'0'.2} {10} {11}", 123, 
		        'x', false, 123.456, "abc", (Size)100, -100, "xyz", 12.34f, 1, 2);
		REQUIRE(String("[DBG] [    mytag   ]  : test 123 x false 123.456 abc 100 -100 xyz 12.34 1 "
		        "2\n") == pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXDBGIF(false, "mytag", "test {1} {2} {3} {4:'0'.3} {5} {6} {7} {8} {9:'0'.2} {10} {11}", 
		        123, 'x', false, 123.456, "abc", (Size)100, -100, "xyz", 12.34f, 1, 2);
		REQUIRE(String("") == pOutput->m_sOutput);


		pOutput->m_sOutput.clear();
		CXVERB("mytag", "test {1} {2} {3} {4:'0'.3} {5} {6} {7} {8} {9:'0'.2} {10} {11}", 123, 'x', 
		       false, 123.456, "abc", (Size)100, -100, "xyz", 12.34f, 1, 2);
		REQUIRE(String("[VRB] [    mytag   ]  : test 123 x false 123.456 abc 100 -100 xyz 12.34 1 "
		        "2\n") == pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXVERBIF(true, "mytag", "test {1} {2} {3} {4:'0'.3} {5} {6} {7} {8} {9:'0'.2} {10} {11}", 
		         123, 'x', false, 123.456, "abc", (Size)100, -100, "xyz", 12.34f, 1, 2);
		REQUIRE(String("[VRB] [    mytag   ]  : test 123 x false 123.456 abc 100 -100 xyz 12.34 1 "
		        "2\n") == pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXVERBIF(false, "mytag", "test {1} {2} {3} {4:'0'.3} {5} {6} {7} {8} {9:'0'.2} {10} {11}", 
		         123, 'x', false, 123.456, "abc", (Size)100, -100, "xyz", 12.34f, 1, 2);
		REQUIRE(String("") == pOutput->m_sOutput);

		Log::Logger::GetDefaultLogger().SetLevel(Log::Level_Error);

		pOutput->m_sOutput.clear();
		CXINFO("mytag", "test {1} {2} {3} {4:'0'.3} {5} {6} {7} {8} {9:'0'.2} {10} {11}", 123, 'x', 
		       false, 123.456, "abc", (Size)100, -100, "xyz", 12.34f, 1, 2);
		REQUIRE(String("") == pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXINFOIF(true, "mytag", "test {1} {2} {3} {4:'0'.3} {5} {6} {7} {8} {9:'0'.2} {10} {11}", 
		         123, 'x', false, 123.456, "abc", (Size)100, -100, "xyz", 12.34f, 1, 2);
		REQUIRE(String("") == pOutput->m_sOutput);
	}

	SECTION("12 args")
	{
		MyOutput *pOutput = New<MyOutput>();

		((Log::DefaultFormatter *)Log::Logger::GetDefaultLogger().GetFormatter())->
		     SetFlags(Log::DefaultFormatter::Show_Tag | Log::DefaultFormatter::Show_Level | 
		              Log::DefaultFormatter::Show_NewLine);
		Log::Logger::GetDefaultLogger().AddOutput(pOutput);
		Log::Logger::GetDefaultLogger().SetLevel(Log::Level_Debug);

		pOutput->m_sOutput.clear();
		CXERR("mytag", "test {1} {2} {3} {4:'0'.3} {5} {6} {7} {8} {9:'0'.2} {10} {11} {12}", 123, 
		      'x', false, 123.456, "abc", (Size)100, -100, "xyz", 12.34f, 1, 2, 3);
		REQUIRE(String("[ERR] [    mytag   ]  : test 123 x false 123.456 abc 100 -100 xyz 12.34 1 "
		        "2 3\n") == pOutput->m_sOutput);
		
		pOutput->m_sOutput.clear();
		CXERRIF(true, "mytag", "test {1} {2} {3} {4:'0'.3} {5} {6} {7} {8} {9:'0'.2} {10} {11} {12}", 
		        123, 'x', false, 123.456, "abc", (Size)100, -100, "xyz", 12.34f, 1, 2, 3);
		REQUIRE(String("[ERR] [    mytag   ]  : test 123 x false 123.456 abc 100 -100 xyz 12.34 1 "
		        "2 3\n") == pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXERRIF(false, "mytag", "test {1} {2} {3} {4:'0'.3} {5} {6} {7} {8} {9:'0'.2} {10} {11} "
		        "{12}", 123, 'x', false, 123.456, "abc", (Size)100, -100, "xyz", 12.34f, 1, 2, 3);
		REQUIRE(String("") == pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXWARN("mytag", "test {1} {2} {3} {4:'0'.3} {5} {6} {7} {8} {9:'0'.2} {10} {11} {12}", 123, 
		       'x', false, 123.456, "abc", (Size)100, -100, "xyz", 12.34f, 1, 2, 3);
		REQUIRE(String("[WRN] [    mytag   ]  : test 123 x false 123.456 abc 100 -100 xyz 12.34 1 "
		        "2 3\n") == pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXWARNIF(true, "mytag", "test {1} {2} {3} {4:'0'.3} {5} {6} {7} {8} {9:'0'.2} {10} {11} "
		         "{12}", 123, 'x', false, 123.456, "abc", (Size)100, -100, "xyz", 12.34f, 1, 2, 3);
		REQUIRE(String("[WRN] [    mytag   ]  : test 123 x false 123.456 abc 100 -100 xyz 12.34 1 "
		        "2 3\n") == pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXWARNIF(false, "mytag", "test {1} {2} {3} {4:'0'.3} {5} {6} {7} {8} {9:'0'.2} {10} {11} "
		         "{12}", 123, 'x', false, 123.456, "abc", (Size)100, -100, "xyz", 12.34f, 1, 2, 3);
		REQUIRE(String("") == pOutput->m_sOutput);


		pOutput->m_sOutput.clear();
		CXINFO("mytag", "test {1} {2} {3} {4:'0'.3} {5} {6} {7} {8} {9:'0'.2} {10} {11} {12}", 123, 
		       'x', false, 123.456, "abc", (Size)100, -100, "xyz", 12.34f, 1, 2, 3);
		REQUIRE(String("[INF] [    mytag   ]  : test 123 x false 123.456 abc 100 -100 xyz 12.34 1 "
		        "2 3\n") == pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXINFOIF(true, "mytag", "test {1} {2} {3} {4:'0'.3} {5} {6} {7} {8} {9:'0'.2} {10} {11} "
		         "{12}", 123, 'x', false, 123.456, "abc", (Size)100, -100, "xyz", 12.34f, 1, 2, 3);
		REQUIRE(String("[INF] [    mytag   ]  : test 123 x false 123.456 abc 100 -100 xyz 12.34 1 "
		        "2 3\n") == pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXINFOIF(false, "mytag", "test {1} {2} {3} {4:'0'.3} {5} {6} {7} {8} {9:'0'.2} {10} {11} "
		         "{12}", 123, 'x', false, 123.456, "abc", (Size)100, -100, "xyz", 12.34f, 1, 2, 3);
		REQUIRE(String("") == pOutput->m_sOutput);


		pOutput->m_sOutput.clear();
		CXDBG("mytag", "test {1} {2} {3} {4:'0'.3} {5} {6} {7} {8} {9:'0'.2} {10} {11} {12}", 123, 
		      'x', false, 123.456, "abc", (Size)100, -100, "xyz", 12.34f, 1, 2, 3);
		REQUIRE(String("[DBG] [    mytag   ]  : test 123 x false 123.456 abc 100 -100 xyz 12.34 1 "
		        "2 3\n") == pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXDBGIF(true, "mytag", "test {1} {2} {3} {4:'0'.3} {5} {6} {7} {8} {9:'0'.2} {10} {11} {12}", 
		        123, 'x', false, 123.456, "abc", (Size)100, -100, "xyz", 12.34f, 1, 2, 3);
		REQUIRE(String("[DBG] [    mytag   ]  : test 123 x false 123.456 abc 100 -100 xyz 12.34 1 "
		        "2 3\n") == pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXDBGIF(false, "mytag", "test {1} {2} {3} {4:'0'.3} {5} {6} {7} {8} {9:'0'.2} {10} {11} "
		        "{12}", 123, 'x', false, 123.456, "abc", (Size)100, -100, "xyz", 12.34f, 1, 2, 3);
		REQUIRE(String("") == pOutput->m_sOutput);


		pOutput->m_sOutput.clear();
		CXVERB("mytag", "test {1} {2} {3} {4:'0'.3} {5} {6} {7} {8} {9:'0'.2} {10} {11} {12}", 123, 
		       'x', false, 123.456, "abc", (Size)100, -100, "xyz", 12.34f, 1, 2, 3);
		REQUIRE(String("[VRB] [    mytag   ]  : test 123 x false 123.456 abc 100 -100 xyz 12.34 1 "
		        "2 3\n") == pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXVERBIF(true, "mytag", "test {1} {2} {3} {4:'0'.3} {5} {6} {7} {8} {9:'0'.2} {10} {11} "
		         "{12}", 123, 'x', false, 123.456, "abc", (Size)100, -100, "xyz", 12.34f, 1, 2, 3);
		REQUIRE(String("[VRB] [    mytag   ]  : test 123 x false 123.456 abc 100 -100 xyz 12.34 1 "
		        "2 3\n") == pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXVERBIF(false, "mytag", "test {1} {2} {3} {4:'0'.3} {5} {6} {7} {8} {9:'0'.2} {10} {11} "
		         "{12}", 123, 'x', false, 123.456, "abc", (Size)100, -100, "xyz", 12.34f, 1, 2, 3);
		REQUIRE(String("") == pOutput->m_sOutput);

		Log::Logger::GetDefaultLogger().SetLevel(Log::Level_Error);

		pOutput->m_sOutput.clear();
		CXINFO("mytag", "test {1} {2} {3} {4:'0'.3} {5} {6} {7} {8} {9:'0'.2} {10} {11} {12}", 123, 
		       'x', false, 123.456, "abc", (Size)100, -100, "xyz", 12.34f, 1, 2, 3);
		REQUIRE(String("") == pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXINFOIF(true, "mytag", "test {1} {2} {3} {4:'0'.3} {5} {6} {7} {8} {9:'0'.2} {10} {11} "
		         "{12}", 123, 'x', false, 123.456, "abc", (Size)100, -100, "xyz", 12.34f, 1, 2, 3);
		REQUIRE(String("") == pOutput->m_sOutput);
	}

	SECTION("13 args")
	{
		MyOutput *pOutput = New<MyOutput>();

		((Log::DefaultFormatter *)Log::Logger::GetDefaultLogger().GetFormatter())->
		     SetFlags(Log::DefaultFormatter::Show_Tag | Log::DefaultFormatter::Show_Level | 
		              Log::DefaultFormatter::Show_NewLine);
		Log::Logger::GetDefaultLogger().AddOutput(pOutput);
		Log::Logger::GetDefaultLogger().SetLevel(Log::Level_Debug);

		pOutput->m_sOutput.clear();
		CXERR("mytag", "test {1} {2} {3} {4:'0'.3} {5} {6} {7} {8} {9:'0'.2} {10} {11} {12} {13}", 
		      123, 'x', false, 123.456, "abc", (Size)100, -100, "xyz", 12.34f, 1, 2, 3, 4);
		REQUIRE(String("[ERR] [    mytag   ]  : test 123 x false 123.456 abc 100 -100 xyz 12.34 1 "
		        "2 3 4\n") == pOutput->m_sOutput);
		
		pOutput->m_sOutput.clear();
		CXERRIF(true, "mytag", "test {1} {2} {3} {4:'0'.3} {5} {6} {7} {8} {9:'0'.2} {10} {11} {12} "
		        "{13}", 123, 'x', false, 123.456, "abc", (Size)100, -100, "xyz", 12.34f, 1, 2, 3, 4);
		REQUIRE(String("[ERR] [    mytag   ]  : test 123 x false 123.456 abc 100 -100 xyz 12.34 1 "
		        "2 3 4\n") == pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXERRIF(false, "mytag", "test {1} {2} {3} {4:'0'.3} {5} {6} {7} {8} {9:'0'.2} {10} {11} "
		        "{12} {13}", 123, 'x', false, 123.456, "abc", (Size)100, -100, "xyz", 12.34f, 1, 2, 
		        3, 4);
		REQUIRE(String("") == pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXWARN("mytag", "test {1} {2} {3} {4:'0'.3} {5} {6} {7} {8} {9:'0'.2} {10} {11} {12} {13}", 
		       123, 'x', false, 123.456, "abc", (Size)100, -100, "xyz", 12.34f, 1, 2, 3, 4);
		REQUIRE(String("[WRN] [    mytag   ]  : test 123 x false 123.456 abc 100 -100 xyz 12.34 1 "
		        "2 3 4\n") == pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXWARNIF(true, "mytag", "test {1} {2} {3} {4:'0'.3} {5} {6} {7} {8} {9:'0'.2} {10} {11} "
		         "{12} {13}", 123, 'x', false, 123.456, "abc", (Size)100, -100, "xyz", 12.34f, 1, 2, 
		         3, 4);
		REQUIRE(String("[WRN] [    mytag   ]  : test 123 x false 123.456 abc 100 -100 xyz 12.34 1 "
		        "2 3 4\n") == pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXWARNIF(false, "mytag", "test {1} {2} {3} {4:'0'.3} {5} {6} {7} {8} {9:'0'.2} {10} {11} "
		         "{12} {13}", 123, 'x', false, 123.456, "abc", (Size)100, -100, "xyz", 12.34f, 1, 2, 
		         3, 4);
		REQUIRE(String("") == pOutput->m_sOutput);


		pOutput->m_sOutput.clear();
		CXINFO("mytag", "test {1} {2} {3} {4:'0'.3} {5} {6} {7} {8} {9:'0'.2} {10} {11} {12} {13}", 
		       123, 'x', false, 123.456, "abc", (Size)100, -100, "xyz", 12.34f, 1, 2, 3, 4);
		REQUIRE(String("[INF] [    mytag   ]  : test 123 x false 123.456 abc 100 -100 xyz 12.34 1 "
		        "2 3 4\n") == pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXINFOIF(true, "mytag", "test {1} {2} {3} {4:'0'.3} {5} {6} {7} {8} {9:'0'.2} {10} {11} "
		         "{12} {13}", 123, 'x', false, 123.456, "abc", (Size)100, -100, "xyz", 12.34f, 1, 2, 
		         3, 4);
		REQUIRE(String("[INF] [    mytag   ]  : test 123 x false 123.456 abc 100 -100 xyz 12.34 1 "
		        "2 3 4\n") == pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXINFOIF(false, "mytag", "test {1} {2} {3} {4:'0'.3} {5} {6} {7} {8} {9:'0'.2} {10} {11} "
		         "{12} {13}", 123, 'x', false, 123.456, "abc", (Size)100, -100, "xyz", 12.34f, 1, 2, 
		         3, 4);
		REQUIRE(String("") == pOutput->m_sOutput);


		pOutput->m_sOutput.clear();
		CXDBG("mytag", "test {1} {2} {3} {4:'0'.3} {5} {6} {7} {8} {9:'0'.2} {10} {11} {12} {13}", 
		      123, 'x', false, 123.456, "abc", (Size)100, -100, "xyz", 12.34f, 1, 2, 3, 4);
		REQUIRE(String("[DBG] [    mytag   ]  : test 123 x false 123.456 abc 100 -100 xyz 12.34 1 "
		        "2 3 4\n") == pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXDBGIF(true, "mytag", "test {1} {2} {3} {4:'0'.3} {5} {6} {7} {8} {9:'0'.2} {10} {11} {12} "
		        "{13}", 123, 'x', false, 123.456, "abc", (Size)100, -100, "xyz", 12.34f, 1, 2, 3, 4);
		REQUIRE(String("[DBG] [    mytag   ]  : test 123 x false 123.456 abc 100 -100 xyz 12.34 1 "
		        "2 3 4\n") == pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXDBGIF(false, "mytag", "test {1} {2} {3} {4:'0'.3} {5} {6} {7} {8} {9:'0'.2} {10} {11} "
		        "{12} {13}", 123, 'x', false, 123.456, "abc", (Size)100, -100, "xyz", 12.34f, 1, 2, 
		        3, 4);
		REQUIRE(String("") == pOutput->m_sOutput);


		pOutput->m_sOutput.clear();
		CXVERB("mytag", "test {1} {2} {3} {4:'0'.3} {5} {6} {7} {8} {9:'0'.2} {10} {11} {12} {13}", 
		       123, 'x', false, 123.456, "abc", (Size)100, -100, "xyz", 12.34f, 1, 2, 3, 4);
		REQUIRE(String("[VRB] [    mytag   ]  : test 123 x false 123.456 abc 100 -100 xyz 12.34 1 "
		        "2 3 4\n") == pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXVERBIF(true, "mytag", "test {1} {2} {3} {4:'0'.3} {5} {6} {7} {8} {9:'0'.2} {10} {11} "
		         "{12} {13}", 123, 'x', false, 123.456, "abc", (Size)100, -100, "xyz", 12.34f, 1, 2, 
		         3, 4);
		REQUIRE(String("[VRB] [    mytag   ]  : test 123 x false 123.456 abc 100 -100 xyz 12.34 1 "
		        "2 3 4\n") == pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXVERBIF(false, "mytag", "test {1} {2} {3} {4:'0'.3} {5} {6} {7} {8} {9:'0'.2} {10} {11} "
		         "{12} {13}", 123, 'x', false, 123.456, "abc", (Size)100, -100, "xyz", 12.34f, 1, 2, 
		         3, 4);
		REQUIRE(String("") == pOutput->m_sOutput);

		Log::Logger::GetDefaultLogger().SetLevel(Log::Level_Error);

		pOutput->m_sOutput.clear();
		CXINFO("mytag", "test {1} {2} {3} {4:'0'.3} {5} {6} {7} {8} {9:'0'.2} {10} {11} {12} {13}", 
		       123, 'x', false, 123.456, "abc", (Size)100, -100, "xyz", 12.34f, 1, 2, 3, 4);
		REQUIRE(String("") == pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXINFOIF(true, "mytag", "test {1} {2} {3} {4:'0'.3} {5} {6} {7} {8} {9:'0'.2} {10} {11} "
		         "{12} {13}", 123, 'x', false, 123.456, "abc", (Size)100, -100, "xyz", 12.34f, 1, 2, 
		         3, 4);
		REQUIRE(String("") == pOutput->m_sOutput);
	}

	SECTION("14 args")
	{
		MyOutput *pOutput = New<MyOutput>();

		((Log::DefaultFormatter *)Log::Logger::GetDefaultLogger().GetFormatter())->
		     SetFlags(Log::DefaultFormatter::Show_Tag | Log::DefaultFormatter::Show_Level | 
		              Log::DefaultFormatter::Show_NewLine);
		Log::Logger::GetDefaultLogger().AddOutput(pOutput);
		Log::Logger::GetDefaultLogger().SetLevel(Log::Level_Debug);

		pOutput->m_sOutput.clear();
		CXERR("mytag", "test {1} {2} {3} {4:'0'.3} {5} {6} {7} {8} {9:'0'.2} {10} {11} {12} {13} "
		      "{14}", 123, 'x', false, 123.456, "abc", (Size)100, -100, "xyz", 12.34f, 1, 2, 3, 4, 5);
		REQUIRE(String("[ERR] [    mytag   ]  : test 123 x false 123.456 abc 100 -100 xyz 12.34 1 "
		        "2 3 4 5\n") == pOutput->m_sOutput);
		
		pOutput->m_sOutput.clear();
		CXERRIF(true, "mytag", "test {1} {2} {3} {4:'0'.3} {5} {6} {7} {8} {9:'0'.2} {10} {11} {12} "
		        "{13} {14}", 123, 'x', false, 123.456, "abc", (Size)100, -100, "xyz", 12.34f, 1, 2, 
		        3, 4, 5);
		REQUIRE(String("[ERR] [    mytag   ]  : test 123 x false 123.456 abc 100 -100 xyz 12.34 1 "
		        "2 3 4 5\n") == pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXERRIF(false, "mytag", "test {1} {2} {3} {4:'0'.3} {5} {6} {7} {8} {9:'0'.2} {10} {11} "
		        "{12} {13} {14}", 123, 'x', false, 123.456, "abc", (Size)100, -100, "xyz", 12.34f, 1, 
		        2, 3, 4, 5);
		REQUIRE(String("") == pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXWARN("mytag", "test {1} {2} {3} {4:'0'.3} {5} {6} {7} {8} {9:'0'.2} {10} {11} {12} {13} "
		       "{14}", 123, 'x', false, 123.456, "abc", (Size)100, -100, "xyz", 12.34f, 1, 2, 3, 4, 
		       5);
		REQUIRE(String("[WRN] [    mytag   ]  : test 123 x false 123.456 abc 100 -100 xyz 12.34 1 "
		        "2 3 4 5\n") == pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXWARNIF(true, "mytag", "test {1} {2} {3} {4:'0'.3} {5} {6} {7} {8} {9:'0'.2} {10} {11} "
		         "{12} {13} {14}", 123, 'x', false, 123.456, "abc", (Size)100, -100, "xyz", 12.34f, 
		         1, 2, 3, 4, 5);
		REQUIRE(String("[WRN] [    mytag   ]  : test 123 x false 123.456 abc 100 -100 xyz 12.34 1 "
		        "2 3 4 5\n") == pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXWARNIF(false, "mytag", "test {1} {2} {3} {4:'0'.3} {5} {6} {7} {8} {9:'0'.2} {10} {11} "
		         "{12} {13} {14}", 123, 'x', false, 123.456, "abc", (Size)100, -100, "xyz", 12.34f, 
		         1, 2, 3, 4, 5);
		REQUIRE(String("") == pOutput->m_sOutput);


		pOutput->m_sOutput.clear();
		CXINFO("mytag", "test {1} {2} {3} {4:'0'.3} {5} {6} {7} {8} {9:'0'.2} {10} {11} {12} {13} "
		       "{14}", 123, 'x', false, 123.456, "abc", (Size)100, -100, "xyz", 12.34f, 1, 2, 3, 4, 
		       5);
		REQUIRE(String("[INF] [    mytag   ]  : test 123 x false 123.456 abc 100 -100 xyz 12.34 1 "
		        "2 3 4 5\n") == pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXINFOIF(true, "mytag", "test {1} {2} {3} {4:'0'.3} {5} {6} {7} {8} {9:'0'.2} {10} {11} "
		         "{12} {13} {14}", 123, 'x', false, 123.456, "abc", (Size)100, -100, "xyz", 12.34f, 
		         1, 2, 3, 4, 5);
		REQUIRE(String("[INF] [    mytag   ]  : test 123 x false 123.456 abc 100 -100 xyz 12.34 1 "
		        "2 3 4 5\n") == pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXINFOIF(false, "mytag", "test {1} {2} {3} {4:'0'.3} {5} {6} {7} {8} {9:'0'.2} {10} {11} "
		         "{12} {13} {14}", 123, 'x', false, 123.456, "abc", (Size)100, -100, "xyz", 12.34f, 
		         1, 2, 3, 4, 5);
		REQUIRE(String("") == pOutput->m_sOutput);


		pOutput->m_sOutput.clear();
		CXDBG("mytag", "test {1} {2} {3} {4:'0'.3} {5} {6} {7} {8} {9:'0'.2} {10} {11} {12} {13} "
		      "{14}", 123, 'x', false, 123.456, "abc", (Size)100, -100, "xyz", 12.34f, 1, 2, 3, 4, 5);
		REQUIRE(String("[DBG] [    mytag   ]  : test 123 x false 123.456 abc 100 -100 xyz 12.34 1 "
		        "2 3 4 5\n") == pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXDBGIF(true, "mytag", "test {1} {2} {3} {4:'0'.3} {5} {6} {7} {8} {9:'0'.2} {10} {11} {12} "
		        "{13} {14}", 123, 'x', false, 123.456, "abc", (Size)100, -100, "xyz", 12.34f, 1, 2, 
		        3, 4, 5);
		REQUIRE(String("[DBG] [    mytag   ]  : test 123 x false 123.456 abc 100 -100 xyz 12.34 1 "
		        "2 3 4 5\n") == pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXDBGIF(false, "mytag", "test {1} {2} {3} {4:'0'.3} {5} {6} {7} {8} {9:'0'.2} {10} {11} "
		        "{12} {13} {14}", 123, 'x', false, 123.456, "abc", (Size)100, -100, "xyz", 12.34f, 1, 
		        2, 3, 4, 5);
		REQUIRE(String("") == pOutput->m_sOutput);


		pOutput->m_sOutput.clear();
		CXVERB("mytag", "test {1} {2} {3} {4:'0'.3} {5} {6} {7} {8} {9:'0'.2} {10} {11} {12} {13} "
		       "{14}", 123, 'x', false, 123.456, "abc", (Size)100, -100, "xyz", 12.34f, 1, 2, 3, 4, 
		       5);
		REQUIRE(String("[VRB] [    mytag   ]  : test 123 x false 123.456 abc 100 -100 xyz 12.34 1 "
		        "2 3 4 5\n") == pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXVERBIF(true, "mytag", "test {1} {2} {3} {4:'0'.3} {5} {6} {7} {8} {9:'0'.2} {10} {11} "
		         "{12} {13} {14}", 123, 'x', false, 123.456, "abc", (Size)100, -100, "xyz", 12.34f, 
		         1, 2, 3, 4, 5);
		REQUIRE(String("[VRB] [    mytag   ]  : test 123 x false 123.456 abc 100 -100 xyz 12.34 1 "
		        "2 3 4 5\n") == pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXVERBIF(false, "mytag", "test {1} {2} {3} {4:'0'.3} {5} {6} {7} {8} {9:'0'.2} {10} {11} "
		         "{12} {13} {14}", 123, 'x', false, 123.456, "abc", (Size)100, -100, "xyz", 12.34f, 
		         1, 2, 3, 4, 5);
		REQUIRE(String("") == pOutput->m_sOutput);

		Log::Logger::GetDefaultLogger().SetLevel(Log::Level_Error);

		pOutput->m_sOutput.clear();
		CXINFO("mytag", "test {1} {2} {3} {4:'0'.3} {5} {6} {7} {8} {9:'0'.2} {10} {11} {12} {13} "
		       "{14}", 123, 'x', false, 123.456, "abc", (Size)100, -100, "xyz", 12.34f, 1, 2, 3, 4, 
		       5);
		REQUIRE(String("") == pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXINFOIF(true, "mytag", "test {1} {2} {3} {4:'0'.3} {5} {6} {7} {8} {9:'0'.2} {10} {11} "
		         "{12} {13} {14}", 123, 'x', false, 123.456, "abc", (Size)100, -100, "xyz", 12.34f, 
		         1, 2, 3, 4, 5);
		REQUIRE(String("") == pOutput->m_sOutput);
	}

	SECTION("15 args")
	{
		MyOutput *pOutput = New<MyOutput>();

		((Log::DefaultFormatter *)Log::Logger::GetDefaultLogger().GetFormatter())->
		     SetFlags(Log::DefaultFormatter::Show_Tag | Log::DefaultFormatter::Show_Level | 
		              Log::DefaultFormatter::Show_NewLine);
		Log::Logger::GetDefaultLogger().AddOutput(pOutput);
		Log::Logger::GetDefaultLogger().SetLevel(Log::Level_Debug);

		pOutput->m_sOutput.clear();
		CXERR("mytag", "test {1} {2} {3} {4:'0'.3} {5} {6} {7} {8} {9:'0'.2} {10} {11} {12} {13} "
		      "{14} {15}", 123, 'x', false, 123.456, "abc", (Size)100, -100, "xyz", 12.34f, 1, 2, 3, 
		      4, 5, 6);
		REQUIRE(String("[ERR] [    mytag   ]  : test 123 x false 123.456 abc 100 -100 xyz 12.34 1 "
		        "2 3 4 5 6\n") == pOutput->m_sOutput);
		
		pOutput->m_sOutput.clear();
		CXERRIF(true, "mytag", "test {1} {2} {3} {4:'0'.3} {5} {6} {7} {8} {9:'0'.2} {10} {11} {12} "
		        "{13} {14} {15}", 123, 'x', false, 123.456, "abc", (Size)100, -100, "xyz", 12.34f, 1,
		        2, 3, 4, 5, 6);
		REQUIRE(String("[ERR] [    mytag   ]  : test 123 x false 123.456 abc 100 -100 xyz 12.34 1 "
		        "2 3 4 5 6\n") == pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXERRIF(false, "mytag", "test {1} {2} {3} {4:'0'.3} {5} {6} {7} {8} {9:'0'.2} {10} {11} "
		        "{12} {13} {14} {15}", 123, 'x', false, 123.456, "abc", (Size)100, -100, "xyz", 
		        12.34f, 1, 2, 3, 4, 5, 6);
		REQUIRE(String("") == pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXWARN("mytag", "test {1} {2} {3} {4:'0'.3} {5} {6} {7} {8} {9:'0'.2} {10} {11} {12} {13} "
		       "{14} {15}", 123, 'x', false, 123.456, "abc", (Size)100, -100, "xyz", 12.34f, 1, 2, 3, 
		       4, 5, 6);
		REQUIRE(String("[WRN] [    mytag   ]  : test 123 x false 123.456 abc 100 -100 xyz 12.34 1 "
		        "2 3 4 5 6\n") == pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXWARNIF(true, "mytag", "test {1} {2} {3} {4:'0'.3} {5} {6} {7} {8} {9:'0'.2} {10} {11} "
		         "{12} {13} {14} {15}", 123, 'x', false, 123.456, "abc", (Size)100, -100, "xyz", 
		         12.34f, 1, 2, 3, 4, 5, 6);
		REQUIRE(String("[WRN] [    mytag   ]  : test 123 x false 123.456 abc 100 -100 xyz 12.34 1 "
		        "2 3 4 5 6\n") == pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXWARNIF(false, "mytag", "test {1} {2} {3} {4:'0'.3} {5} {6} {7} {8} {9:'0'.2} {10} {11} "
		         "{12} {13} {14} {15}", 123, 'x', false, 123.456, "abc", (Size)100, -100, "xyz", 
		         12.34f, 1, 2, 3, 4, 5, 6);
		REQUIRE(String("") == pOutput->m_sOutput);


		pOutput->m_sOutput.clear();
		CXINFO("mytag", "test {1} {2} {3} {4:'0'.3} {5} {6} {7} {8} {9:'0'.2} {10} {11} {12} {13} "
		       "{14} {15}", 123, 'x', false, 123.456, "abc", (Size)100, -100, "xyz", 12.34f, 1, 2, 3, 
		       4, 5, 6);
		REQUIRE(String("[INF] [    mytag   ]  : test 123 x false 123.456 abc 100 -100 xyz 12.34 1 "
		        "2 3 4 5 6\n") == pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXINFOIF(true, "mytag", "test {1} {2} {3} {4:'0'.3} {5} {6} {7} {8} {9:'0'.2} {10} {11} "
		         "{12} {13} {14} {15}", 123, 'x', false, 123.456, "abc", (Size)100, -100, "xyz", 
		         12.34f, 1, 2, 3, 4, 5, 6);
		REQUIRE(String("[INF] [    mytag   ]  : test 123 x false 123.456 abc 100 -100 xyz 12.34 1 "
		        "2 3 4 5 6\n") == pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXINFOIF(false, "mytag", "test {1} {2} {3} {4:'0'.3} {5} {6} {7} {8} {9:'0'.2} {10} {11} "
		         "{12} {13} {14} {15}", 123, 'x', false, 123.456, "abc", (Size)100, -100, "xyz", 
		         12.34f, 1, 2, 3, 4, 5, 6);
		REQUIRE(String("") == pOutput->m_sOutput);


		pOutput->m_sOutput.clear();
		CXDBG("mytag", "test {1} {2} {3} {4:'0'.3} {5} {6} {7} {8} {9:'0'.2} {10} {11} {12} {13} "
		      "{14} {15}", 123, 'x', false, 123.456, "abc", (Size)100, -100, "xyz", 12.34f, 1, 2, 3, 
		      4, 5, 6);
		REQUIRE(String("[DBG] [    mytag   ]  : test 123 x false 123.456 abc 100 -100 xyz 12.34 1 "
		        "2 3 4 5 6\n") == pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXDBGIF(true, "mytag", "test {1} {2} {3} {4:'0'.3} {5} {6} {7} {8} {9:'0'.2} {10} {11} {12} "
		        "{13} {14} {15}", 123, 'x', false, 123.456, "abc", (Size)100, -100, "xyz", 12.34f, 1, 
		        2, 3, 4, 5, 6);
		REQUIRE(String("[DBG] [    mytag   ]  : test 123 x false 123.456 abc 100 -100 xyz 12.34 1 "
		        "2 3 4 5 6\n") == pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXDBGIF(false, "mytag", "test {1} {2} {3} {4:'0'.3} {5} {6} {7} {8} {9:'0'.2} {10} {11} "
		        "{12} {13} {14} {15}", 123, 'x', false, 123.456, "abc", (Size)100, -100, "xyz", 
		        12.34f, 1, 2, 3, 4, 5, 6);
		REQUIRE(String("") == pOutput->m_sOutput);


		pOutput->m_sOutput.clear();
		CXVERB("mytag", "test {1} {2} {3} {4:'0'.3} {5} {6} {7} {8} {9:'0'.2} {10} {11} {12} {13} "
		       "{14} {15}", 123, 'x', false, 123.456, "abc", (Size)100, -100, "xyz", 12.34f, 1, 2, 3, 
		       4, 5, 6);
		REQUIRE(String("[VRB] [    mytag   ]  : test 123 x false 123.456 abc 100 -100 xyz 12.34 1 "
		        "2 3 4 5 6\n") == pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXVERBIF(true, "mytag", "test {1} {2} {3} {4:'0'.3} {5} {6} {7} {8} {9:'0'.2} {10} {11} "
		         "{12} {13} {14} {15}", 123, 'x', false, 123.456, "abc", (Size)100, -100, "xyz", 
		         12.34f, 1, 2, 3, 4, 5, 6);
		REQUIRE(String("[VRB] [    mytag   ]  : test 123 x false 123.456 abc 100 -100 xyz 12.34 1 "
		        "2 3 4 5 6\n") == pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXVERBIF(false, "mytag", "test {1} {2} {3} {4:'0'.3} {5} {6} {7} {8} {9:'0'.2} {10} {11} "
		         "{12} {13} {14} {15}", 123, 'x', false, 123.456, "abc", (Size)100, -100, "xyz", 
		         12.34f, 1, 2, 3, 4, 5, 6);
		REQUIRE(String("") == pOutput->m_sOutput);

		Log::Logger::GetDefaultLogger().SetLevel(Log::Level_Error);

		pOutput->m_sOutput.clear();
		CXINFO("mytag", "test {1} {2} {3} {4:'0'.3} {5} {6} {7} {8} {9:'0'.2} {10} {11} {12} {13} "
		       "{14} {15}", 123, 'x', false, 123.456, "abc", (Size)100, -100, "xyz", 12.34f, 1, 2, 3, 
		       4, 5, 6);
		REQUIRE(String("") == pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXINFOIF(true, "mytag", "test {1} {2} {3} {4:'0'.3} {5} {6} {7} {8} {9:'0'.2} {10} {11} "
		         "{12} {13} {14} {15}", 123, 'x', false, 123.456, "abc", (Size)100, -100, "xyz", 
		         12.34f, 1, 2, 3, 4, 5, 6);
		REQUIRE(String("") == pOutput->m_sOutput);
	}

	SECTION("16 args")
	{
		MyOutput *pOutput = New<MyOutput>();

		((Log::DefaultFormatter *)Log::Logger::GetDefaultLogger().GetFormatter())->
		     SetFlags(Log::DefaultFormatter::Show_Tag | Log::DefaultFormatter::Show_Level | 
		              Log::DefaultFormatter::Show_NewLine);
		Log::Logger::GetDefaultLogger().AddOutput(pOutput);
		Log::Logger::GetDefaultLogger().SetLevel(Log::Level_Debug);

		pOutput->m_sOutput.clear();
		CXERR("mytag", "test {1} {2} {3} {4:'0'.3} {5} {6} {7} {8} {9:'0'.2} {10} {11} {12} {13} "
		      "{14} {15} {16}", 123, 'x', false, 123.456, "abc", (Size)100, -100, "xyz", 12.34f, 1, 
		      2, 3, 4, 5, 6, 7);
		REQUIRE(String("[ERR] [    mytag   ]  : test 123 x false 123.456 abc 100 -100 xyz 12.34 1 "
		        "2 3 4 5 6 7\n") == pOutput->m_sOutput);
		
		pOutput->m_sOutput.clear();
		CXERRIF(true, "mytag", "test {1} {2} {3} {4:'0'.3} {5} {6} {7} {8} {9:'0'.2} {10} {11} {12} "
		        "{13} {14} {15} {16}", 123, 'x', false, 123.456, "abc", (Size)100, -100, "xyz", 
		        12.34f, 1, 2, 3, 4, 5, 6, 7);
		REQUIRE(String("[ERR] [    mytag   ]  : test 123 x false 123.456 abc 100 -100 xyz 12.34 1 "
		        "2 3 4 5 6 7\n") == pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXERRIF(false, "mytag", "test {1} {2} {3} {4:'0'.3} {5} {6} {7} {8} {9:'0'.2} {10} {11} "
		        "{12} {13} {14} {15} {16}", 123, 'x', false, 123.456, "abc", (Size)100, -100, "xyz", 
		        12.34f, 1, 2, 3, 4, 5, 6, 7);
		REQUIRE(String("") == pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXWARN("mytag", "test {1} {2} {3} {4:'0'.3} {5} {6} {7} {8} {9:'0'.2} {10} {11} {12} {13} "
		       "{14} {15} {16}", 123, 'x', false, 123.456, "abc", (Size)100, -100, "xyz", 12.34f, 1, 
		       2, 3, 4, 5, 6, 7);
		REQUIRE(String("[WRN] [    mytag   ]  : test 123 x false 123.456 abc 100 -100 xyz 12.34 1 "
		        "2 3 4 5 6 7\n") == pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXWARNIF(true, "mytag", "test {1} {2} {3} {4:'0'.3} {5} {6} {7} {8} {9:'0'.2} {10} {11} "
		         "{12} {13} {14} {15} {16}", 123, 'x', false, 123.456, "abc", (Size)100, -100, "xyz", 
		         12.34f, 1, 2, 3, 4, 5, 6, 7);
		REQUIRE(String("[WRN] [    mytag   ]  : test 123 x false 123.456 abc 100 -100 xyz 12.34 1 "
		        "2 3 4 5 6 7\n") == pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXWARNIF(false, "mytag", "test {1} {2} {3} {4:'0'.3} {5} {6} {7} {8} {9:'0'.2} {10} {11} "
		         "{12} {13} {14} {15} {16}", 123, 'x', false, 123.456, "abc", (Size)100, -100, "xyz", 
		         12.34f, 1, 2, 3, 4, 5, 6, 7);
		REQUIRE(String("") == pOutput->m_sOutput);


		pOutput->m_sOutput.clear();
		CXINFO("mytag", "test {1} {2} {3} {4:'0'.3} {5} {6} {7} {8} {9:'0'.2} {10} {11} {12} {13} "
		       "{14} {15} {16}", 123, 'x', false, 123.456, "abc", (Size)100, -100, "xyz", 12.34f, 1, 
		       2, 3, 4, 5, 6, 7);
		REQUIRE(String("[INF] [    mytag   ]  : test 123 x false 123.456 abc 100 -100 xyz 12.34 1 "
		        "2 3 4 5 6 7\n") == pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXINFOIF(true, "mytag", "test {1} {2} {3} {4:'0'.3} {5} {6} {7} {8} {9:'0'.2} {10} {11} "
		         "{12} {13} {14} {15} {16}", 123, 'x', false, 123.456, "abc", (Size)100, -100, "xyz", 
		         12.34f, 1, 2, 3, 4, 5, 6, 7);
		REQUIRE(String("[INF] [    mytag   ]  : test 123 x false 123.456 abc 100 -100 xyz 12.34 1 "
		        "2 3 4 5 6 7\n") == pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXINFOIF(false, "mytag", "test {1} {2} {3} {4:'0'.3} {5} {6} {7} {8} {9:'0'.2} {10} {11} "
		         "{12} {13} {14} {15} {16}", 123, 'x', false, 123.456, "abc", (Size)100, -100, "xyz", 
		         12.34f, 1, 2, 3, 4, 5, 6, 7);
		REQUIRE(String("") == pOutput->m_sOutput);


		pOutput->m_sOutput.clear();
		CXDBG("mytag", "test {1} {2} {3} {4:'0'.3} {5} {6} {7} {8} {9:'0'.2} {10} {11} {12} {13} "
		      "{14} {15} {16}", 123, 'x', false, 123.456, "abc", (Size)100, -100, "xyz", 12.34f, 1, 
		      2, 3, 4, 5, 6, 7);
		REQUIRE(String("[DBG] [    mytag   ]  : test 123 x false 123.456 abc 100 -100 xyz 12.34 1 "
		        "2 3 4 5 6 7\n") == pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXDBGIF(true, "mytag", "test {1} {2} {3} {4:'0'.3} {5} {6} {7} {8} {9:'0'.2} {10} {11} {12} "
		        "{13} {14} {15} {16}", 123, 'x', false, 123.456, "abc", (Size)100, -100, "xyz", 
		        12.34f, 1, 2, 3, 4, 5, 6, 7);
		REQUIRE(String("[DBG] [    mytag   ]  : test 123 x false 123.456 abc 100 -100 xyz 12.34 1 "
		        "2 3 4 5 6 7\n") == pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXDBGIF(false, "mytag", "test {1} {2} {3} {4:'0'.3} {5} {6} {7} {8} {9:'0'.2} {10} {11} "
		        "{12} {13} {14} {15} {16}", 123, 'x', false, 123.456, "abc", (Size)100, -100, "xyz", 
		        12.34f, 1, 2, 3, 4, 5, 6, 7);
		REQUIRE(String("") == pOutput->m_sOutput);


		pOutput->m_sOutput.clear();
		CXVERB("mytag", "test {1} {2} {3} {4:'0'.3} {5} {6} {7} {8} {9:'0'.2} {10} {11} {12} {13} "
		       "{14} {15} {16}", 123, 'x', false, 123.456, "abc", (Size)100, -100, "xyz", 12.34f, 1, 
		       2, 3, 4, 5, 6, 7);
		REQUIRE(String("[VRB] [    mytag   ]  : test 123 x false 123.456 abc 100 -100 xyz 12.34 1 "
		        "2 3 4 5 6 7\n") == pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXVERBIF(true, "mytag", "test {1} {2} {3} {4:'0'.3} {5} {6} {7} {8} {9:'0'.2} {10} {11} "
		         "{12} {13} {14} {15} {16}", 123, 'x', false, 123.456, "abc", (Size)100, -100, "xyz", 
		         12.34f, 1, 2, 3, 4, 5, 6, 7);
		REQUIRE(String("[VRB] [    mytag   ]  : test 123 x false 123.456 abc 100 -100 xyz 12.34 1 "
		        "2 3 4 5 6 7\n") == pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXVERBIF(false, "mytag", "test {1} {2} {3} {4:'0'.3} {5} {6} {7} {8} {9:'0'.2} {10} {11} "
		         "{12} {13} {14} {15} {16}", 123, 'x', false, 123.456, "abc", (Size)100, -100, "xyz", 
		         12.34f, 1, 2, 3, 4, 5, 6, 7);
		REQUIRE(String("") == pOutput->m_sOutput);

		Log::Logger::GetDefaultLogger().SetLevel(Log::Level_Error);

		pOutput->m_sOutput.clear();
		CXINFO("mytag", "test {1} {2} {3} {4:'0'.3} {5} {6} {7} {8} {9:'0'.2} {10} {11} {12} {13} "
		       "{14} {15} {16}", 123, 'x', false, 123.456, "abc", (Size)100, -100, "xyz", 12.34f, 1, 
		       2, 3, 4, 5, 6, 7);
		REQUIRE(String("") == pOutput->m_sOutput);

		pOutput->m_sOutput.clear();
		CXINFOIF(true, "mytag", "test {1} {2} {3} {4:'0'.3} {5} {6} {7} {8} {9:'0'.2} {10} {11} "
		         "{12} {13} {14} {15} {16}", 123, 'x', false, 123.456, "abc", (Size)100, -100, "xyz", 
		         12.34f, 1, 2, 3, 4, 5, 6, 7);
		REQUIRE(String("") == pOutput->m_sOutput);
	}
}

