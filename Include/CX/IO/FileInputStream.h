
#pragma once


#include "CX/IO/IInputStream.h"
#include "CX/C/stdio.h"
#include "CX/String.h"


namespace CX
{

namespace IO
{

class FileInputStream : public IInputStream
{
public:

	FileInputStream(const Char *szPath);

	~FileInputStream();

	virtual StatusCode Read(void *pBuffer, Size cbReqSize, Size *pcbAckSize);

	virtual StatusCode SetPos(UInt64 cbPos);

	virtual StatusCode GetPos(UInt64 *pcbPos) const;

	virtual StatusCode GetSize(UInt64 *pcbSize) const;

	virtual bool IsEOF() const;

	virtual bool IsOK() const;

	virtual const Char *GetPath() const;

private:

	FILE     *m_pFile;
	String   m_sPath;

};

}//namespace IO

}//namespace CX

