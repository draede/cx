/* 
 * CX - C++ framework for general purpose development
 *
 * https://github.com/draede/cx
 * 
 * Copyright (C) 2014 - 2019 draede - draede [at] outlook [dot] com
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
#include "CX/String.hpp"
#include "CX/Stack.hpp"
#include "CX/APIDefs.hpp"
#include "CX/IO/Platform/Windows/MemoryMappedFile.hpp"
#include "CX/Data/NPY/Format.hpp"
#include "CX/Data/NPY/ByteOrder.hpp"
#include "CX/Data/NPY/Type.hpp"
#include "CX/Data/NPY/Column.hpp"
#include "CX/Data/NPY/Version.hpp"
#include "CX/Data/NPY/Header.hpp"
#include "CX/Data/NPY/SAXDictionaryParser.hpp"


namespace CX
{

namespace Data
{

namespace NPY
{

class CX_API Reader
{
public:

	Reader();

	~Reader();

	Status Open(const Char *szPath);

	Status Open(const WChar *wszPath);

	Status Close();

	const void *GetRows(Size cStartRowIndex, Size cReqRowsCount, Size *pcAckRowsCount);

	Bool IsOK() const;

	const Char *GetPathA() const;

	const WChar *GetPathW() const;

	const Size GetRowsCount() const;

	const Size GetColumnsCount() const;

	const Column *GetColumns() const;

	Size GetRowSize() const;

	Format GetFormat() const;

	Version GetVersion() const;

private:

	static const UInt64    INITIAL_MAP_SIZE = 32768;

	class SAXDictionaryObserver : public ISAXDictionaryParserObserver
	{
	public:

		typedef Vector<UInt64>::Type   ShapeVector;

		Column::Vector   m_vectorColumns;
		Column           m_all;
		Format           m_nFormat;
		Size             m_cRows;
		Size             m_cColumns;
		Bool             m_bShapeHasDefault;
		ShapeVector      m_vectorShape;
		Size             m_cbRowSize;

		virtual Status OnBeginParse();

		virtual Status OnEndParse();

		virtual Status OnBeginObject();

		virtual Status OnEndObject();

		virtual Status OnBeginArray();

		virtual Status OnEndArray();

		virtual Status OnBeginTuple();

		virtual Status OnEndTuple();

		virtual Status OnKey(const Char *pBuffer, Size cLen);

		virtual Status OnBool(Bool bBool);

		virtual Status OnInt(Int64 nInt);

		virtual Status OnUInt(UInt64 uUInt);

		virtual Status OnReal(Double lfReal);

		virtual Status OnString(const Char *pBuffer, Size cLen);

		virtual Status OnTupleDefault();

	private:

		struct Node
		{
			typedef Stack<Node>::Type   Stack;

			enum Type
			{
				Type_Object,
				Type_Array,
				Type_Tuple,

				Type_Named,
			};

			Type     nType;
			String   sKey;
			Size     cItems;
		};

		Node::Stack   m_stackNodes;

		Status ParseColumnDesc(Column *pColumn, const Char *pBuffer, Size cLen);

	};

#pragma warning(push)
#pragma warning(disable: 4251)
	String                 m_sPath;
	WString                m_wsPath;
	Column::Vector         m_vectorColumns;
#pragma warning(pop)

	IO::MemoryMappedFile   m_mmf;
	Format                 m_nFormat;
	Version                m_nVersion;
	UInt64                 m_cbFirstRowOffset;
	Size                   m_cRows;
	Size                   m_cbRowSize;
	UInt32                 m_cbAllocationGranularity;

	Status Open();

	Status ParseDictionary(const void *pData, Size cbSize);

};

}//namespace NPY

}//namespace Data

}//namespace CX
