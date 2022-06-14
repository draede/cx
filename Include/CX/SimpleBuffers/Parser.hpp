/* 
 * CX - C++ framework for general purpose development
 *
 * https://github.com/draede/cx
 * 
 * Copyright (C) 2014 - 2022 draede
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


#include "CX/IO/IInputStream.hpp"
#include "CX/SimpleBuffers/Object.hpp"
#include "CX/APIDefs.hpp"


namespace CX
{

namespace SimpleBuffers
{

class CX_API Parser
{
public:

	static Status ParseFilePath(const String &sFilePath, Object &object);

	static Status Parse(IO::IInputStream *pInputStream, Object &object);

	static Status Parse(const String &sBuffer, Object &object);

	static Status Parse(const Char *pBuffer, Size cLen, Object &object);

private:

	typedef Map<String, String>::Type   AliasesMap;

	struct CTX
	{
		const Char *pBuffer;
		Size       cbIndex;
		Size       cbSize;
		Size       cLine;
		Size       cColumn;
	};

	Parser();

	~Parser();

	static Status ParseWhiteSpaces(CTX *pCTX);

	static Status ParseIdentifier(CTX *pCTX, String &sIdentifier);

	static Status ParseDefaultValue(CTX *pCTX, String &sDefault);

	static Status ParsePragma(CTX *pCTX, String &sID, String &sValue);

	static Status ParseAlias(CTX *pCTX, String &sType, String &sAlias);

	static Status ParseMember(CTX *pCTX, Object &object, const AliasesMap &mapAliases);

	static Status ParseConst(CTX *pCTX, Object &object, const AliasesMap &mapAliases);

	static void UpdateType(String &sType, const AliasesMap &mapAliases);

	static void PopulateAliases(AliasesMap &mapAliases);

};

}//namespace SimpleBuffers

}//namespace CX
