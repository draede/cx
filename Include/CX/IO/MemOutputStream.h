
#pragma once


#include "CX/IO/IOutputStream.h"
#include "CX/String.h"
#include "CX/Util/MemPool.h"


namespace CX
{

namespace IO
{

class MemOutputStream : public IOutputStream
{
public:

	MemOutputStream(void *pMem, Size cbSize);

	MemOutputStream(String *pStr);

	MemOutputStream(Util::IMemPool *pMemPool);

	~MemOutputStream();

	virtual StatusCode Write(const void *pBuffer, Size cbReqSize, Size *pcbAckSize);

	virtual StatusCode GetSize(UInt64 *pcbSize) const;

	virtual bool IsOK() const;

	virtual const Char *GetPath() const;

private:

	enum Type
	{
		Type_Mem,
		Type_String,
		Type_MemPool,
	};

	Type				m_nType;
	void				*m_pMem;
	Size				m_cbMemSize;
	String			*m_pStr;
	Util::IMemPool *m_pMemPool;
	Size				m_cbOffset;

	Size GetSizeImpl() const;

	StatusCode WriteImpl(const void *pMem, Size cbReqSize, Size *pcbAckSize);

};

}//namespace IO

}//namespace CX

