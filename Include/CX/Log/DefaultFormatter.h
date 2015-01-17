
#pragma once


#include "CX/Types.h"
#include "CX/Log/IFormatter.h"
#include "CX/APIDefs.h"


namespace CX
{

namespace Log
{

class CX_API DefaultFormatter : public IFormatter
{
public:

	enum Flag
	{
		Show_Date    = 1,
		Show_Time    = 2,
		Show_Level   = 4,
		Show_Tag     = 8,
		Show_NewLine = 16,
		Show_TID     = 32,
		Show_PID     = 64,
	};

	static const int FLAGS_MINIMAL = Show_Level | Show_Tag | Show_NewLine;
	static const int FLAGS_MEDIUM  = Show_Date | Show_Time | Show_Level | Show_Tag | 
	                                 Show_NewLine;
	static const int FLAGS_FULL    = Show_Date | Show_Time | Show_Level | Show_Tag | 
	                                 Show_NewLine | Show_TID | Show_PID;

	DefaultFormatter(int nFlags = FLAGS_MEDIUM);

	~DefaultFormatter();

	void SetFlags(int nFlags = FLAGS_MEDIUM);

	int GetFlags() const;

	virtual StatusCode Write(IOutput *pOutput, Level nLevel, const Char *szTag, 
	                         const Char *pBuffer, Size cLen);

private:

	int m_nFlags;

};

}//namespace Log

}//namespace CX

