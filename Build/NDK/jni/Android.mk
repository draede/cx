LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := cx

LOCAL_CFLAGS := -DHAVE_EXPAT_CONFIG_H -DWFMO

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
$(LOCAL_PATH)/../../../Contrib/crypto-algorithms/Include \
$(LOCAL_PATH)/../../../Contrib/cURL/Include \
$(LOCAL_PATH)/../../../Contrib/DoubleConversion/Include \
$(LOCAL_PATH)/../../../Contrib/Expat/Include \
$(LOCAL_PATH)/../../../Contrib/Expat/Src \
$(LOCAL_PATH)/../../../Contrib/LZ4/Include \
$(LOCAL_PATH)/../../../Contrib/LZHAM/Include \
$(LOCAL_PATH)/../../../Contrib/MD5/Include \
$(LOCAL_PATH)/../../../Contrib/PEvents/Include \
$(LOCAL_PATH)/../../../Contrib/RapidJSON/Include \
$(LOCAL_PATH)/../../../Contrib/SHA/Include \
$(LOCAL_PATH)/../../../Contrib/Snappy/Include \
$(LOCAL_PATH)/../../../Contrib/SparseHash/Include \
$(LOCAL_PATH)/../../../Contrib/SQLite/Include \
$(LOCAL_PATH)/../../../Contrib/TinyMT/Include \
$(LOCAL_PATH)/../../../Contrib/UTF8ToUCS4/Include \
$(LOCAL_PATH)/../../../Contrib/xxHash/Include \
$(LOCAL_PATH)/../../../Contrib/Z85/Include

