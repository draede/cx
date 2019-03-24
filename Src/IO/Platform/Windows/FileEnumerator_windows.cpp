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
#include "CX/Sys/BOM.hpp"
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
const WChar *FileEnumerator::Config::ARG_XPATTERN      = L"--xpattern";

FileEnumerator::Config::Pattern::Pattern()
{
}

FileEnumerator::Config::Pattern::Pattern(Bool bHasOffset, UInt64 cbOffset, const void *pPattern, Size cbSize, 
                                         Bool bNegate)
{
	this->bHasOffset = bHasOffset;
	this->cbOffset   = cbOffset;
	this->bNegate    = bNegate;
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
		if (0 == cxw_strcmp(ARG_PATTERN, iter->c_str()) || 0 == cxw_strcmp(ARG_XPATTERN, iter->c_str()))
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

FileEnumerator::FileHandlerFile::FileHandlerFile()
{
	m_pConfig       = NULL;
	m_pHandler      = NULL;
	m_pStats        = NULL;
	m_wszPath       = NULL;
	m_cPathLen      = NULL;
	m_pContent      = NULL;
	m_cbContentSize = NULL;
	m_uAttributes   = NULL;
	m_hFile         = NULL;
	m_hFileMapping  = NULL;
}

FileEnumerator::FileHandlerFile::~FileHandlerFile()
{
	Close();
	m_pConfig       = NULL;
	m_pHandler      = NULL;
	m_pStats        = NULL;
	m_wszPath       = NULL;
	m_cPathLen      = NULL;
	m_pContent      = NULL;
	m_cbContentSize = NULL;
	m_uAttributes   = NULL;
	m_hFile         = NULL;
	m_hFileMapping  = NULL;
}

Status FileEnumerator::FileHandlerFile::Open()
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

const WChar *FileEnumerator::FileHandlerFile::GetPath() const
{
	return m_wszPath;
}

Size FileEnumerator::FileHandlerFile::GetPathLen() const
{
	return m_cPathLen;
}

const void *FileEnumerator::FileHandlerFile::GetContent() const
{
	return m_pContent;
}

UInt64 FileEnumerator::FileHandlerFile::GetContentSize() const
{
	return m_cbContentSize;
}

UInt32 FileEnumerator::FileHandlerFile::GetAttributes() const
{
	return m_uAttributes;
}

HANDLE FileEnumerator::FileHandlerFile::GetHandle()
{
	return m_hFile;
}

HANDLE FileEnumerator::FileHandlerFile::GetMappingHandle()
{
	return m_hFileMapping;
}

void FileEnumerator::FileHandlerFile::Close()
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
	LARGE_INTEGER      liQPerf;
	Status             status;

	QueryPerformanceFrequency(&liQPerf);
	stats.nPerfCounterFreq = liQPerf.QuadPart;
	QueryPerformanceCounter(&liQPerf);
	stats.nPerfCounterStart = liQPerf.QuadPart;

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
		if (0 == cxw_strnicmp(iter->c_str(), L"list:", 5))
		{
			FILE               *pFile;
			Size               cbBOMSize;
			WChar              *wszLine;
			Size               cLen;
			Byte               bom[MAX_BOM_SIZE];
			Sys::BOM::Type     nType;

			if (NULL != (pFile = _wfopen(iter->c_str() + 5, L"rb")))
			{
				if (NULL != (wszLine = new WChar[MAX_LINE_LEN]))
				{
					memset(wszLine, 0, sizeof(WChar) * MAX_LINE_LEN);
					if (sizeof(bom) == fread(&bom, 1, sizeof(bom), pFile))
					{
						if (Sys::BOM::UTF16_LE == (nType = Sys::BOM::Get(bom, sizeof(bom), &cbBOMSize)))
						{
							fseek(pFile, (long)cbBOMSize, SEEK_SET);
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
										if (INVALID_HANDLE_VALUE != (hFind = FindFirstFileW(wszLine, &data)))
										{
											FindClose(hFind);

											OnFile(wszLine, cLen, &data, pHandler, &tp, config, &stats);
										}
									}
								}
							}
						}
					}
					delete [] wszLine;
				}
				fclose(pFile);
			}
		}
		else
		{
			dwAttr = GetFileAttributesW(iter->c_str());
			if (INVALID_FILE_ATTRIBUTES == dwAttr || FILE_ATTRIBUTE_DIRECTORY == (FILE_ATTRIBUTE_DIRECTORY & dwAttr))
			{
				Enumerate(iter->c_str(), pHandler, &tp, config, &stats);
			}
			else
			{
				if (INVALID_HANDLE_VALUE != (hFind = FindFirstFileW(iter->c_str(), &data)))
				{
					FindClose(hFind);

					wsPath = iter->c_str();
					wsPath += L"\\";
					wsPath += data.cFileName;

					OnFile(wsPath.c_str(), wsPath.size(), &data, pHandler, &tp, config, &stats);
				}
			}
		}
	}

	tp.Stop();

	pHandler->OnEnd(&stats);

	return Status();
}

