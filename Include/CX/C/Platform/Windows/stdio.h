/* 
 * CX - C++ framework for general purpose developement
 *
 * https://github.com/draede/cx
 * 
 * Copyright (C) 2014-2015 draede - draede [at] outlook [dot] com
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


#include "CX/C/Platform.h"


#if defined(CX_OS_WINDOWS)


#include <stdio.h>
#include <wchar.h>
#include <sys/types.h>
#include <sys/stat.h>


#define cx_fopen                    fopen
#define cx_freopen                  freopen
#define cx_fdopen                   fdopen
#define cx_remove                   remove
#define cx_rename                   rename
#define cx_fgetc                    fgetc
#define cx_fgets                    fgets
#define cx_fputc                    fputc
#define cx_fputs                    fputs
#define cx_getc                     getc
#define cx_gets                     gets
#define cx_putc                     putc
#define cx_puts                     puts
#define cx_printf                   printf
#define cx_fprintf                  fprintf
#define cx_vprintf                  vprintf
#define cx_vfprintf                 vfprintf
#define cx_sprintf                  sprintf
#define cx_snprintf                 _snprintf
#define cx_vsnprintf                vsnprintf
#define cx_vsprintf                 vsprintf
#define cx_scanf                    scanf
#define cx_fscanf                   fscanf
#define cx_sscanf                   sscanf
#define cx_perror                   perror
#define cx_vscprintf                _vscprintf
#define cx_unlink                   _unlink

#define cxw_fopen                   _wfopen
#define cxw_freopen                 _wfreopen
#define cxw_fdopen                  _wfdopen
#define cxw_remove                  _wremove
#define cxw_rename                  _wrename
#define cxw_fgetc                   fgetwc
#define cxw_fgets                   fgetws
#define cxw_fputc                   fputwc
#define cxw_fputs                   fputws
#define cxw_getc                    getwc
#define cxw_gets                    getwc
#define cxw_putc                    putwc
#define cxw_puts                    putws
#define cxw_printf                  wprintf
#define cxw_fprintf                 fwprintf
#define cxw_vprintf                 vwprintf
#define cxw_vfprintf                vfwprintf
#define cxw_sprintf                 swprintf
#define cxw_snprintf                _snwprintf
#define cxw_vsnprintf               _vsnwprintf
#define cxw_vsprintf                vswprintf
#define cxw_scanf                   wscanf
#define cxw_fscanf                  fwscanf
#define cxw_sscanf                  swscanf
#define cxw_perror                  _wperror
#define cxw_vscprintf               _vscwprintf
#define cxw_unlink                 _wunlink

#ifndef CX_UNICODE
#define cxt_fopen                   cx_fopen
#define cxt_freopen                 cx_freopen
#define cxt_fdopen                  cx_fdopen
#define cxt_remove                  cx_remove
#define cxt_rename                  cx_rename
#define cxt_fgetc                   cx_fgetc
#define cxt_fgets                   cx_fgets
#define cxt_fputc                   cx_fputc
#define cxt_fputs                   cx_fputs
#define cxt_getc                    cx_getc
#define cxt_gets                    cx_gets
#define cxt_putc                    cx_putc
#define cxt_puts                    cx_puts
#define cxt_printf                  cx_printf
#define cxt_fprintf                 cx_fprintf
#define cxt_vprintf                 cx_vprintf
#define cxt_vfprintf                cx_vfprintf
#define cxt_sprintf                 cx_sprintf
#define cxt_snprintf                cx_snprintf
#define cxt_vsnprintf               cx_vsnprintf
#define cxt_vsprintf                cx_vsprintf
#define cxt_scanf                   cx_scanf
#define cxt_fscanf                  cx_fscanf
#define cxt_sscanf                  cx_sscanf
#define cxt_perror                  cx_perror
#define cxt_vscprintf               cx_vscprintf
#define cxt_unlink                  cx_unlink
#else
#define cxt_fopen                   cxw_fopen
#define cxt_freopen                 cxw_freopen
#define cxt_fdopen                  cxw_fdopen
#define cxt_remove                  cxw_remove
#define cxt_rename                  cxw_rename
#define cxt_fgetc                   cxw_fgetc
#define cxt_fgets                   cxw_fgets
#define cxt_fputc                   cxw_fputc
#define cxt_fputs                   cxw_fputs
#define cxt_getc                    cxw_getc
#define cxt_gets                    cxw_gets
#define cxt_putc                    cxw_putc
#define cxt_puts                    cxw_puts
#define cxt_printf                  cxw_printf
#define cxt_fprintf                 cxw_fprintf
#define cxt_vprintf                 cxw_vprintf
#define cxt_vfprintf                cxw_vfprintf
#define cxt_sprintf                 cxw_sprintf
#define cxt_snprintf                cxw_snprintf
#define cxt_vsnprintf               cxw_vsnprintf
#define cxt_vsprintf                cxw_vsprintf
#define cxt_scanf                   cxw_scanf
#define cxt_fscanf                  cxw_fscanf
#define cxt_sscanf                  cxw_sscanf
#define cxt_perror                  cxw_perror
#define cxt_vscprintf               cxw_vscprintf
#define cxt_unlink                  cxw_unlink
#endif

#define cx_statstruct     struct _stat64
#define cx_fstat          _fstati64
#define cx_fileno         _fileno

#define cx_fseek           _fseeki64

#define cx_ftell           _ftelli64

#define CX_SEEK_SET        SEEK_SET
#define CX_SEEK_END        SEEK_END
#define CX_SEEK_CUR        SEEK_CUR

#define CX_FILE_RFLAGS     "rb"
#define CX_FILE_WFLAGS     "wb"
#define CX_FILE_AFLAGS     "ab"

#define CX_WFILE_RFLAGS    L"rb"
#define CX_WFILE_WFLAGS    L"wb"
#define CX_WFILE_AFLAGS    L"ab"


#endif

