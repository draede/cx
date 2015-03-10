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

#include "../Include/BINJSONReader.h"


static CX_StatusCode CX_BINJSON_Reader_Read(CX_BINJSON_Reader *pReader, void *pData, CX_Size cbSize)
{
	return pReader->pfnRead(pReader->pUserContext, pData, cbSize);
}

static CX_StatusCode CX_BINJSON_Reader_ReadEx(CX_BINJSON_Reader *pReader, void *pData, 
                                              CX_Size cbSize)
{
	XXH_errorcode ec;
	CX_StatusCode nStatus;

	if (CXNOK(nStatus = CX_BINJSON_Reader_Read(pReader, pData, cbSize)))
	{
		return nStatus;
	}
	if (XXH_OK != (ec = XXH32_update(&pReader->hash, pData, cbSize)))
	{
		return CX_Status_OperationFailed;
	}

	return CX_Status_OK;
}

static CX_StatusCode CX_BINJSON_Reader_ReadHeader(CX_BINJSON_Reader *pReader)
{
	CX_UInt64     nMagic;
	CX_UInt32     nEncoding;
	CX_StatusCode nStatus;

	if (CXNOK(nStatus = CX_BINJSON_Reader_Read(pReader, &nMagic, sizeof(nMagic))))
	{
		return nStatus;
	}
	if (CX_BINJSON_MAGIC != nMagic)
	{
		return CX_Status_OperationFailed;
	}
	if (CXNOK(nStatus = CX_BINJSON_Reader_Read(pReader, &nEncoding, sizeof(nEncoding))))
	{
		return nStatus;
	}
	if (CX_BINJSON_ENCODING_NONE != nEncoding)
	{
		return CX_Status_OperationFailed;
	}

	return CX_Status_OK;
}

static CX_StatusCode CX_BINJSON_Reader_ReadFooter(CX_BINJSON_Reader *pReader, CX_UInt32 *pnHash)
{
	CX_StatusCode nStatus;

	if (CXNOK(nStatus = CX_BINJSON_Reader_Read(pReader, pnHash, sizeof(*pnHash))))
	{
		return nStatus;
	}

	return CX_Status_OK;
}

static CX_StatusCode CX_BINJSON_Reader_ReadEntryType(CX_BINJSON_Reader *pReader)
{
	CX_StatusCode nStatus;

	if (CXNOK(nStatus = CX_BINJSON_Reader_ReadEx(pReader, &pReader->nCrEntryType, 
	                                             sizeof(pReader->nCrEntryType))))
	{
		return nStatus;
	}

	return CX_Status_OK;
}

static CX_StatusCode CX_BINJSON_Reader_ReadStringData(CX_BINJSON_Reader *pReader, 
                                                      CX_BINJSON_Reader_String *psName)
{
	CX_UInt32     cNameLen;
	CX_StatusCode nStatus;

	if (CXNOK(nStatus = CX_BINJSON_Reader_ReadEx(pReader, &cNameLen, sizeof(cNameLen))))
	{
		return nStatus;
	}
	if (cNameLen + 1 <= sizeof(psName->buffer) / sizeof(psName->buffer[0]))
	{
		psName->pString = psName->buffer;
	}
	else
	{
		if (NULL == (psName->pString = (CX_Char *)pReader->api.Alloc(pReader->pUserContext, 
		                                                            (cNameLen + 1)* sizeof(CX_Char))))
		{
			return CX_Status_MemAllocFailed;
		}
	}
	if (0 < cNameLen)
	{
		if (CXNOK(nStatus = CX_BINJSON_Reader_ReadEx(pReader, psName->pString, 
		                                             cNameLen * sizeof(CX_Char))))
		{
			return nStatus;
		}
		psName->pString[cNameLen] = 0;
	}
	else
	{
		psName->pString[0] = 0;
	}

	return CX_Status_OK;
}

