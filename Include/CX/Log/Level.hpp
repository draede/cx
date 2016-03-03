/* 
 * CX - C++ framework for general purpose development
 *
 * https://github.com/draede/cx
 * 
 * Copyright (C) 2014-2016 draede, draede [at] outlook [dot] com
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


#include "CX/Types.hpp"


#define CX_LOG_NONE      0
#define CX_LOG_ERROR     1
#define CX_LOG_WARN      2
#define CX_LOG_INFO      3
#define CX_LOG_VERB      4
#define CX_LOG_DEBUG     5


namespace CX
{

namespace Log
{

typedef UInt32   Level;

static const Level      Level_None                     = CX_LOG_NONE;
static const Level      Level_Error                    = CX_LOG_ERROR;
static const Level      Level_Warning                  = CX_LOG_WARN;
static const Level      Level_Info                     = CX_LOG_INFO;
static const Level      Level_Verbose                  = CX_LOG_VERB;
static const Level      Level_Debug                    = CX_LOG_DEBUG;

}//namespace Log

}//namespace CX

