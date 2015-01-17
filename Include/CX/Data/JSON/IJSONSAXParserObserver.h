
#pragma once


#include "CX/Types.h"
#include "CX/APIDefs.h"


namespace CX
{

namespace Data
{

namespace JSON
{

class CX_API IJSONSAXParserObserver
{
public:

	virtual ~IJSONSAXParserObserver() { }

	virtual bool OnBeginParse() = 0;

	virtual bool OnEndParse() = 0;

	virtual bool OnBeginObject() = 0;

	virtual bool OnEndObject() = 0;

	virtual bool OnBeginArray() = 0;

	virtual bool OnEndArray() = 0;

	virtual bool OnKey(const Char *pBuffer, Size cLen) = 0;

	virtual bool OnNullValue() = 0;

	virtual bool OnBoolValue(bool bBool) = 0;

	virtual bool OnIntValue(Int64 nInt) = 0;

	virtual bool OnRealValue(Double lfReal) = 0;

	virtual bool OnStringValue(const Char *pBuffer, Size cLen) = 0;

};

}//namespace JSON

}//namespace Data

}//namespace CX
