/* 
 * CX - C++ framework for general purpose development
 *
 * https://github.com/draede/cx
 * 
 * Copyright (C) 2014 - 2022 draede - draede [at] outlook [dot] com
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

#include "CX/precomp.hpp"
#include "CX/Platform.hpp"


#if defined(CX_OS_WINDOWS)


#include "CX/Sys/Platform/Windows/UUID.hpp"
#include "CX/C/Platform/Windows/windows.h"
#include <Rpc.h>


namespace CX
{

namespace Sys
{

Status UUID::Generate()
{
	String       sGUID;
	RPC_STATUS   nRPCStatus;

	if (RPC_S_OK != (nRPCStatus = UuidCreate((::UUID *)this)))
	{
		return Status(Status_OperationFailed, "UuidCreate failed with error {1}", (long)nRPCStatus);
	}

	return Status();
}

Status UUID::ToString(String *psUUID)
{
	RPC_STATUS   nRPCStatus;
	RPC_CSTR     sRPCUUID;

	if (RPC_S_OK != (nRPCStatus = UuidToStringA((::UUID *)this, &sRPCUUID)))
	{
		return Status(Status_OperationFailed, "UuidToStringA failed with error {1}", (long)nRPCStatus);
	}
	*psUUID = (const Char *)sRPCUUID;
	RpcStringFreeA(&sRPCUUID);

	return Status();
}

Status UUID::FromString(const Char *szUUID)
{
	RPC_STATUS   nRPCStatus;

	if (RPC_S_OK != (nRPCStatus = UuidFromStringA((RPC_CSTR)szUUID, (::UUID *)this)))
	{
		return Status(Status_OperationFailed, "UuidFromStringA failed with error {1}", (long)nRPCStatus);
	}

	return Status();
}

}//namespace Sys

}//namespace CX


#endif

