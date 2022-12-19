#pragma once

#include "CoreMinimal.h"

#ifdef PLATFORM_MAC
    #include <experimental/coroutine>
    namespace coro = std::experimental;
#else
    #include <coroutine>
    namespace coro = std;
#endif // PLATFORM MAC
