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


#include "CX/Types.hpp"
#include "CX/Status.hpp"
#include "CX/IO/IOutputStream.hpp"
#include "CX/Data/CSV/ISAXParserObserver.hpp"
#include "CX/Data/NPY/Column.hpp"
#include "CX/Data/NPY/Reader.hpp"
#include "CX/Data/NPY/Writer.hpp"
#include "CX/APIDefs.hpp"


namespace CX
{

namespace Data
{

namespace NPY
{

class CX_API Converter
{
public:

	static const Size   NPY_INPUT_BUFFER_ROWS_COUNT = 65536;
	static const Size   CSV_OUTPUT_BUFFER_SIZE      = 4 * 1024 * 1024;

	static Status CSV2NPY(const Char *szCSVPath, const Char *szNPYPath, const Column *columns, Size cColumns, 
	                      Bool bSkipFirstRow = True);

	static Status CSV2NPY(const WChar *wszCSVPath, const WChar *wszNPYPath, const Column *columns, Size cColumns, 
	                      Bool bSkipFirstRow = True);

	static Status CSV2NPY(const Char *szCSVPath, Writer *pWriter, Bool bSkipFirstRow = True);

	static Status CSV2NPY(const WChar *wszCSVPath, Writer *pWriter, Bool bSkipFirstRow = True);

	static Status NPY2CSV(const Char *szNPYPath, const Char *szCSVPath, 
	                      Size cNPYInputBufferRowsCount = NPY_INPUT_BUFFER_ROWS_COUNT, 
	                      Size cbCSVBufferOutputSize = CSV_OUTPUT_BUFFER_SIZE);

	static Status NPY2CSV(const WChar *wszNPYPath, const WChar *wszCSVPath, 
	                      Size cNPYInputBufferRowsCount = NPY_INPUT_BUFFER_ROWS_COUNT, 
	                      Size cbCSVBufferOutputSize = CSV_OUTPUT_BUFFER_SIZE);

	static Status NPY2CSV(Reader *pReader, IO::IOutputStream *pOutputStream, 
	                      Size cNPYInputBufferRowsCount = NPY_INPUT_BUFFER_ROWS_COUNT, 
	                      Size cbCSVBufferOutputSize = CSV_OUTPUT_BUFFER_SIZE);

protected:

	class SAXCSVObserver : public CSV::ISAXParserObserver
	{
	public:

		Writer   *m_pWriter;
		Bool     m_bSkipFirstRow;

		virtual void OnBeginParse();

		virtual void OnEndParse();

		virtual Bool OnRow(Size cRowIndex, const ColumnsVector &vectorColumns);

	private:

	};

	Converter();

	~Converter();

};

}//namespace NPY

}//namespace Data

}//namespace CX 
