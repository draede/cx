

#pragma once


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

