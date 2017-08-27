/* 
 * CX - C++ framework for general purpose development
 *
 * https://github.com/draede/cx
 * 
 * Copyright (C) 2014 - 2017 draede - draede [at] outlook [dot] com
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

#include "CX/C/Types.h"
#include "CX/C/StatusCodes.h"
#include "CX/CBuffers/Buffer.h"
#include "CX/CBuffers/CLibEnvironment.h"
#include "My_Namespace_Struct1.h"
#include "My_Namespace_Struct2.h"
#include "CX/Print.hpp"
#include "Tester.hpp"


using namespace CX;


#define CHECK(s)             if (CXNOK((s)))             { CX::Print(stdout, "Error at line {1}\n", __LINE__); }
#define VERIFY(v1, v2)       if ((v1) != (v2))           { CX::Print(stdout, "Fail at line {1}\n", __LINE__); }
#define VERIFYPTR(v1, v2, c) if (0 != memcmp(v1, v2, c)) { CX::Print(stdout, "Fail at line {1}\n", __LINE__); }

static CX_Bool       bool_            = CX_True;
static CX_Int8       int8             = 2;
static CX_UInt8      uint8            = 3;
static CX_Int16      int16            = 4;
static CX_UInt16     uint16           = 5;
static CX_Int32      int32            = 6;
static CX_UInt32     uint32           = 7;
static CX_Int64      int64            = 8;
static CX_UInt64     uint64           = 9;
static CX_Float      float_           = 10.0f;
static CX_Double     double_          = 11.0;
static CX_Char       char_            = 'a';
static CX_WChar      wchar            = L'a';
static CX::String    string           = "abc";
static CX::WString   wstring          = L"abcxyz";
static CX::String    blob             = "0123456789";

static CX_Bool       arr_st_bool[]    = { CX_False };
static CX_Int8       arr_st_int8[]    = { 1, 2 };
static CX_UInt8      arr_st_uint8[]   = { 10, 9, 8 };
static CX_Int16      arr_st_int16[]   = { 1, 2, 3, 4 };
static CX_UInt16     arr_st_uint16[]  = { 100, 10, 9, 8, 7 };
static CX_Int32      arr_st_int32[]   = { 1, 500, 2, 3, 4, 11 };
static CX_UInt32     arr_st_uint32[]  = { 10, 400, 300, 9, 8, 7, 6 };
static CX_Int64      arr_st_int64[]   = { 1, 2, 3, 4, 423, 2351235, 125, 0 };
static CX_UInt64     arr_st_uint64[]  = { 10, 9, 8, 1, 1, 3, 555, 7, 6 };
static CX_Float      arr_st_float[]   = { 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f, 10.0f };
static CX_Double     arr_st_double[]  = { 10.0, 20.0, 30.0, 40.0, 50.0, 60.0, 70.0, 80.0, 90.0, 100.0, 110.0 };
static CX_Char       arr_st_char[]    = { 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l' };
static CX_WChar      arr_st_wchar[]   = { L'A', L'B', L'C', L'D', L'E', L'F', L'G', L'H', L'I', L'J', L'K', L'L', L'M' };

static CX_Bool       arr_bool[]       = { CX_False, CX_True };
static CX_Int8       arr_int8[]       = { 1, 2, 3, 4 };
static CX_UInt8      arr_uint8[]      = { 10, 9, 8, 7, 6, 5 };
static CX_Int16      arr_int16[]      = { 1, 2, 3, 4 };
static CX_UInt16     arr_uint16[]     = { 100, 10, 9, 8, 7, 6, 5 };
static CX_Int32      arr_int32[]      = { 1, 500, 2, 3, 4 };
static CX_UInt32     arr_uint32[]     = { 10, 400, 300, 9, 8, 7, 6, 5 };
static CX_Int64      arr_int64[]      = { 1, 2, 3, 4, 423, 2351235, 125 };
static CX_UInt64     arr_uint64[]     = { 10, 9, 8, 1, 1, 3, 555, 7, 6, 5 };
static CX_Float      arr_float[]      = { 1.0f, 2.0f, 3.0f };
static CX_Double     arr_double[]     = { 100.0, 200.0, 300.0, 400.0 };
static CX_Char       arr_char[]       = { 'a', 'b', 'c' };
static CX_WChar      arr_wchar[]      = { L'w', L'x', L'y', L'z' };


static void Setup1(My_Namespace_Struct1 *pStruct1)
{
	CX_CB_Size      cCount;
	CX_Bool         *pBoolPtr;
	CX_Int8         *pInt8Ptr;
	CX_UInt8        *pUInt8Ptr;
	CX_Int16        *pInt16Ptr;
	CX_UInt16       *pUInt16Ptr;
	CX_Int32        *pInt32Ptr;
	CX_UInt32       *pUInt32Ptr;
	CX_Int64        *pInt64Ptr;
	CX_UInt64       *pUInt64Ptr;
	CX_Float        *pFloatPtr;
	CX_Double       *pDoublePtr;
	CX_Char         *pCharPtr;
	CX_WChar        *pWCharPtr;
	CX_StatusCode   nStatus;

	CHECK(nStatus = pStruct1->SetMyBool(pStruct1, bool_));
	CHECK(nStatus = pStruct1->SetMyInt8(pStruct1, int8));
	CHECK(nStatus = pStruct1->SetMyUInt8(pStruct1, uint8));
	CHECK(nStatus = pStruct1->SetMyInt16(pStruct1, int16));
	CHECK(nStatus = pStruct1->SetMyUInt16(pStruct1, uint16));
	CHECK(nStatus = pStruct1->SetMyInt32(pStruct1, int32));
	CHECK(nStatus = pStruct1->SetMyUInt32(pStruct1, uint32));
	CHECK(nStatus = pStruct1->SetMyInt64(pStruct1, int64));
	CHECK(nStatus = pStruct1->SetMyUInt64(pStruct1, uint64));
	CHECK(nStatus = pStruct1->SetMyFloat(pStruct1, float_));
	CHECK(nStatus = pStruct1->SetMyDouble(pStruct1, double_));
	CHECK(nStatus = pStruct1->SetMyChar(pStruct1, char_));
	CHECK(nStatus = pStruct1->SetMyWChar(pStruct1, wchar));

	cCount   = pStruct1->GetMyBoolStaticArrayCount(pStruct1);
	pBoolPtr = pStruct1->GetMyBoolStaticArray(pStruct1);
	VERIFY(cCount, sizeof(arr_st_bool) / sizeof(arr_st_bool[0]));
	for (CX_CB_Size i = 0; i < cCount; i++)
	{
		pBoolPtr[i] = arr_st_bool[i];
	}

	cCount   = pStruct1->GetMyInt8StaticArrayCount(pStruct1);
	pInt8Ptr = pStruct1->GetMyInt8StaticArray(pStruct1);
	VERIFY(cCount, sizeof(arr_st_int8) / sizeof(arr_st_int8[0]));
	memcpy(pInt8Ptr, arr_st_int8, sizeof(arr_st_int8));

	cCount    = pStruct1->GetMyUInt8StaticArrayCount(pStruct1);
	pUInt8Ptr = pStruct1->GetMyUInt8StaticArray(pStruct1);
	VERIFY(cCount, sizeof(arr_st_uint8) / sizeof(arr_st_uint8[0]));
	for (CX_CB_Size i = 0; i < cCount; i++)
	{
		pUInt8Ptr[i] = arr_st_uint8[i];
	}

	cCount    = pStruct1->GetMyInt16StaticArrayCount(pStruct1);
	pInt16Ptr = pStruct1->GetMyInt16StaticArray(pStruct1);
	VERIFY(cCount, sizeof(arr_st_int16) / sizeof(arr_st_int16[0]));
	memcpy(pInt16Ptr, arr_st_int16, sizeof(arr_st_int16));

	cCount     = pStruct1->GetMyUInt16StaticArrayCount(pStruct1);
	pUInt16Ptr = pStruct1->GetMyUInt16StaticArray(pStruct1);
	VERIFY(cCount, sizeof(arr_st_uint16) / sizeof(arr_st_uint16[0]));
	for (CX_CB_Size i = 0; i < cCount; i++)
	{
		pUInt16Ptr[i] = arr_st_uint16[i];
	}

	cCount    = pStruct1->GetMyInt32StaticArrayCount(pStruct1);
	pInt32Ptr = pStruct1->GetMyInt32StaticArray(pStruct1);
	VERIFY(cCount, sizeof(arr_st_int32) / sizeof(arr_st_int32[0]));
	memcpy(pInt32Ptr, arr_st_int32, sizeof(arr_st_int32));

	cCount     = pStruct1->GetMyUInt32StaticArrayCount(pStruct1);
	pUInt32Ptr = pStruct1->GetMyUInt32StaticArray(pStruct1);
	VERIFY(cCount, sizeof(arr_st_uint32) / sizeof(arr_st_uint32[0]));
	for (CX_CB_Size i = 0; i < cCount; i++)
	{
		pUInt32Ptr[i] = arr_st_uint32[i];
	}

	cCount    = pStruct1->GetMyInt64StaticArrayCount(pStruct1);
	pInt64Ptr = pStruct1->GetMyInt64StaticArray(pStruct1);
	VERIFY(cCount, sizeof(arr_st_int64) / sizeof(arr_st_int64[0]));
	memcpy(pInt64Ptr, arr_st_int64, sizeof(arr_st_int64));

	cCount     = pStruct1->GetMyUInt64StaticArrayCount(pStruct1);
	pUInt64Ptr = pStruct1->GetMyUInt64StaticArray(pStruct1);
	VERIFY(cCount, sizeof(arr_st_uint64) / sizeof(arr_st_uint64[0]));
	for (CX_CB_Size i = 0; i < cCount; i++)
	{
		pUInt64Ptr[i] = arr_st_uint64[i];
	}

	cCount    = pStruct1->GetMyFloatStaticArrayCount(pStruct1);
	pFloatPtr = pStruct1->GetMyFloatStaticArray(pStruct1);
	VERIFY(cCount, sizeof(arr_st_float) / sizeof(arr_st_float[0]));
	memcpy(pFloatPtr, arr_st_float, sizeof(arr_st_float));

	cCount     = pStruct1->GetMyDoubleStaticArrayCount(pStruct1);
	pDoublePtr = pStruct1->GetMyDoubleStaticArray(pStruct1);
	VERIFY(cCount, sizeof(arr_st_double) / sizeof(arr_st_double[0]));
	for (CX_CB_Size i = 0; i < cCount; i++)
	{
		pDoublePtr[i] = arr_st_double[i];
	}

	cCount   = pStruct1->GetMyCharStaticArrayCount(pStruct1);
	pCharPtr = pStruct1->GetMyCharStaticArray(pStruct1);
	VERIFY(cCount, sizeof(arr_st_char) / sizeof(arr_st_char[0]));
	memcpy(pCharPtr, arr_st_char, sizeof(arr_st_char));

	cCount    = pStruct1->GetMyWCharStaticArrayCount(pStruct1);
	pWCharPtr = pStruct1->GetMyWCharStaticArray(pStruct1);
	VERIFY(cCount, sizeof(arr_st_wchar) / sizeof(arr_st_wchar[0]));
	for (CX_CB_Size i = 0; i < cCount; i++)
	{
		pWCharPtr[i] = arr_st_wchar[i];
	}

	cCount = sizeof(arr_bool) / sizeof(arr_bool[0]);
	CHECK(nStatus = pStruct1->SetMyBoolArrayCount(pStruct1, cCount));
	pBoolPtr = pStruct1->GetMyBoolArray(pStruct1);
	cCount = pStruct1->GetMyBoolArrayCount(pStruct1);
	VERIFY(cCount, sizeof(arr_bool) / sizeof(arr_bool[0]));
	for (CX_CB_Size i = 0; i < cCount; i++)
	{
		pBoolPtr[i] = arr_bool[i];
	}

	cCount = sizeof(arr_int8) / sizeof(arr_int8[0]);
	CHECK(nStatus = pStruct1->SetMyInt8ArrayCount(pStruct1, cCount));
	pInt8Ptr = pStruct1->GetMyInt8Array(pStruct1);
	cCount = pStruct1->GetMyInt8ArrayCount(pStruct1);
	VERIFY(cCount, sizeof(arr_int8) / sizeof(arr_int8[0]));
	memcpy(pInt8Ptr, arr_int8, sizeof(arr_int8));

	cCount = sizeof(arr_uint8) / sizeof(arr_uint8[0]);
	CHECK(nStatus = pStruct1->SetMyUInt8ArrayCount(pStruct1, cCount));
	pUInt8Ptr = pStruct1->GetMyUInt8Array(pStruct1);
	cCount = pStruct1->GetMyUInt8ArrayCount(pStruct1);
	VERIFY(cCount, sizeof(arr_uint8) / sizeof(arr_uint8[0]));
	for (CX_CB_Size i = 0; i < cCount; i++)
	{
		pUInt8Ptr[i] = arr_uint8[i];
	}

	cCount = sizeof(arr_int16) / sizeof(arr_int16[0]);
	CHECK(nStatus = pStruct1->SetMyInt16ArrayCount(pStruct1, cCount));
	pInt16Ptr = pStruct1->GetMyInt16Array(pStruct1);
	cCount = pStruct1->GetMyInt16ArrayCount(pStruct1);
	VERIFY(cCount, sizeof(arr_int16) / sizeof(arr_int16[0]));
	memcpy(pInt16Ptr, arr_int16, sizeof(arr_int16));

	cCount = sizeof(arr_uint16) / sizeof(arr_uint16[0]);
	CHECK(nStatus = pStruct1->SetMyUInt16ArrayCount(pStruct1, cCount));
	pUInt16Ptr = pStruct1->GetMyUInt16Array(pStruct1);
	cCount = pStruct1->GetMyUInt16ArrayCount(pStruct1);
	VERIFY(cCount, sizeof(arr_uint16) / sizeof(arr_uint16[0]));
	for (CX_CB_Size i = 0; i < cCount; i++)
	{
		pUInt16Ptr[i] = arr_uint16[i];
	}

	cCount = sizeof(arr_int32) / sizeof(arr_int32[0]);
	CHECK(nStatus = pStruct1->SetMyInt32ArrayCount(pStruct1, cCount));
	pInt32Ptr = pStruct1->GetMyInt32Array(pStruct1);
	cCount = pStruct1->GetMyInt32ArrayCount(pStruct1);
	VERIFY(cCount, sizeof(arr_int32) / sizeof(arr_int32[0]));
	memcpy(pInt32Ptr, arr_int32, sizeof(arr_int32));

	cCount = sizeof(arr_uint32) / sizeof(arr_uint32[0]);
	CHECK(nStatus = pStruct1->SetMyUInt32ArrayCount(pStruct1, cCount));
	pUInt32Ptr = pStruct1->GetMyUInt32Array(pStruct1);
	cCount = pStruct1->GetMyUInt32ArrayCount(pStruct1);
	VERIFY(cCount, sizeof(arr_uint32) / sizeof(arr_uint32[0]));
	for (CX_CB_Size i = 0; i < cCount; i++)
	{
		pUInt32Ptr[i] = arr_uint32[i];
	}

	cCount = sizeof(arr_int64) / sizeof(arr_int64[0]);
	CHECK(nStatus = pStruct1->SetMyInt64ArrayCount(pStruct1, cCount));
	pInt64Ptr = pStruct1->GetMyInt64Array(pStruct1);
	cCount = pStruct1->GetMyInt64ArrayCount(pStruct1);
	VERIFY(cCount, sizeof(arr_int64) / sizeof(arr_int64[0]));
	memcpy(pInt64Ptr, arr_int64, sizeof(arr_int64));

	cCount = sizeof(arr_uint64) / sizeof(arr_uint64[0]);
	CHECK(nStatus = pStruct1->SetMyUInt64ArrayCount(pStruct1, cCount));
	pUInt64Ptr = pStruct1->GetMyUInt64Array(pStruct1);
	cCount = pStruct1->GetMyUInt64ArrayCount(pStruct1);
	VERIFY(cCount, sizeof(arr_uint64) / sizeof(arr_uint64[0]));
	for (CX_CB_Size i = 0; i < cCount; i++)
	{
		pUInt64Ptr[i] = arr_uint64[i];
	}

	cCount = sizeof(arr_float) / sizeof(arr_float[0]);
	CHECK(nStatus = pStruct1->SetMyFloatArrayCount(pStruct1, cCount));
	pFloatPtr = pStruct1->GetMyFloatArray(pStruct1);
	cCount = pStruct1->GetMyFloatArrayCount(pStruct1);
	VERIFY(cCount, sizeof(arr_float) / sizeof(arr_float[0]));
	memcpy(pFloatPtr, arr_float, sizeof(arr_float));

	cCount = sizeof(arr_double) / sizeof(arr_double[0]);
	CHECK(nStatus = pStruct1->SetMyDoubleArrayCount(pStruct1, cCount));
	pDoublePtr = pStruct1->GetMyDoubleArray(pStruct1);
	cCount = pStruct1->GetMyDoubleArrayCount(pStruct1);
	VERIFY(cCount, sizeof(arr_double) / sizeof(arr_double[0]));
	for (CX_CB_Size i = 0; i < cCount; i++)
	{
		pDoublePtr[i] = arr_double[i];
	}

	cCount = sizeof(arr_char) / sizeof(arr_char[0]);
	CHECK(nStatus = pStruct1->SetMyCharArrayCount(pStruct1, cCount));
	pCharPtr = pStruct1->GetMyCharArray(pStruct1);
	cCount = pStruct1->GetMyCharArrayCount(pStruct1);
	VERIFY(cCount, sizeof(arr_char) / sizeof(arr_char[0]));
	memcpy(pCharPtr, arr_char, sizeof(arr_char));

	cCount = sizeof(arr_wchar) / sizeof(arr_wchar[0]);
	CHECK(nStatus = pStruct1->SetMyWCharArrayCount(pStruct1, cCount));
	pWCharPtr = pStruct1->GetMyWCharArray(pStruct1);
	cCount = pStruct1->GetMyWCharArrayCount(pStruct1);
	VERIFY(cCount, sizeof(arr_wchar) / sizeof(arr_wchar[0]));
	for (CX_CB_Size i = 0; i < cCount; i++)
	{
		pWCharPtr[i] = arr_wchar[i];
	}
}

static void Check1(My_Namespace_Struct1 *pStruct1)
{
	CX_CB_Size      cCount;
	CX_Bool         *pBoolPtr;
	CX_Int8         *pInt8Ptr;
	CX_UInt8        *pUInt8Ptr;
	CX_Int16        *pInt16Ptr;
	CX_UInt16       *pUInt16Ptr;
	CX_Int32        *pInt32Ptr;
	CX_UInt32       *pUInt32Ptr;
	CX_Int64        *pInt64Ptr;
	CX_UInt64       *pUInt64Ptr;
	CX_Float        *pFloatPtr;
	CX_Double       *pDoublePtr;
	CX_Char         *pCharPtr;
	CX_WChar        *pWCharPtr;

	VERIFY(pStruct1->GetMyBool(pStruct1), bool_);
	VERIFY(pStruct1->GetMyInt8(pStruct1), int8);
	VERIFY(pStruct1->GetMyUInt8(pStruct1), uint8);
	VERIFY(pStruct1->GetMyInt16(pStruct1), int16);
	VERIFY(pStruct1->GetMyUInt16(pStruct1), uint16);
	VERIFY(pStruct1->GetMyInt32(pStruct1), int32);
	VERIFY(pStruct1->GetMyUInt32(pStruct1), uint32);
	VERIFY(pStruct1->GetMyInt64(pStruct1), int64);
	VERIFY(pStruct1->GetMyUInt64(pStruct1), uint64);
	VERIFY(pStruct1->GetMyFloat(pStruct1), float_);
	VERIFY(pStruct1->GetMyDouble(pStruct1), double_);
	VERIFY(pStruct1->GetMyChar(pStruct1), char_);
	VERIFY(pStruct1->GetMyWChar(pStruct1), wchar);

	cCount   = pStruct1->GetMyBoolStaticArrayCount(pStruct1);
	pBoolPtr = pStruct1->GetMyBoolStaticArray(pStruct1);
	VERIFY(cCount, sizeof(arr_st_bool) / sizeof(arr_st_bool[0]));
	VERIFYPTR(pBoolPtr, arr_st_bool, sizeof(arr_st_bool));

	cCount   = pStruct1->GetMyInt8StaticArrayCount(pStruct1);
	pInt8Ptr = pStruct1->GetMyInt8StaticArray(pStruct1);
	VERIFY(cCount, sizeof(arr_st_int8) / sizeof(arr_st_int8[0]));
	VERIFYPTR(pInt8Ptr, arr_st_int8, sizeof(arr_st_int8));

	cCount    = pStruct1->GetMyUInt8StaticArrayCount(pStruct1);
	pUInt8Ptr = pStruct1->GetMyUInt8StaticArray(pStruct1);
	VERIFY(cCount, sizeof(arr_st_uint8) / sizeof(arr_st_uint8[0]));
	VERIFYPTR(pUInt8Ptr, arr_st_uint8, sizeof(arr_st_uint8));

	cCount    = pStruct1->GetMyInt16StaticArrayCount(pStruct1);
	pInt16Ptr = pStruct1->GetMyInt16StaticArray(pStruct1);
	VERIFY(cCount, sizeof(arr_st_int16) / sizeof(arr_st_int16[0]));
	VERIFYPTR(pInt16Ptr, arr_st_int16, sizeof(arr_st_int16));

	cCount     = pStruct1->GetMyUInt16StaticArrayCount(pStruct1);
	pUInt16Ptr = pStruct1->GetMyUInt16StaticArray(pStruct1);
	VERIFY(cCount, sizeof(arr_st_uint16) / sizeof(arr_st_uint16[0]));
	VERIFYPTR(pUInt16Ptr, arr_st_uint16, sizeof(arr_st_uint16));

	cCount    = pStruct1->GetMyInt32StaticArrayCount(pStruct1);
	pInt32Ptr = pStruct1->GetMyInt32StaticArray(pStruct1);
	VERIFY(cCount, sizeof(arr_st_int32) / sizeof(arr_st_int32[0]));
	VERIFYPTR(pInt32Ptr, arr_st_int32, sizeof(arr_st_int32));

	cCount     = pStruct1->GetMyUInt32StaticArrayCount(pStruct1);
	pUInt32Ptr = pStruct1->GetMyUInt32StaticArray(pStruct1);
	VERIFY(cCount, sizeof(arr_st_uint32) / sizeof(arr_st_uint32[0]));
	VERIFYPTR(pUInt32Ptr, arr_st_uint32, sizeof(arr_st_uint32));

	cCount    = pStruct1->GetMyInt64StaticArrayCount(pStruct1);
	pInt64Ptr = pStruct1->GetMyInt64StaticArray(pStruct1);
	VERIFY(cCount, sizeof(arr_st_int64) / sizeof(arr_st_int64[0]));
	VERIFYPTR(pInt64Ptr, arr_st_int64, sizeof(arr_st_int64));

	cCount     = pStruct1->GetMyUInt64StaticArrayCount(pStruct1);
	pUInt64Ptr = pStruct1->GetMyUInt64StaticArray(pStruct1);
	VERIFY(cCount, sizeof(arr_st_uint64) / sizeof(arr_st_uint64[0]));
	VERIFYPTR(pUInt64Ptr, arr_st_uint64, sizeof(arr_st_uint64));

	cCount    = pStruct1->GetMyFloatStaticArrayCount(pStruct1);
	pFloatPtr = pStruct1->GetMyFloatStaticArray(pStruct1);
	VERIFY(cCount, sizeof(arr_st_float) / sizeof(arr_st_float[0]));
	VERIFYPTR(pFloatPtr, arr_st_float, sizeof(arr_st_float));

	cCount     = pStruct1->GetMyDoubleStaticArrayCount(pStruct1);
	pDoublePtr = pStruct1->GetMyDoubleStaticArray(pStruct1);
	VERIFY(cCount, sizeof(arr_st_double) / sizeof(arr_st_double[0]));
	VERIFYPTR(pDoublePtr, arr_st_double, sizeof(arr_st_double));

	cCount   = pStruct1->GetMyCharStaticArrayCount(pStruct1);
	pCharPtr = pStruct1->GetMyCharStaticArray(pStruct1);
	VERIFY(cCount, sizeof(arr_st_char) / sizeof(arr_st_char[0]));
	VERIFYPTR(pCharPtr, arr_st_char, sizeof(arr_st_char));

	cCount    = pStruct1->GetMyWCharStaticArrayCount(pStruct1);
	pWCharPtr = pStruct1->GetMyWCharStaticArray(pStruct1);
	VERIFY(cCount, sizeof(arr_st_wchar) / sizeof(arr_st_wchar[0]));
	VERIFYPTR(pWCharPtr, arr_st_wchar, sizeof(arr_st_wchar));

	cCount = sizeof(arr_bool) / sizeof(arr_bool[0]);
	pBoolPtr = pStruct1->GetMyBoolArray(pStruct1);
	cCount = pStruct1->GetMyBoolArrayCount(pStruct1);
	VERIFY(cCount, sizeof(arr_bool) / sizeof(arr_bool[0]));
	VERIFYPTR(pBoolPtr, arr_bool, sizeof(arr_bool));

	cCount = sizeof(arr_int8) / sizeof(arr_int8[0]);
	pInt8Ptr = pStruct1->GetMyInt8Array(pStruct1);
	cCount = pStruct1->GetMyInt8ArrayCount(pStruct1);
	VERIFY(cCount, sizeof(arr_int8) / sizeof(arr_int8[0]));
	VERIFYPTR(pInt8Ptr, arr_int8, sizeof(arr_int8));

	pUInt8Ptr = pStruct1->GetMyUInt8Array(pStruct1);
	cCount = pStruct1->GetMyUInt8ArrayCount(pStruct1);
	VERIFY(cCount, sizeof(arr_uint8) / sizeof(arr_uint8[0]));
	VERIFYPTR(pUInt8Ptr, arr_uint8, sizeof(arr_uint8));

	pInt16Ptr = pStruct1->GetMyInt16Array(pStruct1);
	cCount = pStruct1->GetMyInt16ArrayCount(pStruct1);
	VERIFY(cCount, sizeof(arr_int16) / sizeof(arr_int16[0]));
	VERIFYPTR(pInt16Ptr, arr_int16, sizeof(arr_int16));

	pUInt16Ptr = pStruct1->GetMyUInt16Array(pStruct1);
	cCount = pStruct1->GetMyUInt16ArrayCount(pStruct1);
	VERIFY(cCount, sizeof(arr_uint16) / sizeof(arr_uint16[0]));
	VERIFYPTR(pUInt16Ptr, arr_uint16, sizeof(arr_uint16));

	pInt32Ptr = pStruct1->GetMyInt32Array(pStruct1);
	cCount = pStruct1->GetMyInt32ArrayCount(pStruct1);
	VERIFY(cCount, sizeof(arr_int32) / sizeof(arr_int32[0]));
	VERIFYPTR(pInt32Ptr, arr_int32, sizeof(arr_int32));

	pUInt32Ptr = pStruct1->GetMyUInt32Array(pStruct1);
	cCount = pStruct1->GetMyUInt32ArrayCount(pStruct1);
	VERIFY(cCount, sizeof(arr_uint32) / sizeof(arr_uint32[0]));
	VERIFYPTR(pUInt32Ptr, arr_uint32, sizeof(arr_uint32));

	pInt64Ptr = pStruct1->GetMyInt64Array(pStruct1);
	cCount = pStruct1->GetMyInt64ArrayCount(pStruct1);
	VERIFY(cCount, sizeof(arr_int64) / sizeof(arr_int64[0]));
	VERIFYPTR(pInt64Ptr, arr_int64, sizeof(arr_int64));

	pUInt64Ptr = pStruct1->GetMyUInt64Array(pStruct1);
	cCount = pStruct1->GetMyUInt64ArrayCount(pStruct1);
	VERIFY(cCount, sizeof(arr_uint64) / sizeof(arr_uint64[0]));
	VERIFYPTR(pUInt64Ptr, arr_uint64, sizeof(arr_uint64));

	pFloatPtr = pStruct1->GetMyFloatArray(pStruct1);
	cCount = pStruct1->GetMyFloatArrayCount(pStruct1);
	VERIFY(cCount, sizeof(arr_float) / sizeof(arr_float[0]));
	VERIFYPTR(pFloatPtr, arr_float, sizeof(arr_float));

	pDoublePtr = pStruct1->GetMyDoubleArray(pStruct1);
	cCount = pStruct1->GetMyDoubleArrayCount(pStruct1);
	VERIFY(cCount, sizeof(arr_double) / sizeof(arr_double[0]));
	VERIFYPTR(pDoublePtr, arr_double, sizeof(arr_double));

	pCharPtr = pStruct1->GetMyCharArray(pStruct1);
	cCount = pStruct1->GetMyCharArrayCount(pStruct1);
	VERIFY(cCount, sizeof(arr_char) / sizeof(arr_char[0]));
	VERIFYPTR(pCharPtr, arr_char, sizeof(arr_char));

	pWCharPtr = pStruct1->GetMyWCharArray(pStruct1);
	cCount = pStruct1->GetMyWCharArrayCount(pStruct1);
	VERIFY(cCount, sizeof(arr_wchar) / sizeof(arr_wchar[0]));
	VERIFYPTR(pWCharPtr, arr_wchar, sizeof(arr_wchar));
}

static void Setup2(My_Namespace_Struct2 *pStruct2)
{
	static const CX_CB_Size COUNT = 3;

	My_Namespace_Struct1   *pStruct1;
	CX_CB_Size             cCount;
	CX_StatusCode          nStatus;

	CHECK(nStatus = pStruct2->SetMyBool(pStruct2, bool_));
	CHECK(nStatus = pStruct2->GetMyStruct1(pStruct2, &pStruct1));
	Setup1(pStruct1);
	Check1(pStruct1);
	pStruct1->Destroy(pStruct1);

	cCount = pStruct2->GetMyStruct1StaticArrayCount(pStruct2);
	VERIFY(cCount, 2);
	for (CX_CB_Size i = 0; i < cCount; i++)
	{
		CHECK(nStatus = pStruct2->GetMyStruct1StaticArrayItem(pStruct2, i, &pStruct1));
		Setup1(pStruct1);
		Check1(pStruct1);
		pStruct1->Destroy(pStruct1);
	}

	CHECK(nStatus = pStruct2->SetMyStruct1ArrayCount(pStruct2, COUNT));
	cCount = pStruct2->GetMyStruct1ArrayCount(pStruct2);
	VERIFY(cCount, COUNT);
	for (CX_CB_Size i = 0; i < cCount; i++)
	{
		CHECK(nStatus = pStruct2->GetMyStruct1ArrayItem(pStruct2, i, &pStruct1));
		Setup1(pStruct1);
		Check1(pStruct1);
		pStruct1->Destroy(pStruct1);
	}
}

static void Check2(My_Namespace_Struct2 *pStruct2)
{
	VERIFY(pStruct2->GetMyBool(pStruct2), bool_);
}

static void Setup()
{
	My_Namespace_Struct2   *pStruct2;
	CX_CB_Buffer           buffer;
	CX_CB_Environment      *pEnv;
	CX_StatusCode          nStatus;

	CHECK(nStatus = CX_CB_CLibEnvironment_Create(&pEnv));
	CHECK(nStatus = CX_CB_Buffer_Init(&buffer));

	CHECK(nStatus = My_Namespace_Struct2_Create(&pStruct2, pEnv, &buffer, 0));
	Setup2(pStruct2);
	Check2(pStruct2);
	CHECK(nStatus = pStruct2->Destroy(pStruct2));

	FILE *pFile = fopen("d:\\temp\\struct.bin", "wb");
	fwrite(buffer.pData, 1, buffer.cbUsedSize, pFile);
	fclose(pFile);

	CHECK(nStatus = CX_CB_Buffer_Uninit(&buffer, pEnv));
	CHECK(pEnv->Destroy(pEnv));
}


void CBuffers_Test()
{
	Setup();
}

REGISTER_TEST(CBuffers_Test);
