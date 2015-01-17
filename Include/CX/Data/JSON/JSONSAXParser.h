
#pragma once


#include "CX/IO/IInputStream.h"
#include "CX/Data/JSON/IJSONSAXParserObserver.h"
#include "CX/Vector.h"
#include "CX/String.h"
#include "CX/Util/IFunction.h"
#include "CX/APIDefs.h"


struct CX_Data_JSON_SAX_Handler;


namespace CX
{

namespace Data
{

namespace JSON
{

class CX_API JSONSAXParser
{
public:

	JSONSAXParser();

	~JSONSAXParser();

	StatusCode ParseStream(IO::IInputStream *pInputStream);

	StatusCode ParseBuffer(const void *pBuffer, Size cbSize);

	StatusCode ParseString(const Char *szString);

	StatusCode ParseString(const String &sString);

	StatusCode AddObserver(IJSONSAXParserObserver *pObserver);

	StatusCode RemoveObservers();

private:

	typedef Vector<IJSONSAXParserObserver *>::Type   ObserversVector;

#pragma warning(push)
#pragma warning(disable: 4251)
	ObserversVector   m_vectorObservers;
#pragma warning(push)

	CX_Data_JSON_SAX_Handler *m_pHandler;

};

}//namespace JSON

}//namespace Data

}//namespace CX
