LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := cx

LOCAL_CFLAGS := -Wno-psabi

ifeq ($(APP_OPTIM), debug)
	LOCAL_CFLAGS += -D_DEBUG -g
else
	LOCAL_CFLAGS += -DNDEBUG
endif


LOCAL_C_INCLUDES := \
$(LOCAL_PATH)/../../../Include \
$(LOCAL_PATH)/../../../Contrib/BLAKE2/Include \
$(LOCAL_PATH)/../../../Contrib/ByteOrder/Include \
$(LOCAL_PATH)/../../../Contrib/Catch/Include \
$(LOCAL_PATH)/../../../Contrib/CRC32Slice8/Include \
$(LOCAL_PATH)/../../../Contrib/DoubleConversion/Include \
$(LOCAL_PATH)/../../../Contrib/MD5/Include \
$(LOCAL_PATH)/../../../Contrib/RapidJSON/Include \
$(LOCAL_PATH)/../../../Contrib/SHA/Include \
$(LOCAL_PATH)/../../../Contrib/SparseHash/Include \
$(LOCAL_PATH)/../../../Contrib/TinyMT/Include \
$(LOCAL_PATH)/../../../Contrib/UTF8ToUCS4/Include \
$(LOCAL_PATH)/../../../Contrib/xxHash/Include \
$(LOCAL_PATH)/../../../Contrib/Z85/Include \
$(NDK_ROOT)/platforms/android-16/arch-arm/usr/include

