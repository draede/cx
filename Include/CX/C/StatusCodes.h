/* 
 * CX - C++ framework for general purpose development
 *
 * https://github.com/draede/cx
 * 
 * Copyright (C) 2014 - 2022 draede
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
static const CX_StatusCode      CX_Status_NoMoreItems             = 31;
static const CX_StatusCode      CX_Status_NoMoreData              = 32;
static const CX_StatusCode      CX_Status_Duplicate               = 33;


#ifdef __cplusplus
}
#endif

