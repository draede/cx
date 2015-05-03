
NAME     = cx
LIBNAME  = libcx.a

TARGET_X86_DBG_OUTPATH  = x86/Debug
TARGET_X86_OPT_OUTPATH  = x86/Release
TARGET_X64_DBG_OUTPATH  = x64/Debug
TARGET_X64_OPT_OUTPATH  = x64/Release

TARGET_X86_DBG_FLAGS    = -m32 -g -O0 -D_DEBUG
TARGET_X86_OPT_FLAGS    = -m32 -O2 -DNDEBUG
TARGET_X64_DBG_FLAGS    = -m64 -g -O0 -D_DEBUG
TARGET_X64_OPT_FLAGS    = -m64 -O2 -DNDEBUG

TARGET_OUTPATH          = $(TARGET_X64_OPT_OUTPATH)
TARGET_FLAGS            = $(TARGET_X64_OPT_FLAGS)

x86dbg : TARGET_OUTPATH = $(TARGET_X86_DBG_OUTPATH)
x86opt : TARGET_OUTPATH = $(TARGET_X86_OPT_OUTPATH)
x64dbg : TARGET_OUTPATH = $(TARGET_X64_DBG_OUTPATH)
x64opt : TARGET_OUTPATH = $(TARGET_X64_OPT_OUTPATH)

x86dbg : TARGET_FLAGS   = $(TARGET_X86_DBG_FLAGS)
x86opt : TARGET_FLAGS   = $(TARGET_X86_OPT_FLAGS)
x64dbg : TARGET_FLAGS   = $(TARGET_X64_DBG_FLAGS)
x64opt : TARGET_FLAGS   = $(TARGET_X64_OPT_FLAGS)

CNTPATH = ../../Contrib
INCPATH = -I../../Include \
				-I$(CNTPATH)/ByteOrder/Include \
				-I$(CNTPATH)/Catch/Include \
				-I$(CNTPATH)/CRC32Slice8/Include \
				-I$(CNTPATH)/DoubleConversion/Include \
				-I$(CNTPATH)/ForestDB/Include \
				-I$(CNTPATH)/ForestDB/Src/src \
				-I$(CNTPATH)/ForestDB/Src/option \
				-I$(CNTPATH)/ForestDB/Src/utils \
				-I$(CNTPATH)/MD5/Include \
				-I$(CNTPATH)/Mintomic/Include \
				-I$(CNTPATH)/RapidJSON/Include \
				-I$(CNTPATH)/SHA/Include \
				-I$(CNTPATH)/SparseHash/Include \
				-I$(CNTPATH)/TinyMT/Include \
				-I$(CNTPATH)/UTF8ToUCS4/Include \
				-I$(CNTPATH)/xxHash/Include \
				-I$(CNTPATH)/Z85/Include
SRCPATH = ../../Src
TMPPATH = ../../Temp/GCC/Linux/$(TARGET_OUTPATH)
OUTPATH = ../../Lib/GCC/Linux/$(TARGET_OUTPATH)
MYFLAGS = $(TARGET_FLAGS) $(INCPATH)

x86dbg: all
x86opt: all
x64dbg: all
x64opt: all