static CX_StatusCode CX_BINJSON_Reader_ReadWStringData(CX_BINJSON_Reader *pReader, 
                                                      CX_BINJSON_Reader_WString *psName)
{
	CX_UInt32     cNameLen;
	CX_StatusCode nStatus;

	if (CXNOK(nStatus = CX_BINJSON_Reader_ReadEx(pReader, &cNameLen, sizeof(cNameLen))))
	{
		return nStatus;
	}
	if (cNameLen + 1 <= sizeof(psName->buffer) / sizeof(psName->buffer[0]))
	{
		psName->pWString = psName->buffer;
	}
	else
	{
		if (NULL == (psName->pWString = (CX_WChar *)pReader->api.Alloc(pReader->pUserContext, 
		                                                           (cNameLen + 1)* sizeof(CX_WChar))))
		{
			return CX_Status_MemAllocFailed;
		}
	}
	if (0 < cNameLen)
	{
		if (CXNOK(nStatus = CX_BINJSON_Reader_ReadEx(pReader, psName->pWString, 
		                                             cNameLen * sizeof(CX_WChar))))
		{
			return nStatus;
		}
		psName->pWString[cNameLen] = 0;
	}
	else
	{
		psName->pWString[0] = 0;
	}

	return CX_Status_OK;
}

static CX_StatusCode CX_BINJSON_Reader_ReadObjectEntry(CX_BINJSON_Reader *pReader, 
                                                       CX_BINJSON_Reader_String *psName, 
                                                       CX_Byte nType, void *pData, CX_Size cbSize,
                                                       void **ppData, CX_Size *pcbSize)
{
	CX_StatusCode nStatus;

	if (0 == pReader->cDepth)
	{
		return CX_Status_InvalidCall;
	}
	if (CX_BINJSON_TYPE_BEGINROOTOBJECT != pReader->stack[pReader->cDepth - 1] &&
	    CX_BINJSON_TYPE_BEGINOBJECT != pReader->stack[pReader->cDepth - 1])
	{
		return CX_Status_InvalidCall;
	}
	if (CX_BINJSON_TYPE_ENDROOTOBJECT == pReader->nCrEntryType ||
	    CX_BINJSON_TYPE_ENDROOTARRAY == pReader->nCrEntryType ||
	    CX_BINJSON_TYPE_ENDOBJECT == pReader->nCrEntryType ||
	    CX_BINJSON_TYPE_ENDARRAY == pReader->nCrEntryType)
	{
		return CX_Status_OutOfBounds;
	}
	if (nType != pReader->nCrEntryType)
	{
		if (!((CX_BINJSON_TYPE_FALSE == pReader->nCrEntryType && CX_BINJSON_TYPE_TRUE == nType) ||
		   (CX_BINJSON_TYPE_TRUE == pReader->nCrEntryType && CX_BINJSON_TYPE_FALSE == nType)))
		{
			return CX_Status_InvalidCall;
		}
	}
	if (CXNOK(nStatus = CX_BINJSON_Reader_ReadStringData(pReader, psName)))
	{
		return nStatus;
	}
	if (CX_BINJSON_TYPE_NULL != nType)
	{
		if (CX_BINJSON_TYPE_FALSE == pReader->nCrEntryType)
		{
			*((CX_Bool *)pData) = CX_False;
		}
		else
		if (CX_BINJSON_TYPE_TRUE == pReader->nCrEntryType)
		{
			*((CX_Bool *)pData) = CX_True;
		}
		else
		if (CX_BINJSON_TYPE_STRING == pReader->nCrEntryType)
		{
			if (CXNOK(nStatus = CX_BINJSON_Reader_ReadStringData(pReader, 
			                                                     (CX_BINJSON_Reader_String *)pData)))
			{
				return nStatus;
			}
		}
		else
		if (CX_BINJSON_TYPE_WSTRING == pReader->nCrEntryType)
		{
			if (CXNOK(nStatus = CX_BINJSON_Reader_ReadWStringData(pReader, 
			                                                      (CX_BINJSON_Reader_WString *)pData)))
			{
				return nStatus;
			}
		}
		else
		if (CX_BINJSON_TYPE_BLOB == pReader->nCrEntryType)
		{
			CX_UInt32 cbTmpSize;

			if (CXNOK(nStatus = CX_BINJSON_Reader_ReadEx(pReader, &cbTmpSize, sizeof(cbTmpSize))))
			{
				return nStatus;
			}
			if (NULL == (*ppData = pReader->api.Alloc(pReader->pUserContext, cbTmpSize)))
			{
				return CX_Status_MemAllocFailed;
			}
			if (CXNOK(nStatus = CX_BINJSON_Reader_ReadEx(pReader, *ppData, cbTmpSize)))
			{
				return nStatus;
			}
			*pcbSize = cbTmpSize;
		}
		else
		{
			if (CXNOK(nStatus = CX_BINJSON_Reader_ReadEx(pReader, pData, cbSize)))
			{
				return nStatus;
			}
		}
	}

	return CX_BINJSON_Reader_ReadEntryType(pReader);
}

