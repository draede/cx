/* 
 * CX - C++ framework for general purpose development
 *
 * https://github.com/draede/cx
 * 
 * Copyright (C) 2014 - 2017 draede - draede [at] outlook [dot] com
 *
 * Released under the MIT License.
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */ 

#pragma once


#include "CX/IO/IInputStream.hpp"
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
		RAPIDJSON_ASSERT(False); 
		
		return 0; 
	}

	void Put(char) 
	{ 
		RAPIDJSON_ASSERT(False); 
	}

	size_t PutEnd(char*) 
	{ 
		RAPIDJSON_ASSERT(False); 
		
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
		RAPIDJSON_ASSERT(False); 
		
		return 0; 
	}

	void Put(char) 
	{ 
		RAPIDJSON_ASSERT(False); 
	}

	size_t PutEnd(char*) 
	{ 
		RAPIDJSON_ASSERT(False); 
		
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
			if (inpstr_->Read(buf_, sizeof(buf_), &bufsize_).IsNOK())
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
			if (inpstr_->Read(buf_, sizeof(buf_), &bufsize_).IsNOK())
			{
				return '\0';
			}
			bufoffset_ = 0;
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
		RAPIDJSON_ASSERT(False);

		return 0;
	}

	void Put(char)
	{
		RAPIDJSON_ASSERT(False);
	}

	size_t PutEnd(char*)
	{
		RAPIDJSON_ASSERT(False);

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

