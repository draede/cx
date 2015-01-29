/* 
 * CX - C++ framework for general purpose developement
 *
 * https://github.com/draede/cx
 * 
 * Copyright (C) 2014 draede - draede [at] outlook [dot] com
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

#include "CX/Status.h"
#include "CX/C/stdio.h"
#include "CX/Detail/DetailPrint/DetailPrint.h"


namespace CX
{

CX_TLS StatusCode   cx_nStatusCode                        = Status_OK;
CX_TLS Char         cx_szStatusMsg[Status::MAX_MSG_LEN]   = "";

Status::Status()
{
}

Status::~Status()
{

}

Char *Status::GetMsgPtr()
{
	return cx_szStatusMsg;
}

Size Status::GetMsgSize()
{
	return MAX_MSG_LEN;
}

StatusCode Status::SetOnlyCode(StatusCode nStatus)
{
	cx_nStatusCode = nStatus;

	return nStatus;
}

StatusCode Status::Set(StatusCode nStatus)
{
	cx_nStatusCode      = nStatus;
	cx_szStatusMsg[0]   = 0;

	return nStatus;
}

StatusCode Status::Set(StatusCode nStatus, const Char *szMsg)
{
	Size cLen;

	cx_nStatusCode = nStatus;
	Detail::DetailPrint::StrCopy(cx_szStatusMsg, MAX_MSG_LEN, szMsg, &cLen);

	return nStatus;
}

StatusCode Status::GetCode()
{
	return cx_nStatusCode;
}

const Char *Status::GetMsg()
{
	return cx_szStatusMsg;
}

void Status::Clear()
{
	cx_nStatusCode     = Status_OK;
	cx_szStatusMsg[0]  = 0;
}

bool Status::IsOK()
{
	return (Status_OK == cx_nStatusCode);
}

bool Status::IsNOK()
{
	return (Status_OK != cx_nStatusCode);
}

}//namespace CX