static CX_StatusCode CX_BINJSON_Reader_ReadArrayEntry(CX_BINJSON_Reader *pReader, CX_Byte nType, 
                                                      void *pData, CX_Size cbSize,
                                                      void **ppData, CX_Size *pcbSize)
{
	CX_StatusCode nStatus;

	if (0 == pReader->cDepth)
	{
		return CX_Status_InvalidCall;
	}
	if (CX_BINJSON_TYPE_BEGINROOTARRAY != pReader->stack[pReader->cDepth - 1] &&
	    CX_BINJSON_TYPE_BEGINARRAY != pReader->stack[pReader->cDepth - 1])
	{
		return CX_Status_InvalidCall;
	}
	if (CX_BINJSON_TYPE_ENDROOTOBJECT == pReader->nCrEntryType ||
	    CX_BINJSON_TYPE_ENDROOTARRAY == pReader->nCrEntryType ||
	    CX_BINJSON_TYPE_ENDOBJECT == pReader->nCrEntryType ||
	    CX_BINJSON_TYPE_ENDARRAY == pReader->nCrEntryType)
	{
		return CX_Status_OutOfBounds;
	}
	if (nType != pReader->nCrEntryType)
	{
		if (!((CX_BINJSON_TYPE_FALSE == pReader->nCrEntryType && CX_BINJSON_TYPE_TRUE == nType) ||
			(CX_BINJSON_TYPE_TRUE == pReader->nCrEntryType && CX_BINJSON_TYPE_FALSE == nType)))
		{
			return CX_Status_InvalidCall;
		}
	}
	if (CX_BINJSON_TYPE_NULL != nType)
	{
		if (CX_BINJSON_TYPE_FALSE == pReader->nCrEntryType)
		{
			*((CX_Bool *)pData) = CX_False;
		}
		else
		if (CX_BINJSON_TYPE_TRUE == pReader->nCrEntryType)
		{
			*((CX_Bool *)pData) = CX_True;
		}
		else
		if (CX_BINJSON_TYPE_STRING == nType)
		{
			if (CXNOK(nStatus = CX_BINJSON_Reader_ReadStringData(pReader, 
			                                                     (CX_BINJSON_Reader_String *)pData)))
			{
				return nStatus;
			}
		}
		else
		if (CX_BINJSON_TYPE_WSTRING == nType)
		{
			if (CXNOK(nStatus = CX_BINJSON_Reader_ReadWStringData(pReader, 
			                                                      (CX_BINJSON_Reader_WString *)pData)))
			{
				return nStatus;
			}
		}
		else
		if (CX_BINJSON_TYPE_BLOB == nType)
		{
			CX_UInt32 cbTmpSize;

			if (CXNOK(nStatus = CX_BINJSON_Reader_ReadEx(pReader, &cbTmpSize, sizeof(cbTmpSize))))
			{
				return nStatus;
			}
			if (NULL == (*ppData = pReader->api.Alloc(pReader->pUserContext, cbTmpSize)))
			{
				return CX_Status_MemAllocFailed;
			}
			if (CXNOK(nStatus = CX_BINJSON_Reader_ReadEx(pReader, *ppData, cbTmpSize)))
			{
				return nStatus;
			}
			*pcbSize = cbTmpSize;
		}
		else
		{
			if (CXNOK(nStatus = CX_BINJSON_Reader_ReadEx(pReader, pData, cbSize)))
			{
				return nStatus;
			}
		}
	}

	return CX_BINJSON_Reader_ReadEntryType(pReader);
}

