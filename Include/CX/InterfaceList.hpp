/* 
 * The author disclaims copyright to this source code.
 */ 

#pragma once


#include "CX/Types.hpp"
#include "CX/C/StatusCodes.h"
#include "CX/Set.hpp"
#include "CX/String.hpp"
#include "CX/IInterfaceList.hpp"


namespace CX
{

class InterfaceList : public IInterfaceList
{
public:

	InterfaceList()
	{
	}

	~InterfaceList()
	{
	}

	virtual StatusCode AddInterface(const Char *szInterface)
	{
		m_setInterfaces.insert(szInterface);

		return Status_OK;
	}

	virtual Bool HasInterface(const Char *szInterface) const
	{
		return (m_setInterfaces.end() != m_setInterfaces.find(szInterface));
	}

	virtual StatusCode RemoveInterface(const Char *szInterface)
	{
		StringsSet::iterator iter = m_setInterfaces.find(szInterface);

		if (m_setInterfaces.end() == iter)
		{
			return Status_NotFound;
		}
		m_setInterfaces.erase(iter);

		return Status_OK;
	}

	virtual StatusCode RemoveInterfaces()
	{
		m_setInterfaces.clear();

		return Status_OK;
	}

	virtual Size GetInterfacesCount() const
	{
		return m_setInterfaces.size();
	}

	virtual StatusCode GetInterface(Size cIndex, const Char **pszInterface) const
	{
		StringsSet::const_iterator iter = m_setInterfaces.begin();

		while (m_setInterfaces.end() != iter && 0 < cIndex)
		{
			iter++;
			cIndex--;
		}
		if (m_setInterfaces.end() == iter)
		{
			return false;
		}
		*pszInterface = iter->c_str();

		return Status_OK;
	}

private:

	typedef CX::Set<CX::String>::Type   StringsSet;

	StringsSet   m_setInterfaces;

};

}//namespace CX
