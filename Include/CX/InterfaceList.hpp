/* 
 * The author disclaims copyright to this source code.
 */ 

#pragma once


#include "CX/Types.hpp"
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

	virtual Bool AddInterface(const Char *szInterface)
	{
		m_setInterfaces.insert(szInterface);

		return True;
	}

	virtual Bool HasInterface(const Char *szInterface) const
	{
		return (m_setInterfaces.end() != m_setInterfaces.find(szInterface));
	}

	virtual Bool RemoveInterface(const Char *szInterface)
	{
		StringsSet::iterator iter = m_setInterfaces.find(szInterface);

		if (m_setInterfaces.end() == iter)
		{
			return False;
		}
		m_setInterfaces.erase(iter);

		return True;
	}

	virtual Bool RemoveInterfaces()
	{
		m_setInterfaces.clear();

		return True;
	}

	virtual Size GetInterfacesCount() const
	{
		return m_setInterfaces.size();
	}

	virtual Bool GetInterface(Size cIndex, const Char **pszInterface) const
	{
		StringsSet::const_iterator iter = m_setInterfaces.begin();

		while (m_setInterfaces.end() != iter && 0 < cIndex)
		{
			iter++;
			cIndex--;
		}
		if (m_setInterfaces.end() == iter)
		{
			return False;
		}
		*pszInterface = iter->c_str();

		return True;
	}

private:

	typedef Set<String>::Type   StringsSet;

	StringsSet   m_setInterfaces;

};

}//namespace CX
