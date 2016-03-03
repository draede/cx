/* 
 * CX - C++ framework for general purpose development
 *
 * https://github.com/draede/cx
 * 
 * Copyright (C) 2014-2016 draede, draede [at] outlook [dot] com
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
#include "CX/APIDefs.hpp"
#include "CX/IObject.hpp"
#include "CX/IO/IOutputStream.hpp"
#include "CX/IO/IOutputFilter.hpp"


namespace CX
{

namespace IO
{

class CX_API FilteredOutputStream : public IO::IOutputStream, public IObject
{
public:

	FilteredOutputStream(IOutputFilter *pFilter, IO::IOutputStream *pOutputStream, bool bTakeStreamOwnership = false);

	~FilteredOutputStream();

	virtual Status Write(const void *pBuffer, Size cbReqSize, Size *pcbAckSize);

	virtual Status GetSize(UInt64 *pcbSize) const;

	virtual Bool IsOK() const;

	virtual const Char *GetPath() const;

	Status Flush();

private:

	static const Size BUFFER_SIZE = 65536;

	IOutputFilter          *m_pFilter;
	IO::IOutputStream      *m_pOutputStream;
	bool                   m_bTakeStreamOwnership;
	Byte                   *m_pBuffer;
	Size                   m_cbBufferSize;
	Size                   m_cbBufferOffset;
	UInt64                 m_cbReceivedSize;
	bool                   m_bReady;

	Status Filter(const void *pInput, Size cbInputSize);

};


}//namespace IO

}//namespace CX
