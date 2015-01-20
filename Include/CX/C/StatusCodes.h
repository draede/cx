
#pragma once


#define CXOK(x)         (CX_Status_OK == (x))
#define CXNOK(x)        (CX_Status_OK != (x))


#ifdef __cplusplus
extern "C" {
#endif


#include "CX/C/Types.h"


static const CX_StatusCode      CX_Status_OK                      = 0;
static const CX_StatusCode      CX_Status_FileNotFound            = 1;
static const CX_StatusCode      CX_Status_FunctionNotFound        = 2;
static const CX_StatusCode      CX_Status_NotFound                = 3;
static const CX_StatusCode      CX_Status_EOF                     = 4;
static const CX_StatusCode      CX_Status_ParseFailed             = 5;
static const CX_StatusCode      CX_Status_TooSmall                = 6;
static const CX_StatusCode      CX_Status_TooBig                  = 7;
static const CX_StatusCode      CX_Status_TooFewItems             = 8;
static const CX_StatusCode      CX_Status_TooManyItems            = 9;
static const CX_StatusCode      CX_Status_OutOfBounds             = 10;
static const CX_StatusCode      CX_Status_WriteFailed             = 11;
static const CX_StatusCode      CX_Status_ReadFailed              = 12;
static const CX_StatusCode      CX_Status_OpenFailed              = 13;
static const CX_StatusCode      CX_Status_CreateFailed            = 14;
static const CX_StatusCode      CX_Status_CloseFailed             = 15;
static const CX_StatusCode      CX_Status_InvalidArg              = 16;
static const CX_StatusCode      CX_Status_InvalidCall             = 17;
static const CX_StatusCode      CX_Status_NotInitialized          = 18;
static const CX_StatusCode      CX_Status_NotSupported            = 19;
static const CX_StatusCode      CX_Status_NotImplemented          = 20;
static const CX_StatusCode      CX_Status_ConversionFailed        = 21;
static const CX_StatusCode      CX_Status_MemAllocFailed          = 22;
static const CX_StatusCode      CX_Status_OperationFailed         = 23;
static const CX_StatusCode      CX_Status_SetPos                  = 24;
static const CX_StatusCode      CX_Status_GetPos                  = 25;
static const CX_StatusCode      CX_Status_SetSize                 = 26;
static const CX_StatusCode      CX_Status_GetSize                 = 27;
static const CX_StatusCode      CX_Status_Busy                    = 28;
static const CX_StatusCode      CX_Status_Cancelled               = 29;
static const CX_StatusCode      CX_Status_Denied                  = 30;


#ifdef __cplusplus
}
#endif