CX_StatusCode CX_BINJSON_Reader_FreeString(CX_BINJSON_Reader *pReader,
                                           CX_BINJSON_Reader_String *pString)
{
	if (pString->pString != pString->buffer)
	{
		pReader->api.Free(pReader->pUserContext, pString->pString);
		pString->pString = pString->buffer;
	}

	return CX_Status_OK;
}

CX_StatusCode CX_BINJSON_Reader_FreeWString(CX_BINJSON_Reader *pReader,
                                            CX_BINJSON_Reader_WString *pWString)
{
	if (pWString->pWString != pWString->buffer)
	{
		pReader->api.Free(pReader->pUserContext, pWString->pWString);
		pWString->pWString = pWString->buffer;
	}

	return CX_Status_OK;
}

CX_StatusCode CX_BINJSON_Reader_FreeBLOB(CX_BINJSON_Reader *pReader, void *pPtr)
{
	pReader->api.Free(pReader->pUserContext, pPtr);

	return CX_Status_OK;
}

CX_StatusCode CX_BINJSON_Reader_Init(CX_BINJSON_Reader *pReader, void *pUserContext, 
                                     CX_BINJSON_HelperAPI *pHelperAPI, 
                                     CX_BINJSON_Reader_Read_Func pfnRead)
{
	XXH_errorcode ec;
	CX_StatusCode nStatus;

	pReader->cDepth       = 0;
	pReader->pUserContext = pUserContext;
	pReader->pfnRead      = pfnRead;
	pReader->api          = *pHelperAPI;
	if (XXH_OK != (ec = XXH32_reset(&pReader->hash, 0)))
	{
		return CX_Status_OperationFailed;
	}

	if (CXNOK(nStatus = CX_BINJSON_Reader_ReadHeader(pReader)))
	{
		return nStatus;
	}
	if (CXNOK(nStatus = CX_BINJSON_Reader_ReadEntryType(pReader)))
	{
		return nStatus;
	}
	if (CX_BINJSON_TYPE_BEGINROOTOBJECT != pReader->nCrEntryType && 
	    CX_BINJSON_TYPE_BEGINROOTARRAY != pReader->nCrEntryType)
	{
		return CX_Status_InvalidCall;
	}
	else
	{
		pReader->nRootEntryType = pReader->nCrEntryType;
	}

	return CX_Status_OK;
}

CX_BINJSON_EntryType CX_BINJSON_Reader_GetRootEntryType(CX_BINJSON_Reader *pReader)
{
	if (CX_BINJSON_TYPE_BEGINROOTOBJECT == pReader->nRootEntryType)
	{
		return CX_BINJSON_EntryType_Object;
	}
	else
	if (CX_BINJSON_TYPE_BEGINROOTARRAY == pReader->nRootEntryType)
	{
		return CX_BINJSON_EntryType_Array;
	}
	else
	{
		return CX_BINJSON_EntryType_Invalid;
	}
}

CX_StatusCode CX_BINJSON_Reader_BeginRootObject(CX_BINJSON_Reader *pReader)
{
	if (0 < pReader->cDepth)
	{
		return CX_Status_InvalidCall;
	}
	if (CX_BINJSON_TYPE_BEGINROOTOBJECT != pReader->nRootEntryType)
	{
		return CX_Status_NotInitialized;
	}
	pReader->stack[0] = CX_BINJSON_TYPE_BEGINROOTOBJECT;
	pReader->cDepth   = 1;

	return CX_BINJSON_Reader_ReadEntryType(pReader);
}

CX_StatusCode CX_BINJSON_Reader_EndRootObject(CX_BINJSON_Reader *pReader)
{
	if (0 == pReader->cDepth)
	{
		return CX_Status_InvalidCall;
	}
	if (CX_BINJSON_TYPE_BEGINROOTOBJECT != pReader->stack[pReader->cDepth - 1])
	{
		return CX_Status_InvalidCall;
	}
	pReader->cDepth--;
	if (0 != pReader->cDepth)
	{
		return CX_Status_InvalidCall;
	}

	CX_UInt32     nHashInFile;
	CX_UInt32     nHashComputed;
	CX_StatusCode nStatus;

	if (CXNOK(nStatus = CX_BINJSON_Reader_ReadFooter(pReader, &nHashInFile)))
	{
		return nStatus;
	}
	nHashComputed = XXH32_digest(&pReader->hash);
	if (nHashComputed != nHashInFile)
	{
		return CX_Status_OperationFailed;
	}

	return CX_Status_OK;
}

