#pragma once

#include <Runtime/Core/Public/Misc/CoreMisc.h>

#ifdef _MSC_VER
    #define __FUNC_NAME__ __FUNCSIG__
#else
    #define __FUNC_NAME__ __PRETTY_FUNCTION__
#endif

#define PURE_VIRTUAL_METHOD PURE_VIRTUAL(__FUNC_NAME__)
