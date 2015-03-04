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
#include "CX/APIDefs.hpp"


namespace CX
{

namespace Data
{

namespace BINJSON
{

/*

HEADER:

MAGIC    = 8 bytes
ENCODING = 4 bytes (0 for now; 0 = no encoding)

FOOTER:
UINT32   = 4 bytes (xxHash of data - header is not used)

ENTRY:

NULL, BOOL, ENDROOTOBJECT, ENDROOTARRAY, ENDOBJECT, ENDARRAY:
UINT8  = entry type - see Spec::TYPE *

INT, REAL:
UINT32 = name size (for childs of objects)
?      = name (for childs of objects)
?......= data

STRING, BLOB:
UINT32 = name size (for childs of objects)
?      = name (for childs of objects)
UINT32 = data size
?......= data

BEGINROOTOBJECT, BEGINROOTARRAY, BEGINOBJECT, BEGINARRAY:
UINT16 = name size (for childs of objects)
?      = name (for childs of objects)

*/

class CX_API Spec
{
public:

	static const UInt64 MAGIC = 0x314E4F534A4E4942; //BINJSON1

	static const UInt32 ENCODING_NONE = 0x0; //no encoding

	static const Byte TYPE_BEGINROOTOBJECT = '<';
	static const Byte TYPE_ENDROOTOBJECT   = '>';
	static const Byte TYPE_BEGINROOTARRAY  = '(';
	static const Byte TYPE_ENDROOTARRAY    = ')';
	static const Byte TYPE_BEGINOBJECT     = '{';
	static const Byte TYPE_ENDOBJECT       = '}';
	static const Byte TYPE_BEGINARRAY      = '[';
	static const Byte TYPE_ENDARRAY        = ']';
	static const Byte TYPE_NULL            = 'N';
	static const Byte TYPE_FALSE           = 'F';
	static const Byte TYPE_TRUE            = 'T';
	static const Byte TYPE_INT             = 'I';
	static const Byte TYPE_REAL            = 'R';
	static const Byte TYPE_STRING          = 'S';
	static const Byte TYPE_BLOB            = 'O';

};

}//namespace BINJSON

}//namespace Data

}//namespace CX