CX_StatusCode CX_BINJSON_Reader_BeginRootArray(CX_BINJSON_Reader *pReader)
{
	if (0 < pReader->cDepth)
	{
		return CX_Status_InvalidCall;
	}
	if (CX_BINJSON_TYPE_BEGINROOTARRAY != pReader->nRootEntryType)
	{
		return CX_Status_NotInitialized;
	}
	pReader->stack[0] = CX_BINJSON_TYPE_BEGINROOTARRAY;
	pReader->cDepth = 1;

	return CX_BINJSON_Reader_ReadEntryType(pReader);
}

CX_StatusCode CX_BINJSON_Reader_EndRootArray(CX_BINJSON_Reader *pReader)
{
	if (0 == pReader->cDepth)
	{
		return CX_Status_InvalidCall;
	}
	if (CX_BINJSON_TYPE_BEGINROOTARRAY != pReader->stack[pReader->cDepth - 1])
	{
		return CX_Status_InvalidCall;
	}
	pReader->cDepth--;
	if (0 != pReader->cDepth)
	{
		return CX_Status_InvalidCall;
	}

	CX_UInt32     nHashInFile;
	CX_UInt32     nHashComputed;
	CX_StatusCode nStatus;

	if (CXNOK(nStatus = CX_BINJSON_Reader_ReadFooter(pReader, &nHashInFile)))
	{
		return nStatus;
	}
	nHashComputed = XXH32_digest(&pReader->hash);
	if (nHashComputed != nHashInFile)
	{
		return CX_Status_OperationFailed;
	}

	return CX_Status_OK;
}

CX_BINJSON_EntryType CX_BINJSON_Reader_GetEntryType(CX_BINJSON_Reader *pReader)
{
	if (CX_BINJSON_TYPE_BEGINROOTOBJECT == pReader->nCrEntryType)
	{
		return CX_BINJSON_EntryType_Object;
	}
	else
	if (CX_BINJSON_TYPE_BEGINROOTARRAY == pReader->nCrEntryType)
	{
		return CX_BINJSON_EntryType_Array;
	}
	else
	if (CX_BINJSON_TYPE_BEGINOBJECT == pReader->nCrEntryType)
	{
		return CX_BINJSON_EntryType_Object;
	}
	else
	if (CX_BINJSON_TYPE_BEGINARRAY == pReader->nCrEntryType)
	{
		return CX_BINJSON_EntryType_Array;
	}
	else
	if (CX_BINJSON_TYPE_NULL == pReader->nCrEntryType)
	{
		return CX_BINJSON_EntryType_Null;
	}
	else
	if (CX_BINJSON_TYPE_FALSE == pReader->nCrEntryType)
	{
		return CX_BINJSON_EntryType_Bool;
	}
	else
	if (CX_BINJSON_TYPE_TRUE == pReader->nCrEntryType)
	{
		return CX_BINJSON_EntryType_Bool;
	}
	else
	if (CX_BINJSON_TYPE_INT == pReader->nCrEntryType)
	{
		return CX_BINJSON_EntryType_Int;
	}
	else
	if (CX_BINJSON_TYPE_REAL == pReader->nCrEntryType)
	{
		return CX_BINJSON_EntryType_Real;
	}
	else
	if (CX_BINJSON_TYPE_STRING == pReader->nCrEntryType)
	{
		return CX_BINJSON_EntryType_String;
	}
	else
	if (CX_BINJSON_TYPE_WSTRING == pReader->nCrEntryType)
	{
		return CX_BINJSON_EntryType_WString;
	}
	else
	if (CX_BINJSON_TYPE_BLOB == pReader->nCrEntryType)
	{
		return CX_BINJSON_EntryType_BLOB;
	}
	else
	if (CX_BINJSON_TYPE_ENDROOTOBJECT == pReader->nCrEntryType)
	{
		return CX_BINJSON_EntryType_EOG;
	}
	else
	if (CX_BINJSON_TYPE_ENDROOTARRAY == pReader->nCrEntryType)
	{
		return CX_BINJSON_EntryType_EOG;
	}
	else
	if (CX_BINJSON_TYPE_ENDOBJECT == pReader->nCrEntryType)
	{
		return CX_BINJSON_EntryType_EOG;
	}
	else
	if (CX_BINJSON_TYPE_ENDARRAY == pReader->nCrEntryType)
	{
		return CX_BINJSON_EntryType_EOG;
	}
	else
	{
		return CX_BINJSON_EntryType_Invalid;
	}
}

