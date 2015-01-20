
#pragma once


#include "CX/C/StatusCode.h"


namespace CX
{

static const StatusCode      Status_OK                      = CX_Status_OK;
static const StatusCode      Status_FileNotFound            = CX_Status_FileNotFound;
static const StatusCode      Status_FunctionNotFound        = CX_Status_FunctionNotFound;
static const StatusCode      Status_NotFound                = CX_Status_NotFound;
static const StatusCode      Status_EOF                     = CX_Status_EOF;
static const StatusCode      Status_ParseFailed             = CX_Status_ParseFailed;
static const StatusCode      Status_TooSmall                = CX_Status_TooSmall;
static const StatusCode      Status_TooBig                  = CX_Status_TooBig;
static const StatusCode      Status_TooFewItems             = CX_Status_TooFewItems;
static const StatusCode      Status_TooManyItems            = CX_Status_TooManyItems;
static const StatusCode      Status_OutOfBounds             = CX_Status_OutOfBounds;
static const StatusCode      Status_WriteFailed             = CX_Status_WriteFailed;
static const StatusCode      Status_ReadFailed              = CX_Status_ReadFailed;
static const StatusCode      Status_OpenFailed              = CX_Status_OpenFailed;
static const StatusCode      Status_CreateFailed            = CX_Status_CreateFailed;
static const StatusCode      Status_CloseFailed             = CX_Status_CloseFailed;
static const StatusCode      Status_InvalidArg              = CX_Status_InvalidArg;
static const StatusCode      Status_InvalidCall             = CX_Status_InvalidCall;
static const StatusCode      Status_NotInitialized          = CX_Status_NotInitialized;
static const StatusCode      Status_NotSupported            = CX_Status_NotSupported;
static const StatusCode      Status_NotImplemented          = CX_Status_NotImplemented;
static const StatusCode      Status_ConversionFailed        = CX_Status_ConversionFailed;
static const StatusCode      Status_MemAllocFailed          = CX_Status_MemAllocFailed;
static const StatusCode      Status_OperationFailed         = CX_Status_OperationFailed;
static const StatusCode      Status_SetPos                  = CX_Status_SetPos;
static const StatusCode      Status_GetPos                  = CX_Status_GetPos;
static const StatusCode      Status_SetSize                 = CX_Status_SetSize;
static const StatusCode      Status_GetSize                 = CX_Status_GetSize;
static const StatusCode      Status_Busy                    = CX_Status_Busy;
static const StatusCode      Status_Cancelled               = CX_Status_Cancelled;
static const StatusCode      Status_Denied                  = CX_Status_Denied;

}//CX_API 

