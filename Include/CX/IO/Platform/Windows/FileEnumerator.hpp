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
#include "CX/Util/Timer.hpp"
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

		static const Size     DEFAULT_THREADS          = 1;
		static const UInt64   DEFAULT_MIN_FILE_SIZE    = 0;
		static const UInt64   DEFAULT_MAX_FILE_SIZE    = 268435456; //256 MB;
		static const Bool     DEFAULT_RECURSIVE        = True;
		static const Bool     DEFAULT_MAP_FILE         = False; //will still be mapped anyway if patterns are set

		static const WChar    *ARG_THREADS;        //--threads <number_of_threads : size>
		static const WChar    *ARG_MIN_FILE_SIZE;  //--minfilesize <min_file_size : uint64>
		static const WChar    *ARG_MAX_FILE_SIZE;  //--maxfilesize <max_file_size : uint64>
		static const WChar    *ARG_RECURSIVE;      //--recursive <yes|no>
		static const WChar    *ARG_EXTENSION;      //--extension <extension:string (e.g. : exe)>
		static const WChar    *ARG_PATTERN;        //--pattern <offset:uint64> <pattern:string_or_hexstring e.g. 0xAABBCC>

		typedef Set<WString, WCaseInsensitiveOrderPolicy>::Type   ExtensionsSet;

		struct Pattern
		{
			UInt64   cbOffset;
			BLOB     pattern;

			Pattern();

			Pattern(UInt64 cbOffset, const void *pPattern, Size cbSize);

			Bool SetupPattern(const void *pPattern, Size cbSize);
		};

		typedef Vector<Pattern>::Type                             PatternsVector;

		typedef Vector<WString>::Type                             ArgsVector;

		Size             cThreads;
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

			virtual void Close() = 0;

		};

		struct Stats
		{
			Size     cAllDirs;

			Size     cAllFiles;
			UInt64   cbAllFilesSize;

			Size     cExcludedFiles;
			UInt64   cbExcludedFilesSize;

			Size     cExcludedTooSmallFiles;
			UInt64   cbExcludedTooSmallFilesSize;

			Size     cExcludedTooBigFiles;
			UInt64   cbExcludedTooBigFilesSize;

			Size     cExcludedExtensionFiles;
			UInt64   cbExcludedExtensionFilesSize;

			Size     cExcludedPatternFiles;
			UInt64   cbExcludedPatternFilesSize;

			Size     cIncludedFiles;
			UInt64   cbIncludedFilesSize;

			Size     cProcessedFiles;
			UInt64   cbProcessedFilesSize;

			Size     cErrorMemAllocFiles;
			UInt64   cbErrorMemAllocFilesSize;

			Size     cErrorProcessingFiles;
			UInt64   cbErrorProcessingFilesSize;

			Double   lfElapsedTime;
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

	class FileHandlerFile : public IHandler::IFile
	{
	public:

		IHandler          *pHandler;
		IHandler::Stats   *pStats;

		const WChar       *wszPath;
		Size              cPathLen;
		const void        *pContent;
		UInt64            cbContentSize;
		UInt32            uAttributes;
		HANDLE            hFile;
		HANDLE            hFileMapping;

		FileHandlerFile();

		~FileHandlerFile();

		virtual const WChar *GetPath() const;

		virtual Size GetPathLen() const;

		virtual const void *GetContent() const;

		virtual UInt64 GetContentSize() const;

		virtual UInt32 GetAttributes() const;

		virtual HANDLE GetHandle();

		virtual HANDLE GetMappingHandle();

		virtual void Close();

	};

	FileEnumerator();

	~FileEnumerator();

	static void Enumerate(const WChar *wszPath, IHandler *pHandler, void *pTP, const Config &config, 
	                      IHandler::Stats *pStats, Util::Timer *pTimer);

	static void OnFile(const WChar *wszPath, Size cPathLen, const void *pFindData, IHandler *pHandler, 
                      void *pTP, const Config &config, IHandler::Stats *pStats);

	static VOID CALLBACK WorkCallback(PTP_CALLBACK_INSTANCE pInstance, PVOID pContext, PTP_WORK pWork);

};

}//namespace IO

}//namespace CX


#endif
