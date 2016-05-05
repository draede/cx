/* 
 * CX - C++ framework for general purpose development
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

#include "CX/Data/XML/SAXParser.hpp"
#include "CX/IO/FileInputStream.hpp"
#include "CX/Print.hpp"
#include "Tester.hpp"


using namespace CX;


class MyXMLSAXObserver : public Data::XML::ISAXParserObserver
{
public:

	virtual void OnBeginParse()
	{
		//Print(stdout, "OnBeginParse\n");
	}

	virtual void OnEndParse()
	{
		//Print(stdout, "OnEndParse\n");
	}

	virtual void OnStartElement(const Char *szName, const AttrsVector *pVectorAttrs)
	{
		szName; pVectorAttrs;
		/*
		Print(stdout, "OnStartElement: [{1}]", szName);
		for (AttrsVector::const_iterator iter = pVectorAttrs->begin(); iter != pVectorAttrs->end(); ++iter)
		{
			Print(stdout, " [{1}]=[{2}]", iter->sName, iter->sValue);
		}
		Print(stdout, "\n");
		*/
	}

	virtual void OnEndElement(const Char *szName, const Char *szText)
	{
		szName; szText;
		//Print(stdout, "OnEndElement: [{1}] [{2}]\n", szName, szText);
	}

};

struct Stats
{
	Stats()
	{
		cFiles = 0;
		cOKFiles = 0;
		cERROpenFiles = 0;
		cERRAddObserverFiles = 0;
		cERRBeginParseFiles = 0;
		cERRParseStreamFiles = 0;
		cERREndParseFiles = 0;
	}

	Size cFiles;
	Size cOKFiles;
	Size cERROpenFiles;
	Size cERRAddObserverFiles;
	Size cERRBeginParseFiles;
	Size cERRParseStreamFiles;
	Size cERREndParseFiles;
};

void Data_XML_SAX_TestFile(const CX::WChar *wszPath, void *pUsrCtx)
{
	Stats               *pStats = (Stats *)pUsrCtx;
	IO::FileInputStream fis(wszPath);
	Data::XML::SAXParser parser;
	MyXMLSAXObserver     obs;
	Status              status;

	pStats->cFiles++;
	if (!fis.IsOK())
	{
		pStats->cERROpenFiles++;
		Print(stdout, "XML: failed to open '{1}'\n", wszPath);

		return;
	}
	if ((status = parser.AddObserver(&obs)).IsNOK())
	{
		pStats->cERRAddObserverFiles++;
		Print(stdout, "XML: failed add observer '{1}'\n", wszPath);

		return;
	}
	if ((status = parser.BeginParse()).IsNOK())
	{
		pStats->cERRBeginParseFiles++;
		Print(stdout, "XML: failed to begin parsing '{1}'\n", wszPath);

		return;
	}
	if ((status = parser.ParseStream(&fis)).IsNOK())
	{
		pStats->cERRParseStreamFiles++;
		Print(stdout, "XML: failed to parse '{1}'\n", wszPath);

		return;
	}
	if ((status = parser.EndParse()).IsNOK())
	{
		pStats->cERREndParseFiles++;
		Print(stdout, "XML: failed to end parsing '{1}'\n", wszPath);

		return;
	}
	pStats->cOKFiles++;
}

void Data_XML_SAX_StressTest()
{
	Stats stats;

	Tester::EnumFiles(TESTFILES_ROOT_PATH L"\\XML", &Data_XML_SAX_TestFile, &stats);

	Print(stdout, "XML: Files               : {1}\n", stats.cFiles);
	Print(stdout, "XML: OKFiles             : {1}\n", stats.cOKFiles);
	Print(stdout, "XML: ERROpenFiles        : {1}\n", stats.cERROpenFiles);
	Print(stdout, "XML: ERRAddObserverFiles : {1}\n", stats.cERRAddObserverFiles);
	Print(stdout, "XML: ERRBeginParseFiles  : {1}\n", stats.cERRBeginParseFiles);
	Print(stdout, "XML: ERRParseStreamFiles : {1}\n", stats.cERRParseStreamFiles);
	Print(stdout, "XML: ERREndParseFiles    : {1}\n", stats.cERREndParseFiles);
}

REGISTER_TEST(Data_XML_SAX_StressTest);