LOCAL_SRC_FILES := \
$(LOCAL_PATH)/../../../../Contrib/BLAKE2/Src/blake2b-ref.c \
$(LOCAL_PATH)/../../../../Contrib/CRC32Slice8/Src/crc32_internal.cpp \
$(LOCAL_PATH)/../../../../Contrib/crypto-algorithms/Src/aes.cpp \
$(LOCAL_PATH)/../../../../Contrib/DoubleConversion/Src/bignum.cc \
$(LOCAL_PATH)/../../../../Contrib/DoubleConversion/Src/bignum-dtoa.cc \
$(LOCAL_PATH)/../../../../Contrib/DoubleConversion/Src/cached-powers.cc \
$(LOCAL_PATH)/../../../../Contrib/DoubleConversion/Src/diy-fp.cc \
$(LOCAL_PATH)/../../../../Contrib/DoubleConversion/Src/double-conversion.cc \
$(LOCAL_PATH)/../../../../Contrib/DoubleConversion/Src/fast-dtoa.cc \
$(LOCAL_PATH)/../../../../Contrib/DoubleConversion/Src/fixed-dtoa.cc \
$(LOCAL_PATH)/../../../../Contrib/DoubleConversion/Src/strtod.cc \
$(LOCAL_PATH)/../../../../Contrib/Expat/Src/xmlparse.c \
$(LOCAL_PATH)/../../../../Contrib/Expat/Src/xmlrole.c \
$(LOCAL_PATH)/../../../../Contrib/Expat/Src/xmltok.c \
$(LOCAL_PATH)/../../../../Contrib/LZ4/Src/lz4.c \
$(LOCAL_PATH)/../../../../Contrib/LZ4/Src/lz4frame.c \
$(LOCAL_PATH)/../../../../Contrib/LZ4/Src/lz4hc.c \
$(LOCAL_PATH)/../../../../Contrib/LZHAM/Src/lzham_assert.cpp \
$(LOCAL_PATH)/../../../../Contrib/LZHAM/Src/lzham_checksum.cpp \
$(LOCAL_PATH)/../../../../Contrib/LZHAM/Src/lzham_huffman_codes.cpp \
$(LOCAL_PATH)/../../../../Contrib/LZHAM/Src/lzham_lib.cpp \
$(LOCAL_PATH)/../../../../Contrib/LZHAM/Src/lzham_lzbase.cpp \
$(LOCAL_PATH)/../../../../Contrib/LZHAM/Src/lzham_lzcomp.cpp \
$(LOCAL_PATH)/../../../../Contrib/LZHAM/Src/lzham_lzcomp_internal.cpp \
$(LOCAL_PATH)/../../../../Contrib/LZHAM/Src/lzham_lzcomp_state.cpp \
$(LOCAL_PATH)/../../../../Contrib/LZHAM/Src/lzham_lzdecomp.cpp \
$(LOCAL_PATH)/../../../../Contrib/LZHAM/Src/lzham_lzdecompbase.cpp \
$(LOCAL_PATH)/../../../../Contrib/LZHAM/Src/lzham_match_accel.cpp \
$(LOCAL_PATH)/../../../../Contrib/LZHAM/Src/lzham_mem.cpp \
$(LOCAL_PATH)/../../../../Contrib/LZHAM/Src/lzham_platform.cpp \
$(LOCAL_PATH)/../../../../Contrib/LZHAM/Src/lzham_prefix_coding.cpp \
$(LOCAL_PATH)/../../../../Contrib/LZHAM/Src/lzham_symbol_codec.cpp \
$(LOCAL_PATH)/../../../../Contrib/LZHAM/Src/lzham_timer.cpp \
$(LOCAL_PATH)/../../../../Contrib/LZHAM/Src/lzham_vector.cpp \
$(LOCAL_PATH)/../../../../Contrib/MD5/Src/md5_orig.c \
$(LOCAL_PATH)/../../../../Contrib/SHA/Src/hmac_sha1.c \
$(LOCAL_PATH)/../../../../Contrib/SHA/Src/sha1.c \
$(LOCAL_PATH)/../../../../Contrib/SHA/Src/sha2.c \
$(LOCAL_PATH)/../../../../Contrib/Snappy/Src/snappy.cc \
$(LOCAL_PATH)/../../../../Contrib/Snappy/Src/snappy-sinksource.cc \
$(LOCAL_PATH)/../../../../Contrib/Snappy/Src/snappy-stubs-internal.cc \
$(LOCAL_PATH)/../../../../Contrib/SQLite/Src/sqlite3.c \
$(LOCAL_PATH)/../../../../Contrib/TinyMT/Src/tinymt32.c \
$(LOCAL_PATH)/../../../../Contrib/TinyMT/Src/tinymt64.c \
$(LOCAL_PATH)/../../../../Contrib/UTF8ToUCS4/Src/utf8.c \
$(LOCAL_PATH)/../../../../Contrib/xxHash/Src/xxHash.c \
$(LOCAL_PATH)/../../../../Contrib/Z85/Src/z85.c \
$(LOCAL_PATH)/../../../../Src/Archive/LZ4InputFilter.cpp \
$(LOCAL_PATH)/../../../../Src/Archive/LZ4OutputFilter.cpp \
$(LOCAL_PATH)/../../../../Src/Archive/LZHAMInputFilter.cpp \
$(LOCAL_PATH)/../../../../Src/Archive/LZHAMOutputFilter.cpp \
$(LOCAL_PATH)/../../../../Src/Archive/SnappyInputFilter.cpp \
$(LOCAL_PATH)/../../../../Src/Archive/SnappyOutputFilter.cpp \
$(LOCAL_PATH)/../../../../Src/C/Platform/Android/Mem_android.c \
$(LOCAL_PATH)/../../../../Src/Crypt/AES128InputFilter.cpp \
$(LOCAL_PATH)/../../../../Src/Crypt/AES128OutputFilter.cpp \
$(LOCAL_PATH)/../../../../Src/Data/JSON/JSONSAXParser.cpp \
$(LOCAL_PATH)/../../../../Src/Data/JSON/JSONNode.cpp \
$(LOCAL_PATH)/../../../../Src/Data/MO/Language.cpp \
$(LOCAL_PATH)/../../../../Src/Data/MO/MOSAXParser.cpp \
$(LOCAL_PATH)/../../../../Src/Data/XML/XMLNode.cpp \
$(LOCAL_PATH)/../../../../Src/Data/XML/XMLSAXParser.cpp \
$(LOCAL_PATH)/../../../../Src/DB/SQLite/Bindings.cpp \
$(LOCAL_PATH)/../../../../Src/DB/SQLite/Database.cpp \
$(LOCAL_PATH)/../../../../Src/DB/SQLite/DBHelper.cpp \
$(LOCAL_PATH)/../../../../Src/DB/SQLite/Statement.cpp \
$(LOCAL_PATH)/../../../../Src/DB/SQLite/Transaction.cpp \
$(LOCAL_PATH)/../../../../Src/Debug/Profiler.cpp \
$(LOCAL_PATH)/../../../../Src/Debug/ScopeProfiler.cpp \
$(LOCAL_PATH)/../../../../Src/Debug/SysLogProfilingHandler.cpp \
$(LOCAL_PATH)/../../../../Src/Debug/ThreadProfiler.cpp \
$(LOCAL_PATH)/../../../../Src/Detail/DetailPrint/Platform/Android/DetailPrint_android.cpp \
$(LOCAL_PATH)/../../../../Src/Hash/BLAKE2Hash.cpp \
$(LOCAL_PATH)/../../../../Src/Hash/CRC32Hash.cpp \
$(LOCAL_PATH)/../../../../Src/Hash/MD5Hash.cpp \
$(LOCAL_PATH)/../../../../Src/Hash/MultiHash.cpp \
$(LOCAL_PATH)/../../../../Src/Hash/SHA1Hash.cpp \
$(LOCAL_PATH)/../../../../Src/Hash/SHA256Hash.cpp \
$(LOCAL_PATH)/../../../../Src/Hash/SHA384Hash.cpp \
$(LOCAL_PATH)/../../../../Src/Hash/SHA512Hash.cpp \
$(LOCAL_PATH)/../../../../Src/Hash/xxHash32.cpp \
$(LOCAL_PATH)/../../../../Src/Hash/xxHash64.cpp \
$(LOCAL_PATH)/../../../../Src/IO/BufferedInputStream.cpp \
$(LOCAL_PATH)/../../../../Src/IO/BufferedOutputStream.cpp \
$(LOCAL_PATH)/../../../../Src/IO/FilteredInputStream.cpp \
$(LOCAL_PATH)/../../../../Src/IO/FilteredOutputStream.cpp \
$(LOCAL_PATH)/../../../../Src/IO/Helper.cpp \
$(LOCAL_PATH)/../../../../Src/IO/MemInputStream.cpp \
$(LOCAL_PATH)/../../../../Src/IO/MemOutputStream.cpp \
$(LOCAL_PATH)/../../../../Src/IO/Platform/Android/LocalFileSysHelper_android.cpp \
$(LOCAL_PATH)/../../../../Src/IO/Platform/Android/FileInputStream_android.cpp \
$(LOCAL_PATH)/../../../../Src/IO/Platform/Android/FileOutputStream_android.cpp \
$(LOCAL_PATH)/../../../../Src/Log/ConsoleOutput.cpp \
$(LOCAL_PATH)/../../../../Src/Log/DefaultFormatter.cpp \
$(LOCAL_PATH)/../../../../Src/Log/Logger.cpp \
$(LOCAL_PATH)/../../../../Src/Log/StreamOutput.cpp \
$(LOCAL_PATH)/../../../../Src/Log/OptimizedStreamOutput.cpp \
$(LOCAL_PATH)/../../../../Src/Log/Platform/Android/SystemOutput_android.cpp \
$(LOCAL_PATH)/../../../../Src/Network/URLParser.cpp \
$(LOCAL_PATH)/../../../../Src/Network/HTTP/Client.cpp \
$(LOCAL_PATH)/../../../../Src/SimpleBuffers/JSONReader.cpp \
$(LOCAL_PATH)/../../../../Src/SimpleBuffers/JSONWriter.cpp \
$(LOCAL_PATH)/../../../../Src/SimpleBuffers/BINReader.cpp \
$(LOCAL_PATH)/../../../../Src/SimpleBuffers/BINWriter.cpp \
$(LOCAL_PATH)/../../../../Src/SimpleBuffers/Member.cpp \
$(LOCAL_PATH)/../../../../Src/SimpleBuffers/Object.cpp \
$(LOCAL_PATH)/../../../../Src/SimpleBuffers/Parser.cpp \
$(LOCAL_PATH)/../../../../Src/Str/Base64BinStr.cpp \
$(LOCAL_PATH)/../../../../Src/Str/HexBinStr.cpp \
$(LOCAL_PATH)/../../../../Src/Str/StringHelper.cpp \
$(LOCAL_PATH)/../../../../Src/Str/WildMat.cpp \
$(LOCAL_PATH)/../../../../Src/Str/Z85BinStr.cpp \
$(LOCAL_PATH)/../../../../Src/Str/Platform/Android/UTF8_android.cpp \
$(LOCAL_PATH)/../../../../Src/Sys/Platform/Android/Atomic_android.cpp \
$(LOCAL_PATH)/../../../../Src/Sys/Platform/Android/DynLib_android.cpp \
$(LOCAL_PATH)/../../../../Src/Sys/Platform/Android/Event_android.cpp \
$(LOCAL_PATH)/../../../../Src/Sys/Platform/Android/Lock_android.cpp \
$(LOCAL_PATH)/../../../../Src/Sys/Platform/Android/Process_android.cpp \
$(LOCAL_PATH)/../../../../Src/Sys/Platform/Android/RWLock_android.cpp \
$(LOCAL_PATH)/../../../../Src/Sys/Platform/Android/Thread_android.cpp \
$(LOCAL_PATH)/../../../../Src/Util/Bitmap.cpp \
$(LOCAL_PATH)/../../../../Src/Util/CircularBuffer.cpp \
$(LOCAL_PATH)/../../../../Src/Util/DynMember.cpp \
$(LOCAL_PATH)/../../../../Src/Util/DynStruct.cpp \
$(LOCAL_PATH)/../../../../Src/Util/MemPool.cpp \
$(LOCAL_PATH)/../../../../Src/Util/RCMemBuffer.cpp \
$(LOCAL_PATH)/../../../../Src/Util/RCMemBufferMgr.cpp \
$(LOCAL_PATH)/../../../../Src/Util/RndGen.cpp \
$(LOCAL_PATH)/../../../../Src/Util/Platform/Android/Time_android.cpp \
$(LOCAL_PATH)/../../../../Src/Util/Platform/Android/Timer_android.cpp \
$(LOCAL_PATH)/../../../../Src/Mem.cpp \
$(LOCAL_PATH)/../../../../Src/Status.cpp \
$(LOCAL_PATH)/../../../../Src/Var.cpp \
$(LOCAL_PATH)/../../../../Src/CXVersion.cpp \
$(LOCAL_PATH)/../../../../Src/Mem.cpp \
$(LOCAL_PATH)/../../../../Src/Status.cpp \
$(LOCAL_PATH)/../../../../Src/Value.cpp \
$(LOCAL_PATH)/../../../../Src/Var.cpp 

include $(BUILD_STATIC_LIBRARY)
