/* 
 * CX - C++ framework for general purpose development
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


#if defined(CX_OS_POSIX)


#include <stdio.h>
#include <wchar.h>
#include <sys/types.h>
#include <sys/stat.h>


//http://en.wikipedia.org/wiki/C_file_input/output


//File access
#define cx_fopen           fopen
#define cx_freopen         freopen
#define cx_fflush          fflush
#define cx_fclose          fclose
#define cx_setbuf          setbuf
#define cx_setvbuf         setvbuf
#define cx_fwide           fwide


//Direct input / output
#define cx_fread           fread
#define cx_fwrite          fwrite


//Unformatted input / output
#define cx_fgetc           fgetc
#define cxw_fgetc          fgetwc
#define cx_getc            getc
#define cxw_getc           getwc
#define cx_fgets           fgets
#define cxw_fgets          fgetws
#define cx_fputc           fputc
#define cxw_fputc          fputwc
#define cx_putc            putc
#define cxw_putc           putwc
#define cx_fputs           fputs
#define cxw_fputs          fputws
#define cx_getchar         getchar
#define cxw_getchar        getwchar
#define cx_gets            gets
//#define cxw_gets NOT AVAILABLE
#define cx_putchar         putchar
#define cxw_putchar        putwchar
#define cx_puts            puts
//#define cxw_puts NOT AVAILABLE
#define cx_ungetc          ungetc
#define cxw_ungetc         ungetwc

#ifdef CX_WCHAR_IS_DEFAULT
#define cxt_fgetc          cxw_fgetc
#define cxt_getc           cxw_getc
#define cxt_fgets          cxw_fgets
#define cxt_fputc          cxw_fputc
#define cxt_putc           cxw_putc
#define cxt_fputs          cxw_fputs
#define cxt_getchar        cxw_getchar
#define cxt_putchar        cxw_putchar
#define cxt_ungetc         cxw_ungetc
#else
#define cxt_fgetc          cx_fgetc
#define cxt_getc           cx_getc
#define cxt_fgets          cx_fgets
#define cxt_fputc          cx_fputc
#define cxt_putc           cx_putc
#define cxt_fputs          cx_fputs
#define cxt_getchar        cx_getchar
#define cxt_putchar        cx_putchar
#define cxt_ungetc         cx_ungetc
#endif


//Formatted input / output
#define cx_scanf           scanf
#define cxw_scanf          wscanf
#define cx_fscanf          fscanf
#define cxw_fscanf         fwscanf
#define cx_sscanf          sscanf
#define cxw_sscanf         swscanf
#define cx_vscanf          vscanf
#define cxw_vscanf         vwscanf
#define cx_vfscanf         vfscanf
#define cxw_vfscanf        vfwscanf
#define cx_vsscanf         vsscanf
#define cxw_vsscanf        vswscanf
#define cx_printf          printf
#define cxw_printf         wprintf
#define cx_fprintf         fprintf
#define cxw_fprintf        fwprintf
#define cx_sprintf         sprintf
#define cxw_sprintf        swprintf
#define cx_snprintf        snprintf
//#define cxw_snprintf NOT AVAILABLE
#define cx_vprintf         vprintf
#define cxw_vprintf        vwprintf
#define cx_vfprintf        vfprintf
#define cxw_vfprintf       vfwprintf
#define cx_vsprintf        vsprintf
#define cxw_vsprintf       vswprintf
#define cx_vsnprintf       vsnprintf
//#define cxw_vsnprintf NOT AVAILABLE

#ifdef CX_WCHAR_IS_DEFAULT
#define cxt_scanf          cxw_scanf
#define cxt_fscanf         cxw_fscanf
#define cxt_sscanf         cxw_sscanf
#define cxt_vscanf         cxw_vscanf
#define cxt_vfscanf        cxw_vfscanf
#define cxt_vsscanf        cxw_vsscanf
#define cxt_printf         cxw_printf
#define cxt_fprintf        cxw_fprintf
#define cxt_sprintf        cxw_sprintf
#define cxt_vprintf        cxw_vprintf
#define cxt_vfprintf       cxw_vfprintf
#define cxt_vsprintf       cxw_vsprintf
#else
#define cxt_scanf          cx_scanf
#define cxt_fscanf         cx_fscanf
#define cxt_sscanf         cx_sscanf
#define cxt_vscanf         cx_vscanf
#define cxt_vfscanf        cx_vfscanf
#define cxt_vsscanf        cx_vsscanf
#define cxt_printf         cx_printf
#define cxt_fprintf        cx_fprintf
#define cxt_sprintf        cx_sprintf
#define cxt_vprintf        cx_vprintf
#define cxt_vfprintf       cx_vfprintf
#define cxt_vsprintf       cx_vsprintf
#endif

//File positioning
#define cx_ftell           ftell
#define cx_fgetpos         fgetpos
#define cx_fseek           fseek
#define cx_fsetpos         fsetpos
#define cx_rewind          rewind


//Error handling
#define cx_clearerr        clearerr
#define cx_feof            feof
#define cx_ferror          ferror


//Operations on files
#define cx_rename          rename
#define cx_tmpfile         tmpfile
#define cx_tmpnam          tmpnam


//others
#define cx_fileno          fileno
#define cx_stat64struct    struct stat64
#define cx_stat64          fstat64
#define cx_fseek64         fseeko64
#define cx_ftell64         ftello64


#endif

