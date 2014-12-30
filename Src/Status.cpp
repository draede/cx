

#pragma once


#include "CX/Status.h"
#include "CX/C/stdio.h"


namespace CX
{

CX_TLS StatusCode	cx_nStatusCode								= Status_OK;
CX_TLS Char			cx_szStatusMsg[Status::MAX_MSG_LEN]	= "";

Status::Status()
{
}

Status::~Status()
{

}

StatusCode Status::Set(StatusCode nStatus)
{
	cx_nStatusCode			= nStatus;
	cx_szStatusMsg[0] 	= 0;

	return nStatus;
}

StatusCode Status::Set(StatusCode nStatus, const Char *szMsg)
{
	Size cLen;

	cx_nStatusCode = nStatus;
	cx_strcopy(cx_szStatusMsg, MAX_MSG_LEN, szMsg, &cLen);

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
	cx_nStatusCode		= Status_OK;
	cx_szStatusMsg[0] 	= 0;
}

bool Status::IsOK()
{
	return (Status_OK == cx_nStatusCode);
}

}//namespace CX

