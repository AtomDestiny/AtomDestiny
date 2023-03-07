#pragma once

#include <type_traits>

#include <AtomDestiny/Core/Concepts.h>
#include <AtomDestiny/Core/ObjectPool/ActorPool.h>

namespace AtomDestiny::Utils
{
    
    ///
    /// Increments Enum value, even it is 'enum class'.
    /// It would check at compile-time clamp value.
    /// By default 'None' value of enum should be a final value.
    ///
    template <typename Enum>
    Enum IncrementEnum(const Enum e)
    {
        using IntegerType = std::underlying_type_t<Enum>;
        
        const auto res = static_cast<Enum>(static_cast<IntegerType>(e) + 1);

        if constexpr (Concepts::HasNoneValue<Enum>)
        {
            const auto converted = static_cast<IntegerType>(res);
            return static_cast<Enum>(std::clamp<IntegerType>(converted, 0, static_cast<IntegerType>(Enum::None)));
        }
        
        return res;
    }
    
} // namespace AtomDestiny

#define NOT_NULLPTR_CALL(value, methodName, ...) \
    static_assert(std::is_pointer_v<decltype(value)>, "Type should be a pointer"); \
    if (value != nullptr) { \
        value->methodName(__VA_ARGS__); \
    } \
