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
#include "CX/String.hpp"
#include "CX/Vector.hpp"


namespace CX
{

namespace Data
{

namespace NPY
{

class CX_API Tools
{
public:

	typedef Vector<String>::Type    PathsVector;
	typedef Vector<WString>::Type   WPathsVector;

	static Status Merge(const Char **paths, Size cPaths);

	static Status Merge(const WChar **paths, Size cPaths);

	static Status Merge(const PathsVector &vectorPaths);

	static Status Merge(const WPathsVector &vectorPaths);

	static Status Truncate(const Char *szPath, Size cRows);

	static Status Truncate(const WChar *wszPath, Size cRows);

private:

	static const Size   MAX_READ_BUFFER_SIZE = 2ULL * 1024ULL * 1024ULL * 1024ULL;
	static const Size   MAX_HEADER_SIZE      = 1024 * 1024;

	Tools();

	~Tools();

	static Status Truncate(void *pFile, Size cRows);

};

}//namespace NPY

}//namespace Data

}//namespace CX