void FileEnumerator::Enumerate(const WChar *wszPath, IHandler *pHandler, void *pTP, const Config &config, 
                               IHandler::Stats *pStats)
{
	WIN32_FIND_DATAW   data;
	HANDLE             hFind;
	WString            wsPath;
	WString            wsMask;

	InterlockedIncrement64(&pStats->cAllDirs);

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

						Enumerate(wsPath.c_str(), pHandler, pTP, config, pStats);
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
	Sys::ThreadPool          *pThreadPool  = (Sys::ThreadPool *)pTP;
	ULARGE_INTEGER           uliSize;
	FileHandlerFile          *pFile;
	Status                   status;

	for (;;)
	{
		uliSize.HighPart = pData->nFileSizeHigh;
		uliSize.LowPart  = pData->nFileSizeLow;

		InterlockedIncrement64(&pStats->cAllFiles);
		InterlockedAdd64(&pStats->cbAllFilesSize, uliSize.QuadPart);

		if (config.cbMinFileSize > uliSize.QuadPart)
		{
			InterlockedIncrement64(&pStats->cExcludedFiles);
			InterlockedAdd64(&pStats->cbExcludedFilesSize, uliSize.QuadPart);

			InterlockedIncrement64(&pStats->cExcludedTooSmallFiles);
			InterlockedAdd64(&pStats->cbExcludedTooSmallFilesSize, uliSize.QuadPart);

			break;
		}
		if (config.cbMaxFileSize < uliSize.QuadPart)
		{
			InterlockedIncrement64(&pStats->cExcludedFiles);
			InterlockedAdd64(&pStats->cbExcludedFilesSize, uliSize.QuadPart);

			InterlockedIncrement64(&pStats->cExcludedTooBigFiles);
			InterlockedAdd64(&pStats->cbExcludedTooBigFilesSize, uliSize.QuadPart);

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
				InterlockedIncrement64(&pStats->cExcludedFiles);
				InterlockedAdd64(&pStats->cbExcludedFilesSize, uliSize.QuadPart);

				InterlockedIncrement64(&pStats->cExcludedExtensionFiles);
				InterlockedAdd64(&pStats->cbExcludedExtensionFilesSize, uliSize.QuadPart);

				break;
			}
		}

		if (NULL != (pFile = (FileHandlerFile *)Mem::Alloc(sizeof(FileHandlerFile) + sizeof(WChar) * (cPathLen + 1))))
		{
			new (pFile) FileHandlerFile();

			memcpy(pFile + 1, wszPath, sizeof(WChar) * (cPathLen + 1));

			pFile->m_pConfig       = &config;
			pFile->m_pHandler      = pHandler;
			pFile->m_pStats        = pStats;
			pFile->m_wszPath       = (const WChar *)(pFile + 1);
			pFile->m_cPathLen      = cPathLen;
			pFile->m_cbContentSize = uliSize.QuadPart;

			if ((status = pThreadPool->AddWork(&FileEnumerator::WorkCallback, pFile)))
			{
				InterlockedIncrement64(&pStats->cProcessedFiles);
				InterlockedAdd64(&pStats->cbProcessedFilesSize, uliSize.QuadPart);
			}
			else
			{
				InterlockedIncrement64(&pStats->cErrorProcessingFiles);
				InterlockedAdd64(&pStats->cbErrorProcessingFilesSize, uliSize.QuadPart);

				pFile->~FileHandlerFile();
				Mem::Free(pFile);
			}
		}
		else
		{
			InterlockedIncrement64(&pStats->cErrorMemAllocFiles);
			InterlockedAdd64(&pStats->cbErrorMemAllocFilesSize, uliSize.QuadPart);
		}

		break;
	}
}

Bool FileEnumerator::FindPatterns(const void *pFileData, UInt64 cbFileSize, Bool bNegate, 
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

VOID CALLBACK FileEnumerator::WorkCallback(PTP_CALLBACK_INSTANCE pInstance, PVOID pContext, PTP_WORK pWork)
{
	CX_UNUSED(pInstance);
	CX_UNUSED(pWork);

	FileHandlerFile   *pFile = (FileHandlerFile *)pContext;
	Size              cInclusionPatterns;
	Status            status;

	for (;;)
	{
		cInclusionPatterns = 0;
		for (auto iter = pFile->m_pConfig->vectorPatterns.begin(); iter != pFile->m_pConfig->vectorPatterns.end(); ++iter)
		{
			if (!iter->bNegate)
			{
				cInclusionPatterns++;
			}
		}

		if (!pFile->m_pConfig->vectorPatterns.empty() || pFile->m_pConfig->bMapFile)
		{
			if (!(status = pFile->Open()))
			{
				break;
			}
		}

		if (!pFile->m_pConfig->vectorPatterns.empty())
		{
			Bool   bMatched = False;

			if (!FindPatterns(pFile->m_pConfig, pFile->m_cbContentSize, True, pFile->m_pConfig->vectorPatterns))
			{
				if (0 < cInclusionPatterns)
				{
					if (FindPatterns(pFile->m_pContent, pFile->m_cbContentSize, False, pFile->m_pConfig->vectorPatterns))
					{
						bMatched = True;
					}
				}
				else
				{
					bMatched = True;
				}
			}

			if (!bMatched)
			{
				InterlockedIncrement64(&pFile->m_pStats->cErrorMemAllocFiles);
				InterlockedAdd64(&pFile->m_pStats->cbExcludedFilesSize, pFile->m_cbContentSize);

				InterlockedIncrement64(&pFile->m_pStats->cExcludedPatternFiles);
				InterlockedAdd64(&pFile->m_pStats->cbExcludedPatternFilesSize, pFile->m_cbContentSize);

				break;
			}
		}

		InterlockedIncrement64(&pFile->m_pStats->cIncludedFiles);
		InterlockedAdd64(&pFile->m_pStats->cbIncludedFilesSize, pFile->m_cbContentSize);

		pFile->m_pHandler->OnFile(pFile, pFile->m_pStats);

		break;
	}


	pFile->~FileHandlerFile();
	Mem::Free(pFile);
}

}//namespace IO

}//namespace CX


#endif
