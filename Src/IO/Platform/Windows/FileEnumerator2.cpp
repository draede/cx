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

#include "CX/precomp.hpp"
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
	Status             status;

	ctx.bRunning                           = True;
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

	for (;;)
	{
		if (NULL == (ctx.files = new (std::nothrow) File[config.cQueuedFiles]))
		{
			status = Status(Status_MemAllocFailed, "Failed to allocate threads data");

			break;
		}
		ctx.cFiles = 0;
		if (!(status = ctx.threads.Start(ctx.config.cThreads)))
		{
			break;
		}

		if (!pHandler->OnBegin())
		{
			status = Status_Cancelled;

			break;
		}

		for (Size cPathIndex = 0; cPathIndex < vectorPaths.size(); cPathIndex++)
		{
			if (!ctx.bRunning)
			{
				status = Status_Cancelled;

				break;
			}
			ctx.cPathIndex = cPathIndex;

			if (!pHandler->OnBeginPath(vectorPaths[cPathIndex].c_str(), cPathIndex))
			{
				status = Status_Cancelled;
			}

			if (status)
			{
				if (0 == cxw_strnicmp(vectorPaths[cPathIndex].c_str(), L"list:", 5))
				{
					if (!(status = RunWithList(vectorPaths[cPathIndex].c_str() + 5, ctx)))
					{
						if (!pHandler->OnError(status))
						{
							status = Status_Cancelled;
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
						}
					}
				}
			}

			if (!pHandler->OnEndPath(vectorPaths[cPathIndex].c_str(), cPathIndex))
			{
				status = Status_Cancelled;
			}

			if (!status)
			{
				break;
			}
		}
		if (!status)
		{
			break;
		}

		break;
	}

	status = ProcessFiles(ctx);

	ctx.threads.Stop();

	if (!pHandler->OnEnd(&ctx.stats))
	{
		status = Status_Cancelled;
	}

	ctx.bRunning = False;

	if (NULL != ctx.files)
	{
		delete [] ctx.files;
	}

	return status;
}

Status FileEnumerator2::RunWithPath(const WChar *wszPath, Context &ctx)
{
	WIN32_FILE_ATTRIBUTE_DATA   data;

	if (!GetFileAttributesExW(wszPath, GetFileExInfoStandard, &data))
	{
		return Status_InvalidArg;
	}

	if (FILE_ATTRIBUTE_DIRECTORY == (FILE_ATTRIBUTE_DIRECTORY & data.dwFileAttributes))
	{
		return Enumerate(wszPath, ctx);
	}
	else
	{
		ULARGE_INTEGER   uliSize;

		uliSize.HighPart = data.nFileSizeHigh;
		uliSize.LowPart  = data.nFileSizeLow;

		return OnFile(wszPath, cxw_strlen(wszPath), uliSize.QuadPart, ctx);
	}
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
		delete [] szLine;

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
					//nothing to do here
				}
			}
		}
	}

	delete [] wszLine;
	delete [] szLine;

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

	delete [] wszLine;

	return status;
}

Status FileEnumerator2::Enumerate(const WChar *wszPath, Context &ctx)
{
	if (!ctx.pHandler->OnFolder(wszPath))
	{
		return Status_Cancelled;
	}

	struct Data
	{
		WIN32_FIND_DATAW   data;
		HANDLE             hFind;
		WString            wsPath;
		WString            wsMask;
		ULARGE_INTEGER     uliSize;
	};

	Data     *pData;
	Status   status;

	if (NULL == (pData = new (std::nothrow) Data()))
	{
		return Status_MemAllocFailed;
	}

	ctx.stats.cDiscoveredDirs++;

	if (ctx.pHandler->OnFolder(wszPath))
	{
		pData->wsMask = wszPath;
		pData->wsMask += L"\\*.*";
		if (INVALID_HANDLE_VALUE != (pData->hFind = FindFirstFileW(pData->wsMask.c_str(), &pData->data)))
		{
			for (;;)
			{
				if (!(FILE_ATTRIBUTE_REPARSE_POINT & pData->data.dwFileAttributes) &&
				    !(FILE_ATTRIBUTE_OFFLINE & pData->data.dwFileAttributes) &&
				    !(0x00400000 & pData->data.dwFileAttributes) && //FILE_ATTRIBUTE_RECALL_ON_DATA_ACCESS
				    !(0x00040000 & pData->data.dwFileAttributes)) //FILE_ATTRIBUTE_RECALL_ON_OPEN
				{
					if (!(FILE_ATTRIBUTE_DIRECTORY & pData->data.dwFileAttributes))
					{
						pData->wsPath = wszPath;
						pData->wsPath += L"\\";
						pData->wsPath += pData->data.cFileName;

						pData->uliSize.HighPart = pData->data.nFileSizeHigh;
						pData->uliSize.LowPart  = pData->data.nFileSizeLow;

						if (!(status = OnFile(pData->wsPath.c_str(), pData->wsPath.size(), pData->uliSize.QuadPart, ctx)))
						{
							break;
						}
					}
					else
					if (0 != wcscmp(pData->data.cFileName, L".") && 0 != wcscmp(pData->data.cFileName, L".."))
					{
						if (ctx.config.bRecursive)
						{
							pData->wsPath = wszPath;
							pData->wsPath += L"\\";
							pData->wsPath += pData->data.cFileName;

							if (!( status = Enumerate(pData->wsPath.c_str(), ctx)))
							{
								break;
							}
						}
					}
				}
				if (!FindNextFileW(pData->hFind, &pData->data))
				{
					break;
				}
			}
			FindClose(pData->hFind);
		}
	}

	delete pData;

	return status;
}

