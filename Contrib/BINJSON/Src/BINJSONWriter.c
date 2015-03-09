/* 
 * CX - C++ framework for general purpose developement
 *
 * https://github.com/draede/cx
 * 
 * Copyright (C) 2014-2015 draede - draede [at] outlook [dot] com
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

#include "../Include/BINJSONWriter.h"
#include "CX/C/Limits.h"
#include "CX/C/string.h"
#include "CX/C/Platform/Windows/windows.h"


static CX_StatusCode CX_BINJSON_Writer_Write(CX_BINJSON_Writer *pWriter, 
                                             const void *pData, CX_Size cbSize)
{
	CX_StatusCode   nStatus;

	if (CXNOK(nStatus = pWriter->pfnWrite(pWriter->pUserContext, pData, cbSize)))
	{
		return nStatus;
	}

	return CX_Status_OK;
}

static CX_StatusCode CX_BINJSON_Writer_WriteEx(CX_BINJSON_Writer *pWriter, 
                                               const void *pData, CX_Size cbSize)
{
	XXH_errorcode ec;
	CX_StatusCode nStatus;

	if (CXNOK(nStatus = CX_BINJSON_Writer_Write(pWriter, pData, cbSize)))
	{
		return nStatus;
	}
	if (XXH_OK != (ec = XXH32_update(&pWriter->hash, pData, cbSize)))
	{
		return CX_Status_OperationFailed;
	}

	return CX_Status_OK;
}

static CX_StatusCode CX_BINJSON_Writer_WriteHeader(CX_BINJSON_Writer *pWriter)
{
	CX_StatusCode nStatus;

	if (CXNOK(nStatus = CX_BINJSON_Writer_Write(pWriter, &CX_BINJSON_MAGIC, 
	                                            sizeof(CX_BINJSON_MAGIC))))
	{
		return nStatus;
	}
	if (CXNOK(nStatus = CX_BINJSON_Writer_Write(pWriter, &CX_BINJSON_ENCODING_NONE, 
	                                            sizeof(CX_BINJSON_ENCODING_NONE))))
	{
		return nStatus;
	}

	return CX_Status_OK;
}

static CX_StatusCode CX_BINJSON_Writer_WriteFooter(CX_BINJSON_Writer *pWriter, 
                                                   CX_UInt32 nHash)
{
	CX_StatusCode nStatus;

	if (CXNOK(nStatus = CX_BINJSON_Writer_Write(pWriter, &nHash, sizeof(nHash))))
	{
		return nStatus;
	}

	return CX_Status_OK;
}

CX_StatusCode CX_BINJSON_Writer_Init(CX_BINJSON_Writer *pWriter, void *pUserContext, 
                                     CX_BINJSON_HelperAPI *pHelperAPI,
                                     CX_BINJSON_Writer_Write_Func pfnWrite)
{
	XXH_errorcode ec;

	pWriter->cDepth         = 1;
	pWriter->stack[0]       = CX_BINJSON_State_None;
	pWriter->pUserContext   = pUserContext;
	pWriter->pfnWrite       = pfnWrite;
	pWriter->api            = *pHelperAPI;
	if (XXH_OK != (ec = XXH32_reset(&pWriter->hash, 0)))
	{
		return CX_Status_OperationFailed;
	}

	return CX_Status_OK;
}

CX_StatusCode CX_BINJSON_Writer_BeginRootObject(CX_BINJSON_Writer *pWriter)
{
	CX_StatusCode nStatus;

	if (0 == pWriter->cDepth || CX_BINJSON_State_None != pWriter->stack[pWriter->cDepth - 1])
	{
		return CX_Status_InvalidCall;
	}
	if (CXNOK(nStatus = CX_BINJSON_Writer_WriteHeader(pWriter)))
	{
		return nStatus;
	}
	if (CXNOK(nStatus = CX_BINJSON_Writer_WriteEx(pWriter, &CX_BINJSON_TYPE_BEGINROOTOBJECT, 
	                                              sizeof(CX_BINJSON_TYPE_BEGINROOTOBJECT))))
	{
		return nStatus;
	}
	if (pWriter->cDepth + 1 > CX_BINJSON_WRITER_MAX_DEPTH)
	{
		return CX_Status_OutOfBounds;
	}
	pWriter->stack[pWriter->cDepth] = CX_BINJSON_State_RootObject;
	pWriter->cDepth++;

	return CX_Status_OK;
}

CX_StatusCode CX_BINJSON_Writer_EndRootObject(CX_BINJSON_Writer *pWriter)
{
	CX_StatusCode nStatus;

	if (0 == pWriter->cDepth || CX_BINJSON_State_RootObject != pWriter->stack[pWriter->cDepth - 1])
	{
		return CX_Status_InvalidCall;
	}
	if (CXNOK(nStatus = CX_BINJSON_Writer_WriteEx(pWriter, &CX_BINJSON_TYPE_ENDROOTOBJECT, 
	                                              sizeof(CX_BINJSON_TYPE_ENDROOTOBJECT))))
	{
		return nStatus;
	}

	CX_UInt32       nHash;
	
	nHash = XXH32_digest(&pWriter->hash);

	if (CXNOK(nStatus = CX_BINJSON_Writer_WriteFooter(pWriter, nHash)))
	{
		return nStatus;
	}

	pWriter->cDepth--;

	return CX_Status_OK;
}

CX_StatusCode CX_BINJSON_Writer_BeginRootArray(CX_BINJSON_Writer *pWriter)
{
	CX_StatusCode nStatus;

	if (0 == pWriter->cDepth || CX_BINJSON_State_None != pWriter->stack[pWriter->cDepth - 1])
	{
		return CX_Status_InvalidCall;
	}
	if (CXNOK(nStatus = CX_BINJSON_Writer_WriteHeader(pWriter)))
	{
		return nStatus;
	}
	if (CXNOK(nStatus = CX_BINJSON_Writer_WriteEx(pWriter, &CX_BINJSON_TYPE_BEGINROOTARRAY, 
	                                              sizeof(CX_BINJSON_TYPE_BEGINROOTARRAY))))
	{
		return nStatus;
	}
	if (pWriter->cDepth + 1 > CX_BINJSON_WRITER_MAX_DEPTH)
	{
		return CX_Status_OutOfBounds;
	}
	pWriter->stack[pWriter->cDepth] = CX_BINJSON_State_RootArray;
	pWriter->cDepth++;

	return CX_Status_OK;
}

CX_StatusCode CX_BINJSON_Writer_EndRootArray(CX_BINJSON_Writer *pWriter)
{
	CX_StatusCode nStatus;

	if (0 == pWriter->cDepth || CX_BINJSON_State_RootArray != pWriter->stack[pWriter->cDepth - 1])
	{
		return CX_Status_InvalidCall;
	}
	if (CXNOK(nStatus = CX_BINJSON_Writer_WriteEx(pWriter, &CX_BINJSON_TYPE_ENDROOTARRAY, 
	                                              sizeof(CX_BINJSON_TYPE_ENDROOTARRAY))))
	{
		return nStatus;
	}

	CX_UInt32       nHash;

	nHash = XXH32_digest(&pWriter->hash);

	if (CXNOK(nStatus = CX_BINJSON_Writer_WriteFooter(pWriter, nHash)))
	{
		return nStatus;
	}

	pWriter->cDepth--;

	return CX_Status_OK;
}

//object member
CX_StatusCode CX_BINJSON_Writer_ObjWriteNull(CX_BINJSON_Writer *pWriter,
                                             const CX_Char *szName)
{
	CX_UInt32     cNameLen;
	CX_Size       cTmpNameLen;
	CX_StatusCode nStatus;

	if (0 == pWriter->cDepth ||
	   (CX_BINJSON_State_Object != pWriter->stack[pWriter->cDepth - 1] && 
	   CX_BINJSON_State_RootObject != pWriter->stack[pWriter->cDepth - 1]))
	{
		return CX_Status_InvalidCall;
	}
	if (CXNOK(nStatus = CX_BINJSON_Writer_WriteEx(pWriter, &CX_BINJSON_TYPE_NULL, 
	                                              sizeof(CX_BINJSON_TYPE_NULL))))
	{
		return nStatus;
	}
	cTmpNameLen = cx_strlen(szName);
	if ((CX_Size)CX_UINT32_MAX < cTmpNameLen)
	{
		return CX_Status_OutOfBounds;
	}
	cNameLen = (CX_UInt32)cTmpNameLen;
	if (CXNOK(nStatus = CX_BINJSON_Writer_WriteEx(pWriter, &cNameLen, sizeof(cNameLen))))
	{
		return nStatus;
	}
	if (CXNOK(nStatus = CX_BINJSON_Writer_WriteEx(pWriter, szName, cNameLen * sizeof(CX_Char))))
	{
		return nStatus;
	}

	return CX_Status_OK;
}

//array item
CX_StatusCode CX_BINJSON_Writer_ArrWriteNull(CX_BINJSON_Writer *pWriter)
{
	CX_StatusCode nStatus;

	if (0 == pWriter->cDepth ||
	   (CX_BINJSON_State_Array != pWriter->stack[pWriter->cDepth - 1] && 
	   CX_BINJSON_State_RootArray != pWriter->stack[pWriter->cDepth - 1]))
	{
		return CX_Status_InvalidCall;
	}
	if (CXNOK(nStatus = CX_BINJSON_Writer_WriteEx(pWriter, &CX_BINJSON_TYPE_NULL, sizeof(CX_BINJSON_TYPE_NULL))))
	{
		return nStatus;
	}

	return CX_Status_OK;
}

//object member
CX_StatusCode CX_BINJSON_Writer_ObjWriteBool(CX_BINJSON_Writer *pWriter,
                                             const CX_Char *szName, CX_Bool bValue)
{
	CX_UInt32     cNameLen;
	CX_Size       cTmpNameLen;
	CX_StatusCode nStatus;

	if (0 == pWriter->cDepth ||
	   (CX_BINJSON_State_Object != pWriter->stack[pWriter->cDepth - 1] && 
	   CX_BINJSON_State_RootObject != pWriter->stack[pWriter->cDepth - 1]))
	{
		return CX_Status_InvalidCall;
	}
	if (bValue)
	{
		if (CXNOK(nStatus = CX_BINJSON_Writer_WriteEx(pWriter, &CX_BINJSON_TYPE_TRUE, 
		                                              sizeof(CX_BINJSON_TYPE_FALSE))))
		{
			return nStatus;
		}
	}
	else
	{
		if (CXNOK(nStatus = CX_BINJSON_Writer_WriteEx(pWriter, &CX_BINJSON_TYPE_FALSE, 
		                                              sizeof(CX_BINJSON_TYPE_FALSE))))
		{
			return nStatus;
		}
	}
	if ((CX_Size)CX_UINT32_MAX < cx_strlen(szName))
	{
		return CX_Status_OutOfBounds;
	}
	cTmpNameLen = cx_strlen(szName);
	if ((CX_Size)CX_UINT32_MAX < cTmpNameLen)
	{
		return CX_Status_OutOfBounds;
	}
	cNameLen = (CX_UInt32)cTmpNameLen;
	if (CXNOK(nStatus = CX_BINJSON_Writer_WriteEx(pWriter, &cNameLen, sizeof(cNameLen))))
	{
		return nStatus;
	}
	if (CXNOK(nStatus = CX_BINJSON_Writer_WriteEx(pWriter, szName, cNameLen * sizeof(CX_Char))))
	{
		return nStatus;
	}

	return CX_Status_OK;
}

//array item
CX_StatusCode CX_BINJSON_Writer_ArrWriteBool(CX_BINJSON_Writer *pWriter, CX_Bool bValue)
{
	CX_StatusCode nStatus;

	if (0 == pWriter->cDepth ||
	   (CX_BINJSON_State_Array != pWriter->stack[pWriter->cDepth - 1] && 
	   CX_BINJSON_State_RootArray != pWriter->stack[pWriter->cDepth - 1]))
	{
		return CX_Status_InvalidCall;
	}
	if (bValue)
	{
		if (CXNOK(nStatus = CX_BINJSON_Writer_WriteEx(pWriter, &CX_BINJSON_TYPE_TRUE, 
		                                              sizeof(CX_BINJSON_TYPE_FALSE))))
		{
			return nStatus;
		}
	}
	else
	{
		if (CXNOK(nStatus = CX_BINJSON_Writer_WriteEx(pWriter, &CX_BINJSON_TYPE_FALSE, 
		                                              sizeof(CX_BINJSON_TYPE_FALSE))))
		{
			return nStatus;
		}
	}

	return CX_Status_OK;
}

//object member
CX_StatusCode CX_BINJSON_Writer_ObjWriteInt(CX_BINJSON_Writer *pWriter,
                                            const CX_Char *szName, CX_Int64 nValue)
{
	CX_UInt32     cNameLen;
	CX_Size       cTmpNameLen;
	CX_StatusCode nStatus;

	if (0 == pWriter->cDepth ||
	   (CX_BINJSON_State_Object != pWriter->stack[pWriter->cDepth - 1] && 
	   CX_BINJSON_State_RootObject != pWriter->stack[pWriter->cDepth - 1]))
	{
		return CX_Status_InvalidCall;
	}
	if (CXNOK(nStatus = CX_BINJSON_Writer_WriteEx(pWriter, &CX_BINJSON_TYPE_INT, 
	                                              sizeof(CX_BINJSON_TYPE_INT))))
	{
		return nStatus;
	}
	cTmpNameLen = cx_strlen(szName);
	if ((CX_Size)CX_UINT32_MAX < cTmpNameLen)
	{
		return CX_Status_OutOfBounds;
	}
	cNameLen = (CX_UInt32)cTmpNameLen;
	if (CXNOK(nStatus = CX_BINJSON_Writer_WriteEx(pWriter, &cNameLen, sizeof(cNameLen))))
	{
		return nStatus;
	}
	if (CXNOK(nStatus = CX_BINJSON_Writer_WriteEx(pWriter, szName, cNameLen * sizeof(CX_Char))))
	{
		return nStatus;
	}
	if (CXNOK(nStatus = CX_BINJSON_Writer_WriteEx(pWriter, &nValue, sizeof(nValue))))
	{
		return nStatus;
	}

	return CX_Status_OK;
}

//array item
CX_StatusCode CX_BINJSON_Writer_ArrWriteInt(CX_BINJSON_Writer *pWriter, CX_Int64 nValue)
{
	CX_StatusCode nStatus;

	if (0 == pWriter->cDepth ||
	   (CX_BINJSON_State_Array != pWriter->stack[pWriter->cDepth - 1] && 
	   CX_BINJSON_State_RootArray != pWriter->stack[pWriter->cDepth - 1]))
	{
		return CX_Status_InvalidCall;
	}
	if (CXNOK(nStatus = CX_BINJSON_Writer_WriteEx(pWriter, &CX_BINJSON_TYPE_INT, 
	                                              sizeof(CX_BINJSON_TYPE_INT))))
	{
		return nStatus;
	}
	if (CXNOK(nStatus = CX_BINJSON_Writer_WriteEx(pWriter, &nValue, sizeof(nValue))))
	{
		return nStatus;
	}

	return CX_Status_OK;
}

//object member
CX_StatusCode CX_BINJSON_Writer_ObjWriteReal(CX_BINJSON_Writer *pWriter,
                                             const CX_Char *szName, CX_Double lfValue)
{
	CX_UInt32     cNameLen;
	CX_Size       cTmpNameLen;
	CX_StatusCode nStatus;

	if (0 == pWriter->cDepth ||
	   (CX_BINJSON_State_Object != pWriter->stack[pWriter->cDepth - 1] && 
	   CX_BINJSON_State_RootObject != pWriter->stack[pWriter->cDepth - 1]))
	{
		return CX_Status_InvalidCall;
	}
	if (CXNOK(nStatus = CX_BINJSON_Writer_WriteEx(pWriter, &CX_BINJSON_TYPE_REAL, 
	                                              sizeof(CX_BINJSON_TYPE_REAL))))
	{
		return nStatus;
	}
	cTmpNameLen = cx_strlen(szName);
	if ((CX_Size)CX_UINT32_MAX < cTmpNameLen)
	{
		return CX_Status_OutOfBounds;
	}
	cNameLen = (CX_UInt32)cTmpNameLen;
	if (CXNOK(nStatus = CX_BINJSON_Writer_WriteEx(pWriter, &cNameLen, sizeof(cNameLen))))
	{
		return nStatus;
	}
	if (CXNOK(nStatus = CX_BINJSON_Writer_WriteEx(pWriter, szName, cNameLen * sizeof(CX_Char))))
	{
		return nStatus;
	}
	if (CXNOK(nStatus = CX_BINJSON_Writer_WriteEx(pWriter, &lfValue, sizeof(lfValue))))
	{
		return nStatus;
	}

	return CX_Status_OK;
}

//array item
CX_StatusCode CX_BINJSON_Writer_ArrWriteReal(CX_BINJSON_Writer *pWriter, CX_Double lfValue)
{
	CX_StatusCode nStatus;

	if (0 == pWriter->cDepth ||
	   (CX_BINJSON_State_Array != pWriter->stack[pWriter->cDepth - 1] && 
	   CX_BINJSON_State_RootArray != pWriter->stack[pWriter->cDepth - 1]))
	{
		return CX_Status_InvalidCall;
	}
	if (CXNOK(nStatus = CX_BINJSON_Writer_WriteEx(pWriter, &CX_BINJSON_TYPE_REAL, 
	                                              sizeof(CX_BINJSON_TYPE_REAL))))
	{
		return nStatus;
	}
	if (CXNOK(nStatus = CX_BINJSON_Writer_WriteEx(pWriter, &lfValue, sizeof(lfValue))))
	{
		return nStatus;
	}

	return CX_Status_OK;
}

//object member
CX_StatusCode CX_BINJSON_Writer_ObjWriteString(CX_BINJSON_Writer *pWriter,
                                               const CX_Char *szName, const CX_Char *szValue)
{
	CX_UInt32     cNameLen;
	CX_Size       cTmpNameLen;
	CX_UInt32     cValueLen;
	CX_Size       cTmpValueLen;
	CX_StatusCode nStatus;

	if (0 == pWriter->cDepth ||
	   (CX_BINJSON_State_Object != pWriter->stack[pWriter->cDepth - 1] && 
	   CX_BINJSON_State_RootObject != pWriter->stack[pWriter->cDepth - 1]))
	{
		return CX_Status_InvalidCall;
	}
	if (CXNOK(nStatus = CX_BINJSON_Writer_WriteEx(pWriter, &CX_BINJSON_TYPE_STRING, 
	                                              sizeof(CX_BINJSON_TYPE_STRING))))
	{
		return nStatus;
	}
	cTmpNameLen = cx_strlen(szName);
	if ((CX_Size)CX_UINT32_MAX < cTmpNameLen)
	{
		return CX_Status_OutOfBounds;
	}
	cNameLen = (CX_UInt32)cTmpNameLen;
	if (CXNOK(nStatus = CX_BINJSON_Writer_WriteEx(pWriter, &cNameLen, sizeof(cNameLen))))
	{
		return nStatus;
	}
	if (CXNOK(nStatus = CX_BINJSON_Writer_WriteEx(pWriter, szName, cNameLen * sizeof(CX_Char))))
	{
		return nStatus;
	}
	cTmpValueLen = cx_strlen(szValue);
	if ((CX_Size)CX_UINT32_MAX < cTmpValueLen)
	{
		return CX_Status_OutOfBounds;
	}
	cValueLen = (CX_UInt32)cTmpValueLen;
	if (CXNOK(nStatus = CX_BINJSON_Writer_WriteEx(pWriter, &cValueLen, sizeof(cValueLen))))
	{
		return nStatus;
	}
	if (CXNOK(nStatus = CX_BINJSON_Writer_WriteEx(pWriter, szValue, cValueLen * sizeof(CX_Char))))
	{
		return nStatus;
	}

	return CX_Status_OK;
}

//array item
CX_StatusCode CX_BINJSON_Writer_ArrWriteString(CX_BINJSON_Writer *pWriter,
                                               const CX_Char *szValue)
{
	CX_UInt32     cValueLen;
	CX_Size       cTmpValueLen;
	CX_StatusCode nStatus;

	if (0 == pWriter->cDepth ||
	   (CX_BINJSON_State_Array != pWriter->stack[pWriter->cDepth - 1] && 
	   CX_BINJSON_State_RootArray != pWriter->stack[pWriter->cDepth - 1]))
	{
		return CX_Status_InvalidCall;
	}
	if (CXNOK(nStatus = CX_BINJSON_Writer_WriteEx(pWriter, &CX_BINJSON_TYPE_STRING, 
	                                              sizeof(CX_BINJSON_TYPE_STRING))))
	{
		return nStatus;
	}
	cTmpValueLen = cx_strlen(szValue);
	if ((CX_Size)CX_UINT32_MAX < cTmpValueLen)
	{
		return CX_Status_OutOfBounds;
	}
	cValueLen = (CX_UInt32)cTmpValueLen;
	if (CXNOK(nStatus = CX_BINJSON_Writer_WriteEx(pWriter, &cValueLen, sizeof(cValueLen))))
	{
		return nStatus;
	}
	if (CXNOK(nStatus = CX_BINJSON_Writer_WriteEx(pWriter, szValue, cValueLen * sizeof(CX_Char))))
	{
		return nStatus;
	}

	return CX_Status_OK;
}

//object member
CX_StatusCode CX_BINJSON_Writer_ObjWriteWString(CX_BINJSON_Writer *pWriter,
                                                const CX_Char *szName, const CX_WChar *wszValue)
{
	CX_UInt32     cNameLen;
	CX_Size       cTmpNameLen;
	CX_UInt32     cValueLen;
	CX_Size       cTmpValueLen;
	CX_StatusCode nStatus;

	if (0 == pWriter->cDepth ||
	   (CX_BINJSON_State_Object != pWriter->stack[pWriter->cDepth - 1] && 
	   CX_BINJSON_State_RootObject != pWriter->stack[pWriter->cDepth - 1]))
	{
		return CX_Status_InvalidCall;
	}
	if (CXNOK(nStatus = CX_BINJSON_Writer_WriteEx(pWriter, &CX_BINJSON_TYPE_WSTRING, 
	                                              sizeof(CX_BINJSON_TYPE_WSTRING))))
	{
		return nStatus;
	}
	cTmpNameLen = cx_strlen(szName);
	if ((CX_Size)CX_UINT32_MAX < cTmpNameLen)
	{
		return CX_Status_OutOfBounds;
	}
	cNameLen = (CX_UInt32)cTmpNameLen;
	if CXNOK((nStatus = CX_BINJSON_Writer_WriteEx(pWriter, &cNameLen, sizeof(cNameLen))))
	{
		return nStatus;
	}
	if (CXNOK(nStatus = CX_BINJSON_Writer_WriteEx(pWriter, szName, cNameLen * sizeof(CX_Char))))
	{
		return nStatus;
	}

	cTmpValueLen = cxw_strlen(wszValue);
	if ((CX_Size)CX_UINT32_MAX < cTmpValueLen)
	{
		return CX_Status_OutOfBounds;
	}
	cValueLen = (CX_UInt32)cTmpValueLen;
	if CXNOK((nStatus = CX_BINJSON_Writer_WriteEx(pWriter, &cValueLen, sizeof(cValueLen))))
	{
		return nStatus;
	}
	if (CXNOK(nStatus = CX_BINJSON_Writer_WriteEx(pWriter, wszValue, cValueLen * sizeof(CX_WChar))))
	{
		return nStatus;
	}

	return CX_Status_OK;
}

//array item
CX_StatusCode CX_BINJSON_Writer_ArrWriteWString(CX_BINJSON_Writer *pWriter,
                                                const CX_WChar *wszValue)
{
	CX_UInt32     cValueLen;
	CX_Size       cTmpValueLen;
	CX_StatusCode nStatus;

	if (0 == pWriter->cDepth ||
	   (CX_BINJSON_State_Array != pWriter->stack[pWriter->cDepth - 1] && 
	   CX_BINJSON_State_RootArray != pWriter->stack[pWriter->cDepth - 1]))
	{
		return CX_Status_InvalidCall;
	}
	if (CXNOK(nStatus = CX_BINJSON_Writer_WriteEx(pWriter, &CX_BINJSON_TYPE_WSTRING, 
	                                              sizeof(CX_BINJSON_TYPE_WSTRING))))
	{
		return nStatus;
	}

	cTmpValueLen = cxw_strlen(wszValue);
	if ((CX_Size)CX_UINT32_MAX < cTmpValueLen)
	{
		return CX_Status_OutOfBounds;
	}
	cValueLen = (CX_UInt32)cTmpValueLen;
	if CXNOK((nStatus = CX_BINJSON_Writer_WriteEx(pWriter, &cValueLen, sizeof(cValueLen))))
	{
		return nStatus;
	}
	if (CXNOK(nStatus = CX_BINJSON_Writer_WriteEx(pWriter, wszValue, cValueLen * sizeof(CX_WChar))))
	{
		return nStatus;
	}

	return CX_Status_OK;
}

//object member
CX_StatusCode CX_BINJSON_Writer_ObjWriteBLOB(CX_BINJSON_Writer *pWriter,
                                             const CX_Char *szName, const void *pData, 
                                             CX_Size cbSize)
{
	CX_UInt32     cNameLen;
	CX_Size       cTmpNameLen;
	CX_UInt32     cbValueSize;
	CX_StatusCode nStatus;

	if (0 == pWriter->cDepth ||
	   (CX_BINJSON_State_Object != pWriter->stack[pWriter->cDepth - 1] && 
	   CX_BINJSON_State_RootObject != pWriter->stack[pWriter->cDepth - 1]))
	{
		return CX_Status_InvalidCall;
	}
	if (CXNOK(nStatus = CX_BINJSON_Writer_WriteEx(pWriter, &CX_BINJSON_TYPE_BLOB, 
	                                              sizeof(CX_BINJSON_TYPE_BLOB))))
	{
		return nStatus;
	}
	cTmpNameLen = cx_strlen(szName);
	if ((CX_Size)CX_UINT32_MAX < cTmpNameLen)
	{
		return CX_Status_OutOfBounds;
	}
	cNameLen = (CX_UInt32)cTmpNameLen;
	if (CXNOK(nStatus = CX_BINJSON_Writer_WriteEx(pWriter, &cNameLen, sizeof(cNameLen))))
	{
		return nStatus;
	}
	if (CXNOK(nStatus = CX_BINJSON_Writer_WriteEx(pWriter, szName, cNameLen * sizeof(CX_Char))))
	{
		return nStatus;
	}
	if ((CX_Size)CX_UINT32_MAX < cbSize)
	{
		return CX_Status_OutOfBounds;
	}
	cbValueSize = (CX_UInt32)cbSize;
	if (CXNOK(nStatus = CX_BINJSON_Writer_WriteEx(pWriter, &cbValueSize, sizeof(cbValueSize))))
	{
		return nStatus;
	}
	if (CXNOK(nStatus = CX_BINJSON_Writer_WriteEx(pWriter, pData, cbSize)))
	{
		return nStatus;
	}

	return CX_Status_OK;
}

//array item
CX_StatusCode CX_BINJSON_Writer_ArrWriteBLOB(CX_BINJSON_Writer *pWriter,
                                             const void *pData, CX_Size cbSize)
{
	CX_UInt32     cbValueSize;
	CX_StatusCode nStatus;

	if (0 == pWriter->cDepth ||
	   (CX_BINJSON_State_Array != pWriter->stack[pWriter->cDepth - 1] && 
	   CX_BINJSON_State_RootArray != pWriter->stack[pWriter->cDepth - 1]))
	{
		return CX_Status_InvalidCall;
	}
	if (CXNOK(nStatus = CX_BINJSON_Writer_WriteEx(pWriter, &CX_BINJSON_TYPE_BLOB, 
	                                              sizeof(CX_BINJSON_TYPE_BLOB))))
	{
		return nStatus;
	}
	if ((CX_Size)CX_UINT32_MAX < cbSize)
	{
		return CX_Status_OutOfBounds;
	}
	cbValueSize = (CX_UInt32)cbSize;
	if (CXNOK(nStatus = CX_BINJSON_Writer_WriteEx(pWriter, &cbValueSize, sizeof(cbValueSize))))
	{
		return nStatus;
	}
	if (CXNOK(nStatus = CX_BINJSON_Writer_WriteEx(pWriter, pData, cbSize)))
	{
		return nStatus;
	}

	return CX_Status_OK;
}

//object member
CX_StatusCode CX_BINJSON_Writer_ObjBeginObject(CX_BINJSON_Writer *pWriter,
                                               const CX_Char *szName)
{
	CX_UInt32     cNameLen;
	CX_Size       cTmpNameLen;
	CX_StatusCode nStatus;

	if (0 == pWriter->cDepth ||
	   (CX_BINJSON_State_Object != pWriter->stack[pWriter->cDepth - 1] && 
	   CX_BINJSON_State_RootObject != pWriter->stack[pWriter->cDepth - 1]))
	{
		return CX_Status_InvalidCall;
	}
	if (CXNOK(nStatus = CX_BINJSON_Writer_WriteEx(pWriter, &CX_BINJSON_TYPE_BEGINOBJECT, 
	                                              sizeof(CX_BINJSON_TYPE_BEGINOBJECT))))
	{
		return nStatus;
	}
	cTmpNameLen = cx_strlen(szName);
	if ((CX_Size)CX_UINT32_MAX < cTmpNameLen)
	{
		return CX_Status_OutOfBounds;
	}
	cNameLen = (CX_UInt32)cTmpNameLen;
	if (CXNOK(nStatus = CX_BINJSON_Writer_WriteEx(pWriter, &cNameLen, sizeof(cNameLen))))
	{
		return nStatus;
	}
	if (CXNOK(nStatus = CX_BINJSON_Writer_WriteEx(pWriter, szName, cNameLen * sizeof(CX_Char))))
	{
		return nStatus;
	}
	if (pWriter->cDepth + 1 > CX_BINJSON_WRITER_MAX_DEPTH)
	{
		return CX_Status_OutOfBounds;
	}
	pWriter->stack[pWriter->cDepth] = CX_BINJSON_State_Object;
	pWriter->cDepth++;

	return CX_Status_OK;
}

//array item
CX_StatusCode CX_BINJSON_Writer_ArrBeginObject(CX_BINJSON_Writer *pWriter)
{
	CX_StatusCode nStatus;

	if (0 == pWriter->cDepth ||
	   (CX_BINJSON_State_Array != pWriter->stack[pWriter->cDepth - 1] && 
	   CX_BINJSON_State_RootArray != pWriter->stack[pWriter->cDepth - 1]))
	{
		return CX_Status_InvalidCall;
	}
	if (CXNOK(nStatus = CX_BINJSON_Writer_WriteEx(pWriter, &CX_BINJSON_TYPE_BEGINOBJECT, 
	                                              sizeof(CX_BINJSON_TYPE_BEGINOBJECT))))
	{
		return nStatus;
	}
	if (pWriter->cDepth + 1 > CX_BINJSON_WRITER_MAX_DEPTH)
	{
		return CX_Status_OutOfBounds;
	}
	pWriter->stack[pWriter->cDepth] = CX_BINJSON_State_Object;
	pWriter->cDepth++;

	return CX_Status_OK;
}

//object member
CX_StatusCode CX_BINJSON_Writer_ObjBeginArray(CX_BINJSON_Writer *pWriter,
                                              const CX_Char *szName)
{
	CX_UInt32     cNameLen;
	CX_Size       cTmpNameLen;
	CX_StatusCode nStatus;

	if (0 == pWriter->cDepth ||
	   (CX_BINJSON_State_Object != pWriter->stack[pWriter->cDepth - 1] && 
	   CX_BINJSON_State_RootObject != pWriter->stack[pWriter->cDepth - 1]))
	{
		return CX_Status_InvalidCall;
	}
	if (CXNOK(nStatus = CX_BINJSON_Writer_WriteEx(pWriter, &CX_BINJSON_TYPE_BEGINARRAY, 
	                                              sizeof(CX_BINJSON_TYPE_BEGINARRAY))))
	{
		return nStatus;
	}
	cTmpNameLen = cx_strlen(szName);
	if ((CX_Size)CX_UINT32_MAX < cTmpNameLen)
	{
		return CX_Status_OutOfBounds;
	}
	cNameLen = (CX_UInt32)cTmpNameLen;
	if (CXNOK(nStatus = CX_BINJSON_Writer_WriteEx(pWriter, &cNameLen, sizeof(cNameLen))))
	{
		return nStatus;
	}
	if (CXNOK(nStatus = CX_BINJSON_Writer_WriteEx(pWriter, szName, cNameLen * sizeof(CX_Char))))
	{
		return nStatus;
	}
	if (pWriter->cDepth + 1 > CX_BINJSON_WRITER_MAX_DEPTH)
	{
		return CX_Status_OutOfBounds;
	}
	pWriter->stack[pWriter->cDepth] = CX_BINJSON_State_Array;
	pWriter->cDepth++;

	return CX_Status_OK;
}

//array item
CX_StatusCode CX_BINJSON_Writer_ArrBeginArray(CX_BINJSON_Writer *pWriter)
{
	CX_StatusCode nStatus;

	if (0 == pWriter->cDepth ||
	   (CX_BINJSON_State_Array != pWriter->stack[pWriter->cDepth - 1] && 
	   CX_BINJSON_State_RootArray != pWriter->stack[pWriter->cDepth - 1]))
	{
		return CX_Status_InvalidCall;
	}
	if (CXNOK(nStatus = CX_BINJSON_Writer_WriteEx(pWriter, &CX_BINJSON_TYPE_BEGINARRAY, 
	                                              sizeof(CX_BINJSON_TYPE_BEGINARRAY))))
	{
		return nStatus;
	}
	if (pWriter->cDepth + 1 > CX_BINJSON_WRITER_MAX_DEPTH)
	{
		return CX_Status_OutOfBounds;
	}
	pWriter->stack[pWriter->cDepth] = CX_BINJSON_State_Array;
	pWriter->cDepth++;

	return CX_Status_OK;
}

CX_StatusCode CX_BINJSON_Writer_EndObject(CX_BINJSON_Writer *pWriter)
{
	CX_StatusCode nStatus;

	if (0 == pWriter->cDepth || CX_BINJSON_State_Object != pWriter->stack[pWriter->cDepth - 1])
	{
		return CX_Status_InvalidCall;
	}
	if (CXNOK(nStatus = CX_BINJSON_Writer_WriteEx(pWriter, &CX_BINJSON_TYPE_ENDOBJECT, 
	                                              sizeof(CX_BINJSON_TYPE_ENDOBJECT))))
	{
		return nStatus;
	}
	pWriter->cDepth--;

	return CX_Status_OK;
}

CX_StatusCode CX_BINJSON_Writer_EndArray(CX_BINJSON_Writer *pWriter)
{
	CX_StatusCode nStatus;

	if (0 == pWriter->cDepth || CX_BINJSON_State_Array != pWriter->stack[pWriter->cDepth - 1])
	{
		return CX_Status_InvalidCall;
	}
	if (CXNOK(nStatus = CX_BINJSON_Writer_WriteEx(pWriter, &CX_BINJSON_TYPE_ENDARRAY, 
	                                              sizeof(CX_BINJSON_TYPE_ENDARRAY))))
	{
		return nStatus;
	}
	pWriter->cDepth--;

	return CX_Status_OK;
}