//object member - will return Status_OutOfBounds at the end of the object
CX_StatusCode CX_BINJSON_Reader_ObjReadNull(CX_BINJSON_Reader *pReader, 
                                            CX_BINJSON_Reader_String *psName)
{
	return CX_BINJSON_Reader_ReadObjectEntry(pReader, psName, CX_BINJSON_TYPE_NULL, NULL, 0, NULL, 
	                                         NULL);
}

//array item - will return Status_OutOfBounds at the end of the array
CX_StatusCode CX_BINJSON_Reader_ArrReadNull(CX_BINJSON_Reader *pReader)
{
	return CX_BINJSON_Reader_ReadArrayEntry(pReader, CX_BINJSON_TYPE_NULL, NULL, 0, NULL, NULL);
}

//object member - will return Status_OutOfBounds at the end of the object
CX_StatusCode CX_BINJSON_Reader_ObjReadBool(CX_BINJSON_Reader *pReader, 
                                            CX_BINJSON_Reader_String *psName, CX_Bool *pbValue)
{
	return CX_BINJSON_Reader_ReadObjectEntry(pReader, psName, CX_BINJSON_TYPE_FALSE, pbValue, 
	                                         sizeof(*pbValue), NULL, NULL);
}

//array item - will return Status_OutOfBounds at the end of the array
CX_StatusCode CX_BINJSON_Reader_ArrReadBool(CX_BINJSON_Reader *pReader, CX_Bool *pbValue)
{
	return CX_BINJSON_Reader_ReadArrayEntry(pReader, CX_BINJSON_TYPE_FALSE, pbValue, 
	                                        sizeof(*pbValue), NULL, NULL);
}

//object member - will return Status_OutOfBounds at the end of the object
CX_StatusCode CX_BINJSON_Reader_ObjReadInt(CX_BINJSON_Reader *pReader, 
                                           CX_BINJSON_Reader_String *psName, CX_Int64 *pnValue)
{
	return CX_BINJSON_Reader_ReadObjectEntry(pReader, psName, CX_BINJSON_TYPE_INT, pnValue, 
	                                         sizeof(*pnValue), NULL, NULL);
}

//array item - will return Status_OutOfBounds at the end of the array
CX_StatusCode CX_BINJSON_Reader_ArrReadInt(CX_BINJSON_Reader *pReader, CX_Int64 *pnValue)
{
	return CX_BINJSON_Reader_ReadArrayEntry(pReader, CX_BINJSON_TYPE_INT, pnValue, 
	                                        sizeof(*pnValue), NULL, NULL);
}

//object member - will return Status_OutOfBounds at the end of the object
CX_StatusCode CX_BINJSON_Reader_ObjReadReal(CX_BINJSON_Reader *pReader, 
                                            CX_BINJSON_Reader_String *psName, CX_Double *plfValue)
{
	return CX_BINJSON_Reader_ReadObjectEntry(pReader, psName, CX_BINJSON_TYPE_REAL, plfValue, 
	                                         sizeof(*plfValue), NULL, NULL);
}

//array item - will return Status_OutOfBounds at the end of the array
CX_StatusCode CX_BINJSON_Reader_ArrReadReal(CX_BINJSON_Reader *pReader, CX_Double *plfValue)
{
	return CX_BINJSON_Reader_ReadArrayEntry(pReader, CX_BINJSON_TYPE_REAL, plfValue, 
	                                        sizeof(*plfValue), NULL, NULL);
}

