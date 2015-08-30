LZ4 - Extremely fast compression

LZ4 is lossless compression algorithm, providing compression speed at 400 MB/s per core, scalable with multi-cores CPU. It also 
features an extremely fast decoder, with speed in multiple GB/s per core, typically reaching RAM speed limits on multi-core 
systems.

Speed can be tuned dynamically, selecting an "acceleration" factor which trades compression ratio for more speed up. On the 
other end, a high compression derivative, LZ4_HC, is also provided, trading CPU time for improved compression ratio. All 
versions feature the same excellent decompression speed.