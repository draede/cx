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

Status::Status()
{
	m_nCode    = Status_OK;
	m_szMsg[0] = 0;
}

Status::Status(const Status &status)
{
	m_nCode = status.m_nCode;
	memcpy(m_szMsg, status.m_szMsg, MAX_MSG_LEN + 1);
}

Status::Status(StatusCode nCode)
{
	m_nCode    = nCode;
	m_szMsg[0] = 0;
}

Status::Status(StatusCode nCode, const Char *szMsg)
{
	Char       *pDst;
	Char       *pMaxDst;
	const Char *pSrc;

	m_nCode    = nCode;
	m_szMsg[0] = 0;
	pDst       = m_szMsg;
	pMaxDst    = m_szMsg + MAX_MSG_LEN + 1;
	pSrc       = szMsg;

	for (;;)
	{
		*pDst = *pSrc;
		if (0 == *pSrc)
		{
			break;
		}
		pDst++;
		if (pDst >= pMaxDst)
		{
			m_szMsg[MAX_MSG_LEN] = 0;
			break;
		}
		pSrc++;
	}
}

StatusCode Status::GetCode()
{
	return m_nCode;
}

const Char *Status::GetMsg()
{
	return m_szMsg;
}

bool Status::IsOK()
{
	return CXOK(m_nCode);
}

bool Status::IsNOK()
{
	return CXNOK(m_nCode);
}

}//namespace CX

