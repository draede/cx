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


#include "CX/Types.hpp"
#include "CX/Status.hpp"
#include "CX/Set.hpp"
#include "CX/IO/IInputStream.hpp"
#include "CX/SB/Object.hpp"
#include "CX/APIDefs.hpp"
#include "CX/IObject.hpp"


namespace CX
{

namespace SB
{

class CX_API Parser : public IObject
{
public:

	typedef Set<String>::Type   ImportsSet;
	typedef Set<String>::Type   ExternalsSet;

	typedef struct _Data
	{
		ObjectsMap   mapObjects;
		ImportsSet   setImports;
		ExternalsSet setExternals;
	}Data;

	Parser();

	Status Parse(const Char *szFilePath, Data &data);

	Status Parse(IO::IInputStream *pInputStream, Data &data);

private:

	static const Size  BUFFER_SIZE = 8192;

	static const Size  MAX_IMPORTS_DEPTH = 16;

	static const Char    *RESERVED_KEYWORDS[];
	static const CX_Size RESERVED_KEYWORDS_COUNT;

	enum WSType
	{
		WSType_Normal,
		WSType_SLComment,
		WSType_MLComment
	};

	typedef Set<String>::Type   KeywordsSet;

	IO::IInputStream *m_pInputStream;
#pragma warning(push)
#pragma warning(disable: 4251)
	String           m_sPath;
	KeywordsSet      m_setKeywords;
#pragma warning(pop)
	Byte             m_buffer[BUFFER_SIZE];
	Size             m_cbSize;
	Size             m_cbOffset;
	Size             m_cLine;
	Size             m_cColumn;

	Status ReadMore();

	bool IsEOF();

	Byte Get();

	void Next();

	//===

	Status Parse(Data &data);

	Status ParseObject(Object &obj);

	Status ParseMember(Member &member);

	Status ParseImport(String &sPath);

	Status ParseExternal(String &sType);

	Status ParseMemberName(String &sName, String &sAlias);

	Status ParseScalarType(String &sType);

	Status ParseIdentifier(String &sIdentifier);

	Status ParseWhiteSpace();

};

}//namespace SB

}//namespace CX