all: \
	$(CNTPATH)/CRC32Slice8/Src/crc32_internal.cpp \
	$(CNTPATH)/DoubleConversion/Src/bignum.cc \
	$(CNTPATH)/DoubleConversion/Src/bignum-dtoa.cc \
	$(CNTPATH)/DoubleConversion/Src/cached-powers.cc \
	$(CNTPATH)/DoubleConversion/Src/diy-fp.cc \
	$(CNTPATH)/DoubleConversion/Src/double-conversion.cc \
	$(CNTPATH)/DoubleConversion/Src/fast-dtoa.cc \
	$(CNTPATH)/DoubleConversion/Src/fixed-dtoa.cc \
	$(CNTPATH)/DoubleConversion/Src/strtod.cc \
	$(CNTPATH)/ForestDB/Src/utils/adler32.cc \
	$(CNTPATH)/ForestDB/Src/utils/crc32.cc \
	$(CNTPATH)/ForestDB/Src/utils/debug.cc \
	$(CNTPATH)/ForestDB/Src/utils/iniparser.cc \
	$(CNTPATH)/ForestDB/Src/utils/memleak.cc \
	$(CNTPATH)/ForestDB/Src/utils/partiallock.cc \
	$(CNTPATH)/ForestDB/Src/utils/time_utils.cc \
	$(CNTPATH)/ForestDB/Src/src/api_wrapper.cc \
	$(CNTPATH)/ForestDB/Src/src/atomic.cc \
	$(CNTPATH)/ForestDB/Src/src/avltree.cc \
	$(CNTPATH)/ForestDB/Src/src/blockcache.cc \
	$(CNTPATH)/ForestDB/Src/src/btree.cc \
	$(CNTPATH)/ForestDB/Src/src/btree_fast_str_kv.cc \
	$(CNTPATH)/ForestDB/Src/src/btree_kv.cc \
	$(CNTPATH)/ForestDB/Src/src/btree_str_kv.cc \
	$(CNTPATH)/ForestDB/Src/src/btreeblock.cc \
	$(CNTPATH)/ForestDB/Src/src/compactor.cc \
	$(CNTPATH)/ForestDB/Src/src/configuration.cc \
	$(CNTPATH)/ForestDB/Src/src/docio.cc \
	$(CNTPATH)/ForestDB/Src/src/fdb_errors.cc \
	$(CNTPATH)/ForestDB/Src/src/filemgr.cc \
	$(CNTPATH)/ForestDB/Src/src/filemgr_ops.cc \
	$(CNTPATH)/ForestDB/Src/src/filemgr_ops_linux.cc \
	$(CNTPATH)/ForestDB/Src/src/forestdb.cc \
	$(CNTPATH)/ForestDB/Src/src/hash.cc \
	$(CNTPATH)/ForestDB/Src/src/hash_functions.cc \
	$(CNTPATH)/ForestDB/Src/src/hbtrie.cc \
	$(CNTPATH)/ForestDB/Src/src/iterator.cc \
	$(CNTPATH)/ForestDB/Src/src/kv_instance.cc \
	$(CNTPATH)/ForestDB/Src/src/list.cc \
	$(CNTPATH)/ForestDB/Src/src/snapshot.cc \
	$(CNTPATH)/ForestDB/Src/src/transaction.cc \
	$(CNTPATH)/ForestDB/Src/src/wal.cc \
	$(CNTPATH)/MD5/Src/md5.c \
	$(CNTPATH)/Mintomic/Src/mintomic_gcc.c \
	$(CNTPATH)/SHA/Src/hmac_sha1.c \
	$(CNTPATH)/SHA/Src/sha1.c \
	$(CNTPATH)/SHA/Src/sha2.c \
	$(CNTPATH)/TinyMT/Src/tinymt32.c \
	$(CNTPATH)/TinyMT/Src/tinymt64.c \
	$(CNTPATH)/UTF8ToUCS4/Src/utf8.c \
	$(CNTPATH)/xxHash/Src/xxHash.c \
	$(CNTPATH)/Z85/Src/z85.c \
	$(SRCPATH)/C/Platform/Posix/Alloc_posix.c \
	$(SRCPATH)/Data/JSON/JSONSAXParser.cpp \
	$(SRCPATH)/Detail/DetailPrint/Platform/Posix/DetailPrint_posix.cpp \
	$(SRCPATH)/Hash/CRC32Hash.cpp \
	$(SRCPATH)/Hash/MD5Hash.cpp \
	$(SRCPATH)/Hash/SHA1Hash.cpp \
	$(SRCPATH)/Hash/SHA256Hash.cpp \
	$(SRCPATH)/Hash/SHA384Hash.cpp \
	$(SRCPATH)/Hash/SHA512Hash.cpp \
	$(SRCPATH)/Hash/xxHash32.cpp \
	$(SRCPATH)/Hash/xxHash64.cpp \
	$(SRCPATH)/IO/Platform/Posix/LocalFileSysHelper_posix.cpp \
	$(SRCPATH)/IO/Platform/Posix/FileInputStream_posix.cpp \
	$(SRCPATH)/IO/Platform/Posix/FileOutputStream_posix.cpp \
	$(SRCPATH)/IO/Helper.cpp \
	$(SRCPATH)/IO/MemInputStream.cpp \
	$(SRCPATH)/IO/MemOutputStream.cpp \
	$(SRCPATH)/KVDB/FDBDatabase.cpp \
	$(SRCPATH)/KVDB/FDBIterator.cpp \
	$(SRCPATH)/KVDB/FDBRecord.cpp \
	$(SRCPATH)/KVDB/FDBTable.cpp \
	$(SRCPATH)/KVDB/MemDatabase.cpp \
	$(SRCPATH)/KVDB/MemIterator.cpp \
	$(SRCPATH)/KVDB/MemRecord.cpp \
	$(SRCPATH)/KVDB/MemTable.cpp \
	$(SRCPATH)/Log/Platform/Posix/SystemOutput_posix.cpp \
	$(SRCPATH)/Log/ConsoleOutput.cpp \
	$(SRCPATH)/Log/DefaultFormatter.cpp \
	$(SRCPATH)/Log/Logger.cpp \
	$(SRCPATH)/Log/StreamOutput.cpp \
	$(SRCPATH)/SB/BinaryDataReader.cpp \
	$(SRCPATH)/SB/BinaryDataWriter.cpp \
	$(SRCPATH)/SB/Parser.cpp \
	$(SRCPATH)/SB/StatsDataReader.cpp \
	$(SRCPATH)/SB/StatsDataWriter.cpp \
	$(SRCPATH)/SB/TextDataReader.cpp \
	$(SRCPATH)/SB/TextDataWriter.cpp \
	$(SRCPATH)/Str/Platform/Posix/UTF8_posix.cpp \
	$(SRCPATH)/Str/HexBinStr.cpp \
	$(SRCPATH)/Str/Z85BinStr.cpp \
	$(SRCPATH)/Sys/Platform/Posix/DynLib_posix.cpp \
	$(SRCPATH)/Sys/Platform/Posix/Lock_posix.cpp \
	$(SRCPATH)/Sys/Platform/Posix/Process_posix.cpp \
	$(SRCPATH)/Sys/Platform/Posix/RWLock_posix.cpp \
	$(SRCPATH)/Sys/Platform/Posix/Thread_posix.cpp \
	$(SRCPATH)/Util/Platform/Posix/StackTrace_posix.cpp \
	$(SRCPATH)/Util/Platform/Posix/Time_posix.cpp \
	$(SRCPATH)/Util/Platform/Posix/Timer_posix.cpp \
	$(SRCPATH)/Util/MemPool.cpp \
	$(SRCPATH)/Util/RCMemBuffer.cpp \
	$(SRCPATH)/Util/RCMemBufferMgr.cpp \
	$(SRCPATH)/Util/RndGen.cpp \
	$(SRCPATH)/Component.cpp \
	$(SRCPATH)/IObject.cpp \
	$(SRCPATH)/Mem.cpp \
	$(SRCPATH)/Status.cpp \
	$(SRCPATH)/Var.cpp \
	$(SRCPATH)/Version.cpp
	mkdir -p $(TMPPATH)
	mkdir -p $(OUTPATH)
	$(CXX) $(MYFLAGS) -w -c $(CNTPATH)/CRC32Slice8/Src/crc32_internal.cpp -o $(TMPPATH)/crc32_internal.o
	$(CXX) $(MYFLAGS) -w -c $(CNTPATH)/DoubleConversion/Src/bignum.cc -o $(TMPPATH)/bignum.o
	$(CXX) $(MYFLAGS) -w -c $(CNTPATH)/DoubleConversion/Src/bignum-dtoa.cc -o $(TMPPATH)/bignum-dtoa.o
	$(CXX) $(MYFLAGS) -w -c $(CNTPATH)/DoubleConversion/Src/cached-powers.cc -o $(TMPPATH)/cached-powers.o
	$(CXX) $(MYFLAGS) -w -c $(CNTPATH)/DoubleConversion/Src/diy-fp.cc -o $(TMPPATH)/diy-fp.o
	$(CXX) $(MYFLAGS) -w -c $(CNTPATH)/DoubleConversion/Src/double-conversion.cc -o $(TMPPATH)/double-conversion.o
	$(CXX) $(MYFLAGS) -w -c $(CNTPATH)/DoubleConversion/Src/fast-dtoa.cc -o $(TMPPATH)/fast-dtoa.o
	$(CXX) $(MYFLAGS) -w -c $(CNTPATH)/DoubleConversion/Src/fixed-dtoa.cc -o $(TMPPATH)/fixed-dtoa.o
	$(CXX) $(MYFLAGS) -w -c $(CNTPATH)/DoubleConversion/Src/strtod.cc -o $(TMPPATH)/strtod.o
	$(CXX) $(MYFLAGS) -w -c $(CNTPATH)/ForestDB/Src/utils/adler32.cc -o $(TMPPATH)/fdb_adler32.o
	$(CXX) $(MYFLAGS) -w -c $(CNTPATH)/ForestDB/Src/utils/crc32.cc -o $(TMPPATH)/fdb_crc32.o
	$(CXX) $(MYFLAGS) -w -c $(CNTPATH)/ForestDB/Src/utils/debug.cc -o $(TMPPATH)/fdb_debug.o
	$(CXX) $(MYFLAGS) -w -c $(CNTPATH)/ForestDB/Src/utils/iniparser.cc -o $(TMPPATH)/fdb_iniparser.o
	$(CXX) $(MYFLAGS) -w -c $(CNTPATH)/ForestDB/Src/utils/memleak.cc -o $(TMPPATH)/fdb_memleak.o
	$(CXX) $(MYFLAGS) -w -c $(CNTPATH)/ForestDB/Src/utils/partiallock.cc -o $(TMPPATH)/fdb_partiallock.o
	$(CXX) $(MYFLAGS) -w -c $(CNTPATH)/ForestDB/Src/utils/time_utils.cc -o $(TMPPATH)/fdb_time_utils.o
	$(CXX) $(MYFLAGS) -w -c $(CNTPATH)/ForestDB/Src/src/api_wrapper.cc -o $(TMPPATH)/fdb_api_wrapper.o
	$(CXX) $(MYFLAGS) -w -c $(CNTPATH)/ForestDB/Src/src/atomic.cc -o $(TMPPATH)/fdb_atomic.o
	$(CXX) $(MYFLAGS) -w -c $(CNTPATH)/ForestDB/Src/src/avltree.cc -o $(TMPPATH)/fdb_avltree.o
	$(CXX) $(MYFLAGS) -w -c $(CNTPATH)/ForestDB/Src/src/blockcache.cc -o $(TMPPATH)/fdb_blockcache.o
	$(CXX) $(MYFLAGS) -w -c $(CNTPATH)/ForestDB/Src/src/btree.cc -o $(TMPPATH)/fdb_btree.o
	$(CXX) $(MYFLAGS) -w -c $(CNTPATH)/ForestDB/Src/src/btree_fast_str_kv.cc -o $(TMPPATH)/fdb_btree_fast_str_kv.o
	$(CXX) $(MYFLAGS) -w -c $(CNTPATH)/ForestDB/Src/src/btree_kv.cc -o $(TMPPATH)/fdb_btree_kv.o
	$(CXX) $(MYFLAGS) -w -c $(CNTPATH)/ForestDB/Src/src/btree_str_kv.cc -o $(TMPPATH)/fdb_btree_str_kv.o
	$(CXX) $(MYFLAGS) -w -c $(CNTPATH)/ForestDB/Src/src/btreeblock.cc -o $(TMPPATH)/fdb_btreeblock.o
	$(CXX) $(MYFLAGS) -w -c $(CNTPATH)/ForestDB/Src/src/compactor.cc -o $(TMPPATH)/fdb_compactor.o
	$(CXX) $(MYFLAGS) -w -c $(CNTPATH)/ForestDB/Src/src/configuration.cc -o $(TMPPATH)/fdb_configuration.o
	$(CXX) $(MYFLAGS) -w -c $(CNTPATH)/ForestDB/Src/src/docio.cc -o $(TMPPATH)/fdb_docio.o
	$(CXX) $(MYFLAGS) -w -c $(CNTPATH)/ForestDB/Src/src/fdb_errors.cc -o $(TMPPATH)/fdb_fdb_errors.o
	$(CXX) $(MYFLAGS) -w -c $(CNTPATH)/ForestDB/Src/src/filemgr.cc -o $(TMPPATH)/fdb_filemgr.o
	$(CXX) $(MYFLAGS) -w -c $(CNTPATH)/ForestDB/Src/src/filemgr_ops.cc -o $(TMPPATH)/fdb_filemgr_ops.o
	$(CXX) $(MYFLAGS) -w -c $(CNTPATH)/ForestDB/Src/src/filemgr_ops_linux.cc -o $(TMPPATH)/fdb_filemgr_ops_linux.o
	$(CXX) $(MYFLAGS) -w -c $(CNTPATH)/ForestDB/Src/src/forestdb.cc -o $(TMPPATH)/fdb_forestdb.o
	$(CXX) $(MYFLAGS) -w -c $(CNTPATH)/ForestDB/Src/src/hash.cc -o $(TMPPATH)/fdb_hash.o
	$(CXX) $(MYFLAGS) -w -c $(CNTPATH)/ForestDB/Src/src/hash_functions.cc -o $(TMPPATH)/fdb_hash_functions.o
	$(CXX) $(MYFLAGS) -w -c $(CNTPATH)/ForestDB/Src/src/hbtrie.cc -o $(TMPPATH)/fdb_hbtrie.o
	$(CXX) $(MYFLAGS) -w -c $(CNTPATH)/ForestDB/Src/src/iterator.cc -o $(TMPPATH)/fdb_iterator.o
	$(CXX) $(MYFLAGS) -w -c $(CNTPATH)/ForestDB/Src/src/kv_instance.cc -o $(TMPPATH)/fdb_kv_instance.o
	$(CXX) $(MYFLAGS) -w -c $(CNTPATH)/ForestDB/Src/src/list.cc -o $(TMPPATH)/fdb_list.o
	$(CXX) $(MYFLAGS) -w -c $(CNTPATH)/ForestDB/Src/src/snapshot.cc -o $(TMPPATH)/fdb_snapshot.o
	$(CXX) $(MYFLAGS) -w -c $(CNTPATH)/ForestDB/Src/src/transaction.cc -o $(TMPPATH)/fdb_transaction.o
	$(CXX) $(MYFLAGS) -w -c $(CNTPATH)/ForestDB/Src/src/wal.cc -o $(TMPPATH)/fdb_wal.o
	$(CC) $(MYFLAGS) -w -c $(CNTPATH)/MD5/Src/md5.c -o $(TMPPATH)/md5.o
	$(CC) $(MYFLAGS) -w -c $(CNTPATH)/Mintomic/Src/mintomic_gcc.c -o $(TMPPATH)/mintomic_gcc.o
	$(CC) $(MYFLAGS) -w -c $(CNTPATH)/SHA/Src/hmac_sha1.c -o $(TMPPATH)/hmac_sha1.o
	$(CC) $(MYFLAGS) -w -c $(CNTPATH)/SHA/Src/sha1.c -o $(TMPPATH)/sha1.o
	$(CC) $(MYFLAGS) -w -c $(CNTPATH)/SHA/Src/sha2.c -o $(TMPPATH)/sha2.o
	$(CC) $(MYFLAGS) -std=c99 -w -c $(CNTPATH)/TinyMt/Src/tinymt32.c -o $(TMPPATH)/tinymt32.o
	$(CC) $(MYFLAGS) -std=c99 -w -c $(CNTPATH)/TinyMt/Src/tinymt64.c -o $(TMPPATH)/tinymt64.o
	$(CC) $(MYFLAGS) -w -c $(CNTPATH)/UTF8ToUCS4/Src/utf8.c -o $(TMPPATH)/utf8.o
	$(CC) $(MYFLAGS) -w -c $(CNTPATH)/xxHash/Src/xxHash.c -o $(TMPPATH)/xxHash.o
	$(CC) $(MYFLAGS) -w -c $(CNTPATH)/Z85/Src/z85.c -o $(TMPPATH)/z85.o
	$(CC) $(MYFLAGS) -c $(SRCPATH)/C/Platform/Posix/Alloc_posix.c -o $(TMPPATH)/Alloc_posix.o
	$(CXX) $(MYFLAGS) -c $(SRCPATH)/Data/JSON/JSONSAXParser.cpp -o $(TMPPATH)/JSONSAXParser.o
	$(CXX) $(MYFLAGS) -c $(SRCPATH)/Detail/DetailPrint/Platform/Posix/DetailPrint_posix.cpp -o $(TMPPATH)/DetailPrint_posix.o
	$(CXX) $(MYFLAGS) -c $(SRCPATH)/Hash/CRC32Hash.cpp -o $(TMPPATH)/CRC32Hash.o
	$(CXX) $(MYFLAGS) -c $(SRCPATH)/Hash/MD5Hash.cpp -o $(TMPPATH)/MD5Hash.o
	$(CXX) $(MYFLAGS) -c $(SRCPATH)/Hash/SHA1Hash.cpp -o $(TMPPATH)/SHA1Hash.o
	$(CXX) $(MYFLAGS) -c $(SRCPATH)/Hash/SHA256Hash.cpp -o $(TMPPATH)/SHA256Hash.o
	$(CXX) $(MYFLAGS) -c $(SRCPATH)/Hash/SHA384Hash.cpp -o $(TMPPATH)/SHA384Hash.o
	$(CXX) $(MYFLAGS) -c $(SRCPATH)/Hash/SHA512Hash.cpp -o $(TMPPATH)/SHA512Hash.o
	$(CXX) $(MYFLAGS) -c $(SRCPATH)/Hash/xxHash32.cpp -o $(TMPPATH)/xxHash32.o
	$(CXX) $(MYFLAGS) -c $(SRCPATH)/Hash/xxHash64.cpp -o $(TMPPATH)/xxHash64.o
	$(CXX) $(MYFLAGS) -c $(SRCPATH)/IO/Platform/Posix/LocalFileSysHelper_posix.cpp -o $(TMPPATH)/LocalFileSysHelper_posix.o
	$(CXX) $(MYFLAGS) -c $(SRCPATH)/IO/Platform/Posix/FileInputStream_posix.cpp -o $(TMPPATH)/FileInputStream.o
	$(CXX) $(MYFLAGS) -c $(SRCPATH)/IO/Platform/Posix/FileOutputStream_posix.cpp -o $(TMPPATH)/FileOutputStream.o
	$(CXX) $(MYFLAGS) -c $(SRCPATH)/IO/Helper.cpp -o $(TMPPATH)/Helper.o
	$(CXX) $(MYFLAGS) -c $(SRCPATH)/IO/MemInputStream.cpp -o $(TMPPATH)/MemInputStream.o
	$(CXX) $(MYFLAGS) -c $(SRCPATH)/IO/MemOutputStream.cpp -o $(TMPPATH)/MemOutputStream.o
	$(CXX) $(MYFLAGS) -c $(SRCPATH)/KVDB/FDBDatabase.cpp -o $(TMPPATH)/FDBDatabase.o
	$(CXX) $(MYFLAGS) -c $(SRCPATH)/KVDB/FDBIterator.cpp -o $(TMPPATH)/FDBIterator.o
	$(CXX) $(MYFLAGS) -c $(SRCPATH)/KVDB/FDBRecord.cpp -o $(TMPPATH)/FDBRecord.o
	$(CXX) $(MYFLAGS) -c $(SRCPATH)/KVDB/FDBTable.cpp -o $(TMPPATH)/FDBTable.o
	$(CXX) $(MYFLAGS) -c $(SRCPATH)/KVDB/MemDatabase.cpp -o $(TMPPATH)/MemDatabase.o
	$(CXX) $(MYFLAGS) -c $(SRCPATH)/KVDB/MemIterator.cpp -o $(TMPPATH)/MemIterator.o
	$(CXX) $(MYFLAGS) -c $(SRCPATH)/KVDB/MemRecord.cpp -o $(TMPPATH)/MemRecord.o
	$(CXX) $(MYFLAGS) -c $(SRCPATH)/KVDB/MemTable.cpp -o $(TMPPATH)/MemTable.o
	$(CXX) $(MYFLAGS) -c $(SRCPATH)/Log/Platform/Posix/SystemOutput_posix.cpp -o $(TMPPATH)/SystemOutput_posix.o
	$(CXX) $(MYFLAGS) -c $(SRCPATH)/Log/ConsoleOutput.cpp -o $(TMPPATH)/ConsoleOutput.o
	$(CXX) $(MYFLAGS) -c $(SRCPATH)/Log/DefaultFormatter.cpp -o $(TMPPATH)/DefaultFormatter.o
	$(CXX) $(MYFLAGS) -c $(SRCPATH)/Log/Logger.cpp -o $(TMPPATH)/Logger.o
	$(CXX) $(MYFLAGS) -c $(SRCPATH)/Log/StreamOutput.cpp -o $(TMPPATH)/StreamOutput.o
	$(CXX) $(MYFLAGS) -c $(SRCPATH)/SB/BinaryDataReader.cpp -o $(TMPPATH)/BinaryDataReader.o
	$(CXX) $(MYFLAGS) -c $(SRCPATH)/SB/BinaryDataWriter.cpp -o $(TMPPATH)/BinaryDataWriter.o
	$(CXX) $(MYFLAGS) -c $(SRCPATH)/SB/Parser.cpp -o $(TMPPATH)/StreamOutput.o
	$(CXX) $(MYFLAGS) -c $(SRCPATH)/SB/StatsDataReader.cpp -o $(TMPPATH)/StatsDataReader.o
	$(CXX) $(MYFLAGS) -c $(SRCPATH)/SB/StatsDataWriter.cpp -o $(TMPPATH)/StatsDataWriter.o
	$(CXX) $(MYFLAGS) -c $(SRCPATH)/SB/TextDataReader.cpp -o $(TMPPATH)/TextDataReader.o
	$(CXX) $(MYFLAGS) -c $(SRCPATH)/SB/TextDataWriter.cpp -o $(TMPPATH)/TextDataWriter.o
	$(CXX) $(MYFLAGS) -c $(SRCPATH)/Str/Platform/Posix/UTF8_posix.cpp -o $(TMPPATH)/UTF8_posix.o
	$(CXX) $(MYFLAGS) -c $(SRCPATH)/Str/HexBinStr.cpp -o $(TMPPATH)/HexBinStr.o
	$(CXX) $(MYFLAGS) -c $(SRCPATH)/Str/Z85BinStr.cpp -o $(TMPPATH)/Z85BinStr.o
	$(CXX) $(MYFLAGS) -c $(SRCPATH)/Sys/Platform/Posix/DynLib_posix.cpp -o $(TMPPATH)/DynLib_posix.o
	$(CXX) $(MYFLAGS) -c $(SRCPATH)/Sys/Platform/Posix/Lock_posix.cpp -o $(TMPPATH)/Lock_posix.o
	$(CXX) $(MYFLAGS) -c $(SRCPATH)/Sys/Platform/Posix/Process_posix.cpp -o $(TMPPATH)/Process_posix.o
	$(CXX) $(MYFLAGS) -c $(SRCPATH)/Sys/Platform/Posix/RWLock_posix.cpp -o $(TMPPATH)/RWLock_posix.o
	$(CXX) $(MYFLAGS) -c $(SRCPATH)/Sys/Platform/Posix/Thread_posix.cpp -o $(TMPPATH)/Thread_posix.o
	$(CXX) $(MYFLAGS) -c $(SRCPATH)/Util/Platform/Posix/StackTrace_posix.cpp -o $(TMPPATH)/StackTrace_posix.o
	$(CXX) $(MYFLAGS) -c $(SRCPATH)/Util/Platform/Posix/Time_posix.cpp -o $(TMPPATH)/Time_posix.o
	$(CXX) $(MYFLAGS) -c $(SRCPATH)/Util/Platform/Posix/Timer_posix.cpp -o $(TMPPATH)/Timer_posix.o
	$(CXX) $(MYFLAGS) -c $(SRCPATH)/Util/MemPool.cpp -o $(TMPPATH)/MemPool.o
	$(CXX) $(MYFLAGS) -c $(SRCPATH)/Util/RCMemBuffer.cpp -o $(TMPPATH)/RCMemBuffer.o
	$(CXX) $(MYFLAGS) -c $(SRCPATH)/Util/RCMemBufferMgr.cpp -o $(TMPPATH)/RCMemBufferMgr.o
	$(CXX) $(MYFLAGS) -c $(SRCPATH)/Util/RndGen.cpp	-o $(TMPPATH)/RndGen.o
	$(CXX) $(MYFLAGS) -c $(SRCPATH)/Component.cpp -o $(TMPPATH)/Component.o
	$(CXX) $(MYFLAGS) -c $(SRCPATH)/IObject.cpp -o $(TMPPATH)/IObject.o
	$(CXX) $(MYFLAGS) -c $(SRCPATH)/Mem.cpp -o $(TMPPATH)/Mem.o
	$(CXX) $(MYFLAGS) -c $(SRCPATH)/Status.cpp -o $(TMPPATH)/Status.o
	$(CXX) $(MYFLAGS) -c $(SRCPATH)/Var.cpp -o $(TMPPATH)/Var.o
	$(CXX) $(MYFLAGS) -c $(SRCPATH)/Version.cpp -o $(TMPPATH)/Version.o
	ar rcs $(OUTPATH)/$(LIBNAME) $(TMPPATH)/*.o

clean:
	rm -f -r $(TMPPATH) $(OUTPATH)
