this is the reference source code package of BLAKE2, which includes

ref/ 
C implementations of blake2b, blake2bp, blake2s, blake2sp, aimed at
portability and simplicity
copyright (c) 2013-2014 Samuel Neves

sse/
C implementations of blake2b, blake2bp, blake2s, blake2sp, optimized 
for speed on CPUs supporting SSE2, SSSE3, SSE4.1, AVX, or XOP
copyright (c) 2013-2014 Samuel Neves

csharp/
C# implementation of blake2b
copyright (c) 2013 Christian Winnerlein

b2sum/
command line tool to hash files, based on the sse/ implementations
copyright (c) 2013-2014 Samuel Neves

bench/
benchmark tool to measure cycles-per-byte speeds and produce graphs
copyright (c) 2013-2014 Samuel Neves