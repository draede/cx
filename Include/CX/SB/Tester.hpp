/* 
 * CX - C++ framework for general purpose developement
 *
 * https://github.com/draede/cx
 * 
 * Copyright (C) 2014-2015 draede - draede [at] outlook [dot] com
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


#include "CX/IO/MemInputStream.hpp"
#include "CX/IO/MemOutputStream.hpp"
#include "CX/IO/FileInputStream.hpp"
#include "CX/IO/FileOutputStream.hpp"
#include "CX/SB/BinaryDataReader.hpp"
#include "CX/SB/BinaryDataWriter.hpp"
#include "CX/SB/StatsDataReader.hpp"
#include "CX/SB/StatsDataWriter.hpp"


namespace CX
{

namespace SB
{

template <typename T>
Status TestSerialize(StatsData &readdata, StatsData &writedata)
{
	T                *pP1 = New<T>();
	T                *pP2 = New<T>();
	String           s1;
	Status           status;

	TestInit(*pP1);

	{
		IO::MemOutputStream os(&s1);
		//IO::FileOutputStream os("sb_test.out");
		BinaryDataWriter    w(&os);
		StatsDataWriter     sw(&w);

		if ((status = Write(*pP1, &sw)).IsNOK())
		{
			Delete(pP1);
			Delete(pP2);

			return status;
		}
		writedata = sw.m_data;
	}
	{
		IO::MemInputStream is(&s1);
		//IO::FileInputStream is("sb_test.out");
		BinaryDataReader   r(&is);
		StatsDataReader    sr(&r);

		if ((status = Read(*pP2, &sr)).IsNOK())
		{
			Delete(pP1);
			Delete(pP2);

			return status;
		}
		readdata = sr.m_data;
	}
	if (0 != Compare(*pP1, *pP2))
	{
		Delete(pP1);
		Delete(pP2);

		return Status(Status_OperationFailed, "Objects are different");
	}
	Delete(pP1);
	Delete(pP2);

	return Status();
}

}//namespace SB

}//namespace CX

