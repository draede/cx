APP_ABI := armeabi armeabi-v7a x86
APP_PLATFORM := android-9
APP_MODULES := cx
APP_CPPFLAGS := -fpermissive -fexceptions -std=c++11
APP_STL := gnustl_static
CR_LOCAL_PATH := $(call my-dir)
NDK_APP_OUT := $(CR_LOCAL_PATH)/../../../Temp/NDK/

ifeq ($(NDK_DEBUG),1)
	APP_OPTIM := debug
else
	APP_OPTIM := release
endif

