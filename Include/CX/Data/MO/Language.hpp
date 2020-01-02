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

#pragma once


#include "CX/IO/IInputStream.hpp"
#include "CX/Data/MO/SAXParser.hpp"
#include "CX/IO/IInputStream.hpp"
#include "CX/Map.hpp"
#include "CX/String.hpp"
#include "CX/Status.hpp"
#include "CX/APIDefs.hpp"


namespace CX
{

namespace Data
{

namespace MO
{

class CX_API Language
{
public:

	Language();

	Language(const String &sID);

	Language(const Language &lang);

	~Language();

	Status Copy(const Language &lang);

	Status SetID(const String &sID);

	const String &GetID() const;
	Size GetStringsCount() const;

	const String &GetString(Size cIndex) const;

	const String &GetTranslation(Size cIndex) const;

	const String &GetTranslation(const String &sString) const;

	Status LoadFromStream(const Char *szPath);

	Status LoadFromStream(IO::IInputStream *pInputStream);

private:

	typedef Map<String, String>::Type   StringsMap;

	String     m_sID;
	StringsMap m_mapStrings;

	class SAXObserver : public ISAXParserObserver
	{
	public:

		StringsMap *m_pMapStrings;

		virtual void OnBeginParse();

		virtual void OnEndParse();

		virtual void OnString(const Char *szOrig, const Char *szTranslated);

	};

};

}//namespace MO

}//namespace Data

}//namespace CX
