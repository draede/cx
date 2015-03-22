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


#include "CX/IO/SimpleBuffers/Types.hpp"
#include "CX/IO/MemInputStream.hpp" 
#include "CX/IO/MemOutputStream.hpp"
#include "CX/Data/JSON/DataReader.hpp"
#include "CX/Data/JSON/DataWriter.hpp"
#include "CX/String.hpp"
#include "CX/Print.hpp"
#include "CX/C/string.h"


namespace CX
{

namespace IO
{

namespace SimpleBuffers
{

class ObjectTester
{
public:

	template <typename ObjectType, 
	          typename DataReaderType = Data::JSON::DataReader, 
	          typename DataWriterType = Data::JSON::DataWriter>
	static bool Test(String *psDetail)
	{
		ObjectType       obj1;
		ObjectType       obj2;
		String           sOutput1;
		String           sOutput2;
		Status           status;

		psDetail->clear();

		//write 1
		{
			MemOutputStream  mos(&sOutput1);
			DataWriterType   writer(&mos);

			if ((status = writer.Begin()).IsNOK())
			{
				Print(psDetail, "writer.Begin1 failed with code {1} and msg \"{2}\"", status.GetCode(), status.GetMsg());

				return false;
			}
			if ((status = obj1.Write(&writer)).IsNOK())
			{
				Print(psDetail, "Write1 failed with code {1} and msg \"{2}\"", status.GetCode(), status.GetMsg());

				return false;
			}
			if ((status = writer.End()).IsNOK())
			{
				Print(psDetail, "writer.End1 failed with code {1} and msg \"{2}\"", status.GetCode(), status.GetMsg());

				return false;
			}
		}

		{
			MemInputStream   mis(&sOutput1);
			DataReaderType   reader(&mis);

			if ((status = reader.Begin()).IsNOK())
			{
				Print(psDetail, "reader.Begin1 failed with code {1} and msg \"{2}\"", status.GetCode(), status.GetMsg());

				return false;
			}
			if ((status = obj2.Read(&reader)).IsNOK())
			{
				Print(psDetail, "Read1 failed with code {1} and msg \"{2}\"", status.GetCode(), status.GetMsg());

				return false;
			}
			if ((status = reader.End()).IsNOK())
			{
				Print(psDetail, "reader.End1 failed with code {1} and msg \"{2}\"", status.GetCode(), status.GetMsg());

				return false;
			}
		}

		//write2
		{
			MemOutputStream  mos(&sOutput2);
			DataWriterType   writer(&mos);

			if ((status = writer.Begin()).IsNOK())
			{
				Print(psDetail, "writer.Begin2 failed with code {1} and msg \"{2}\"", status.GetCode(), status.GetMsg());

				return false;
			}
			if ((status = obj2.Write(&writer)).IsNOK())
			{
				Print(psDetail, "Write2 failed with code {1} and msg \"{2}\"", status.GetCode(), status.GetMsg());

				return false;
			}
			if ((status = writer.End()).IsNOK())
			{
				Print(psDetail, "writer.End2 failed with code {1} and msg \"{2}\"", status.GetCode(), status.GetMsg());

				return false;
			}
		}

		if (!obj1.Compare(&obj2))
		{
			Print(psDetail, "Objects are different");

			return false;
		}

		if (sOutput1.size() != sOutput2.size())
		{
			Print(psDetail, "Object output sizes are different ({1} != {2})", sOutput1.size(), sOutput2.size());

			return false;
		}

		if (0 != cx_strcmp(sOutput1.c_str(), sOutput2.c_str()))
		{
			Print(psDetail, "Object outputs are different");

			return false;
		}

		return true;
	}

private:

};

}//namespace SimpleBuffers

}//namespace SB

}//namespace CX

