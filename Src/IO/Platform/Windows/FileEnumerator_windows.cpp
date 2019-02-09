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


#include "CX/IO/Platform/Windows/FileEnumerator.hpp"
#include "CX/Sys/Platform/Windows/ThreadPool.hpp"
#include "CX/Str/HexBinStr.hpp"
#include "CX/Str/UTF8.hpp"
#include "CX/C/string.h"
#include <sstream>


namespace CX
{

namespace IO
{

const WChar *FileEnumerator::Config::ARG_THREADS       = L"--threads";
const WChar *FileEnumerator::Config::ARG_MIN_FILE_SIZE = L"--minfilesize";
const WChar *FileEnumerator::Config::ARG_MAX_FILE_SIZE = L"--maxfilesize";
const WChar *FileEnumerator::Config::ARG_RECURSIVE     = L"--recursive";
const WChar *FileEnumerator::Config::ARG_EXTENSION     = L"--extension";
const WChar *FileEnumerator::Config::ARG_PATTERN       = L"--pattern";

FileEnumerator::Config::Pattern::Pattern()
{
}

FileEnumerator::Config::Pattern::Pattern(UInt64 cbOffset, const void *pPattern, Size cbSize)
{
	this->cbOffset = cbOffset;
	SetupPattern(pPattern, cbSize);
}

Bool FileEnumerator::Config::Pattern::SetupPattern(const void *pPattern, Size cbSize)
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

const FileEnumerator::Config &FileEnumerator::Config::GetDefault()
{
	static Config   config =
	{
		DEFAULT_THREADS,
		DEFAULT_MIN_FILE_SIZE,
		DEFAULT_MAX_FILE_SIZE,
		DEFAULT_RECURSIVE,
		DEFAULT_MAP_FILE,
	};

	return config;
};

Status FileEnumerator::Config::FromCmdLine(ArgsVector &vectorArgs, Size cStartArg/* = 0*/)
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
			iter = vectorArgs.erase(iter);
			if (vectorArgs.end() == iter)
			{
				status = Status(Status_InvalidArg, "No offset specified for {1}", ARG_PATTERN);

				break;
			}