Status FileEnumerator2::OnFile(const WChar *wszPath, Size cPathLen, UInt64 cbSize, Context &ctx)
{
	CX_UNUSED(cPathLen);

	Status           status;

	ctx.stats.cDiscoveredFiles++;
	ctx.stats.cbDiscoveredFilesSize += cbSize;

	if (ctx.config.cbMinFileSize > cbSize)
	{
		return Status();
	}
	if (ctx.config.cbMaxFileSize < cbSize)
	{
		return Status();
	}

	if (!ctx.config.setExtensions.empty())
	{
		const WChar *wszPos = wszPath;
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

	ctx.files[ctx.cFiles].pCTX       = &ctx;
	ctx.files[ctx.cFiles].wsPath     = wszPath;
	ctx.files[ctx.cFiles].cbSize     = cbSize;
	ctx.files[ctx.cFiles].cPathIndex = ctx.cPathIndex;
	ctx.files[ctx.cFiles].pResult    = NULL;
	ctx.cFiles++;

	if (ctx.cFiles == ctx.config.cQueuedFiles)
	{
		ProcessFiles(ctx);
	}
	if (!ctx.bRunning)
	{
		return Status_Cancelled;
	}

	return Status();
}

Status FileEnumerator2::ProcessFiles(Context &ctx)
{
	void   *pContext = NULL;

	ctx.threads.RunJobs(ctx.files, ctx.cFiles, sizeof(File), &FileEnumerator2::HandleFileJob);

	if (!ctx.pHandler->OnBeginResults(&pContext))
	{
		ctx.bRunning = False;
	}
	for (Size i = 0; i < ctx.cFiles; i++)
	{
		if (NULL != ctx.files[i].pResult)
		{
			if (!ctx.pHandler->OnResult(ctx.files[i].pResult, pContext))
			{
				ctx.bRunning = False;
			}
		}
	}
	ctx.cFiles = 0;
	if (!ctx.pHandler->OnEndResults(pContext))
	{
		ctx.bRunning = False;
	}

	return Status();
}

Bool FileEnumerator2::HandleFileJob(void *pJob, Size cbSize)
{
	CX_UNUSED(cbSize);

	File      *pFile = (File *)pJob;
	Context   *pCTX  = (Context *)pFile->pCTX;
	Bool      bMatched;
	Size      cInclusionPatterns;
	Status    status;

	pFile->file.m_wszPath       = pFile->wsPath.c_str();
	pFile->file.m_cPathLen      = pFile->wsPath.size();
	pFile->file.m_pContent      = NULL;
	pFile->file.m_cbContentSize = pFile->cbSize;
	pFile->file.m_hFile         = NULL;
	pFile->file.m_hFileMapping  = NULL;
	pFile->file.m_cPathIndex    = pFile->cPathIndex;

	if (!pCTX->config.vectorPatterns.empty() ||pCTX->config.bMapFile)
	{
		if (!(status = pFile->file.Open()))
		{
			return pCTX->bRunning;
		}
	}

	bMatched = True;
	if (!pCTX->config.vectorPatterns.empty())
	{
		bMatched           = False;
		cInclusionPatterns = 0;

		for (auto iter = pCTX->config.vectorPatterns.begin(); iter != pCTX->config.vectorPatterns.end(); ++iter)
		{
			if (!iter->bNegate)
			{
				cInclusionPatterns++;
			}
		}

		if (!FindPatterns(&pCTX->config, pFile->file.m_cbContentSize, True, pCTX->config.vectorPatterns))
		{
			if (0 < cInclusionPatterns)
			{
				if (FindPatterns(pFile->file.m_pContent, pFile->file.m_cbContentSize, False, pCTX->config.vectorPatterns))
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

	if (bMatched)
	{
		if (!pCTX->pHandler->OnFile(&pFile->file, &pFile->pResult, &pCTX->stats))
		{
			pCTX->bRunning = False;
		}
	}

	pFile->file.Close();

	return pCTX->bRunning;
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

//===

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
