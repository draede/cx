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

#include "CX/Network/URLParser.hpp"
#include "Tester.hpp"


using namespace CX;


void Network_URLParser_Test_Parse()
{
	String sProtocol;
	String sHost;
	int    nPort;
	String sPath;
	String sQuery;
	Status status;

	status = Network::URLParser::Parse("http://google.com", sProtocol, sHost, nPort, sPath, sQuery);
	status = Network::URLParser::Parse("http://google.com:1000", sProtocol, sHost, nPort, sPath, sQuery);
	status = Network::URLParser::Parse("http://google.com:1000/", sProtocol, sHost, nPort, sPath, sQuery);
	status = Network::URLParser::Parse("http://google.com:1000/test", sProtocol, sHost, nPort, sPath, sQuery);
	status = Network::URLParser::Parse("http://google.com:1000/test?name1=val1&name2=val2", sProtocol, sHost, nPort, sPath, sQuery);
	status = Network::URLParser::Parse("http://google.com/test?name1=val1&name2=val2", sProtocol, sHost, nPort, sPath, sQuery);
	status = Network::URLParser::Parse("http://google.com/?name1=val1&name2=val2", sProtocol, sHost, nPort, sPath, sQuery);
	status = Network::URLParser::Parse("http://google.com/", sProtocol, sHost, nPort, sPath, sQuery);
}

void Network_URLParser_Test_EncodeDecode()
{
	String sIn = "abcdef!*\'x();:aa@&=+$/aa?%#aa[a\n]";
	String sOut1;
	String sOut2;
	bool   bRes;
	Status status;

	status = Network::URLParser::Encode(sIn.c_str(), sOut1);
	status = Network::URLParser::Decode(sOut1.c_str(), sOut2);
	bRes   = (sIn == sOut2);
}

REGISTER_TEST(Network_URLParser_Test_Parse);
REGISTER_TEST(Network_URLParser_Test_EncodeDecode);

