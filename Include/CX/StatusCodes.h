
#pragma once


#include "CX/Types.h"


#define CXOK(x)         (CX::Status_OK == (x))
#define CXNOK(x)        (CX::Status_OK != (x))


namespace CX
{

static const StatusCode      Status_OK                      = 0;
static const StatusCode      Status_FileNotFound            = 1;
static const StatusCode      Status_FunctionNotFound        = 2;
static const StatusCode      Status_NotFound                = 3;
static const StatusCode      Status_EOF                     = 4;
static const StatusCode      Status_ParseFailed             = 5;
static const StatusCode      Status_TooSmall                = 6;
static const StatusCode      Status_TooBig                  = 7;
static const StatusCode      Status_TooFewItems             = 8;
static const StatusCode      Status_TooManyItems            = 9;
static const StatusCode      Status_OutOfBounds             = 10;
static const StatusCode      Status_WriteFailed             = 11;
static const StatusCode      Status_ReadFailed              = 12;
static const StatusCode      Status_OpenFailed              = 13;
static const StatusCode      Status_CreateFailed            = 14;
static const StatusCode      Status_CloseFailed             = 15;
static const StatusCode      Status_InvalidArg              = 16;
static const StatusCode      Status_InvalidCall             = 17;
static const StatusCode      Status_NotInitialized          = 18;
static const StatusCode      Status_NotSupported            = 19;
static const StatusCode      Status_NotImplemented          = 20;
static const StatusCode      Status_ConversionFailed        = 21;
static const StatusCode      Status_MemAllocFailed          = 22;
static const StatusCode      Status_OperationFailed         = 23;

}//CX_API 

