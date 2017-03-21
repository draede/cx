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

#include "Tester.hpp"


#include "CX/IO/FileInputStream.hpp"
#include "CX/IO/FileOutputStream.hpp"
#include "CX/SimpleBuffers/JSONReader.hpp"
#include "CX/SimpleBuffers/DataReader.hpp"
#include "CX/SimpleBuffers/DataWriter.hpp"
#include "CX/SimpleBuffers/DataIniter.hpp"


class Test
{
public:

	CX::IO::IOutputStream *m_pOS;

};


template <typename OUTPUT>
class Custom : public CX::SimpleBuffers::IReader::ICustom
{
public:

	OUTPUT   m_output;
	CX::Size m_cDepth;

	Custom(OUTPUT output)
	{
		m_output = output;
		m_cDepth = 0;
	}
	
	virtual CX::Status OnBeginObject(const CX::Char *szName/* = NULL*/)
	{
		CX::String sIndent(m_cDepth, '\t');

		if (NULL != szName)
		{
			CX::Print(m_output, "{1}\"{2}\":\n", sIndent, szName);
		}
		CX::Print(m_output, "{1}{{\n", sIndent);
		m_cDepth++;

		return CX::Status();
	}

	virtual CX::Status OnEndObject()
	{
		m_cDepth--;

		CX::String sIndent(m_cDepth, '\t');

		CX::Print(m_output, "{1}}\n", sIndent);

		return CX::Status();
	}

	virtual CX::Status OnBeginArray(const CX::Char *szName/* = NULL*/)
	{
		CX::String sIndent(m_cDepth, '\t');

		if (NULL != szName)
		{
			CX::Print(m_output, "{1}\"{2}\":\n", sIndent, szName);
		}
		CX::Print(m_output, "{1}[\n", sIndent);
		m_cDepth++;

		return CX::Status();
	}

	virtual CX::Status OnEndArray()
	{
		m_cDepth--;

		CX::String sIndent(m_cDepth, '\t');

		CX::Print(m_output, "{1}]\n", sIndent);

		return CX::Status();
	}

	virtual CX::Status OnBool(CX::Bool v, const CX::Char *szName/* = NULL*/)
	{
		CX::String sIndent(m_cDepth, '\t');

		if (NULL != szName)
		{
			CX::Print(m_output, "{1}\"{2}\": {3}\n", sIndent, szName, v);
		}
		else
		{
			CX::Print(m_output, "{1}{2}\n", sIndent, v);
		}

		return CX::Status();
	}

	virtual CX::Status OnInt(CX::Int64 v, const CX::Char *szName/* = NULL*/)
	{
		CX::String sIndent(m_cDepth, '\t');

		if (NULL != szName)
		{
			CX::Print(m_output, "{1}\"{2}\": {3}\n", sIndent, szName, v);
		}
		else
		{
			CX::Print(m_output, "{1}{2}\n", sIndent, v);
		}

		return CX::Status();
	}

	virtual CX::Status OnUInt(CX::UInt64 v, const CX::Char *szName/* = NULL*/)
	{
		CX::String sIndent(m_cDepth, '\t');

		if (NULL != szName)
		{
			CX::Print(m_output, "{1}\"{2}\": {3}\n", sIndent, szName, v);
		}
		else
		{
			CX::Print(m_output, "{1}{2}\n", sIndent, v);
		}

		return CX::Status();
	}

	virtual CX::Status OnDouble(CX::Double v, const CX::Char *szName/* = NULL*/)
	{
		CX::String sIndent(m_cDepth, '\t');

		if (NULL != szName)
		{
			CX::Print(m_output, "{1}\"{2}\": {3}\n", sIndent, szName, v);
		}
		else
		{
			CX::Print(m_output, "{1}{2}\n", sIndent, v);
		}

		return CX::Status();
	}

	virtual CX::Status OnString(const CX::Char *v, const CX::Char *szName/* = NULL*/)
	{
		CX::String sIndent(m_cDepth, '\t');

		if (NULL != szName)
		{
			CX::Print(m_output, "{1}\"{2}\": \"{3}\"\n", sIndent, szName, v);
		}
		else
		{
			CX::Print(m_output, "{1}\"{2}\"\n", sIndent, v);
		}

		return CX::Status();
	}

};

namespace CX
{

namespace SimpleBuffers
{

template <>
struct DataWriter<MemberType_Scalar, Test>
{
	static Status Write(IWriter *pWriter, const Test &v, const Char *szName = NULL)
	{
		return Status();
	}
};

template <>
struct DataReader<MemberType_Scalar, Test>
{
	static Status Read(IReader *pReader, Test &v, const Char *szName = NULL)
	{
		Custom<CX::IO::IOutputStream *> custom(v.m_pOS);

		pReader->ReadCustom(&custom, szName);

		return Status();
	}
};

template <>
struct DataIniter<MemberType_Scalar, Test>
{
	static void Init(Test &v)
	{
	}
};

}//namespace SimpleBuffers

}//namespace CX



using namespace CX;


int main(int argc, char *argv[])
{
	for (int i = 1; i <= 40; i++)
	{
		IO::FileInputStream       fis("c:\\temp\\json\\3.json");
		IO::FileOutputStream

		SimpleBuffers::JSONReader reader;
		Test                      test;
		Status                    status;

		test.m_pOS = 

		status = reader.Begin(&fis);
		status = SimpleBuffers::DataReader<SimpleBuffers::MemberType_Scalar, Test>::Read(&reader, test);
		status = reader.End();
	}

	CX_UNUSED(argc);
	CX_UNUSED(argv);

	Tester::Run();

	return 0;
}