			Pattern              pattern;
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
			if (L'x' == *iter->c_str())
			{
				if (MAX_PATTERN_SIZE + 1 < iter->size() || 1 != (iter->size() % 2))
				{
					status = Status(Status_InvalidArg, "Invalid pattern specified for {1}", ARG_PATTERN);

					break;
				}

				Str::HexBinStr   hbs;
				String           sPattern;
				Byte             hex[MAX_PATTERN_SIZE];

				if (!(status = Str::UTF8::FromWChar(iter->c_str() + 1, &sPattern)))
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

				if (!(status = Str::UTF8::FromWChar(iter->c_str(), &sPattern)))
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

FileEnumerator::FileEnumerator()
{
}

FileEnumerator::~FileEnumerator()
{
}

Status FileEnumerator::Run(const WChar *wszPath, IHandler *pHandler, const Config &config/* = Config::GetDefault()*/)
{
	PathsVector   vectorPaths;

	vectorPaths.push_back(wszPath);

	return Run(vectorPaths, pHandler, config);
}

Status FileEnumerator::Run(const PathsVector &vectorPaths, IHandler *pHandler, 
                           const Config &config/* = Config::GetDefault()*/)
{
	Sys::ThreadPool    tp;
	WIN32_FIND_DATAW   data;
	HANDLE             hFind;
	DWORD              dwAttr;
	WString            wsPath;
	IHandler::Stats    stats;
	Util::Timer        timer;
	Status             status;

	stats.cAllDirs                     = 0;
	stats.cAllFiles                    = 0;
	stats.cbAllFilesSize               = 0;
	stats.cExcludedFiles               = 0;
	stats.cbExcludedFilesSize          = 0;
	stats.cExcludedTooSmallFiles       = 0;
	stats.cbExcludedTooSmallFilesSize  = 0;
	stats.cExcludedTooBigFiles         = 0;
	stats.cbExcludedTooBigFilesSize    = 0;
	stats.cExcludedExtensionFiles      = 0;
	stats.cbExcludedExtensionFilesSize = 0;
	stats.cExcludedPatternFiles        = 0;
	stats.cbExcludedPatternFilesSize   = 0;
	stats.cIncludedFiles               = 0;
	stats.cbIncludedFilesSize          = 0;
	stats.cProcessedFiles              = 0;
	stats.cbProcessedFilesSize         = 0;
	stats.cErrorMemAllocFiles          = 0;
	stats.cbErrorMemAllocFilesSize     = 0;
	stats.cErrorProcessingFiles        = 0;
	stats.cbErrorProcessingFilesSize   = 0;
	stats.lfElapsedTime                = 0.0;

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

	if (!(status = tp.Start(config.cThreads)))
	{
		return status;
	}

	pHandler->OnBegin();
	for (auto iter = vectorPaths.begin(); iter != vectorPaths.end(); ++iter)
	{
		dwAttr = GetFileAttributesW(iter->c_str());
		if (INVALID_FILE_ATTRIBUTES == dwAttr || FILE_ATTRIBUTE_DIRECTORY == (FILE_ATTRIBUTE_DIRECTORY & dwAttr))
		{
			Enumerate(iter->c_str(), pHandler, &tp, config, &stats, &timer);
		}
		else
		{
			if (INVALID_HANDLE_VALUE != (hFind = FindFirstFileW(iter->c_str(), &data)))
			{
				FindClose(hFind);

				wsPath = iter->c_str();
				wsPath += L"\\";
				wsPath += data.cFileName;

				stats.lfElapsedTime = timer.GetElapsedTime();
				OnFile(wsPath.c_str(), wsPath.size(), &data, pHandler, &tp, config, &stats);
			}
		}
	}

	tp.Stop();

	stats.lfElapsedTime = timer.GetElapsedTime();
	pHandler->OnEnd(&stats);

	return Status();
}

void FileEnumerator::Enumerate(const WChar *wszPath, IHandler *pHandler, void *pTP, const Config &config, 
                               IHandler::Stats *pStats, Util::Timer *pTimer)
{
	WIN32_FIND_DATAW   data;
	HANDLE             hFind;
	WString            wsPath;
	WString            wsMask;

	pStats->cAllDirs++;

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

					pStats->lfElapsedTime = pTimer->GetElapsedTime();
					OnFile(wsPath.c_str(), wsPath.size(), &data, pHandler, pTP, config, pStats);
				}
				else
				if (0 != wcscmp(data.cFileName, L".") && 0 != wcscmp(data.cFileName, L".."))
				{
					if (config.bRecursive)
					{
						wsPath = wszPath;
						wsPath += L"\\";
						wsPath += data.cFileName;

						Enumerate(wsPath.c_str(), pHandler, pTP, config, pStats, pTimer);
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
}

void FileEnumerator::OnFile(const WChar *wszPath, Size cPathLen, const void *pFindData, IHandler *pHandler, 
                            void *pTP, const Config &config, IHandler::Stats *pStats)
{
	const WIN32_FIND_DATAW   *pData       = (const WIN32_FIND_DATAW *)pFindData;
	Sys::ThreadPool         *pThreadPool  = (Sys::ThreadPool *)pTP;
	ULARGE_INTEGER           uliSize;
	Arg                      *pArg;
	HANDLE                   hFile        = NULL;
	HANDLE                   hFileMapping = NULL;
	const void               *pFileData   = NULL;
	Status                   status;

	for (;;)
	{
		uliSize.HighPart = pData->nFileSizeHigh;
		uliSize.LowPart  = pData->nFileSizeLow;

		pStats->cAllFiles++;
		pStats->cbAllFilesSize += uliSize.QuadPart;

		if (config.cbMinFileSize > uliSize.QuadPart)
		{
			pStats->cExcludedFiles++;
			pStats->cbExcludedFilesSize += uliSize.QuadPart;

			pStats->cExcludedTooSmallFiles++;
			pStats->cbExcludedTooSmallFilesSize += uliSize.QuadPart;

			break;
		}
		if (config.cbMaxFileSize < uliSize.QuadPart)
		{
			pStats->cExcludedFiles++;
			pStats->cbExcludedFilesSize += uliSize.QuadPart;

			pStats->cExcludedTooBigFiles++;
			pStats->cbExcludedTooBigFilesSize += uliSize.QuadPart;

			break;
		}

		if (!config.setExtensions.empty())
		{
			const WChar *wszPos = pData->cFileName;
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
			if (config.setExtensions.end() == config.setExtensions.find(wszExt))
			{
				pStats->cExcludedFiles++;
				pStats->cbExcludedFilesSize += uliSize.QuadPart;

				pStats->cExcludedExtensionFiles++;
				pStats->cbExcludedExtensionFilesSize += uliSize.QuadPart;

				break;
			}
		}

		if (!config.vectorPatterns.empty())
		{
			if (INVALID_HANDLE_VALUE == (hFile = ::CreateFileW(wszPath, GENERIC_READ, FILE_SHARE_READ, NULL, 
			                                                   OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL)))
			{
				hFile  = NULL;
				status = Status(Status_OpenFailed, "CreateFileW failed with error {1}", GetLastError());

				break;
			}
			if (NULL == (hFileMapping = ::CreateFileMapping(hFile, NULL, PAGE_READONLY, 0, 0, NULL)))
			{
				status = Status(Status_OpenFailed, "CreateFileMapping failed with error {1}", GetLastError());

				break;
			}
			if (NULL == (pFileData = ::MapViewOfFile(hFileMapping, FILE_MAP_READ, 0, 0, 0)))
			{
				status = Status(Status_OperationFailed, "MapViewOfFile failed with error {1}", GetLastError());

				break;
			}

			Bool   bFound = False;

			for (auto iter = config.vectorPatterns.begin(); iter != config.vectorPatterns.end(); ++iter)
			{
				if (iter->cbOffset < uliSize.QuadPart && 
				    (UInt64)iter->pattern.size() <= uliSize.QuadPart - iter->cbOffset)
				{
					if (0 == memcmp(&iter->pattern[0], (const Byte *)pFileData + iter->cbOffset, iter->pattern.size()))
					{
						bFound = True;

						break;
					}
				}
			}
			if (!bFound)
			{
				pStats->cExcludedFiles++;
				pStats->cbExcludedFilesSize += uliSize.QuadPart;

				pStats->cExcludedPatternFiles++;
				pStats->cbExcludedPatternFilesSize += uliSize.QuadPart;

				break;
			}
		}

		pStats->cIncludedFiles++;
		pStats->cbIncludedFilesSize += uliSize.QuadPart;

		if (NULL != (pArg = (Arg *)Mem::Alloc(sizeof(Arg) + sizeof(WChar) * cPathLen)))
		{
			pArg->pHandler     = pHandler;
			pArg->pStats       = pStats;
			pArg->cbSize       = uliSize.QuadPart;
			pArg->cPathLen     = cPathLen;
			pArg->hFile        = hFile;
			pArg->hFileMapping = hFileMapping;
			pArg->pFileData    = pFileData;
			memcpy(pArg->wszPath, wszPath, sizeof(WChar) * (cPathLen + 1));
			if (!(status = pThreadPool->AddWork(&FileEnumerator::WorkCallback, pArg)))
			{
				pStats->cErrorProcessingFiles++;
				pStats->cbErrorProcessingFilesSize += uliSize.QuadPart;

				if (NULL != pFileData)
				{
					UnmapViewOfFile(pFileData);
					pFileData = NULL;
				}
				if (NULL != hFileMapping)
				{
					CloseHandle(hFileMapping);
					hFileMapping = NULL;
				}
				if (NULL != hFile)
				{
					CloseHandle(hFile);
					hFile = NULL;
				}
				Mem::Free(pArg);
			}
			else
			{
				pStats->cProcessedFiles++;
				pStats->cbProcessedFilesSize += uliSize.QuadPart;

				pFileData    = NULL;
				hFileMapping = NULL;
				hFile        = NULL;
			}
		}
		else
		{
			pStats->cErrorMemAllocFiles++;
			pStats->cbErrorMemAllocFilesSize += uliSize.QuadPart;
		}

		break;
	}
	if (NULL != pFileData)
	{
		UnmapViewOfFile(pFileData);
		pFileData = NULL;
	}
	if (NULL != hFileMapping)
	{
		CloseHandle(hFileMapping);
		hFileMapping = NULL;
	}
	if (NULL != hFile)
	{
		CloseHandle(hFile);
		hFile = NULL;
	}
}

VOID CALLBACK FileEnumerator::WorkCallback(PTP_CALLBACK_INSTANCE pInstance, PVOID pContext, PTP_WORK pWork)
{
	CX_UNUSED(pInstance);
	CX_UNUSED(pWork);

	Arg   *pArg = (Arg *)pContext;

	pArg->pHandler->OnFile(pArg->wszPath, pArg->cPathLen, pArg->cbSize, pArg->pFileData, pArg->pStats);

	if (NULL != pArg->pFileData)
	{
		UnmapViewOfFile(pArg->pFileData);
	}
	if (NULL != pArg->hFileMapping)
	{
		CloseHandle(pArg->hFileMapping);
	}
	if (NULL != pArg->hFile)
	{
		CloseHandle(pArg->hFile);
	}
	Mem::Free(pArg);
}

}//namespace IO

}//namespace CX


#endif
