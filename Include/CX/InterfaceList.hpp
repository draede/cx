/* 
 * The author disclaims copyright to this source code.
 */ 

#pragma once


#include <set>
#include <string>
#include <stddef.h>
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

	virtual bool AddInterface(const char *szInterface)
	{
		m_setInterfaces.insert(szInterface);

		return true;
	}

	virtual bool HasInterface(const char *szInterface) const
	{
		return (m_setInterfaces.end() != m_setInterfaces.find(szInterface));
	}

	virtual bool RemoveInterface(const char *szInterface)
	{
		StringsSet::iterator iter = m_setInterfaces.find(szInterface);

		if (m_setInterfaces.end() == iter)
		{
			return false;
		}
		m_setInterfaces.erase(iter);

		return true;
	}

	virtual bool RemoveInterfaces()
	{
		m_setInterfaces.clear();

		return true;
	}

	virtual size_t GetInterfacesCount() const
	{
		return m_setInterfaces.size();
	}

	virtual bool GetInterface(size_t cIndex, const char **pszInterface) const
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

		return true;
	}

private:

	typedef std::set<std::string>   StringsSet;

	StringsSet   m_setInterfaces;

};

}//namespace CX