//object member - will return Status_OutOfBounds at the end of the object
CX_StatusCode CX_BINJSON_Reader_ObjReadString(CX_BINJSON_Reader *pReader, 
                                              CX_BINJSON_Reader_String *psName, 
                                              CX_BINJSON_Reader_String *psValue)
{
	return CX_BINJSON_Reader_ReadObjectEntry(pReader, psName, CX_BINJSON_TYPE_STRING, psValue, 0, 
	                                         NULL, NULL);
}

//array item - will return Status_OutOfBounds at the end of the array
CX_StatusCode CX_BINJSON_Reader_ArrReadString(CX_BINJSON_Reader *pReader, 
                                           CX_BINJSON_Reader_String *psValue)
{
	return CX_BINJSON_Reader_ReadArrayEntry(pReader, CX_BINJSON_TYPE_STRING, psValue, 0, NULL, 
	                                        NULL);
}

//object member - will return Status_OutOfBounds at the end of the object
CX_StatusCode CX_BINJSON_Reader_ObjReadWString(CX_BINJSON_Reader *pReader, 
                                               CX_BINJSON_Reader_String *psName, 
                                               CX_BINJSON_Reader_WString *pwsValue)
{
	return CX_BINJSON_Reader_ReadObjectEntry(pReader, psName, CX_BINJSON_TYPE_WSTRING, pwsValue, 0, 
	                                         NULL, NULL);
}

//array item - will return Status_OutOfBounds at the end of the array
CX_StatusCode CX_BINJSON_Reader_ArrReadWString(CX_BINJSON_Reader *pReader, 
                                               CX_BINJSON_Reader_WString *pwsValue)
{
	return CX_BINJSON_Reader_ReadArrayEntry(pReader, CX_BINJSON_TYPE_WSTRING, pwsValue, 0,
	                                        NULL, NULL);
}

//object member - will return Status_OutOfBounds at the end of the object; free using CX::Free
CX_StatusCode CX_BINJSON_Reader_ObjReadBLOB(CX_BINJSON_Reader *pReader, 
                                            CX_BINJSON_Reader_String *psName, 
                                            void **ppData, CX_Size *pcbSize)
{
	return CX_BINJSON_Reader_ReadObjectEntry(pReader, psName, CX_BINJSON_TYPE_BLOB, NULL, 0, 
	                                         ppData, pcbSize);
}

//array item - will return Status_OutOfBounds at the end of the array free using CX::Free
CX_StatusCode CX_BINJSON_Reader_ArrReadBLOB(CX_BINJSON_Reader *pReader, 
                                            void **ppData, CX_Size *pcbSize)
{
	return CX_BINJSON_Reader_ReadArrayEntry(pReader, CX_BINJSON_TYPE_BLOB, NULL, 0, 
	                                        ppData, pcbSize);
}

//object member
CX_StatusCode CX_BINJSON_Reader_ObjBeginObject(CX_BINJSON_Reader *pReader, 
                                               CX_BINJSON_Reader_String *psName)
{
	CX_StatusCode nStatus;

	if (0 == pReader->cDepth)
	{
		return CX_Status_InvalidCall;
	}
	if (CX_BINJSON_TYPE_BEGINROOTOBJECT != pReader->stack[pReader->cDepth - 1] &&
	    CX_BINJSON_TYPE_BEGINOBJECT != pReader->stack[pReader->cDepth - 1])
	{
		return CX_Status_InvalidCall;
	}
	if (CX_BINJSON_TYPE_BEGINOBJECT != pReader->nCrEntryType)
	{
		return CX_Status_InvalidCall;
	}
	if (CXNOK(nStatus = CX_BINJSON_Reader_ReadStringData(pReader, psName)))
	{
		return nStatus;
	}
	if (pReader->cDepth + 1 > CX_BINJSON_READER_MAX_DEPTH)
	{
		return CX_Status_OutOfBounds;
	}
	pReader->stack[pReader->cDepth] = CX_BINJSON_TYPE_BEGINOBJECT;
	pReader->cDepth++;

	return CX_BINJSON_Reader_ReadEntryType(pReader);
}

