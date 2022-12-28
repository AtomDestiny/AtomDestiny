#pragma once

#include <type_traits>

#include <AtomDestiny/Core/ObjectPool/ActorPool.h>

namespace AtomDestiny::Utils
{
    // Preloads blueprint (Prefab) to object pool from component
    inline void PreloadBlueprint(const TWeakObjectPtr<AActor>& blueprint, const uint32_t count)
    {
        if (blueprint.IsValid())
        {
            ObjectPool::Instance().Preload(blueprint, count);
        }
    }
    
} // namespace AtomDestiny::Utils

#define NOT_NULLPTR_CALL(value, methodName, ...) \
    static_assert(std::is_pointer_v<decltype(value)>, "Type should be a pointer"); \
    if (value != nullptr) { \
        value->methodName(__VA_ARGS__); \
    } \
