
#pragma once


#include "CX/IO/IInputStream.h"
#include "CX/String.h"
#include "CX/Util/MemPool.h"
#include "CX/APIDefs.h"


namespace CX
{

namespace IO
{

class CX_API MemInputStream : public IInputStream
{
public:

	MemInputStream(const void *pMem, Size cbSize);

	MemInputStream(const String *pStr);

	MemInputStream(const Util::IMemPool *pMemPool);

	~MemInputStream();

	virtual StatusCode Read(void *pBuffer, Size cbReqSize, Size *pcbAckSize);

	virtual StatusCode SetPos(UInt64 cbPos);

	virtual StatusCode GetPos(UInt64 *pcbPos) const;

	virtual StatusCode GetSize(UInt64 *pcbSize) const;

	virtual bool IsEOF() const;

	virtual bool IsOK() const;

	virtual const Char *GetPath() const;

private:

	enum Type
	{
		Type_Mem,
		Type_String,
		Type_MemPool,
	};

	Type                  m_nType;
	const void            *m_pMem;
	Size                  m_cbMemSize;
	const String          *m_pStr;
	const Util::IMemPool  *m_pMemPool;
	Size                  m_cbOffset;

	Size GetSizeImpl() const;

	const void *GetMemImpl(Size cbOffset) const;

};

}//namespace IO

}//namespace CX

