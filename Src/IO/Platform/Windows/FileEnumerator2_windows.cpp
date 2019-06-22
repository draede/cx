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

#include "CX/Platform.hpp"


#if defined(CX_OS_WINDOWS)

#include "CX/IO/Platform/Windows/FileEnumerator2.hpp"
#include "CX/Str/UTF8.hpp"
#include "CX/Str/HexBinStr.hpp"
#include "CX/Sys/BOM.hpp"
#include "CX/Print.hpp"
#include <sstream>


namespace CX
{

namespace IO
{

const WChar *FileEnumerator2::Config::ARG_THREADS           = L"--threads";
const WChar *FileEnumerator2::Config::ARG_QUEUED_FILES      = L"--queuedfiles";
const WChar *FileEnumerator2::Config::ARG_MIN_FILE_SIZE     = L"--minfilesize";
const WChar *FileEnumerator2::Config::ARG_MAX_FILE_SIZE     = L"--maxfilesize";
const WChar *FileEnumerator2::Config::ARG_RECURSIVE         = L"--recursive";
const WChar *FileEnumerator2::Config::ARG_EXTENSION         = L"--extension";
const WChar *FileEnumerator2::Config::ARG_PATTERN           = L"--pattern";

FileEnumerator2::Config::Pattern::Pattern()
{
}

FileEnumerator2::Config::Pattern::Pattern(Bool bHasOffset, UInt64 cbOffset, const void *pPattern, Size cbSize, 
                                          Bool bNegate)
{
	this->bHasOffset = bHasOffset;
	this->cbOffset   = cbOffset;
	this->bNegate    = bNegate;
	SetupPattern(pPattern, cbSize);
}

Bool FileEnumerator2::Config::Pattern::SetupPattern(const void *pPattern, Size cbSize)
{
	if (0 == cbSize)
	{
		return False;
	}
	if (MIN_PATTERN_SIZE > cbSize)
	{
		return False;
	}
	if (MAX_PATTERN_SIZE < cbSize)
	{
		return False;
	}
	pattern.resize(cbSize);
	memcpy(&pattern[0], pPattern, cbSize);

	return True;
}

const FileEnumerator2::Config &FileEnumerator2::Config::GetDefault()
{
	static Config   config =
	{
		DEFAULT_THREADS,
		DEFAULT_QUEUED_FILES,
		DEFAULT_MIN_FILE_SIZE,
		DEFAULT_MAX_FILE_SIZE,
		DEFAULT_RECURSIVE,
		DEFAULT_MAP_FILE,
	};

	return config;
};

Status FileEnumerator2::Config::FromCmdLine(ArgsVector &vectorArgs, Size cStartArg/* = 0*/)
{
	Status   status;

	*this = GetDefault();

	if (vectorArgs.size() <= cStartArg)
	{
		return Status();
	}

	auto iter    = vectorArgs.begin() + cStartArg;
	auto iterTmp = iter;

	if (vectorArgs.end() == iter)
	{
		return Status();
	}

	while (vectorArgs.end() != iter)
	{
		if (0 == cxw_strcmp(ARG_THREADS, iter->c_str()))
		{
			iter = vectorArgs.erase(iter);
			if (vectorArgs.end() == iter)
			{
				status = Status(Status_InvalidArg, "No value specified for {1}", ARG_THREADS);

				break;
			}

			std::wstringstream   wiss;

			wiss.str(iter->c_str());
			wiss >> cThreads;
			if (wiss.fail() || !wiss.eof())
			{
				status = Status(Status_InvalidArg, "Invalid value specified for {1}", ARG_THREADS);

				break;
			}
			if (MIN_THREADS > cThreads)
			{
				status = Status(Status_InvalidArg, "Threads too small (was {1}, min is {2})", cThreads, MIN_THREADS);

				break;
			}
			if (MAX_THREADS < cThreads)
			{
				status = Status(Status_InvalidArg, "Threads too big (was {1}, max is {2})", cThreads, MIN_THREADS);

				break;
			}
			iter = vectorArgs.erase(iter);
		}
		else
		if (0 == cxw_strcmp(ARG_QUEUED_FILES, iter->c_str()))
		{
			iter = vectorArgs.erase(iter);
			if (vectorArgs.end() == iter)
			{
				status = Status(Status_InvalidArg, "No value specified for {1}", ARG_QUEUED_FILES);

				break;
			}

			std::wstringstream   wiss;

			wiss.str(iter->c_str());
			wiss >> cQueuedFiles;
			if (wiss.fail() || !wiss.eof())
			{
				status = Status(Status_InvalidArg, "Invalid value specified for {1}", ARG_QUEUED_FILES);

				break;
			}
			iter = vectorArgs.erase(iter);
		}
		else
		if (0 == cxw_strcmp(ARG_MIN_FILE_SIZE, iter->c_str()))
		{
			iter = vectorArgs.erase(iter);
			if (vectorArgs.end() == iter)
			{
				status = Status(Status_InvalidArg, "No value specified for {1}", ARG_MIN_FILE_SIZE);

				break;
			}

			std::wstringstream   wiss;

			wiss.str(iter->c_str());
			wiss >> cbMinFileSize;
			if (wiss.fail() || !wiss.eof())
			{
				status = Status(Status_InvalidArg, "Invalid value specified for {1}", ARG_MIN_FILE_SIZE);

				break;
			}
			if (MIN_FILE_SIZE > cbMinFileSize)
			{
				status = Status(Status_InvalidArg, "Min file size too small (was {1}, min is {2})", cbMinFileSize, 
				                MIN_FILE_SIZE);

				break;
			}
			if (MAX_FILE_SIZE < cbMinFileSize)
			{
				status = Status(Status_InvalidArg, "Min file size too big (was {1}, max is {2})", cbMinFileSize, 
				                MAX_FILE_SIZE);

				break;
			}
			iter = vectorArgs.erase(iter);
		}
		else
		if (0 == cxw_strcmp(ARG_MAX_FILE_SIZE, iter->c_str()))
		{
			iter = vectorArgs.erase(iter);
			if (vectorArgs.end() == iter)
			{
				status = Status(Status_InvalidArg, "No value specified for {1}", ARG_MAX_FILE_SIZE);

				break;
			}

			std::wstringstream   wiss;

			wiss.str(iter->c_str());
			wiss >> cbMaxFileSize;
			if (wiss.fail() || !wiss.eof())
			{
				status = Status(Status_InvalidArg, "Invalid value specified for {1}", ARG_MAX_FILE_SIZE);

				break;
			}
			if (MIN_FILE_SIZE > cbMaxFileSize)
			{
				status = Status(Status_InvalidArg, "Max file size too small (was {1}, min is {2})", cbMaxFileSize,
				                MIN_FILE_SIZE);

				break;
			}
			if (MAX_FILE_SIZE < cbMaxFileSize)
			{
				status = Status(Status_InvalidArg, "Max file size too big (was {1}, max is {2})", cbMaxFileSize,
				                MAX_FILE_SIZE);

				break;
			}
			iter = vectorArgs.erase(iter);
		}
		else
		if (0 == cxw_strcmp(ARG_RECURSIVE, iter->c_str()))
		{
			iter = vectorArgs.erase(iter);
			if (vectorArgs.end() == iter)
			{
				status = Status(Status_InvalidArg, "No value specified for {1}", ARG_RECURSIVE);

				break;
			}
			if (0 == cxw_stricmp(iter->c_str(), L"yes"))
			{
				bRecursive = True;
			}
			else
			if (0 == cxw_stricmp(iter->c_str(), L"no"))
			{
				bRecursive = True;
			}
			else
			{
				status = Status(Status_InvalidArg, "Invalid value specified for {1}", ARG_RECURSIVE);

				break;
			}
			iter = vectorArgs.erase(iter);
		}
		else
		if (0 == cxw_strcmp(ARG_EXTENSION, iter->c_str()))
		{
			iter = vectorArgs.erase(iter);
			if (vectorArgs.end() == iter)
			{
				status = Status(Status_InvalidArg, "No value specified for {1}", ARG_EXTENSION);

				break;
			}
			setExtensions.insert(*iter);
			iter = vectorArgs.erase(iter);
		}
		else
		if (0 == cxw_strcmp(ARG_PATTERN, iter->c_str()))
		{
			Pattern   pattern;

			if (0 == cxw_strcmp(ARG_PATTERN, iter->c_str()))
			{
				iter = vectorArgs.erase(iter);
				if (vectorArgs.end() == iter)
				{
					status = Status(Status_InvalidArg, "No offset specified for {1}", ARG_PATTERN);

					break;
				}

				pattern.bHasOffset = True;

				std::wstringstream   wiss;

				wiss.str(iter->c_str());
				wiss >> pattern.cbOffset;
				if (wiss.fail() || !wiss.eof())
				{
					status = Status(Status_InvalidArg, "Invalid offset specified for {1}", ARG_PATTERN);

					break;
				}
				iter = vectorArgs.erase(iter);
				if (vectorArgs.end() == iter)
				{
					status = Status(Status_InvalidArg, "No pattern specified for {1}", ARG_PATTERN);

					break;
				}
				if (iter->empty())
				{
					status = Status(Status_InvalidArg, "Invalid pattern specified for {1}", ARG_PATTERN);

					break;
				}
			}
			else
			{
				iter = vectorArgs.erase(iter);
				if (vectorArgs.end() == iter)
				{
					status = Status(Status_InvalidArg, "No offset specified for {1}", ARG_PATTERN);

					break;
				}

				pattern.bHasOffset = False;
				pattern.cbOffset   = 0;
			}

			const WChar   *wszPattern = iter->c_str();
			Size          cPatternLen = iter->size();

			if (L'!' == *wszPattern)
			{
				pattern.bNegate = True;
				wszPattern++;
				cPatternLen--;
			}
			else
			{
				pattern.bNegate = False;
			}

			if (4 <= cPatternLen && L'0' == *wszPattern && (L'x' == *(wszPattern + 1) || L'X' == *(wszPattern + 1)))
			{
				if (MAX_PATTERN_SIZE + 2 < cPatternLen || 0 != (cPatternLen % 2))
				{
					status = Status(Status_InvalidArg, "Invalid pattern specified for {1}", ARG_PATTERN);

					break;
				}

				Str::HexBinStr   hbs;
				String           sPattern;
				Byte             hex[MAX_PATTERN_SIZE];

				if (!(status = Str::UTF8::FromWChar(wszPattern + 2, &sPattern)))
				{
					status = Status(Status_InvalidArg, "Invalid pattern specified for {1}", ARG_PATTERN);

					break;
				}
				if (!(status = hbs.FromString(sPattern.c_str(), sPattern.size(), hex, sPattern.size() / 2)))
				{
					status = Status(Status_InvalidArg, "Invalid pattern specified for {1}", ARG_PATTERN);

					break;
				}
				if (!pattern.SetupPattern(hex, sPattern.size() / 2))
				{
					status = Status(Status_InvalidArg, "Invalid pattern specified for {1}", ARG_PATTERN);

					break;
				}
			}
			else
			{
				String   sPattern;

				if (!(status = Str::UTF8::FromWChar(wszPattern, &sPattern)))
				{
					status = Status(Status_InvalidArg, "Invalid pattern specified for {1}", ARG_PATTERN);

					break;
				}
				if (!pattern.SetupPattern(sPattern.c_str(), sPattern.size()))
				{
					status = Status(Status_InvalidArg, "Invalid pattern specified for {1}", ARG_PATTERN);

					break;
				}
			}
			vectorPatterns.push_back(pattern);
			iter = vectorArgs.erase(iter);
		}
		else
		{
			++iter;
		}
	}

	return status;
}

//===

FileEnumerator2::FileEnumerator2()
{
}

FileEnumerator2::~FileEnumerator2()
{
}

Status FileEnumerator2::Run(const WChar *wszPath, IHandler *pHandler, const Config &config/* = Config::GetDefault()*/)
{
	PathsVector   vectorPaths;

	vectorPaths.push_back(wszPath);

	return Run(vectorPaths, pHandler, config);
}

Status FileEnumerator2::Run(const PathsVector &vectorPaths, IHandler *pHandler, 
                            const Config &config/* = Config::GetDefault()*/)
{
	Context            ctx;
	LARGE_INTEGER      liQPerf;
	DWORD              dwThreadID;
	Size               cFilesPerThread;
	Size               cFiles;
	Status             status;

	ctx.bRunning                           = True;

	ctx.threadHandles                      = NULL;
	ctx.threadReadyEvents                  = NULL;
	ctx.threadData                         = NULL;
	ctx.cCurrentThread                     = 0;

	ctx.cPathIndex                         = 0;

	ctx.pHandler                           = pHandler;
	ctx.config                             = config;

	QueryPerformanceFrequency(&liQPerf);
	ctx.stats.nPerfCounterFreq             = liQPerf.QuadPart;
	QueryPerformanceCounter(&liQPerf);
	ctx.stats.nPerfCounterStart            = liQPerf.QuadPart;

	ctx.stats.cDiscoveredDirs              = 0;
	ctx.stats.cDiscoveredFiles             = 0;
	ctx.stats.cbDiscoveredFilesSize        = 0;

	if (Config::MIN_THREADS > config.cThreads)
	{
		return Status(Status_InvalidArg, "Threads too small (min {1})", Config::MIN_THREADS);
	}
	if (Config::MAX_THREADS < config.cThreads)
	{
		return Status(Status_InvalidArg, "Threads too big (max {1})", Config::MAX_THREADS);
	}
	if (Config::MIN_FILE_SIZE > config.cbMinFileSize)
	{
		return Status(Status_InvalidArg, "Min file size too small (min {1})", Config::MIN_FILE_SIZE);
	}
	if (Config::MAX_FILE_SIZE < config.cbMinFileSize)
	{
		return Status(Status_InvalidArg, "Min file size too big (min {1})", Config::MAX_FILE_SIZE);
	}
	if (Config::MIN_FILE_SIZE > config.cbMaxFileSize)
	{
		return Status(Status_InvalidArg, "Max file size too small (min {1})", Config::MIN_FILE_SIZE);
	}
	if (Config::MAX_FILE_SIZE < config.cbMaxFileSize)
	{
		return Status(Status_InvalidArg, "Max file size too big (min {1})", Config::MAX_FILE_SIZE);
	}
	if (Config::MIN_QUEUED_FILES > config.cQueuedFiles)
	{
		return Status(Status_InvalidArg, "Queued files too small (min {1})", Config::MIN_QUEUED_FILES);
	}
	if (Config::MAX_QUEUED_FILES < config.cQueuedFiles)
	{
		return Status(Status_InvalidArg, "Queued files too big (max {1})", Config::MAX_QUEUED_FILES);
	}
	for (Size i = 0; i < config.vectorPatterns.size(); i++)
	{
		if (Config::MIN_PATTERN_SIZE > config.vectorPatterns[i].pattern.size())
		{
			return Status(Status_InvalidArg, "Pattern {1} size too small (min {1})", i + 1, Config::MIN_PATTERN_SIZE);
		}
		if (Config::MAX_PATTERN_SIZE < config.vectorPatterns[i].pattern.size())
		{
			return Status(Status_InvalidArg, "Pattern {1} size too big (max {1})", i + 1, Config::MAX_PATTERN_SIZE);
		}
	}

	cFiles = config.cQueuedFiles;
	cFilesPerThread = cFiles / config.cThreads;
	if (0 < (cFiles % config.cThreads))
	{
		cFilesPerThread++;
	}

	for (;;)
	{
		if (NULL == (ctx.threadData = (ThreadData *)Mem::Alloc(sizeof(ThreadData) * ctx.config.cThreads)))
		{
			status = Status(Status_MemAllocFailed, "Failed to allocate threads data");

			break;
		}
		for (Size i = 0; i < ctx.config.cThreads; i++)
		{
			ctx.threadData[i].hEvent         = NULL;
			ctx.threadData[i].pbRunning      = &ctx.bRunning;
			ctx.threadData[i].pHandler       = ctx.pHandler;
			ctx.threadData[i].pStats         = &ctx.stats;
			ctx.threadData[i].pConfig        = &ctx.config;
			ctx.threadData[i].files          = NULL;
			ctx.threadData[i].cActualFiles   = 0;
			ctx.threadData[i].results        = NULL;
			ctx.threadData[i].cActualResults = 0;
			if (cFilesPerThread <= cFiles)
			{
				ctx.threadData[i].cFiles    = cFilesPerThread;
			}
			else
			{
				ctx.threadData[i].cFiles    = cFiles;
			}
			cFiles -= ctx.threadData[i].cFiles;
		}
		for (Size i = 0; i < ctx.config.cThreads; i++)
		{
			if (NULL == (ctx.threadData[i].files = new File[ctx.threadData[i].cFiles]))
			{
				status = Status(Status_MemAllocFailed, "Failed to create files data");

				break;
			}
			if (NULL == (ctx.threadData[i].results = (void **)Mem::Alloc(sizeof(void *) * ctx.threadData[i].cFiles)))
			{
				status = Status(Status_MemAllocFailed, "Failed to create results data");

				break;
			}
			if (NULL == (ctx.threadData[i].hEvent = CreateEvent(NULL, FALSE, FALSE, NULL)))
			{
				status = Status(Status_MemAllocFailed, "Failed to create event");

				break;
			}
		}
		if (!status)
		{
			break;
		}

		if (NULL == (ctx.threadReadyEvents = (HANDLE *)Mem::Alloc(sizeof(HANDLE) * ctx.config.cThreads)))
		{
			status = Status(Status_MemAllocFailed, "Failed to allocate thread ready events");

			break;
		}
		for (Size i = 0; i < ctx.config.cThreads; i++)
		{
			ctx.threadReadyEvents[i] = NULL;
		}
		for (Size i = 0; i < ctx.config.cThreads; i++)
		{
			if (NULL == (ctx.threadReadyEvents[i] = CreateEvent(NULL, FALSE, FALSE, NULL)))
			{
				status = Status(Status_MemAllocFailed, "Failed to create ready event");

				break;
			}
			ctx.threadData[i].hReadyEvent = ctx.threadReadyEvents[i];
		}
		if (!status)
		{
			break;
		}

		if (NULL == (ctx.threadHandles = (HANDLE *)Mem::Alloc(sizeof(HANDLE) * ctx.config.cThreads)))
		{
			status = Status(Status_MemAllocFailed, "Failed to allocate threads");

			break;
		}
		for (Size i = 0; i < ctx.config.cThreads; i++)
		{
			ctx.threadHandles[i] = NULL;
		}

		for (Size i = 0; i < ctx.config.cThreads; i++)
		{
			ctx.threadHandles[i] = CreateThread(NULL, 0, &FileEnumerator2::WorkerThread, &ctx.threadData[i], 0, 
			                                    &dwThreadID);
		}

		if (!pHandler->OnBegin())
		{
			status = Status_Cancelled;

			break;
		}

		for (Size cPathIndex = 0; cPathIndex < vectorPaths.size(); cPathIndex++)
		{
			ctx.cPathIndex = cPathIndex;
			if (0 == cxw_strnicmp(vectorPaths[cPathIndex].c_str(), L"list:", 5))
			{
				if (!(status = RunWithList(vectorPaths[cPathIndex].c_str() + 5, ctx)))
				{
					if (!pHandler->OnError(status))
					{
						status = Status_Cancelled;

						break;
					}
				}
			}
			else
			{
				if (!(status = RunWithPath(vectorPaths[cPathIndex].c_str(), ctx)))
				{
					if (!pHandler->OnError(status))
					{
						status = Status_Cancelled;

						break;
					}
				}
			}
		}
		if (!status)
		{
			break;
		}

		break;
	}

	if (status)
	{
		status = ProcessFiles(ctx);
	}
	if (status)
	{
		if (!pHandler->OnEnd(&ctx.stats))
		{
			status = Status_Cancelled;
		}
	}

	ctx.bRunning = False;

	if (NULL != ctx.threadHandles)
	{
		for (Size i = 0; i < ctx.config.cThreads; i++)
		{
			if (NULL != ctx.threadData[i].hEvent)
			{
				SetEvent(ctx.threadData[i].hEvent);
			}
		}
		WaitForMultipleObjects((DWORD)ctx.config.cThreads, ctx.threadHandles, TRUE, INFINITE);

		for (Size i = 0; i < ctx.config.cThreads; i++)
		{
			if (NULL != ctx.threadHandles[i])
			{
				CloseHandle(ctx.threadHandles[i]);
			}
		}
		Mem::Free(ctx.threadHandles);
	}

	if (NULL != ctx.threadReadyEvents)
	{
		for (Size i = 0; i < ctx.config.cThreads; i++)
		{
			if (NULL != ctx.threadReadyEvents[i])
			{
				CloseHandle(ctx.threadReadyEvents[i]);
			}
		}
		Mem::Free(ctx.threadReadyEvents);
	}

	if (NULL != ctx.threadData)
	{
		for (Size i = 0; i < ctx.config.cThreads; i++)
		{
			if (NULL != ctx.threadData[i].results)
			{
				Mem::Free(ctx.threadData[i].results);
			}
			if (NULL != ctx.threadData[i].files)
			{
				delete [] ctx.threadData[i].files;
			}
			if (NULL != ctx.threadData[i].hEvent)
			{
				CloseHandle(ctx.threadData[i].hEvent);
			}
		}
		Mem::Free(ctx.threadData);
	}

	return status;
}

Status FileEnumerator2::RunWithPath(const WChar *wszPath, Context &ctx)
{
	HANDLE             hFind;
	WIN32_FIND_DATAW   data;
	DWORD              dwAttr;
	Status             status;

	dwAttr = GetFileAttributesW(wszPath);
	if (INVALID_FILE_ATTRIBUTES == dwAttr)
	{
		return Enumerate(wszPath, ctx);
	}
	else
	{
		if (FILE_ATTRIBUTE_DIRECTORY == (FILE_ATTRIBUTE_DIRECTORY & dwAttr))
		{
			return Enumerate(wszPath, ctx);
		}
		else
		{
			if (INVALID_HANDLE_VALUE != (hFind = FindFirstFileW(wszPath, &data)))
			{
				FindClose(hFind);

				if (!(status = OnFile(wszPath, cxw_strlen(wszPath), data, ctx)))
				{
					return status;
				}
			}
		}
	}

	return Status();
}

Status FileEnumerator2::RunWithList(const WChar *wszListPath, Context &ctx)
{
	FILE             *pFile;
	Byte             bom[MAX_BOM_SIZE];
	Size             cbSize;
	Size             cbBOMSize;
	Sys::BOM::Type   nBOMType;
	Status           status;

	pFile = NULL;
	for (;;)
	{
		if (NULL == (pFile = _wfopen(wszListPath, L"rb")))
		{
			status = Status(Status_OpenFailed, "Failed tot open list '{1}'", wszListPath);
			if (!ctx.pHandler->OnError(status))
			{
				status = Status_Cancelled;
			}

			break;
		}
		cbSize = fread(bom, 1, sizeof(bom), pFile);
		if (0 == cbSize) //empty file?
		{
			break;
		}
		nBOMType = Sys::BOM::Get(bom, cbSize, &cbBOMSize);

		if (Sys::BOM::None == nBOMType)
		{
			fseek(pFile, 0, SEEK_SET);
			status = RunWithListUTF8(pFile, ctx);
		}
		else
		if (Sys::BOM::UTF8 == nBOMType)
		{
			fseek(pFile, (long)cbBOMSize, SEEK_SET);
			status = RunWithListUTF8(pFile, ctx);
		}
		else
		if (Sys::BOM::UTF16_LE == nBOMType)
		{
			fseek(pFile, (long)cbBOMSize, SEEK_SET);
			status = RunWithListUTF16(pFile, ctx);
		}
		else
		{
			status = Status(Status_InvalidArg, "Unsupported BOM for list '{1}'", wszListPath);
			if (!ctx.pHandler->OnError(status))
			{
				status = Status_Cancelled;
			}

			break;
		}

		break;
	}
	if (NULL != pFile)
	{
		fclose(pFile);
	}

	return status;
}

Status FileEnumerator2::RunWithListUTF8(FILE *pFile, Context &ctx)
{
	Char     *szLine;
	WChar    *wszLine;
	Size     cLen;
	Size     cLenW;
	Status   status;

	if (NULL == (szLine = new Char[MAX_LINE_LEN]))
	{
		return ctx.pHandler->OnError(Status(Status_MemAllocFailed, "Failed to allocate line buffer"));
	}
	if (NULL == (wszLine = new WChar[MAX_LINE_LEN * 6]))
	{
		free(szLine);

		return ctx.pHandler->OnError(Status(Status_MemAllocFailed, "Failed to allocate line buffer (2)"));
	}

	while (!feof(pFile))
	{
		if (NULL != fgets(szLine, MAX_LINE_LEN, pFile))
		{
			cLen = cx_strlen(szLine);

			if (0 < cLen && 32 > szLine[cLen - 1])
			{
				szLine[cLen - 1] = 0;
				cLen--;
				if (0 < cLen && 32 > szLine[cLen - 1])
				{
					szLine[cLen - 1] = 0;
					cLen--;
				}
			}
			if (0 < cLen)
			{
				cLenW = MAX_LINE_LEN * 6;
				if (!(status = Str::UTF8::ToWChar(szLine, cLen + 1, wszLine, &cLenW)))
				{
					if (!ctx.pHandler->OnError(status))
					{
						status = Status_Cancelled;
					}

					break;
				}
				if (!(status = RunWithPath(wszLine, ctx)))
				{
					break;
				}
			}
		}
	}

	free(wszLine);
	free(szLine);

	return status;
}

Status FileEnumerator2::RunWithListUTF16(FILE *pFile, Context &ctx)
{
	WChar    *wszLine;
	Size     cLen;
	Status   status;

	if (NULL == (wszLine = new WChar[MAX_LINE_LEN]))
	{
		return ctx.pHandler->OnError(Status(Status_MemAllocFailed, "Failed to allocate line buffer"));
	}

	while (!feof(pFile))
	{
		if (NULL != fgetws(wszLine, MAX_LINE_LEN, pFile))
		{
			cLen = cxw_strlen(wszLine);

			if (0 < cLen && 32 > wszLine[cLen - 1])
			{
				wszLine[cLen - 1] = 0;
				cLen--;
				if (0 < cLen && 32 > wszLine[cLen - 1])
				{
					wszLine[cLen - 1] = 0;
					cLen--;
				}
			}
			if (0 < cLen)
			{
				if (!(status = RunWithPath(wszLine, ctx)))
				{
					break;
				}
			}
		}
	}

	free(wszLine);

	return status;
}

Status FileEnumerator2::Enumerate(const WChar *wszPath, Context &ctx)
{
	WIN32_FIND_DATAW   data;
	HANDLE             hFind;
	WString            wsPath;
	WString            wsMask;
	Status             status;

	ctx.stats.cDiscoveredDirs++;

	wsMask = wszPath;
	wsMask += L"\\*.*";
	if (INVALID_HANDLE_VALUE != (hFind = FindFirstFileW(wsMask.c_str(), &data)))
	{
		for (;;)
		{
			if (!(FILE_ATTRIBUTE_REPARSE_POINT & data.dwFileAttributes) &&
			    !(FILE_ATTRIBUTE_OFFLINE & data.dwFileAttributes) &&
			    !(0x00400000 & data.dwFileAttributes) && //FILE_ATTRIBUTE_RECALL_ON_DATA_ACCESS
			    !(0x00040000 & data.dwFileAttributes)) //FILE_ATTRIBUTE_RECALL_ON_OPEN
			{
				if (!(FILE_ATTRIBUTE_DIRECTORY & data.dwFileAttributes))
				{
					wsPath = wszPath;
					wsPath += L"\\";
					wsPath += data.cFileName;

					if (!(status = OnFile(wsPath.c_str(), wsPath.size(), data, ctx)))
					{
						return status;
					}
				}
				else
				if (0 != wcscmp(data.cFileName, L".") && 0 != wcscmp(data.cFileName, L".."))
				{
					if (ctx.config.bRecursive)
					{
						wsPath = wszPath;
						wsPath += L"\\";
						wsPath += data.cFileName;

						Enumerate(wsPath.c_str(), ctx);
					}
				}
			}
			if (!FindNextFileW(hFind, &data))
			{
				break;
			}
		}
		FindClose(hFind);
	}

	return Status();
}

Status FileEnumerator2::OnFile(const WChar *wszPath, Size cPathLen, WIN32_FIND_DATAW &data, Context &ctx)
{
	ULARGE_INTEGER   uliSize;
	Size             cFileIndex;
	Status           status;

	uliSize.HighPart = data.nFileSizeHigh;
	uliSize.LowPart  = data.nFileSizeLow;

	ctx.stats.cDiscoveredDirs++;
	ctx.stats.cbDiscoveredFilesSize += uliSize.QuadPart;

	if (ctx.config.cbMinFileSize > uliSize.QuadPart)
	{
		return Status();
	}
	if (ctx.config.cbMaxFileSize < uliSize.QuadPart)
	{
		return Status();
	}

	if (!ctx.config.setExtensions.empty())
	{
		const WChar *wszPos = data.cFileName;
		const WChar *wszExt = NULL;

		while (0 != *wszPos)
		{
			if (L'.' == *wszPos)
			{
				wszExt = wszPos + 1;
			}
			wszPos++;
		}
		if (NULL == wszExt)
		{
			wszExt = L"";
		}
		if (ctx.config.setExtensions.end() == ctx.config.setExtensions.find(wszExt))
		{
			return Status();
		}
	}

	cFileIndex = ctx.threadData[ctx.cCurrentThread].cActualFiles;
	ctx.threadData[ctx.cCurrentThread].files[cFileIndex].data       = data;
	ctx.threadData[ctx.cCurrentThread].files[cFileIndex].wsPath     = wszPath;
	ctx.threadData[ctx.cCurrentThread].files[cFileIndex].cbSize     = uliSize.QuadPart;
	ctx.threadData[ctx.cCurrentThread].files[cFileIndex].cPathIndex = ctx.cPathIndex;
	ctx.threadData[ctx.cCurrentThread].cActualFiles++;
	if (ctx.threadData[ctx.cCurrentThread].cActualFiles == ctx.threadData[ctx.cCurrentThread].cFiles)
	{
		ctx.cCurrentThread++;
		if (ctx.cCurrentThread >= ctx.config.cThreads)
		{
			ProcessFiles(ctx);
			ctx.cCurrentThread = 0;
		}
	}

	return Status();
}

Status FileEnumerator2::ProcessFiles(Context &ctx)
{
	for (Size i = 0; i < ctx.config.cThreads; i++)
	{
		SetEvent(ctx.threadData[i].hEvent);
	}
	WaitForMultipleObjects((DWORD)ctx.config.cThreads, ctx.threadReadyEvents, TRUE, INFINITE);
	for (Size i = 0; i < ctx.config.cThreads; i++)
	{
		if (0 < ctx.threadData[i].cActualResults)
		{
			if (!ctx.pHandler->OnResults(ctx.threadData[i].results, ctx.threadData[i].cActualResults))
			{
				ctx.bRunning = False;
			}
			ctx.threadData[i].cActualResults = 0;
		}
	}

	return Status();
}

DWORD WINAPI FileEnumerator2::WorkerThread(void *pArgs)
{
	ThreadData   *pData = (ThreadData *)pArgs;
	FileImpl     file;
	Size         cInclusionPatterns;
	Bool         bMatched;
	Status       status;

	for (;;)
	{
		WaitForSingleObject(pData->hEvent, INFINITE);
		
		if (!*pData->pbRunning)
		{
			SetEvent(pData->hReadyEvent);

			break;
		}

		//Print(stdout, "{1} : Processing {2} files...\n", GetCurrentThreadId(), pData->cActualFiles);

		pData->cActualResults = 0;
		for (Size i = 0; i < pData->cActualFiles; i++)
		{
			file.m_wszPath       = pData->files[i].wsPath.c_str();
			file.m_cPathLen      = pData->files[i].wsPath.size();
			file.m_pContent      = NULL;
			file.m_cbContentSize = pData->files[i].cbSize;
			file.m_uAttributes   = pData->files[i].data.dwFileAttributes;
			file.m_hFile         = NULL;
			file.m_hFileMapping  = NULL;
			file.m_cPathIndex    = pData->files[i].cPathIndex;

			if (!pData->pConfig->vectorPatterns.empty() || pData->pConfig->bMapFile)
			{
				if ((status = file.Open()))
				{
					continue;
				}
			}

			bMatched = True;
			if (!pData->pConfig->vectorPatterns.empty())
			{
				bMatched           = False;
				cInclusionPatterns = 0;

				for (auto iter = pData->pConfig->vectorPatterns.begin(); 
				     iter != pData->pConfig->vectorPatterns.end(); ++iter)
				{
					if (!iter->bNegate)
					{
						cInclusionPatterns++;
					}
				}

				if (!FindPatterns(pData->pConfig, file.m_cbContentSize, True, pData->pConfig->vectorPatterns))
				{
					if (0 < cInclusionPatterns)
					{
						if (FindPatterns(file.m_pContent, file.m_cbContentSize, False, pData->pConfig->vectorPatterns))
						{
							bMatched = True;
						}
					}
					else
					{
						bMatched = True;
					}
				}
			}

			if (!bMatched)
			{
				file.Close();

				continue;
			}

			if (!pData->pHandler->OnFile(&file, &pData->results[pData->cActualResults], pData->pStats))
			{
				*pData->pbRunning = False;

				break;
			}
			pData->cActualResults++;

			file.Close();
		}

		pData->cActualFiles = 0;

		SetEvent(pData->hReadyEvent);
	}

	return 0;
}

Bool FileEnumerator2::FindPatterns(const void *pFileData, UInt64 cbFileSize, Bool bNegate, 
                                  const Config::PatternsVector &vectorPatterns)
{
	const Byte *pData = (const Byte *)pFileData;

	for (auto iter = vectorPatterns.begin(); iter != vectorPatterns.end(); ++iter)
	{
		if (bNegate == iter->bNegate)
		{
			if (iter->bHasOffset)
			{
				if (iter->cbOffset < cbFileSize && (UInt64)iter->pattern.size() <= cbFileSize - iter->cbOffset)
				{
					if (0 == memcmp(&iter->pattern[0], (const Byte *)pFileData + iter->cbOffset, iter->pattern.size()))
					{
						return True;
					}
				}
			}
			else
			{
				Size   cCount;

				while (cbFileSize >= iter->pattern.size())
				{
					cCount = 0;
					while (iter->pattern.size() > cCount && pData[cCount] == iter->pattern[cCount])
					{
						cCount++;
					}
					if (iter->pattern.size() == cCount)
					{
						return True;
					}
					pData++;
					cbFileSize--;
				}
			}
		}
	}

	return False;
}

FileEnumerator2::FileImpl::FileImpl()
{
	m_wszPath       = NULL;
	m_cPathLen      = NULL;
	m_pContent      = NULL;
	m_cbContentSize = NULL;
	m_uAttributes   = NULL;
	m_hFile         = NULL;
	m_hFileMapping  = NULL;
	m_cPathIndex    = 0;
}

FileEnumerator2::FileImpl::~FileImpl()
{
	Close();
	m_wszPath       = NULL;
	m_cPathLen      = NULL;
	m_pContent      = NULL;
	m_cbContentSize = NULL;
	m_uAttributes   = NULL;
	m_hFile         = NULL;
	m_hFileMapping  = NULL;
	m_cPathIndex    = 0;
}

Status FileEnumerator2::FileImpl::Open()
{
	Status   status;

	for (;;)
	{
		if ((UInt64)((SIZE_T)-1) < m_cbContentSize)
		{
			status = Status(Status_OpenFailed, "File too big to map");

			break;
		}
		if (INVALID_HANDLE_VALUE == (m_hFile = ::CreateFileW(m_wszPath, GENERIC_READ, FILE_SHARE_READ, NULL,
		                                                     OPEN_EXISTING, FILE_FLAG_RANDOM_ACCESS, NULL)))
		{
			m_hFile = NULL;
			status  = Status(Status_OpenFailed, "CreateFileW failed with error {1}", GetLastError());

			break;
		}
		if (NULL == (m_hFileMapping = ::CreateFileMapping(m_hFile, NULL, PAGE_READONLY, 0, 0, NULL)))
		{
			status = Status(Status_OpenFailed, "CreateFileMapping failed with error {1}", GetLastError());

			break;
		}
		if (NULL == (m_pContent = ::MapViewOfFile(m_hFileMapping, FILE_MAP_READ, 0, 0, (SIZE_T)m_cbContentSize)))
		{
			status = Status(Status_OperationFailed, "MapViewOfFile failed with error {1}", GetLastError());

			break;
		}

		break;
	}
	if (!status)
	{
		Close();
	}

	return status;
}

const WChar *FileEnumerator2::FileImpl::GetPath() const
{
	return m_wszPath;
}

Size FileEnumerator2::FileImpl::GetPathLen() const
{
	return m_cPathLen;
}

const void *FileEnumerator2::FileImpl::GetContent() const
{
	return m_pContent;
}

UInt64 FileEnumerator2::FileImpl::GetContentSize() const
{
	return m_cbContentSize;
}

UInt32 FileEnumerator2::FileImpl::GetAttributes() const
{
	return m_uAttributes;
}

HANDLE FileEnumerator2::FileImpl::GetHandle()
{
	return m_hFile;
}

HANDLE FileEnumerator2::FileImpl::GetMappingHandle()
{
	return m_hFileMapping;
}

Size FileEnumerator2::FileImpl::GetPathIndex() const
{
	return m_cPathIndex;
}

void FileEnumerator2::FileImpl::Close()
{
	if (NULL != m_pContent)
	{
		UnmapViewOfFile(m_pContent);
		m_pContent = NULL;
	}
	if (NULL != m_hFileMapping)
	{
		CloseHandle(m_hFileMapping);
		m_hFileMapping = NULL;
	}
	if (NULL != m_hFile)
	{
		CloseHandle(m_hFile);
		m_hFile = NULL;
	}
}

}//namespace IO

}//namespace CX

#endif
