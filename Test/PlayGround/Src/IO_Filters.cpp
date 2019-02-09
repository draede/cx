/* 
 * CX - C++ framework for general purpose development
 *
 * https://github.com/draede/cx
 * 
 * Copyright (C) 2014 - 2019 draede - draede [at] outlook [dot] com
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

#include "CX/Types.hpp"
#include "CX/Status.hpp"
#include "CX/Util/MemPool.hpp"
#include "CX/Util/Timer.hpp"
#include "CX/Util/RndGen.hpp"
#include "CX/Print.hpp"
#include "CX/Crypt/AES128InputFilter.hpp"
#include "CX/Crypt/AES128OutputFilter.hpp"
#include "CX/Archive/SnappyInputFilter.hpp"
#include "CX/Archive/SnappyOutputFilter.hpp"
#include "CX/Archive/LZ4InputFilter.hpp"
#include "CX/Archive/LZ4OutputFilter.hpp"
#include "CX/Archive/LZHAMInputFilter.hpp"
#include "CX/Archive/LZHAMOutputFilter.hpp"
#include "CX/IO/FilteredInputStream.hpp"
#include "CX/IO/FilteredOutputStream.hpp"
#include "CX/IO/MemInputStream.hpp"
#include "CX/IO/MemOutputStream.hpp"
#include "CX/IO/Helper.hpp"
#include "Tester.hpp"


using namespace CX;

Status CreateInput(Util::DynMemPool *pMemPool, Size cbMinSize, Size cbMaxSize)
{
	static Byte byteset[256];
	static Bool bInited = false;

	if (!bInited)
	{
		bInited = true;
		for (Size i = 0; i < sizeof(byteset) / sizeof(byteset[0]); i++)
		{
			byteset[i] = (Byte)(i % ((Size)TYPE_UINT8_MAX + 1));
		}
	}

	Size   cbSize = Util::RndGen::Get().GetSizeRange(cbMinSize, cbMaxSize);
	Status status;

	if (!(status = pMemPool->SetSize(cbSize)))
	{
		return status;
	}

	Util::RndGen::Get().GetBytes((Byte *)pMemPool->GetMem(), cbSize, byteset + 65, 26);

	return Status();
}

typedef IO::IInputStream * (*InputStreamFactory)(IO::IInputStream *pIS);

typedef IO::IOutputStream * (*OutputStreamFactory)(IO::IOutputStream *pOS);

void TestFilter(const Char *szName, InputStreamFactory pfnInputStreamFactory, OutputStreamFactory pfnOutputStreamFactory, 
                IO::IInputStream *pInputStream)
{

	Util::DynMemPool   mem1;
	Util::DynMemPool   mem2;
	Util::DynMemPool   mem3;
	UInt64             cbSize1;
	UInt64             cbSize2;
	UInt64             cbSize3;
	Status             status;

	{
		IO::MemOutputStream   mos(&mem1);

		if (!(status = IO::Helper::CopyStream(pInputStream, &mos)))
		{
			Print(stdout, "{1} : copy error {2}, msg '{3}'\n", szName, status.GetCode(), status.GetMsg());

			return;
		}

		mos.GetSize(&cbSize1);
	}

	{
		IO::MemOutputStream   mos(&mem2);

		{
			IO::MemInputStream    mis(&mem1, (Size)cbSize1);
			IO::IOutputStream     *pOS = pfnOutputStreamFactory(&mos);

			if (!(status = IO::Helper::CopyStream(&mis, pOS)))
			{
				Print(stdout, "{1} : encode error {2}, msg '{3}'\n", szName, status.GetCode(), status.GetMsg());
				delete pOS;

				return;
			}
			delete pOS;
		}

		mos.GetSize(&cbSize2);
	}
	{
		IO::MemOutputStream      mos(&mem3);

		{
			IO::MemInputStream       mis(&mem2, (Size)cbSize2);
			IO::IInputStream         *pIS = pfnInputStreamFactory(&mis);

			if (!(status = IO::Helper::CopyStream(pIS, &mos)))
			{
				Print(stdout, "{1} : decode error {2}, msg '{3}'\n", szName, status.GetCode(), status.GetMsg());
				delete pIS;

				return;
			}

			delete pIS;
		}

		mos.GetSize(&cbSize3);
	}
	if (cbSize1 != cbSize3)
	{
		Print(stdout, "{1} : size mismatched\n", szName);

		return;
	}
	if (0 != memcmp(mem1.GetMem(), mem3.GetMem(), (Size)cbSize1))
	{
		Print(stdout, "{1} : content mismatched\n", szName);

		return;
	}
}

IO::IInputStream *AESInputStreamFactory(IO::IInputStream *pIS)
{
	return new (std::nothrow) IO::FilteredInputStream(new (std::nothrow) Crypt::AES128InputFilter("12345678"), pIS);
}

IO::IOutputStream *AESOutputStreamFactory(IO::IOutputStream *pOS)
{
	return new (std::nothrow) IO::FilteredOutputStream(new (std::nothrow) Crypt::AES128OutputFilter("12345678"), pOS);
}

IO::IInputStream *LZ4InputStreamFactory(IO::IInputStream *pIS)
{
	return new (std::nothrow) IO::FilteredInputStream(new (std::nothrow) Archive::LZ4InputFilter(), pIS);
}

IO::IOutputStream *LZ4OutputStreamFactory(IO::IOutputStream *pOS)
{
	return new (std::nothrow) IO::FilteredOutputStream(new (std::nothrow) Archive::LZ4OutputFilter(), pOS);
}

IO::IInputStream *SnappyInputStreamFactory(IO::IInputStream *pIS)
{
	return new (std::nothrow) IO::FilteredInputStream(new (std::nothrow) Archive::SnappyInputFilter(), pIS);
}

IO::IOutputStream *SnappyOutputStreamFactory(IO::IOutputStream *pOS)
{
	return new (std::nothrow) IO::FilteredOutputStream(new (std::nothrow) Archive::SnappyOutputFilter(), pOS);
}

IO::IInputStream *LZHAMInputStreamFactory(IO::IInputStream *pIS)
{
	return new (std::nothrow) IO::FilteredInputStream(new (std::nothrow) Archive::LZHAMInputFilter(), pIS);
}

IO::IOutputStream *LZHAMOutputStreamFactory(IO::IOutputStream *pOS)
{
	return new (std::nothrow) IO::FilteredOutputStream(new (std::nothrow) Archive::LZHAMOutputFilter(), pOS);
}

IO::IInputStream *LZ4AESInputStreamFactory(IO::IInputStream *pIS)
{
	return new (std::nothrow) IO::FilteredInputStream(new (std::nothrow) Archive::LZ4InputFilter(), 
	                                        new (std::nothrow) IO::FilteredInputStream(new (std::nothrow) Crypt::AES128InputFilter("12345678"), pIS), true);
}

IO::IOutputStream *LZ4AESOutputStreamFactory(IO::IOutputStream *pOS)
{
	return new (std::nothrow) IO::FilteredOutputStream(new (std::nothrow) Archive::LZ4OutputFilter(16), 
	                                        new (std::nothrow) IO::FilteredOutputStream(new (std::nothrow) Crypt::AES128OutputFilter("12345678"), pOS), true);
}

IO::IInputStream *SnappyAESInputStreamFactory(IO::IInputStream *pIS)
{
	return new (std::nothrow) IO::FilteredInputStream(new (std::nothrow) Archive::SnappyInputFilter(), 
	                                        new (std::nothrow) IO::FilteredInputStream(new (std::nothrow) Crypt::AES128InputFilter("12345678"), pIS), true);
}

IO::IOutputStream *SnappyAESOutputStreamFactory(IO::IOutputStream *pOS)
{
	return new (std::nothrow) IO::FilteredOutputStream(new (std::nothrow) Archive::SnappyOutputFilter(), 
	                                        new (std::nothrow) IO::FilteredOutputStream(new (std::nothrow) Crypt::AES128OutputFilter("12345678"), pOS), true);
}

IO::IInputStream *LZHAMAESInputStreamFactory(IO::IInputStream *pIS)
{
	return new (std::nothrow) IO::FilteredInputStream(new (std::nothrow) Archive::LZHAMInputFilter(), 
	                                        new (std::nothrow) IO::FilteredInputStream(new (std::nothrow) Crypt::AES128InputFilter("12345678"), pIS), true);
}

IO::IOutputStream *LZHAMAESOutputStreamFactory(IO::IOutputStream *pOS)
{
	return new (std::nothrow) IO::FilteredOutputStream(new (std::nothrow) Archive::LZHAMOutputFilter(), 
	                                        new (std::nothrow) IO::FilteredOutputStream(new (std::nothrow) Crypt::AES128OutputFilter("12345678"), pOS), true);
}

void Filter_Test(const Char *szName, InputStreamFactory pfnInputStreamFactory, OutputStreamFactory pfnOutputStreamFactory)
{
	Util::DynMemPool   mem;

	CreateInput(&mem, 1000, 1000000);

	IO::MemInputStream mis(&mem, mem.GetSize());

	TestFilter(szName, pfnInputStreamFactory, pfnOutputStreamFactory, &mis);
}

void AESFilter_Test()
{
	Filter_Test("AESFilter_Test", &AESInputStreamFactory, &AESOutputStreamFactory);
}

void SnappyFilter_Test()
{
	Filter_Test("SnappyFilter_Test", &SnappyInputStreamFactory, &SnappyOutputStreamFactory);
}

void LZ4Filter_Test()
{
	Filter_Test("LZ4Filter_Test", &LZ4InputStreamFactory, &LZ4OutputStreamFactory);
}

void LZHAMFilter_Test()
{
	Filter_Test("LZHAMFilter_Test", &LZHAMInputStreamFactory, &LZHAMOutputStreamFactory);
}

void SnappyAESFilter_Test()
{
	Filter_Test("SnappyAESFilter_Test", &SnappyAESInputStreamFactory, &SnappyAESOutputStreamFactory);
}

void LZ4AESFilter_Test()
{
	Filter_Test("LZ4AESFilter_Test", &LZ4AESInputStreamFactory, &LZ4AESOutputStreamFactory);
}

void LZHAMAESFilter_Test()
{
	Filter_Test("LZHAMAESFilter_Test", &LZHAMAESInputStreamFactory, &LZHAMAESOutputStreamFactory);
}

REGISTER_TEST(AESFilter_Test);
REGISTER_TEST(SnappyFilter_Test);
REGISTER_TEST(LZ4Filter_Test);
REGISTER_TEST(LZHAMFilter_Test);
REGISTER_TEST(SnappyAESFilter_Test);
REGISTER_TEST(LZ4AESFilter_Test);
REGISTER_TEST(LZHAMAESFilter_Test);