LOCAL_SRC_FILES := \
$(LOCAL_PATH)/../../../../Contrib/BLAKE2/Src/blake2b-ref.c \
$(LOCAL_PATH)/../../../../Contrib/CRC32Slice8/Src/crc32_internal.cpp \
$(LOCAL_PATH)/../../../../Contrib/DoubleConversion/Src/bignum.cc \
$(LOCAL_PATH)/../../../../Contrib/DoubleConversion/Src/bignum-dtoa.cc \
$(LOCAL_PATH)/../../../../Contrib/DoubleConversion/Src/cached-powers.cc \
$(LOCAL_PATH)/../../../../Contrib/DoubleConversion/Src/diy-fp.cc \
$(LOCAL_PATH)/../../../../Contrib/DoubleConversion/Src/double-conversion.cc \
$(LOCAL_PATH)/../../../../Contrib/DoubleConversion/Src/fast-dtoa.cc \
$(LOCAL_PATH)/../../../../Contrib/DoubleConversion/Src/fixed-dtoa.cc \
$(LOCAL_PATH)/../../../../Contrib/DoubleConversion/Src/strtod.cc \
$(LOCAL_PATH)/../../../../Contrib/MD5/Src/md5.c \
$(LOCAL_PATH)/../../../../Contrib/SHA/Src/hmac_sha1.c \
$(LOCAL_PATH)/../../../../Contrib/SHA/Src/sha1.c \
$(LOCAL_PATH)/../../../../Contrib/SHA/Src/sha2.c \
$(LOCAL_PATH)/../../../../Contrib/TinyMT/Src/tinymt32.c \
$(LOCAL_PATH)/../../../../Contrib/TinyMT/Src/tinymt64.c \
$(LOCAL_PATH)/../../../../Contrib/UTF8ToUCS4/Src/utf8.c \
$(LOCAL_PATH)/../../../../Contrib/xxHash/Src/xxHash.c \
$(LOCAL_PATH)/../../../../Contrib/Z85/Src/z85.c \
$(LOCAL_PATH)/../../../../Src/Archive/JSONReader.cpp \
$(LOCAL_PATH)/../../../../Src/Archive/JSONWriter.cpp \
$(LOCAL_PATH)/../../../../Src/Archive/Member.cpp \
$(LOCAL_PATH)/../../../../Src/Archive/Object.cpp \
$(LOCAL_PATH)/../../../../Src/Archive/Parser.cpp \
$(LOCAL_PATH)/../../../../Src/C/Platform/Android/Alloc_android.c \
$(LOCAL_PATH)/../../../../Src/Data/JSON/JSONSAXParser.cpp \
$(LOCAL_PATH)/../../../../Src/Detail/DetailPrint/Platform/Android/DetailPrint_android.cpp \
$(LOCAL_PATH)/../../../../Src/Hash/BLAKE2Hash.cpp \
$(LOCAL_PATH)/../../../../Src/Hash/CRC32Hash.cpp \
$(LOCAL_PATH)/../../../../Src/Hash/MD5Hash.cpp \
$(LOCAL_PATH)/../../../../Src/Hash/SHA1Hash.cpp \
$(LOCAL_PATH)/../../../../Src/Hash/SHA256Hash.cpp \
$(LOCAL_PATH)/../../../../Src/Hash/SHA384Hash.cpp \
$(LOCAL_PATH)/../../../../Src/Hash/SHA512Hash.cpp \
$(LOCAL_PATH)/../../../../Src/Hash/xxHash32.cpp \
$(LOCAL_PATH)/../../../../Src/Hash/xxHash64.cpp \
$(LOCAL_PATH)/../../../../Src/IO/Platform/Android/LocalFileSysHelper_android.cpp \
$(LOCAL_PATH)/../../../../Src/IO/Platform/Android/FileInputStream_android.cpp \
$(LOCAL_PATH)/../../../../Src/IO/Platform/Android/FileOutputStream_android.cpp \
$(LOCAL_PATH)/../../../../Src/IO/Helper.cpp \
$(LOCAL_PATH)/../../../../Src/IO/MemInputStream.cpp \
$(LOCAL_PATH)/../../../../Src/IO/MemOutputStream.cpp \
$(LOCAL_PATH)/../../../../Src/Log/Platform/Android/SystemOutput_android.cpp \
$(LOCAL_PATH)/../../../../Src/Log/ConsoleOutput.cpp \
$(LOCAL_PATH)/../../../../Src/Log/DefaultFormatter.cpp \
$(LOCAL_PATH)/../../../../Src/Log/Logger.cpp \
$(LOCAL_PATH)/../../../../Src/Log/StreamOutput.cpp \
$(LOCAL_PATH)/../../../../Src/Sys/Platform/Android/DynLib_android.cpp \
$(LOCAL_PATH)/../../../../Src/Sys/Platform/Android/Lock_android.cpp \
$(LOCAL_PATH)/../../../../Src/Sys/Platform/Android/Process_android.cpp \
$(LOCAL_PATH)/../../../../Src/Sys/Platform/Android/RWLock_android.cpp \
$(LOCAL_PATH)/../../../../Src/Sys/Platform/Android/Thread_android.cpp \
$(LOCAL_PATH)/../../../../Src/Str/Platform/Android/UTF8_android.cpp \
$(LOCAL_PATH)/../../../../Src/Str/HexBinStr.cpp \
$(LOCAL_PATH)/../../../../Src/Str/Z85BinStr.cpp \
$(LOCAL_PATH)/../../../../Src/Util/Platform/Android/StackTrace_android.cpp \
$(LOCAL_PATH)/../../../../Src/Util/Platform/Android/Time_android.cpp \
$(LOCAL_PATH)/../../../../Src/Util/Platform/Android/Timer_android.cpp \
$(LOCAL_PATH)/../../../../Src/Util/MemPool.cpp \
$(LOCAL_PATH)/../../../../Src/Util/RCMemBuffer.cpp \
$(LOCAL_PATH)/../../../../Src/Util/RCMemBufferMgr.cpp \
$(LOCAL_PATH)/../../../../Src/Util/RndGen.cpp \
$(LOCAL_PATH)/../../../../Src/Platform/Android/Component_android.cpp \
$(LOCAL_PATH)/../../../../Src/IObject.cpp \
$(LOCAL_PATH)/../../../../Src/Mem.cpp \
$(LOCAL_PATH)/../../../../Src/Status.cpp \
$(LOCAL_PATH)/../../../../Src/Var.cpp \
$(LOCAL_PATH)/../../../../Src/Version.cpp

include $(BUILD_STATIC_LIBRARY)
