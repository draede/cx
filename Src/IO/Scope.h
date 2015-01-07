
#pragma once


namespace CX
{

template <typename T, void (T::*pfnEnter)() = &T::Enter, void (T::*pfnLeave)() = &T::Leave>
class Scope
{
public:

	Scope(T *pInst);
	{
		m_pInst = pInst;
		(m_pInst->*pfnEnter)();
	}

	virtual ~Scope()
	{
		(m_pInst->*pfnLeave)();
	}

private:

	T *m_pInst;

};

}//namespace CX

