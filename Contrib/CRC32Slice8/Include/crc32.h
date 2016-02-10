// //////////////////////////////////////////////////////////
// Crc32.cpp
// Copyright (c) 2011-2013 Stephan Brumme. All rights reserved.
// see http://create.stephan-brumme.com/disclaimer.html
// 

#pragma once


#include <stdint.h>
#include <stdlib.h>


uint32_t crc32_8bytes_internal(const void* data, size_t length, uint32_t previousCrc32 = 0);

