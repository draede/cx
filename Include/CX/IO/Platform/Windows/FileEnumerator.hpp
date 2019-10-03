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

#pragma once


#include "CX/Platform.hpp"


#if defined(CX_OS_WINDOWS)


#include "CX/Types.hpp"
#include "CX/Status.hpp"
#include "CX/Vector.hpp"
#include "CX/Set.hpp"
#include "CX/String.hpp"
#include "CX/C/Platform/Windows/windows.h"
#include "CX/BLOB.hpp"
#include "CX/APIDefs.hpp"


namespace CX
{

namespace IO
{

class CX_API FileEnumerator
{
public:

	struct Config
	{

		static const Size     MIN_THREADS              = 1;
		static const Size     MAX_THREADS              = 64;

		static const UInt64   MIN_FILE_SIZE            = 0;
		static const UInt64   MAX_FILE_SIZE            = 70368744177664; //64 TB ... yeah right

		static const Size     MIN_PATTERN_SIZE         = 1;
		static const Size     MAX_PATTERN_SIZE         = 1024;

		static const Size     DEFAULT_THREADS           = 1;
		static const Size     DEFAULT_MAX_WAITING_FILES = 0;
		static const UInt64   DEFAULT_MIN_FILE_SIZE     = 0;
		static const UInt64   DEFAULT_MAX_FILE_SIZE     = 268435456; //256 MB;
		static const Bool     DEFAULT_RECURSIVE         = True;
		static const Bool     DEFAULT_MAP_FILE          = False; //will still be mapped anyway if patterns are set

		static const WChar    *ARG_THREADS;           //--threads <number_of_threads : size>
		static const WChar    *ARG_MAX_WAITING_FILES; //--maxwaitfiles <number_of_max_wait_files : size>
		static const WChar    *ARG_MIN_FILE_SIZE;     //--minfilesize <min_file_size : uint64>
		static const WChar    *ARG_MAX_FILE_SIZE;     //--maxfilesize <max_file_size : uint64>
		static const WChar    *ARG_RECURSIVE;         //--recursive <yes|no>
		static const WChar    *ARG_EXTENSION;         //--extension <extension:string (e.g. : exe)>
		static const WChar    *ARG_PATTERN;           //--pattern <offset:uint64> [!]<pattern:string_or_hexstring e.g. 0xAABBCC>
		static const WChar    *ARG_XPATTERN;          //--xpattern [!]<pattern:string_or_hexstring e.g. 0xAABBCC>

		typedef Set<WString, WCaseInsensitiveOrderPolicy>::Type   ExtensionsSet;

		struct Pattern
		{
			Bool     bHasOffset;
			UInt64   cbOffset;
			BLOB     pattern;
			Bool     bNegate;

			Pattern();

			Pattern(Bool bHasOffset, UInt64 cbOffset, const void *pPattern, Size cbSize, Bool bNegate);

			Bool SetupPattern(const void *pPattern, Size cbSize);
		};

		typedef Vector<Pattern>::Type                             PatternsVector;

		typedef Vector<WString>::Type                             ArgsVector;

		UInt32           cThreads;
		UInt32           cMaxWaitFiles;
		UInt64           cbMinFileSize;
		UInt64           cbMaxFileSize;
		Bool             bRecursive;
		Bool             bMapFile;
		PatternsVector   vectorPatterns;
		ExtensionsSet    setExtensions;

		static const Config &GetDefault();

		Status FromCmdLine(ArgsVector &vectorArgs, Size cStartArg = 0);

	};

	class IHandler
	{
	public:

		class IFile
		{
		public:

			~IFile() { }

			virtual const WChar *GetPath() const = 0;

			virtual Size GetPathLen() const = 0;

			virtual const void *GetContent() const = 0;

			virtual UInt64 GetContentSize() const = 0;

			virtual UInt32 GetAttributes() const = 0;

			virtual HANDLE GetHandle() = 0;

