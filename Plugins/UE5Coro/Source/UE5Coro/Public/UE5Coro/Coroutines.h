#pragma once

#include "CoreMinimal.h"

#ifdef __cpp_lib_coroutine
    #define COROUTINE
#endif // __cpp_lib_coroutine

#ifdef COROUTINE
    #include <coroutine>
    namespace coro = std;
#else
    #include <experimental/coroutine>
    namespace coro = std::experimental;
#endif // COROUTINE
