#pragma once

#include <Runtime/Core/Public/Misc/CoreMisc.h>

#ifdef _MSC_VER
    #define __FUNC_NAME__ __FUNCSIG__
#else
    #define __FUNC_NAME__ __PRETTY_FUNCTION__
#endif

constexpr bool IsVoid(const std::string_view function)
{
    return function.find_first_of("void") != std::string_view::npos;
}

#define PURE_VIRTUAL_METHOD { \
    constexpr auto res = IsVoid(__FUNC_NAME__); \
    PURE_VIRTUAL(__FUNC_NAME__) \
    if constexpr (!res) return {}; \
}
