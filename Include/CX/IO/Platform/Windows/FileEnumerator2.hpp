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
#include "CX/String.hpp"
#include "CX/Vector.hpp"
#include "CX/Set.hpp"
#include "CX/BLOB.hpp"
#include "CX/C/Platform/Windows/windows.h"
#include "CX/Sys/Platform/Windows/SyncedWorkerThreads.hpp"
#include "CX/APIDefs.hpp"


namespace CX
{

namespace IO
{

//this preservers the order of the files
//added support for canceling
//other improvements
class CX_API FileEnumerator2
{
public:

	struct Config
	{

		static const Size     MIN_QUEUED_FILES          = 1;
		static const Size     MAX_QUEUED_FILES          = 1048576;

		static const Size     MIN_THREADS               = 1;
		static const Size     MAX_THREADS               = 1024;

		static const Size     MIN_PATTERN_SIZE          = 1;
		static const Size     MAX_PATTERN_SIZE          = 1024;

		static const UInt64   MIN_FILE_SIZE             = 0;
		static const UInt64   MAX_FILE_SIZE             = 4294967295;

		static const Size     DEFAULT_THREADS           = 1;
		static const Size     DEFAULT_QUEUED_FILES      = 1024;
		static const UInt64   DEFAULT_MIN_FILE_SIZE     = 0;
		static const UInt64   DEFAULT_MAX_FILE_SIZE     = 268435456; //256 MB;
		static const Bool     DEFAULT_RECURSIVE         = True;
		static const Bool     DEFAULT_MAP_FILE          = False; //will still be mapped anyway if patterns are set

		static const WChar    *ARG_THREADS;             //--threads <number_of_threads : size>
		static const WChar    *ARG_QUEUED_FILES;        //--queuedfiles <number_of_queued_files : size>
		static const WChar    *ARG_MIN_FILE_SIZE;       //--minfilesize <min_file_size : uint64>
		static const WChar    *ARG_MAX_FILE_SIZE;       //--maxfilesize <max_file_size : uint64>
		static const WChar    *ARG_RECURSIVE;           //--recursive <yes|no>
		static const WChar    *ARG_EXTENSION;           //--extension <extension:string (e.g. : exe)>
		static const WChar    *ARG_PATTERN;             //--pattern <offset:uint64> [!]<pattern:string_or_hexstring e.g. 
		                                                //  0xAABBCC>

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

		Size             cThreads;
		Size             cQueuedFiles;
		UInt64           cbMinFileSize;
		UInt64           cbMaxFileSize;
		Bool             bRecursive;
		Bool             bMapFile;
		PatternsVector   vectorPatterns;
		ExtensionsSet    setExtensions;

		static const Config &GetDefault();

		Status FromCmdLine(ArgsVector& vectorArgs, Size cStartArg = 0);

	};

	struct Stats
	{
		volatile Int64     cDiscoveredDirs;

		volatile Int64     cDiscoveredFiles;
		volatile Int64     cbDiscoveredFilesSize;

		Int64              nPerfCounterFreq;
		Int64              nPerfCounterStart;
	};

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

		virtual Size GetPathIndex() const = 0;

		virtual Status Open() = 0;

		virtual void Close() = 0;

	};

	class IHandler
	{
	public:

		~IHandler() { }

		virtual Bool OnBegin() = 0;

		//called from multiple threads
		virtual Bool OnFile(IFile *pFile, void **ppResult, const Stats *pStats) = 0;

		//called from a single thread
		virtual Bool OnBeginResults(void **ppContext) =0;

		//called from a single thread
		virtual Bool OnResult(void *pResult, void *pContext) = 0;

		//called from a single thread
		virtual Bool OnEndResults(void *pContext) = 0;

		virtual Bool OnEnd(const Stats *pStats) = 0;

		virtual Bool OnError(const Status &status) = 0;

		//new 
		virtual Bool OnFolder(const WChar *wszPath) { wszPath; return True; }

		//new 
		virtual Bool OnBeginPath(const WChar *wszPath, Size cIndex) { wszPath; cIndex; return True; }

		//new 
		virtual Bool OnEndPath(const WChar *wszPath, Size cIndex) { wszPath; cIndex; return True; }

		//new 
		virtual Bool OnListLineError(const WChar *wszLine, Size cPathIndex, const Status &status)
		{
			wszLine;
			cPathIndex;
			status;
			
			return True;
		}

	};

	typedef Vector<WString>::Type   PathsVector;

	static const Size MAX_THREADS = 256;

	static Status Run(const WChar *wszPath, IHandler *pHandler, const Config &config = Config::GetDefault());

	static Status Run(const PathsVector &vectorPaths, IHandler *pHandler, const Config &config = Config::GetDefault());

private:

	static const Size   MAX_LINE_LEN = 32768;
	static const Size   MAX_BOM_SIZE = 4;

	class FileImpl : public IFile
	{
	public:

		const WChar       *m_wszPath;
		Size              m_cPathLen;
		const void        *m_pContent;
		UInt64            m_cbContentSize;
		UInt32            m_uAttributes;
		HANDLE            m_hFile;
		HANDLE            m_hFileMapping;
		Size              m_cPathIndex;

		FileImpl();

		~FileImpl();

		virtual const WChar *GetPath() const;

		virtual Size GetPathLen() const;

		virtual const void *GetContent() const;

		virtual UInt64 GetContentSize() const;

		virtual UInt32 GetAttributes() const;

		virtual HANDLE GetHandle();

		virtual HANDLE GetMappingHandle();

		virtual Size GetPathIndex() const;

		virtual Status Open();

		virtual void Close();

	};

	struct File
	{
		void                        *pCTX;
		UInt64                      cbSize;
		WString                     wsPath;
		Size                        cPathIndex;
		FileImpl                    file;
		void                        *pResult;
	};

	struct Context
	{
		Bool                        bRunning;
		Sys::SyncedWorkerThreads    threads;
		IHandler                    *pHandler;
		Stats                       stats;
		Config                      config;
		Size                        cPathIndex;
		File                        *files;
		Size                        cFiles;
	};

	FileEnumerator2();

	~FileEnumerator2();

	static Status RunWithPath(const WChar* wszPath, Context &ctx);

	static Status RunWithList(const WChar* wszListPath, Context &ctx);

	static Status RunWithListUTF8(FILE *pFile, Context &ctxg);

	static Status RunWithListUTF16(FILE *pFile, Context &ctx);

	static Status Enumerate(const WChar *wszPath, Context &ctx);

	static Status OnFile(const WChar *wszPath, Size cPathLen, UInt64 cbSize, Context &ctx);

	static Status ProcessFiles(Context &ctx);

	static Bool HandleFileJob(void *pJob, Size cbSize);

	static Bool FindPatterns(const void *pFileData, UInt64 cbFileSize, Bool bNegate, 
	                         const Config::PatternsVector &vectorPatterns);

};

}//namespace IO

}//namespace CX

#endif
