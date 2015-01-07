
#pragma once


#include "CX/Platform.h"


#if defined(CX_OS_WINDOWS)


#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>


#define cx_fopen          fopen
#define cx_freopen        freopen
#define cx_fdopen         fdopen
#define cx_fclose         fclose
#define cx_remove         remove
#define cx_rename         rename
#define cx_fgetc          fgetc
#define cx_fgets          fgets
#define cx_fputc          fputc
#define cx_fputs          fputs
#define cx_getc           getc
#define cx_gets           gets
#define cx_putc           putc
#define cx_puts           puts
#define cx_printf         printf
#define cx_fprintf        fprintf
#define cx_vprintf        vprintf
#define cx_vfprintf       vfprintf
#define cx_sprintf        sprintf
#define cx_snprintf       _snprintf
#define cx_vsnprintf      vsnprintf
#define cx_vsprintf       vsprintf
#define cx_scanf          scanf
#define cx_fscanf         fscanf
#define cx_sscanf         sscanf
#define cx_perror         perror
#define cx_vscprintf      _vscprintf
#define cx_unlink         _unlink

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


#endif

