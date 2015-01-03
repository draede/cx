
#pragma once


#include "CX/Platform.h"


#if defined(CX_OS_WINDOWS)


#include "CX/Version.h"


#define CX_RES_VER_MAJOR               CX_VER_MAJOR
#define CX_RES_VER_MINOR               CX_VER_MINOR
#define CX_RES_VER_REVISION            CX_VER_REVISION
#define CX_RES_VER_BUILD               CX_VER_BUILD

#define CX_RES_VER_PRODUCT_MAJOR       CX_VER_MAJOR
#define CX_RES_VER_PRODUCT_MINOR       CX_VER_MINOR
#define CX_RES_VER_PRODUCT_REVISION    CX_VER_REVISION
#define CX_RES_VER_PRODUCT_BUILD       CX_VER_BUILD

#define CX_RES_VER_NAME                CX_VER_NAME
#define CX_RES_VER_PRODUCT             CX_VER_NAME
#define CX_RES_VER_DESCRIPTION         CX_VER_DESCRIPTION
#define CX_RES_VER_COPYRIGHT           CX_VER_COPYRIGHT
#define CX_RES_VER_COMPANY             CX_VER_COMPANY


#define CX_RES_VER_STRBASE(x)                 #x

#define CX_RES_VER_STR(x)                    CX_RES_VER_STRBASE(x)

#define CX_RES_VER_NUMBER(a, b, c, d)        (a), (b), (c), (d)

#define CX_RES_VER_STRING(a, b, c, d)        CX_RES_VER_STR(a) "." CX_RES_VER_STR(b) "."           \
                                             CX_RES_VER_STR(c) "." CX_RES_VER_STR(d)


#endif

