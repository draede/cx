
#pragma once


#include "CX/IO/IOutputStream.h"
#include "CX/C/stdio.h"
#include "CX/String.h"


namespace CX
{

namespace IO
{

class FileOutputStream : public IOutputStream
{
public:

	FileOutputStream(const Char *szPath);

	~FileOutputStream();

	virtual StatusCode Write(const void *pBuffer, Size cbReqSize, Size *pcbAckSize);

	virtual StatusCode GetSize(UInt64 *pcbSize) const;

	virtual bool IsOK() const;

	virtual const Char *GetPath() const;

private:

	FILE     *m_pFile;
	String   m_sPath;

};

}//namespace IO

}//namespace CX

