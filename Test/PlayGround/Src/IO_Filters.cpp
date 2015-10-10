/* 
 * CX - C++ framework for general purpose development
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

#include "CX/Types.hpp"
#include "CX/Status.hpp"
#include "CX/Util/MemPool.hpp"
#include "CX/Util/Timer.hpp"
#include "CX/Util/RndGen.hpp"
#include "CX/Print.hpp"
#include "CX/Archive/LZHAMFilter.hpp"
#include "CX/Crypt/AES128Filter.hpp"
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

	return Util::RndGen::Get().GetBytes((Byte *)pMemPool->GetMem(), cbSize, byteset + 65, 26);
}

Status TestFilterAsInput(IO::IFilter *pFilter, Util::DynMemPool *pMemPoolIn, Util::DynMemPool *pMemPoolOut)
{
	IO::MemInputStream       mis(pMemPoolIn);
	IO::MemOutputStream      mos(pMemPoolOut);
	IO::FilteredInputStream  is(pFilter, &mis);
	Status                   status;

	if (!(status = IO::Helper::CopyStream(&is, &mos)))
	{
		return status;
	}

	return Status();
}

Status TestFilterAsOutput(IO::IFilter *pFilter, Util::DynMemPool *pMemPoolIn, Util::DynMemPool *pMemPoolOut)
{
	IO::MemInputStream       mis(pMemPoolIn);
	IO::MemOutputStream      mos(pMemPoolOut);
	IO::FilteredOutputStream os(pFilter, &mos);
	Status                   status;

	if (!(status = IO::Helper::CopyStream(&mis, &os)))
	{
		return status;
	}

	return Status();
}

void TestFilter(IO::IFilter *pInFilter, IO::IFilter *pOutFilter, Util::DynMemPool *pMemPoolIn)
{
	Util::Timer      timer;
	Status           status;

	{
		Util::DynMemPool    output1;
		Util::DynMemPool    output2;

		pInFilter->Reset();
		pOutFilter->Reset();

		timer.ResetTimer();
		if ((status = TestFilterAsInput(pInFilter, pMemPoolIn, &output1)))
		{
			Print(stdout, "Input filtered (=>) {1} bytes into {2} bytes in {3:.3} seconds ({4:.3} MB/sec)\n", 
			      pMemPoolIn->GetSize(), output1.GetSize(), timer.GetElapsedTime(), 
			      ((double)pMemPoolIn->GetSize() / 1048576.0) / timer.GetElapsedTime());
		}

		timer.ResetTimer();
		if ((status = TestFilterAsInput(pOutFilter, &output1, &output2)))
		{
			Print(stdout, "Input filtered (<=) {1} bytes into {2} bytes in {3:.3} seconds ({4:.3} MB/sec)\n", output1.GetSize(), 
			      output2.GetSize(), timer.GetElapsedTime(), ((double)output1.GetSize() / 1048576.0) / timer.GetElapsedTime());
		}

		if (pMemPoolIn->GetSize() == output2.GetSize())
		{
			if (0 != memcmp(pMemPoolIn->GetMem(), output2.GetMem(), pMemPoolIn->GetSize()))
			{
				Print(stdout, "Buffer difference\n");
			}
		}
		else
		{
			Print(stdout, "Size difference\n");
		}
	}
	{
		Util::DynMemPool    output1;
		Util::DynMemPool    output2;

		pInFilter->Reset();
		pOutFilter->Reset();

		timer.ResetTimer();
		if ((status = TestFilterAsOutput(pInFilter, pMemPoolIn, &output1)))
		{
			Print(stdout, "Output filtered (=>) {1} bytes into {2} bytes in {3:.3} seconds ({4:.3} MB/sec)\n", 
			      pMemPoolIn->GetSize(), output1.GetSize(), timer.GetElapsedTime(), 
			      ((double)pMemPoolIn->GetSize() / 1048576.0) / timer.GetElapsedTime());
		}

		timer.ResetTimer();
		if ((status = TestFilterAsOutput(pOutFilter, &output1, &output2)))
		{
			Print(stdout, "Output filtered (<=) {1} bytes into {2} bytes in {3:.3} seconds ({4:.3} MB/sec)\n", output1.GetSize(), 
			      output2.GetSize(), timer.GetElapsedTime(), ((double)output1.GetSize() / 1048576.0) / timer.GetElapsedTime());
		}

		if (pMemPoolIn->GetSize() == output2.GetSize())
		{
			if (0 != memcmp(pMemPoolIn->GetMem(), output2.GetMem(), pMemPoolIn->GetSize()))
			{
				Print(stdout, "Buffer difference\n");
			}
		}
		else
		{
			Print(stdout, "Size difference\n");
		}
	}
}

void Archive_LZHAMFilter_Test()
{
	Util::DynMemPool input;
	Util::Timer      timer;
	Status           status;

	timer.ResetTimer();
	status = CreateInput(&input, 4096, 131072);
	Print(stdout, "Generated {1} bytes random input in {2:.3} seconds\n", input.GetSize(), timer.GetElapsedTime());

	{
		Archive::LZHAMFilter flt1(Archive::LZHAMFilter::Dir_Compress);
		Archive::LZHAMFilter flt2(Archive::LZHAMFilter::Dir_Uncompress);

		Print(stdout, "Testing LZHAMFilter...\n");
		TestFilter(&flt1, &flt2, &input);
	}
}

REGISTER_TEST(Archive_LZHAMFilter_Test);

