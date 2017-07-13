/* 
 * The author disclaims copyright to this source code.
 */ 

#pragma once


#include "CX/Types.hpp"


namespace CX
{

class IInterfaceList
{
public:

	virtual ~IInterfaceList() { }

	virtual Bool AddInterface(const Char *szInterface) = 0;

	virtual Bool HasInterface(const Char *szInterface) const = 0;

	virtual Bool RemoveInterface(const Char *szInterface) = 0;

	virtual Bool RemoveInterfaces() = 0;

	virtual Size GetInterfacesCount() const = 0;

	virtual Bool GetInterface(Size cIndex, const Char **pszInterface) const = 0;

};

}//namespace CX
