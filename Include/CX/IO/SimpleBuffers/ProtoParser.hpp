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
#include "CX/String.hpp"
#include "CX/Vector.hpp"
#include "CX/IO/IInputStream.hpp"
#include "CX/IO/SimpleBuffers/Object.hpp"
#include "CX/Status.hpp"
#include "CX/APIDefs.hpp"


namespace CX
{

namespace IO
{

namespace SimpleBuffers
{

class CX_API ProtoParser
{
public:

	ProtoParser();

	Status BeginParse();

	Status EndParse();

	Status Parse(const Char *szFilePath);

	Status Parse(IO::IInputStream *pInputStream);

	Status Parse(const void *pBuffer, Size cbSize);

	void ClearObjects()
	{
		m_mapObjects.clear();
	}

	const ObjectsMap &GetObjects()
	{
		return m_mapObjects;
	}

private:

	static const Size  MAX_IMPORTS_DEPTH = 16;

	static const Char    *RESERVED_KEYWORDS[];
	static const CX_Size RESERVED_KEYWORDS_COUNT;

	enum State
	{
		State_None,
		State_BeforeDeclaration,
		State_InDeclaration,
		State_BeforeObjectName,
		State_InObjectName,
		State_AfterObjectName,
		State_BeforeObjectStart,
		State_BeforeName,
		State_InName,
		State_AfterName,
		State_BeforeColon,
		State_BeforeSemicolon,
		State_BeforeType,
		State_InType,
		State_AfterType,
		State_BeforeArrayBegin,
		State_BeforeArrayEnd,
		State_AfterObject,
		State_BeforeImport,
		State_InImport,
		State_AfterImport,
		State_BeforeComment,
		State_InSLComment,
		State_InMLComment,
		State_BeforeMLCommentEnd,
	};

	Size m_cLine;
	Size m_cColumn;

	State      m_nState;
	State      m_nSavedState;
	const Char *m_pPos;
	const Char *m_pEnd;
	bool       m_bIsArray;
	Object     m_object;
	Size       m_cImportDepth;
#pragma warning(push)
#pragma warning(disable: 4251)
	Field      m_field;
	String     m_sPath;
	String     m_sToken;
	ObjectsMap m_mapObjects;
#pragma warning(pop)

	Status WalkWhiteSpaces();

	Status WalkToken();

	bool IsReservedKeyword(const Char *szStr);

	void GetFilePath(const Char *szRefPath, const Char *szOrigPath, String *psPath);

	Status BeginParseInternal(Size cImportDepth);

};

}//namespace SimpleBuffers

}//namespace IO

}//namespace CX

