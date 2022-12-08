#pragma once

#include <Runtime/Core/Public/Misc/CoreMisc.h>

#ifdef _MSC_VER
    #define __FUNC_NAME__ __FUNCSIG__
#else
    #define __FUNC_NAME__ __PRETTY_FUNCTION__
#endif

namespace AtomDestiny
{
    constexpr bool IsReturnTypeVoid(const std::string_view function)
    {
        return function.find_first_of("void") != std::string_view::npos;
    }
    
} // namespace AtomDestiny

#define GENERATE_RETURN(name, func_name) { \
    constexpr auto res##name = AtomDestiny::IsReturnTypeVoid(func_name); \
    if constexpr (res##name) return {}; \
}

#define PURE_VIRTUAL_METHOD { \
    PURE_VIRTUAL(__FUNC_NAME__) \
}

#define PURE_VIRTUAL_RETURN_METHOD { \
    PURE_VIRTUAL(__FUNC_NAME__, return {};) \
}
