/* 
 * CX - C++ framework for general purpose developement
 *
 * https://github.com/draede/cx
 * 
 * Copyright (C) 2014 draede - draede [at] outlook [dot] com
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


#ifdef _DEBUG
	#define CX_DEBUG
#endif

#if defined(_WIN32)
	#define CX_TLS          __declspec(thread)
	#define CX_DLL_EXPORT   __declspec(dllexport)
	#define CX_DLL_IMPORT   __declspec(dllimport)
	#define CX_INLINE       __inline
#ifdef _UNICODE
	#define CX_UNICODE
#endif
#if _MSC_VER == 1400
	#define CX_BUILDSYS_VS2005
	#define CX_BUILDSYS     "VS2005"
#elif _MSC_VER == 1500
	#define CX_BUILDSYS_VS2008
	#define CX_BUILDSYS     "VS2008"
#elif _MSC_VER == 1600
	#define CX_BUILDSYS_VS2010
	#define CX_BUILDSYS     "VS2010"
#elif _MSC_VER == 1700
	#define CX_BUILDSYS_VS2012
	#define CX_BUILDSYS     "VS2012"
#elif _MSC_VER == 1800
	#define CX_BUILDSYS_VS2013
	#define CX_BUILDSYS     "VS2013"
#endif
#ifdef _WIN64
	#define CX_64BIT_ARCH
#else
	#define CX_32BIT_ARCH
#endif
#if (defined(_MSC_VER) && (_MSC_VER >= 1700) && !_USING_V110_SDK71_)
#include <winapifamily.h>
#if WINAPI_FAMILY == WINAPI_FAMILY_PC_APP
	//Windows Modern (=Metro)
	#define CX_OS_WINSTORE
#elif WINAPI_FAMILY == WINAPI_FAMILY_PHONE_APP
	//Windows Phone
	#define CX_OS_WINPHONE
#else
	//Windows
	#define CX_OS_WINDOWS
#endif
#else
	//Windows
	#define CX_OS_WINDOWS
#endif
#endif

#ifdef __ANDROID__
	#define CX_TLS          __thread
	#define CX_DLL_EXPORT
	#define CX_DLL_IMPORT
	//Android
	#define CX_OS_ANDROID
#endif

#if defined(__linux__)
	#define CX_TLS		__thread
	#define CX_DLL_EXPORT
	#define CX_DLL_IMPORT
	//Linux
	#define CX_OS_LINUX
#endif

#if defined(__APPLE__) && defined(__MACH__)
	#define CX_TLS          __thread
	#define CX_DLL_EXPORT
	#define CX_DLL_IMPORT
#include <TargetConditionals.h>
#if TARGET_OS_MAC == 1
	//OSX
	#define CX_OS_OSX
#elif TARGET_IPHONE_SIMULATOR == 1
	//iOS Simulator
	#define CX_OS_IOSSIM
#elif TARGET_OS_IPHONE == 1
	//iOS
	#define CX_OS_IOS
#endif
#endif

