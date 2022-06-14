/* 
 * CX - C++ framework for general purpose development
 *
 * https://github.com/draede/cx
 * 
 * Copyright (C) 2014 - 2021 draede
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
#include "CX/IO/FileInputStream.hpp"
#include "CX/IO/BufferedInputStream.hpp"
#include "CX/IO/FileOutputStream.hpp"
#include "CX/IO/BufferedOutputStream.hpp"
#include "CX/Util/RndGen.hpp"
#include "CX/Print.hpp"
#include "Tester.hpp"


using namespace CX;


void IO_Buffered_Test()
{
	static const UInt64 FILE_SIZE = 62710561;
	static const Size   MIN_WRITE_SIZE = 1;
	static const Size   MAX_WRITE_SIZE = 100000;

	static Byte buffer[MAX_WRITE_SIZE];

	Byte   *pPos;
	UInt64 cbSize;
	UInt64 cbIndex;
	Size   cbCrSize;
	Size   cbAckSize;
	Status status;

	Util::RndGen::Get().Seed32(GetTickCount());
	Util::RndGen::Get().Seed64(GetTickCount64());

	{
		IO::FileOutputStream fos("io_normal.dat");

		cbSize = 0;
		cbIndex = 0;
		while (cbSize < FILE_SIZE)
		{
			cbCrSize = Util::RndGen::Get().GetSizeRange(MIN_WRITE_SIZE, MAX_WRITE_SIZE);
			if (cbCrSize + cbSize > FILE_SIZE)
			{
				cbCrSize = (Size)(FILE_SIZE - cbSize);
			}

			cbSize += cbCrSize;

			for (Size i = 0; i < cbCrSize; i++)
			{
				buffer[i] = (Byte)(cbIndex % 256);
				cbIndex++;
			}

			pPos = buffer;
			while (0 < cbCrSize)
			{
				fos.Write(pPos, cbCrSize, &cbAckSize);
				cbCrSize -= cbAckSize;
				pPos += cbAckSize;
			}
		}
	}

	{
		IO::FileInputStream fis("io_normal.dat");

		cbSize = 0;
		cbIndex = 0;
		while (cbSize < FILE_SIZE)
		{
			cbCrSize = Util::RndGen::Get().GetSizeRange(MIN_WRITE_SIZE, MAX_WRITE_SIZE);
			if (cbCrSize + cbSize > FILE_SIZE)
			{
				cbCrSize = (Size)(FILE_SIZE - cbSize);
			}

			cbSize += cbCrSize;

			pPos = buffer;
			while (0 < cbCrSize)
			{
				fis.Read(pPos, cbCrSize, &cbAckSize);
				cbCrSize -= cbAckSize;
				pPos += cbAckSize;
			}

			for (Size i = 0; i < cbCrSize; i++)
			{
				if (buffer[i] != (Byte)(cbIndex % 256))
				{
					Print(stdout, "ERROR\n");
				}
				cbIndex++;
			}
		}
	}

	{
		IO::FileOutputStream     fos("io_buffered.dat");
		IO::BufferedOutputStream bos(&fos);

		cbSize = 0;
		cbIndex = 0;
		while (cbSize < FILE_SIZE)
		{
			cbCrSize = Util::RndGen::Get().GetSizeRange(MIN_WRITE_SIZE, MAX_WRITE_SIZE);
			if (cbCrSize + cbSize > FILE_SIZE)
			{
				cbCrSize = (Size)(FILE_SIZE - cbSize);
			}

			cbSize += cbCrSize;

			for (Size i = 0; i < cbCrSize; i++)
			{
				buffer[i] = (Byte)(cbIndex % 256);
				cbIndex++;
			}

			pPos = buffer;
			while (0 < cbCrSize)
			{
				bos.Write(pPos, cbCrSize, &cbAckSize);
				cbCrSize -= cbAckSize;
				pPos += cbAckSize;
			}
		}
	}

	{
		IO::FileInputStream     fis("io_buffered.dat");
		IO::BufferedInputStream bis(&fis);

		cbSize = 0;
		cbIndex = 0;
		while (cbSize < FILE_SIZE)
		{
			cbCrSize = Util::RndGen::Get().GetSizeRange(MIN_WRITE_SIZE, MAX_WRITE_SIZE);
			if (cbCrSize + cbSize > FILE_SIZE)
			{
				cbCrSize = (Size)(FILE_SIZE - cbSize);
			}

			cbSize += cbCrSize;

			pPos = buffer;
			while (0 < cbCrSize)
			{
				bis.Read(pPos, cbCrSize, &cbAckSize);
				cbCrSize -= cbAckSize;
				pPos += cbAckSize;
			}

			for (Size i = 0; i < cbCrSize; i++)
			{
				if (buffer[i] != (Byte)(cbIndex % 256))
				{
					Print(stdout, "ERROR\n");
				}
				cbIndex++;
			}
		}
	}
}

REGISTER_TEST(IO_Buffered_Test);
