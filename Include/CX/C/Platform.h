
#pragma once


#ifdef _DEBUG
	#define CX_DEBUG
#endif

#if defined(_WIN32)
	#define CX_TLS          __declspec(thread)
	#define CX_DLL_EXPORT   __declspec(dllexport)
	#define CX_DLL_IMPORT   __declspec(dllimport)
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

