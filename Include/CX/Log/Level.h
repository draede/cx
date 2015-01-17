
#pragma once


#include "CX/Types.h"


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

