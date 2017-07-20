/* 
 * The author disclaims copyright to this source code.
 */ 

#pragma once


#include "CX/Types.hpp"
#include <stddef.h>


namespace CX
{

class IInterfaceList
{
public:

	virtual ~IInterfaceList() { }

	virtual StatusCode AddInterface(const Char *szInterface) = 0;

	virtual Bool HasInterface(const Char *szInterface) const = 0;

	virtual StatusCode RemoveInterface(const Char *szInterface) = 0;

	virtual StatusCode RemoveInterfaces() = 0;

	virtual Size GetInterfacesCount() const = 0;

	virtual StatusCode GetInterface(Size cIndex, const Char **pszInterface) const = 0;

};

}//namespace CX
