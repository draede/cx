LZHAM is a lossless (LZ based) data compression codec optimized for particularly fast decompression at very high compression 
ratios with a zlib compatible API. It's been developed over a period of 3 years and alpha versions have already shipped in 
many products. (The alpha is here: https://code.google.com/p/lzham/) LZHAM's decompressor is slower than zlib's, but generally 
much faster than LZMA's, with a compression ratio that is typically within a few percent of LZMA's and sometimes better.