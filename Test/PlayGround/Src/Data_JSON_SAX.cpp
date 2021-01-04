/* 
 * CX - C++ framework for general purpose development
 *
 * https://github.com/draede/cx
 * 
 * Copyright (C) 2014 - 2021 draede - draede [at] outlook [dot] com
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

#include "CX/Data/JSON/SAXParser.hpp"
#include "CX/IO/FileInputStream.hpp"
#include "CX/Print.hpp"
#include "Tester.hpp"


using namespace CX;


class MyJSONSAXObserver : public Data::JSON::ISAXParserObserver
{
public:

	virtual Bool OnBeginParse()
	{
		//Print(stdout, "OnBeginParse\n");

		return True;
	}

	virtual Bool OnEndParse()
	{
		//Print(stdout, "OnEndParse\n");

		return True;
	}

	virtual Bool OnBeginObject()
	{
		//Print(stdout, "OnBeginObject\n");

		return True;
	}

	virtual Bool OnEndObject()
	{
		//Print(stdout, "OnEndObject\n");

		return True;
	}

	virtual Bool OnBeginArray()
	{
		//Print(stdout, "OnBeginArray\n");

		return True;
	}

	virtual Bool OnEndArray()
	{
		//Print(stdout, "OnEndArray\n");

		return True;
	}

	virtual Bool OnKey(const Char *pBuffer, Size cLen)
	{
		pBuffer; cLen;
		//String sName(pBuffer, cLen);

		//Print(stdout, "OnKey : [{1}]\n", sName);

		return True;
	}

	virtual Bool OnNullValue()
	{
		//Print(stdout, "OnNullValue\n");

		return True;
	}

	virtual Bool OnBoolValue(Bool bBool)
	{
		bBool;
		//Print(stdout, "OnBoolValue : {1}\n", bBool);

		return True;
	}

	virtual Bool OnIntValue(Int64 nInt)
	{
		nInt;
		//Print(stdout, "OnIntValue : {1}\n", nInt);

		return True;
	}

	virtual Bool OnUIntValue(UInt64 uInt)
	{
		uInt;
		//Print(stdout, "OnUIntValue : {1}\n", uInt);

		return True;
	}

	virtual Bool OnRealValue(Double lfReal)
	{
		lfReal;
		//Print(stdout, "OnRealValue : {1}\n", lfReal);

		return True;
	}

	virtual Bool OnStringValue(const Char *pBuffer, Size cLen)
	{
		pBuffer; cLen;
		//String sValue(pBuffer, cLen);

		//Print(stdout, "OnStringValue : [{1}]\n", sValue);

		return True;
	}

};


struct Stats
{
	Stats()
	{
		cFiles               = 0;
		cOKFiles             = 0;
		cERROpenFiles        = 0;
		cERRAddObserverFiles = 0;
		cERRBeginParseFiles  = 0;
		cERRParseStreamFiles = 0;
		cERREndParseFiles    = 0;
	}

	Size cFiles;
	Size cOKFiles;
	Size cERROpenFiles;
	Size cERRAddObserverFiles;
	Size cERRBeginParseFiles;
	Size cERRParseStreamFiles;
	Size cERREndParseFiles;
};

void Data_JSON_SAX_TestFile(const CX::WChar *wszPath, void *pUsrCtx)
{
	Stats                 *pStats = (Stats *)pUsrCtx;
	IO::FileInputStream   fis(wszPath);
	Data::JSON::SAXParser parser;
	MyJSONSAXObserver     obs;
	Status                status;

	pStats->cFiles++;
	if (!fis.IsOK())
	{
		pStats->cERROpenFiles++;
		Print(stdout, "JSON: failed to open '{1}'\n", wszPath);

		return;
	}
	if ((status = parser.AddObserver(&obs)).IsNOK())
	{
		pStats->cERRAddObserverFiles++;
		Print(stdout, "JSON: failed add observer '{1}'\n", wszPath);

		return;
	}
	if ((status = parser.ParseStream(&fis)).IsNOK())
	{
		pStats->cERRParseStreamFiles++;
		Print(stdout, "JSON: failed to parse '{1}'\n", wszPath);

		return;
	}
	pStats->cOKFiles++;
}


void Data_JSON_SAX_StressTest()
{
	Stats stats;

	Tester::EnumFiles(TESTFILES_ROOT_PATH L"\\JSON", &Data_JSON_SAX_TestFile, &stats);

	Print(stdout, "JSON: Files               : {1}\n", stats.cFiles);
	Print(stdout, "JSON: OKFiles             : {1}\n", stats.cOKFiles);
	Print(stdout, "JSON: ERROpenFiles        : {1}\n", stats.cERROpenFiles);
	Print(stdout, "JSON: ERRAddObserverFiles : {1}\n", stats.cERRAddObserverFiles);
	Print(stdout, "JSON: ERRBeginParseFiles  : {1}\n", stats.cERRBeginParseFiles);
	Print(stdout, "JSON: ERRParseStreamFiles : {1}\n", stats.cERRParseStreamFiles);
	Print(stdout, "JSON: ERREndParseFiles    : {1}\n", stats.cERREndParseFiles);
}

//REGISTER_TEST(Data_JSON_SAX_StressTest);