//array item
CX_StatusCode CX_BINJSON_Reader_ArrBeginObject(CX_BINJSON_Reader *pReader)
{
	if (0 == pReader->cDepth)
	{
		return CX_Status_InvalidCall;
	}
	if (CX_BINJSON_TYPE_BEGINROOTARRAY != pReader->stack[pReader->cDepth - 1] &&
	    CX_BINJSON_TYPE_BEGINARRAY != pReader->stack[pReader->cDepth - 1])
	{
		return CX_Status_InvalidCall;
	}
	if (CX_BINJSON_TYPE_BEGINOBJECT != pReader->nCrEntryType)
	{
		return CX_Status_InvalidCall;
	}
	if (pReader->cDepth + 1 > CX_BINJSON_READER_MAX_DEPTH)
	{
		return CX_Status_OutOfBounds;
	}
	pReader->stack[pReader->cDepth] = CX_BINJSON_TYPE_BEGINOBJECT;
	pReader->cDepth++;

	return CX_BINJSON_Reader_ReadEntryType(pReader);
}

//object member
CX_StatusCode CX_BINJSON_Reader_ObjBeginArray(CX_BINJSON_Reader *pReader, 
                                              CX_BINJSON_Reader_String *psName)
{
	CX_StatusCode nStatus;

	if (0 == pReader->cDepth)
	{
		return CX_Status_InvalidCall;
	}
	if (CX_BINJSON_TYPE_BEGINROOTOBJECT != pReader->stack[pReader->cDepth - 1] &&
	    CX_BINJSON_TYPE_BEGINOBJECT != pReader->stack[pReader->cDepth - 1])
	{
		return CX_Status_InvalidCall;
	}
	if (CX_BINJSON_TYPE_BEGINARRAY != pReader->nCrEntryType)
	{
		return CX_Status_InvalidCall;
	}
	if (CXNOK(nStatus = CX_BINJSON_Reader_ReadStringData(pReader, psName)))
	{
		return nStatus;
	}
	if (pReader->cDepth + 1 > CX_BINJSON_READER_MAX_DEPTH)
	{
		return CX_Status_OutOfBounds;
	}
	pReader->stack[pReader->cDepth] = CX_BINJSON_TYPE_BEGINARRAY;
	pReader->cDepth++;

	return CX_BINJSON_Reader_ReadEntryType(pReader);
}

//array item
CX_StatusCode CX_BINJSON_Reader_ArrBeginArray(CX_BINJSON_Reader *pReader)
{
	if (0 == pReader->cDepth)
	{
		return CX_Status_InvalidCall;
	}
	if (CX_BINJSON_TYPE_BEGINROOTARRAY != pReader->stack[pReader->cDepth - 1] &&
	    CX_BINJSON_TYPE_BEGINARRAY != pReader->stack[pReader->cDepth - 1])
	{
		return CX_Status_InvalidCall;
	}
	if (CX_BINJSON_TYPE_BEGINARRAY != pReader->nCrEntryType)
	{
		return CX_Status_InvalidCall;
	}
	if (pReader->cDepth + 1 > CX_BINJSON_READER_MAX_DEPTH)
	{
		return CX_Status_OutOfBounds;
	}
	pReader->stack[pReader->cDepth] = CX_BINJSON_TYPE_BEGINARRAY;
	pReader->cDepth++;

	return CX_BINJSON_Reader_ReadEntryType(pReader);
}

CX_StatusCode CX_BINJSON_Reader_EndObject(CX_BINJSON_Reader *pReader)
{
	if (0 == pReader->cDepth)
	{
		return CX_Status_InvalidCall;
	}
	if (CX_BINJSON_TYPE_BEGINOBJECT != pReader->stack[pReader->cDepth - 1] )
	{
		return CX_Status_InvalidCall;
	}
	pReader->cDepth--;

	return CX_BINJSON_Reader_ReadEntryType(pReader);
}

CX_StatusCode CX_BINJSON_Reader_EndArray(CX_BINJSON_Reader *pReader)
{
	if (0 == pReader->cDepth)
	{
		return CX_Status_InvalidCall;
	}
	if (CX_BINJSON_TYPE_BEGINARRAY != pReader->stack[pReader->cDepth - 1])
	{
		return CX_Status_InvalidCall;
	}
	pReader->cDepth--;

	return CX_BINJSON_Reader_ReadEntryType(pReader);

}
