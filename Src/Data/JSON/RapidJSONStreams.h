
#pragma once


#include "CX/IO/IInputStream.h"
#include "../../../Contrib/RapidJSON/Include/rapidjson.h"


namespace CX
{

namespace Data
{

namespace JSON
{

struct RapidJSONBufferInputStream
{
	typedef char Ch;

	RapidJSONBufferInputStream(const char *src, size_t cCount) : src_(src), head_(src), cCount_(cCount)
	{
	}

	char Peek() 
	{ 
		if (0 == cCount_) 
		{ 
			return '\0'; 
		} 
		else 
		{ 
			return *src_; 
		} 
	}

	char Take() 
	{ 
		if (0 == cCount_) 
		{ 
			return '\0'; 
		} 
		else
		{ 
			cCount_--; 
			return *src_++; 
		} 
	}

	size_t Tell() const 
	{ 
		return src_ - head_; 
	}

	char *PutBegin() 
	{ 
		RAPIDJSON_ASSERT(false); 
		
		return 0; 
	}

	void Put(char) 
	{ 
		RAPIDJSON_ASSERT(false); 
	}

	size_t PutEnd(char*) 
	{ 
		RAPIDJSON_ASSERT(false); 
		
		return 0; 
	}

	const char	*src_;
	const char	*head_;
	size_t		cCount_;

};

struct RapidJSONStringInputStream
{
	typedef char Ch;

	RapidJSONStringInputStream(const char *src) : src_(src), head_(src)
	{
	}

	char Peek() 
	{ 
		return *src_; 
	}

	char Take() 
	{ 
		if ('\0' == *src_)
		{
			return '\0';
		}
		else
		{
			return *src_++; 
		}
	}

	size_t Tell() const 
	{ 
		return src_ - head_; 
	}

	char *PutBegin() 
	{ 
		RAPIDJSON_ASSERT(false); 
		
		return 0; 
	}

	void Put(char) 
	{ 
		RAPIDJSON_ASSERT(false); 
	}

	size_t PutEnd(char*) 
	{ 
		RAPIDJSON_ASSERT(false); 
		
		return 0; 
	}

	const char	*src_;
	const char	*head_;

};

struct RapidJSONInputStream
{
	typedef char Ch;

	RapidJSONInputStream(IO::IInputStream *pInputStream) 
		: inpstr_(pInputStream), offset_(0), bufoffset_(0), bufsize_(0)
	{
	}

	char Peek()
	{
		if (bufoffset_ >= bufsize_)
		{
			if (CXNOK(inpstr_->Read(buf_, sizeof(buf_), &bufsize_)))
			{
				return '\0';
			}
			bufoffset_ = 0;
		}

		return *(buf_ + bufoffset_);
	}

	char Take()
	{
		if (bufoffset_ >= bufsize_)
		{
			if (CXNOK(inpstr_->Read(buf_, sizeof(buf_), &bufsize_)))
			{
				return '\0';
			}
		}
		bufoffset_++;
		offset_++;

		return *(buf_ + bufoffset_ - 1);
	}

	size_t Tell() const
	{
		return offset_;
	}

	char *PutBegin()
	{
		RAPIDJSON_ASSERT(false);

		return 0;
	}

	void Put(char)
	{
		RAPIDJSON_ASSERT(false);
	}

	size_t PutEnd(char*)
	{
		RAPIDJSON_ASSERT(false);

		return 0;
	}

	static const Size BUFFER_SIZE = 16384;


	IO::IInputStream *inpstr_;
	char             buf_[BUFFER_SIZE];
	Size             bufsize_;
	Size             bufoffset_;
	Size             offset_;

};

}//namespace JSON

}//namespace Data

}//namespace CX

