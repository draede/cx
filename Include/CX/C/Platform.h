/* 
 * CX - C++ framework for general purpose development
 *
 * https://github.com/draede/cx
 * 
 * Copyright (C) 2014 - 2021 draede - draede [at] outlook [dot] com
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


#define CX_UNUSED(x)   (void)x

//debug | release
#if defined(_DEBUG) || !defined(NDEBUG)
	#define CX_DEBUG
#else
	#define CX_RELEASE
#endif

//OS
#if defined(_WIN32)
	#define CX_OS           "WINDOWS"
	#define CX_OS_WINDOWS
	#define CX_TLS          __declspec(thread)
	#define CX_DLL_EXPORT   __declspec(dllexport)
	#define CX_DLL_IMPORT   __declspec(dllimport)
	#define CX_INLINE       __inline
	#ifdef _WIN64
		#define CX_64BIT_ARCH
	#else
		#define CX_32BIT_ARCH
	#endif
#elif defined(__ANDROID__)
	#define CX_OS           "ANDROID"
	#define CX_OS_ANDROID
	#define CX_TLS          __thread
	#define CX_DLL_EXPORT
	#define CX_DLL_IMPORT
	#define CX_INLINE       inline
	#ifdef __LP64__
		#define CX_64BIT_ARCH
	#else
		#define CX_32BIT_ARCH
	#endif
#elif defined(__linux__)
	#define CX_OS           "LINUX"
	#define CX_OS_LINUX
	#define CX_TLS          __thread
	#define CX_DLL_EXPORT
	#define CX_DLL_IMPORT
	#define CX_INLINE       inline
	#ifdef __LP64__
		#define CX_64BIT_ARCH
	#else
		#define CX_32BIT_ARCH
	#endif
#elif defined(__APPLE__) && defined(__MACH__)
	#include <TargetConditionals.h>
	#if TARGET_OS_IPHONE == 1
		#define CX_OS           "IOS"
		#define CX_OS_IOS
		#define CX_TLS          __thread
		#define CX_DLL_EXPORT
		#define CX_DLL_IMPORT
		#define CX_INLINE       inline
	#elif TARGET_IPHONE_SIMULATOR == 1
		#define CX_OS           "IOSSIM"
		#define CX_OS_IOSSIM
		#define CX_TLS          __thread
		#define CX_DLL_EXPORT
		#define CX_DLL_IMPORT
		#define CX_INLINE       inline
	#elif TARGET_OS_MAC == 1
		#define CX_OS           "MAC"
		#define CX_OS_MAC
		#define CX_TLS          __thread
		#define CX_DLL_EXPORT
		#define CX_DLL_IMPORT
		#define CX_INLINE       inline
	#endif
	#ifdef __LP64__
		#define CX_64BIT_ARCH
	#else
		#define CX_32BIT_ARCH
	#endif
#else
	#error "Unknown OS" 
#endif

//compiler
#ifdef _MSC_VER
	#if _MSC_VER == 1400
		#define CX_BUILDSYS_VS
		#define CX_BUILDSYS_VS2005
		#define CX_BUILDSYS     "VS2005"
	#elif _MSC_VER == 1500
		#define CX_BUILDSYS_VS
		#define CX_BUILDSYS_VS2008
		#define CX_BUILDSYS     "VS2008"
	#elif _MSC_VER == 1600
		#define CX_BUILDSYS_VS
		#define CX_BUILDSYS_VS2010
		#define CX_BUILDSYS     "VS2010"
	#elif _MSC_VER == 1700
		#define CX_BUILDSYS_VS
		#define CX_BUILDSYS_VS2012
		#define CX_BUILDSYS     "VS2012"
	#elif _MSC_VER == 1800
		#define CX_BUILDSYS_VS
		#define CX_BUILDSYS_VS2013
		#define CX_BUILDSYS     "VS2013"
	#elif _MSC_VER == 1900
		#define CX_BUILDSYS_VS
		#define CX_BUILDSYS_VS2015
		#define CX_BUILDSYS     "VS2015"
	#elif _MSC_VER == 1910
		#define CX_BUILDSYS_VS
		#define CX_BUILDSYS_VS2017
		#define CX_BUILDSYS     "VS2017"
	#elif _MSC_VER >= 1911
		#define CX_BUILDSYS_VS
		#define CX_BUILDSYS_VS2017
		#define CX_BUILDSYS     "VS2017"
	#else
		#error "Unknown Visual Studio Version" 
	#endif
#elif defined(__GNUC__) || defined(__GNUG__)
	#define CX_BUILDSYS_GCC
	#define CX_BUILDSYS        "GCC"
	#ifdef __ANDROID__
		#define CX_BUILDSYS_NDK
	#endif
#elif defined(__clang__)
	#define CX_BUILDSYS_CLANG
	#define CX_BUILDSYS        "CLANG"
#else
	#error "Unknown compiler" 
#endif