			virtual HANDLE GetMappingHandle() = 0;

			virtual Size GetEnumPathIndex() const = 0;

			virtual Status Open() = 0;

			virtual void Close() = 0;

		};

		struct Stats
		{
			volatile Int64     cAllDirs;
			
			volatile Int64     cAllFiles;
			volatile Int64     cbAllFilesSize;

			volatile Int64     cExcludedFiles;
			volatile Int64     cbExcludedFilesSize;

			volatile Int64     cExcludedTooSmallFiles;
			volatile Int64     cbExcludedTooSmallFilesSize;

			volatile Int64     cExcludedTooBigFiles;
			volatile Int64     cbExcludedTooBigFilesSize;

			volatile Int64     cExcludedExtensionFiles;
			volatile Int64     cbExcludedExtensionFilesSize;

			volatile Int64     cExcludedPatternFiles;
			volatile Int64     cbExcludedPatternFilesSize;

			volatile Int64     cIncludedFiles;
			volatile Int64     cbIncludedFilesSize;

			volatile Int64     cProcessedFiles;
			volatile Int64     cbProcessedFilesSize;

			volatile Int64     cErrorMemAllocFiles;
			volatile Int64     cbErrorMemAllocFilesSize;

			volatile Int64     cErrorProcessingFiles;
			volatile Int64     cbErrorProcessingFilesSize;

			Int64              nPerfCounterFreq;
			Int64              nPerfCounterStart;
		};

		~IHandler() { }

		virtual void OnBegin() = 0;

		virtual void OnFile(IFile *pFile, const Stats *pStats) = 0;

		virtual void OnEnd(const Stats *pStats) = 0;

	};

	typedef Vector<WString>::Type   PathsVector;

	static const Size MAX_THREADS = 64;

	static Status Run(const WChar *wszPath, IHandler *pHandler, const Config &config = Config::GetDefault());

	static Status Run(const PathsVector &vectorPaths, IHandler *pHandler, const Config &config = Config::GetDefault());

private:

	static const Size   MAX_LINE_LEN = 32768;
	static const Size   MAX_BOM_SIZE = 4;

	class FileHandlerFile : public IHandler::IFile
	{
	public:

		const Config      *m_pConfig;
		IHandler          *m_pHandler;
		IHandler::Stats   *m_pStats;

		const WChar       *m_wszPath;
		Size              m_cPathLen;
		const void        *m_pContent;
		UInt64            m_cbContentSize;
		UInt32            m_uAttributes;
		HANDLE            m_hFile;
		HANDLE            m_hFileMapping;
		Size              m_cEnumPathIndex;

		FileHandlerFile();

		~FileHandlerFile();

		virtual const WChar *GetPath() const;

		virtual Size GetPathLen() const;

		virtual const void *GetContent() const;

		virtual UInt64 GetContentSize() const;

		virtual UInt32 GetAttributes() const;

		virtual HANDLE GetHandle();

		virtual HANDLE GetMappingHandle();

		virtual Size GetEnumPathIndex() const;

		virtual Status Open();

		virtual void Close();

	};

	FileEnumerator();

	~FileEnumerator();

	static Bool FindPatterns(const void *pFileData, UInt64 cbFileSize, Bool bNegate, 
	                         const Config::PatternsVector &vectorPatterns);

	static void Enumerate(const WChar *wszPath, Size cEnumPathIndex, IHandler *pHandler, void *pTP, 
	                      const Config &config, IHandler::Stats *pStats);

	static void OnFile(const WChar *wszPath, Size cPathLen, Size cEnumPathIndex, const void *pFindData, 
	                   IHandler *pHandler, void *pTP, const Config &config, IHandler::Stats *pStats);

	static VOID CALLBACK WorkCallback(PTP_CALLBACK_INSTANCE pInstance, PVOID pContext, PTP_WORK pWork);

};

}//namespace IO

}//namespace CX


#endif
