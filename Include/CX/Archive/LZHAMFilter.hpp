
#pragma once


#include "CX/IO/IFilter.hpp"
#include "CX/IObject.hpp"


namespace CX
{

namespace Archive
{

class LZHAMFilter : public IO::IFilter, public IObject
{
public:

	enum Dir
	{
		Dir_Compress,
		Dir_Uncompress,
	};

	static const int DEF_DICT_SIZE = 23;

	enum Level
	{
		Level_0 = (int)0,
		Level_1 = (int)1,
		Level_2 = (int)2,
		Level_3 = (int)3,
		Level_4 = (int)4,
		Level_5 = (int)5,
		Level_6 = (int)6,
		Level_7 = (int)7,
		Level_8 = (int)8,
		Level_9 = (int)9,
		Level_10 = (int)10,

		Level_No_Compression   = Level_0,
		Level_Best_Speed       = Level_1,
		Level_Best_Compression = Level_9,
		Level_Uber_Compression = Level_10, //very slow
		Level_Default          = Level_9,
	};

	enum Strategy
	{
		Strategy_Default     = (int)0,
		Strategy_Filtered    = (int)1,
		Strategy_HuffmanOnly = (int)2,
		Strategy_RLE         = (int)3,
		Strategy_Fixed       = (int)4
	};

	enum MemLevel
	{
		MemLevel_1 = (int)1,
		MemLevel_2 = (int)2,
		MemLevel_3 = (int)3,
		MemLevel_4 = (int)4,
		MemLevel_5 = (int)5,
		MemLevel_6 = (int)6,
		MemLevel_7 = (int)7,
		MemLevel_8 = (int)8,
		MemLevel_9 = (int)9,

		MemLevel_Default = MemLevel_9,
	};

	LZHAMFilter(Dir nDir = Dir_Compress, int nDictSize = DEF_DICT_SIZE, Level nLevel = Level_Default, 
	            MemLevel nMemLevel = MemLevel_Default, Strategy nStrategy = Strategy_Default);

	~LZHAMFilter();

	virtual Status Filter(Buffers *pBuffers);

private:

	void *m_pStream;
	Dir  m_nDir;

};

}//namespace Archive

}//namespace CX
