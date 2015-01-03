
#pragma once


#include "CX/Types.h"


namespace CX
{

typedef struct _Slice
{
	_Slice()
	{
		pBuffer = NULL;
		cbSize  = 0;
	}

	_Slice(void *pBuffer, Size cbSize)
	{
		this->pBuffer = pBuffer;
		this->cbSize  = cbSize;
	}

	void   *pBuffer;
	Size   cbSize;
}Slice;

typedef struct _ConstSlice
{
	_ConstSlice()
	{
		pBuffer = NULL;
		cbSize  = 0;
	}

	_ConstSlice(const void *pBuffer, Size cbSize)
	{
		this->pBuffer = pBuffer;
		this->cbSize  = cbSize;
	}

	const void   *pBuffer;
	Size         cbSize;
}ConstSlice;

}//namespace CX

