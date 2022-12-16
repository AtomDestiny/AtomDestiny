#pragma once

#ifndef __cpp_lib_coroutine
    #include <experimental/coroutine>

    namespace std
    {
        template<typename T>
        using coroutine_handle = std::experimental::coroutine_handle<T>;

        template<typename... Args>
        using coroutine_traits = std::experimental::coroutine_traits<Args...>;
        
        using suspend_never = std::experimental::suspend_never;
        using suspend_always = std::experimental::suspend_always;
    }

    namespace coro = std::experimental;
#else
    #include <coroutine>

    namespace coro = std;
#endif
