#pragma once

#include <Runtime/Core/Public/Misc/CoreMisc.h>

#ifndef __FUNCTION_NAME__
    #if defined __func__
        // Undeclared
        #define __FUNCTION_NAME__   __func__
    #elif defined __FUNCTION__ 
        // Undeclared
        #define __FUNCTION_NAME__   __FUNCTION__
    #elif defined __PRETTY_FUNCTION__
        // Undeclared
        #define __FUNCTION_NAME__   __PRETTY_FUNCTION__
    #else
        // Declared
        #define __FUNCTION_NAME__   "N/A"
    #endif // __func__
#endif // __FUNCTION_NAME__

#define PURE_VIRTUAL_METHOD PURE_VIRTUAL(__FUNCTION_NAME__)
