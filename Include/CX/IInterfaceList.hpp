/* 
 * The author disclaims copyright to this source code.
 */ 

#pragma once


#include <stddef.h>


namespace CX
{

class IInterfaceList
{
public:

	virtual ~IInterfaceList() { }

	virtual bool AddInterface(const char *szInterface) = 0;

	virtual bool HasInterface(const char *szInterface) const = 0;

	virtual bool RemoveInterface(const char *szInterface) = 0;

	virtual bool RemoveInterfaces() = 0;

	virtual size_t GetInterfacesCount() const = 0;

	virtual bool GetInterface(size_t cIndex, const char **pszInterface) const = 0;

};

}//namespace CX
