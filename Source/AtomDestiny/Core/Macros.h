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

#if CHECK_PUREVIRTUALS
    #define PURE_VIRTUAL_METHOD =0;
#else
    #define PURE_VIRTUAL_METHOD { \
        LowLevelFatalError(TEXT("Pure virtual not implemented (%s)"), TEXT(__FUNCTION_NAME__)); \
    }
#endif
